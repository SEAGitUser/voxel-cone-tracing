#pragma once

#include "Opengl_Includes.h"
#include "Graphic/Material/Texture/Texture.h"
#include "glm/glm.hpp"
#include <vector>


class FBO
{
    
public:

    class Commands
    {
    public:
        Commands(FBO* fbo);
        virtual ~Commands();
        
        void setViewport(GLint width, GLint height);
        void getPreviousViewportSize();
        
        void backFaceCulling(bool value);
        void enableDepthTest(bool value);
        void colorMask( bool value);
        void depthMask( bool value);
        void activateCulling(bool value);
        void enableBlend(bool value);
        void enableAdditiveBlending();
        void blendSrcAlphaOneMinusSrcAlpha();
        void setClearColor(glm::vec4 color = glm::vec4(0.0f));
        void setDetphClearValue(GLfloat value = 0.0f);
        void clearRenderTarget();
        void end();
        void setupTargetsForRendering(bool threeDimensions = false);
        
    protected:
        static FBO* fbo;
        void init(FBO* fbo);
        Commands();
        
    private:
        
        //if you make these public, you'll have to think about changing the code in destructor of this class as well.
        //this is because rvalues get destroyed immidiately, causing things to malfunction
        Commands(const Commands& rhs);
        FBO& operator=(Commands& rhs);
        
        GLint previousViewportWidth;
        GLint previousViewportHeight;
        GLint previousFBO;

    };
    
    
    Texture::Dimensions     dimensions;
    Texture::Properties     textureProperties;
    
    FBO():  frameBuffer(DEFAULT_FRAMEBUFFER)
    {
    }
    
    FBO(Texture::Dimensions &_dimensions, Texture::Properties &_textureProperties)
    {
        dimensions = _dimensions;
        textureProperties = _textureProperties;
        renderTextures.reserve(MAX_RENDER_TARGETS);
    }
    
    
    inline          GLint getFrameBufferID();
    inline const    Texture::Dimensions& getDimensions();
    inline          GLint getColorBufferTextureName();
    inline          Texture* getRenderTexture(GLuint index);
    inline Commands& getCommands();
    inline void      getPreviousViewportDimensions();
    
    
    virtual void ClearRenderTextures() = 0;
    virtual Texture* addRenderTarget() = 0;
    
    virtual ~FBO();
    
protected:
    
    static const GLint MAX_RENDER_TARGETS = 15;
    static const GLint INVALID_FRAME_BUFFER = -1;
    static const GLuint DEFAULT_FRAMEBUFFER = 0;
    
    //keep the variable frameBuffer protected as it guarantees that the only classes that can create
    //FBO::Commands objects are the FBO family of classes.  This should guarantee that when you start
    //executing commands against a frame buffer, the frame buffer is active.
    GLuint  frameBuffer, textureColorBuffer, attachment, rbo;
    
    std::vector<Texture*> renderTextures;
    GLint setupRenderTarget(Texture* target);
    
};

const Texture::Dimensions& FBO::getDimensions()
{
    return dimensions;
}

GLint FBO::getColorBufferTextureName()
{
    return textureColorBuffer;
}

Texture* FBO::getRenderTexture(GLuint index)
{
    assert(renderTextures.size() > index);
    return renderTextures[index];
};



