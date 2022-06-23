#pragma once
#include "VAO.h"
#include "shader/Shader.h"
#include "math_headers.h"
#include "Texture.h"
#include "Renderer.h"


class Object
{
public:
	struct VertexData
	{
		const void* data;
		std::size_t size;
		unsigned vertexCount;
		unsigned char vertexCoordsCount;
		unsigned char colorCoordsCount;
		unsigned char textureCoordsCount;
	};

public:
	Object(const VertexData& vData, const unsigned* indices,
		std::size_t indices_size, const std::vector<Texture> textures,
		const char* vertShaderPath, const char* fragShaderPath,
		const glm::vec3& position = glm::vec3(0.0f) );

	Object(const VertexData& vData, const std::vector<Texture> textures,
		const char* vertShaderPath, const char* fragShaderPath,
		const glm::vec3& position = glm::vec3(0.0f));

	
	~Object() {}

	void Move(glm::vec3 by);
	void Rotate(float angle, glm::vec3 axis);
	void Scale(glm::vec3 by);

	void SetProjMat(const glm::mat4 projMat);
	void SetViewMat(const glm::mat4 viewMat);

	void Draw();
	void DrawNoIndex();
	inline void ResetTransform() { m_Model = glm::mat4(1.0f); };
	
private:
	VAO m_VAO;
	VBO m_VBO;
	EBO m_EBO;
	Shader m_Shader;
	glm::mat4 m_Model;
	int m_TextureCount;
	Renderer m_Renderer;

	static const std::string textureUniformName;
	static const std::string modelMatUniformName;
	static const std::string projMatUniformName;
	static const std::string viewMatUniformName;
};

