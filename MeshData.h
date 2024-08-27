#pragma once

#include "framework.h"
#include "struct.h"

class MeshData
{
public:
	std::vector<Vertex3D> vertices;
	std::vector<uint32_t> indices;

	std::vector<Vertex3D> normalVertices;
	std::vector<uint32_t> normalIndices;

public:
	MeshData();
	~MeshData();

	std::vector<Vertex3D>& GetVertices() { return vertices; };
	std::vector<uint32_t>& GetIndices() { return indices; };

	std::vector<Vertex3D>& GetNormalVertices() { return normalVertices; };
	std::vector<uint32_t>& GetNormalIndices() { return normalIndices; };
};