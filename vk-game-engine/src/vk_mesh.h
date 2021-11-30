#pragma once

#include <vector>
#include <glm/glm.hpp>

//Vertex Data
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 uv;
};

struct Mesh
{
	std::vector<Vertex> _vertices;
	
};