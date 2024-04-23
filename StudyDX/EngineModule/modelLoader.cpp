#include "Precompiled.h"

using namespace DirectX::SimpleMath;

void ModelLoader::Load(std::string basePath, std::string filename) {
    this->basePath = basePath;

    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(
        this->basePath + filename,
        aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

    if (!pScene) {
        std::cout << "Failed to read file: " << this->basePath + filename
            << std::endl;
    }
    else {
        Matrix tr; // Initial transformation
        ProcessNode(pScene->mRootNode, pScene, tr);
    }

    // https://github.com/microsoft/DirectXMesh/wiki/ComputeNormals
    // ComputeNormals()과 비슷합니다.

    // 노멀 벡터가 없는 경우를 대비하여 다시 계산
    // 한 위치에는 한 버텍스만 있어야 연결 관계를 찾을 수 있음
    /* for (auto &m : this->meshes) {

        vector<Vector3> normalsTemp(m.vertices.size(), Vector3(0.0f));
        vector<float> weightsTemp(m.vertices.size(), 0.0f);

        for (int i = 0; i < m._Indices.size(); i += 3) {

            int idx0 = m._Indices[i];
            int idx1 = m._Indices[i + 1];
            int idx2 = m._Indices[i + 2];

            auto v0 = m.vertices[idx0];
            auto v1 = m.vertices[idx1];
            auto v2 = m.vertices[idx2];

            auto faceNormal =
                (v1.position - v0.position).Cross(v2.position - v0.position);

            normalsTemp[idx0] += faceNormal;
            normalsTemp[idx1] += faceNormal;
            normalsTemp[idx2] += faceNormal;
            weightsTemp[idx0] += 1.0f;
            weightsTemp[idx1] += 1.0f;
            weightsTemp[idx2] += 1.0f;
        }

        for (int i = 0; i < m.vertices.size(); i++) {
            if (weightsTemp[i] > 0.0f) {
                m.vertices[i].normalModel = normalsTemp[i] / weightsTemp[i];
                m.vertices[i].normalModel.Normalize();
            }
        }
    }*/

    UpdateTangents();
}

void ModelLoader::UpdateTangents() {

    using namespace std;
    using namespace DirectX;

    // https://github.com/microsoft/DirectXMesh/wiki/ComputeTangentFrame

    for (auto& m : this->meshes) {

        vector<XMFLOAT3> positions(m._Vertices.size());
        vector<XMFLOAT3> normals(m._Vertices.size());
        vector<XMFLOAT2> texcoords(m._Vertices.size());
        vector<XMFLOAT3> tangents(m._Vertices.size());
        vector<XMFLOAT3> bitangents(m._Vertices.size());

        for (size_t i = 0; i < m._Vertices.size(); i++) {
            auto& v = m._Vertices[i];
            positions[i] = v.position;
            normals[i] = v.normalModel;
            texcoords[i] = v.texcoord;
        }

        ComputeTangentFrame(m._Indices.data(), m._Indices.size() / 3,
            positions.data(), normals.data(), texcoords.data(),
            m._Vertices.size(), tangents.data(),
            bitangents.data());

        for (size_t i = 0; i < m._Vertices.size(); i++) {
            m._Vertices[i].tangentModel = tangents[i];
        }
    }
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, Matrix tr) {

    // std::cout << node->mName.C_Str() << " : " << node->mNumMeshes << " "
    //           << node->mNumChildren << std::endl;

    Matrix m;
    ai_real* temp = &node->mTransformation.a1;
    float* mTemp = &m._11;
    for (int t = 0; t < 16; t++) {
        mTemp[t] = float(temp[t]);
    }
    m = m.Transpose() * tr;

    for (UINT i = 0; i < node->mNumMeshes; i++) {

        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        auto newMesh = this->ProcessMesh(mesh, scene);

        for (auto& v : newMesh._Vertices) {
            v.position = DirectX::SimpleMath::Vector3::Transform(v.position, m);
        }

        meshes.push_back(newMesh);
    }

    for (UINT i = 0; i < node->mNumChildren; i++) {
        this->ProcessNode(node->mChildren[i], scene, m);
    }
}

std::string ModelLoader::ReadFilename(aiMaterial* material, aiTextureType type) {

    if (material->GetTextureCount(type) > 0) {
        aiString filepath;
        material->GetTexture(type, 0, &filepath);

        std::string fullPath =
            this->basePath +
            std::string(
                std::filesystem::path(filepath.C_Str()).filename().string());

        return fullPath;
    }
    else {
        return "";
    }
}

MeshData ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene) {

    // Data to fill
    std::vector<Vertex> _Vertices;
    std::vector<uint32_t> _Indices;

    // Walk through each of the mesh's _Vertices
    for (UINT i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.normalModel.x = mesh->mNormals[i].x;
        vertex.normalModel.y = mesh->mNormals[i].y;
        vertex.normalModel.z = mesh->mNormals[i].z;
        //vertex.normalModel.y = mesh->mNormals[i].z;
        //vertex.normalModel.z = -mesh->mNormals[i].y;
        vertex.normalModel.Normalize();

        if (mesh->mTextureCoords[0]) {
            vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
        }

        _Vertices.push_back(vertex);
    }

    for (UINT i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++)
            _Indices.push_back(face.mIndices[j]);
    }

    MeshData newMesh;
    newMesh._Vertices = _Vertices;
    newMesh._Indices = _Indices;

    // http://assimp.sourceforge.net/lib_html/materials.html
    if (mesh->mMaterialIndex >= 0) {

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        newMesh._Textures[(int)TextureType::albedo] =
            ReadFilename(material, aiTextureType_BASE_COLOR);
        newMesh._Textures[(int)TextureType::emissive] =
            ReadFilename(material, aiTextureType_EMISSIVE);
        newMesh._Textures[(int)TextureType::height] =
            ReadFilename(material, aiTextureType_HEIGHT);
        newMesh._Textures[(int)TextureType::normal] =
            ReadFilename(material, aiTextureType_NORMALS);
        newMesh._Textures[(int)TextureType::metallic] =
            ReadFilename(material, aiTextureType_METALNESS);
        newMesh._Textures[(int)TextureType::roughness] =
            ReadFilename(material, aiTextureType_DIFFUSE_ROUGHNESS);
        newMesh._Textures[(int)TextureType::ao] =
            ReadFilename(material, aiTextureType_AMBIENT_OCCLUSION);
    }

    return newMesh;
}
