#include "drawer.h"
#include "lingeo3D.h"
#include "intersect.h"
#include <cstdlib>

using namespace lingeo3D;
std::vector<polygon_t<float>> read_triangles(){
	std::vector<polygon_t<float>> triangles;

	int tri_n;
	std::cin >> tri_n;
	if(!std::cin.good()){
		std::cout << "Invalid input!\n";
		return triangles;
	}



	for(int i = 0; i < tri_n; i++){
		polygon_t<float> tri;
		for(int j = 0; j < 3; j++){
			float a, b, c;
			std::cin >> a >> b >> c;
			if(!std::cin.good()){
				std::cout << "Invalid input!\n";
				return triangles;
			}
			tri.add(point_t<float>{a, b, c});
		}
		triangles.insert(triangles.end(), tri);
	}

	return triangles;
}


std::vector<VulkanExample::triangle_to_draw> make_vertices(std::vector<polygon_t<float>> tris, std::vector<bool> intersected){
	std::vector<VulkanExample::triangle_to_draw> ret;

	if(tris.size() != intersected.size())
		return ret;

	for(int j = 0; j < tris.size(); j++){
		plane_t<float>	tri_plane(tris[j].vertices[0], tris[j].vertices[1], tris[j].vertices[2]);
		point_t<float> normal_pnt = tri_plane.get_normal();
		float normal[3] = {normal_pnt.x_, normal_pnt.y_, normal_pnt.z_};
		float color[3] = {1.0f * (intersected[j] ? 1.0f : 0.0f), 1.0f * (intersected[j] ? 0.0f : 1.0f), 0.0f};
		VulkanExample::triangle_to_draw polygon;
		for(int i = 0; i < 3; i++){
			point_t<float> cur_pnt = tris[j].vertices[i];
			VulkanExample::Vertex vert{{cur_pnt.x_, cur_pnt.y_, cur_pnt.z_}, {color[0] , color[1], color[2]}, {normal[0], normal[1], normal[2]} };
			polygon.vertices[i] = vert;
		}
		ret.push_back(polygon);
	}
}

Drawer* drawer;
std::vector<VulkanExample::triangle_to_draw> triangles;

void myHandleEvent(const xcb_generic_event_t *event){
	static int counter = 1;
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
				case KEY_D:
					break;
				case KEY_P:
					counter = counter * 2;
					if(counter > triangles.size())
						counter = 1;
					drawer->set(triangles.begin(), triangles.begin() + (unsigned int)(triangles.size() / counter));
					break;
				case KEY_F1:
					break;				
			}
		}
		break;
	}	

}


int main(int argc, char** argv){
	

	std::vector<polygon_t<float>> tris = read_triangles(); //loading triangles from stdin

	std::vector<bool> intersected = get_intersected(tris); //getting the intersected 

	triangles = make_vertices(tris, intersected); //converting from polygon_t to triangle_to_draw
	
	if(triangles.size() == 0){
		std::cout << "No triangles got, stopped!" << std::endl;
		return 0;
	}

	std::string fullscreen_msg = "-fullscreen";
	
	enum WindowStyle style = WS_WINDOWED;

	for(int i = 1; i < argc; i++){
		std::string arg = argv[i];
		if(arg == fullscreen_msg)
			style = WS_FULLSCREEN;
	}

	drawer = new Drawer(style, &myHandleEvent, "Intersected triangles");

	drawer->set(triangles);

	drawer->render();
	
	delete drawer;

	return 0; 
}