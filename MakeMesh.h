#pragma once

#include "framework.h"
#include "struct.h"

#include "MeshData.h"
#include "ModelLoader.h"

class MakeMesh {
public:
	static std::vector<MeshData> ReadFromFile(
		std::string basePath,
		std::string filename
	);

	static MeshData MakeNormal(
		const std::vector<Vertex3D>& vertices,
		std::vector<Vertex3D>& normalVertices,
		std::vector<uint32_t>& normalIndices
	);

	static MeshData MakeBox(const float scale);

	static MeshData MakeCylinder(
		std::vector<Vertex3D>& vertices,
		std::vector<uint32_t>& indices,
		const float radius, 
		const float height
	);

	static MeshData MakeSphere(
		std::vector<Vertex3D>& vertices,
		std::vector<uint32_t>& indices,
		const float radius
	);
};