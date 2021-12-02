#pragma once

#include <vulkan/vulkan.h>
#include "vk_types.h"
#include <glm/glm.hpp>

struct FrameData 
{
	//buffer that holds a single buffer to use when rendering
	AllocatedBuffer globalFrameDataBuffer;

	VkDescriptorSet globalDescriptor;
};