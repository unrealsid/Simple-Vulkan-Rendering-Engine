#pragma once

#include "vk_types.h"
#include "vk_game_engine.h"

namespace vktexture
{
	bool load_image_from_file(VulkanEngine& engine, const char* file, AllocatedImage& outImage);

	bool make_texture(VulkanEngine& engine, const char* filepath, TextureAsset& outAsset);
}