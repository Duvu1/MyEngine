#pragma once

#include <vector>

#include "framework.h"
#include "Light.h"
#include "Sphere.h"

class Raytracer
{
public:
	int width, height;
	Light light;
	std::shared_ptr<Sphere> sphere;

public:
	Raytracer(const int& width, const int& height)
		: width(width)
		, height(height)
	{
		sphere = std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 0.5f), 0.5f);
		sphere->ambient = glm::vec3(0.0f);
		sphere->diffuse = glm::vec3(0.0f, 1.0f, 1.0f);
		sphere->specular = glm::vec3(1.0f);
		sphere->alpha = 9.0f;
		sphere->ks = 0.8;

		light = Light{ { 0.0f, 0.0f, -1.0f } }; // point lihgt
	}

	glm::vec3 TransformScreenToWorld(glm::vec2 positionScreen)
	{
		float x = (positionScreen.x - width / 2) / (height / 2);
		float y = (positionScreen.y - height / 2) / (height / 2);

		return glm::vec3(x, y, 0.0f);
	}

	glm::vec3 traceRay(Ray& ray)
	{
		const Hit hit = sphere->IntersectRayCollision(ray);

		if (hit.dist < 0)
		{
			return glm::vec3{ 0.0f };
		}
		else
		{
			//return sphere->color * hit.dist;

			// Diffuse
			const glm::vec3 directionToLight = glm::normalize(light.pos - hit.hitPos);
			const float diffuse = glm::max(glm::dot(hit.normal, directionToLight), 0.0f);

			// Specular
			const glm::vec3 reflectDirection = 2.0f * glm::dot(hit.normal, directionToLight) * hit.normal - directionToLight;
			const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDirection), 0.0f), sphere->alpha);

			return sphere->ambient + sphere->diffuse * diffuse + sphere->specular * specular * sphere->ks;
		}
	}

	void Render(std::vector<glm::vec4>& pixels)
	{
		std::fill(pixels.begin(), pixels.end(), glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });

		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				glm::vec3 pixelPosWorld = TransformScreenToWorld(glm::vec2(i, j));
				glm::vec3 rayDir = glm::vec3(0.0f, 0.0f, 1.0f);

				Ray pixelRay{ pixelPosWorld, rayDir };

				pixels[i + j * width] = glm::vec4(traceRay(pixelRay), 1.0f);
			}
	}
};