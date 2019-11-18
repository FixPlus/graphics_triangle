
CFLAGS = -std=c++17 -I./external -I./external/glm -I./base
SOURCE_DIR = triangle
LDFLAGS = -L./libs/vulkan -lvulkan -L./base -lxcb

DEFS = -D VK_USE_PLATFORM_XCB_KHR

BASE_SOURCES =   base/vulkanexamplebase.cpp base/VulkanTools.cpp base/VulkanDebug.cpp
MY_SOURCES = $(SOURCE_DIR)/drawer.cpp $(SOURCE_DIR)/VulkanExample.cpp

SOURCES = $(BASE_SOURCES) $(MY_SOURCES)

SOURCES1 = $(SOURCES) $(SOURCE_DIR)/triangle.cpp
SOURCES2 = $(SOURCES) $(SOURCE_DIR)/triangle_mesh.cpp

OBJECTS1 = $(SOURCES1:.cpp=.o)
OBJECTS2 = $(SOURCES2:.cpp=.o)


OBJECTS_TO_CLEAN = $(OBJECTS1) $(OBJECTS2) test_generator/test_generator.o test_generator/test_analyzer.o

TEST_DIRECTORY = ./tests4

EXECUTABLE1 = InterAndDraw
EXECUTABLE2 = TriangleMesh




$(EXECUTABLE1): $(OBJECTS1)
	g++ $(CFLAGS) $(OBJECTS1)  -o $@ $(LDFLAGS) $(CFLAGS) $(DEFS)

$(EXECUTABLE2): $(OBJECTS2)
	g++ $(CFLAGS) $(OBJECTS2)  -o $@ $(LDFLAGS) $(DEFINES) $(DEFS)

include .depend

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
	./InterAndDraw <$(TEST_DIRECTORY)/1.dat >$(TEST_DIRECTORY)/1.ans
	./InterAndDraw <$(TEST_DIRECTORY)/2.dat >$(TEST_DIRECTORY)/2.ans
	./InterAndDraw <$(TEST_DIRECTORY)/3.dat >$(TEST_DIRECTORY)/3.ans
	./InterAndDraw <$(TEST_DIRECTORY)/4.dat >$(TEST_DIRECTORY)/4.ans
	./InterAndDraw <$(TEST_DIRECTORY)/5.dat >$(TEST_DIRECTORY)/5.ans
	./InterAndDraw <$(TEST_DIRECTORY)/6.dat >$(TEST_DIRECTORY)/6.ans

analyze_tests:
	./test_analyzer $(TEST_DIRECTORY)/1
	./test_analyzer $(TEST_DIRECTORY)/2
	./test_analyzer $(TEST_DIRECTORY)/3
	./test_analyzer $(TEST_DIRECTORY)/4
	./test_analyzer $(TEST_DIRECTORY)/5
	./test_analyzer $(TEST_DIRECTORY)/6

build_all: $(EXECUTABLE1) $(EXECUTABLE2) test_gen test_analyzer




clean:
	rm -f $(OBJECTS_TO_CLEAN) *.o $(EXECUTABLE1) $(EXECUTABLE2) 
