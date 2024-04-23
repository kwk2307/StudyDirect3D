#pragma once
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

class ModelLoader
{
public:
    void Load(std::string basePath, std::string filename);

    void ProcessNode(aiNode* node, const aiScene* scene,
        DirectX::SimpleMath::Matrix tr);

    MeshData ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::string ReadFilename(aiMaterial* material, aiTextureType type);

    void UpdateTangents();

public:
    std::string basePath;
    std::vector<MeshData> meshes;
};

