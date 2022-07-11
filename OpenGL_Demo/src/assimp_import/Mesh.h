#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <renderer/Shader.h>

namespace AssimpImport {

    using namespace std;

    #define MAX_BONE_INFLUENCE 4

    struct Vertex {
        // position
        glm::vec3 Position;
        //colors
        glm::vec4 Color;
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

    struct STexture {
        unsigned int id;
        string type;
        string path;
    };

    class Mesh {
    public:
        // mesh Data
        vector<Vertex>       vertices;
        vector<unsigned int> indices;
        vector<STexture>      textures;
        unsigned int VAO;

        // constructor
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
            vector<STexture> textures);

        // render the mesh
        void Draw(Shader& shader);
    public:
        static constexpr const char* DIFFUSE_TEX_NAME = "material.diffuse";
        static constexpr const char* SPECULAR_TEX_NAME = "material.specular";
        static constexpr const char* NORMAL_TEX_NAME = "material.normal";
        static constexpr const char* HEIGHT_TEX_NAME = "material.height";

    private:
        // render data 
        unsigned int VBO, EBO;

        // initializes all the buffer objects/arrays
        void setupMesh();
    };
}
