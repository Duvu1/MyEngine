#pragma once

#include "Hit.h"
#include "Ray.h"

class Sphere
{
public:
	glm::vec3 center;
	float radius;

	glm::vec3 color = glm::vec3(1.0f);
	glm::vec3 ambient = glm::vec3(0.0f);
	glm::vec3 diffuse = glm::vec3(0.0f);
	glm::vec3 specular = glm::vec3(0.0f);

	float alpha = 0.0f;
	float ks = 0.0f;

public:
	Sphere(glm::vec3 center, float radius)
		: center(center)
		, radius(radius)
	{}

	Sphere(glm::vec3 center, float radius, glm::vec3 color)
		: center(center)
		, radius(radius)
		, color(color)
	{}

	Hit IntersectRayCollision(Ray& ray)
	{
		Hit hit = Hit{ -1.0f, glm::vec3(0.0f), glm::vec3(0.0f) }; // d가 음수이면 충돌하지 않은 것

		const float b = 2.0f * glm::dot(ray.dir, ray.start - this->center);
		const float c = glm::dot(ray.start - this->center, ray.start - this->center) - this->radius * this->radius;
		const float nabla = b * b / 4.0f - c;

		if (nabla >= 0.0f)
		{
			const float d1 = -b / 2.0f + sqrt(nabla);
			const float d2 = -b / 2.0f - sqrt(nabla);

			hit.dist = glm::min(d1, d2); // 둘 중 작은 것
			hit.hitPos = ray.start + ray.dir * hit.dist;
			hit.normal = glm::normalize(hit.hitPos - this->center);
		}

		return hit;
	}
};