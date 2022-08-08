#pragma once
#include "math_headers.h"
#include "renderer/Texture.h"
#include "renderer/VertexArray.h"

#include <cereal/archives/json.hpp>
#include <cereal/access.hpp>

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
    Mesh() {}
    Mesh(const Mesh& m) 
        : m_UniformColor(m.m_UniformColor), m_VAO(m.m_VAO),
        m_VBO(m.m_VBO), m_EBO(m.m_EBO),
        m_Textures(m.m_Textures)
    {}
    //For model import
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
        std::unordered_map<TexType, std::vector<std::string>> textures);

    //For primitives
    Mesh(const void* vertexData, std::size_t size, unsigned vertexCount,
        std::unordered_map<TexType, std::vector<std::string>> textures);

    Ref<VAO> Vao() { return m_VAO; }
    Ref<VBO> Vbo() { return m_VBO; }
    Ref<EBO> Ebo() { return m_EBO; }
    const glm::vec4& UniformColor() const { return m_UniformColor; }

    std::unordered_map<TexType, std::vector<Ref<Texture>>>& Textures()
        { return m_Textures; }

    ~Mesh() {}

private:
    glm::vec4 m_UniformColor;

    Ref<VAO> m_VAO;
    Ref<VBO> m_VBO;
    Ref<EBO> m_EBO;

    std::unordered_map<TexType, std::vector<Ref<Texture>>> m_Textures;
    
private:
    friend class cereal::access;
    template<typename Archive>
    void serialize(Archive& ar)
    {
        ar & cereal::make_nvp("VertexArray", m_VAO);
        ar & cereal::make_nvp("VertexBuffer", m_VBO);
        ar & cereal::make_nvp("IndexBuffer", m_EBO);
        ar & cereal::make_nvp("Textures", m_Textures);
    }
};

struct MeshInstance
{
    Mesh mesh;
    bool HasTextures{ true };
    bool NormalsOut{ true };
    glm::vec4 Color{ 1.f, 0.f, 1.f, 1.f }; //magenta;

    MeshInstance() = default;
    MeshInstance(const MeshInstance& mi)
        : mesh(mi.mesh), HasTextures(mi.HasTextures),
        NormalsOut(mi.NormalsOut), Color(mi.Color) {}
    MeshInstance(MeshInstance&& mi)
        : mesh(mi.mesh), HasTextures(mi.HasTextures),
        NormalsOut(mi.NormalsOut), Color(mi.Color) {}
    MeshInstance(Mesh& mesh, bool hasTex = true, bool normOut = true)
        : mesh(mesh), HasTextures(hasTex),
        NormalsOut(normOut), Color(mesh.UniformColor()) {}
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