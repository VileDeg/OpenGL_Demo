#include "pch.h"
#include "Object.h"
#include "renderer/Renderer.h"
#include "glad/glad.h"

void Object::DrawColor(const glm::vec4& color)
{
	Renderer::Draw(m_Model, m_VAO, color);
}

void Object::Draw()
{
	Renderer::Draw(m_Model, m_VAO, m_DiffuseTex);
}

void Object::Init(ObjectType type)
{
	

	VertexLayout layout
	{
		{GL_FLOAT, 3, GL_FALSE},
		{GL_FLOAT, 3, GL_FALSE},
		{GL_FLOAT, 2, GL_FALSE}
	};
	m_VBO->SetLayout(layout);
	m_VAO->AddBuffer(*m_VBO);
	
	
}

