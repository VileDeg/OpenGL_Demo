#pragma once

#include "pch.h"
#include "VAO.h"
#include "shader/Shader.h"
#include "math_headers.h"
#include "Texture.h"
#include "Renderer.h"
#include "Camera.h"

class Object
{
public:
	enum class ObjectType
	{
		NO_TDATA = -1, DEFAULT = 0
	};
	struct VertexData
	{
		const void* data;
		std::size_t size;
		unsigned vertexCount;
		unsigned char vertexCoordsCount;
		unsigned char normalCoordsCount;
		unsigned char colorCoordsCount;
		unsigned char textureCoordsCount;
	};
	struct TextureData
	{
		const char* diffusePath;
		const char* specularPath;
	};

public:
	/*Object(const VertexData& vData, const unsigned* indices,
		std::size_t indices_size, const std::vector<Texture>& textures,
		const char* vertShaderPath, const char* fragShaderPath,
		const glm::vec3& position = glm::vec3(0.0f) );*/
	
	Object(const VertexData& vData, 
		const Shader shader, const TextureData& tData = {"@#none#@", "@#none#@"},
		const glm::vec3 position = glm::vec3(0.0f),
		ObjectType type = ObjectType::DEFAULT);


	~Object() {}

	inline void MoveBy(glm::vec3 by) { m_Model = glm::translate(m_Model, by); }
	inline void MoveTo(glm::vec3 to) { m_Model = glm::translate(glm::mat4(1.0f), to); }
	inline void Rotate(float angle, glm::vec3 axis) { m_Model = glm::rotate(m_Model, angle, axis); }
	inline void Scale(glm::vec3 by) { m_Model = glm::scale(m_Model, by); }


	void SetProjMat(const glm::mat4 projMat);
	void SetViewMat(const glm::mat4 viewMat);

	void WatchedBy(const Camera& camera);

	//void Draw();
	void DrawNoIndex();
	inline void ResetTransform() { m_Model = glm::mat4(1.0f); };
	inline const glm::vec3& Position() { return m_Model[3]; }

	inline Shader& GetShader() { return *m_Shader; }
	
private:
	void SetModelMat(const glm::mat4 modelMat);

	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<VAO> m_VAO;
	std::unique_ptr<VBO> m_VBO;
	std::unique_ptr<EBO> m_EBO;

	//std::vector< std::unique_ptr<Texture> > m_Textures;
	std::unique_ptr<Texture> m_DiffuseTex;
	std::unique_ptr<Texture> m_SpecularTex;

	glm::mat4 m_Model;
	
	int m_TextureSlot;
	Renderer m_Renderer;

	static constexpr const char* DIFFUSE_TEX_UNIFORM_NAME  = "material.diffuse";
	static constexpr const char* SPECULAR_TEX_UNIFORM_NAME = "material.specular";

	static constexpr const char* MODEL_MAT_UNIFORM_NAME    = "u_ModelMat";
	static constexpr const char* PROJ_MAT_UNIFORM_NAME     = "u_ProjMat";
	static constexpr const char* VIEW_MAT_UNIFORM_NAME     = "u_ViewMat";
};

