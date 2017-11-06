#pragma once

#include "OpenGL_Includes.h"

#include <iostream> // TODO: Remove.


#include "glm/gtx/rotate_vector.hpp"

#include "Graphic/Camera/Camera.h"
#include "Time/Time.h"
#include "Graphic/Camera/PerspectiveCamera.h"
#include "Application.h"

/// <summary> A first person controller that can be attached to a camera. </summary>
class FirstPersonController {
public:


    FirstPersonController(Camera * camera):
    mouseXStart(0.0f),
    mouseYStart(0.0f)
    {
		targetCamera = new PerspectiveCamera();
		renderingCamera = camera;

	}

	~FirstPersonController() { delete targetCamera; }

    void update();
private:
	bool firstUpdate = true;
    
    const float CAMERA_SPEED = 1.4f;
    const float CAMERA_ROTATION_SPEED = 0.003f;
    const float CAMERA_POSITION_INTERPOLATION_SPEED = 8.0f;
    const float CAMERA_ROTATION_INTERPOLATION_SPEED = 8.0f;
    
    Camera * renderingCamera;
    Camera * targetCamera; // Dummy camera used for interpolation.
    
    float mouseXStart;
    float mouseYStart;
};
