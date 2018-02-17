#include "OrthographicCamera.h"
OrthographicCamera::OrthographicCamera() :
    Camera(glm::ortho(-1, 1, -1, 1, -1, 1))
{
    near = -1.0f;
    far = 1.0f;
    aspect = 1.0f;
}
OrthographicCamera::OrthographicCamera(GLfloat _aspect, GLfloat _near, GLfloat _far) :
    Camera(glm::ortho(-1.0f, 1.0f, -aspect, aspect, near, far))
{
    aspect = _aspect;
    near = _near;
    far = _far;
}
