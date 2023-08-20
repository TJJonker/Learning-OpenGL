#pragma once
#include "../Mesh/Mesh.h"
#include <ASSIMP/Importer.hpp>
#include <ASSIMP/postprocess.h>
#include <ASSIMP/scene.h>
class Model
{
private:
	std::vector<Mesh*> m_Meshes;
	std::string m_Directory;

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh);
	std::vector<Vertex> GetVertexInformation(aiMesh* mesh);
	std::vector<unsigned int> GetIndices(aiMesh* mesh);

public:
	Model(const std::string path);
	void Draw(Shader& shader);
};

