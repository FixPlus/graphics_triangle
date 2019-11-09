#include "drawer.h"
#include "lingeo3D.h"
#include "intersect.h"
#include <cstdlib>
#include <fstream>
#include "DrawableTriangle.h"

#define __LEVEL4__

using namespace lingeo3D;


Drawer* drawer;
std::vector<DrawableTriangle> triangles;
bool paused = false, reversed = false;
float playSpeed = 1.0f;

void myHandleEvent(const xcb_generic_event_t *event){
	static int counter = 1;
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

#ifdef __LEVEL4__ 

void setup_triangles(std::vector<DrawableTriangle> &triangles, std::ifstream &input){
	for(int i = 0; i < triangles.size(); i++){
		for(int j = 0; j < 3; j++){
			glm::vec3 pos;
			input >> pos.x >> pos.y >> pos.z;
			
			
			if(!std::cin.good()){
				std::cout << "Invalid input!\n";
				return;
			}
			triangles[i].vertex(j).position = pos;
			triangles[i].vertex(j).color = glm::vec3{0.0f, 1.0f, 0.0f};
		}

		glm::vec3 line1 = triangles[i].vertex(0).position - triangles[i].vertex(1).position;
		glm::vec3 line2 = triangles[i].vertex(0).position - triangles[i].vertex(2).position;
		glm::vec3 norm = glm::cross(line1, line2);

		triangles[i].vertex(0).normal = norm;
		triangles[i].vertex(1).normal = norm;
		triangles[i].vertex(2).normal = norm;

		glm::vec3 pnt1, pnt2;
		
		input >> pnt1.x >> pnt1.y >> pnt1.z >> pnt2.x >> pnt2.y >> pnt2.z >> triangles[i].rotSpeed;

		if(!std::cin.good()){
			std::cout << "Error: Invalid input!\n";
			return;
		}

		triangles[i].rotRoot = pnt1;
		triangles[i].rotAxis = pnt2 - pnt1;
	}
}

#else //Level 3

void setup_triangles(std::vector<DrawableTriangle> &triangles, std::ifstream &input){
	for(int i = 0; i < triangles.size(); i++){
		for(int j = 0; j < 3; j++){
			glm::vec3 pos;
			input >> pos.x >> pos.y >> pos.z;
			
			if(!input.good()){
				std::cout << "Error: Invalid input!\n";
				return;
			}
			triangles[i].vertex(j).position = pos;
		}
		glm::vec3 line1 = triangles[i].vertex(0).position - triangles[i].vertex(1).position;
		glm::vec3 line2 = triangles[i].vertex(0).position - triangles[i].vertex(2).position;
		glm::vec3 norm = glm::cross(line1, line2);

		triangles[i].vertex(0).normal = norm;
		triangles[i].vertex(1).normal = norm;
		triangles[i].vertex(2).normal = norm;
	}

	get_intersected(triangles);
}

#endif


void print_vec(glm::vec3 const &vec, std::ofstream &output){
	output << vec.x << " " << vec.y << " " << vec.z << " ";
}



int main(int argc, char** argv){

	std::string fullscreen_msg = "-fullscreen";
	std::string file_msg = "-f";
	std::string test_mode_msg = "-t"; //if run in test mode program will close after given time passes
	
	enum WindowStyle style = WS_WINDOWED;
	bool got_filename = false;
	bool test_mode = false;
	std::string filename;

	for(int i = 1; i < argc; i++){
		std::string arg = argv[i];
		if(arg == fullscreen_msg){
				style = WS_FULLSCREEN;
		}
		if(arg == file_msg && i != argc - 1){
			filename = argv[i + 1];
			got_filename = true;					
		}
		if(arg == test_mode_msg)
			test_mode = true;
	}

	if(!got_filename){
		std::cout << "Error: no input file! Use: -f \"*filepath*/name\" (without .dat)" << std::endl;
		return 0;
	}

	std::string dat_filename = filename + ".dat"; //file with triangle data to read
	std::string ans_filename = filename + ".ans"; //file to write the processed triangle position 

	std::ifstream dat_file{dat_filename};

	if(!dat_file){
		std::cout << "Error: can't open file: " << dat_filename << std::endl;
		return 0;
	}

	std::ofstream ans_file{ans_filename};

	if(!dat_file){
		std::cout << "Error: can't open file: " << ans_filename << std::endl;
		return 0;
	}


	int tri_n;
	dat_file >> tri_n;
	if(!dat_file.good()){
		std::cout << "Error: Invalid input!\n";
		return 0;
	}

#ifdef __LEVEL4__ 

	int period_in_sec;
	dat_file >> period_in_sec;

#endif


	drawer = new Drawer(style, &myHandleEvent, "Intersected triangles");

	if(test_mode)
		std::cout << "Starting rendering loop using triangle data from " << dat_filename << std::endl << "Duration time: " << period_in_sec << std::endl; 

	float deltaTime = 0.0f;
	float overallTime = 0.0f;

	triangles.resize(tri_n);

	drawer->connect(triangles.begin(), triangles.end()); //connecting the memory that is copied to GPU in drawer->draw()  to DrawableTriangles

	setup_triangles(triangles, dat_file); //reads from file triangle information 


	//Drawing loop starts here

	while(!drawer->shouldQuit()) //rendering till window will close
	{ 
		auto tStart = std::chrono::high_resolution_clock::now();

		drawer->draw(); // renders the connected DrawableTriangles
		drawer->handleEvents(); 

#ifdef __LEVEL4__ 

		if(!paused){
			for(int i = 0; i < tri_n; i++)
				triangles[i].update(reversed ? deltaTime * playSpeed: -deltaTime * playSpeed); // updates time-dependant members of DrawingTriangle (e.g. position)

			get_intersected(triangles);// changes colors of intersecting triangles to red and non-intersecting to green 
		}

#endif
		//FPS limitation down there
		
		auto tEnd = std::chrono::high_resolution_clock::now();
		auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();

		int timeToSleepMicroSecs = 1000000u/60 - tDiff * 1000;
		if(timeToSleepMicroSecs < 0)
			timeToSleepMicroSecs = 0;

		usleep((unsigned int)timeToSleepMicroSecs);

		tEnd = std::chrono::high_resolution_clock::now();
		tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
		
		deltaTime = tDiff / 1000.0f; // time of current cycle turn in seconds
		overallTime += deltaTime;
		
		if(test_mode && overallTime >= static_cast<float>(period_in_sec)){ //saving triangle positions to the file at exact momemnt (period_in_sec)
			for(int i = 0; i < tri_n; i++){
				print_vec(triangles[i].vertex(0).position, ans_file);
				print_vec(triangles[i].vertex(1).position, ans_file);
				print_vec(triangles[i].vertex(2).position, ans_file);
				ans_file << std::endl;
			}
			std::cout << "Test finished, data saved. Location: " << ans_filename << std::endl;
			break;
		}
		
	}

	
	delete drawer;

	return 0; 
}