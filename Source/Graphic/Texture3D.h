#pragma once


#include <vector>

#include "OpenGL_Includes.h"

#include "SOIL/SOIL.h"

/// <summary> A 3D texture wrapper class. Handles important OpenGL calls. </summary>
class Texture3D {
public:
	unsigned char * textureBuffer = nullptr;
	GLuint textureID;

	/// <summary> Activates this texture and passes it on to a texture unit on the GPU. </summary>
	void Activate(const int shaderProgram, const std::string glSamplerName, const int textureUnit = GL_TEXTURE0);

	/// <summary> Clears this texture using a given clear color. </summary>
	void Clear(GLfloat clearColor[4]);

	Texture3D(
		const std::vector<GLfloat> & textureBuffer,
		const int width, const int height, const int depth,
		const bool generateMipmaps = true
	);
    
#ifdef __APPLE__
    
    //these functions are here because Apple supports up to OpenGL 4.1, so I will implement them myself in hopes one day
    //I won't need this.  I won't put them in a header file as I don't see these functions being called anywhere else in the code.
    //Also, this implementation is very specific to how it's called in this class, do not use it anywhere else
    
    void glTexStorage3D(	GLenum target,
                        GLsizei levels,
                        GLenum internalformat,
                        GLsizei width,
                        GLsizei height,
                        GLsizei depth);
    
    void glClearTexImage(	GLuint texture,
                         GLint level,
                         GLenum format,
                         GLenum type,
                         const void * data);
#endif
    
private:
	int width, height, depth;
	std::vector<GLfloat> clearData;
};
