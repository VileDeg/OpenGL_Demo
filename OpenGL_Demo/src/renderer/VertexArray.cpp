#include "pch.h"
#include "VertexArray.h"
#include "glad/glad.h"

VAO::VAO()
{
	glGenVertexArrays(1, &m_Id);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &m_Id);
}

void VAO::AddBuffer(const VBO& vbo, EBO* ebo)
{
	Bind();
	vbo.Bind();
	vbo.GetLayout().Enable();
	m_Count = vbo.Count();
	if (ebo)
		m_EBO = CreateRef<EBO>(*ebo);
}

void VAO::Bind() const
{
	glBindVertexArray(m_Id);
}

void VAO::Unbind() const
{
	glBindVertexArray(0);
}