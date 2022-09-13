#include "pch.h"

#include "renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "glad/glad.h"
#include "geometry/GeoData.h"

#include <imgui.h>

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

			constexpr const int MAX_LIGHTS_COUNT = 16; //6

			struct RenderData
			{
				std::unordered_map<ShaderType, Ref<Shader>> Shader;
				Ref<ShaderBlock> SceneUBO;
				Ref<ShaderBlock> LightUBO;
				//std::vector<std::pair<unsigned, LightData>> LightDataSubmitted{ MAX_LIGHTS_COUNT };
				/*struct
				{
					std::vector<unsigned> id;
					std::vector<LightData> data;
				} LightDataSubmitted;*/
				struct
				{
					//LightData data[MAX_LIGHTS_COUNT];
					bool enabled;
					LightData data;
				} LightDataSubmitted[MAX_LIGHTS_COUNT];
				//Ref<ShaderBlock> MiscSSBO;
				Ref<Framebuffer> ViewportFB;
				Ref<Framebuffer> DepthMapFBO;
				Ref<Texture> DepthMap;
				Ref<Camera> Camera;

				//glm::mat4 ViewMat = glm::mat4(1.f);
				//glm::mat4 ProjMat = glm::mat4(1.f);

				std::unordered_map<unsigned, unsigned> TexSlotId;
				/*float lightNearPlane{ 0.1f };
				float lightFarPlane{ 50.f };*/
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

			constexpr const float POINT_NEAR_PLANE = 0.1f;
			constexpr const float POINT_FAR_PLANE = 250.f;

			constexpr const float SPOT_NEAR_PLANE = 0.1f;
			constexpr const float SPOT_FAR_PLANE = 250.f;

			constexpr const float DIR_NEAR_PLANE = -500.f;
			constexpr const float DIR_FAR_PLANE = 500.f;

			constexpr const int SHADER_LIGHT_SIZE = 176;

			//constexpr const int MAX_POINT_LIGHTS = 3;
			

			constexpr const int SFRAME_SIZE = 1024;
			//Atlas 3x2
			constexpr const glm::ivec2 SATLAS_DIM = { 6, 4 };
			constexpr const glm::ivec2 SATLAS_SIZE = SATLAS_DIM * SFRAME_SIZE;
			//constexpr const unsigned SHADOW_ATLAS_HEIGHT = ;

			//unsigned m_AtlasFramesCount = MAX_POINT_LIGHTS * 6 + MAX_LIGHTS_COUNT - MAX_POINT_LIGHTS;
			//unsigned m_AtlasFramesPerRow = SHADOW_ATLAS_WIDTH / SHADOW_FRAME_DIM;
			RenderData* s_Data = nullptr;

			constexpr short DIFF_TEX_SLOT = 0;
			constexpr short SPEC_TEX_SLOT = 1;
			constexpr short NORM_TEX_SLOT = 2;
			constexpr short SKYBOX_TEX_SLOT = 7;
			constexpr short DEPTH_TEX_SLOT = 8;

			void LoadShaders();
			void CreateSkybox();
			void GLDraw(const Ref<VAO> vao);

			

			/*void SetUniformBuffer(const Ref<ShaderBlock> ssbo, const short slot,
				std::vector<ShaderType> shTypes);
			void SetShaderStorageBuffer(const Ref<ShaderBlock> ssbo, const short slot,
				std::vector<ShaderType> shTypes);*/

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

			s_Data->DepthMapFBO = CreateRef<Framebuffer>(Framebuffer::Config{ false, Framebuffer::DrawBuffers::None, Framebuffer::ReadBuffers::None,
				std::initializer_list<Texture::Config>{
					{Texture::Type::Depth, Texture::Target::Texture2D, Texture::MMFilter::Nearest, Texture::WrapMode::ClampToEdge} } });
			s_Data->DepthMapFBO->Invalidate(SATLAS_SIZE);
			s_Data->DepthMap = s_Data->DepthMapFBO->GetDepthAttachment();
			//s_Data->DepthMapFBO->AttachDepthCubemap(s_Data->DepthMap->Id(), SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);

			LoadShaders();
			CreateSkybox();


			s_Data->LightUBO = CreateRef<ShaderBlock>(
				"LightData", (const void*)NULL,
				MAX_LIGHTS_COUNT * SHADER_LIGHT_SIZE,
				GL_UNIFORM_BUFFER);
			s_Data->LightUBO->Bind(0);

			s_Data->SceneUBO = CreateRef<ShaderBlock>(
				"SceneData", (const void*)NULL,
				256,
				GL_UNIFORM_BUFFER);
			s_Data->SceneUBO->Bind(1);

			/*s_Data->MiscSSBO = CreateRef<ShaderBlock>(
				"LightData", (const void*)NULL,
				MAX_LIGHTS_COUNT * 64,
				GL_SHADER_STORAGE_BUFFER);
			s_Data->MiscSSBO->Bind(0);*/

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

		void DrawDepth(const glm::mat4& modelMat, Ref<Mesh> mesh, ShaderType shType)
		{
			Ref<Shader> sh = s_Data->Shader[shType];
			BindShader(sh);
			sh->setMat4f("u_ModelMat", modelMat);

			BindVAO(mesh->Vao());

			GLDraw(mesh->Vao());
		}

		void GlobalShadowSetup()
		{
			s_Data->DepthMapFBO->Bind();
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		void DirSpotShadowSetup(LightData& data, int frameNum, LightType type)
		{
			// render scene from light's point of view
			glm::ivec2 offset = { (frameNum *3) % SATLAS_DIM.x, (frameNum *3) / SATLAS_DIM.x * 2 };
			offset *= SFRAME_SIZE;
			data.atlasoffset = offset;
			
			glViewport(offset.x, offset.y, SFRAME_SIZE, SFRAME_SIZE);

			Ref<Shader> sh{};
			if (type == LightType::Directional)
			{
				sh = s_Data->Shader[ShaderType::DirDepth];
				BindShader(sh);
			}
			else if (type == LightType::Spot)
			{
				sh = s_Data->Shader[ShaderType::SpotDepth];
				BindShader(sh);
				//sh->setFloat3("u_LightPos", data.position);
			}
			else DEBUG_BREAK("");
			sh->setMat4f("u_LightSpaceMat", data.projViewMat);
		}

		void PointShadowSetup(LightData& data, int frameNum)
		{
			//frameNum += 1;
			glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f),
				1.f, POINT_NEAR_PLANE, POINT_FAR_PLANE);
			std::vector<glm::mat4> shadowTransforms;
			shadowTransforms.push_back(shadowProj * glm::lookAt(data.position, data.position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(data.position, data.position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(data.position, data.position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(data.position, data.position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(data.position, data.position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(data.position, data.position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

			glm::ivec2 offsetExternal = { frameNum % SATLAS_DIM.x, frameNum / SATLAS_DIM.x };
			offsetExternal *= glm::ivec2(3, 2) * SFRAME_SIZE;
			data.atlasoffset = offsetExternal;
			for (int h = 0; h < 2; ++h)
			{
				for (int w = 0; w < 3; ++w)
				{
					glm::ivec2 offset = { SFRAME_SIZE * w, SFRAME_SIZE * h };
					offset += offsetExternal;
					glViewportIndexedf(w+h*3, offset.x, offset.y, SFRAME_SIZE, SFRAME_SIZE);
				}
			}

			auto sh = s_Data->Shader[ShaderType::PointDepth];
			BindShader(sh);
			for (unsigned int i = 0; i < 6; ++i)
				sh->setMat4f("u_ShadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
			
			
			sh->setFloat3("u_LightPos", data.position);
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
			bool isPersp = s_Data->Camera->GetIsPerspective();
			s_Data->Camera->SetIsPerspective(true);
			sh->setMat4f("u_ProjMat", s_Data->Camera->GetProjMat());
			s_Data->Camera->SetIsPerspective(isPersp);
			sh->setMat4f("u_ViewMat",
				glm::mat4(glm::mat3(s_Data->Camera->GetViewMat())));
			

			BindTexture(s_Data->skyboxData.SkyboxTex, SKYBOX_TEX_SLOT);
			BindVAO(s_Data->skyboxData.SkyboxVAO);

			glDrawArrays(GL_TRIANGLES, 0, s_Data->skyboxData.SkyboxVAO->Count());
			glDepthFunc(GL_LESS);

		}

		void UpdateLightPosition(const float pos[3], const unsigned lightIndex)
		{
			static unsigned posSize = 3 * sizeof(float);
			int test = sizeof(LightData);
			s_Data->LightUBO->Upload(pos, posSize, lightIndex * SHADER_LIGHT_SIZE);
		}

		void EraseLightDataAt(unsigned index)
		{
			ASSERT(index < MAX_LIGHTS_COUNT, "");
			s_Data->LightDataSubmitted[index].enabled = false;
			/*for (unsigned i = 0; i < s_Data->LightDataSubmitted.data.size(); ++i)
			{
				if (s_Data->LightDataSubmitted.id[i] == index)
					s_Data->LightDataSubmitted.data.erase(s_Data->LightDataSubmitted.data.begin() + i);
			}*/
			
			/*for (unsigned i = index; i < MAX_LIGHTS_COUNT-1; ++i)
			{
				s_Data->LightDataSubmitted[i] = s_Data->LightDataSubmitted[i+1];
			}

			s_Data->LightsCount--;*/
		}

		void SubmitLightData(const LightData& data, unsigned index)
		{
			ASSERT(index < MAX_LIGHTS_COUNT, "");
			s_Data->LightDataSubmitted[index].data = data;
			s_Data->LightDataSubmitted[index].enabled = true;
			/*s_Data->LightDataSubmitted.data.push_back(data);
			s_Data->LightDataSubmitted.id.push_back(index);*/
		}

		LightData GetDefaultLightData(LightType type)
		{
			static glm::vec3 ambient  = glm::vec3(0.1f);
			static glm::vec3 diffuse  = glm::vec3(0.5f);
			static glm::vec3 specular = glm::vec3(1.f);

			static float brightness = 2.f;

			static float constant = 1.f;
			static float linear = 0.09f;
			static float quadratic = 0.032f;

			static float cutOff = 25.f;
			static float outerCutOff = cutOff + 5.f;

			LightData data{};
			//data.enabled     = true;
			data.type	     = type;
			data.brightness  = brightness;
			data.color	     = { 1.f, 1.f, 1.f };
			data.atlasoffset = { 0, 0 };

			data.position   = glm::vec3{0.f};

			data.diffuse    = diffuse;
			data.specular   = specular;

			if (type == LightType::Point || type == LightType::Spot)
			{
				data.ambient   = glm::vec3(0.f);

				data.constant  = constant;
				data.linear    = linear;
				data.quadratic = quadratic;

				if (type == LightType::Spot)
				{
					data.cutOff		 = glm::cos(glm::radians(cutOff));
					data.outerCutOff = glm::cos(glm::radians(outerCutOff));

					data.direction   = { 0.f, 0.f, 1.f };

					glm::vec3 front = data.position + glm::normalize(data.direction);
					glm::mat4 lightView = glm::lookAt(data.position, front, glm::vec3(0.f, 1.f, 0.f));

					data.projViewMat = Renderer::GetSpotLightProjMat() * lightView;
				}
			}
			else if (type == LightType::Directional)
			{
				data.ambient = ambient;

				glm::mat4 lightView = glm::lookAt(data.position,
					glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
				data.projViewMat = Renderer::GetDirLightProjMat() * lightView;
			}
			else
				DEBUG_BREAK("");

			return data;
		}

		unsigned AddNewLight(const LightData& data)
		{
			s_Data->LightsCount++;
			//static unsigned offset = 0;
			static unsigned lightIndex = 0;

			SubmitLightData(data, lightIndex);
			//s_Data->LightUBO->Upload(&data, SHADER_LIGHT_SIZE, offset);

			//offset += SHADER_LIGHT_SIZE;
			++lightIndex;
			return lightIndex - 1;
		}

		glm::vec4 SetOutlineColor(const glm::vec4& color)
		{
			glm::vec4 old = s_Data->outlineColor;
			s_Data->outlineColor = color;
			return old;
		}

		void ClearState()
		{
			s_Data->boundVaoId = 0;
			s_Data->boundShaderId = 0;
			s_Data->TexSlotId.clear();
		}

		void BeginScene(Ref<Camera> cam, bool castShadows) //unsigned lightCount, 
		{
			
			SceneData data = { cam->GetProjViewMat(), cam->Position(), s_Data->LightsCount, castShadows };
			s_Data->Camera = cam;
			s_Data->SceneUBO->Upload((const void*)&data, sizeof(data), 0);
			
			
			/*s_Data->ViewMat = cam->GetViewMat();
			s_Data->ProjMat = cam->GetProjMat();*/

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

		glm::mat4& GetDirLightProjMat()
		{
			static float size = 100.f;
			static glm::mat4 mat = 
				glm::ortho(-size, size, -size, size, DIR_NEAR_PLANE, DIR_FAR_PLANE);
			return mat;
		}

		glm::mat4& GetSpotLightProjMat()
		{
			static float fovy = 90.f;
			static glm::mat4 mat = glm::perspective(
				glm::radians(fovy), 1.f, 
				SPOT_NEAR_PLANE, SPOT_FAR_PLANE);
			return mat;
		}

		void OnImGuiRender()
		{
			ImGui::Begin("Renderer debug info");
			ImVec2 size = { 600, 400 };
			ImVec4 tint_col = { 0.3f, 1, 1, 1 };
			
			ImGui::Image((void*)s_Data->DepthMap->Id(), size,
				ImVec2{ 0, 1 }, ImVec2{ 1, 0 }, tint_col);
			ImGui::End();
		}

		//void ResetViewport()
		//{
		//	glViewport(0, 0, s_Data->viewportWidth, s_Data->viewportHeight);
		//}

		void Shutdown()
		{
			delete s_Data;
		}

		void UploadLightDataToShader()
		{
			static LightData buffer[MAX_LIGHTS_COUNT];
			unsigned i = 0;
			for (auto& [enabled, data] : s_Data->LightDataSubmitted)
			{
				if (enabled)
				{
					buffer[i] = data;
					++i;
				}
			}
			s_Data->LightsCount = i;
			s_Data->LightUBO->UploadFull(buffer);
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

			

			/*void SetUniformBuffer(const Ref<ShaderBlock> ubo, const short slot,
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
			}*/

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
				auto sh = s_Data->Shader[ShaderType::General] = CreateRef<Shader>(
					std::unordered_map<Shader::Type, std::string>{
						{ Shader::Type::VERTEX, "general.vert" },
						{ Shader::Type::FRAGMENT, "general.frag" }});
				//auto sh = s_Data->Shader[ShaderType::General] = CreateRef<Shader>("general.shader");
				sh->Bind();

				sh->setInt("material.diffuseTex", DIFF_TEX_SLOT);
				sh->setInt("material.specularTex", SPEC_TEX_SLOT);
				sh->setInt("material.normalTex", NORM_TEX_SLOT);
				sh->setFloat("material.specularFloat", 0.5f);
				sh->setFloat("material.shininess", 32.0f);
				sh->setFloat4("material.color", {1.f, 0.f, 1.f, 1.f}); //magenta

				sh->setInt("u_SAtlas", DEPTH_TEX_SLOT);
				sh->setInt("u_SAtlasFramesPerRow", SATLAS_DIM.x);
				sh->setInt("u_SFrameSize", SFRAME_SIZE);
				sh->setInt2("u_SAtlasSize", SATLAS_SIZE);
				sh->setFloat("u_PointLightFarPlane", POINT_FAR_PLANE);
				sh->setFloat("u_SpotLightFarPlane", SPOT_FAR_PLANE);
				

				s_Data->Shader[ShaderType::UniformColor] = CreateRef<Shader>("color.shader");

				s_Data->Shader[ShaderType::AttribColor] = CreateRef<Shader>("colorAttrib.shader");

	

				s_Data->Shader[ShaderType::Skybox] = CreateRef<Shader>("skybox.shader");
				s_Data->Shader[ShaderType::Skybox]->Bind();
				s_Data->Shader[ShaderType::Skybox]->setInt("u_SkyboxTex", SKYBOX_TEX_SLOT);

				sh = s_Data->Shader[ShaderType::PointDepth] = CreateRef<Shader>("pointDepth.shader");
				sh->Bind();
				sh->setFloat("u_FarPlane", POINT_FAR_PLANE);

				sh = s_Data->Shader[ShaderType::DirDepth] = CreateRef<Shader>("dirDepth.shader");
				/*sh->Bind();
				sh->setFloat("u_FarPlane", DIR_FAR_PLANE);*/
				sh = s_Data->Shader[ShaderType::SpotDepth] = CreateRef<Shader>("spotDepth.shader");
				sh->Bind();
				sh->setFloat("u_FarPlane", SPOT_FAR_PLANE);
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