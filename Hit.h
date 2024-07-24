#pragma once

#include "framework.h"

class Object;

class Hit
{
public:
	float dist;
	glm::vec3 hitPos;
	glm::vec3 normal;

	std::shared_ptr<Object> object;
};