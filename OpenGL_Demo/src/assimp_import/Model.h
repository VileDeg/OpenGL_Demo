#pragma once

#include <assimp/scene.h>
#include <assimp_import/Mesh.h>

namespace AssimpImport 
{
    using namespace std;

    class Model
    {
    public:
        vector<STexture> textures_loaded;
        vector<Mesh>    meshes;
        string directory;
        bool gammaCorrection;

        Model(string const& path, bool gamma = false);

        void Draw(Shader& shader);
    private:
        void loadModel(string const& shortPath);

        void processNode(aiNode* node, const aiScene* scene);

        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        
        // checks all material textures of a given type and 
        // loads the textures if they're not loaded yet.
        // the required info is returned as a STexture struct.
        vector<STexture> loadMaterialTextures(aiMaterial* mat,
            aiTextureType type, string typeName);
    private:
        static constexpr const char* BASE_MODEL_PATH = "res/models/";
    };
}