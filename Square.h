#pragma once

#include "framework.h"

#include "Triangle.h"

class Square : public Object
{
public:
	Triangle triangle0;
	Triangle triangle1;

	Square(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 color)
		: triangle0(Triangle(v0, v1, v2, color))
		, triangle1(Triangle(v0, v2, v3, color))
		, Object(color)
	{
	}

	Hit CheckRayCollision(MyRay& ray)
	{
		const auto hit0 = triangle0.CheckRayCollision(ray);
		const auto hit1 = triangle1.CheckRayCollision(ray);

		if (hit0.dist >= 0.0f && hit1.dist >= 0.0f)
		{
			return hit0.dist < hit1.dist ? hit0 : hit1;
		}
		else if (hit0.dist >= 0.0f)
		{
			return hit0;
		}
		else
		{
			return hit1;
		}
	}
};