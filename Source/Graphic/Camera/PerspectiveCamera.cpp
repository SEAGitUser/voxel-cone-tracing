#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(GLfloat _fov, GLfloat _aspect, GLfloat _near, GLfloat _far)
	: Camera(glm::perspective(_fov, _aspect, _near, _far))
{
    fov = _fov;
    aspect = _aspect;
    near  =_near;
    far  = _far;
}
