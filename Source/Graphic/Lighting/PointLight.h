#pragma once

#include "OpenGL_Includes.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

#include <iostream>
#include <string>

/// <summary> A simple point light. </summary>
class PointLight {
public:
	bool tweakable = true;
	glm::vec3 position, color;
    unsigned int index;
	PointLight(glm::vec3 _position = { 0, 0, 0 }, glm::vec3 _color = { 1, 1, 1 }) : position(_position), color(_color), index(0) {}
};
