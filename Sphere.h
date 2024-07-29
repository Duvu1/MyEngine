#pragma once

#include "Object.h"

class Sphere : public Object
{
public:
	glm::vec3	center;
	float		radius;

public:
	Sphere(glm::vec3 center, float radius, glm::vec3 color)
		: center(center)
		, radius(radius)
		, Object(color)
	{
	}

	// when draw 2D circle
	bool IsInside(const glm::vec2& pos)
	{
		glm::vec2 center2D = { center.x, center.y };

		if (glm::dot(pos - center2D, pos - center2D) < radius * radius)
			return true;
		else
			return false;
	}

	// when draw 3D sphere
	Hit CheckRayCollision(MyRay& ray)
	{
		Hit hit = Hit{ -1.0f, glm::vec3(0.0f), glm::vec3(0.0f) }; // d�� �����̸� �浹���� ���� ��

		const float b = 2.0f * glm::dot(ray.dir, ray.start - this->center);
		const float c = glm::dot(ray.start - this->center, ray.start - this->center) - this->radius * this->radius;
		const float nabla = b * b / 4.0f - c;

		if (nabla >= 0.0f)
		{
			const float d1 = -b / 2.0f + sqrt(nabla);
			const float d2 = -b / 2.0f - sqrt(nabla);

			hit.dist = glm::min(d1, d2);
			hit.hitPos = ray.start + ray.dir * hit.dist;
			hit.normal = glm::normalize(hit.hitPos - this->center);
		}

		return hit;
	}
};