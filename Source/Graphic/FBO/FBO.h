#pragma once

#include "Opengl_Includes.h"
#include "Graphic/Material/Texture/Texture.h"

#include <vector>


class FBO
{
    
public:

    Texture::Dimensions     dimensions;
    Texture::Properties     textureProperties;
    
    FBO():  frameBuffer(DEFAULT_FRAMEBUFFER),
            previousFrameBuffer(INVALID_FRAME_BUFFER)
    {
        getDefaultFBODimensions();
    }
    
    FBO(Texture::Dimensions &_dimensions, Texture::Properties &_textureProperties):
        frameBuffer(INVALID_FRAME_BUFFER),
        previousFrameBuffer(INVALID_FRAME_BUFFER)
    {
        dimensions = _dimensions;
        textureProperties = _textureProperties;
        renderTextures.reserve(MAX_RENDER_TARGETS);
    }
    
    inline GLint getFrameBufferID(){ return frameBuffer; }
    inline const Texture::Dimensions& getDimensions(){ return dimensions; }

    inline GLint getColorBufferTextureName() { return textureColorBuffer; }
    inline Texture* getRenderTexture(GLuint index){ return renderTextures[index];};
    void colorMaskOn(GLboolean value);
    void activateCulling(GLboolean value);
    
    void ClearRenderTextures();
    
    inline void ClearFBO()
    {
        assert(previousFrameBuffer != INVALID_FRAME_BUFFER);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    }
    
    void Activate();
    void Deactivate();
    
    virtual GLint AddRenderTarget() = 0;

private:
    inline void getDefaultFBODimensions()
    {
        GLint dims[4] = {0};
        glGetIntegerv(GL_VIEWPORT, dims);
        
        dimensions.width = dims[2];
        dimensions.height = dims[3];
        dimensions.depth = 0;
    }
protected:
    
    static const GLint MAX_RENDER_TARGETS = 15;
    static const GLint INVALID_FRAME_BUFFER = -1;
    static const GLuint DEFAULT_FRAMEBUFFER = 0;
    
    GLuint  frameBuffer, textureColorBuffer, attachment, rbo;
    
    std::vector<Texture*> renderTextures;
    GLint AddRenderTarget(Texture* target);
    GLint previousFrameBuffer;
    
};
