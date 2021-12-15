#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "vk_mesh.h"
#include "vk_shader_config.h"

struct TextureAsset
{
	//Texture stuff
	std::vector<Texture>	             textureData;
	std::vector<std::string>             paths;
	VkDescriptorSet			             textureSet{ VK_NULL_HANDLE };
	VkDescriptorSetLayout				 textureLayout;
	VkSampler							 sampler;
	std::vector<VkDescriptorImageInfo>	 descriptorImageInfos;
};

struct Material 
{
	std::vector<VkPipeline> pipelines;
	VkPipelineLayout pipelineLayout;

	TextureAsset textureAsset;
};

struct RenderObject 
{
	Mesh* mesh;
	Material* material;
	glm::mat4 transformMatrix;
};