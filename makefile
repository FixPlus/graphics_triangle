VULKAN_SDK_PATH = /home/fixplus/"Рабочий стол"/progs/cpp/Vulkaan-API/triangles_graphic

CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/external -I$(VULKAN_SDK_PATH)/external/glm -I$(VULKAN_SDK_PATH)/base
LDFLAGS = -L$(VULKAN_SDK_PATH)/libs/vulkan -lvulkan -L$(VULKAN_SDK_PATH)/base -lxcb

VulkanTest:
	g++ -c $(CFLAGS) -o base/v_base.o base/vulkanexamplebase.cpp $(LDFLAGS) -D VK_USE_PLATFORM_XCB_KHR
	g++ -c $(CFLAGS) -o base/v_tools.o base/VulkanTools.cpp $(LDFLAGS) -D VK_USE_PLATFORM_XCB_KHR
	g++ -c $(CFLAGS) -o base/v_debug.o base/VulkanDebug.cpp $(LDFLAGS) -D VK_USE_PLATFORM_XCB_KHR
	g++ $(CFLAGS) -o VulkanTest base/v_base.o base/v_tools.o base/v_debug.o triangle/triangle.cpp $(LDFLAGS) -D VK_USE_PLATFORM_XCB_KHR

.PHONY: test clean

test: VulkanTest
	LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/vulkan/explicit_layer.d ./VulkanTest

clean:
