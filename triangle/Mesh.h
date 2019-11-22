#pragma once
#include "DrawableTriangle.h"
#include "MeshBuilder.h"
#include "Rotatible.h"



//This class holds a bunch of DrawableTriangle objects that are associated together as "triangle mesh"
//In order to setup triangles it uses class MeshBuilder


using triIterator = std::vector<triGraphic::DrawableTriangle>::iterator;

namespace triGraphic{


glm::vec3 pntToVec(lingeo3D::point_t<float> pnt){
	return glm::vec3{pnt.x_, pnt.y_, pnt.z_};
}

class Mesh : public Rotatible{
	
	triIterator triangles_;
	int size_;

	void setup_triangles(MeshBuilder const &mesh){
		for(int i = 0; i < mesh.size(); i++){
			glm::vec3 normal = pntToVec(mesh[i].second);
			glm::vec3 color = {0.0f, 1.0f, 0.0f};

			triangles_[i].vertex(0).position = pntToVec(mesh[i].first.vertices[0]);
			triangles_[i].vertex(0).normal = normal;
			triangles_[i].vertex(0).color = color;

			triangles_[i].vertex(1).position = pntToVec(mesh[i].first.vertices[1]);
			triangles_[i].vertex(1).normal = normal;
			triangles_[i].vertex(1).color = color;

			triangles_[i].vertex(2).position = pntToVec(mesh[i].first.vertices[2]);
			triangles_[i].vertex(2).normal = normal;
			triangles_[i].vertex(2).color = color;

		}
	}

public:

	Mesh(MeshBuilder const &mesh, triIterator triangles){
		size_ = mesh.size();
		triangles_ = triangles;
		setup_triangles(mesh);

		glm::vec3 rotRoot = center();

		auto another_ref_point = triangles_[std::rand() % size_].vertex(std::rand() % 3).position;
		while(rotRoot == another_ref_point)
			another_ref_point = triangles_[std::rand() % size_].vertex(std::rand() % 3).position;
		
		float rotSpeed = 45.0f;

		rotations.clear();
		rotations.emplace_back(std::pair{rotRoot, another_ref_point}, rotSpeed);
	};

	void update(float dt) { //here I suppose to call all methods that updates all time-dependant states of class 
		rotate(dt);

//  	move(dt); for example

	}

	int size() const{
		return size_;
	}

	void setColor(glm::vec3 const &newColor) {
		for(int i = 0; i < size_; i++)
			triangles_[i].setColor(newColor);
	}

	void move(glm::vec3 const &shift) {
		for(int i = 0; i < size_; i++)
			triangles_[i].move(shift);

		rotations[rotations.size() - 1].first.first += shift;

	}

	glm::vec3 center() const{
		glm::vec3 ret{0.0f, 0.0f, 0.0f};
		for(int i = 0 ; i < size_; i++){
			glm::vec3 cenOfTri = triangles_[i].vertex(0).position;
			cenOfTri += triangles_[i].vertex(1).position;
			cenOfTri += triangles_[i].vertex(2).position;
			cenOfTri /= 3.0f;
			ret += cenOfTri; 
		}
		ret /= static_cast<float>(size_);
		return ret;
	}

	void rotate(float dt) override{
		for(int i = 0; i < rotations.size(); i++){
			
			glm::vec3 rotRoot = rotations[i].first.first;
			glm::vec3 rotAxis = rotations[i].first.second - rotRoot;
			float rotSpeed = rotations[i].second;

			glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f) , glm::radians(rotSpeed * dt), rotAxis);

			for(int j = i + 1; j < rotations.size(); j++){
				rotations[j].first.first -= rotRoot;
				rotations[j].first.first = glm::vec3(rotateMat * glm::vec4(rotations[j].first.first, 0.0f));
				rotations[j].first.first += rotRoot;

				rotations[j].first.second -= rotRoot;
				rotations[j].first.second = glm::vec3(rotateMat * glm::vec4(rotations[j].first.second, 0.0f));
				rotations[j].first.second += rotRoot;
			}

			for(int j = 0; j < size_; j++){
				DrawableTriangle& tri = triangles_[j];
				for(int k = 0; k < 3; k++){
					tri.vertex(k).position -= rotRoot;
					tri.vertex(k).position = glm::vec3(rotateMat * glm::vec4(tri.vertex(k).position, 0.0f));
					tri.vertex(k).position += rotRoot;
					tri.vertex(k).normal = glm::vec3(rotateMat * glm::vec4(tri.vertex(k).normal, 0.0f));
				}
			}
		}
	}


	virtual ~Mesh() {};

};

};