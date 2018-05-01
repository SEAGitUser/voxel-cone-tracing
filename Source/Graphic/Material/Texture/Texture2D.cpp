#include "Texture2D.h"

#include <iostream>
#include <vector>
#include "glm/glm.hpp"

Texture2D::Texture2D(
	const std::string _shaderTextureName,
	const std::string &_path,
	const bool generateMipmaps,
	int _forceChannels)
: shaderTextureSamplerName(_shaderTextureName),
Texture(_path, 0,0)
{
}

void Texture2D::SaveTextureState( bool generateMipmaps, bool loadTexture)
{
    Texture2D::Commands commands(this);
    commands.setWrapMode(wrap);
    commands.setMinFiltering(minFilter);
    commands.setMagFiltering(magFilter);
    glError();
    
    if (generateMipmaps) {
        //commands.generateMipmaps();
        commands.enableMipMaps();
    }
    
    commands.allocateOnGPU();

    /*
     
     TODO: DON'T SUPPORT TEXTURES YET
    if(!path.empty() && loadTexture)
    {
        // Load image from disk.
        auto * textureBuffer = SOIL_load_image(path.c_str(), &width, &height, &channels, forceChannels);
        if (textureBuffer == nullptr) {
            std::cerr << "ERROR: Failed to load image with path " << path << " into a texture." << std::endl;
            return;
        }
        else {
            std::cout << "- SOIL: Successfully loaded image with path '" << path << "'." << std::endl;
        }
        
        //TODO: FIND  AWAY TO DETERMINE THE FORMAT/TYPE OF AN IMAGE BASED ON WHAT YOU ARE LOADING
        // Upload texture buffer.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, pixelFormat, dataType, textureBuffer);
        
        // Clean up.
        SOIL_free_image_data(textureBuffer);
    }
     */

}

Texture2D::Texture2D(bool dummyTexture):
shaderTextureSamplerName(""),
Texture()
{
    if(!dummyTexture)
    {
        SaveTextureState();
    }
}

Texture2D::~Texture2D()
{
    Texture2D::Commands commands(this);
    commands.deleteTexture();
    commands.end();
}


///Commands
Texture2D::Commands::Commands(Texture2D* _texture):
Texture::Commands(_texture)
{
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTexture);
    
    texture = _texture;
    if(texture->textureID == 0)
    {
        glGenTextures(1, &texture->textureID);
    }
    
//    assert((GL_TEXTURE0 + textureUnit) < GL_ACTIVE_TEXTURE);
//    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture->textureID);
}


void Texture2D::Commands::setWrapMode(unsigned int wrap)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
}

void Texture2D::Commands::setMagFiltering(unsigned int filter)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
}

void Texture2D::Commands::setMinFiltering(unsigned int filter)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
}

void Texture2D::Commands::generateMipmaps()
{
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::Commands::enableMipMaps()
{
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
}

void Texture2D::Commands::allocateOnGPU()
{
    static const int border = 0;
    int format = texture->pixelFormat == GL_DEPTH_COMPONENT32 ? GL_DEPTH_COMPONENT : texture->pixelFormat;
    glTexImage2D(GL_TEXTURE_2D, 0, texture->pixelFormat, texture->width, texture->height, border, format , texture->dataType , &texture->textureBuffer[0]);
    glError();
}

Texture2D::Commands::~Commands()
{
    end();
}

void Texture2D::Commands::clear(glm::vec4 clearColor)
{
    glClearTexImage(texture->textureID, 0, GL_RGBA, GL_FLOAT, &clearColor);
}

void Texture2D::Commands::end()
{
    Texture::Commands::end();
    glBindTexture(GL_TEXTURE_2D, previousTexture);
}

#if __APPLE__

void Texture2D::Commands::glClearTexImage(    unsigned int textureID,
                                unsigned int levels,
                                GLenum format,
                                GLenum type,
                                const void * data)
{
    //based off of https://stackoverflow.com/questions/7195130/how-to-efficiently-initialize-texture-with-zeroes
    
    int tempWidth = texture->width;
    int tempHeight = texture->height;
    for (int i = 0; i < levels; i++)
    {
        //TODO: lots of memory here, good enough for now. the number 16384 is  as big as the driver can handle on the mac
        static std::vector<GLubyte> emptyData(16384 * 16384  *4 * sizeof(GLfloat), 0);
        
        glTexSubImage2D(GL_TEXTURE_2D, i, 0, 0, tempWidth, tempHeight, texture->pixelFormat, texture->dataType, &emptyData[0]);
        
        tempWidth = std::max(1, (tempWidth / 2));
        tempHeight = std::max(1, (tempHeight / 2));
    }
}

#endif



