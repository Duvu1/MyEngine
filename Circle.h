#pragma once

#include "framework.h"

class Circle
{
public:
	glm::vec2 center;
	float radius;
	glm::vec4 color;

	Circle(glm::vec2 center, float radius, glm::vec4 color)
		: center(center)
		, radius(radius)
		, color(color)
	{}

	bool IsInside(const glm::vec2 &pos)
	{
		if (glm::dot(pos - center, pos - center) < radius)
			return true;
		else
			return false;
	}
};

