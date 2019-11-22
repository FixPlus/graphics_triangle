#include "lingeo3D.h"
#include <vector>
#include "intersect.h"
#include "config.h"



//This class is used to construct a triangle mesh using lingeo3D structures that will be converted in drawable Mesh class next

namespace triGraphic{

class MeshBuilder final{
	
	std::vector<lingeo3D::polygon_t<float>> mesh;
	std::vector<lingeo3D::point_t<float>> normals;
	
	lingeo3D::point_t<float> setup_normal(lingeo3D::plane_t<float> plane, lingeo3D::point_t<float> ref_point) const{ 
		return (plane.get_normal() * ((plane.get_side_area(ref_point) == lingeo3D::LEFT_SIDE) ? -1.0f : 1.0f)). normalize();
	}

	lingeo3D::polygon_t<float> stretch_triangle(lingeo3D::polygon_t<float> const &triangle, int ref_index, float multiplier) const{
		return lingeo3D::polygon_t<float>{{triangle.vertices[ref_index], 
										   triangle.vertices[ref_index] * (1.0f - multiplier) + triangle.vertices[(ref_index + 1) % 3] * multiplier, 
										   triangle.vertices[ref_index] * (1.0f - multiplier) + triangle.vertices[(ref_index + 2) % 3] * multiplier}};
	}

public:
	explicit MeshBuilder(float init_size = INITIAL_SIZE, lingeo3D::point_t<float> pnt1 = INITIAL_TETRA[0], lingeo3D::point_t<float> pnt2 = INITIAL_TETRA[1],
	                     lingeo3D::point_t<float> pnt3 = INITIAL_TETRA[2], lingeo3D::point_t<float> pnt4 = INITIAL_TETRA[3]){

		std::vector<lingeo3D::point_t<float>> pnts_tri1 = {pnt1 * init_size, pnt2 * init_size, pnt3 * init_size};
		std::vector<lingeo3D::point_t<float>> pnts_tri2 = {pnt2 * init_size, pnt3 * init_size, pnt4 * init_size};
		std::vector<lingeo3D::point_t<float>> pnts_tri3 = {pnt3 * init_size, pnt4 * init_size, pnt1 * init_size};
		std::vector<lingeo3D::point_t<float>> pnts_tri4 = {pnt4 * init_size, pnt1 * init_size, pnt2 * init_size};

		mesh.emplace_back(pnts_tri1);
		mesh.emplace_back(pnts_tri2);
		mesh.emplace_back(pnts_tri3);
		mesh.emplace_back(pnts_tri4);

		lingeo3D::plane_t<float> temp = mesh[0].get_plane();
		normals.push_back(setup_normal(temp, pnt4));
		
		temp = mesh[1].get_plane();
		normals.push_back(setup_normal(temp, pnt1));
		
		temp = mesh[2].get_plane();
		normals.push_back(setup_normal(temp, pnt2));
		
		temp = mesh[3].get_plane();
		normals.push_back(setup_normal(temp, pnt3));

	}

	void move(lingeo3D::point_t<float> vector){
		for(int i = 0; i < mesh.size(); i++){
			mesh[i].vertices[0] += vector;
			mesh[i].vertices[1] += vector;
			mesh[i].vertices[2] += vector;
		}
	}

	void popRandomPoint(){
		while(true){
			int index = std::rand() % mesh.size();
			
			float rnd_tmp = (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX));  
			lingeo3D::point_t<float> newPoint = mesh[index].vertices[0] * rnd_tmp + mesh[index].vertices[1] * (1 - rnd_tmp);
			rnd_tmp = (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX));
			newPoint = newPoint * rnd_tmp + mesh[index].vertices[2] * (1 - rnd_tmp);

			float len_side1 = mesh[index].vertices[0].distance(mesh[index].vertices[1]);
			float len_side2 = mesh[index].vertices[1].distance(mesh[index].vertices[2]);
			float len_side3 = mesh[index].vertices[2].distance(mesh[index].vertices[0]);
			float avg_len = (len_side1 + len_side2 + len_side3) / 3.0f;
			float min_len = (len_side1 <= len_side2 && len_side1 <= len_side3) ? len_side1 : (len_side2 <= len_side3) ? len_side2 : len_side3;

			newPoint += normals[index] * (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * min_len;
			lingeo3D::polygon_t<float> base_tri = mesh[index];
			lingeo3D::point_t<float> base_normal = normals[index];
			
			mesh[index] = lingeo3D::polygon_t<float>{{base_tri.vertices[0], base_tri.vertices[1], newPoint}};
			lingeo3D::plane_t<float> temp = mesh[index].get_plane();
			normals[index] = setup_normal(temp, base_tri.vertices[2]);

			mesh.emplace_back(lingeo3D::polygon_t<float>{{base_tri.vertices[1], base_tri.vertices[2], newPoint}});
			temp = mesh[mesh.size() - 1].get_plane();
			normals.emplace_back(setup_normal(temp, base_tri.vertices[0]));

			mesh.emplace_back(lingeo3D::polygon_t<float>{{base_tri.vertices[2], base_tri.vertices[0], newPoint}});
			temp = mesh[mesh.size() - 1].get_plane();
			normals.emplace_back(setup_normal(temp, base_tri.vertices[1]));

			bool self_intersect = false;
			
			lingeo3D::polygon_t<float> tri_check1 = stretch_triangle(mesh[index],           2, STRETCH_LEVEL);
			lingeo3D::polygon_t<float> tri_check2 = stretch_triangle(mesh[mesh.size() - 1], 2, STRETCH_LEVEL);
			lingeo3D::polygon_t<float> tri_check3 = stretch_triangle(mesh[mesh.size() - 2], 2, STRETCH_LEVEL);

			for(int i = 0; i < mesh.size() - 2; i++){
				if(i == index)
					continue;
				if(mesh[i].intersect(tri_check1) || mesh[i].intersect(tri_check2) || mesh[i].intersect(tri_check3)){
					self_intersect = true;
					break;
				}
			}


			if(self_intersect){
				mesh[index] = base_tri;
				normals[index] = base_normal;
				mesh.erase(mesh.end() - 2, mesh.end());
				normals.erase(normals.end() - 2, normals.end());
			}
			else
				break;
		}
	}



	int size() const{ return  mesh.size(); };
	std::pair<lingeo3D::polygon_t<float>, lingeo3D::point_t<float>> operator[](int index) const{ return {mesh[index % mesh.size()], normals[index % mesh.size()]};};
};

};