#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "renderer/Texture.h"
#include "renderer/VertexArray.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    // color
    glm::vec4 Color;
    //bone indexes which will influence this vertex
    int   m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

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

    Ref<VAO> Vao() { return m_VAO; }

    std::unordered_map<TexType, std::vector<Ref<Texture>>>& Textures()
        { return m_Textures; }

    ~Mesh() {}
    glm::vec4 UniformColor;
private:

    Ref<VAO> m_VAO;
    Ref<VBO> m_VBO;
    Ref<EBO> m_EBO;

    

    std::unordered_map<TexType, std::vector<Ref<Texture>>> m_Textures;
};

struct MeshInstance
{
    Mesh mesh;
    bool HasTextures{ true };
    bool NormalsOut{ true };
    glm::vec4 Color{ 1.f, 0.f, 1.f, 1.f }; //magenta;

    /*bool HasTextures(bool has) { return m_HasTextures = has; }
    bool HasTextures() { return m_HasTextures; }

    bool NormalsOut(bool out) { return m_NormalsOut = out; }
    bool NormalsOut() { return m_NormalsOut; }

    const glm::vec4& Color(const glm::vec4& color) { return m_Color = color; }
    const glm::vec4& Color() { return m_Color; }*/

    MeshInstance() = default;
    MeshInstance(const MeshInstance& mi)
        : mesh(mi.mesh), HasTextures(mi.HasTextures),
        NormalsOut(mi.NormalsOut), Color(mi.Color) {}
    MeshInstance(MeshInstance&& mi)
        : mesh(mi.mesh), HasTextures(mi.HasTextures),
        NormalsOut(mi.NormalsOut), Color(mi.Color) {}
    MeshInstance(Mesh& mesh, bool hasTex = true, bool normOut = true)
        : mesh(mesh), HasTextures(hasTex),
        NormalsOut(normOut), Color(mesh.UniformColor) {}
    MeshInstance& operator=(MeshInstance& mi)
    { 
        if (this == &mi)
            return *this;
        mesh = mi.mesh;
        HasTextures = mi.HasTextures;
        NormalsOut = mi.NormalsOut;
        Color = mi.Color;
        return *this; 
    };
};