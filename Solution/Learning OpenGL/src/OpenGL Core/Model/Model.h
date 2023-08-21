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
	std::map<std::string, std::shared_ptr<Texture>> m_LoadedTextures;

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Vertex> GetVertexInformation(aiMesh* mesh);
	std::vector<unsigned int> GetIndices(aiMesh* mesh);
	std::vector<std::shared_ptr<Texture>> GetTextures(aiMesh* mesh, const aiScene* scene);
	std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* mat, aiTextureType aiType, Texture::TextureType textureType);
	void LoadAndInsertTextures(std::vector<std::shared_ptr<Texture>>& textures, aiMaterial* mat, aiTextureType aiType, Texture::TextureType textureType);

public:
	Model(const std::string path);
	void Draw(Shader& shader);
};

