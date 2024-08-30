#include "MakeMesh.h"

using namespace std;

vector<MeshData> MakeMesh::ReadFromFile(string basePath, string filename)
{
    ModelLoader modelLoader;
    modelLoader.LoadOBJ(basePath, filename);
    vector<MeshData>& meshes = modelLoader.meshes;

    Vector3 vMin(1000, 1000, 1000);
    Vector3 vMax(-1000, -1000, -1000);

    // scale in proper size
    for (auto& mesh : meshes)
        for (auto& v : mesh.vertices)
        {
            vMin.x = XMMin(vMin.x, v.position.x);
            vMin.y = XMMin(vMin.y, v.position.y);
            vMin.z = XMMin(vMin.z, v.position.z);
            vMax.x = XMMin(vMax.x, v.position.x);
            vMax.y = XMMin(vMax.y, v.position.y);
            vMax.z = XMMin(vMax.z, v.position.z);
        }

    float dx = vMax.x - vMin.x;
    float dy = vMax.y - vMin.y;
    float dz = vMax.z - vMin.z;
    float dl = XMMax(XMMax(dx, dy), dz);

    float cx = (vMax.x + vMin.x) * 0.5f;
    float cy = (vMax.y + vMin.y) * 0.5f;
    float cz = (vMax.z + vMin.z) * 0.5f;

    //for (auto& mesh : meshes)
    //    for (auto& v : mesh.vertices)
    //    {
    //        v.position.x = (v.position.x - cx) / dl;
    //        v.position.y = (v.position.y - cy) / dl;
    //        v.position.z = (v.position.z - cz) / dl;
    //    }

    return meshes;
}

MeshData MakeMesh::MakeNormal(const vector<Vertex3D>& vertices,
    vector<Vertex3D>& normalVertices,
    vector<uint32_t>& normalIndices)
{
    for (size_t i = 0; i < vertices.size(); i++)
    {
        Vertex3D vertex = vertices[i];

        vertex.texcoord.x = 0.0f;
        normalVertices.push_back(vertex);
        normalIndices.push_back(uint32_t(i * 2));

        vertex.texcoord.x = 1.0f;
        normalVertices.push_back(vertex);
        normalIndices.push_back(uint32_t(i * 2 + 1));
    }

    return MeshData();
}

MeshData MakeMesh::MakeBox(const float scale)
{
    vector<Vector3> positions;
    vector<Vector3> normals;
    vector<Vector2> texcoords;

    // À­¸é
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));

    // ¾Æ·§¸é
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));

    // ¾Õ¸é
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));

    // µÞ¸é
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));

    // ¿ÞÂÊ
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));

    // ¿À¸¥ÂÊ
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));

    MeshData meshData;

    for (size_t i = 0; i < positions.size(); i++)
    {
        Vertex3D vertex;
        vertex.position = positions[i];
        vertex.normal = normals[i];
        vertex.texcoord = texcoords[i];
        meshData.vertices.push_back(vertex);
    }

   meshData.indices = {
        0,  1,  2,  0,  2,  3,  // À­¸é
        4,  5,  6,  4,  6,  7,  // ¾Æ·§¸é
        8,  9,  10, 8,  10, 11, // ¾Õ¸é
        12, 13, 14, 12, 14, 15, // µÞ¸é
        16, 17, 18, 16, 18, 19, // ¿ÞÂÊ
        20, 21, 22, 20, 22, 23  // ¿À¸¥ÂÊ
    };

   return meshData;
}

MeshData MakeMesh::MakeCylinder(vector<Vertex3D>& vertices, vector<uint32_t>& indices, const float radius, const float height)
{
    vector<Vector3> positions;
    vector<Vector3> normals;
    vector<Vector2> texcoords;

    return MeshData();
}

MeshData MakeMesh::MakeSphere(vector<Vertex3D>& vertices, vector<uint32_t>& indices, const float radius)
{
    vector<Vector3> positions;
    vector<Vector3> normals;
    vector<Vector2> texcoords;

    return MeshData();
}