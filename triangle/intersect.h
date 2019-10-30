#include "lingeo3D.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ctime>

using namespace lingeo3D;

#define WITH_SORTED
//#define TRI_LOGGING

template<typename T>
class sorted_cubes;

template<typename T>
class cube_t{ //3d cube

	T x1, y1, z1, x2, y2, z2;

public:
	cube_t(polygon_t<T> polygon = polygon_t<T>{}, T cube_size = 0.0){
		T min_x = -1.0, min_y = -1.0, min_z = -1.0, max_x = 0.0, max_y = 0.0, max_z = 0.0;
		for(int i = 0; i < polygon.vertices.size(); i++){
			point_t<T> pnt = polygon.vertices[i];
			if(min_x < 0.0 || min_x > pnt.x_) min_x = pnt.x_;
			if(min_y < 0.0 || min_y > pnt.y_) min_y = pnt.y_;
			if(min_z < 0.0 || min_z > pnt.z_) min_z = pnt.z_;

			if(max_x < pnt.x_) max_x = pnt.x_;
			if(max_y < pnt.y_) max_y = pnt.y_;
			if(max_z < pnt.z_) max_z = pnt.z_;

		}
		x1 = min_x;
		y1 = min_y;
		z1 = min_z;

		x2 = (max_x - min_x < cube_size) ? min_x + cube_size : max_x;
		y2 = (max_y - min_y < cube_size) ? min_y + cube_size : max_y;
		z2 = (max_z - min_z < cube_size) ? min_z + cube_size : max_z;
	}

	bool x_interfere(cube_t<T> const &cube) const{
		return !(cube.x1 > x2 + flt_tolerance || cube.x2 < x1 - flt_tolerance);
	}

	bool y_interfere(cube_t<T> const &cube) const{
		return !(cube.y1 > y2 + flt_tolerance || cube.y2 < y1 - flt_tolerance);
	}

	bool z_interfere(cube_t<T> const &cube) const{
		return !(cube.z1 > z2 + flt_tolerance || cube.z2 < z1 - flt_tolerance);
	}

	bool interfare(cube_t<T> const &cube) const{
		return x_interfere(cube) && y_interfere(cube) && z_interfere(cube);
	}

	friend class sorted_cubes<T>;
};

template<typename T>
class sorted_cubes{
	std::vector<cube_t<T>> cubes_; //holds cubes of the same size that holding their triangles inside

	std::vector<int> x_sorted_cubes; //holds indexes of cubes_ sorted by x coordinate

public:
	sorted_cubes(std::vector<polygon_t<T>> const &polys){

		T x_size_max = 0.0, y_size_max = 0.0, z_size_max = 0.0;
		for(int i = 0; i < polys.size(); i++){
			for(int j = 0; j < polys[i].vertices.size(); j++){
				for(int k = 0; k < polys[i].vertices.size(); k++){
					point_t<T> pnt1 = polys[i].vertices[j];
					point_t<T> pnt2 = polys[i].vertices[k];
					T x_size = std::abs(pnt1.x_ - pnt2.x_);
					T y_size = std::abs(pnt1.y_ - pnt2.y_);
					T z_size = std::abs(pnt1.z_ - pnt2.z_);

					if(x_size > x_size_max) x_size_max = x_size;
					if(y_size > y_size_max) y_size_max = y_size;
					if(z_size > z_size_max) z_size_max = z_size;
				}			

			}			
		}

		T cube_size = 0.0;                   // cube size must be not less than maximum linear size of triangle appeared in polys to fit all of them
		
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

	cube_t<T> operator[](int idx) const{
		return cubes_[idx];
	}

};


std::vector<bool> get_intersected(std::vector<polygon_t<float>> triangles){

	std::vector<bool> intersected;



#ifndef WITH_SORTED

	std::vector<cube_t<float>> cubes;

#endif

	for(int i = 0; i < triangles.size(); i++){

#ifndef WITH_SORTED

		cubes.insert(cubes.end(), {triangles[i], 0.0});

#endif

	}

#ifdef WITH_SORTED

	sorted_cubes<float> s_cubes{triangles};

#endif


	intersected.resize(triangles.size(), false);


#ifdef TRI_LOGGING

	std::string algo_name;


#ifdef WITH_SORTED

	algo_name = "using sorted cubes vector";

#else

	algo_name = "using unsorted vector";


#endif

	unsigned long long delta_time = time(nullptr);
	std::cout << "Start checking " << algo_name << "..." << std::endl;

#endif

//												average concentration in whole volume		volume of layer that conatains cubes with the intersected x coord
//		SORTED CUBES ALGORYTHM									                  |                |
//																			     \/			      \/		

#ifdef WITH_SORTED   // TIME COMPLEXITY: O(N * (logN + M(N, bounds, lin_size)))  ; M = n(N, bounds) * VxDelta(bounds, lin_size) = (N / bounds^3) * ((bounds ^ 2) * lin_size) = N * lin_size / bounds
	//					if we consider bounds to be constant and if lin_size = 1 / O(N/logN) (lin_size drops to zero not slower than N/logN rises to inf) 
	//					then M(N, lin_size) would be O(logN) and the complexity of the whole algorythm would be O(N*logN)
	//					otherwise it could be up to O(N*N) if lin_size is comparable to bounds or triangles spread unevenly or N >> 1/lin_size
	//                   *****************************************************************************************
	//                  MEMORY COMPLEXITY: O(N) 


#ifdef TRI_LOGGING

	
	int check_point = 0;
	float check_step = 0.02;

#endif

	for(int i = 0; i < triangles.size(); i++){ // N

#ifdef TRI_LOGGING

		if(i >= check_point){
			std::cout << (int)((float)check_point/(float)tri_n * 100.0) << "% done..." << std::endl;
			check_point += tri_n * check_step;
		}

#endif

		if(intersected[i])
			continue;
		std::vector<int> prob_intersect = s_cubes.interfere_x(i); // logN

		for(int j = 0; j < prob_intersect.size(); j++){ // M
			int idx_cube = prob_intersect[j];
			if(i == idx_cube || !(s_cubes[i].z_interfere(s_cubes[idx_cube]) && s_cubes[i].y_interfere(s_cubes[idx_cube])))
				continue;
			if(triangles[i].intersect(triangles[idx_cube])){
				intersected[i] = true;
				intersected[idx_cube] = true;
				break;
			}
		}
	}

//
//           UNSORTED ONE BY ONE CHECKING
// 
#else // 				TIME COMPLEXITY: O(N * N)
// 						***********************************
// 						MEMORY COMPLXTY: O(N)
//

	for(int i = 0; i < tri_n; i++){
		if(intersected[i])
			continue;

		for(int j = 0; j < tri_n; j++){
			if(i == j  || !cubes[i].interfare(cubes[j]))
				continue;
			if(triangles[i].intersect(triangles[j])){
				intersected[i] = true;
				intersected[j] = true;
				break;
			}
		}
	}


#endif

#ifdef TRI_LOGGING

	delta_time = time(nullptr) - delta_time;
	std::cout << "Time elapsed:" << delta_time << std::endl; 
#endif


	return intersected;
}