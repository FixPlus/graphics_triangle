#include "drawer.h"

Drawer::Drawer(enum WindowStyle style, CHEFR che, std::string windowName, uint32_t width, uint32_t height, int32_t init_camera_rot_x, int32_t init_camera_rot_y): customHandleEvent(che){
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


void Drawer::draw() const{
	APIManager->updateUniformBuffers();
	APIManager->renderLoop();
}
Drawer::~Drawer(){
	delete APIManager;
}

void Drawer::handleEvents() const{
	xcb_generic_event_t *event;
	while ((event = xcb_poll_for_event(APIManager->connection)))
	{
		handleEvent(event);
		free(event);
	}


}
void Drawer::handleEvent(const xcb_generic_event_t *event) const //handles the xcb window events
{
	static float angle = 5;
	if (APIManager != nullptr)
	{
		APIManager->handleEvent(event); //handling events by the APIManager
	}

	if(customHandleEvent != nullptr)
		customHandleEvent(event); //handling events by given customHandleEvent function

	switch(event->response_type & 0x7f){
		case XCB_KEY_PRESS: //Keyboard input
		{
			const xcb_key_release_event_t *keyEvent = (const xcb_key_release_event_t *)event;

		switch (keyEvent->detail)
			{
				case KEY_W:
					break;
				case KEY_S:
					break;
				case KEY_A:
					break;
				case KEY_D:{
					break;
				}
				case KEY_P:
					break;
				case KEY_F1:
					break;				
			}
		}
		break;
	}	

}
