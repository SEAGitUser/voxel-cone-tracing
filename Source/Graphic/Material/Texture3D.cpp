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
        GLuint pixelFormat = internalformat;
        GLuint dataType = GL_FLOAT;
        glTexImage3D(target, i, internalformat, tempWidth, tempHeight, tempDepth, 0, pixelFormat, dataType, NULL);
        tempWidth = std::max(1, (tempWidth / 2));
        tempHeight = std::max(1, (tempHeight / 2));
        tempDepth = std::max(1, (tempDepth / 2));
    }
    glError();
}


void Texture3D::glClearTexImage(	GLuint texture,
                     GLint level,
                     GLenum format,
                     GLenum type,
                     const void * data)
{
    //based off of https://stackoverflow.com/questions/7195130/how-to-efficiently-initialize-texture-with-zeroes
    //TODO: this function has potential to be a performance bottleneck, make sure to profile
    GLsizei levels = 7;
    
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

Texture3D::Texture3D(const std::vector<GLfloat> & textureBuffer, const int _width, const int _height, const int _depth, const bool generateMipmaps) :
	Texture("", _width, _height), depth(_depth), clearData(4 * _width * _height * _depth * sizeof(GLfloat), 0.0f)
{

}

void Texture3D::Activate(const int shaderProgram, const std::string glSamplerName, const int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_3D, textureID);
	glUniform1i(glGetUniformLocation(shaderProgram, glSamplerName.c_str()), textureUnit);
}

void Texture3D::Clear()
{
    glm::vec4 clearColor(0,0,0,0);
	GLint previousBoundTextureID;
	glGetIntegerv(GL_TEXTURE_BINDING_3D, &previousBoundTextureID);
	glBindTexture(GL_TEXTURE_3D, textureID);
    
    glError();

	glClearTexImage(textureID, 0, GL_RGBA, GL_FLOAT, &clearData[0]);
	glBindTexture(GL_TEXTURE_3D, previousBoundTextureID);
}


void Texture3D::SaveTextureState(GLboolean generateMipmaps, GLboolean loadTexture)
{
    GLint previousTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_3D, &previousTexture);
    
    glError();
    // Generate texture on GPU.
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_3D, textureID);
    
    glError();
    // Parameter options.
    const auto wrap = GL_CLAMP_TO_BORDER;
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrap);
    
    glError();
    const auto filter = GL_LINEAR_MIPMAP_LINEAR;
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glError();
    // Upload texture buffer.
    const int levels = 7;
    glTexStorage3D(GL_TEXTURE_3D, levels, GL_RGBA);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, width, height, depth, 0, GL_RGBA, GL_FLOAT, &textureBuffer[0]);
    if (generateMipmaps) glGenerateMipmap(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, previousTexture);
}

Texture3D::Texture3D():
Texture()
{
    
}