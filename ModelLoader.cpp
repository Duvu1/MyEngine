#include "ModelLoader.h"

#include <filesystem>
#include <fstream>

#include "framework.h"

using namespace std;

void ModelLoader::Load(std::string basePath, std::string filename)
{
	this->basePath = basePath;

	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(
		this->basePath + filename,
		aiProcess_Triangulate | aiProcess_ConvertToLeftHanded
	);

	Matrix initMatrix;
	ProcessNode(pScene->mRootNode, pScene, initMatrix);
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, Matrix matrix)
{
	Matrix m;
	ai_real* temp = &node->mTransformation.a1;
	float* mTemp = &m._11;

	for (int t = 0; t < 16; t++)
		mTemp[t] = float(temp[t]);

	m = m.Transpose() * matrix;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		auto newMesh = this->ProcessMesh(mesh, scene);

		for (auto& v : newMesh.vertices)
			v.position = DirectX::SimpleMath::Vector3::Transform(v.position, m);
		
		meshes.push_back(newMesh);
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
		this->ProcessNode(node->mChildren[i], scene, m);
}

MeshData ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex3D> vertices;
	std::vector<uint32_t> indices;

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex3D vertex;

		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;
		vertex.normal.Normalize();

		if (mesh->mTextureCoords[0])
		{
			vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[i]);
	}

	MeshData newMesh;
	newMesh.vertices = vertices;
	newMesh.indices = indices;

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString filepath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &filepath);

			std::string fullPath = 
				this->basePath +
				std::string(std::filesystem::path(filepath.C_Str()).filename().string());
		}
	}

	return newMesh;
}


void ModelLoader::LoadOBJ(std::string basePath, std::string filename)
{
    ifstream fin;
    char input = 0;
    //vector<MeshData> meshes;

    // count
    int vertexCount = 0;
    int textureCount = 0;
    int normalCount = 0;
    int indexCount = 0;
    //vector<int> vertexCount;
    //vector<int> textureCount;
    //vector<int> normalCount;
    //vector<int> indexCount;
    //int meshIndex = -1;

    fin.open(basePath + filename);

    if (fin.fail()) return;

    fin.get(input);
    while (!fin.eof())
    {
        //if (input == 'o')
        //{
        //    meshIndex++;
        //    vertexCount.push_back(0);
        //    textureCount.push_back(0);
        //    normalCount.push_back(0);
        //    indexCount.push_back(0);
        //}

        if (input == 'v')
        {
            fin.get(input);
            if (input == ' ')
                vertexCount++;
                //vertexCount[meshIndex]++;
            if (input == 'n')
                normalCount++;
                //normalCount[meshIndex]++;
            if (input == 't')
                textureCount++;
                //textureCount[meshIndex]++;
        }

        if (input == 'f')
        {
            fin.get(input);

            while (input != '\n')
            {
                if (input == ' ')
                    indexCount++;
                    //indexCount[meshIndex]++;

                fin.get(input);
            }
        }

        while (input != '\n')
            fin.get(input);

        fin.get(input);
    }

    fin.close();

    // load
    MeshData meshData;

    vector<Vector3> vertices(vertexCount);
    //vector<Vector3> vertices;
    int vertexIndex = 0;

    vector<Vector3> normals(normalCount);
    //vector<Vector3> normals;
    int normalIndex = 0;

    vector<Vector2> texcoords(textureCount);
    //vector<Vector2> texcoords;
    int texcoordsIndex = 0;

    vector<uint32_t> indices;
    //vector<uint32_t> indices;
    int indexIndex = 0;

    int i = -1;

    fin.open(basePath + filename);

    fin.get(input);
    while (!fin.eof())
    {
        //if (input == 'o')
        //{
        //    i++;
        //    vertices.resize(vertexCount[i]);
        //    vertexIndex = 0;
        //    normals.resize(normalCount[i]);
        //    normalIndex = 0;
        //    texcoords.resize(textureCount[i]);
        //    texcoordsIndex = 0;
        //    indices.resize(indexCount[i]);
        //    indexIndex = 0;
        //}

        if (input == 'v')
        {
            fin.get(input);

            Vector3 vertex;
            if (input == ' ')
            {
                fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;
                vertexIndex++;
            }

            if (input == 'n')
            {
                fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;
                normalIndex++;
            }

            if (input == 't')
            {
                fin >> texcoords[texcoordsIndex].x >> texcoords[texcoordsIndex].y;
                texcoordsIndex++;
            }
        }

        char inputTemp = 0;
        if (input == 'f')
        {
            int vertexPerLine = 0;

            while (input != '\n')
            {
                Vertex3D vertex;

                int num;
                fin >> num >> inputTemp;
                vertex.position = vertices[num - 1];

                fin >> num >> inputTemp;
                vertex.texcoord = texcoords[num - 1];

                fin >> num;
                vertex.normal = normals[num - 1];

                meshData.vertices.push_back(vertex);

                vertexPerLine++;

                fin.get(input);
            }

            for (int i = 0; i < vertexPerLine - 2; i++)
            {
                indices.push_back((uint32_t)indexIndex);
                indices.push_back((uint32_t)(indexIndex + i + 1));

                if (i == 29)
                    int a = 0;

                indices.push_back((uint32_t)(indexIndex + i + 2));
            }
            indexIndex += vertexPerLine;
        }

        while (input != '\n')
            fin.get(input);

        fin.get(input);
    }

    fin.close();

    meshData.indices = indices;
    meshes.push_back(meshData);
}
