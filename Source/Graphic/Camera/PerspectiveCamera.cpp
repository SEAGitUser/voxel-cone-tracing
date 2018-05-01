#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float _fov, float _aspect, float _near, float _far)
	: Camera(_fov, _aspect, _near, _far)
{
    projectionMatrix =glm::perspective(fov, aspect, near, far);
    focalLength = 1.0f/tanf(fov*.5f);
}
