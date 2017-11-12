//
//  Texture.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/11/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#pragma once

#include "OpenGL_Includes.h"
#include <string>

#include "Graphic/Material/Resource.h"

class Texture : public Resource
{
    
public:
    
    Texture():
    forceChannels(0),
    path(""),
    dataType(GL_FLOAT),
    wrap (GL_REPEAT),
    pixelFormat(GL_RGBA),
    minFilter(GL_NEAREST),
    magFilter(GL_NEAREST),
    width(0),
    height(0)
    {
        
    }
    
    Texture(const std::string &_path, GLint _width, GLint _height):
    forceChannels(0),
    path(_path),
    dataType(GL_FLOAT),
    wrap (GL_REPEAT),
    pixelFormat(GL_RGBA),
    minFilter(GL_NEAREST),
    magFilter(GL_NEAREST),
    width(_width),
    height(_height)
    {
        
    }
    ///<summary> This saves the texture state in the GPU using the platform's graphics library based on arguments passed in setter functions and constructor arguments </summary>
    virtual void SaveTextureState(GLboolean generateMipmaps = false, GLboolean loadTexture = GL_FALSE) = 0;
    
    
    inline void SetWrap(GLint _wrap){ wrap = _wrap; }
    inline void SetPixelFormat(GLint _format){ pixelFormat = _format; }
    inline void SetWidth(GLint _width){ width = _width;};
    inline void SetHeight(GLint _height){ height = _height;};
    inline void SetMinFilter(GLint _minFilter){ minFilter = _minFilter; }
    inline void SetMagFilter(GLint _magFilter){ magFilter = _magFilter; }
    inline void SetDataType(GLint _type){ dataType = _type; }
    
    inline GLint  GetTextureID() const { return textureID; }
    
    ~Texture()
    {
        glDeleteTextures(1, &textureID);
    }
    
protected:
    
    GLint width, height, channels;
    const std::string path;
    GLboolean forceChannels;
    
    GLuint textureID;
    
    GLint pixelFormat;
    GLint dataType;
    GLint wrap;
    GLint minFilter;
    GLint magFilter;
    
};
