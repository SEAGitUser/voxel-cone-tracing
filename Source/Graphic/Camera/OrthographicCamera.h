#pragma once

#include "Camera.h"

/// <summary> An orthographic camera. </summary>
class OrthographicCamera : public Camera
{
public:
    
    //the default constructor of the orthographic camera creates OpenGL's orthographic canonical view and it is where clipping happens.
    //check out "Real-Time Rendering, Third Edition", section 4.6
	OrthographicCamera();

    OrthographicCamera(float viewSpaceWidth, float viewSpaceHeight, float viewSpaceDepth);
    
    inline float getLeft(){ return left;}
    inline float getRigth(){ return right;}
    inline float getBottom(){ return bottom; }
    inline float getTop(){ return top; }
    
private:
    float left;
    float right;
    float bottom;
    float top;
};
