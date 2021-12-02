#pragma once
#include <glm/glm.hpp>

struct GlobalData
{
	glm::vec4 lightDir = glm::vec4();

	//Current time
	 alignas(4) float time = 0.0f;
};

//Stores camera transforms
struct ObjectData1
{
	alignas(4) float translationSine = 0.0f;
};

//Stores object Data
struct ObjectData2
{
	alignas(4) float translationCosine = 0.0f;
};