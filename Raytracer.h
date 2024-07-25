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
	std::vector<std::shared_ptr<Sphere>> objects;

public:
	Raytracer(const int& width, const int& height)
		: width(width)
		, height(height)
	{
		sphere = std::make_shared<Sphere>(glm::vec3(0.5f, 0.0f, 0.5f), 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
		sphere->ambient = glm::vec3(0.2f);
		sphere->specular = glm::vec3(0.5f);
		sphere->alpha = 10.0f;
		sphere->reflection = 0.5;
		objects.push_back(sphere);

		sphere = std::make_shared<Sphere>(glm::vec3(-0.5f, 0.0f, 0.5f), 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		sphere->ambient = glm::vec3(0.2f);
		sphere->specular = glm::vec3(0.5f);
		objects.push_back(sphere);
		
		sphere = std::make_shared<Sphere>(glm::vec3(0.0f, 0.5f, 0.5f), 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
		sphere->ambient = glm::vec3(0.2f);
		sphere->specular = glm::vec3(0.5f);
		objects.push_back(sphere);

		light = Light{ { 0.0f, 0.0f, -1.0f } }; // point lihgt
	}

	glm::vec3 TransformScreenToWorld(glm::vec2 positionScreen)
	{
		float x = (positionScreen.x - width / 2) / (height / 2);
		float y = (positionScreen.y - height / 2) / (height / 2);

		return glm::vec3(x, y, 0.0f);
	}

	Hit FindClosestCollision(Ray& ray)
	{
		float closestDist = 99999.0f;
		Hit closestHit = Hit{ -1.0f, glm::vec3(0.0f), glm::vec3(0.0f) };

		for (int l = 0; l < objects.size(); l++)
		{
			Hit hit = objects[l]->CheckRayCollision(ray);

			if (hit.dist >= 0.0f && hit.dist < closestDist)
			{
				closestDist = hit.dist;
				closestHit = hit;
				closestHit.object = objects[l];
			}
		}

		return closestHit;
	}

	glm::vec3 TraceRay(Ray& ray, int recursiveLevel)
	{
		if (recursiveLevel < 0)
			return glm::vec3(0.0f);

		const Hit hit = FindClosestCollision(ray);

		if (hit.dist < 0)
		{
			return glm::vec3(0.0f);
		}
		else
		{
			glm::vec3 color(0.0f);
			color += hit.object->ambient;

			const glm::vec3 directionToLight = glm::normalize(light.pos - hit.hitPos);

			Ray shadowRay{ hit.hitPos + directionToLight * 1e-2f, directionToLight };

			if (FindClosestCollision(shadowRay).dist < 0.0f ||
				FindClosestCollision(shadowRay).dist > glm::length(light.pos - hit.hitPos))
			{
				// Diffuse
				const glm::vec3 directionToLight = glm::normalize(light.pos - hit.hitPos);
				const float diffuse = glm::max(glm::dot(hit.normal, directionToLight), 0.0f);

				// Specular
				const glm::vec3 reflectDirection = 2.0f * glm::dot(hit.normal, directionToLight) * hit.normal - directionToLight;
				const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDirection), 0.0f), hit.object->alpha);

				color += hit.object->ambient + hit.object->diffuse * diffuse + hit.object->specular * specular;
			}

			if (hit.object->reflection)
			{
				const glm::vec3 m = glm::dot(-ray.dir, hit.normal) * hit.normal + ray.dir;
				const glm::vec3 reflectionDirection = glm::normalize(-ray.dir + 2.0f * m);
				Ray reflectRay{ hit.hitPos + reflectionDirection * 1e-4f, reflectionDirection  };

				color += TraceRay(reflectRay, recursiveLevel - 1) * hit.object->reflection;
			}

			return color;
		}
	}

	void Render(std::vector<glm::vec4>& pixels)
	{
		std::fill(pixels.begin(), pixels.end(), glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });

		const glm::vec3 eyePos(0.0f, 0.0f, -1.5f);

		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				glm::vec3 pixelPosWorld = TransformScreenToWorld(glm::vec2(i, j));
				glm::vec3 rayDir = glm::vec3(0.0f, 0.0f, 1.0f);

				Ray pixelRay{ pixelPosWorld, glm::normalize(pixelPosWorld - eyePos) };

				pixels[i + j * width] = glm::vec4(glm::clamp(TraceRay(pixelRay, 1), 0.0f, 1.0f), 1.0f);
			}
	}
};