#include "vk_game_engine.h"

void VulkanEngine::update_descriptors()
{
	//Binding 0 in the buffer
	//Vertex
	char* vertexData;
	vmaMapMemory(_allocator, _frameData.globalFrameDataBuffer._allocation, (void**)&vertexData);

	float sine = abs(sin(_frameNumber / 120.f));

	_cameraData.view = glm::mat4();
	_cameraData.translation.x = sine;

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