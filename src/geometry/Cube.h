#pragma once
#include "Transform.h"
#include "Buffer.h"
#include "shader/Shader.h"
#include "Texture.h"
#include "Camera.h"

class Cube : public Transform
{
public:
	//struct TexturePack
	//{
	//	const char* diffuse;
	//	const char* specular;
	//	//const char* normal;
	//};
	enum class CubeType
	{
		NO_TEXTURES=-1, DIFFUSE_ONLY=0, BOTH_TEXTURES=1
	};
public:
	Cube(const char* diffuse, const char* specular,
		Shader shader,
		const glm::vec3 pos = glm::vec3(0.0f),
		const float rotAngle = 0.0f,
		const glm::vec3 rotAxis = Transform::WORLD_UP,
		const float scale = 1.0f)
		: Transform(pos, rotAngle, rotAxis, scale),
		m_Shader(std::make_unique<Shader>(shader)),
		m_VAO(std::make_unique<VAO>()),
		m_VBO(std::make_unique<VBO>(DATA, sizeof(DATA), 36)),
		m_DiffuseTex (std::make_unique<Texture>(diffuse )),
		m_SpecularTex(std::make_unique<Texture>(specular))
	{
		Init(CubeType::BOTH_TEXTURES);
	}
	
	Cube(const char* diffuse, Shader shader,
		const glm::vec3 pos = glm::vec3(0.0f),
		const float rotAngle = 0.0f,
		const glm::vec3 rotAxis = Transform::WORLD_UP,
		const float scale = 1.0f)
		: Transform(pos, rotAngle, rotAxis, scale),
		m_Shader(std::make_unique<Shader>(shader)),
		m_VAO(std::make_unique<VAO>()),
		m_VBO(std::make_unique<VBO>(DATA, sizeof(DATA), 36)),
		m_DiffuseTex(std::make_unique<Texture>(diffuse)),
		m_SpecularTex(nullptr)
	{
		Init(CubeType::DIFFUSE_ONLY);
	}
	Cube(Shader shader,
		const glm::vec3 pos = glm::vec3(0.0f),
		const float rotAngle = 0.0f,
		const glm::vec3 rotAxis = Transform::WORLD_UP,
		const float scale = 1.0f)
		: Transform(pos, rotAngle, rotAxis, scale),
		m_Shader(std::make_unique<Shader>(shader)),
		m_VAO(std::make_unique<VAO>()),
		m_VBO(std::make_unique<VBO>(DATA, sizeof(DATA), 36)),
		m_DiffuseTex(nullptr),
		m_SpecularTex(nullptr)
	{
		Init(CubeType::NO_TEXTURES);
	}

	void SetModelMat(const glm::mat4 modelMat);
	void SetViewMat(const glm::mat4 viewMat);
	void SetProjMat(const glm::mat4 projMat);

	void WatchedBy(const Camera& camera);

	
	void Draw();
	
	inline Shader& GetShader() { return *m_Shader; }

	~Cube() {}
private:
	void Init(CubeType type);

	unsigned int vao, vbo;
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<VAO> m_VAO;
	std::unique_ptr<VBO> m_VBO;
	//std::unique_ptr<EBO> m_EBO;



	std::unique_ptr<Texture> m_DiffuseTex;
	std::unique_ptr<Texture> m_SpecularTex;

	static constexpr const char* DIFFUSE_TEX_UNIFORM_NAME  = "material.diffuse";
	static constexpr const char* SPECULAR_TEX_UNIFORM_NAME = "material.specular";
	static constexpr const char* NORMAL_TEX_UNIFORM_NAME   = "material.normal";

	static constexpr const char* MODEL_MAT_UNIFORM_NAME    = "u_ModelMat";
	static constexpr const char* PROJ_MAT_UNIFORM_NAME     = "u_ProjMat";
	static constexpr const char* VIEW_MAT_UNIFORM_NAME     = "u_ViewMat";


	static constexpr const float DATA[] =
	{
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	
};

