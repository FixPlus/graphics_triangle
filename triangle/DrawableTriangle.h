#pragma once
#include "VulkanExample.h"
#include "Rotatible.h"

using vertIterator = std::vector<VulkanExample::Vertex>::iterator;

namespace triGraphic {

class DrawableTriangle {
	vertIterator vertices;
public:	

	explicit DrawableTriangle() {};

	void setIt(vertIterator iter){ vertices = iter; };
	
	void setColor(glm::vec3 const &newColor){
		vertices[0].color = newColor;
		vertices[1].color = newColor;
		vertices[2].color = newColor;
	};

	void move(glm::vec3 const &shift){
		vertices[0].position += shift;
		vertices[1].position += shift;
		vertices[2].position += shift;

	}

	VulkanExample::Vertex& vertex(int id) const{ return vertices[id % 3];};

	virtual ~DrawableTriangle(){};
};

};