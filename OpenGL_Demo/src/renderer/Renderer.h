#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include "renderer/Mesh.h"
#include "Texture.h"
#include "Framebuffer.h"

namespace GLBuffer {
	enum {
		None = -1, Depth = 0x00000100, Stencil = 0x00000400, Color = 0x00004000
	};
}

struct Light
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
	int type;
};

enum class LightType
{
	None = -1, Directional, Point, Spot
};
enum class ShaderType
{
	None = -1, DepthShader, Skybox, UniformColor,
	AttribColor, Diffuse, DiffNSpec, NormalMap
};

namespace Renderer
{
	void Draw(const glm::mat4& modelMat, MeshInstance& mi);
	void DrawOutlined(const glm::mat4& modelMat, MeshInstance& mi, const glm::vec3& modelScale);
	void DrawDepth(const glm::mat4& modelMat, const MeshInstance& mi);
	void ShadowRenderSetup(glm::vec3 lightPos);
	void ShadowRenderEnd();
	
	void DrawSkybox();

	unsigned UploadLightData(const void* data);
	void UpdateLightPosition(const float pos[3], const unsigned lightIndex);

	void Init(unsigned width, unsigned height);
	void ClearState();
	
	unsigned GetFBColorAttachmentID();
	void SetRenderImageSize(const unsigned width, const unsigned height);
	void BeginScene(Ref<Camera> cam, unsigned lightCount, bool castShadows);
	void EndScene();
	void Shutdown();

	void Clear(int mode);
	void SetClearColor(float r, float g, float b, float a);
	void ResetViewport();
};