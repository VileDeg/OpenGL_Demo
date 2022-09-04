#include "pch.h"

#include "renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "glad/glad.h"
#include "geometry/GeoData.h"

namespace Crave
{
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
				Ref<Framebuffer> ViewportFB;
				Ref<Framebuffer> DepthMapFBO;
				Ref<Texture> DepthMap;
				Ref<Camera> Camera;

				glm::mat4 ViewMat = glm::mat4(1.f);
				glm::mat4 ProjMat = glm::mat4(1.f);

				std::unordered_map<unsigned, unsigned> TexSlotId;
				float lightNearPlane{ 0.f };
				float lightFarPlane{ 50.f };
				unsigned LightsCount;
				unsigned boundVaoId;
				unsigned boundShaderId;
				//unsigned boundFramebufferId;
				unsigned viewportWidth;
				unsigned viewportHeight;
				SkyboxData skyboxData;
				float outlineBorderScale = 0.1f;
				float outlineBrightness = 1.f;
				glm::vec4 outlineColor = glm::vec4(glm::vec3(242, 140, 40) / 256.f * outlineBrightness, 1); //bright orange
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

		void Init(Ref<Framebuffer> viewportfb, unsigned width, unsigned height)
		{
			s_Data = new RenderData();
			s_Data->viewportWidth = width;
			s_Data->viewportHeight = height;
			s_Data->ViewportFB = viewportfb;
			//s_Data->ViewportFB->Invalidate({ width, height });

			//s_Data->DepthMap = CreateRef<Texture>(SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);

			s_Data->DepthMapFBO = CreateRef<Framebuffer>(Framebuffer::Config{ false, Framebuffer::DrawBuffers::None, Framebuffer::ReadBuffers::None,
				std::initializer_list<Texture::Config>{
					{Texture::Type::Depth, Texture::Target::TextureCubeMap, Texture::MMFilter::Nearest, Texture::WrapMode::ClampToEdge} } });
			s_Data->DepthMapFBO->Invalidate({ SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT });
			s_Data->DepthMap = s_Data->DepthMapFBO->GetDepthAttachment();
			//s_Data->DepthMapFBO->AttachDepthCubemap(s_Data->DepthMap->Id(), SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);

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


		void DrawMesh(int drawID, const glm::mat4& modelMat, Ref<Mesh> mesh, bool withTextures, glm::vec4 color)
		{
			auto& tex = mesh->Textures();
			auto& diff = tex[Mesh::TexType::Diffuse];
			auto& spec = tex[Mesh::TexType::Specular];
			auto& norm = tex[Mesh::TexType::Normal];
			Ref<Shader> sh = nullptr;

			if (withTextures)
			{
				sh = s_Data->Shader[ShaderType::General];
				BindShader(sh);
				BindTexture(s_Data->DepthMap, DEPTH_TEX_SLOT);
				if (!diff.empty() && !norm.empty())
				{
					/*sh = s_Data->Shader[ShaderType::NormalMap];
					BindShader(sh);*/
					BindTexture(diff[0], DIFF_TEX_SLOT);
					BindTexture(norm[0], NORM_TEX_SLOT);
					sh->setUint("u_ObjType", 2);
				}
				else if (!diff.empty() && !spec.empty())
				{
					/*sh = s_Data->Shader[ShaderType::DiffNSpec];
					BindShader(sh);*/
					BindTexture(diff[0], DIFF_TEX_SLOT);
					BindTexture(spec[0], SPEC_TEX_SLOT);
					//BindTexture(s_Data->DepthMap, DEPTH_TEX_SLOT);
					sh->setUint("u_ObjType", 1);
				}
				else if (!diff.empty())
				{
					/*sh = s_Data->Shader[ShaderType::Diffuse];
					BindShader(sh);*/
					BindTexture(diff[0], DIFF_TEX_SLOT);
					//BindTexture(s_Data->DepthMap, DEPTH_TEX_SLOT);
					sh->setUint("u_ObjType", 0);
				}
				else
				{
					//Mesh has no textues. Most likely error.
					//sh = s_Data->Shader[ShaderType::UniformColor];
					//BindShader(sh);
					//sh->setFloat4("u_Color", { 1.f, 0.f, 1.f, 1.f }); //magenta
					ASSERT(false, "");
				}
			}
			else
			{
				sh = s_Data->Shader[ShaderType::UniformColor];
				BindShader(sh);
				sh->setFloat4("u_Color", color);
			}

			sh->setMat4f("u_ModelMat", modelMat);
			sh->setInt("u_DrawId", drawID);

			GLDraw(mesh->Vao());
		}

		void DrawOutlined(int drawID, const glm::mat4& modelMat, Ref<Mesh> mesh,
			bool withTextures, glm::vec4 color)
		{
			glClear(GL_STENCIL_BUFFER_BIT);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
			glStencilMask(0xFF);

			DrawMesh(drawID, modelMat, mesh, withTextures, color);

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);

			glm::vec3 pos = modelMat[3];
			glm::vec3 scale = { glm::length(modelMat[0]), glm::length(modelMat[1]), glm::length(modelMat[2]) };
			float avScale = scale.x + scale.y + scale.z;
			avScale /= 3.f;
			float distToCam = glm::length(s_Data->Camera->Position() - pos);
			float borderWidth = s_Data->outlineBorderScale;

			glm::vec3 finScale =
				glm::vec3(borderWidth) / scale;

			finScale *= distToCam * 0.025;

			glm::mat4 outlineMat = glm::scale(modelMat,
				glm::vec3(finScale) + 1.f);

			Ref<Shader> sh = s_Data->Shader[ShaderType::UniformColor];
			BindShader(sh);
			sh->setFloat4("u_Color", s_Data->outlineColor);

			sh->setMat4f("u_ModelMat", outlineMat);
			sh->setInt("u_DrawId", drawID);

			GLDraw(mesh->Vao());

			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glEnable(GL_DEPTH_TEST);
		}

		void DrawDepth(const glm::mat4& modelMat, Ref<Mesh> mesh)
		{

			Ref<Shader> sh = s_Data->Shader[ShaderType::DepthShader];
			BindShader(sh);
			sh->setMat4f("u_ModelMat", modelMat);

			BindVAO(mesh->Vao());

			GLDraw(mesh->Vao());
		}



		void ShadowRenderSetup(glm::vec3 lightPos)
		{
			//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			//float near_plane = 1.0f;
			//float far_plane = 25.0f;
			glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_MAP_WIDTH / (float)SHADOW_MAP_HEIGHT, s_Data->lightNearPlane, s_Data->lightFarPlane);
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
			sh->setFloat("far_plane", s_Data->lightFarPlane);
			sh->setFloat3("lightPos", lightPos);
		}

		void ShadowRenderEnd()
		{
			//BindViewportFB();
			s_Data->ViewportFB->Bind();
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
			//static unsigned offset = 0;
			//static unsigned lightIndex = 0;
			s_Data->LightSSBO->Upload(data, SSBO_LIGHT_SIZE, 0); //offset
			//offset += SSBO_LIGHT_SIZE;
			//++lightIndex;
			//return lightIndex - 1;
			return 0;
		}

		glm::vec4 SetOutlineColor(const glm::vec4& color)
		{
			glm::vec4 old = s_Data->outlineColor;
			s_Data->outlineColor = color;
			return old;
		}

		//void SetRenderImageSize(const unsigned width, const unsigned height)
		//{
		//	s_Data->viewportWidth = width;
		//	s_Data->viewportHeight = height;
		//	s_Data->ViewportFB->Invalidate(width, height);

		//}

		void ClearState()
		{
			s_Data->boundVaoId = 0;
			s_Data->boundShaderId = 0;
			s_Data->TexSlotId.clear();
		}

		void BeginScene(Ref<Camera> cam, unsigned lightCount, bool castShadows)
		{
			SceneData data = { cam->GetProjViewMat(), cam->Position(), lightCount, castShadows };
			s_Data->Camera = cam;
			s_Data->SceneUBO->Upload((const void*)&data, sizeof(data), 0);

			s_Data->ViewMat = cam->GetViewMat();
			s_Data->ProjMat = cam->GetProjMat();

			s_Data->ViewportFB->Bind();

			/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			s_Data->ViewportFB->ClearIntAttachment(-1);*/
		}

		void EndScene()
		{

			s_Data->ViewportFB->Unbind();
		}

		void Clear(int buffers)
		{
			glClear(buffers);
		}

		void SetClearColor(float r, float g, float b, float a)
		{
			glClearColor(r, g, b, a);
		}

		//void ResetViewport()
		//{
		//	glViewport(0, 0, s_Data->viewportWidth, s_Data->viewportHeight);
		//}

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
				auto& sh = s_Data->Shader[ShaderType::General] = CreateRef<Shader>("general.shader");
				sh->Bind();

				sh->setInt("material.diffuseTex", DIFF_TEX_SLOT);
				sh->setInt("material.specularTex", SPEC_TEX_SLOT);
				sh->setInt("material.normalTex", NORM_TEX_SLOT);
				sh->setFloat("material.specularFloat", 0.5f);
				sh->setFloat("material.shininess", 32.0f);
				sh->setFloat4("material.color", {1.f, 0.f, 1.f, 1.f}); //magenta

				sh->setInt("depthMap", DEPTH_TEX_SLOT);
				sh->setFloat("far_plane", s_Data->lightFarPlane);

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
}