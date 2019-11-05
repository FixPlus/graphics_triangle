#pragma once
#include "VulkanExample.h"

using vertIterator = std::vector<VulkanExample::Vertex>::iterator;
class DrawableTriangle{
	vertIterator vertices;
public:	
	glm::vec3 velocity, rotAxis, rotRoot;
	float rotSpeed;

	explicit DrawableTriangle(glm::vec3 vel = glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3 rotAx = glm::vec3{1.0f, 1.0f, 1.0f},
					 glm::vec3 rotR = glm::vec3{0.0f, 0.0f, 0.0f}, float rotSpd = 0.0f) : velocity(vel), rotAxis(rotAx), rotRoot(rotR), rotSpeed(rotSpd){};

	void update(float dt); //updates time-dependant values (including vertices)
	void setIt(vertIterator iter);
	VulkanExample::Vertex& vertex(int id) const{ return vertices[id % 3];};
};
