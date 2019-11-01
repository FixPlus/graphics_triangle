
CFLAGS = -std=c++17 -I./external -I./external/glm -I./base
LDFLAGS = -L./libs/vulkan -lvulkan -L./base -lxcb
DEFINES = -D VK_USE_PLATFORM_XCB_KHR
SOURCES = base/vulkanexamplebase.cpp base/VulkanTools.cpp base/VulkanDebug.cpp triangle/triangle.cpp triangle/drawer.cpp triangle/VulkanExample.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = InterAndDraw


$(EXECUTABLE): $(OBJECTS)
	g++ $(CFLAGS) $(OBJECTS)  -o $@ $(LDFLAGS) $(DEFINES)


.cpp.o:
	g++  $(CFLAGS) -c -o $@ $< $(LDFLAGS) $(DEFINES)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) 
