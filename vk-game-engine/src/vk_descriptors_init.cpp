#include "vk_game_engine.h"
#include <iostream>
#include "vk_global_data.h"

void VulkanEngine::init_descriptors()
{
	//create a descriptor pool that will hold 10 uniform buffers
	std::vector<VkDescriptorPoolSize> sizes =
	{
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10 }
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = 0;
	pool_info.maxSets = 10;
	pool_info.poolSizeCount = (uint32_t)sizes.size();
	pool_info.pPoolSizes = sizes.data();

	vkCreateDescriptorPool(_device, &pool_info, nullptr, &_descriptorPool);

	//information about the binding.
	VkDescriptorSetLayoutBinding globalFrameDataBufferBinding = {};
	globalFrameDataBufferBinding.binding = 0;
	globalFrameDataBufferBinding.descriptorCount = 1;
	// it's a uniform buffer binding
	globalFrameDataBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	// we use it from the fragment shader
	globalFrameDataBufferBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo setInfo = {};
	setInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	setInfo.pNext = nullptr;

	//we are going to have 1 binding
	setInfo.bindingCount = 1;
	//no flags
	setInfo.flags = 0;
	//point to the camera buffer binding
	setInfo.pBindings = &globalFrameDataBufferBinding;

	vkCreateDescriptorSetLayout(_device, &setInfo, nullptr, &_globalSetLayout);

	_frameData.globalFrameDataBuffer = create_buffer(sizeof(GlobalData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

	//allocate one descriptor set for this frame
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.pNext = nullptr;
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	//using the pool we just set
	allocInfo.descriptorPool = _descriptorPool;
	//only 1 descriptor
	allocInfo.descriptorSetCount = 1;
	//using the global data layout
	allocInfo.pSetLayouts = &_globalSetLayout;

	vkAllocateDescriptorSets(_device, &allocInfo, &_frameData.globalDescriptor);

	//information about the buffer we want to point at in the descriptor
	VkDescriptorBufferInfo binfo;
	//it will be the camera buffer
	binfo.buffer = _frameData.globalFrameDataBuffer._buffer;
	//at 0 offset
	binfo.offset = 0;
	//of the size of a global data struct
	binfo.range = sizeof(GlobalData);

	VkWriteDescriptorSet setWrite = {};
	setWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	setWrite.pNext = nullptr;

	//we are going to write into binding number 0
	setWrite.dstBinding = 0;
	//of the global descriptor
	setWrite.dstSet = _frameData.globalDescriptor;

	setWrite.descriptorCount = 1;
	//and the type is uniform buffer
	setWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	setWrite.pBufferInfo = &binfo;

	vkUpdateDescriptorSets(_device, 1, &setWrite, 0, nullptr);
}