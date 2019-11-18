#pragma once
#include "lingeo3D.h"
#include "DrawableTriangle.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ctime>

using namespace lingeo3D;

//#define TRI_LOGGING

class sorted_cubes;

class cube_t final{ //3d cube

	float x1, y1, z1, x2, y2, z2;

public:
	cube_t(triGraphic::DrawableTriangle polygon = triGraphic::DrawableTriangle{}, float cube_size = 0.0){
		float min_x = -1.0, min_y = -1.0, min_z = -1.0, max_x = 0.0, max_y = 0.0, max_z = 0.0;
		for(int i = 0; i < 3; i++){
			VulkanExample::Vertex pnt = polygon.vertex(i);
			if(min_x < 0.0 || min_x > pnt.position.x) min_x = pnt.position.x;
			if(min_y < 0.0 || min_y > pnt.position.y) min_y = pnt.position.y;
			if(min_z < 0.0 || min_z > pnt.position.z) min_z = pnt.position.z;

			if(max_x < pnt.position.x) max_x = pnt.position.x;
			if(max_y < pnt.position.y) max_y = pnt.position.y;
			if(max_z < pnt.position.z) max_z = pnt.position.z;

		}
		x1 = min_x;
		y1 = min_y;
		z1 = min_z;

		x2 = (max_x - min_x < cube_size) ? min_x + cube_size : max_x;
		y2 = (max_y - min_y < cube_size) ? min_y + cube_size : max_y;
		z2 = (max_z - min_z < cube_size) ? min_z + cube_size : max_z;
	}

	bool x_interfere(cube_t const &cube) const{
		return !(cube.x1 > x2 + flt_tolerance || cube.x2 < x1 - flt_tolerance);
	}

	bool y_interfere(cube_t const &cube) const{
		return !(cube.y1 > y2 + flt_tolerance || cube.y2 < y1 - flt_tolerance);
	}

	bool z_interfere(cube_t const &cube) const{
		return !(cube.z1 > z2 + flt_tolerance || cube.z2 < z1 - flt_tolerance);
	}

	bool interfare(cube_t const &cube) const{
		return x_interfere(cube) && y_interfere(cube) && z_interfere(cube);
	}

	friend class sorted_cubes;
};


class sorted_cubes final{
	std::vector<cube_t> cubes_; //holds cubes of the same size that holding their triangles inside

	std::vector<int> x_sorted_cubes; //holds indexes of cubes_ sorted by x coordinate

public:
	sorted_cubes(std::vector<triGraphic::DrawableTriangle> const &polys){

		float x_size_max = 0.0, y_size_max = 0.0, z_size_max = 0.0;
		for(int i = 0; i < polys.size(); i++){
			for(int j = 0; j < 3; j++){
				for(int k = 0; k < 3; k++){
					VulkanExample::Vertex pnt1 = polys[i].vertex(j);
					VulkanExample::Vertex pnt2 = polys[i].vertex(k);
					float x_size = std::abs(pnt1.position.x - pnt2.position.x);
					float y_size = std::abs(pnt1.position.y - pnt2.position.y);
					float z_size = std::abs(pnt1.position.z - pnt2.position.z);

					if(x_size > x_size_max) x_size_max = x_size;
					if(y_size > y_size_max) y_size_max = y_size;
					if(z_size > z_size_max) z_size_max = z_size;
				}			

			}			
		}

		float cube_size = 0.0;                   // cube size must be not less than maximum linear size of triangle appeared in polys to fit all of them
		
		if(x_size_max >= y_size_max && x_size_max >= z_size_max)
			cube_size = x_size_max;
		else
			if(y_size_max >= z_size_max && y_size_max >= x_size_max)
				cube_size = y_size_max;
			else
				cube_size = z_size_max;


		for(int i = 0; i < polys.size(); i++){
			cubes_.insert(cubes_.end(), {polys[i], cube_size});
			x_sorted_cubes.insert(x_sorted_cubes.end(), i);
		}

		//sorting by x coordinate
		std::sort(x_sorted_cubes.begin(), x_sorted_cubes.end(), [this](int a1, int a2) -> bool {return cubes_[a1].x1 < cubes_[a2].x1;});

	}

	std::vector<int> interfere_x(int index) const{

		//binary search for range of cubes_ interfered by x coordinate with cubes_[index]
		auto x_range_pair = std::equal_range(x_sorted_cubes.begin(), x_sorted_cubes.end(), index, [this](int a1, int a2) -> bool { return  cubes_[a1].x2 < cubes_[a2].x1;}); 

		std::vector<int> x_interfered = {x_range_pair.first, x_range_pair.second};		

		return x_interfered;
	}

	cube_t const & operator[](int idx) const{
		return cubes_[idx];
	}

};

polygon_t<float> drawTriToPolygon(triGraphic::DrawableTriangle const &draw_tri){
	polygon_t<float> ret;
	ret.vertices.push_back(point_t<float>{draw_tri.vertex(0).position.x, draw_tri.vertex(0).position.y, draw_tri.vertex(0).position.z});
	ret.vertices.push_back(point_t<float>{draw_tri.vertex(1).position.x, draw_tri.vertex(1).position.y, draw_tri.vertex(1).position.z});
	ret.vertices.push_back(point_t<float>{draw_tri.vertex(2).position.x, draw_tri.vertex(2).position.y, draw_tri.vertex(2).position.z});
	return ret;
}

void get_intersected(std::vector<polygon_t<float>> &triangles){

}

void get_intersected(std::vector<triGraphic::DrawableTriangle> &triangles){

	std::vector<bool> intersected;

	sorted_cubes s_cubes{triangles};

	intersected.resize(triangles.size(), false);


#ifdef TRI_LOGGING

	std::string algo_name;

	algo_name = "using sorted cubes vector";


	unsigned long long delta_time = time(nullptr);
	std::cout << "Start checking " << algo_name << "..." << std::endl;

#endif



#ifdef TRI_LOGGING

	
	int check_point = 0;
	float check_step = 0.02;

#endif

	for(int i = 0; i < triangles.size(); i++){ // N

#ifdef TRI_LOGGING

		if(i >= check_point){
			std::cout << static_cast<int>(static_cast<float>(check_point)/static_cast<float>(triangles.size()) * 100.0) << "% done..." << std::endl;
			check_point += triangles.size() * check_step;
		}

#endif

		if(intersected[i])
			continue;
		std::vector<int> prob_intersect = s_cubes.interfere_x(i); // logN

		for(int j = 0; j < prob_intersect.size(); j++){ // M
			int idx_cube = prob_intersect[j];
			if(i == idx_cube || !(s_cubes[i].z_interfere(s_cubes[idx_cube]) && s_cubes[i].y_interfere(s_cubes[idx_cube])))
				continue;

			polygon_t<float> poly1 = drawTriToPolygon(triangles[i]);
			polygon_t<float> poly2 = drawTriToPolygon(triangles[idx_cube]);
			
			if(poly1.intersect(poly2)){
				intersected[i] = true;
				intersected[idx_cube] = true;
				break;
			}
		}
	}

#ifdef TRI_LOGGING

	delta_time = time(nullptr) - delta_time;
	std::cout << "Time elapsed:" << delta_time << std::endl; 
#endif

	for(int i = 0; i < intersected.size(); i++){
		glm::vec3 color = intersected[i] ? glm::vec3{1.0f, 0.0f, 0.0f} : glm::vec3{0.0f, 1.0f, 0.0f};
		triangles[i].setColor(color);
	}


	//return intersected;
}