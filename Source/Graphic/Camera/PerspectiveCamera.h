#pragma once

#include "Camera.h"
#include "OpenGL_Includes.h"

/// <summary> A perspective camera. </summary>
class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(GLfloat _fov = 0.7, GLfloat _aspect = 1.0, GLfloat _near = 0.1, GLfloat _far = 500);
    
private:
    
};
