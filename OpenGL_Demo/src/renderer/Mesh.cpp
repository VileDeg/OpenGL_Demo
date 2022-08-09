#include "pch.h"
#include "renderer/Mesh.h"
#include "glad/glad.h"

Mesh::Mesh(const MeshData& data)
{
    m_VAO = CreateRef<VAO>();
    auto gd = GeoData::GetData(data.primType);
    
    m_VBO = CreateRef<VBO>(gd.data, gd.size, gd.count);

    VertexLayout layout
    {
        {GL_FLOAT, 3, GL_FALSE}, //position
        {GL_FLOAT, 3, GL_FALSE}, //normal
        {GL_FLOAT, 2, GL_FALSE}, //texcoords
        {GL_FLOAT, 3, GL_FALSE}, //tangent
        {GL_FLOAT, 3, GL_FALSE}  //bitangent
    };
    m_VBO->SetLayout(layout);
    m_VAO->AddBuffer(*m_VBO, nullptr); //No EBO

    for (auto& [type, paths] : data.textures)
    {
        for (auto& p : paths)
        {
            m_Textures[type].push_back(CreateRef<Texture>(p));
            //m_HasTextures = true;
        }
    }
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
    std::unordered_map<TexType, std::vector<std::string>> textures)
{
    m_VAO = CreateRef<VAO>();
    m_VBO = CreateRef<VBO>(
        (const void*)&vertices[0], vertices.size() * sizeof(Vertex), vertices.size());
    m_EBO = CreateRef<EBO>(&indices[0], indices.size());
    VertexLayout layout
    {
        {GL_FLOAT, 3, GL_FALSE}, //position
        {GL_FLOAT, 3, GL_FALSE}, //normal
        {GL_FLOAT, 2, GL_FALSE}, //texcoords
        {GL_FLOAT, 3, GL_FALSE}, //tangent
        {GL_FLOAT, 3, GL_FALSE}, //bitangent
        {GL_FLOAT, 4, GL_FALSE}, //color
        {GL_INT  , 4          }, //boneid
        {GL_FLOAT, 4, GL_FALSE}  //weights
    };
    m_VBO->SetLayout(layout);
    m_VAO->AddBuffer(*m_VBO, m_EBO);

    for (auto& [type, paths] : textures)
    {
        for (auto& p : paths)
        {
            m_Textures[type].push_back(CreateRef<Texture>(p, false));
            //m_HasTextures = true;
        }
    }
}