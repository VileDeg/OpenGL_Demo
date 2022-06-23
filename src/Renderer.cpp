#include "Renderer.h"



void Renderer::Draw(const VAO& vao, const EBO& ebo, const Shader& shader) const
{
	shader.Bind();
	vao.Bind();
	glDrawElements(GL_TRIANGLES, ebo.Count(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawNoIndex(const VAO& vao, const Shader& shader) const
{
	shader.Bind();
	vao.Bind();
	glDrawArrays(GL_TRIANGLES, 0, vao.Count());

}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
