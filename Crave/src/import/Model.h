#pragma once

#include <assimp/scene.h>
#include "renderer/Mesh.h"
#include "renderer/Texture.h"

namespace Import
{
    class Model
    {
    public:
        struct NodeData
        {
            std::string            name{};
            glm::mat4              transform{};
            std::vector<Ref<Mesh>> meshes{};
            std::vector<NodeData>  childData{};
        };
    public:
        


        //std::vector<Ref<Mesh>> Meshes() { return m_Meshes; }

    private:
        //For import
        Model(const std::string& path, bool gamma = false);
        void loadModel(std::string const& shortPath);

        NodeData processNode(aiNode* node, const aiScene* scene);

        Ref<Mesh> processMesh(aiMesh* mesh, const aiScene* scene, const aiMatrix4x4& nodeTransform);

        // checks all material textures of a given type and 
        // loads the textures if they're not loaded yet.
        // the required info is returned as a STexture struct.
        std::vector<std::string> loadMaterialTextures(aiMaterial* mat,
            aiTextureType type);
    private:
        friend class Scene;
        std::vector<std::string> m_TexturesLoaded;
        NodeData m_NodeData;
        std::string              m_Directory;
        bool                     m_GammaCorrection;
    private:
        static constexpr const char* BASE_MODEL_PATH = "res/models/";
    };
}
