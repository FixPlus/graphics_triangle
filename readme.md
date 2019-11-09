======================================


Triangle Intersection demonstration


======================================

To build the binary executive use make / make all

To build this you need to have xcb lib and vulkan
lib installed (or use the one in ./libs/vulkan)

To open test file use ./InterAndDraw -f tests4/filename
To run in test mode use ./InterAndDraw -t *other arguements*
To generate tests use ./test_gen n_tri n_sec test4/filename
You can generate and run all tests automatically: make gen_run_analyze

To control camera use the mouse:
	left mbtn + drag to rotate camera
	right mbtn + drag to move camera in screen plane
	middle mbtn + drag to move camera perpendicular the screen plane


This project uses code from Vulkan Example Collection
by SaschaWillems: https://github.com/SaschaWillems/Vulkan

That is liciensed under:

The MIT License (MIT)

Copyright (c) 2016 Sascha Willems

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
