#include "drawer.h"
#include "Mesh.h"
#include <cstdlib>
#include "intersect.h"

using namespace triGraphic;

Drawer* drawer;
std::vector<Mesh> meshes;
std::vector<DrawableTriangle> triangles;


bool paused = true, reversed = false;
float playSpeed = 1.0f;

void myHandleEvent(const xcb_generic_event_t *event){
	static int counter = 0;
	switch(event->response_type & 0x7f){
		case XCB_KEY_PRESS: //Keyboard input
		{
			const xcb_key_release_event_t *keyEvent = reinterpret_cast<const xcb_key_release_event_t*>(event);
		switch (keyEvent->detail)
			{
				case KEY_W:

					playSpeed += 0.05f;
					if(playSpeed > 5.0f)
						playSpeed = 5.0f;
					break;
				case KEY_S:

					playSpeed -= 0.05f;
					if(playSpeed < 0.05f)
						playSpeed = 0.05f;

					break;
				case KEY_A:
					break;
				case KEY_D:
					break;
				case KEY_R:
					reversed = !reversed;
					break;
				case KEY_P:
					paused = !paused;
					break;
				case KEY_F1:
					break;				
			}
		}
		break;
	}	

}

std::vector<MeshBuilder> setup_build_meshes(int n_meshes, int n_pops, float cube_size){
	std::vector<MeshBuilder> build_meshes;
	

	for(int i = 0; i < n_meshes; i++){
		build_meshes.emplace_back();
		build_meshes[i].move(lingeo3D::point_t<float>{static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * cube_size, static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * cube_size ,static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * cube_size});
		for(int j = 0; j < n_pops; j++)
			build_meshes[i].popRandomPoint();
	}
	return build_meshes;	
}

int main(int argc, char** argv){
	std::string fullscreen_msg = "-fullscreen";
	std::string mesh_data_msg = "-md";
	std::string random_coloring_msg = "-rc";

	enum WindowStyle style = WS_WINDOWED;
	
	bool test_mode = false;
	bool random_coloring = false;

	bool mesh_data_got = false;
	int n_meshes, n_pops;
	for(int i = 1; i < argc; i++){
		std::string arg = argv[i];

		if(arg == fullscreen_msg){
				style = WS_FULLSCREEN;
				continue;
		}

		if(arg == random_coloring_msg){
				random_coloring = true;
				continue;
		}

		if(arg == mesh_data_msg){
			if(argc < i + 3){
				std::cout << "Error: not enogh arguements for mesh data!" << std::endl;
				return 0;
			}
			n_meshes = std::atoi(argv[i + 1]);
			n_pops =   std::atoi(argv[i + 2]);
			mesh_data_got = true;
		}
	}

	if(!mesh_data_got){
		n_meshes = 200;
		n_pops = 10;
	}

	int tri_n = 0;
	float cube_size = 50.0f * std::pow(static_cast<float>(n_meshes), 1.0f/ 3.0f);


	std::vector<MeshBuilder> build_meshes = setup_build_meshes(n_meshes, n_pops, cube_size);
	
	for(int i = 0; i < build_meshes.size(); i++)
		tri_n += build_meshes[i].size();
	

	


	drawer = new Drawer(style, &myHandleEvent, "Triangle meshes");
	
	triangles.resize(tri_n);

	drawer->connect(triangles.begin(), triangles.end());
	
	

	float half_cube_size = static_cast<float>(cube_size) / 2.0f;
	glm::vec3 cube_center = {half_cube_size, half_cube_size, half_cube_size};
	auto triIt = triangles.begin();
	
	for(int i = 0; i < build_meshes.size(); i++){
		meshes.emplace_back(build_meshes[i], triIt);

		if(random_coloring){
			int pair = std::rand() % 3;
			float stage = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
			switch(pair){
				case 0:{ 
					meshes[i].setColor(glm::vec3(stage, 1.0f - stage, 0.0f));
					break;
				}
				case 1:{
					meshes[i].setColor(glm::vec3(stage, 0.0f, 1.0f - stage));
					break;
				}
				case 2:{
					meshes[i].setColor(glm::vec3(0.0f, stage, 1.0f - stage));
					break;
				}
			}
		}
		
		triIt += build_meshes[i].size();
		glm::vec3 radius = cube_center - meshes[i].center();
		glm::vec3 rotAxis = {-radius.y / radius.x, 1.0f, 0.0f};
		meshes[i].addNewRotationFront(Rotation{{cube_center, cube_center + rotAxis}, 5.0f});
	}

	float deltaTime = 0.0f;
	float overallTime = 0.0f;


	while(!drawer->shouldQuit()){
		auto tStart = std::chrono::high_resolution_clock::now();
		
		drawer->draw();
		drawer->handleEvents();

		if(!paused)
			for(int i = 0; i < meshes.size(); i++)
				meshes[i].update(reversed ? deltaTime * playSpeed: -deltaTime * playSpeed); // updates time-dependant members of DrawingTriangle (e.g. position)
		

		auto tEnd = std::chrono::high_resolution_clock::now();
		auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();

		int timeToSleepMicroSecs = 1000000u/60 - tDiff * 1000;
		if(timeToSleepMicroSecs < 0)
			timeToSleepMicroSecs = 0;

		usleep(static_cast<unsigned int>(timeToSleepMicroSecs));

		tEnd = std::chrono::high_resolution_clock::now();
		tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();

		deltaTime = tDiff / 1000.0f; // time of current cycle turn in seconds
		overallTime += deltaTime;

	}	

	delete drawer;
}