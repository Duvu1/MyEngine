#include "Model.h"

using namespace std;

Model::Model()
{

}

Model::~Model()
{

}

void Model::MakeBox()
{
    vector<Vector3> positions;
    vector<Vector3> colors;
    vector<Vector3> normals;

    const float scale = 1.0f;

    // ����
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    //colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    //colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    //colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    //colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));

    // �Ʒ���
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    //colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    //colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    //colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    //colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));

    // �ո�
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    //colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    //colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    //colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    //colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));

    // �޸�
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    //colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    //colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    //colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    //colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));

    // ����
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    //colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    //colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    //colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    //colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));

    // ������
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    //colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    //colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    //colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    //colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    colors.push_back(Vector3(0.5f, 0.5f, 0.5f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));

    for (size_t i = 0; i < positions.size(); i++)
    {
        Vertex3D vertex;
        vertex.position = positions[i];
        vertex.color = colors[i];
        vertex.normal = normals[i];
        vertices.push_back(vertex);
    }

    vector<uint16_t> index = {
        0,  1,  2,  0,  2,  3,  // ����
        4,  5,  6,  4,  6,  7,  // �Ʒ���
        8,  9,  10, 8,  10, 11, // �ո�
        12, 13, 14, 12, 14, 15, // �޸�
        16, 17, 18, 16, 18, 19, // ����
        20, 21, 22, 20, 22, 23  // ������
    };

    for (size_t i = 0; i < index.size(); i++)
        indices.push_back(index[i]);
}