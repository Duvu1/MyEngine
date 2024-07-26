#pragma once

#include "framework.h"

#include "Hit.h"
#include "Ray.h"
#include "Object.h"

class Triangle : public Object
{
public:
	glm::vec3 v0;
	glm::vec3 v1;
	glm::vec3 v2;

	Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 color)
		: v0(v0)
		, v1(v1)
		, v2(v2)
		, Object(color)
	{
	}

	Hit CheckRayCollision(MyRay& ray)
	{
		Hit hit = Hit{ -1.0f, glm::vec3(0.0), glm::vec3(0.0f) };

		glm::vec3 point, faceNormal;
		float t, u, v;

		if (IntersectRayTriangle(ray.start, ray.dir,
			this->v0, this->v1, this->v2,
			point, faceNormal, t, u, v))
		{
			hit.dist = t;
			hit.hitPos = point;
			hit.normal = faceNormal;
		}

		return hit;
	}

	bool IntersectRayTriangle(const glm::vec3& rayStart, const glm::vec3& rayDir,
							  const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
							  glm::vec3& point, glm::vec3& faceNormal,
							  float& t, float& u, float& v)
	{
		faceNormal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

		// 뒷면은 그리지 않음
		if (glm::dot(faceNormal, -rayDir) < 0.0f) return false;

		// 수평에 가까우면 충돌하지 못하는 것으로 판단
		if (glm::abs(glm::dot(faceNormal, rayDir)) < 1e-2f) return false;

		t = (glm::dot(v0, faceNormal) - glm::dot(rayStart, faceNormal)) / (glm::dot(rayDir, faceNormal));

		// 광선 시작점 이전에 충돌하면 없는 것으로 판단
		if (t < 0.0f) return false;

		point = rayStart + t * rayDir;

		glm::vec3 normal0 = glm::normalize(glm::cross(v1 - point, v2 - point));
		glm::vec3 normal1 = glm::normalize(glm::cross(v2 - point, v0 - point));
		glm::vec3 normal2 = glm::normalize(glm::cross(v0 - point, v1 - point));

		if (glm::dot(normal0, faceNormal) < 0.0f) return false;
		if (glm::dot(normal1, faceNormal) < 0.0f) return false;
		if (glm::dot(normal2, faceNormal) < 0.0f) return false;

		return true;
	}
};