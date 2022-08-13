#include "pch.h"

#include "renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "glad/glad.h"
#include "geometry/GeoData.h"

namespace Renderer
{
	namespace //private
	{
		struct SkyboxData
		{
			Ref<VAO> SkyboxVAO;
			Ref<VBO> SkyboxVBO;
			Ref<Texture> SkyboxTex;
		};
		struct SceneData
		{
			const glm::mat4 camProjView;
			const glm::vec3 camPos;
			unsigned lightCount;
			unsigned castShadows;
		};
		struct RenderData
		{
			std::unordered_map<ShaderType, Ref<Shader>> Shader;
			Ref<ShaderBlock> SceneUBO;
			Ref<ShaderBlock> LightSSBO;
			Ref<Framebuffer> DefaultFramebuffer;
			Ref<Framebuffer> DepthMapFBO;
			Ref<Texture> DepthMap;

			glm::mat4 ViewMat = glm::mat4(1.f);
			glm::mat4 ProjMat = glm::mat4(1.f);

			std::unordered_map<unsigned, unsigned> TexSlotId;
			float lightFarPlane{ 25.f };
			unsigned LightsCount;
			unsigned boundVaoId;
			unsigned boundShaderId;
			//unsigned boundFramebufferId;
			unsigned viewportWidth;
			unsigned viewportHeight;
			SkyboxData skyboxData;
			float outlineBorderScale = 0.1f;
			float outlineBrightness = 1.f;
			glm::vec4 outlineColor = glm::vec4(glm::vec3(242, 140, 40) / 256.f * outlineBrightness, 1 ) ; //bright orange
		};

		constexpr const unsigned SSBO_DIRLIGHT_OFFSET = 0;
		constexpr const unsigned SSBO_SPOTLIGHT_OFFSET = SSBO_DIRLIGHT_OFFSET + 64;
		constexpr const unsigned SSBO_POINTLIGHT_OFFSET = SSBO_SPOTLIGHT_OFFSET + 80;

		constexpr const unsigned SSBO_POINTLIGHT_SIZE = 16 * 4;

		constexpr const unsigned SSBO_LIGHT_SIZE = 96;

		constexpr const unsigned SHADOW_MAP_WIDTH = 1024;
		constexpr const unsigned SHADOW_MAP_HEIGHT = 1024;

		RenderData* s_Data = nullptr;

		constexpr short DIFF_TEX_SLOT = 0;
		constexpr short SPEC_TEX_SLOT = 1;
		constexpr short NORM_TEX_SLOT = 2;
		constexpr short SKYBOX_TEX_SLOT = 7;
		constexpr short DEPTH_TEX_SLOT = 8;

		void LoadShaders();
		void CreateSkybox();
		void GLDraw(const Ref<VAO> vao);

		void SetUniformBuffer(const Ref<ShaderBlock> ssbo, const short slot,
			std::vector<ShaderType> shTypes);
		void SetShaderStorageBuffer(const Ref<ShaderBlock> ssbo, const short slot,
			std::vector<ShaderType> shTypes);

		void BindShader(const Ref<Shader> shader);
		void BindVAO(const Ref<VAO> vao);
		void BindTexture(const Ref<Texture> tex, const short slot);
	}

	void Init(unsigned width, unsigned height)
	{
		s_Data = new RenderData();
		s_Data->viewportWidth = width;
		s_Data->viewportHeight = height;
		s_Data->DefaultFramebuffer = CreateRef<Framebuffer>();
		s_Data->DefaultFramebuffer->Invalidate(width, height);
		
		s_Data->DepthMap = CreateRef<Texture>(SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
		s_Data->DepthMapFBO = CreateRef<Framebuffer>();
		s_Data->DepthMapFBO->AttachDepthCubemap(s_Data->DepthMap->Id(), SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);

		LoadShaders();
		CreateSkybox();

		s_Data->SceneUBO = CreateRef<ShaderBlock>(
			"SceneData", (const void*)NULL,
			256,
			GL_UNIFORM_BUFFER);

		SetUniformBuffer(s_Data->SceneUBO, 0,
			{ ShaderType::UniformColor, ShaderType::AttribColor,
			ShaderType::Diffuse, ShaderType::DiffNSpec, ShaderType::NormalMap });

		s_Data->LightSSBO = CreateRef<ShaderBlock>(
			"LightData", (const void*)NULL,
			512,
			GL_SHADER_STORAGE_BUFFER);

		SetShaderStorageBuffer(s_Data->LightSSBO, 0,
			{ ShaderType::Diffuse, ShaderType::DiffNSpec, ShaderType::NormalMap });
		//Set gl config
		{
			glEnable(GL_CULL_FACE);

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			glEnable(GL_STENCIL_TEST);
		}
		glClearColor(0.049f, 0.0f, 0.1f, 1.f); //Dark purple
	}

	void Draw(int drawID, const glm::mat4& modelMat, MeshInstance& mi)
	{
		auto& Mesh = mi.mesh;
		auto& tex = Mesh->Textures();
		auto& diff = tex[TexType::Diffuse];
		auto& spec = tex[TexType::Specular];
		auto& norm = tex[TexType::Normal];
		Ref<Shader> sh = nullptr;

		if (mi.HasTextures)
		{
			if (!diff.empty() && !norm.empty())
			{
				sh = s_Data->Shader[ShaderType::NormalMap];
				BindShader(sh);
				BindTexture(diff[0], DIFF_TEX_SLOT);
				BindTexture(norm[0], NORM_TEX_SLOT);
				BindTexture(s_Data->DepthMap, DEPTH_TEX_SLOT);
			}
			else if (!diff.empty() && !spec.empty())
			{
				sh = s_Data->Shader[ShaderType::DiffNSpec];
				BindShader(sh);
				BindTexture(diff[0], DIFF_TEX_SLOT);
				BindTexture(spec[0], SPEC_TEX_SLOT);
				BindTexture(s_Data->DepthMap, DEPTH_TEX_SLOT);
			}
			else if (!diff.empty())
			{
				sh = s_Data->Shader[ShaderType::Diffuse];
				BindShader(sh);
				BindTexture(diff[0], DIFF_TEX_SLOT);
				BindTexture(s_Data->DepthMap, DEPTH_TEX_SLOT);
			}
			else
			{
				sh = s_Data->Shader[ShaderType::UniformColor];
				BindShader(sh);
				sh->setFloat4("u_Color", mi.Color);
			}
		}
		else
		{
			sh = s_Data->Shader[ShaderType::UniformColor];
			BindShader(sh);
			sh->setFloat4("u_Color", mi.Color);
		}

		sh->setMat4f("u_ModelMat", modelMat);
		sh->setInt  ("u_DrawId", drawID);

		if (mi.NormalsOut)
			GLDraw(Mesh->Vao());
		else
		{
			glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
			sh->setInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
			GLDraw(Mesh->Vao());
			sh->setInt("reverse_normals", 0); // and of course disable it
			glEnable(GL_CULL_FACE);
		}
	}

	void DrawOutlined(int drawID, const glm::mat4& modelMat, MeshInstance& mi)
	{
		glClear(GL_STENCIL_BUFFER_BIT);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
		glStencilMask(0xFF);

		Draw(drawID, modelMat, mi);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		glm::mat4 newMat = modelMat;
		newMat[0][0] += 0.1f;
		newMat[1][1] += 0.1f;
		newMat[2][2] += 0.1f;

		Ref<Shader> sh = s_Data->Shader[ShaderType::UniformColor];
		BindShader(sh);
		sh->setFloat4("u_Color", s_Data->outlineColor);

		sh->setMat4f("u_ModelMat", newMat);
		sh->setInt	("u_DrawId", drawID);
		GLDraw(mi.mesh->Vao());

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);
	}

	void DrawDepth(const glm::mat4& modelMat, const MeshInstance& mi)
	{
		auto Mesh = mi.mesh;
		Ref<Shader> sh = s_Data->Shader[ShaderType::DepthShader];
		BindShader(sh);
		sh->setMat4f("u_ModelMat", modelMat);

		BindVAO(Mesh->Vao());

		if (mi.NormalsOut)
			GLDraw(Mesh->Vao());
		else
		{
			glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
			sh->setInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
			GLDraw(Mesh->Vao());
			sh->setInt("reverse_normals", 0); // and of course disable it
			glEnable(GL_CULL_FACE);
		}
	}

	void ShadowRenderSetup(glm::vec3 lightPos)
	{
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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
	}

	void ShadowRenderEnd()
	{
		//BindDefaultFramebuffer();
		s_Data->DefaultFramebuffer->Bind();
		//ResetViewport();
		//s_Data->DepthMapFBO->Unbind(s_Data->viewportWidth, s_Data->viewportHeight);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void DrawSkybox()
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

	void UpdateLightPosition(const float pos[3], const unsigned lightIndex)
	{
		static unsigned posSize = 3 * sizeof(float);
		s_Data->LightSSBO->Upload(pos, posSize, lightIndex * SSBO_LIGHT_SIZE);
	}

	unsigned UploadLightData(const void* data)
	{
		static unsigned offset = 0;
		static unsigned lightIndex = 0;
		s_Data->LightSSBO->Upload(data, SSBO_LIGHT_SIZE, offset);
		offset += SSBO_LIGHT_SIZE;
		++lightIndex;
		return lightIndex - 1;
	}

	const Ref<Framebuffer>& Renderer::GetMainFB()
	{
		return s_Data->DefaultFramebuffer;
	}

	void SetRenderImageSize(const unsigned width, const unsigned height)
	{
		s_Data->viewportWidth = width;
		s_Data->viewportHeight = height;
		s_Data->DefaultFramebuffer->Invalidate(width, height);

	}

	void ClearState()
	{
		s_Data->boundVaoId = 0;
		s_Data->boundShaderId = 0;
		s_Data->TexSlotId.clear();
	}

	void BeginScene(Ref<Camera> cam, unsigned lightCount, bool castShadows)
	{
		SceneData data = { cam->GetProjViewMat(), cam->Position(), lightCount, castShadows };
		s_Data->SceneUBO->Upload((const void*)&data, sizeof(data), 0);

		s_Data->ViewMat = cam->GetViewMat();
		s_Data->ProjMat = cam->GetProjMat();

		s_Data->DefaultFramebuffer->Bind();
		
		/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		s_Data->DefaultFramebuffer->ClearIntAttachment(-1);*/
	}

	void EndScene()
	{
		
		s_Data->DefaultFramebuffer->Unbind(s_Data->viewportWidth, s_Data->viewportHeight);
	}

	void Clear(int buffers)
	{
		glClear(buffers);
	}

	void SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void ResetViewport()
	{
		glViewport(0, 0, s_Data->viewportWidth, s_Data->viewportHeight);
	}

	void Shutdown()
	{
		delete s_Data;
	}


	namespace //private
	{
		void GLDraw(const Ref<VAO> vao)
		{
			BindVAO(vao);
			auto ebo = vao->Ebo();
			if (ebo)
			{
				ebo->Bind();
				glDrawElements(GL_TRIANGLES, ebo->Count(), GL_UNSIGNED_INT, 0);
			}
			else
				glDrawArrays(GL_TRIANGLES, 0, vao->Count());
		}

		void SetUniformBuffer(const Ref<ShaderBlock> ubo, const short slot,
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

		void SetShaderStorageBuffer(const Ref<ShaderBlock> ssbo, const short slot,
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

		void BindShader(const Ref<Shader> shader)
		{
			if (s_Data->boundShaderId != shader->Id())
			{
				shader->Bind();
				s_Data->boundShaderId = shader->Id();
			}
		}

		void BindVAO(const Ref<VAO> vao)
		{
			if (s_Data->boundVaoId != vao->Id())
			{
				vao->Bind();
				s_Data->boundVaoId = vao->Id();
			}
		}

		void BindTexture(const Ref<Texture> tex, const short slot)
		{
			if (s_Data->TexSlotId[slot] != tex->Id())
			{
				tex->Bind(slot);
				s_Data->TexSlotId[slot] = tex->Id();
			}
		}

		void LoadShaders()
		{
			s_Data->Shader[ShaderType::UniformColor] = CreateRef<Shader>("color.shader");

			s_Data->Shader[ShaderType::AttribColor] = CreateRef<Shader>("colorAttrib.shader");

			s_Data->Shader[ShaderType::Diffuse] = CreateRef<Shader>("diffuse.shader");
			s_Data->Shader[ShaderType::Diffuse]->Bind();
			s_Data->Shader[ShaderType::Diffuse]->setInt("material.diffuse", DIFF_TEX_SLOT);
			s_Data->Shader[ShaderType::Diffuse]->setFloat("material.specular", 0.5f);
			s_Data->Shader[ShaderType::Diffuse]->setFloat("material.shininess", 32.0f);
			s_Data->Shader[ShaderType::Diffuse]->setInt("depthMap", DEPTH_TEX_SLOT);

			s_Data->Shader[ShaderType::DiffNSpec] = CreateRef<Shader>("diffNSpec.shader");
			s_Data->Shader[ShaderType::DiffNSpec]->Bind();
			s_Data->Shader[ShaderType::DiffNSpec]->setInt("material.diffuse", DIFF_TEX_SLOT);
			s_Data->Shader[ShaderType::DiffNSpec]->setInt("material.specular", SPEC_TEX_SLOT);
			s_Data->Shader[ShaderType::DiffNSpec]->setFloat("material.shininess", 32.0f);
			s_Data->Shader[ShaderType::DiffNSpec]->setInt("depthMap", DEPTH_TEX_SLOT);
			s_Data->Shader[ShaderType::DiffNSpec]->setFloat("far_plane", s_Data->lightFarPlane);

			s_Data->Shader[ShaderType::NormalMap] = CreateRef<Shader>("normalMap.shader");
			s_Data->Shader[ShaderType::NormalMap]->Bind();
			s_Data->Shader[ShaderType::NormalMap]->setInt("diffuseMap", DIFF_TEX_SLOT);
			s_Data->Shader[ShaderType::NormalMap]->setInt("normalMap", NORM_TEX_SLOT);
			s_Data->Shader[ShaderType::NormalMap]->setInt("depthMap", DEPTH_TEX_SLOT);
			s_Data->Shader[ShaderType::NormalMap]->setFloat("far_plane", s_Data->lightFarPlane);

			s_Data->Shader[ShaderType::Skybox] = CreateRef<Shader>("skybox.shader");
			s_Data->Shader[ShaderType::Skybox]->Bind();
			s_Data->Shader[ShaderType::Skybox]->setInt("u_SkyboxTex", SKYBOX_TEX_SLOT);

			s_Data->Shader[ShaderType::DepthShader] = CreateRef<Shader>("depthShader.shader");
		}

		void CreateSkybox()
		{
			const char* SKYBOX_FACES[] = {
				"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"
			};

			s_Data->skyboxData.SkyboxVAO = CreateRef<VAO>();
			auto& gd = GeoData::GetData(Primitive::Skybox);
			s_Data->skyboxData.SkyboxVBO = CreateRef<VBO>(
				gd.data, gd.size, gd.count);
			VertexLayout layout{ {GL_FLOAT, 3, GL_FALSE} };
			s_Data->skyboxData.SkyboxVBO->SetLayout(layout);
			s_Data->skyboxData.SkyboxVAO->AddBuffer(*s_Data->skyboxData.SkyboxVBO, nullptr);
			s_Data->skyboxData.SkyboxTex = CreateRef<Texture>("skybox", SKYBOX_FACES);
		}
	}
}
