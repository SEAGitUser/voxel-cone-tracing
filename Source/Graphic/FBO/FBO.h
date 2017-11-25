#pragma once

#include "Opengl_Includes.h"


class Texture;
class FBO {
    
public:
    
    FBO(GLuint w, GLuint h, GLuint _minFilter, GLuint _magFilter, GLuint _pixelFormat, GLuint _dataFormat, GLuint _wrap )
    {
        width = w;
        height = h;
        minFilter = _minFilter;
        magFilter = _magFilter;
        dataFormat = _dataFormat;
        wrap = _wrap;
        pixelFormat = _pixelFormat;
    }
    
    
    inline GLint getFrameBufferID(){ return frameBuffer; }
    inline GLint getWidth(){ return width; }
    inline GLint getHeight(){ return height; }
    inline GLint getColorBufferTextureName() { return textureColorBuffer; }
    
    
    virtual GLint AddRenderTarget() = 0;
    GLint AddRenderTarget(Texture* target);
    
protected:
    static const GLint MAX_RENDER_TARGETS = 15;
    
    GLuint width, height, frameBuffer, textureColorBuffer, attachment, rbo;
    GLuint minFilter;
    GLuint magFilter;
    GLuint dataFormat, wrap, pixelFormat;
};
