#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <vk_mem_alloc.h>
#include "vk_mesh.h"
#include "vk_frame_data.h"
#include "vk_global_data.h"

//we want to immediately abort when there is an error. In normal engines this would give an error message to the user, or perform a dump of state.
#define VK_CHECK(x)                                                 \
	do                                                              \
	{                                                               \
		VkResult err = x;                                           \
		if (err)                                                    \
		{                                                           \
			std::cout <<"Detected Vulkan error: " << err << std::endl; \
			abort();                                                \
		}                                                           \
	} while (0)				\

class VulkanEngine
{
public:

	bool _isInitialized{ false };
	int _frameNumber{ 0 };

	VkExtent2D _windowExtent{ 1700 , 900 };

	struct SDL_Window* _window{ nullptr };

	//initializes everything in the engine
	void init();

	//shuts down the engine
	void cleanup();

	//draw loop
	void draw();

	//run main loop
	void run();

	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debug_messenger;
	VkPhysicalDevice _chosenGPU;
	VkDevice _device;
	VkSurfaceKHR _surface;

	VkSwapchainKHR _swapchain; // from other articles

	// image format expected by the windowing system
	VkFormat _swapchainImageFormat;

	//array of images from the swapchain
	std::vector<VkImage> _swapchainImages;

	//array of image-views from the swapchain
	std::vector<VkImageView> _swapchainImageViews;

	VkQueue _graphicsQueue; //queue we will submit to
	uint32_t _graphicsQueueFamily; //family of that queue

	VkCommandPool _commandPool; //the command pool for our commands
	VkCommandBuffer _mainCommandBuffer; //the buffer we will record into

	VkRenderPass _renderPass;

	std::vector<VkFramebuffer> _framebuffers;

	VkSemaphore _presentSemaphore, _renderSemaphore;
	VkFence _renderFence;

	//VMA Library allocator
	VmaAllocator _allocator;

	//Inputs for this object
	VkPipelineLayout _quadPipelineLayout;
	VkPipeline _quadPipeline;

	Mesh _quadMesh;

	AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

	FrameData _frameData;

	VkDescriptorSetLayout _globalSetLayout;
	VkDescriptorPool _descriptorPool;

	VkPhysicalDeviceProperties _gpuProperties;

private:
	void init_vulkan();

	void init_vma();

	void init_swapchain();

	void init_commands();

	void init_pipelines();

	bool load_shader_module(const char* filePath, VkShaderModule* outShaderModule);

	void init_default_renderpass();

	void init_framebuffers();

	void init_sync_structures();

	void init_descriptors();

	size_t pad_uniform_buffer_size(size_t originalSize);

	void load_meshes();

	void upload_mesh(Mesh& mesh);
	
	void update_descriptors();
};
