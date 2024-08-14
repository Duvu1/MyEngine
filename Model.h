#pragma once

#include "framework.h"
#include "Appbase.h"

class Model
{
private:
	std::vector<Vertex3D> vertices;
	std::vector<uint16_t> indices;

public:
	Model();
	~Model();

	const std::vector<Vertex3D>& GetVertices() { return vertices; };
	const std::vector<uint16_t>& GetIndices() { return indices;	};

	void MakeBox();
};