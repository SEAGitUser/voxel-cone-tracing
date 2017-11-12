//
//  OpenGL_Includes.h
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/1/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#ifndef OpenGL_Includes_h
#define OpenGL_Includes_h

#ifndef __APPLE__
#define GLEW_STATIC
//in Apple's environment you link against opengl, in other environments you have to find out
//what parts of opengl are supported at run time, this is where glew comes in...
#include "GL/glew.h"

#else

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <assert.h>

#endif


#include "GLFW/glfw3.h"

static inline const char * GetGLErrorString(GLenum error)
{
    const char *str;
    switch( error )
    {
        case GL_NO_ERROR:
            str = "GL_NO_ERROR";
            break;
        case GL_INVALID_ENUM:
            str = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            str = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            str = "GL_INVALID_OPERATION";
            break;
#if defined __gl_h_ || defined __gl3_h_
        case GL_OUT_OF_MEMORY:
            str = "GL_OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            str = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
#endif
#if defined __gl_h_
        case GL_STACK_OVERFLOW:
            str = "GL_STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            str = "GL_STACK_UNDERFLOW";
            break;
        case GL_TABLE_TOO_LARGE:
            str = "GL_TABLE_TOO_LARGE";
            break;
#endif
        default:
            str = "(ERROR: Unknown Error Enum)";
            break;
    }
    return str;
}

#define glError() { \
GLenum err = glGetError(); \
while (err != GL_NO_ERROR) { \
__builtin_printf("glError: %s caught at %s:%u\n", (char *)GetGLErrorString(err), __FILE__, __LINE__); \
assert(0 && "Dying..."); \
} \
}


#endif /* OpenGL_Includes_h */
