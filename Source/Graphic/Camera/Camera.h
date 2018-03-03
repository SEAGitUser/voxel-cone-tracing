#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "OpenGl_Includes.h"

/// <summary> A camera base class. </summary>
class Camera {
    
public:
    
	glm::vec3 up = { 0,1,0 }, forward = { 0,0,-1 }, position = { 0,0,0 };
	glm::mat4 viewMatrix;
    bool projectionMatrixHasChanged = true;
    bool dirtyProjectionMatrix = true;
    
	const glm::mat4 & getProjectionMatrix() const;
	void setProjectionMatrix(glm::mat4 projectionMatrix);
	virtual void updateViewMatrix();
	glm::vec3 right() { return glm::normalize(-glm::cross(up, forward)); }
	glm::vec3 front() { return glm::normalize(forward); }

    
    inline GLfloat getFov() const {return fov;}
    inline GLfloat getAspectRatio() const { return aspect;}
    inline GLfloat getNear() const {return near;}
    inline GLfloat getFar() const {return far;}
    inline GLfloat getFocalLength() const {return focalLength;}
    
protected:
    Camera(glm::mat4 _projectionMatrix) : projectionMatrix(_projectionMatrix),
        fov(0.0f),
        aspect(0.0f),
        near(0.0f),
        far(0.0f),
        focalLength(0.0f)
    {
    }
    
    Camera(GLfloat _fov, GLfloat _aspect, GLfloat _near, GLfloat _far) :
        projectionMatrix(glm::mat4(1)),
        fov(_fov),
        aspect(_aspect),
        near(_near),
        far(_far),
        focalLength(0.0f)
    {
    }
    
    glm::mat4 projectionMatrix;
    GLfloat fov;
    GLfloat aspect;
    GLfloat near;
    GLfloat far;
    GLfloat focalLength;
};
