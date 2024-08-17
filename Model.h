#pragma once

#include "framework.h"
#include "Appbase.h"

class Model
{
private:
	std::vector<Vertex3D> vertices;
	std::vector<uint16_t> indices;

	std::vector<Vertex3D> normalVertices;
	std::vector<uint16_t> normalIndices;

public:
	Model();
	~Model();

	std::vector<Vertex3D>& GetVertices() { return vertices; };
	std::vector<uint16_t>& GetIndices() { return indices; };

	std::vector<Vertex3D>& GetNormalVertices() { return normalVertices; };
	std::vector<uint16_t>& GetNormalIndices() { return normalIndices; };
};