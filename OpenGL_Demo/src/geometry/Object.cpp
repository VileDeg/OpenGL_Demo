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
	
	//m_Shader->Bind();
	VertexLayout layout
	{
		{GL_FLOAT, 3, GL_FALSE},
		{GL_FLOAT, 3, GL_FALSE},
		{GL_FLOAT, 2, GL_FALSE}
	};
	m_VBO->SetLayout(layout);
	m_VAO->AddBuffer(*m_VBO);
	
	//unsigned slot = 0;
	//switch (type)
	//{
	//case ObjectType::NO_TEXTURES:
	//	return;
	//case ObjectType::DIFFUSE_ONLY:
	//	//m_Shader->setInt(DIFFUSE_TEX_UNIFORM_NAME + std::to_string(0), slot);
	//	//m_DiffuseTex->Bind(slot);
	//	break;
	//case ObjectType::BOTH_TEXTURES:
	//	m_Shader->setInt(DIFFUSE_TEX_UNIFORM_NAME + std::to_string(0), slot);
	//	m_DiffuseTex->Bind(slot);
	//	++slot;

	//	m_Shader->setInt(SPECULAR_TEX_UNIFORM_NAME + std::to_string(0), slot);
	//	m_SpecularTex->Bind(slot);
		
	//}

}

//void Object::SetModelMat(const glm::mat4 modelMat)
//{
//	m_Shader->Bind();
//	m_Shader->setMat4f(MODEL_MAT_UNIFORM_NAME, modelMat);
//}
//
//void Object::SetProjMat(const glm::mat4 projMat)
//{
//	m_Shader->Bind();
//	m_Shader->setMat4f(PROJ_MAT_UNIFORM_NAME, projMat);
//}
//
//void Object::SetViewMat(const glm::mat4 viewMat)
//{
//	m_Shader->Bind();
//	m_Shader->setMat4f(VIEW_MAT_UNIFORM_NAME, viewMat);
//}
//
//void Object::WatchedBy(const Camera& camera)
//{
//	m_Shader->Bind();
//	m_Shader->setMat4f(PROJ_MAT_UNIFORM_NAME, camera.GetProjMat());
//	m_Shader->setMat4f(VIEW_MAT_UNIFORM_NAME, camera.GetViewMat());
//}