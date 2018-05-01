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
#include "glm.hpp"
#include "Graphic/Material/Resource.h"

class Texture : public Resource
{
public:
    
    class Commands
    {
    public:
        Commands(Texture* texture);
        virtual void setWrapMode(GLuint wrap) = 0;
        virtual void clear(glm::vec4 clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        virtual void setMinFiltering(GLuint minFilter) = 0;
        virtual void setMagFiltering(GLuint magFilter) = 0;
        virtual void generateMipmaps() =0;
        virtual void end();
        virtual void enableMipMaps() = 0;
        virtual void allocateOnGPU() = 0;
        void unpackAlignment(GLuint alignment);
        
        
        void deleteTexture();
        virtual ~Commands();
        
    private:
        //in certain situations, copying a Commands objects may trigger the destructor of the class which might deactivate the texture
        //in OpenGL, I don't think the sequence in which the copy constructor and destructor is guaranteed, in short, try not to copy
        //these classes
        Commands( Commands& rhs);
        
#if __APPLE__
        //this function is implemented  on opengl 4.4 and above, macs run on 4.1 so we'll implement it manually
        virtual void glClearTexImage(    GLuint texture,
                                     GLuint level,
                                     GLenum format,
                                     GLenum type,
                                     const void * data) = 0;
#endif
        
    protected:
        int previousTexture;
    private:

        static GLuint activeTexture;
        Texture* tex;
        
    };
    
    typedef struct Dimensions
    {
        inline Dimensions():width(0), height(0), depth(0){}
        
        GLuint width;
        GLuint height;
        GLuint depth;
        
    } Dimensions;
    
    
    typedef struct Properties
    {
        inline Properties():pixelFormat(GL_RGBA),
        dataFormat(GL_FLOAT), wrap(GL_CLAMP_TO_EDGE), minFilter(GL_LINEAR_MIPMAP_LINEAR), magFilter(GL_LINEAR),
        internalFormat(GL_RGBA32F)
        {}
        
        GLuint pixelFormat;
        GLuint dataFormat;
        GLuint wrap;
        GLuint internalFormat;
        GLuint minFilter;
        GLuint magFilter;
        
    } Properties;
    
    Texture():
        forceChannels(0),
        path(""),
        dataType(GL_FLOAT),
        wrap (GL_REPEAT),
        pixelFormat(GL_RGBA),
        minFilter(GL_LINEAR_MIPMAP_LINEAR),
        magFilter(GL_LINEAR),
        width(0),
        height(0),
        textureID(INVALID_TEXTURE)
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
        height(_height),
        textureID(INVALID_TEXTURE)
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
    inline void SetBuffer(unsigned char* buffer){ textureBuffer = buffer; } 
    
    inline int  GetTextureID() const { return textureID; }
    
    ~Texture()
    {
        glDeleteTextures(1, &textureID);
    }
    
protected:
    
    unsigned char * textureBuffer = nullptr;
    
    static const GLuint INVALID_TEXTURE = 0;
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
