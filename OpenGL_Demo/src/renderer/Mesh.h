#pragma once
#include <glm/glm.hpp>
#include "renderer/Texture.h"
#include "renderer/VertexArray.h"
#include "geometry/GeoData.h"

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

struct MeshData //Data needed to construct a primitive mesh.
{
    Primitive primType;
    std::unordered_map<TexType, std::vector<std::string>> textures{};

    MeshData() = default;
    MeshData(const MeshData& data) = default;

    friend bool operator==(const MeshData& lhs, const MeshData& rhs)
    {
        return (lhs.primType == rhs.primType &&
            lhs.textures == rhs.textures);
    }
};

class Mesh
{
public:
    //For model import
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
        std::unordered_map<TexType, std::vector<std::string>> textures);

    Ref<VAO> Vao() { return m_VAO; }

    const glm::vec4& UniformColor() const { return m_UniformColor; }

    std::unordered_map<TexType, std::vector<Ref<Texture>>>& Textures()
        { return m_Textures; }

    ~Mesh() {}

    friend bool operator==(const Mesh& lhs, const Mesh& rhs)
    {
        return std::tie(lhs.m_UniformColor, lhs.m_VAO, lhs.m_VBO, lhs.m_EBO, lhs.m_Textures) ==
            std::tie(rhs.m_UniformColor, rhs.m_VAO, rhs.m_VBO, rhs.m_EBO, rhs.m_Textures);
    }

private:
    friend class MeshManager;
    //For primitives. Called by MeshManager
    Mesh(const MeshData& data);
private:
    glm::vec4 m_UniformColor;

    Ref<VAO> m_VAO;
    Ref<VBO> m_VBO;
    Ref<EBO> m_EBO;

    std::unordered_map<TexType, std::vector<Ref<Texture>>> m_Textures;
};

struct MeshInstance
{
    Ref<Mesh> mesh;
    bool HasTextures{ true };
    bool NormalsOut{ true };
    glm::vec4 Color{ 1.f, 0.f, 1.f, 1.f }; //magenta;
};