#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "renderer/Texture.h"
#include "renderer/VertexArray.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    //colors
    //glm::vec4 Color;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int   m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

//struct STexture {
//    unsigned int id;
//    std::string type;
//    std::string path;
//};

enum class TexType
{
    None = -1, Diffuse, Specular, Normal, Height
};
  
class Mesh
{
public:
    //For model import
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
        std::unordered_map<TexType, std::vector<std::string>> textures);

    //For primitives
    Mesh(const void* vertexData, std::size_t size, unsigned vertexCount,
        std::unordered_map<TexType, std::vector<std::string>> textures);

    bool HasTextures(bool has)                     { return m_HasTextures = has; }
    bool HasTextures()                             { return m_HasTextures; }

    bool NormalsOut(bool out)                      { return m_NormalsOut = out; }
    bool NormalsOut()                              { return m_NormalsOut; }

    const glm::vec4& Color(const glm::vec4& color) { return m_Color = color; }
    const glm::vec4& Color()                       { return m_Color; }

    //Ref<Texture> Diffuse()  { 
    //    return !m_Textures[TexType::Diffuse].empty() ? m_Textures[TexType::Diffuse ][0] : nullptr; }//To change
    //Ref<Texture> Specular() { return m_Textures[TexType::Specular][0]; }//To change
    Ref<VAO>     Vao()      { return m_VAO;                         }

    const std::unordered_map<TexType, std::vector<Ref<Texture>>>& Textures() const
        { return m_Textures; }

    ~Mesh() {}
private:
    bool m_HasTextures{ false };
    bool m_NormalsOut { true };
    glm::vec4 m_Color { 1.f, 0.f, 1.f, 1.f }; //magenta;

    Ref<VAO> m_VAO;
    Ref<VBO> m_VBO;
    Ref<EBO> m_EBO;

    std::unordered_map<TexType, std::vector<Ref<Texture>>> m_Textures;
};