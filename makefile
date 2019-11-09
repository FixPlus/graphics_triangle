
CFLAGS = -std=c++17 -I./external -I./external/glm -I./base
SOURCE_DIR = triangle
LDFLAGS = -L./libs/vulkan -lvulkan -L./base -lxcb
DEFS = -D VK_USE_PLATFORM_XCB_KHR
SOURCES = base/vulkanexamplebase.cpp base/VulkanTools.cpp base/VulkanDebug.cpp $(SOURCE_DIR)/triangle.cpp  $(SOURCE_DIR)/drawer.cpp $(SOURCE_DIR)/DrawableTriangle.cpp  $(SOURCE_DIR)/VulkanExample.cpp
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS_TO_CLEAN = $(OBJECTS) test_generator/test_generator.o test_generator/test_analyzer.o
TEST_DIRECTORY = ./tests4
EXECUTABLE = InterAndDraw


$(EXECUTABLE): $(OBJECTS)
	g++ $(CFLAGS) $(OBJECTS)  -o $@ $(LDFLAGS) $(DEFINES) $(DEFS)

test_gen: test_generator/test_generator.o
	g++ $(CFLAGS) test_generator/test_generator.o -o $@ $(LDFLAGS) $(DEFINES) $(DEFS)

test_analyzer: test_generator/test_analyzer.o
	g++ $(CFLAGS) test_generator/test_analyzer.o -o $@ $(LDFLAGS) $(DEFINES) $(DEFS)

.cpp.o:
	g++  $(CFLAGS) -c -o $@ $< $(LDFLAGS) $(DEFINES) $(DEFS)

gen_run_analyze: generate_tests run_tests analyze_tests

generate_tests:
	rm -r -f $(TEST_DIRECTORY)
	mkdir $(TEST_DIRECTORY)
	./test_gen 5 10   $(TEST_DIRECTORY)/1
	./test_gen 10 10  $(TEST_DIRECTORY)/2
	./test_gen 50 10  $(TEST_DIRECTORY)/3
	./test_gen 100 10 $(TEST_DIRECTORY)/4
	./test_gen 200 10 $(TEST_DIRECTORY)/5
	./test_gen 500 10 $(TEST_DIRECTORY)/6

run_tests:
	./InterAndDraw -t -f $(TEST_DIRECTORY)/1
	./InterAndDraw -t -f $(TEST_DIRECTORY)/2
	./InterAndDraw -t -f $(TEST_DIRECTORY)/3
	./InterAndDraw -t -f $(TEST_DIRECTORY)/4
	./InterAndDraw -t -f $(TEST_DIRECTORY)/5
	./InterAndDraw -t -f $(TEST_DIRECTORY)/6

analyze_tests:
	./test_analyzer $(TEST_DIRECTORY)/1
	./test_analyzer $(TEST_DIRECTORY)/2
	./test_analyzer $(TEST_DIRECTORY)/3
	./test_analyzer $(TEST_DIRECTORY)/4
	./test_analyzer $(TEST_DIRECTORY)/5
	./test_analyzer $(TEST_DIRECTORY)/6

build_all: $(EXECUTABLE) test_gen test_analyzer

clean:
	rm -f $(OBJECTS_TO_CLEAN) *.o $(EXECUTABLE) 
