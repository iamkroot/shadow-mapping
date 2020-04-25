#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int vao{0}, vbo{0}, ebo{0};
public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);

    void draw();
};

class ObjModel {
    std::vector<Mesh> meshes;

    /**
     * @brief Parse and load the .obj file using ASSIMP.
     * @param path
     */
    void load(const char* path);

    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

public:
    explicit ObjModel(const char* path);

    /**
     * @brief Draws all meshes of the model
     */
    void draw();
};


#endif //OBJMODEL_H
