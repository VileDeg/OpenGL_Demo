#include "pch.h"
#include "renderer/Renderer.h"
#include "glad/glad.h"

Renderer::RenderData* Renderer::s_Data = nullptr;

void Renderer::Init()
{
	s_Data = new RenderData();
	s_Data->Shader[ShaderType::Color] = CreateRef<Shader>("color.shader");

	s_Data->Shader[ShaderType::Diffuse] = CreateRef<Shader>("diffuse.shader");
	s_Data->Shader[ShaderType::Diffuse]->setInt("u_DiffuseTex", 0);

	s_Data->Shader[ShaderType::DiffNSpec] = CreateRef<Shader>("diffNSpec.shader");
	s_Data->Shader[ShaderType::DiffNSpec]->setInt("material.diffuse", 0);
	s_Data->Shader[ShaderType::DiffNSpec]->setInt("material.specular", 0);

}

void Renderer::Draw(const glm::mat4& modelMat, const Ref<VAO> vao, const glm::vec4& color)
{
	Ref<Shader> sh = s_Data->Shader[ShaderType::Color];
	sh->Bind();
	sh->setMat4f("u_ModelMat", modelMat);

	sh->setFloat4("u_Color", color);

	vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, vao->Count());
}

void Renderer::Draw(const glm::mat4& modelMat, const Ref<VAO> vao, const Ref<Texture> diffuse)
{
	Ref<Shader> sh = s_Data->Shader[ShaderType::Diffuse];
	sh->Bind();
	sh->setMat4f("u_ModelMat", modelMat);

	diffuse->Bind(0);

	vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, vao->Count());
}

void Renderer::Draw(const glm::mat4& modelMat, const Ref<VAO> vao, const Ref<Texture> diffuse, const Ref<Texture> specular)
{
	Ref<Shader> sh = s_Data->Shader[ShaderType::DiffNSpec];
	sh->Bind();
	sh->setMat4f("u_ModelMat", modelMat);

	diffuse->Bind(0);
	specular->Bind(1);

	vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, vao->Count());
}


void Renderer::BeginScene(const Camera& camera)
{
	for (auto& sh : s_Data->Shader)
	{
		sh.second->Bind();
		sh.second->setMat4f("u_ViewProjMat", camera.GetProjMat() * camera.GetViewMat());
	}
	
}

void Renderer::EndScene()
{
}

void Renderer::Shutdown()
{
	delete s_Data;
}


//void Renderer::DrawNoIndex(const VAO& vao, const Shader& shader) const
//{
//	shader.Bind();
//	vao.Bind();
//	glDrawArrays(GL_TRIANGLES, 0, vao.Count());
//
//}