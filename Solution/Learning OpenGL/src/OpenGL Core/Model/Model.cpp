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
        m_Meshes.push_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]]));
    }

    // Check if the current Node has more child Nodes and recursively call this function.
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh* Model::ProcessMesh(aiMesh* mesh) {

    std::vector<Vertex> vertices = GetVertices(mesh);
    std::vector<unsigned int> indices = GetIndices(mesh);  
        
    // Create a mesh with the collected Vertex structs.    
    return new Mesh(vertices, indices);
}

std::vector<Vertex> Model::GetVertices(aiMesh* mesh) {
    std::vector<Vertex> vertices;

    // Process the mesh and collect the necessary data. 
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        // Place this data in a Vertex struct for easy overview.
        Vertex vertex;

        // Process Vertex Position
        aiVector3D pos = mesh->mVertices[i];
        vertex.Position = { pos.x, pos.y, pos.z };


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

void Model::Draw(Shader& shader)
{
    for (unsigned int i = 0; i < m_Meshes.size(); ++i)
        (*m_Meshes[i]).Draw(shader);
}

