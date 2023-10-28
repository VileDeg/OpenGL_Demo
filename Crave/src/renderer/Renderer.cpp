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

			constexpr const int   MAX_LIGHTS_COUNT = 16; //6
			constexpr const int   MAX_SFRAME_MIPMAP_LEVEL = 5;
			constexpr const float SFRAME_MIPMAP_DISTANCE_STEP = 40.f;

			constexpr const float POINT_NEAR_PLANE = 0.1f;
			constexpr const float POINT_FAR_PLANE = 250.f;
			constexpr const float SPOT_NEAR_PLANE = 0.1f;
			constexpr const float SPOT_FAR_PLANE = 250.f;
			constexpr const float DIR_NEAR_PLANE = -500.f;
			constexpr const float DIR_FAR_PLANE = 500.f;
			constexpr const int	  SHADER_LIGHT_SIZE = 176;


			constexpr const int		   SFRAME_SIZE = 1024;
			constexpr const glm::ivec2 SATLAS_DIM = { 10, 10 };
			constexpr const glm::ivec2 SATLAS_SIZE = SATLAS_DIM * SFRAME_SIZE;

			struct RenderData
			{
				std::unordered_map<ShaderType, Ref<Shader>> Shader;
				Ref<ShaderBlock> SceneUBO;
				Ref<ShaderBlock> LightUBO;

				int FramesFilledByLevel[MAX_SFRAME_MIPMAP_LEVEL]{};
				
				glm::ivec2 NextSAtlasOffset{};
				std::vector<int> LightIndexByDistance[MAX_SFRAME_MIPMAP_LEVEL];
				std::vector<LightData> LightDataSubmitted{};
				
				Ref<Framebuffer> ViewportFB;
				Ref<Framebuffer> DepthMapFBO;
				Ref<Texture> DepthMap;
				Ref<Camera> Camera;

				std::unordered_map<unsigned, unsigned> TexSlotId;

				unsigned LightsCount;
				unsigned boundVaoId;
				unsigned boundShaderId;
				unsigned viewportWidth;
				unsigned viewportHeight;
				SkyboxData skyboxData;
				float outlineBorderScale = 0.1f;
				float outlineBrightness = 1.f;
				glm::vec4 outlineColor = glm::vec4(glm::vec3(242, 140, 40) / 256.f * outlineBrightness, 1); //bright orange
			};

			RenderData* s_Data = nullptr;

			constexpr short DIFF_TEX_SLOT = 0;
			constexpr short SPEC_TEX_SLOT = 1;
			constexpr short NORM_TEX_SLOT = 2;
			constexpr short SKYBOX_TEX_SLOT = 7;
			constexpr short DEPTH_TEX_SLOT = 8;

			void LoadShaders();
			void CreateSkybox();
			void GLDraw(const Ref<VAO> vao);

			glm::ivec2 GetNextOffsetInAtlas();
			glm::ivec2 GetNextOffsetInAtlasMipmap(int level, int& framesize);

			void DepthRenderSetup();
			void SortLightsByDistance();
			ShaderType ShadowSetupByLightType(LightData& data, int frameNum, int level);

			void SpotShadowSetup(LightData& data, int frameNum, int mipmapLevel);
			void DirShadowSetup(LightData& data, int frameNum, int mipmapLevel);
			void PointShadowSetup(LightData& data, int frameNum, int level);
			void DepthRenderEnd();

			void UploadLightDataToShader();

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
					BindTexture(diff[0], DIFF_TEX_SLOT);
					BindTexture(norm[0], NORM_TEX_SLOT);
					sh->setUint("u_ObjType", 2);
				}
				else if (!diff.empty() && !spec.empty())
				{
					BindTexture(diff[0], DIFF_TEX_SLOT);
					BindTexture(spec[0], SPEC_TEX_SLOT);
					sh->setUint("u_ObjType", 1);
				}
				else if (!diff.empty())
				{
				
					BindTexture(diff[0], DIFF_TEX_SLOT);
					sh->setUint("u_ObjType", 0);
				}
				else
				{
					//Mesh has no textues. Most likely error.
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

		void RenderLigthDepthToAtlas(std::function<void(ShaderType)> renderDepthFunc)
		{
			DepthRenderSetup();

			SortLightsByDistance();
			
			for (int lv = 0; lv < MAX_SFRAME_MIPMAP_LEVEL; ++lv)
			{
				auto& libd = s_Data->LightIndexByDistance[lv];
				for (int i = 0; i < libd.size(); ++i)
				{
					auto& data = s_Data->LightDataSubmitted[libd[i]];
					ShaderType shType = ShadowSetupByLightType(data, i, lv);
					renderDepthFunc(shType);
				}
			}

			UploadLightDataToShader();
			DepthRenderEnd();
		}

		void DrawSkybox()
		{
			glDepthFunc(GL_LEQUAL);
			Ref<Shader> sh = s_Data->Shader[ShaderType::Skybox];
			BindShader(sh);
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

		void SubmitLightData(const LightData& data, unsigned index)
		{
			s_Data->LightDataSubmitted.push_back(data);
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
			data.type	     = type;
			data.brightness  = brightness;
			data.color	     = { 1.f, 1.f, 1.f };
			data.atlasoffset = { 0, 0 };
			data.mipmaplevel = 0;

			data.position   = glm::vec3{0.f};

			data.diffuse    = diffuse;
			data.specular   = specular;

			if (type == LightType::Point || type == LightType::Spot)
			{
				data.ambient   = ambient;

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
			static unsigned lightIndex = 0;

			SubmitLightData(data, lightIndex);
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
			s_Data->LightDataSubmitted.clear();
			for (int i = 0; i < MAX_SFRAME_MIPMAP_LEVEL; ++i)
			{
				s_Data->LightIndexByDistance[i].clear();
				s_Data->FramesFilledByLevel[i] = 0;
			}
			
			s_Data->NextSAtlasOffset = { 0, 0 };
			
			SceneData data = { cam->GetProjViewMat(), cam->Position(), s_Data->LightsCount, castShadows };
			s_Data->Camera = cam;
			s_Data->SceneUBO->Upload((const void*)&data, sizeof(data), 0);
			
			s_Data->ViewportFB->Bind();
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
			static float scale = 0.1f / (SFRAME_SIZE / 1024) / (SATLAS_DIM.x / 4);
			ImVec2 size = { SATLAS_SIZE.x * scale, SATLAS_SIZE.y * scale };

			ImVec4 tint_col = { 1.f, 1, 1, 1 };
			
			ImGui::Image((void*)s_Data->DepthMap->Id(), size,
				ImVec2{ 0, 1 }, ImVec2{ 1, 0 }, tint_col);

			ImGui::Text("LightDataSubmitted: %ld", s_Data->LightDataSubmitted.size());
			ImGui::Separator();
			for (int lv = 0; lv < MAX_SFRAME_MIPMAP_LEVEL; ++lv)
			{
				auto& libd = s_Data->LightIndexByDistance[lv];
				ImGui::Text("Mipmap level %d: %ld", lv, libd.size());
			}

			ImGui::End();
		}

		void Shutdown()
		{
			delete s_Data;
		}

		
		namespace //private
		{
			
			glm::ivec2 GetNextOffsetInAtlas()
			{
				glm::ivec2 prev = s_Data->NextSAtlasOffset;
				glm::ivec2& offset = s_Data->NextSAtlasOffset;
				offset = { (offset.x + SFRAME_SIZE) % SATLAS_SIZE.x,
					offset.y + (offset.x + SFRAME_SIZE) / SATLAS_SIZE.x * SFRAME_SIZE };
				return prev;
			}

			glm::ivec2 GetNextOffsetInAtlasMipmap(int level, int& framesize)
			{
				glm::ivec2 prev = s_Data->NextSAtlasOffset;
				glm::ivec2& offset = s_Data->NextSAtlasOffset;
				if (level == 0)
				{
					offset = { (offset.x + SFRAME_SIZE) % SATLAS_SIZE.x,
						offset.y + (offset.x + SFRAME_SIZE) / SATLAS_SIZE.x * SFRAME_SIZE };
					framesize = SFRAME_SIZE;
					return prev;
				}
					
				int upframesize = SFRAME_SIZE / std::pow(2, level - 1);
				framesize = upframesize / 2;
				int numofframes = std::pow(4, level);
				int cellsizex = upframesize;

				int remx = offset.x % SFRAME_SIZE;
				int remy = offset.y % SFRAME_SIZE;
				int basex = offset.x - remx;
				int basey = offset.y - remy;
				int addx = (remx + framesize) % SFRAME_SIZE + (remy + framesize) / SFRAME_SIZE * (remx + framesize) / SFRAME_SIZE * SFRAME_SIZE;
				int addy = (remy + (remx + framesize) / SFRAME_SIZE * framesize) % SFRAME_SIZE;
				offset = { (basex + addx) % SATLAS_SIZE.x,
						basey + addy + (basex + addx) / SATLAS_SIZE.x * SFRAME_SIZE };
				

				return prev;
			}
		

			void DepthRenderSetup()
			{
				s_Data->DepthMapFBO->Bind();
				glClear(GL_DEPTH_BUFFER_BIT);
			}

			void SpotShadowSetup(LightData& data, int frameNum, int mipmapLevel)
			{
				int framesize{};
				glm::ivec2 offset = GetNextOffsetInAtlasMipmap(mipmapLevel, framesize);
				data.atlasoffset = offset;
				glViewport(offset.x, offset.y, framesize, framesize);

				Ref<Shader> sh = s_Data->Shader[ShaderType::SpotDepth];
				BindShader(sh);
				sh->setMat4f("u_LightSpaceMat", data.projViewMat);
			}

			void DirShadowSetup(LightData& data, int frameNum, int mipmapLevel)
			{
				glm::ivec2 offset = GetNextOffsetInAtlas();
				data.atlasoffset = offset;
				glViewport(offset.x, offset.y, SFRAME_SIZE, SFRAME_SIZE);

				Ref<Shader> sh = s_Data->Shader[ShaderType::DirDepth];
				BindShader(sh);
				sh->setMat4f("u_LightSpaceMat", data.projViewMat);
			}

			void PointShadowSetup(LightData& data, int frameNum, int mipmapLevel)
			{

				glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f),
					1.f, POINT_NEAR_PLANE, POINT_FAR_PLANE);
				std::vector<glm::mat4> shadowTransforms;
				shadowTransforms.push_back(shadowProj * glm::lookAt(data.position, data.position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
				shadowTransforms.push_back(shadowProj * glm::lookAt(data.position, data.position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
				shadowTransforms.push_back(shadowProj * glm::lookAt(data.position, data.position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
				shadowTransforms.push_back(shadowProj * glm::lookAt(data.position, data.position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
				shadowTransforms.push_back(shadowProj * glm::lookAt(data.position, data.position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
				shadowTransforms.push_back(shadowProj * glm::lookAt(data.position, data.position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

				int framesize{};
				glm::ivec2 offset = GetNextOffsetInAtlasMipmap(mipmapLevel, framesize);
				data.atlasoffset = offset;
				glViewportIndexedf(0, offset.x, offset.y, framesize, framesize);
				for (int i = 1; i < 6; ++i)
				{
					offset = GetNextOffsetInAtlasMipmap(mipmapLevel, framesize);
					glViewportIndexedf(i, offset.x, offset.y, framesize, framesize);
				}

				auto sh = s_Data->Shader[ShaderType::PointDepth];
				BindShader(sh);
				for (unsigned int i = 0; i < 6; ++i)
					sh->setMat4f("u_ShadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);


				sh->setFloat3("u_LightPos", data.position);
			}

			ShaderType ShadowSetupByLightType(LightData& data, int frameNum, int mipmapLevel)
			{
				data.mipmaplevel = mipmapLevel;
				ShaderType shType;
				switch (data.type)
				{
				case LightType::Point:
					PointShadowSetup(data, frameNum, mipmapLevel); //light.ShaderIndex
					shType = ShaderType::PointDepth;
					break;
				case LightType::Spot:
					SpotShadowSetup(data, frameNum, mipmapLevel);
					shType = ShaderType::SpotDepth;
					break;
				case LightType::Directional:
					DirShadowSetup(data, frameNum, mipmapLevel);
					shType = ShaderType::DirDepth;
					break;
				default:
					ASSERT(false, "");
				}
				return shType;
			}

			void SortLightsByDistance()
			{
				for (int i = 0; i < s_Data->LightDataSubmitted.size(); ++i)
				{
					auto& ld = s_Data->LightDataSubmitted[i];
					if (ld.type == LightType::Directional)
					{
						//Directional light's distance makes no sense. It will always have mipmap level 0.
						s_Data->LightIndexByDistance[0].push_back(i);
						continue;
					}
					float distToCam = glm::length(s_Data->Camera->Position() - ld.position);
					int level = distToCam / SFRAME_MIPMAP_DISTANCE_STEP + 0.5f;

					int maxlv = MAX_SFRAME_MIPMAP_LEVEL - 1;
					level = level > maxlv ? maxlv : level;


					s_Data->LightIndexByDistance[level].push_back(i);
				}
			}

			void DepthRenderEnd()
			{
				s_Data->ViewportFB->Bind();
			}

			void UploadLightDataToShader()
			{
				size_t size = s_Data->LightDataSubmitted.size() * SHADER_LIGHT_SIZE;
				const void* data = s_Data->LightDataSubmitted.data();
				s_Data->LightUBO->Upload(data, size, 0);
			}



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