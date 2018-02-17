#include "Texture3D.h"

#include <string>
#include <vector>

#include <assert.h>
#include "glm/glm.hpp"


#ifdef __APPLE__


void Texture3D::glTexStorage3D(	GLenum target,
                        GLsizei levels,
                        GLenum internalformat)
{
    //based off of https://www.khronos.org/opengl/wiki/GLAPI/glTexStorage3D
    
    GLint tempWidth = width;
    GLint tempHeight = height;
    GLint tempDepth = depth;
    
    assert(target == GL_TEXTURE_3D || target == GL_PROXY_TEXTURE_3D && "update implementation of glTexStorate3D to support the texture target");

    glError();
    for (GLsizei i = 0; i < levels; i++)
    {
        GLuint dataType = GL_FLOAT;
        glTexImage3D(target, i, internalformat, tempWidth, tempHeight, tempDepth, 0, pixelFormat, dataType, NULL);
        tempWidth = std::max(1, (tempWidth / 2));
        tempHeight = std::max(1, (tempHeight / 2));
        tempDepth = std::max(1, (tempDepth / 2));
    }
    glError();
}


void Texture3D::glClearTexImage(	GLuint texture,
                     GLuint levels,
                     GLenum format,
                     GLenum type,
                     const void * data)
{
    //based off of https://stackoverflow.com/questions/7195130/how-to-efficiently-initialize-texture-with-zeroes

    
    GLint tempWidth = width;
    GLint tempHeight = height;
    GLint tempDepth = depth;
    for (GLint i = 0; i < levels; i++)
    {
        
        glTexSubImage3D(GL_TEXTURE_3D, i, 0, 0, 0, tempWidth, tempHeight, tempDepth, pixelFormat, dataType, data);
        
        tempWidth = std::max(1, (tempWidth / 2));
        tempHeight = std::max(1, (tempHeight / 2));
        tempDepth = std::max(1, (tempDepth / 2));
    }
}

#endif


Texture3D::Texture3D():
Texture()
{

}


Texture3D::Texture3D(const std::vector<GLfloat> & textureBuffer, const GLuint _width, const GLuint _height, const GLuint _depth, const GLboolean generateMipmaps, GLuint _internalFormat) :
	Texture("", _width, _height), depth(_depth), internalFormat(_internalFormat)
{
    SaveTextureState(GL_FALSE, GL_FALSE);
}

void Texture3D::Clear()
{
    glError();
    glm::vec4 clearColor(0,0,0,0);
	GLint previousBoundTextureID;
	glGetIntegerv(GL_TEXTURE_BINDING_3D, &previousBoundTextureID);
	glBindTexture(GL_TEXTURE_3D, textureID);
    
    glError();

    //TODO: find a way to get rid of this clearData array
    if(clearData.size() == 0)
    {
        clearData.resize(4 * width * height * depth * sizeof(GLfloat), 0.0f);
    }
    glClearTexImage(textureID, levels, GL_RGBA, GL_FLOAT, &clearData[0]);
    glError();
    glBindTexture(GL_TEXTURE_3D, previousBoundTextureID);
}


void Texture3D::SaveTextureState(GLboolean generateMipmaps, GLboolean loadTexture)
{
    GLint previousTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_3D, &previousTexture);
    
    glError();
    if(textureID == INVALID_TEXTURE)
    {
        glGenTextures(1, &textureID);
    }
    
    glBindTexture(GL_TEXTURE_3D, textureID);
    
    glError();
    // Parameter options.
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrap);
    
    glError();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, minFilter);
    glError();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, magFilter);
    glError();

    glTexStorage3D(GL_TEXTURE_3D, levels, pixelFormat);
    GLint level = 0, border = 0;
    glTexImage3D(GL_TEXTURE_3D, level, internalFormat, width, height, depth, border, pixelFormat, dataType, &textureBuffer[0]);
    glError();
    if (generateMipmaps) glGenerateMipmap(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, previousTexture);
    
    glError();
}

void Texture3D::generateMipMap()
{
    glGenerateMipmap(GL_TEXTURE_3D);
}


