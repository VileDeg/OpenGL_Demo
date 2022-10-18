#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include "renderer/Mesh.h"
#include "Texture.h"
#include "Framebuffer.h"

namespace Crave
{
	namespace GLBuffer {
		enum {
			None = -1, Depth = 0x00000100, Stencil = 0x00000400, Color = 0x00004000
		};
	}

	enum class LightType 
	{
		None = -1, Directional, Point, Spot
	};


	struct LightData
	{
		glm::vec3 position;
		float constant;
		glm::vec3 direction;
		float linear;
		glm::vec3 ambient;
		float quadratic;
		glm::vec3 diffuse;
		float cutOff;
		glm::vec3 specular;
		float outerCutOff;
		glm::mat4 projViewMat;
		glm::vec3 color;
		float brightness;
		LightType type; //alignas(8) 
		int mipmaplevel;
		glm::ivec2 atlasoffset;
	};
	
	enum class ShaderType
	{
		None = -1, General, PointDepth, DirDepth, SpotDepth, Skybox, UniformColor,
		AttribColor, Diffuse, DiffNSpec, NormalMap
	};

	namespace Renderer
	{
		void DrawMesh(int drawID, const glm::mat4& modelMat, Ref<Mesh> mesh,
			bool withTextures, glm::vec4 color = { 1.f, 0.f, 1.f, 1.f });

		void DrawOutlined(int drawID, const glm::mat4& modelMat, Ref<Mesh> mesh,
			bool withTextures, glm::vec4 color = { 1.f, 0.f, 1.f, 1.f });
		void DrawDepth(const glm::mat4& modelMat, Ref<Mesh> mesh, ShaderType shType);

		void RenderLigthDepthToAtlas(std::function<void(ShaderType)> renderDepthFunc);
		
		void DrawSkybox();

		LightData GetDefaultLightData(LightType type);
		unsigned AddNewLight(const LightData& data);

		void SubmitLightData(const LightData& data, unsigned index);
		//void UpdateLightPosition(const float pos[3], const unsigned lightIndex);

		void Init(Ref<Framebuffer> viewportfb, unsigned width, unsigned height);
		void ClearState();

		void BeginScene(Ref<Camera> cam, bool castShadows);
		void EndScene();
		void Shutdown();

		glm::vec4 SetOutlineColor(const glm::vec4& color);

		void Clear(int mode);
		void SetClearColor(float r, float g, float b, float a);
		
		glm::mat4& GetDirLightProjMat();
		glm::mat4& GetSpotLightProjMat();

		void OnImGuiRender();

		/*constexpr const int LIGHT_MIN_RADIUS = 1.f;
		constexpr const int LIGHT_MAX_RADIUS = 15.f;*/

		constexpr const int LIGHT_MAX_BRIGHTNESS = 10.f;
	};
}