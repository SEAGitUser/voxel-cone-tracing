#include "OrthographicCamera.h"
#include "Logger.h"


OrthographicCamera::OrthographicCamera() :
    Camera(glm::ortho(-1, 1, -1, 1, -1, 1))
{
    near = -1.0f;
    far = 1.0f;
    aspect = 1.0f;
}

OrthographicCamera::OrthographicCamera(GLfloat viewSpaceWidth, GLfloat viewSpaceHeight, GLfloat viewSpaceDepth):
    Camera(glm::mat4(1.0f))
{
    aspect = viewSpaceWidth/viewSpaceHeight;
    left = viewSpaceWidth * -.5f;
    right = -left;
    top = viewSpaceHeight * .5f;
    bottom = -top;
    //anything behind the camera will not be projected
    near = 0.0f;
    far = viewSpaceDepth ;
    focalLength = near;
    
    projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
}
