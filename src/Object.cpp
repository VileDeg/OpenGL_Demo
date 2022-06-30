#include "pch.h"
#include "Object.h"

Object::Object(const VertexData& vData,
	const Shader shader, const TextureData& tData = { "@#none#@", "@#none#@" },
	const glm::vec3 position = glm::vec3(0.0f),
	ObjectType type = ObjectType::DEFAULT)
	: m_TextureSlot(0),
	m_Model(glm::translate(glm::mat4(1.0f), position)),
	m_Shader(std::make_unique<Shader>(shader)),
	m_VAO(std::make_unique<VAO>()),
	m_VBO(std::make_unique<VBO>(vData.data, vData.size, vData.vertexCount))	,
	m_DiffuseTex(
		type == ObjectType::DEFAULT ?
		std::make_unique<Texture>(Texture(tData.diffusePath)) : nullptr
	),
	m_SpecularTex(
		type == ObjectType::DEFAULT ?
		std::make_unique<Texture>(Texture(tData.specularPath)) : nullptr
	)
{
		
	VertexLayout layout;
	layout.Push<float>(vData.vertexCoordsCount);
	layout.Push<float>(vData.normalCoordsCount);
	layout.Push<float>(vData.colorCoordsCount);	
	layout.Push<float>(vData.textureCoordsCount);

	m_VAO->AddBuffer(*m_VBO, layout);

	m_Shader->Bind();

	m_Shader->setInt(DIFFUSE_TEX_UNIFORM_NAME + std::to_string(m_TextureSlot), 0);
	m_DiffuseTex->Bind(0);

	m_Shader->setInt(DIFFUSE_TEX_UNIFORM_NAME + std::to_string(m_TextureSlot), 1);
	m_SpecularTex->Bind(1);
}


//void Object::Draw()
//{
//	m_Shader->Bind();
//	m_Shader->setMat4f(modelMatUniformName, m_Model);
//	m_Renderer.Draw(*m_VAO, *m_EBO, *m_Shader);
//}

void Object::DrawNoIndex()
{
	m_Shader->Bind();
	m_Shader->setMat4f(MODEL_MAT_UNIFORM_NAME, m_Model);
	m_Renderer.DrawNoIndex(*m_VAO, *m_Shader);
}

void Object::SetModelMat(const glm::mat4 modelMat)
{
	m_Shader->Bind();
	m_Shader->setMat4f(MODEL_MAT_UNIFORM_NAME, modelMat);
}

void Object::SetProjMat(const glm::mat4 projMat)
{
	m_Shader->Bind();
	m_Shader->setMat4f(PROJ_MAT_UNIFORM_NAME, projMat);
}

void Object::SetViewMat(const glm::mat4 viewMat)
{
	m_Shader->Bind();
	m_Shader->setMat4f(VIEW_MAT_UNIFORM_NAME, viewMat);
}

void Object::WatchedBy(const Camera& camera)
{
	m_Shader->Bind();
	m_Shader->setMat4f(PROJ_MAT_UNIFORM_NAME, camera.GetProjMat());
	m_Shader->setMat4f(VIEW_MAT_UNIFORM_NAME, camera.GetViewMat());
}
