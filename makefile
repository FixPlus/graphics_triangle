
CFLAGS = -std=c++17 -I./external -I./external/glm -I./base
LDFLAGS = -L./libs/vulkan -lvulkan -L./base -lxcb

all:
	g++ -c $(CFLAGS) -o base/v_base.o base/vulkanexamplebase.cpp $(LDFLAGS) -D VK_USE_PLATFORM_XCB_KHR
	g++ -c $(CFLAGS) -o base/v_tools.o base/VulkanTools.cpp $(LDFLAGS) -D VK_USE_PLATFORM_XCB_KHR
	g++ -c $(CFLAGS) -o base/v_debug.o base/VulkanDebug.cpp $(LDFLAGS) -D VK_USE_PLATFORM_XCB_KHR
	g++ -c $(CFLAGS) -o triangle/triangle.o triangle/triangle.cpp $(LDFLAGS) -D VK_USE_PLATFORM_XCB_KHR
	g++ $(CFLAGS) -o InterAndDraw base/v_base.o base/v_tools.o base/v_debug.o triangle/triangle.o $(LDFLAGS) -D VK_USE_PLATFORM_XCB_KHR


clean:
	rm -f base/v_base.o base/v_tools.o base/v_debug.o InterAndDraw 
