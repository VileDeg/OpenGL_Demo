#pragma once

#include <assimp/scene.h>
#include "Mesh.h"
#include "renderer/Texture.h"

class Model
{
public:
    //For import
    Model(std::string const& path, bool gamma = false);
    

    std::vector<Ref<Mesh>> Meshes() { return m_Meshes; }
private:
    void loadModel(std::string const& shortPath);

    void processNode(aiNode* node, const aiScene* scene, aiMatrix4x4& parentTransform);

    Ref<Mesh> processMesh(aiMesh* mesh, const aiScene* scene, const aiMatrix4x4& nodeTransform);
        
    // checks all material textures of a given type and 
    // loads the textures if they're not loaded yet.
    // the required info is returned as a STexture struct.
    std::vector<std::string> loadMaterialTextures(aiMaterial* mat,
        aiTextureType type);
private:
    std::vector<std::string> m_TexturesLoaded;
    std::vector<Ref<Mesh>>        m_Meshes;
    std::string              m_Directory;
    bool                     m_GammaCorrection;
private:
    static constexpr const char* BASE_MODEL_PATH = "res/models/";
};
