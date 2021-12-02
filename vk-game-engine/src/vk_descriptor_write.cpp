#include "vk_game_engine.h"

void VulkanEngine::update_descriptors()
{
	//Binding 0 in the buffer
	//Vertex
	char* vertexData;
	vmaMapMemory(_allocator, _frameData.globalFrameDataBuffer._allocation, (void**)&vertexData);

	float sine = abs(sin(_frameNumber / 120.f));

	ObjectData1 data1;
	data1.translationSine = sine;

	memcpy(vertexData, &data1, sizeof(ObjectData1));

	vmaUnmapMemory(_allocator, _frameData.globalFrameDataBuffer._allocation);

	vertexData = nullptr;

	//Binding 1 in the buffer
	//Fragment
	char* fragmentData;
	vmaMapMemory(_allocator, _frameData.globalFrameDataBuffer._allocation, (void**)&fragmentData);

	GlobalData frameData;
	frameData.time = sine;

	fragmentData += pad_uniform_buffer_size(sizeof(GlobalData));
	memcpy(fragmentData, &frameData, sizeof(GlobalData));
	vmaUnmapMemory(_allocator, _frameData.globalFrameDataBuffer._allocation);


	//Binding 2 in the buffer
	//Goes in the vertex Shader
	vmaMapMemory(_allocator, _frameData.globalFrameDataBuffer._allocation, (void**)&vertexData);

	float cosine = abs(cos(_frameNumber / 120.0f));

	ObjectData2 data2;
	data2.translationCosine = cosine;
	vertexData += pad_uniform_buffer_size(sizeof(ObjectData2)) + pad_uniform_buffer_size(sizeof(GlobalData));

	memcpy(vertexData, &data2, sizeof(ObjectData2));
	vmaUnmapMemory(_allocator, _frameData.globalFrameDataBuffer._allocation);
}