#include "Renderer.h"


void Renderer::Draw(const VAO& vao, const EBO& ebo, const Shader& shader) const
{
	shader.Use();
	vao.Bind();
	glDrawElements(GL_TRIANGLES, ebo.Count(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
