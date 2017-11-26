#pragma once


#include <vector>

#include "OpenGL_Includes.h"
#include "Texture.h"
#include "SOIL/SOIL.h"

/// <summary> A 3D texture wrapper class. Handles important OpenGL calls. </summary>
class Texture3D : public Texture{
public:
	unsigned char * textureBuffer = nullptr;

	Texture3D(
		const std::vector<GLfloat> & textureBuffer,
		const GLuint width, const GLuint height, const GLuint depth,
		const GLboolean generateMipmaps = GL_TRUE,
        GLuint internalFormat = GL_RGBA32F
	);
    
    Texture3D();
    
    inline void SetInternalFormat(GLuint value){ internalFormat = value; }
    inline void SetDepth(GLuint value){ depth = value; }
    
    virtual void SaveTextureState(GLboolean generateMipmaps = false, GLboolean loadTexture = GL_FALSE) override;
    virtual void Clear() override;
    
    
#ifdef __APPLE__
    
    //these functions are here because Apple supports up to OpenGL 4.1, so I will implement them myself in hopes one day
    //I won't need this.  These implementations are very specific to how it's called in this class, do not use it anywhere else
    
    void glTexStorage3D(	GLenum target,
                        GLsizei levels,
                        GLenum internalformat);
    
    virtual void glClearTexImage(	GLuint texture,
                         GLuint levels,
                         GLenum format,
                         GLenum type,
                         const void * data) override;
#endif
    
private:

    
    GLuint depth;
    GLuint internalFormat;
    //TODO: THIS ARRAY IS USED TO CLEAR MEMORY, WILL NOT BE NEEDED IN OPENGL 4.4 AND ABOVE
    //ANOTHER WAY TO DO THIS IN 4.1 IS TO USE OPENCL.  I WILL IMPLEMENT THIS IN A MEMORY PASS. 
	std::vector<GLfloat> clearData;
};
