#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &id);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &id);
}

void VAO::AddBuffer(const VBO& vbo, const VertexLayout& layout)
{
	Bind();
	vbo.Bind();
	layout.Enable();
}

void VAO::Bind() const
{
	glBindVertexArray(id);
}

void VAO::Unbind() const
{
	glBindVertexArray(0);
}
