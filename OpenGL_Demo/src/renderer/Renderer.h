#pragma once

#include "math_headers.h"
#include "Buffer.h"

#include "Shader.h"
#include "Camera.h"
#include "geometry/GeoData.h"
#include "Texture.h"
#include "Light.h"
#include "Mesh.h"
#include "Framebuffer.h"


enum class ShaderType
{
	None = -1, Skybox, Color, Diffuse, DiffNSpec, DirDepth, SpotDepth, PointDepth
};

//struct SceneData
//{
//	glm::mat4 CameraProjMat;
//	glm::mat4 CameraViewMat;
//};

class Renderer
{

private:
	struct SkyboxData
	{
		Ref<VAO> SkyboxVAO;
		Ref<VBO> SkyboxVBO;
		Ref<Texture> SkyboxTex;
	};

	//static constexpr const unsigned MAX_LIGHTS_COUNT = 8;
	static constexpr const int MAX_POINTLIGHTS_COUNT   = 4;
	static constexpr const int MAX_DIRNSPOTLIGHTS_COUNT = 8;
	//static constexpr const int MAX_SPOTLIGHTS_COUNT = 4;
	struct RenderData
	{
		std::unordered_map<ShaderType, Ref<Shader>> Shader;

		Ref<ShaderBlock> SceneUBO;
		Ref<ShaderBlock> LightSSBO;
		Ref<Framebuffer> DepthMapFBO;
		Ref<Texture>     DepthMap [MAX_LIGHTS_COUNT];
		glm::vec3        LightsPos[MAX_LIGHTS_COUNT];
		const float      lightFarPlane{25.f};
		
		glm::mat4 ViewMat = glm::mat4(1.f);
		glm::mat4 ProjMat = glm::mat4(1.f);
		
		std::unordered_map<unsigned, unsigned> TexSlotId;
		unsigned lightsCount{0};
		unsigned boundVaoId;
		unsigned boundShaderId;
		unsigned viewportWidth;
		unsigned viewportHeight;
		SkyboxData skyboxData;
	};

public:
	/*static void BindShaderBlock(const Ref<ShaderBlock> block, const short slot,
		std::vector<ShaderType> shTypes);*/
	static void SetUniformBuffer(const Ref<ShaderBlock> ssbo, const short slot,
		std::vector<ShaderType> shTypes);
	static void SetShaderStorageBuffer(const Ref<ShaderBlock> ssbo, const short slot,
		std::vector<ShaderType> shTypes);

	//static void Submit(const glm::mat4& modelMat, const Ref<VAO> vao, const glm::vec4& color);
	static void Draw(const glm::mat4& modelMat, const Ref<VAO> vao, const glm::vec4& color);
	static void Draw(const glm::mat4& modelMat, const Ref<VAO> vao, const Ref<Texture> diffuse);
	static void Draw(const glm::mat4& modelMat, const Ref<VAO> vao, const Ref<Texture> diffuse, const Ref<Texture> specular);
	static void DrawDepth(const glm::mat4& modelMat, const Ref<VAO> vao);
	static void DrawDepthInside(const glm::mat4& modelMat, const Ref<VAO> vao);
	static void DrawInside(const glm::mat4& modelMat, const Ref<VAO> vao,
		const Ref<Texture> diffuse, const Ref<Texture> specular);
	
	//static void RenderShadowMap();
	static void PointDepthRenderSetup(glm::vec3 lightPos, int lightIndex);
	static void DirDepthRenderSetup  (glm::vec3 lightPos, int lightIndex);
	static void SpotDepthRenderSetup (glm::vec3 lightPos, int lightIndex);
	static void ShadowRenderEnd();
		
	static void DrawSkybox();

	static const unsigned UploadLightData(const Light* data, const LightType type);
	static void UpdateLightPosition(const glm::vec3& pos, const unsigned lightIndex);

	static void Init(unsigned width, unsigned height);
	static void LoadShaders();
	static void CreateSkybox();

	static void Clear(std::bitset<3> bufferBits);
	static void SetClearColor(float r, float g, float b, float a);
	static void ResetViewport();

	static void BeginScene(const Camera& camera);
	static void EndScene();
	static void Shutdown();

	static bool BindShader(const Ref<Shader> shader);
	static void BindVAO(const Ref<VAO> vao);
	static void BindTexture(const Ref<Texture> tex, const short slot);
private:
	static RenderData* s_Data;

	static constexpr const unsigned SSBO_DIRLIGHT_OFFSET   = 0;
	static constexpr const unsigned SSBO_SPOTLIGHT_OFFSET  = SSBO_DIRLIGHT_OFFSET + 64;
	static constexpr const unsigned SSBO_POINTLIGHT_OFFSET = SSBO_SPOTLIGHT_OFFSET + 80;

	static constexpr const unsigned SSBO_POINTLIGHT_SIZE = 16*4;

	static constexpr const unsigned SSBO_LIGHT_SIZE = 96;

	static constexpr const unsigned SHADOW_MAP_WIDTH = 1024;
	static constexpr const unsigned SHADOW_MAP_HEIGHT = 1024;

	static constexpr short DIFF_TEX_SLOT     = 0;
	static constexpr short SPEC_TEX_SLOT     = 1;
	static constexpr short SKYBOX_TEX_SLOT   = 2;
	static constexpr short DEPTHMAP_SLOT     = 3;
	static constexpr short DEPTHCUBEMAP_SLOT = 15;

	/*static constexpr const char* DIFFUSE_TEX_UNIFORM_NAME = "material.diffuse";
	static constexpr const char* SPECULAR_TEX_UNIFORM_NAME = "material.specular";
	static constexpr const char* NORMAL_TEX_UNIFORM_NAME = "material.normal";

	static constexpr const char* MODEL_MAT_UNIFORM_NAME = "u_ModelMat";
	static constexpr const char* PROJ_MAT_UNIFORM_NAME = "u_ProjMat";
	static constexpr const char* VIEW_MAT_UNIFORM_NAME = "u_ViewMat";*/
};