#pragma once

#include "Camera.h"

/// <summary> An orthographic camera. </summary>
class OrthographicCamera : public Camera {
public:
	OrthographicCamera();
	OrthographicCamera(GLfloat aspect, GLfloat near = -100.0f, GLfloat far = 100.0f);
};
