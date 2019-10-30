#include "triangle_drawer.h"
#include "lingeo3D.h"
#include "intersect.h"

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


int main(){

	std::vector<polygon_t<float>> tris = read_triangles(); //loading triangles from stdin

	std::vector<bool> intersected = get_intersected(tris); //getting the intersected 

	std::vector<VulkanExample::triangle_to_draw> triangles = make_vertices(tris, intersected); //converting from polygon_t to triangle_to_draw
	
	if(triangles.size() == 0){
		std::cout << "No triangles got, stopped!" << std::endl;
		return 0;
	}

	return draw_triangles(triangles); //drawing	
}