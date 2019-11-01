#pragma once
#include "VulkanExample.h"

// Drawer is used to operate with VulkanExample class by creating, initializing, preparing, rendering, handling events and deleting


enum WindowStyle{WS_WINDOWED, WS_FULLSCREEN};


class Drawer 
{

	typedef void (*CHEFR)(const xcb_generic_event_t *event); //typedefed pointer to handleEvent function 

	VulkanExample* APIManager = nullptr;    //Contains VulkanExample instance by address 
	CHEFR customHandleEvent;                //used to have a possibility to handle events out of Drawer class

	void initManager(enum WindowStyle style, std::string windowName, uint32_t width, uint32_t height, int32_t init_camera_rot_x, int32_t init_camera_rot_y); //inits Vulkan

public:

	Drawer(enum WindowStyle style = WS_WINDOWED, CHEFR che = nullptr, std::string windowName = "Window", uint32_t width = 1240, uint32_t height = 780, int32_t init_camera_rot_x = 90, int32_t init_camera_rot_y = -540);

	~Drawer(); //deletes APIManager created by initManager

	void set(std::vector<VulkanExample::triangle_to_draw> const &triangles); //prepares a new set of vertices using vector

	void set(std::vector<VulkanExample::triangle_to_draw>::const_iterator it1, std::vector<VulkanExample::triangle_to_draw>::const_iterator it2); //prepares a new set of vertices using iterators

	void render();   //starts rendering loop

	void handleEvent(const xcb_generic_event_t *event); //handles the xcb window events

};

