
CFLAGS = -std=c++17 -I./external -I./external/glm -I./base
SOURCE_DIR = triangle
LDFLAGS = -L./libs/vulkan -lvulkan -L./base -lxcb
DEFS = -D VK_USE_PLATFORM_XCB_KHR
SOURCES = base/vulkanexamplebase.cpp base/VulkanTools.cpp base/VulkanDebug.cpp $(SOURCE_DIR)/triangle.cpp  $(SOURCE_DIR)/drawer.cpp $(SOURCE_DIR)/DrawableTriangle.cpp  $(SOURCE_DIR)/VulkanExample.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = InterAndDraw


$(EXECUTABLE): $(OBJECTS)
	g++ $(CFLAGS) $(OBJECTS)  -o $@ $(LDFLAGS) $(DEFINES) $(DEFS)


.cpp.o:
	g++  $(CFLAGS) -c -o $@ $< $(LDFLAGS) $(DEFINES) $(DEFS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) 
