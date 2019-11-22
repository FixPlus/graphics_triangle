#pragma once
#include <glm/glm.hpp>
#include <deque>



//Rotatible is an interface that can be inheritated by classes which can be described like a "rotatible spatial object"

namespace triGraphic{

using Rotation = std::pair<std::pair<glm::vec3, glm::vec3>, float>; // <<point1, point2>, rotSpeed>

class Rotatible{
protected:
	std::deque<Rotation> rotations;
public:

	void addNewRotationBack(Rotation const &rot){
		rotations.push_back(rot);
	};

	void addNewRotationFront(Rotation const &rot){
		rotations.push_front(rot);
	};

	virtual void rotate(float dt) = 0;

	virtual ~Rotatible() {};
};

};