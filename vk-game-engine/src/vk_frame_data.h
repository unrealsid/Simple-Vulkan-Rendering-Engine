#pragma once

#include <vulkan/vulkan.h>
#include "vk_types.h"

struct FrameData 
{
	//buffer that holds a single GPUCameraData to use when rendering
	AllocatedBuffer globalFrameDataBuffer;

	VkDescriptorSet globalDescriptor;
};