#include "pch.h"
#include "renderer/Renderer.h"
#include "glad/glad.h"
#include "Light.h"
#include "geometry/GeoData.h"

Renderer::RenderData* Renderer::s_Data = nullptr;

static constexpr short DIFF_TEX_SLOT   = 0;
static constexpr short SPEC_TEX_SLOT   = 1;
static constexpr short SKYBOX_TEX_SLOT = 2;
static constexpr short DEPTH_TEX_SLOT  = 3;

void Renderer::SetUniformBuffer(const Ref<ShaderBlock> ubo, const short slot,
	std::vector<ShaderType> shTypes)
{
	ubo->Bind(slot);
	for (ShaderType type : shTypes)
	{
		auto shader = s_Data->Shader[type];
		unsigned index = glGetUniformBlockIndex(shader->Id(), ubo->Name());

		ASSERT(index != GL_INVALID_INDEX, "UBO not found");

		glUniformBlockBinding(shader->Id(), index, slot);
	}
}

void Renderer::SetShaderStorageBuffer(const Ref<ShaderBlock> ssbo, const short slot,
	std::vector<ShaderType> shTypes)
{
	ssbo->Bind(slot);
	for (ShaderType type : shTypes)
	{
		auto shader = s_Data->Shader[type];
		unsigned index = glGetProgramResourceIndex(
			shader->Id(), GL_SHADER_STORAGE_BLOCK, ssbo->Name());

		ASSERT(index != GL_INVALID_INDEX, "SSBO not found");

		glShaderStorageBlockBinding(shader->Id(), index, slot);
	}
}

void Renderer::Init(unsigned width, unsigned height)
{
	s_Data = new RenderData();
	s_Data->viewportWidth  = width;
	s_Data->viewportHeight = height;
	s_Data->DepthMap = CreateRef<Texture>(SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
	s_Data->DepthMapFBO = CreateRef<Framebuffer>();
	s_Data->DepthMapFBO->AttachDepthCubemap(s_Data->DepthMap->Id());

	LoadShaders();
	CreateSkybox();

	s_Data->SceneUBO = CreateRef<ShaderBlock>(
		"SceneData", (const void*)NULL,
		sizeof(glm::mat4) + sizeof(glm::vec3) + sizeof(unsigned),
		GL_UNIFORM_BUFFER);

	s_Data->LightsCount = 3;
	s_Data->SceneUBO->Upload(
		&s_Data->LightsCount, 4, 76);

	SetUniformBuffer(s_Data->SceneUBO, 0,
		{ ShaderType::Diffuse, ShaderType::DiffNSpec, ShaderType::Color });

	s_Data->LightSSBO = CreateRef<ShaderBlock>(
		"LightData", (const void*)NULL,
		SSBO_LIGHT_SIZE * s_Data->LightsCount,
		GL_SHADER_STORAGE_BUFFER);

	SetShaderStorageBuffer(s_Data->LightSSBO, 0,
		{ ShaderType::DiffNSpec });

	
}

void Renderer::Draw(const glm::mat4& modelMat, const Ref<VAO> vao,
	const Ref<Texture> diffuse)
{
	Ref<Shader> sh = s_Data->Shader[ShaderType::Diffuse];
	BindShader(sh);
	sh->setMat4f("u_ModelMat", modelMat);

	BindTexture(diffuse, DIFF_TEX_SLOT);
	BindVAO(vao);
	
	glDrawArrays(GL_TRIANGLES, 0, vao->Count());
}

void Renderer::Draw(const glm::mat4& modelMat, const Ref<VAO> vao,
	const Ref<Texture> diffuse, const Ref<Texture> specular)
{
	Ref<Shader> sh = s_Data->Shader[ShaderType::DiffNSpec];
	BindShader(sh);
	sh->setMat4f("u_ModelMat", modelMat);
	/*sh->setFloat3("lightPos", glm::vec3(0.0f, 5.0f, 0.0f));
	sh->setFloat("far_plane", 25.f);*/

	BindTexture(diffuse, DIFF_TEX_SLOT);
	BindTexture(specular, SPEC_TEX_SLOT);
	BindTexture(s_Data->DepthMap, DEPTH_TEX_SLOT);
	BindVAO(vao);

	glDrawArrays(GL_TRIANGLES, 0, vao->Count());
}

void Renderer::Draw(const glm::mat4& modelMat, const Ref<VAO> vao, const glm::vec4& color)
{
	Ref<Shader> sh = s_Data->Shader[ShaderType::Color];
	BindShader(sh);
	sh->setMat4f("u_ModelMat", modelMat);

	sh->setFloat4("u_Color", color);

	BindVAO(vao);
	glDrawArrays(GL_TRIANGLES, 0, vao->Count());
}


void Renderer::DrawDepth(const glm::mat4& modelMat, const Ref<VAO> vao)
{
	Ref<Shader> sh = s_Data->Shader[ShaderType::DepthShader];
	BindShader(sh);
	sh->setMat4f("u_ModelMat", modelMat);

	BindVAO(vao);

	glDrawArrays(GL_TRIANGLES, 0, vao->Count());
}

void Renderer::DrawDepthInside(const glm::mat4& modelMat, const Ref<VAO> vao)
{
	Ref<Shader> sh = s_Data->Shader[ShaderType::DepthShader];
	BindShader(sh);
	sh->setMat4f("u_ModelMat", modelMat);

	BindVAO(vao);

	glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	sh->setInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	glDrawArrays(GL_TRIANGLES, 0, vao->Count());
	sh->setInt("reverse_normals", 0); // and of course disable it
	glEnable(GL_CULL_FACE);
}

void Renderer::DrawInside(const glm::mat4& modelMat, const Ref<VAO> vao, const Ref<Texture> diffuse, const Ref<Texture> specular)
{
	Ref<Shader> sh = s_Data->Shader[ShaderType::DiffNSpec];
	BindShader(sh);
	sh->setMat4f("u_ModelMat", modelMat);

	BindTexture(diffuse, DIFF_TEX_SLOT);
	BindTexture(specular, SPEC_TEX_SLOT);
	BindTexture(s_Data->DepthMap, DEPTH_TEX_SLOT);
	BindVAO(vao);

	glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	sh->setInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	glDrawArrays(GL_TRIANGLES, 0, vao->Count());
	sh->setInt("reverse_normals", 0); // and of course disable it
	glEnable(GL_CULL_FACE);
}

void Renderer::ShadowRenderSetup(glm::vec3 lightPos)
{
	float near_plane = 1.0f;
	float far_plane = 25.0f;
	unsigned SHADOW_WIDTH = 1024;
	unsigned SHADOW_HEIGHT = 1024;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
	s_Data->DepthMapFBO->Bind();
	glClear(GL_DEPTH_BUFFER_BIT);

	auto sh = s_Data->Shader[ShaderType::DepthShader];
	BindShader(sh);
	for (unsigned int i = 0; i < 6; ++i)
		sh->setMat4f("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	sh->setFloat("far_plane", far_plane);
	sh->setFloat3("lightPos", lightPos);
	sh = s_Data->Shader[ShaderType::DiffNSpec];
	BindShader(sh);
	sh->setFloat("far_plane", far_plane);
	sh->setFloat3("lightPos", lightPos);
}
void Renderer::ShadowRenderEnd()
{
	s_Data->DepthMapFBO->Unbind();
}
void Renderer::DrawSkybox()
{
	glDepthFunc(GL_LEQUAL);
	Ref<Shader> sh = s_Data->Shader[ShaderType::Skybox];
	BindShader(sh);
	//s_Data->Shader[ShaderType::Skybox]->setInt("u_SkyboxTex", SKYBOX_TEX_SLOT);
	sh->setMat4f("u_ProjMat", s_Data->ProjMat);
	sh->setMat4f("u_ViewMat",
		glm::mat4(glm::mat3(s_Data->ViewMat)));

	BindTexture(s_Data->skyboxData.SkyboxTex, SKYBOX_TEX_SLOT);
	BindVAO(s_Data->skyboxData.SkyboxVAO);

	glDrawArrays(GL_TRIANGLES, 0, s_Data->skyboxData.SkyboxVAO->Count());
	glDepthFunc(GL_LESS);
}

void Renderer::LoadShaders()
{
	s_Data->Shader[ShaderType::Color] = CreateRef<Shader>("color.shader");

	s_Data->Shader[ShaderType::Diffuse] = CreateRef<Shader>("diffuse.shader");
	s_Data->Shader[ShaderType::Diffuse]->Bind();
	s_Data->Shader[ShaderType::Diffuse]->setInt(  "material.diffuse", DIFF_TEX_SLOT);
	s_Data->Shader[ShaderType::Diffuse]->setFloat("material.specular", 0.5f);
	s_Data->Shader[ShaderType::Diffuse]->setFloat("material.shininess", 32.0f);

	s_Data->Shader[ShaderType::DiffNSpec] = CreateRef<Shader>("diffNSpec.shader");
	s_Data->Shader[ShaderType::DiffNSpec]->Bind();
	s_Data->Shader[ShaderType::DiffNSpec]->setInt("material.diffuse", DIFF_TEX_SLOT);
	s_Data->Shader[ShaderType::DiffNSpec]->setInt("material.specular", SPEC_TEX_SLOT);
	s_Data->Shader[ShaderType::DiffNSpec]->setFloat("material.shininess", 32.0f);
	s_Data->Shader[ShaderType::DiffNSpec]->setInt("depthMap", DEPTH_TEX_SLOT);

	s_Data->Shader[ShaderType::Skybox] = CreateRef<Shader>("skybox.shader");
	s_Data->Shader[ShaderType::Skybox]->Bind();
	s_Data->Shader[ShaderType::Skybox]->setInt("u_SkyboxTex", SKYBOX_TEX_SLOT);

	s_Data->Shader[ShaderType::DepthShader] = CreateRef<Shader>("depthShader.shader");
	
}

void Renderer::CreateSkybox()
{
	const char* SKYBOX_FACES[] = {
		"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"
	};

	s_Data->skyboxData.SkyboxVAO = CreateRef<VAO>();
	s_Data->skyboxData.SkyboxVBO = CreateRef<VBO>(
		GeoData::SKYBOX_DATA, sizeof(GeoData::SKYBOX_DATA), 36);
	VertexLayout layout{ {GL_FLOAT, 3, GL_FALSE} };
	s_Data->skyboxData.SkyboxVBO->SetLayout(layout);
	s_Data->skyboxData.SkyboxVAO->AddBuffer(*s_Data->skyboxData.SkyboxVBO);
	s_Data->skyboxData.SkyboxTex = CreateRef<Texture>("skybox", SKYBOX_FACES);
}

std::pair<unsigned, unsigned> Renderer::GetSizeOffset(const LightType type)
{
	unsigned size   = 0;
	unsigned offset = 0;
	switch (type)
	{
	case LightType::Directional:
		size = SSBO_SPOTLIGHT_OFFSET - SSBO_DIRLIGHT_OFFSET;
		offset = SSBO_DIRLIGHT_OFFSET;
		break;
	case LightType::Spot:
		size = SSBO_POINTLIGHT_OFFSET - SSBO_SPOTLIGHT_OFFSET;
		offset = SSBO_SPOTLIGHT_OFFSET;
		break;
	case LightType::Point:
		size = SSBO_POINTLIGHT_SIZE;
		offset = SSBO_POINTLIGHT_OFFSET;
		break;
	}
	return std::pair<unsigned, unsigned>(size, offset);
}

void Renderer::UpdateLightPosition(const float pos[3], const unsigned lightIndex)
{
	static unsigned posSize = 3 * sizeof(float);
	s_Data->LightSSBO->Upload(pos, posSize, lightIndex * SSBO_LIGHT_SIZE);
}

const unsigned Renderer::UploadLightData(const void* data)
{
	static unsigned offset = 0;
	static unsigned lightIndex = 0;
	s_Data->LightSSBO->Upload(data, SSBO_LIGHT_SIZE, offset);
	offset += SSBO_LIGHT_SIZE;
	++lightIndex;
	return lightIndex - 1;
}

bool Renderer::BindShader(const Ref<Shader> shader)
{
	if (s_Data->boundShaderId != shader->Id())
	{
		shader->Bind();
		s_Data->boundShaderId = shader->Id();
		return true;
	}
	return false;
}

void Renderer::BindVAO(const Ref<VAO> vao)
{
	if (s_Data->boundVaoId != vao->Id())
	{
		vao->Bind();
		s_Data->boundVaoId = vao->Id();
	}
}

void Renderer::BindTexture(const Ref<Texture> tex, const short slot)
{
	if (s_Data->TexSlotId[slot] != tex->Id())
	{
		tex->Bind(slot);
		s_Data->TexSlotId[slot] = tex->Id();
	}
}

void Renderer::BeginScene(const Camera& camera)
{
	s_Data->SceneUBO->Upload(
		glm::value_ptr(camera.GetProjViewMat()), sizeof(glm::mat4), 0);
	s_Data->SceneUBO->Upload(
		glm::value_ptr(camera.Position()), sizeof(glm::vec3), 64);
	

	s_Data->ViewMat = camera.GetViewMat();
	s_Data->ProjMat = camera.GetProjMat();
}

void Renderer::EndScene()
{
}

void Renderer::Clear(std::bitset<3> bufferBits)
{
	int buffers = 0;
	if (bufferBits[0])
	{
		buffers |= GL_COLOR_BUFFER_BIT;
	}
	if (bufferBits[1])
	{
		buffers |= GL_DEPTH_BUFFER_BIT;
	}
	if (bufferBits[2])
	{
		buffers |= GL_STENCIL_BUFFER_BIT;
	}
	glClear(buffers);
}
void Renderer::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void Renderer::ResetViewport()
{
	glViewport(0, 0, s_Data->viewportWidth, s_Data->viewportHeight);
}

void Renderer::Shutdown()
{
	delete s_Data;
}