#pragma once
#include "../Mesh/Mesh.h"
#include <ASSIMP/Importer.hpp>
#include <ASSIMP/postprocess.h>
#include <ASSIMP/scene.h>
class Model
{
private:
	std::vector<Mesh> m_Meshes;
	std::string m_Directory;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
};

