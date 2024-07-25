#pragma once

#include "framework.h"

#include "Hit.h"
#include "Ray.h"

class Object
{
public:
	glm::vec3 color = glm::vec3(1.0f);
	glm::vec3 ambient = glm::vec3(0.0f);
	glm::vec3 diffuse = glm::vec3(0.0f);
	glm::vec3 specular = glm::vec3(0.0f);

	float alpha = 10.0f;
	float ks = 0.0f;

	float reflection = 0.0f;

	Object(const glm::vec3& color)
		: ambient(color)
		, diffuse(color)
		, specular(color)
	{}

	virtual Hit CheckRayCollision(Ray& ray) = 0;
};