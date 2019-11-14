#pragma once
/*
	
	Multiple triangle drawing 

	THIS IS A MODIFY OF EXISTING CODE:

/*
* Vulkan Example - Basic indexed triangle rendering
*
*
* Copyright (C) 2016-2017 by Sascha Willems - www.saschawillems.de
*
*/

// ALL MODIFICATIONS ARE NOTED WITH "MODIFIED: comment"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fstream>
#include <vector>
#include <exception>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unistd.h>

#include <vulkan/vulkan.h>
#include "vulkanexamplebase.h"

// Set to "true" to enable Vulkan's validation layers (see vulkandebug.cpp for details)
#define ENABLE_VALIDATION false
// Set to "true" to use staging buffers for uploading vertex and index data to device local memory
// See "prepareVertices" for details on what's staging and on why to use it
#define USE_STAGING true


class VulkanExample : public VulkanExampleBase
{
public:

	// Vertex layout used in this example
	//MODIFIED: added normal vector to calculate brightness of vertex in shader
	struct Vertex {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec3 normal;
	};

	//MODIFIED: used to contain the triangles

	struct triangle_to_draw
	{
		Vertex vertices[3];
	};		



	// Vertex buffer and attributes
	struct {
		VkDeviceMemory memory;															// Handle to the device memory for this buffer
		VkBuffer buffer;																// Handle to the Vulkan buffer object that the memory is bound to
	} vertices;

	// Index buffer
	struct 
	{
		VkDeviceMemory memory;		
		VkBuffer buffer;			
		uint32_t count;
	} indices;

	// Uniform buffer block object
	struct {
		VkDeviceMemory memory;		
		VkBuffer buffer;			
		VkDescriptorBufferInfo descriptor;
	}  uniformBufferVS;

	// For simplicity we use the same uniform block layout as in the shader:
	//
	//	layout(set = 0, binding = 0) uniform UBO
	//	{
	//		mat4 projectionMatrix;
	//		mat4 modelMatrix;
	//		mat4 viewMatrix;
	//	} ubo;
	//
	// This way we can just memcopy the ubo data to the ubo
	// Note: You should use data types that align with the GPU in order to avoid manual padding (vec4, mat4)
	struct {
		glm::mat4 projectionMatrix;
		glm::mat4 modelMatrix;
		glm::mat4 viewMatrix;
	} uboVS;

	std::vector<Vertex> localVertices;
	uint32_t localVerticesSize;
	std::vector<uint32_t> localIndices;
	uint32_t localIndicesSize;

	// The pipeline layout is used by a pipline to access the descriptor sets 
	// It defines interface (without binding any actual data) between the shader stages used by the pipeline and the shader resources
	// A pipeline layout can be shared among multiple pipelines as long as their interfaces match
	VkPipelineLayout pipelineLayout;

	// Pipelines (often called "pipeline state objects") are used to bake all states that affect a pipeline
	// While in OpenGL every state can be changed at (almost) any time, Vulkan requires to layout the graphics (and compute) pipeline states upfront
	// So for each combination of non-dynamic pipeline states you need a new pipeline (there are a few exceptions to this not discussed here)
	// Even though this adds a new dimension of planing ahead, it's a great opportunity for performance optimizations by the driver
	VkPipeline pipeline;

	// The descriptor set layout describes the shader binding layout (without actually referencing descriptor)
	// Like the pipeline layout it's pretty much a blueprint and can be used with different descriptor sets as long as their layout matches
	VkDescriptorSetLayout descriptorSetLayout;

	// The descriptor set stores the resources bound to the binding points in a shader
	// It connects the binding points of the different shaders with the buffers and images used for those bindings
	VkDescriptorSet descriptorSet;


	// Synchronization primitives
	// Synchronization is an important concept of Vulkan that OpenGL mostly hid away. Getting this right is crucial to using Vulkan.

	// Semaphores
	// Used to coordinate operations within the graphics queue and ensure correct command ordering
	VkSemaphore presentCompleteSemaphore;
	VkSemaphore renderCompleteSemaphore;

	// Fences
	// Used to check the completion of queue operations (e.g. command buffer execution)
	std::vector<VkFence> waitFences;

	VulkanExample(std::string windowName);

	~VulkanExample();

	// This function is used to request a device memory type that supports all the property flags we request (e.g. device local, host visibile)
	// Upon success it will return the index of the memory type that fits our requestes memory properties
	// This is necessary as implementations can offer an arbitrary number of memory types with different
	// memory properties. 
	// You can check http://vulkan.gpuinfo.org/ for details on different memory configurations
	uint32_t getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties);

	// Create the Vulkan synchronization primitives used in this example
	void prepareSynchronizationPrimitives();

	// Get a new command buffer from the command pool
	// If begin is true, the command buffer is also started so we can start adding commands
	VkCommandBuffer getCommandBuffer(bool begin);
	// End the command buffer and submit it to the queue
	// Uses a fence to ensure command buffer has finished executing before deleting it
	void flushCommandBuffer(VkCommandBuffer commandBuffer);

	// Build separate command buffers for every framebuffer image
	// Unlike in OpenGL all rendering commands are recorded once into command buffers that are then resubmitted to the queue
	// This allows to generate work upfront and from multiple threads, one of the biggest advantages of Vulkan
	void buildCommandBuffers();

	void draw();

	// Prepare vertex and index buffers for an indexed triangle
	// Also uploads them to device local memory using staging and initializes vertex input and attribute binding to match the vertex shader
	void prepareVertices(bool s);

	void setupDescriptorPool();

	void setupDescriptorSetLayout();

	void setupDescriptorSet();

	// Create the depth (and stencil) buffer attachments used by our framebuffers
	// Note: Override of virtual function in the base class and called from within VulkanExampleBase::prepare
	void setupDepthStencil();

	// Create a frame buffer for each swap chain image
	// Note: Override of virtual function in the base class and called from within VulkanExampleBase::prepare
	void setupFrameBuffer();

	// Render pass setup
	// Render passes are a new concept in Vulkan. They describe the attachments used during rendering and may contain multiple subpasses with attachment dependencies 
	// This allows the driver to know up-front what the rendering will look like and is a good opportunity to optimize especially on tile-based renderers (with multiple subpasses)
	// Using sub pass dependencies also adds implicit layout transitions for the attachment used, so we don't need to add explicit image memory barriers to transform them
	// Note: Override of virtual function in the base class and called from within VulkanExampleBase::prepare
	void setupRenderPass();

	// Vulkan loads it's shaders from an immediate binary representation called SPIR-V
	// Shaders are compiled offline from e.g. GLSL using the reference glslang compiler
	// This function loads such a shader from a binary file and returns a shader module structure
	VkShaderModule loadSPIRVShader(std::string filename);

	void preparePipelines();

	void prepareUniformBuffers();


	void updateUniformBuffers(); //MODIFIED: changed the order of matrix multiplication to rotate camera, not objects

	void prepare();
	void resize_vertices();

	void render() override;	

//MODIFIED: using mouse to control camera
	
	void mouseMoved(double x, double y, bool &handled) override;

	void viewChanged() override;
	void vulkanClear();
};

// OS specific macros for the example main entry points
// Most of the code base is shared for the different supported operating systems, but stuff like message handling diffes

//MODIFIED: now it takes a triangles to draw, deleted all non-xcb WSI and non-linux platforms


