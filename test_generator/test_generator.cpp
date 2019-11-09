#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

glm::vec3 gen_random_point(glm::vec3 cube_pnt1, glm::vec3 cube_pnt2){
	glm::vec3 ret{cube_pnt1.x + (cube_pnt2.x - cube_pnt1.x) * (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)),
				  cube_pnt1.y + (cube_pnt2.y - cube_pnt1.y) * (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)),
				  cube_pnt1.z + (cube_pnt2.z - cube_pnt1.z) * (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)), };
	return ret;
}

glm::mat3 gen_triangle(float size, float lower_bound, float upper_bound){
	if(size < 0.0f)
		size = - size;

	float div = upper_bound - lower_bound;
	if(div < size)
		div = 2.0f * size;
	glm::vec3 ref_point = gen_random_point(glm::vec3{lower_bound, lower_bound, lower_bound}, glm::vec3{lower_bound + div - size, lower_bound + div - size, lower_bound + div - size});
	glm::vec3 ref_point2 = ref_point + glm::vec3(size, size, size);
	glm::mat3 ret{gen_random_point(ref_point, ref_point2), gen_random_point(ref_point, ref_point2), gen_random_point(ref_point, ref_point2)};
	return ret;
}

void print_vec(glm::vec3 const &vec, std::ofstream &stream){
	stream << vec.x << " " << vec.y << " " << vec.z << " ";
}


int main(int argc, char** argv){
	if(argc != 4){
		std::cout << "Usage: n_triangles n_secs \"*filepath*/name\"" << std::endl;
		return 0;
	}

	int n_triangles, n_secs;
	n_triangles = std::atoi(argv[1]);
	n_secs =      std::atoi(argv[2]);

	std::string filename =  argv[3];
	std::string filename_dat = filename + ".dat";
	std::string filename_ans = filename + ".rans";

	std::ofstream dat_file{filename_dat}; 
	std::ofstream ans_file{filename_ans};

	if(!dat_file || !ans_file){
		std::cout << "Error: cannot open file for writing!" << std::endl;
		return 0;
	}

	float l_bound = 0.0f, u_bound = 100.0f;
	float size = u_bound / std::pow(static_cast<float>(n_triangles), 0.5f);
	glm::vec3 cube_pnt1{l_bound, l_bound, l_bound};
	glm::vec3 cube_pnt2{u_bound, u_bound, u_bound};

	std::vector<glm::mat3> triangles;
	std::vector<std::pair<glm::vec3, glm::vec3>> rot_points;
	std::vector<int> rot_speeds;

	for(int i = 0; i < n_triangles; i++){
		triangles.push_back(gen_triangle(size, l_bound, u_bound));
		rot_points.push_back({ gen_random_point(cube_pnt1, cube_pnt2), gen_random_point(cube_pnt1, cube_pnt2)});
		rot_speeds.push_back(std::rand() % 100 - 50);
	}

	dat_file << n_triangles << std::endl << n_secs << std::endl;
	for(int i = 0; i < n_triangles; i++){
		print_vec(triangles[i] * glm::vec3{1.0f, 0.0f, 0.0f}, dat_file);
		print_vec(triangles[i] * glm::vec3{0.0f, 1.0f, 0.0f}, dat_file);
		print_vec(triangles[i] * glm::vec3{0.0f, 0.0f, 1.0f}, dat_file);
		dat_file << std::endl;
		print_vec(rot_points[i].first, dat_file); print_vec(rot_points[i].second, dat_file);
		dat_file << std::endl << rot_speeds[i] << std::endl;
	}


	for(int i = 0; i < n_triangles; i++){
		std::vector<glm::vec3> pnts;
		pnts.push_back(triangles[i] * glm::vec3{1.0f, 0.0f, 0.0f});
		pnts.push_back(triangles[i] * glm::vec3{0.0f, 1.0f, 0.0f});
		pnts.push_back(triangles[i] * glm::vec3{0.0f, 0.0f, 1.0f});
		
		glm::vec3 rot_root = rot_points[i].first;
		glm::vec3 rot_axis = rot_points[i].second - rot_points[i].first;
		
		glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f) , glm::radians(rot_speeds[i] * static_cast<float>(n_secs)), -rot_axis);
		
		for(int j = 0; j < 3; j++){
			pnts[j] -= rot_root;
			pnts[j] = glm::vec3(rotateMat * glm::vec4(pnts[j], 0.0f));
			pnts[j] += rot_root;
		}

		triangles[i] = glm::mat3{pnts[0], pnts[1], pnts[2]};
	}
	for(int i = 0; i < n_triangles; i++){
		print_vec(triangles[i] * glm::vec3{1.0f, 0.0f, 0.0f}, ans_file);
		print_vec(triangles[i] * glm::vec3{0.0f, 1.0f, 0.0f}, ans_file);
		print_vec(triangles[i] * glm::vec3{0.0f, 0.0f, 1.0f}, ans_file);
		ans_file << std::endl;
	}

}