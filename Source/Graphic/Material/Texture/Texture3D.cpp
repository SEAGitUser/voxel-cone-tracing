#include "Texture3D.h"

#include <string>
#include <vector>

#include <assert.h>
#include "glm/glm.hpp"


#ifdef __APPLE__

std::vector<float> Texture3D::clearData = std::vector<float>();

#endif


Texture3D::Texture3D():
    Texture(),
    depth(0.0f),
    internalFormat(GL_RGBA32F)
{

}


Texture3D::Texture3D(const std::vector<float> & textureBuffer, const unsigned int _width, const unsigned int _height, const unsigned int _depth, const bool generateMipmaps, unsigned int _internalFormat) :
	Texture("", _width, _height), depth(_depth), internalFormat(_internalFormat)
{
    SaveTextureState(GL_FALSE, GL_FALSE);
}


void Texture3D::SaveTextureState(bool generateMipmaps, bool loadTexture)
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

void Texture3D::Commands::glClearTexImage(unsigned int texture_id, unsigned int levels, GLenum format, GLenum type, const void *data)
{
    //based off of https://stackoverflow.com/questions/7195130/how-to-efficiently-initialize-texture-with-zeroes
    
    unsigned int tempWidth = texture->width;
    unsigned int tempHeight = texture->height;
    unsigned int tempDepth = texture->depth;
    
    for (int i = 0; i < levels; i++)
    {
        
        glTexSubImage3D(GL_TEXTURE_3D, i, 0, 0, 0, tempWidth, tempHeight, tempDepth, texture->pixelFormat, texture->dataType, data);
        
        tempWidth = std::max(1, (int)(tempWidth >> 1));
        tempHeight = std::max(1, (int)(tempHeight >> 1));
        tempDepth = std::max(1, (int)(tempDepth >> 1));
    }
}
void Texture3D::Commands::glTexStorage3D(    GLenum target,
                               GLsizei levels,
                               GLenum internalformat)
{
    //based off of https://www.khronos.org/opengl/wiki/GLAPI/glTexStorage3D
    
    unsigned int tempWidth = texture->width;
    unsigned int tempHeight = texture->height;
    unsigned int tempDepth = texture->depth;
    
    assert(target == GL_TEXTURE_3D || target == GL_PROXY_TEXTURE_3D && "update implementation of glTexStorate3D to support the texture target");
    
    glError();
    for (GLsizei i = 0; i < levels; i++)
    {
        unsigned int dataType = GL_FLOAT;
        glTexImage3D(target, i, internalformat, tempWidth, tempHeight, tempDepth, 0, texture->pixelFormat, dataType, NULL);
        tempWidth = std::max(1, (int)(tempWidth >> 1));
        tempHeight = std::max(1, (int)(tempHeight >> 1));
        tempDepth = std::max(1, (int)(tempDepth >> 1));
    }
    glError();
}
#endif

void Texture3D::Commands::setWrapMode(unsigned int wrap)
{
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrap);
}

void Texture3D::Commands::setMinFiltering(unsigned int filter)
{
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter);
}

void Texture3D::Commands::setMagFiltering(unsigned int filter)
{
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filter);
}

void Texture3D::Commands::allocateOnGPU()
{
    glError();
    glTexStorage3D(GL_TEXTURE_3D, levels, texture->pixelFormat);
    int level = 0, border = 0;
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
