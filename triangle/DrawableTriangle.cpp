#include "DrawableTriangle.h"

using namespace triGraphic;

void DrawableTriangle::update(float dt){
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f) , glm::radians(rotSpeed * dt), rotAxis);
	for(auto temp = vertices; temp < vertices + 3; temp++){
		temp[0].position -= rotRoot;
		temp[0].position = glm::vec3(rotateMat * glm::vec4(temp[0].position, 0.0f));
		temp[0].position += rotRoot + velocity * dt;
		temp[0].normal = glm::vec3(rotateMat * glm::vec4(temp[0].normal, 0.0f));
	}
	rotRoot += velocity * dt;

}

void DrawableTriangle::setColor(glm::vec3 newColor){
	vertices[0].color = newColor;
	vertices[1].color = newColor;
	vertices[2].color = newColor;
}
