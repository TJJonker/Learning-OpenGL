#include "TexturePlane.h"

TexturePlane::TexturePlane(std::shared_ptr<Texture> texture)
{
    std::vector<Vertex> vertexList;
    vertexList.push_back({ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) });
    vertexList.push_back({ glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) });
    vertexList.push_back({ glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) });
    vertexList.push_back({ glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) });

    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    std::vector<std::shared_ptr<Texture>> textures;
    textures.push_back(texture);

    mesh = new Mesh(vertexList, indices, textures);
}

TexturePlane::~TexturePlane()
{
    delete mesh;
}

void TexturePlane::Draw(Shader& shader)
{
    mesh->Draw(shader);
}
