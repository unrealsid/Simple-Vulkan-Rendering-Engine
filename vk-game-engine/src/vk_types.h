#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

struct AllocatedBuffer
{
	VkBuffer _buffer;
	VmaAllocation _allocation;
};

struct AllocatedImage 
{
	VkImage _image;
	VmaAllocation _allocation;
};

struct UploadContext
{
	VkFence _uploadFence;
	VkCommandPool _commandPool;
};

struct Texture
{
	AllocatedImage image;
	VkImageView imageView;
	VkFormat format;

	uint32_t textureWidth;
	uint32_t textureHeight;
	uint32_t channelCount;
};