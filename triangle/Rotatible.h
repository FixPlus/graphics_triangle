#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace triGraphic{

using Rotation = std::pair<std::pair<glm::vec3, glm::vec3>, float>; // <<point1 point2>, rotSpeed>

class Rotatible{
protected:
	std::vector<Rotation> rotations;
public:

	explicit Rotatible(std::vector<Rotation> const &rots = {{{{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}, 0.0f}}) : rotations(rots){};

	void addNewRotationBack(Rotation const &rot){
		rotations.push_back(rot);
	};

	void addNewRotationFront(Rotation const &rot){
		rotations.insert(rotations.begin(), rot);
	};

	virtual void rotate(float dt){};

	virtual ~Rotatible() {};
};

};