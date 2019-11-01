#include "drawer.h"

void Drawer::initManager(enum WindowStyle style, std::string windowName, uint32_t width, uint32_t height, int32_t init_camera_rot_x, int32_t init_camera_rot_y){
	if(APIManager != nullptr){
		delete APIManager;
	}

	APIManager = new VulkanExample(windowName);

	if(style == WS_FULLSCREEN)
		APIManager->settings.fullscreen = true;
	else{
		APIManager->width = width;
		APIManager->height = height;		
	}

	APIManager->rotation.x = init_camera_rot_x;
	APIManager->rotation.y = init_camera_rot_y;

	APIManager->initVulkan();
	APIManager->setupWindow();

}

Drawer::Drawer(enum WindowStyle style, CHEFR che, std::string windowName, uint32_t width, uint32_t height, int32_t init_camera_rot_x, int32_t init_camera_rot_y): customHandleEvent(che){
	initManager(style, windowName, width, height, init_camera_rot_x, init_camera_rot_y);
}

void Drawer::set(std::vector<VulkanExample::triangle_to_draw> const &triangles) { //prepares a new set of vertices
	APIManager->prepared = false;
	APIManager->prepare(triangles);
}

void Drawer::set(std::vector<VulkanExample::triangle_to_draw>::const_iterator it1, std::vector<VulkanExample::triangle_to_draw>::const_iterator it2) { //prepares a new set of vertices
	APIManager->prepared = false;
	std::vector<VulkanExample::triangle_to_draw> triangles{it1, it2};
	APIManager->prepare(triangles);
}

void Drawer::render(){   //starts render loop
	

	//I overrided the renderLoop function in class VulkanExampleBase by moving the while(!quit) loop and handle event to here to have access to handling events from Drawer class

	APIManager->preRenderLoop(); //so I needed to add function that do operations that stayed before while(!quit) loop

	while (!APIManager->quit)
	{
		xcb_generic_event_t *event;
		while ((event = xcb_poll_for_event(APIManager->connection)))
		{
			handleEvent(event);
			free(event);
		}

		APIManager->renderLoop(); // here now it only lays the body of while(!quit) section without handling events in it (only render call and fps counter operations)
	}

	APIManager->postRenderLoop(); // and after while(!quit) loop
}

Drawer::~Drawer(){
	delete APIManager;
}

void Drawer::handleEvent(const xcb_generic_event_t *event) //handles the xcb window events
{
	if (APIManager != nullptr)
	{
		APIManager->handleEvent(event); //handling events by the APIManager
	}

	if(customHandleEvent != nullptr)
		customHandleEvent(event); //handling events by given customHandleEvent function

}
