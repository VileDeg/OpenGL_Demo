#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "renderer/Texture.h"
#include "renderer/Buffer.h"

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
};

enum class TexType
{
    None = -1, Diffuse, Specular
};
  
class Mesh
{
public:
    Mesh(const void* vertexData, std::size_t size, unsigned vertexCount,
        std::unordered_map<TexType, const char*> textures);

    ~Mesh() {}

    Ref<Texture> Diffuse()  { return m_Textures[TexType::Diffuse ]; }
    Ref<Texture> Specular() { return m_Textures[TexType::Specular]; }
    Ref<VAO>     Vao()      { return m_VAO;                         }

    const std::unordered_map<TexType, Ref<Texture>>& Textures() const 
        { return m_Textures; }
private:
    std::shared_ptr<VAO> m_VAO;
    std::shared_ptr<VBO> m_VBO;
    //std::unique_ptr<EBO> m_EBO;

    std::unordered_map<TexType, Ref<Texture>> m_Textures;
};