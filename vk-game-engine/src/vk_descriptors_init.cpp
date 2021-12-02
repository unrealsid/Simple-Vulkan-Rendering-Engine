#include "vk_game_engine.h"
#include <iostream>
#include "vk_global_data.h"
#include "vk_initializers.h"

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
	VkDescriptorSetLayoutBinding objectBinding1 = vkinit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);

	VkDescriptorSetLayoutBinding globalFrameDataBufferBinding = vkinit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);

	VkDescriptorSetLayoutBinding objectBinding2 = vkinit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 2);

	VkDescriptorSetLayoutBinding bindings[] = { objectBinding1, globalFrameDataBufferBinding, objectBinding2 };

	VkDescriptorSetLayoutCreateInfo setInfo = {};
	setInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	setInfo.pNext = nullptr;

	//we are going to have 1 binding
	setInfo.bindingCount = 3;
	//no flags
	setInfo.flags = 0;
	//point to the camera buffer binding
	setInfo.pBindings = bindings;

	vkCreateDescriptorSetLayout(_device, &setInfo, nullptr, &_globalSetLayout);

	const size_t sceneParamBufferSize = pad_uniform_buffer_size(sizeof(GlobalData)) + pad_uniform_buffer_size(sizeof(ObjectData1)) + pad_uniform_buffer_size(sizeof(ObjectData2));
	_frameData.globalFrameDataBuffer = create_buffer(sceneParamBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

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
	VkDescriptorBufferInfo objectData1Info;
	objectData1Info.buffer = _frameData.globalFrameDataBuffer._buffer;
	objectData1Info.offset = 0;
	objectData1Info.range = sizeof(ObjectData1);

	VkDescriptorBufferInfo sceneInfo;
	//it will be the camera buffer
	sceneInfo.buffer = _frameData.globalFrameDataBuffer._buffer;
	//at an offset defined by the distance to a GlobalData Structs
	sceneInfo.offset = pad_uniform_buffer_size(sizeof(GlobalData));
	//of the size of a global data struct
	sceneInfo.range = sizeof(GlobalData);
	
	VkDescriptorBufferInfo objectData2Info;
	//Which buffer
	objectData2Info.buffer = _frameData.globalFrameDataBuffer._buffer;
	//at an offset that will be object data
	objectData2Info.offset = pad_uniform_buffer_size(sizeof(ObjectData2)) + pad_uniform_buffer_size(sizeof(GlobalData));
	//of the size of a global data struct
	objectData2Info.range = sizeof(ObjectData2);

	VkWriteDescriptorSet objectData1Write = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, _frameData.globalDescriptor, &objectData1Info, 0);

	VkWriteDescriptorSet sceneWrite = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, _frameData.globalDescriptor, &sceneInfo, 1);

	VkWriteDescriptorSet objectData2Write = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, _frameData.globalDescriptor, &objectData2Info, 2);

	VkWriteDescriptorSet setWrites[] = { objectData1Write, sceneWrite, objectData2Write };

	vkUpdateDescriptorSets(_device, 3, setWrites, 0, nullptr);
}

size_t VulkanEngine::pad_uniform_buffer_size(size_t originalSize)
{
	// Calculate required alignment based on minimum device offset alignment
	size_t minUboAlignment = _gpuProperties.limits.minUniformBufferOffsetAlignment;
	size_t alignedSize = originalSize;
	if (minUboAlignment > 0)
	{
		alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
	}
	return alignedSize;
}
