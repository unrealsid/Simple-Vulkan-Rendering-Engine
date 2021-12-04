#include "vk_game_engine.h"
#include <iostream>

void VulkanEngine::update_descriptors()
{
	//Binding 0 in the buffer
	//Fragment
	char* fragmentData;
	vmaMapMemory(_allocator, _frameData.globalFrameDataBuffer._allocation, (void**)&fragmentData);

	auto current_time = std::chrono::high_resolution_clock::now();
	auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - appStartTime).count();

	ShaderInputs frameData;
	frameData.time.x = seconds * 0.001;

	std::cout << frameData.time.x << std::endl;
	frameData.resolution = glm::vec4(_windowExtent.width, _windowExtent.height, 0, 0);

	memcpy(fragmentData, &frameData, sizeof(ShaderInputs));

	vmaUnmapMemory(_allocator, _frameData.globalFrameDataBuffer._allocation);
}