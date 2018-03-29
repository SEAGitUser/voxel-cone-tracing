#pragma once

#include "Camera.h"

/// <summary> An orthographic camera. </summary>
class OrthographicCamera : public Camera
{
public:
    
    //the default constructor of the orthographic camera creates OpenGL's orthographic canonical view and it is where clipping happens.
    //check out "Real-Time Rendering, Third Edition", section 4.6
	OrthographicCamera();

    OrthographicCamera(GLfloat viewSpaceWidth, GLfloat viewSpaceHeight, GLfloat viewSpaceDepth);
    
    inline GLfloat getLeft(){ return left;}
    inline GLfloat getRigth(){ return right;}
    inline GLfloat getBottom(){ return bottom; }
    inline GLfloat getTop(){ return top; }
    
private:
    GLfloat left;
    GLfloat right;
    GLfloat bottom;
    GLfloat top;
};
