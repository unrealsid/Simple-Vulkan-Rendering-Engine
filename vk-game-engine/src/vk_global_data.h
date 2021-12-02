#pragma once
#include <glm/glm.hpp>

struct GlobalData
{
	//Current time
	glm::vec4 time = glm::vec4();
};

//Stores camera transforms
struct GPUCameraData
{
	glm::mat4 view = glm::mat4();
	glm::mat4 projection = glm::mat4();
	glm::mat4 viewprojection = glm::mat4();

	glm::vec4 translation = glm::vec4();
};