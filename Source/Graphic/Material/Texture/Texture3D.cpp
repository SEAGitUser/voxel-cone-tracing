#include "Texture3D.h"

#include <string>
#include <vector>

#include <assert.h>
#include "glm/glm.hpp"


#ifdef __APPLE__

std::vector<GLfloat> Texture3D::clearData = std::vector<GLfloat>();

#endif


Texture3D::Texture3D():
    Texture(),
    depth(0.0f),
    internalFormat(GL_RGBA32F)
{

}


Texture3D::Texture3D(const std::vector<GLfloat> & textureBuffer, const GLuint _width, const GLuint _height, const GLuint _depth, const GLboolean generateMipmaps, GLuint _internalFormat) :
	Texture("", _width, _height), depth(_depth), internalFormat(_internalFormat)
{
    SaveTextureState(GL_FALSE, GL_FALSE);
}


void Texture3D::SaveTextureState(GLboolean generateMipmaps, GLboolean loadTexture)
{

    Texture3D::Commands commands (this);
    commands.setWrapMode(wrap);
    glError();
    commands.setMinFiltering(minFilter);
    commands.setMagFiltering(magFilter);

    commands.allocateOnGPU();
    
    if(generateMipmaps)
    {
        commands.enableMipMaps();
    }
    glError();
}


Texture3D::~Texture3D()
{
    Texture3D::Commands commands(this);
    commands.deleteTexture();
    commands.end();
    glError();
}

///COMMANDS
Texture3D::Commands::Commands(Texture3D* _texture):
Texture::Commands(_texture)
{
    texture = _texture;
    if(texture->textureID == INVALID_TEXTURE)
    {
        glGenTextures(1, &texture->textureID);
    }
    
    glBindTexture(GL_TEXTURE_3D, texture->textureID);
}

#ifdef __APPLE__

void Texture3D::Commands::glClearTexImage(GLuint texture_id, GLuint levels, GLenum format, GLenum type, const void *data)
{
    //based off of https://stackoverflow.com/questions/7195130/how-to-efficiently-initialize-texture-with-zeroes
    
    GLuint tempWidth = texture->width;
    GLuint tempHeight = texture->height;
    GLuint tempDepth = texture->depth;
    
    for (GLint i = 0; i < levels; i++)
    {
        
        glTexSubImage3D(GL_TEXTURE_3D, i, 0, 0, 0, tempWidth, tempHeight, tempDepth, texture->pixelFormat, texture->dataType, data);
        
        tempWidth = std::max(1, (GLint)(tempWidth >> 1));
        tempHeight = std::max(1, (GLint)(tempHeight >> 1));
        tempDepth = std::max(1, (GLint)(tempDepth >> 1));
    }
}
void Texture3D::Commands::glTexStorage3D(    GLenum target,
                               GLsizei levels,
                               GLenum internalformat)
{
    //based off of https://www.khronos.org/opengl/wiki/GLAPI/glTexStorage3D
    
    GLuint tempWidth = texture->width;
    GLuint tempHeight = texture->height;
    GLuint tempDepth = texture->depth;
    
    assert(target == GL_TEXTURE_3D || target == GL_PROXY_TEXTURE_3D && "update implementation of glTexStorate3D to support the texture target");
    
    glError();
    for (GLsizei i = 0; i < levels; i++)
    {
        GLuint dataType = GL_FLOAT;
        glTexImage3D(target, i, internalformat, tempWidth, tempHeight, tempDepth, 0, texture->pixelFormat, dataType, NULL);
        tempWidth = std::max(1, (GLint)(tempWidth >> 1));
        tempHeight = std::max(1, (GLint)(tempHeight >> 1));
        tempDepth = std::max(1, (GLint)(tempDepth >> 1));
    }
    glError();
}
#endif

void Texture3D::Commands::setWrapMode(GLuint wrap)
{
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrap);
}

void Texture3D::Commands::setMinFiltering(GLuint filter)
{
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter);
}

void Texture3D::Commands::setMagFiltering(GLuint filter)
{
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filter);
}

void Texture3D::Commands::allocateOnGPU()
{
    glError();
    glTexStorage3D(GL_TEXTURE_3D, levels, texture->pixelFormat);
    GLint level = 0, border = 0;
    glTexImage3D(GL_TEXTURE_3D, level, texture->internalFormat, texture->width, texture->height, texture->depth, border, texture->pixelFormat, texture->dataType, &texture->textureBuffer[0]);
    glError();
}

void Texture3D::Commands::end()
{
    Texture::Commands::end();
    glBindTexture(GL_TEXTURE_2D, previousTexture);
}
void Texture3D::Commands::generateMipmaps()
{
    glGenerateMipmap(GL_TEXTURE_3D);
}

void Texture3D::Commands::enableMipMaps()
{
    glTexParameteri(GL_TEXTURE_3D, GL_GENERATE_MIPMAP, GL_TRUE);
}

Texture3D::Commands::~Commands()
{
    end();
}
