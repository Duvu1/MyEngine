#pragma once

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

#include "framework.h"
#include "struct.h"

#include "MeshData.h"

class ModelLoader
{
public:
	void Load(std::string basePath, std::string filename);
	void ProcessNode(aiNode* node, const aiScene* scene,
		DirectX::SimpleMath::Matrix matrix);
	MeshData ProcessMesh(aiMesh* mesh, const aiScene* scene);

	void LoadOBJ(
		std::string basePath,
		std::string filename
	);

public:
	std::string basePath;
	std::vector<MeshData> meshes;
};

