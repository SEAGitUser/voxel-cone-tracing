#include "Texture2D.h"

#include <iostream>
#include <vector>
#include "glm/glm.hpp"

Texture2D::Texture2D(
	const std::string _shaderTextureName,
	const std::string &_path,
	const bool generateMipmaps,
	GLint _forceChannels)
: shaderTextureSamplerName(_shaderTextureName),
Texture(_path, 0,0)
{
    // Generate texture on GPU.
    glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::SaveTextureState( GLboolean generateMipmaps, GLboolean loadTexture)
{

    GLint previousTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTexture);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    glError();
    // Parameter options.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glError();
    // Set texture filtering options.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glError();
    
    // Mip maps.
    if (generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    static const GLint border = 0;
    glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, width, height, border, pixelFormat , dataType , nullptr);
    glError();
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
    glBindTexture(GL_TEXTURE_2D, previousTexture);
}

Texture2D::Texture2D():
shaderTextureSamplerName(""),
Texture()
{
    glGenTextures(1, &textureID);
}

void Texture2D::Activate(int shaderProgram, int textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(shaderProgram, shaderTextureSamplerName.c_str()), textureUnit);
}


#if __APPLE__

void Texture2D::glClearTexImage(	GLuint texture,
                             GLuint levels,
                             GLenum format,
                             GLenum type,
                             const void * data)
{
    //based off of https://stackoverflow.com/questions/7195130/how-to-efficiently-initialize-texture-with-zeroes
    
    GLint tempWidth = width;
    GLint tempHeight = height;
    for (GLint i = 0; i < levels; i++)
    {
        //TODO: lots of memory here, good enough for now. the number 16384 is  as big as the driver can handle on the mac
        static std::vector<GLubyte> emptyData(16384 * 16384  *4 * sizeof(GLfloat), 0);
        
        glTexSubImage2D(GL_TEXTURE_2D, i, 0, 0, tempWidth, tempHeight, pixelFormat, dataType, &emptyData[0]);
        
        tempWidth = std::max(1, (tempWidth / 2));
        tempHeight = std::max(1, (tempHeight / 2));
    }
}

#endif



void Texture2D::Clear()
{
    glm::vec4 clearColor(0,0,0,0);
    GLint previousBoundTextureID;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousBoundTextureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glClearTexImage(textureID, 0, GL_RGBA, GL_FLOAT, &clearColor);
    glBindTexture(GL_TEXTURE_2D, previousBoundTextureID);
}
Texture2D::~Texture2D()
{
    
}
