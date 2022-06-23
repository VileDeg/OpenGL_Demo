#include "Object.h"

const std::string Object::textureUniformName = "u_Texture";

const std::string Object::modelMatUniformName = "u_ModelMat";
const std::string Object::projMatUniformName = "u_ProjMat";
const std::string Object::viewMatUniformName = "u_ViewMat";

void Object::Move(glm::vec3 by)
{
	m_Model = glm::translate(m_Model, by);
}

void Object::Rotate(float angle, glm::vec3 axis)
{
	m_Model = glm::rotate(m_Model, angle, axis);
}

Object::Object(const VertexData& vData, const unsigned* indices,
	std::size_t indices_size, const std::vector<Texture> textures,
	const char* vertShaderPath, const char* fragShaderPath,
	const glm::vec3& position)
	: m_Shader(vertShaderPath, fragShaderPath), 
	m_Model(glm::translate(glm::mat4(1.0f),position)),
	m_VAO(), m_VBO(vData.data, vData.size, vData.vertexCount), 
	m_EBO(indices, indices_size), m_TextureCount(0)
{
	VBO vbo(vData.data, vData.size, vData.vertexCount);
	VertexLayout layout;
	layout.Push<float>(vData.vertexCoordsCount);
	layout.Push<float>(vData.colorCoordsCount);
	for (auto texture : textures)
	{
		layout.Push<float>(vData.textureCoordsCount);
	}
	m_VAO.AddBuffer(vbo, layout);
	m_EBO.Bind();
	m_Shader.Bind();
	for (auto texture : textures)
	{
		m_Shader.setInt(Object::textureUniformName + std::to_string(m_TextureCount), m_TextureCount);
		texture.Bind();
		++m_TextureCount;
	}
	
}

Object::Object(const VertexData& vData, const std::vector<Texture> textures,
	const char* vertShaderPath, const char* fragShaderPath,
	const glm::vec3& position)
	: m_Shader(vertShaderPath, fragShaderPath),
	m_Model(glm::translate(glm::mat4(1.0f), position)),
	m_VAO(), m_VBO(vData.data, vData.size, vData.vertexCount)
{

	
	VertexLayout layout;
	layout.Push<float>(vData.vertexCoordsCount);
	layout.Push<float>(vData.colorCoordsCount);
	for (auto texture : textures)
	{
		layout.Push<float>(vData.textureCoordsCount);
	}
	m_VAO.AddBuffer(m_VBO, layout);
	
	m_Shader.Bind();
	for (auto texture : textures)
	{
		m_Shader.setInt(Object::textureUniformName + std::to_string(m_TextureCount), m_TextureCount);
		texture.Bind();
		++m_TextureCount;
	}
}

void Object::Draw()
{
	m_Shader.Bind();
	m_Shader.setMat4f(modelMatUniformName, m_Model);
	m_Renderer.Draw(m_VAO, m_EBO, m_Shader);
}

void Object::DrawNoIndex()
{
	m_Shader.Bind();
	m_Shader.setMat4f(modelMatUniformName, m_Model);
	m_Renderer.DrawNoIndex(m_VAO, m_Shader);
}

void Object::Scale(glm::vec3 by)
{
	m_Model = glm::scale(m_Model, by);
}

void Object::SetProjMat(const glm::mat4 projMat)
{
	m_Shader.Bind();
	m_Shader.setMat4f(Object::projMatUniformName, projMat);
}

void Object::SetViewMat(const glm::mat4 viewMat)
{
	m_Shader.Bind();
	m_Shader.setMat4f(Object::viewMatUniformName, viewMat);
}
