#include "vk_game_engine.h"
#include "vk_textures.h"
#include "vk_initializers.h"
#include <vulkan/vulkan.h>
#include <vector>

void VulkanEngine::load_image()
{
	vktexture::load_image_from_file(*this, TEXTURE_LOCATION, _loadedTexture.image);

	VkImageViewCreateInfo imageinfo = vkinit::imageview_create_info(VK_FORMAT_R8G8B8A8_SRGB, _loadedTexture.image._image, VK_IMAGE_ASPECT_COLOR_BIT);
	vkCreateImageView(_device, &imageinfo, nullptr, &_loadedTexture.imageView);
}

void VulkanEngine::load_images()
{
	for (uint32_t i = 0; i < _renderables.size(); ++i)
	{
		for (const auto& path : _renderables[i].material->textureAsset.paths)
		{
			vktexture::make_texture(*this, path.c_str(), _renderables[i].material->textureAsset);
		}
	}
}

void VulkanEngine::init_texture_descriptors(RenderObject* object)
{
	VkSamplerCreateInfo samplerInfo = vkinit::sampler_create_info(VK_FILTER_NEAREST);

	vkCreateSampler(_device, &samplerInfo, nullptr, &object->material->textureAsset.sampler);
	
	size_t count = object->material->textureAsset.textureData.size();

	for (uint32_t i = 0; i < count; ++i)
	{
		object->material->textureAsset.descriptorImageInfos[i].sampler = nullptr;
		object->material->textureAsset.descriptorImageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		object->material->textureAsset.descriptorImageInfos[i].imageView = object->material->textureAsset.textureData[i].imageView;																
	}

	VkDescriptorSetLayoutBinding layoutBindings[2];

	layoutBindings[0] = vkinit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0);
	
	layoutBindings[1] = vkinit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
	layoutBindings[1].descriptorCount = count;

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.bindingCount = 2;
	layoutInfo.flags = 0;
	layoutInfo.pNext = nullptr;
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.pBindings = layoutBindings;

	vkCreateDescriptorSetLayout(_device, &layoutInfo, nullptr, &object->material->textureAsset.textureLayout);

	//allocate the descriptor set for single-texture to use on the material
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.pNext = nullptr;
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = _descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &object->material->textureAsset.textureLayout;

	vkAllocateDescriptorSets(_device, &allocInfo, &object->material->textureAsset.textureSet);

	//write 
	VkWriteDescriptorSet setWrites[2];

	VkDescriptorImageInfo imageInfo = {};
	imageInfo.sampler = object->material->textureAsset.sampler;

	setWrites[0] = {};
	setWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	setWrites[0].dstBinding = 0;
	setWrites[0].dstArrayElement = 0;
	setWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
	setWrites[0].descriptorCount = 1;
	setWrites[0].dstSet = object->material->textureAsset.textureSet;
	setWrites[0].pBufferInfo = 0;
	setWrites[0].pImageInfo = &imageInfo;

	setWrites[1] = {};
	setWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	setWrites[1].dstBinding = 1;
	setWrites[1].dstArrayElement = 0;
	setWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	setWrites[1].descriptorCount = object->material->textureAsset.textureData.size();
	setWrites[1].pBufferInfo = 0;
	setWrites[1].dstSet = object->material->textureAsset.textureSet;
	setWrites[1].pImageInfo = object->material->textureAsset.descriptorImageInfos.data();

	vkUpdateDescriptorSets(_device, 2, setWrites, 0, nullptr);
}