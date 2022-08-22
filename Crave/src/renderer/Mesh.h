#pragma once
#include <glm/glm.hpp>
#include "renderer/Texture.h"
#include "renderer/VertexArray.h"
#include "geometry/GeoData.h"

namespace Crave
{
#define MAX_BONE_INFLUENCE 4

    class Mesh
    {
    public:
        enum class TexType
        {
            None = -1, Diffuse, Specular, Normal, Height
        };
        struct Vertex
        {
            glm::vec3 Position; // position
            glm::vec3 Normal; // normal
            glm::vec2 TexCoords; // texCoords
            glm::vec3 Tangent; // tangent
            glm::vec3 Bitangent; // bitangent
            glm::vec4 Color; // color
            int   m_BoneIDs[MAX_BONE_INFLUENCE]; //bone indexes which will influence this vertex
            float m_Weights[MAX_BONE_INFLUENCE]; //weights from each bone

            friend bool operator==(const Vertex& lhs, const Vertex& rhs)
            {
                return (lhs.Position == rhs.Position && lhs.Normal == rhs.Normal &&
                    lhs.TexCoords == rhs.TexCoords && lhs.Tangent == rhs.Tangent &&
                    lhs.Bitangent == rhs.Bitangent && lhs.Color == rhs.Color &&
                    lhs.m_BoneIDs == rhs.m_BoneIDs && lhs.m_Weights == rhs.m_Weights);
            }
        };
        struct PrimitiveData //Data needed to construct a primitive mesh.
        {
            Primitive primType;
            std::unordered_map<TexType, std::vector<std::string>> textures{};

            PrimitiveData() = default;
            PrimitiveData(const PrimitiveData& data) = default;

            friend bool operator==(const PrimitiveData& lhs, const PrimitiveData& rhs)
            {
                return (lhs.primType == rhs.primType &&
                    lhs.textures == rhs.textures);
            }
        };
        struct ModelData //Data needed to construct an imported model mesh.
        {
            std::vector<Vertex> vertices{};
            std::vector<unsigned> indices{};
            std::unordered_map<TexType, std::vector<std::string>> textures{};

            ModelData() = default;
            ModelData(const ModelData& data) = default;

            friend bool operator==(const ModelData& lhs, const ModelData& rhs)
            {
                return (lhs.vertices == rhs.vertices && lhs.indices == rhs.indices &&
                    lhs.textures == rhs.textures);
            }
        };
    public:
        Ref<VAO> Vao() { return m_VAO; }

        const glm::vec4& UniformColor() const { return m_UniformColor; }

        std::unordered_map<TexType, std::vector<Ref<Texture>>>& Textures()
        {
            return m_Textures;
        }

        ~Mesh() {}

        friend bool operator==(const Mesh& lhs, const Mesh& rhs)
        {
            return (lhs.m_UniformColor == rhs.m_UniformColor && lhs.m_VAO == rhs.m_VAO &&
                lhs.m_VBO == rhs.m_VBO && lhs.m_EBO == rhs.m_EBO && lhs.m_Textures == rhs.m_Textures);
        }

    private:
        friend class MeshManager;
        //For primitives. Called by MeshManager
        Mesh(const PrimitiveData& data);

        //For model import. Called by MeshManager
        Mesh(const ModelData& data);

    private:
        glm::vec4 m_UniformColor{};

        Ref<VAO> m_VAO{};
        Ref<VBO> m_VBO{};
        Ref<EBO> m_EBO{};

        std::unordered_map<TexType, std::vector<Ref<Texture>>> m_Textures{};
    };

}
