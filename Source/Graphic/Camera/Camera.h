#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "OpenGl_Includes.h"

/// <summary> A camera base class. </summary>
class Camera {
    
public:
    
	glm::vec3 up = { 0.0f,1.0f,0.0f }, forward = { 0.0f,0.0f,-1.f }, position = { 0.f,0.f,0.f };
	glm::mat4 viewMatrix;
    bool projectionMatrixHasChanged = true;
    bool dirtyProjectionMatrix = true;
    
	const glm::mat4 & getProjectionMatrix() const;
	void setProjectionMatrix(glm::mat4 projectionMatrix);
    
	virtual void updateViewMatrix();
	
    inline glm::vec3 right() { return glm::normalize(-glm::cross(up, forward)); }
	inline glm::vec3 front() { return glm::normalize(forward); }

    
    inline float getFov() const {return fov;}
    inline float getAspectRatio() const { return aspect;}
    inline float getNear() const {return near;}
    inline float getFar() const {return far;}
    inline float getFocalLength() const {return focalLength;}
    
protected:
    Camera(glm::mat4 _projectionMatrix) : projectionMatrix(_projectionMatrix),
        fov(0.0f),
        aspect(0.0f),
        near(0.0f),
        far(0.0f),
        focalLength(0.0f)    
    {
    }
    
    Camera(float _fov, float _aspect, float _near, float _far) :
        projectionMatrix(glm::mat4(1)),
        fov(_fov),
        aspect(_aspect),
        near(_near),
        far(_far),
        focalLength(0.0f)
    {
    }
    
    glm::mat4 projectionMatrix;
    float fov;
    float aspect;
    float near;
    float far;
    float focalLength;
};
