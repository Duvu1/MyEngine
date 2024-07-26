#pragma once

#include <vector>

#include "framework.h"
#include "Light.h"
#include "Sphere.h"
#include "Square.h" // include triangle

class Raytracer
{
public:
	int width, height;
	glm::vec3 eyePos = glm::vec3(0.0f, 0.0f, -1.5f);
	Light light;
	std::shared_ptr<Sphere> sphere;
	std::shared_ptr<Triangle> triangle;
	std::shared_ptr<Square> square;
	std::vector<std::shared_ptr<Object>> objects;

public:
	Raytracer(const int& width, const int& height)
		: width(width)
		, height(height)
	{
		sphere = std::make_shared<Sphere>(
			glm::vec3(0.0f, 0.0f, 0.5f),
			0.5f,
			glm::vec3(0.0f, 1.0f, 1.0f)
		);
		sphere->ambient = glm::vec3(0.0f);
		sphere->specular = glm::vec3(0.5f);
		sphere->alpha = 10.0f;
		sphere->reflection = 0.5;

		objects.push_back(sphere);

		square = std::make_shared<Square>(
			glm::vec3{ -1.0f, -1.0f, -1.0f }, 
			glm::vec3{ -1.0f,  1.0f, -1.0f }, 
			glm::vec3{ -1.0f,  1.0f, 10.0f }, 
			glm::vec3{ -1.0f, -1.0f, 10.0f }, 
			glm::vec4{  1.0f,  1.0f, 1.0f, 1.0f }
		);
		square->ambient = glm::vec3(0.0f);
		square->diffuse = glm::vec3(0.0f, 0.1, 0.5f);
		square->specular = glm::vec3(0.5f);
		square->alpha = 10.0f;
		square->reflection = 1.0f;

		objects.push_back(square);

		square = std::make_shared<Square>(
			glm::vec3{  1.0f, -1.0f, 10.0f },
			glm::vec3{  1.0f,  1.0f, 10.0f },
			glm::vec3{  1.0f,  1.0f, -1.0f },
			glm::vec3{  1.0f, -1.0f, -1.0f },
			glm::vec4{  1.0f,  1.0f, 1.0f, 1.0f }
		);
		square->ambient = glm::vec3(0.0f);
		square->diffuse = glm::vec3(0.1f);
		square->specular = glm::vec3(0.5f);
		square->alpha = 10.0f;
		square->reflection = 1.0f;

		objects.push_back(square);

		light = Light{ { 0.0f, 0.0f, -1.0f } }; // point lihgt
	}

	glm::vec3 TransformScreenToWorld(glm::vec3 positionScreen)
	{
		float x = (positionScreen.x - width / 2) / (height / 2);
		float y = -(positionScreen.y - height / 2) / (height / 2);
		float z = positionScreen.z;

		return glm::vec3(x, y, z);
	}

	Hit FindClosestCollision(MyRay& ray)
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

	glm::vec3 TraceRay(MyRay& ray, int recursiveLevel)
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

			MyRay shadowRay{ hit.hitPos + directionToLight * 1e-2f, directionToLight };

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
				MyRay reflectRay{ hit.hitPos + reflectionDirection * 1e-4f, reflectionDirection  };

				color += TraceRay(reflectRay, recursiveLevel - 1) * hit.object->reflection;
			}

			return color;
		}
	}

	void Render(std::vector<glm::vec4>& pixels)
	{
		std::fill(pixels.begin(), pixels.end(), glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });

		//const glm::vec3 eyePos(0.0f, 0.0f, -1.5f);

		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				glm::vec3 pixelPosWorld = TransformScreenToWorld(glm::vec3(i, j, eyePos.z + 1.0f));
				//glm::vec3 rayDir = glm::vec3(0.0f, 0.0f, 1.0f);

				MyRay pixelRay{ pixelPosWorld, glm::normalize(pixelPosWorld - eyePos) };

				pixels[i + j * width] = glm::vec4(glm::clamp(TraceRay(pixelRay, 1), 0.0f, 1.0f), 1.0f);
			}
	}
};