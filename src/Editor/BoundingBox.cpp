#include "BoundingBox.h"

#include <iostream>

const bool BoundingBox::checkAABB(const BoundingBox& obj)
{
	return (this->m_vecMax.x > obj.m_vecMin.x &&
		this->m_vecMin.x < obj.m_vecMax.x &&
		this->m_vecMax.y > obj.m_vecMin.y &&
		this->m_vecMin.y < obj.m_vecMax.y &&
		this->m_vecMax.z > obj.m_vecMin.z &&
		this->m_vecMin.z < obj.m_vecMax.z);
}

const bool BoundingBox::checkAABB(const glm::vec3& obj_minv3, const glm::vec3& obj_maxv3)
{
	return (this->m_vecMax.x > obj_minv3.x &&
		this->m_vecMin.x < obj_maxv3.x &&
		this->m_vecMax.y > obj_minv3.y &&
		this->m_vecMin.y < obj_maxv3.y &&
		this->m_vecMax.z > obj_minv3.z &&
		this->m_vecMin.z < obj_maxv3.z);
}

const bool BoundingBox::checkAABB(const glm::vec3 & point)
{
	return (point.x > m_vecMin.x &&
		point.x < m_vecMax.x &&
		point.y > m_vecMin.y &&
		point.y < m_vecMax.y &&
		point.z > m_vecMin.z &&
		point.z < m_vecMax.z);
}

const bool BoundingBox::checkRayCast(const MouseRay::Ray & ray, float t0, float t1)
{
	// ???
	glm::vec3 min = m_vecMin / WORLD_SIZE;
	glm::vec3 max = m_vecMax / WORLD_SIZE;

	// Using Smits' Method 
	// Ref: https://people.csail.mit.edu/amy/papers/box-jgt.pdf
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	// Initialize tmin/max (x axis) & tymin/max (y axis)
	if (ray.direction.x >= 0) {
		tmin = (min.x - ray.origin.x) / ray.direction.x;
		tmax = (max.x - ray.origin.x) / ray.direction.x;
	}
	else {
		tmin = (max.x - ray.origin.x) / ray.direction.x;
		tmax = (min.x - ray.origin.x) / ray.direction.x;
	}

	if (ray.direction.y >= 0) {
		tymin = (min.y - ray.origin.y) / ray.direction.y;
		tymax = (max.y - ray.origin.y) / ray.direction.y;
	}
	else {
		tymin = (max.y - ray.origin.y) / ray.direction.y;
		tymax = (min.y - ray.origin.y) / ray.direction.y;
	}

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	if (ray.direction.z >= 0) {
		tzmin = (min.z - ray.origin.z) / ray.direction.z;
		tzmax = (max.z - ray.origin.z) / ray.direction.z;
	}
	else {
		tzmin = (max.z - ray.origin.z) / ray.direction.z;
		tzmax = (min.z - ray.origin.z) / ray.direction.z;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	return ((tmin < t1) && (tmax > t0));
}

const glm::vec3& BoundingBox::getVecMin()
{
	return m_vecMin;
}

const glm::vec3& BoundingBox::getVecMax()
{
	return m_vecMax;
}


BoundingBox::BoundingBox(glm::vec3 min, glm::vec3 max)
	: m_vecMin(min)
	, m_vecMax(max)
{
	// If rendering the boxes then create a model for it
	if (RENDER_BOUNDING_BOX)
	{
		// TODO: Create a namespace for quickly creating primitive shapes
		Mesh cubeMesh;

		// Vertices (Static)
		cubeMesh.vertices = {
			// front
			m_vecMin.x / WORLD_SIZE, m_vecMin.y / WORLD_SIZE,  m_vecMax.z / WORLD_SIZE,
			m_vecMax.x / WORLD_SIZE, m_vecMin.y / WORLD_SIZE,  m_vecMax.z / WORLD_SIZE,
			m_vecMax.x / WORLD_SIZE, m_vecMax.y / WORLD_SIZE,  m_vecMax.z / WORLD_SIZE,
			m_vecMin.x / WORLD_SIZE, m_vecMax.y / WORLD_SIZE,  m_vecMax.z / WORLD_SIZE,
			// back
			m_vecMin.x / WORLD_SIZE, m_vecMin.y / WORLD_SIZE, m_vecMin.z / WORLD_SIZE,
			m_vecMax.x / WORLD_SIZE, m_vecMin.y / WORLD_SIZE, m_vecMin.z / WORLD_SIZE,
			m_vecMax.x / WORLD_SIZE, m_vecMax.y / WORLD_SIZE, m_vecMin.z / WORLD_SIZE,
			m_vecMin.x / WORLD_SIZE, m_vecMax.y / WORLD_SIZE, m_vecMin.z / WORLD_SIZE
		};

		// Indices
		cubeMesh.indices = {
			// front
			0, 1, 2,
			2, 3, 0,
			// right
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// left
			4, 0, 3,
			3, 7, 4,
			// bottom
			4, 5, 1,
			1, 0, 4,
			// top
			3, 2, 6,
			6, 7, 3,

			5, 6, 2,
			6, 2, 1
		};

		// Fill in unnecessary info
		for (int i = 0; i < 8; i++)
		{
			cubeMesh.texCoords.insert(cubeMesh.texCoords.end(), { 1.0f, 1.0f });
			cubeMesh.normals.insert(cubeMesh.normals.end(), { 1.0f, 1.0f, 1.0f });
			cubeMesh.colour.insert(cubeMesh.colour.end(), { 1.0f, 0.0f, 0.0f });
		}

		m_staticBox.create(cubeMesh);
		m_staticBox.setMode(GL_LINES);
	}
}

void BoundingBox::render(Renderer& renderer)
{
	renderer.draw(m_staticBox);
}