#pragma once
#include "VulkanExample.h"
#include "DrawableTriangle.h"

// Drawer is used to operate with VulkanExample class by creating, initializing, preparing, rendering, handling events and deleting


enum WindowStyle{WS_WINDOWED, WS_FULLSCREEN};


class Drawer 
{

	typedef void (*CHEFR)(const xcb_generic_event_t *event); //typedefed pointer to handleEvent function 

	VulkanExample* APIManager = nullptr;    //Contains VulkanExample instance by address 
	CHEFR customHandleEvent;                //used to have a possibility to handle events out of Drawer class

public:

	explicit Drawer(enum WindowStyle style = WS_WINDOWED, CHEFR che = nullptr, std::string windowName = "Window", uint32_t width = 1240, uint32_t height = 780, int32_t init_camera_rot_x = 90, int32_t init_camera_rot_y = -540);

	Drawer(Drawer const &rhs) = delete;

	~Drawer(); //deletes APIManager created by initManager

	void draw() const;   //copies the vertices info to GPU memory and renders frame

	template<typename It> //any random access iterator
	void connect(It begin, It end) const{
		APIManager->prepared = false;
		APIManager->prepare((end - begin) * 3);

		auto vertIt = APIManager->localVertices.begin();

		for(; begin < end; begin++, vertIt += 3)
			begin[0].setIt(vertIt);
	} //connects the triangles iterators to localVertices buffer

	bool shouldQuit() const { return APIManager->quit;}; //return the APIManager state quit

	void handleEvents() const; //handles the xcb window events

	void handleEvent(const xcb_generic_event_t *event) const; //handles current xcb event

	Drawer& operator=(Drawer const &rhs) = delete;


};

