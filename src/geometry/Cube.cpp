#include "pch.h"
#include "Cube.h"
#include "Renderer.h"

void Cube::Draw()
{
	m_Shader->Bind();
	m_Shader->setMat4f(MODEL_MAT_UNIFORM_NAME, m_Model);

	
	m_VAO->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cube::Init(CubeType type)
{
	
	m_Shader->Bind();
	VertexLayout layout
	{
		{GL_FLOAT, 3, GL_FALSE},
		{GL_FLOAT, 3, GL_FALSE},
		{GL_FLOAT, 2, GL_FALSE}
	};
	m_VBO->SetLayout(layout);
	m_VAO->AddBuffer(*m_VBO);
	
	unsigned slot = 0;
	switch (type)
	{
	case CubeType::NO_TEXTURES:
		return;
	case CubeType::DIFFUSE_ONLY:
		m_Shader->setInt(DIFFUSE_TEX_UNIFORM_NAME + std::to_string(0), slot);
		m_DiffuseTex->Bind(slot);
		break;
	case CubeType::BOTH_TEXTURES:
		m_Shader->setInt(DIFFUSE_TEX_UNIFORM_NAME + std::to_string(0), slot);
		m_DiffuseTex->Bind(slot);
		++slot;

		m_Shader->setInt(SPECULAR_TEX_UNIFORM_NAME + std::to_string(0), slot);
		m_SpecularTex->Bind(slot);
		
	}

}

void Cube::SetModelMat(const glm::mat4 modelMat)
{
	m_Shader->Bind();
	m_Shader->setMat4f(MODEL_MAT_UNIFORM_NAME, modelMat);
}

void Cube::SetProjMat(const glm::mat4 projMat)
{
	m_Shader->Bind();
	m_Shader->setMat4f(PROJ_MAT_UNIFORM_NAME, projMat);
}

void Cube::SetViewMat(const glm::mat4 viewMat)
{
	m_Shader->Bind();
	m_Shader->setMat4f(VIEW_MAT_UNIFORM_NAME, viewMat);
}

void Cube::WatchedBy(const Camera& camera)
{
	m_Shader->Bind();
	m_Shader->setMat4f(PROJ_MAT_UNIFORM_NAME, camera.GetProjMat());
	m_Shader->setMat4f(VIEW_MAT_UNIFORM_NAME, camera.GetViewMat());
}