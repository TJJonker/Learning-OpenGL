#include "Model.h"

Model::Model(const std::string path)
{
    // Read file
	Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    // Save path to directory
    m_Directory = path.substr(0, path.find_last_of('/'));

    // Start processing file
    ProcessNode(scene->mRootNode, scene);
}


void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // Iterate over current Node's meshes and process the meshes
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        m_Meshes.push_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene));
    }

    // Check if the current Node has more child Nodes and recursively call this function.
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {

    std::vector<Vertex> vertices = GetVertexInformation(mesh);
    std::vector<unsigned int> indices = GetIndices(mesh);
    std::vector<std::shared_ptr<Texture>> textures = GetTextures(mesh, scene);

        
    // Create a mesh with the collected Vertex structs.    
    return new Mesh(vertices, indices, textures);
}

std::vector<Vertex> Model::GetVertexInformation(aiMesh* mesh) {
    std::vector<Vertex> vertices;

    // Process the mesh and collect the necessary data. 
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        // Place this data in a Vertex struct for easy overview.
        Vertex vertex;

        // Process Vertex Position
        aiVector3D pos = mesh->mVertices[i];
        vertex.Position = { pos.x, pos.y, pos.z };

        // Process Vertex Normals
        aiVector3D norm = mesh->mNormals[i];
        vertex.Normals = { norm.x, norm.y, norm.z };
        
        if (mesh->mTextureCoords[0]) {
            // Process Texture Coords
            aiVector3D texco = mesh->mTextureCoords[0][i];
            vertex.TextureCoords = { texco.x, texco.y };

            aiVector3D tangent = mesh->mTangents[i];
            vertex.Tangent = { tangent.x, tangent.y, tangent.z };

            aiVector3D bitangent = mesh->mBitangents[i];
            vertex.Bitangent = { bitangent.x, bitangent.y, bitangent.z };
        }
        else {
            vertex.TextureCoords = { 0.0, 0.0 };
        }

        // Place all the collected structs inside a vector.
        vertices.push_back(vertex);
    }

    return vertices;
}

std::vector<unsigned int> Model::GetIndices(aiMesh* mesh) {
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return indices;
}

std::vector<std::shared_ptr<Texture>> Model::GetTextures(aiMesh* mesh, const aiScene* scene)
{
    std::vector<std::shared_ptr<Texture>> textures;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    LoadAndInsertTextures(textures, material, aiTextureType_DIFFUSE, Texture::TextureType::DIFFUSE);
    LoadAndInsertTextures(textures, material, aiTextureType_SPECULAR, Texture::TextureType::SPECULAR);
    LoadAndInsertTextures(textures, material, aiTextureType_NORMALS, Texture::TextureType::NORMAL);
    LoadAndInsertTextures(textures, material, aiTextureType_HEIGHT, Texture::TextureType::HEIGHT);

    return textures;
}

void Model::LoadAndInsertTextures(std::vector<std::shared_ptr<Texture>>& textures, aiMaterial* material, aiTextureType aiType, Texture::TextureType textureType)
{
    std::vector<std::shared_ptr<Texture>> textureMaps = LoadMaterialTextures(material, aiType, textureType);
    textures.insert(textures.end(), textureMaps.begin(), textureMaps.end());
}

std::vector<std::shared_ptr<Texture>> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType aiType, Texture::TextureType textureType)
{
    std::vector<std::shared_ptr<Texture>> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(aiType); ++i) {
        aiString path;
        mat->GetTexture(aiType, i, &path);

        // If the texture already exists in the list.
        if (m_LoadedTextures.find(path.C_Str()) != m_LoadedTextures.end()) {
            textures.push_back(m_LoadedTextures[path.C_Str()]);
            std::cout << "Texure Already Loaded!" << std::endl << std::endl;
            continue;
        }

        std::shared_ptr<Texture> texture = std::make_shared<Texture>(m_Directory, path.C_Str(), textureType); 
        m_LoadedTextures.insert({ path.C_Str(), texture});
        textures.push_back(texture);
        std::cout << "Texture not loaded yet..." << std::endl << std::endl;
    }

    return textures;
}

void Model::Draw(Shader& shader)
{
    for (unsigned int i = 0; i < m_Meshes.size(); ++i)
        (*m_Meshes[i]).Draw(shader);
}

