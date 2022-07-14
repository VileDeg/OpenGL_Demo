#include "pch.h"
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h> 
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <stb_image.h>

Model::Model(std::string const& shortPath, bool gamma)
    : m_GammaCorrection(gamma)
{
    /*auto last = shortPath.find_last_of("/\\");
    m_Directory = shortPath.substr(0, last+1);
    std::cout << "Dir: " << m_Directory << '\n';*/
    stbi_set_flip_vertically_on_load(1);
    loadModel(shortPath);
    
}

void Model::loadModel(std::string const& shortPath)
{
    std::string path = BASE_MODEL_PATH + shortPath;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << '\n';
        return;
    }
    m_Directory = path.substr(0, path.find_last_of('/')+1);
    std::cout << "Dir: " << m_Directory << '\n';

    processNode(scene->mRootNode, scene);

    std::cout << "Finished loading model at path :" << path << ".\n";
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    //std::unordered_map<TexType, const char*> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        //// colors MY ADDITION
        //if (mesh->HasVertexColors(0))
        //{
        //    vertex.Color.x = mesh->mColors[0][i].r;
        //    vertex.Color.y = mesh->mColors[0][i].g;
        //    vertex.Color.z = mesh->mColors[0][i].b;
        //    vertex.Color.w = mesh->mColors[0][i].a;
        //}

        // texture coordinates
        if (mesh->HasTextureCoords(0)) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<std::string> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
    // 2. specular maps
    std::vector<std::string> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
    // 3. normal maps
    std::vector<std::string> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT);
    // 4. height maps
    std::vector<std::string> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT);

    std::unordered_map<TexType, std::vector<std::string>> map;
    for (auto& x : diffuseMaps )
    {
        map[TexType::Diffuse ].push_back(m_Directory + x);
    }
    for (auto& x : specularMaps)
    { 
        map[TexType::Specular].push_back(m_Directory + x);
    }
    for (auto& x : normalMaps  )
    {
        map[TexType::Normal  ].push_back(m_Directory + x);
    }
    for (auto& x : heightMaps  )
    {
        map[TexType::Height  ].push_back(m_Directory + x);
    }

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, map);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a STexture struct.
std::vector<std::string> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
    std::vector<std::string> texturePaths;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        //for (unsigned int j = 0; j < m_TexturesLoaded.size(); j++)
        //{
        //    if (std::strcmp(m_TexturesLoaded[j].data(), str.C_Str()) == 0)
        //    {
        //        texturePaths.push_back(m_TexturesLoaded[j]);
        //        skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
        //        break;
        //    }
        //}
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            texturePaths.push_back(str.C_Str());
            m_TexturesLoaded.push_back(str.C_Str());  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return texturePaths;
}
