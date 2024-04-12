#include "Precompiled.h"
using std::vector;

std::vector<MeshData> GeometryGenerator::ReadFromFile(std::string InBasePath, std::string Infilename)
{
    using namespace DirectX;

    ModelLoader modelLoader;
    modelLoader.Load(InBasePath, Infilename);
    vector<MeshData>& meshes = modelLoader.meshes;

    // Normalize vertices
    Vector3 vmin(1000, 1000, 1000);
    Vector3 vmax(-1000, -1000, -1000);
    for (auto& mesh : meshes) {
        for (auto& v : mesh._Vertices) {
            vmin.x = XMMin(vmin.x, v.position.x);
            vmin.y = XMMin(vmin.y, v.position.y);
            vmin.z = XMMin(vmin.z, v.position.z);
            vmax.x = XMMax(vmax.x, v.position.x);
            vmax.y = XMMax(vmax.y, v.position.y);
            vmax.z = XMMax(vmax.z, v.position.z);
        }
    }

    float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
    float dl = XMMax(XMMax(dx, dy), dz);
    float cx = (vmax.x + vmin.x) * 0.5f, cy = (vmax.y + vmin.y) * 0.5f,
        cz = (vmax.z + vmin.z) * 0.5f;

    for (auto& mesh : meshes) {
        for (auto& v : mesh._Vertices) {
            v.position.x = (v.position.x - cx) / dl;
            v.position.y = (v.position.y - cy) / dl;
            v.position.z = (v.position.z - cz) / dl;
        }
    }

    return meshes;
}

MeshData GeometryGenerator::MakeSquare()
{
    vector<Vector3> positions;
    vector<Vector3> colors;
    vector<Vector3> normals;
    vector<Vector2> texcoords; // �ؽ��� ��ǥ

    const float scale = 1.f;

    // �ո�
    positions.push_back(Vector3(-1.0f, 1.0f, 0.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 0.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 0.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, 0.0f) * scale);
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));

    // Texture Coordinates (Direct3D 9)
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/texture-coordinates
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    MeshData meshData;

    for (size_t i = 0; i < positions.size(); i++) {
        Vertex v;
        v.position = positions[i];
        v.normal = normals[i];
        v.texcoord = texcoords[i];

        // v.color = colors[i];

        meshData._Vertices.push_back(v);
    }
    meshData._Indices = {
        0, 1, 2, 0, 2, 3, // �ո�
    };

    return meshData;
}

MeshData GeometryGenerator::MakeBox()
{
    vector<Vector3> positions;
    vector<Vector3> colors;
    vector<Vector3> normals;
    vector<Vector2> texcoords; // �ؽ��� ��ǥ

    const float scale = 1.f;

    // ����
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // �Ʒ���
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // �ո�
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // �޸�
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // ����
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // ������
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    MeshData meshData;
    for (size_t i = 0; i < positions.size(); i++) {
        Vertex v;
        v.position = positions[i];
        v.normal = normals[i];
        v.texcoord = texcoords[i];
        meshData._Vertices.push_back(v);
    }

    meshData._Indices = {
        0,  1,  2,  0,  2,  3,  // ����
        4,  5,  6,  4,  6,  7,  // �Ʒ���
        8,  9,  10, 8,  10, 11, // �ո�
        12, 13, 14, 12, 14, 15, // �޸�
        16, 17, 18, 16, 18, 19, // ����
        20, 21, 22, 20, 22, 23  // ������
    };

    return meshData;
}

MeshData GeometryGenerator::MakeGrid(const float width, const float height, const int numSlices, const int numStacks)
{
    const float dx = width / numSlices;
    const float dy = height / numStacks;

    MeshData meshData;

    vector<Vertex>& _Vertices = meshData._Vertices;
    vector<uint32_t>& _Indices = meshData._Indices;

    //// y = -0.5f * height �� ����
    // Vector3 stackStartPoint = Vector3(-0.5f * width, -0.5f * height, 0.0f);
    // for (int i = 0; i <= numSlices; i++) {
    //     Vertex v;

    //    // x-y ��鿡�� �������� x �������� �̵�
    //    v.position = stackStartPoint + Vector3(dx * i, 0.0f, 0.0f);

    //    // ������ ���ϴ� ����
    //    v.normal = Vector3(0.0f, 0.0f, -1.0f);

    //    v.texcoord = Vector2(1.0f / numSlices * i, 0.0f);

    //    _Vertices.push_back(v);
    //}

    //// y = 0.5f * height �� ����
    // stackStartPoint = Vector3(-0.5f * width, 0.5f * height, 0.0f);
    // for (int i = 0; i <= numSlices; i++) {
    //     Vertex v;

    //    // x-y ��鿡�� �������� x �������� �̵�
    //    v.position = stackStartPoint + Vector3(dx * i, 0.0f, 0.0f);

    //    // ������ ���ϴ� ����
    //    v.normal = Vector3(0.0f, 0.0f, -1.0f);

    //    v.texcoord = Vector2(1.0f / numSlices * i, 1.0f);

    //    _Vertices.push_back(v);
    //}

    // y = -0.5f * height �� ����

    Vector3 stackStartPoint = Vector3(-0.5f * width, -0.5f * height, 0.0f);
    for (int j = 0; j <= numStacks; ++j) {
        for (int i = 0; i <= numSlices; i++) {
            Vertex v;

            // x-y ��鿡�� �������� x �������� �̵�
            v.position = stackStartPoint + Vector3(dx * i, dy * j, 0.0f);

            // ������ ���ϴ� ����
            v.normal = Vector3(0.0f, 0.0f, -1.0f);

            v.texcoord = Vector2(1.0f / numSlices * i, 1.0f / numStacks * j);

            _Vertices.push_back(v);
        }
    }

    // �ε��� �߰�
    for (int j = 0; j < numStacks; ++j) {
        for (int i = 0; i < numSlices; i++) {
            // ù��° �ﰢ��
            _Indices.push_back(i + (numSlices + 1) * j);           // 0 6
            _Indices.push_back(i + 1 + (numSlices + 1) * j);       // 1 7
            _Indices.push_back(i + 1 + (numSlices + 1) * (j + 1)); // 7 13

            // �� ��° �ﰢ��
            _Indices.push_back(i + (numSlices + 1) * j);           // 0 6
            _Indices.push_back(i + 1 + (numSlices + 1) * (j + 1)); // 7 13
            _Indices.push_back(i + (numSlices + 1) * (j + 1));     // 6 12
        }
    }

    return meshData;
}

MeshData GeometryGenerator::MakeCylinder(const float bottomRadius, const float topRadius, float height, int sliceCount)
{
    MeshData meshData;

    vector<Vertex>& _Vertices = meshData._Vertices;
    vector<uint32_t>& _Indices = meshData._Indices;

    for (int i = 0; i <= sliceCount; ++i) {
        Vertex v;

        v.position =
            Vector3::Transform(Vector3(bottomRadius, height * (-0.5f), 0),
                Matrix::CreateRotationY(DirectX::XMConvertToRadians(
                    360.0f / sliceCount * i)));

        v.normal = v.position - Vector3(0.0f, -0.5f * height, 0.0f);
        v.normal.Normalize();

        v.texcoord = Vector2(1.0f / sliceCount * i, 0.0f);

        _Vertices.push_back(v);
    }

    for (int i = 0; i <= sliceCount; ++i) {
        Vertex v;

        v.position =
            Vector3::Transform(Vector3(topRadius, height * (0.5f), 0),
                Matrix::CreateRotationY(DirectX::XMConvertToRadians(
                    360.0f / sliceCount * i)));

        v.normal = v.position - Vector3(0.0f, 0.5f * height, 0.0f);
        v.normal.Normalize();

        v.texcoord = Vector2(1.0f / sliceCount * i, 1.0f);

        _Vertices.push_back(v);
    }

    for (int i = 0, j = 0; i < sliceCount; i++) {
        // ù��° �ﰢ��
        _Indices.push_back(i + (sliceCount + 1) * j);           // 0 6
        _Indices.push_back(i + 1 + (sliceCount + 1) * j);       // 1 7
        _Indices.push_back(i + 1 + (sliceCount + 1) * (j + 1)); // 7 13

        // �� ��° �ﰢ��
        _Indices.push_back(i + (sliceCount + 1) * j);           // 0 6
        _Indices.push_back(i + 1 + (sliceCount + 1) * (j + 1)); // 7 13
        _Indices.push_back(i + (sliceCount + 1) * (j + 1));     // 6 12
    }

    return meshData;
}

MeshData GeometryGenerator::MakeSphere(const float radius, const int numSlices, const int numStacks)
{
    MeshData meshData;

    vector<Vertex>& _Vertices = meshData._Vertices;
    vector<uint32_t>& _Indices = meshData._Indices;

    for (int j = 0; j <= numStacks; ++j) {
        for (int i = 0; i <= numSlices; i++) {
            Vertex v;
            Vector3 pos(
                radius * DirectX::XMScalarCos(DirectX::XM_PIDIV2 -
                    j * (DirectX::XM_PI / numStacks)),
                radius * DirectX::XMScalarSin(DirectX::XM_PIDIV2 -
                    j * (DirectX::XM_PI / numStacks)),
                0.0f);

            v.position = Vector3::Transform(
                pos, Matrix::CreateRotationY(
                    DirectX::XMConvertToRadians(360.0f / numSlices * i)));

            v.normal = v.position - Vector3(0.0f, 0.0f, 0.0f);
            v.normal.Normalize();

            v.texcoord = Vector2(1.0f / numSlices * i, 1.0f / numStacks * j);

            _Vertices.push_back(v);
        }
    }

    // �ε��� �߰�
    for (int j = 0; j < numStacks; ++j) {
        for (int i = 0; i < numSlices; i++) {
            // ù��° �ﰢ��
            _Indices.push_back(i + (numSlices + 1) * j);           // 0 6
            _Indices.push_back(i + 1 + (numSlices + 1) * j);       // 1 7
            _Indices.push_back(i + 1 + (numSlices + 1) * (j + 1)); // 7 13

            // �� ��° �ﰢ��
            _Indices.push_back(i + (numSlices + 1) * j);           // 0 6
            _Indices.push_back(i + 1 + (numSlices + 1) * (j + 1)); // 7 13
            _Indices.push_back(i + (numSlices + 1) * (j + 1));     // 6 12
        }
    }

    return meshData;
}

MeshData GeometryGenerator::SubdivideToSphere(const float radius, MeshData meshData)
{
    using namespace DirectX;
    using DirectX::SimpleMath::Matrix;
    using DirectX::SimpleMath::Vector3;

    auto ProjectVertex = [&](Vertex& v) {
        v.normal = v.position;
        v.normal.Normalize();
        v.position = v.normal * radius;
        };

    auto UpdateFaceNormal = [](Vertex& v0, Vertex& v1, Vertex& v2) {
        Vector3 faceNormal = XMVector3Cross(
            v2.position - v0.position, v1.position - v0.position);

        faceNormal.Normalize();

        v0.normal = faceNormal;
        v1.normal = faceNormal;
        v2.normal = faceNormal;
        };

    MeshData newMesh;
    uint32_t count = 0;

    for (size_t i = 0; i < meshData._Indices.size(); i += 3) {
        size_t i0 = meshData._Indices[i];
        size_t i1 = meshData._Indices[i + 1];
        size_t i2 = meshData._Indices[i + 2];

        Vertex v0 = meshData._Vertices[i0];
        Vertex v1 = meshData._Vertices[i1];
        Vertex v2 = meshData._Vertices[i2];

        Vertex v3;
        v3.position = (v0.position + v2.position) / 2;
        v3.texcoord = (v0.texcoord + v2.texcoord) / 2;

        Vertex v4;
        v4.position = (v0.position + v1.position) / 2;
        v4.texcoord = (v0.texcoord + v1.texcoord) / 2;

        Vertex v5;
        v5.position = (v1.position + v2.position) / 2;
        v5.texcoord = (v1.texcoord + v2.texcoord) / 2;

        ProjectVertex(v3);
        ProjectVertex(v4);
        ProjectVertex(v5);

        //UpdateFaceNormal(v4, v1, v5);
        newMesh._Vertices.push_back(v4);
        newMesh._Vertices.push_back(v1);
        newMesh._Vertices.push_back(v5);

        //UpdateFaceNormal(v0, v4, v3);
        newMesh._Vertices.push_back(v0);
        newMesh._Vertices.push_back(v4);
        newMesh._Vertices.push_back(v3);

        //UpdateFaceNormal(v3, v4, v5);
        newMesh._Vertices.push_back(v3);
        newMesh._Vertices.push_back(v4);
        newMesh._Vertices.push_back(v5);

        //UpdateFaceNormal(v3, v5, v2);
        newMesh._Vertices.push_back(v3);
        newMesh._Vertices.push_back(v5);
        newMesh._Vertices.push_back(v2);

        //�ε��� ������Ʈ
        for (uint32_t j = 0; j < 12; j++) {
            newMesh._Indices.push_back(j + count);
        }
        count += 12;
    }

    return newMesh;
}