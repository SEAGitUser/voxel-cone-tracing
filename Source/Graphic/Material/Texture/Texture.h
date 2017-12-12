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
    minFilter(GL_LINEAR_MIPMAP_LINEAR),
    magFilter(GL_LINEAR),
    width(0),
    height(0)
    {
        
    }
    
    Texture(const std::string &_path, GLuint _width, GLuint _height):
    forceChannels(0),
    path(_path),
    dataType(GL_FLOAT),
    wrap (GL_REPEAT),
    pixelFormat(GL_RGBA),
    minFilter(GL_LINEAR_MIPMAP_LINEAR),
    magFilter(GL_LINEAR),
    width(_width),
    height(_height)
    {
        
    }
    ///<summary> This saves the texture state in the GPU using the platform's graphics library based on arguments passed in setter functions and constructor arguments </summary>
    virtual void SaveTextureState(GLboolean generateMipmaps = false, GLboolean loadTexture = GL_FALSE) = 0;
    
    
    inline void SetWrap(GLuint _wrap){ wrap = _wrap; }
    inline void SetPixelFormat(GLuint _format){ pixelFormat = _format; }
    inline void SetWidth(GLuint _width){ width = _width;};
    inline void SetHeight(GLuint _height){ height = _height;};
    inline void SetMinFilter(GLuint _minFilter){ minFilter = _minFilter; }
    inline void SetMagFilter(GLuint _magFilter){ magFilter = _magFilter; }
    inline void SetDataType(GLuint _type){ dataType = _type; }
    
    inline GLint  GetTextureID() const { return textureID; }
    
    virtual void Clear() = 0;
    
    ~Texture()
    {
        glDeleteTextures(1, &textureID);
    }
    
protected:
#if __APPLE__
    //this function is implemented  on opengl 4.4 and above, macs run on 4.1 so we'll implemented manually
    virtual void glClearTexImage(	GLuint texture,
                                 GLuint level,
                                 GLenum format,
                                 GLenum type,
                                 const void * data) = 0;
    
#endif
protected:
    
    static const GLsizei levels = 7;
    
    GLuint width, height, channels;
    const std::string path;
    GLboolean forceChannels;
    
    GLuint textureID;
    
    GLuint pixelFormat;
    GLuint dataType;
    GLuint wrap;
    GLuint minFilter;
    GLuint magFilter;
    
};
