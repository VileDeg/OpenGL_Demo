#include "pch.h"
#include "renderer/Mesh.h"
#include "glad/glad.h"

Mesh::Mesh(const void* vertexData, std::size_t size, unsigned vertexCount,
    std::unordered_map<TexType, const char*> textures)
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

    for (auto [type, texture] : textures)
    {
        m_Textures[type] = CreateRef<Texture>(texture);
    }
}