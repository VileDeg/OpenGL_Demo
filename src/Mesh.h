#pragma once
#include "Texture.h"
#include "VAO.h"
#include "shader/Shader.h"

class Mesh
{
public:
private:
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<VAO> m_VAO;
	std::unique_ptr<VBO> m_VBO;
	std::unique_ptr<EBO> m_EBO;

	//std::vector< std::unique_ptr<Texture> > m_Textures;
	std::unique_ptr<Texture> m_DiffuseTex;
	std::unique_ptr<Texture> m_SpecularTex;

	glm::mat4 m_Model;

	int m_TextureSlot;
	
	static constexpr const char* DIFFUSE_TEX_UNIFORM_NAME = "material.diffuse";
	static constexpr const char* SPECULAR_TEX_UNIFORM_NAME = "material.specular";

	static constexpr const char* MODEL_MAT_UNIFORM_NAME = "u_ModelMat";
	static constexpr const char* PROJ_MAT_UNIFORM_NAME = "u_ProjMat";
	static constexpr const char* VIEW_MAT_UNIFORM_NAME = "u_ViewMat";
};

