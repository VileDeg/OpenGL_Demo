#pragma once

#include <assimp/scene.h>
#include "renderer/Mesh.h"
#include "renderer/Texture.h"

namespace Crave
{
    namespace Import
    {
        struct ModelNodeData
        {
            std::string                 name{};
            glm::mat4                   transform{};
            std::vector<Ref<Mesh>>      meshes{};
            std::vector<ModelNodeData>  childData{};
        };

        class Model
        {
        public:
            
        private:
            //For import. Used by Scene class.
            Model(const std::string& path, bool gamma = false);
            void loadModel(std::string const& shortPath);

            ModelNodeData processNode(aiNode* node, const aiScene* scene);

            Ref<Mesh> processMesh(aiMesh* mesh, const aiScene* scene, const aiMatrix4x4& nodeTransform);

            // checks all material textures of a given type and 
            // loads the textures if they're not loaded yet.
            // the required info is returned as a STexture struct.
            std::vector<std::string> loadMaterialTextures(aiMaterial* mat,
                aiTextureType type);
        private:
            friend class Scene;
            std::vector<std::string> m_TexturesLoaded;
            ModelNodeData m_NodeData;
            std::string              m_Directory;
            bool                     m_GammaCorrection;
        private:
            static constexpr const char* BASE_MODEL_PATH = "res/models/";
        };
    }
}