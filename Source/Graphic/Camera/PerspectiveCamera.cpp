#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(GLfloat _fov, GLfloat _aspect, GLfloat _near, GLfloat _far)
	: Camera(_fov, _aspect, _near, _far)
{
    projectionMatrix = glm::perspective(fov, aspect, near, far);
    focalLength = 1.0f/tanf(fov*.5f);
}
