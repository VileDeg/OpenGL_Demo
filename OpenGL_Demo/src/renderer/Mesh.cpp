#include "pch.h"
#include "renderer/Mesh.h"
#include "glad/glad.h"

Mesh::Mesh(const void* vertexData, std::size_t size, unsigned vertexCount,
    std::unordered_map<TexType, std::vector<std::string>> textures)
    : m_VAO(CreateRef<VAO>()),
    m_VBO(CreateRef<VBO>(vertexData, size, vertexCount))
{
    VertexLayout layout
    {
        {GL_FLOAT, 3, GL_FALSE},
        {GL_FLOAT, 3, GL_FALSE},
        {GL_FLOAT, 2, GL_FALSE}
    };
    m_VBO->SetLayout(layout);
    m_VAO->AddBuffer(*m_VBO);

    for (auto& [type, paths] : textures)
    {
        for (auto& p : paths)
        {
            m_Textures[type].push_back(CreateRef<Texture>(p));
            m_HasTextures = true;
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
        {GL_FLOAT, 3, GL_FALSE},
        //{GL_FLOAT, 4, GL_FALSE},
        {GL_FLOAT, 3, GL_FALSE},
        {GL_FLOAT, 2, GL_FALSE},
        {GL_FLOAT, 3, GL_FALSE},
        {GL_FLOAT, 3, GL_FALSE},
        {GL_INT  , 4          },
        {GL_FLOAT, 4, GL_FALSE}
    };
    m_VBO->SetLayout(layout);
    m_VAO->AddBuffer(*m_VBO);

    for (auto& [type, paths] : textures)
    {
        for (auto& p : paths)
        {
            m_Textures[type].push_back(CreateRef<Texture>(p, false));
            m_HasTextures = true;
        }
    }
}