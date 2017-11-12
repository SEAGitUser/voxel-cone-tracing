#pragma once

#include "Opengl_Includes.h"


class Texture;
class FBO {
    
public:
    
    FBO(GLuint w, GLuint h )
    {
        width = w;
        height = h;
    }
    
    
    inline GLint getFrameBufferID(){ return frameBuffer; }
    inline GLint getWidth(){ return width; }
    inline GLint getHeight(){ return height; }
    inline GLint getColorBufferTextureID() { return textureColorBuffer; }
    
    
    virtual GLint AddRenderTarget() = 0;
    GLint AddRenderTarget(Texture* target);
    
protected:
    static const GLint MAX_RENDER_TARGETS = 15;
    
    GLuint width, height, frameBuffer, textureColorBuffer, attachment, rbo;
};
