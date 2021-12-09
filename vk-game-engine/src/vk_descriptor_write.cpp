#include "vk_game_engine.h"
#include <glm/gtc/matrix_transform.hpp>

void VulkanEngine::update_descriptors()
{
	//Binding 0 in the buffer
	//Vertex
	char* vertexData;
	vmaMapMemory(_allocator, _frameData.globalFrameDataBuffer._allocation, (void**)&vertexData);

	float sine = abs(sin(_frameNumber / 120.f));

	//make a model view matrix for rendering the object
	//camera view
	glm::vec3 camPos = glm::vec3(0.0f, 0.0f, -3.0f);

	glm::mat4 view = glm::mat4(1.0f);
	// note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, camPos);

	//camera projection
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);


	_cameraData.view = view;
	_cameraData.projection = projection;
	_cameraData.viewprojection = glm::mat4(1.0f);

	memcpy(vertexData, &_cameraData, sizeof(GPUCameraData));

	vmaUnmapMemory(_allocator, _frameData.globalFrameDataBuffer._allocation);


	//Binding 1 in the buffer
	//Fragment
	char* fragmentData;
	vmaMapMemory(_allocator, _frameData.globalFrameDataBuffer._allocation, (void**)&fragmentData);

	GlobalData frameData;
	frameData.time.x = sine;

	fragmentData += pad_uniform_buffer_size(sizeof(GlobalData));
	memcpy(fragmentData, &frameData, sizeof(GlobalData));

	vmaUnmapMemory(_allocator, _frameData.globalFrameDataBuffer._allocation);

}