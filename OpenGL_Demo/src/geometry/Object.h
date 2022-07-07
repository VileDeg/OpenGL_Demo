#pragma once
#include "Transform.h"
#include "renderer/Buffer.h"

#include "renderer/Texture.h"

#include "GeoData.h"

class Object : public Transform
{
public:
	//struct TexturePack
	//{
	//	const char* diffuse;
	//	const char* specular;
	//	//const char* normal;
	//};
	enum class ObjectType
	{
		NO_TEXTURES=-1, DIFFUSE_ONLY=0, BOTH_TEXTURES=1
	};
public:
	Object(Primitive primType,
		const char* diffuse, const char* specular,
		//Shader shader,
		const glm::vec3 pos = glm::vec3(0.0f),
		const float rotAngle = 0.0f,
		const glm::vec3 rotAxis = Transform::WORLD_UP,
		const float scale = 1.0f)
		: Transform(pos, rotAngle, rotAxis, scale),
		m_Data(GeoData::GetData(primType)),
		m_VAO(std::make_shared<VAO>()),
		m_VBO(std::make_shared<VBO>(m_Data.first, m_Data.second, 36)),
		m_DiffuseTex (std::make_shared<Texture>(diffuse )),
		m_SpecularTex(std::make_shared<Texture>(specular))
	{
		Init(ObjectType::BOTH_TEXTURES);
	}
	
	Object(Primitive primType,
		const char* diffuse, 
		//Shader shader,
		const glm::vec3 pos = glm::vec3(0.0f),
		const float rotAngle = 0.0f,
		const glm::vec3 rotAxis = Transform::WORLD_UP,
		const float scale = 1.0f)
		: Transform(pos, rotAngle, rotAxis, scale),
		m_Data(GeoData::GetData(primType)),
		m_VAO(std::make_shared<VAO>()),
		m_VBO(std::make_shared<VBO>(m_Data.first, m_Data.second, 36)),
		m_DiffuseTex(std::make_shared<Texture>(diffuse)),
		m_SpecularTex(nullptr)
	{
		Init(ObjectType::DIFFUSE_ONLY);
	}
	Object(Primitive primType,
		//Shader shader,
		const glm::vec3 pos = glm::vec3(0.0f),
		const float rotAngle = 0.0f,
		const glm::vec3 rotAxis = Transform::WORLD_UP,
		const float scale = 1.0f)
		: Transform(pos, rotAngle, rotAxis, scale),
		m_Data(GeoData::GetData(primType)),
		m_VAO(std::make_shared<VAO>()),
		m_VBO(std::make_shared<VBO>(m_Data.first, m_Data.second, 36)),
		m_DiffuseTex(nullptr),
		m_SpecularTex(nullptr)
	{
		Init(ObjectType::NO_TEXTURES);
	}

	/*void SetModelMat(const glm::mat4 modelMat);
	void SetViewMat(const glm::mat4 viewMat);
	void SetProjMat(const glm::mat4 projMat);

	void WatchedBy(const Camera& camera);*/
	void DrawColor(const glm::vec4& color);
	void Draw();
	
	//inline Shader& GetShader() { return *m_Shader; }

	~Object() {}
private:
	void Init(ObjectType type);
private:
	std::pair<const float*, std::size_t> m_Data;
	//unsigned int vao, vbo;
	//std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<VAO> m_VAO;
	std::shared_ptr<VBO> m_VBO;
	//std::unique_ptr<EBO> m_EBO;

	std::shared_ptr<Texture> m_DiffuseTex;
	std::shared_ptr<Texture> m_SpecularTex;

	

	

	
};
