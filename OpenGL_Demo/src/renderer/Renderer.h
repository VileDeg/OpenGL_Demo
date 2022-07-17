#pragma once

#include "math_headers.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include "renderer/Mesh.h"
#include "Texture.h"
#include "Framebuffer.h"

enum class LightType
{
	None = -1, Directional, Point, Spot
};
enum class ShaderType
{
	None = -1, Skybox, UniformColor, AttribColor, Diffuse, DiffNSpec, DepthShader
};

class Renderer
{
private:
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
		Ref<Framebuffer> DepthMapFBO;
		Ref<Texture> DepthMap;
		
		glm::mat4 ViewMat = glm::mat4(1.f);
		glm::mat4 ProjMat = glm::mat4(1.f);
		
		std::unordered_map<unsigned, unsigned> TexSlotId;
		unsigned LightsCount;
		unsigned boundVaoId;
		unsigned boundShaderId;
		unsigned viewportWidth;
		unsigned viewportHeight;
		SkyboxData skyboxData;
	};

public:
	static void SetUniformBuffer(const Ref<ShaderBlock> ssbo, const short slot,
		std::vector<ShaderType> shTypes);
	static void SetShaderStorageBuffer(const Ref<ShaderBlock> ssbo, const short slot,
		std::vector<ShaderType> shTypes);

	static void Draw(const glm::mat4& modelMat, MeshInstance& mi);
	static void DrawDepth(const glm::mat4& modelMat, const MeshInstance& mi);
	static void ShadowRenderSetup(glm::vec3 lightPos);
	static void ShadowRenderEnd();
	
	static void DrawSkybox();

	static const unsigned UploadLightData(const void* data);
	static void UpdateLightPosition(const float pos[3], const unsigned lightIndex);

	static void Init(unsigned width, unsigned height);
	static void LoadShaders();
	static void CreateSkybox();

	static void BeginScene(const Camera& cam, unsigned lightCount, bool castShadows);
	static void EndScene();
	static void Shutdown();

	
	static bool BindShader(const Ref<Shader> shader);
	static void BindVAO(const Ref<VAO> vao);
	static void BindTexture(const Ref<Texture> tex, const short slot);

	static void Clear(std::bitset<3> bufferBits);
	static void SetClearColor(float r, float g, float b, float a);
	static void ResetViewport();
private:
	static void GLDraw(const Ref<VAO> vao);
private:
	static RenderData* s_Data;

	static constexpr const unsigned SSBO_DIRLIGHT_OFFSET   = 0;
	static constexpr const unsigned SSBO_SPOTLIGHT_OFFSET  = SSBO_DIRLIGHT_OFFSET + 64;
	static constexpr const unsigned SSBO_POINTLIGHT_OFFSET = SSBO_SPOTLIGHT_OFFSET + 80;

	static constexpr const unsigned SSBO_POINTLIGHT_SIZE = 16*4;

	static constexpr const unsigned SSBO_LIGHT_SIZE = 96;

	static constexpr const unsigned SHADOW_MAP_WIDTH = 1024;
	static constexpr const unsigned SHADOW_MAP_HEIGHT = 1024;
};