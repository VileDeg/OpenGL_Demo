#pragma once

#include "Buffer.h"

#include "Shader.h"
#include "Camera.h"
#include "geometry/GeoData.h"
#include "Texture.h"
#include "math_headers.h"

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

class Renderer
{
	enum class ShaderType
	{
		None=-1, Skybox, Color, Diffuse, DiffNSpec
	};

	/*enum class TextureType
	{
		None=-1, Diffuse, Specular, Skybox
	};*/
	
	struct SkyboxData
	{
		Ref<VAO> SkyboxVAO;
		Ref<VBO> SkyboxVBO;
		Ref<Texture> SkyboxTex;
	};

	struct RenderData
	{
		std::unordered_map<ShaderType, Ref<Shader>> Shader;
		//glm::mat4 ViewProjMat = glm::mat4(1.f);
		glm::mat4 ViewMat = glm::mat4(1.f);
		glm::mat4 ProjMat = glm::mat4(1.f);
		//std::vector<ShaderType> ShaderQueue;
		//std::vector<std::function<void(void)>> DrawCalls;
		//unsigned NextTexSlot;
		std::unordered_map<unsigned, unsigned> TexSlotId; //(key,value) = (slot, id)
		unsigned boundVaoId;
		unsigned boundShaderId;
		SkyboxData skyboxData;
		/*RenderData(const std::string& folderName, const std::vector<std::string> faces)
			: SkyboxTex(CreateRef<Texture>(folderName, faces))
		{}*/
	};

public:
	static void SetUniformBuffer(const Ref<UBO> ubo, const short slot);

	//static void Submit(const glm::mat4& modelMat, const Ref<VAO> vao, const glm::vec4& color);
	static void Draw(const glm::mat4& modelMat, const Ref<VAO> vao, const glm::vec4& color);
	static void Draw(const glm::mat4& modelMat, const Ref<VAO> vao, const Ref<Texture> diffuse);
	static void Draw(const glm::mat4& modelMat, const Ref<VAO> vao, const Ref<Texture> diffuse, const Ref<Texture> specular);
	
	static void DrawSkybox();

	static void BindShader(const Ref<Shader> shader);
	static void BindVAO(const Ref<VAO> vao);
	static void BindTexture(const Ref<Texture> tex, const short slot);

	static void Init();
	static void BeginScene(const Camera& camera);
	static void EndScene();
	static void Shutdown();
private:
	static RenderData* s_Data;



	/*static constexpr const char* DIFFUSE_TEX_UNIFORM_NAME = "material.diffuse";
	static constexpr const char* SPECULAR_TEX_UNIFORM_NAME = "material.specular";
	static constexpr const char* NORMAL_TEX_UNIFORM_NAME = "material.normal";

	static constexpr const char* MODEL_MAT_UNIFORM_NAME = "u_ModelMat";
	static constexpr const char* PROJ_MAT_UNIFORM_NAME = "u_ProjMat";
	static constexpr const char* VIEW_MAT_UNIFORM_NAME = "u_ViewMat";*/
};