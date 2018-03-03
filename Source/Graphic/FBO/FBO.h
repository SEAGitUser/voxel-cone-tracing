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
        Commands(GLint fboID);
        Commands(const Commands& rhs);
        ~Commands();
        
        void setViewport(GLint width, GLint height);
        void enableCullFace(bool value);
        void enableDepthTest(bool value);
        void colorMask( bool value);
        void activateCulling(bool value);
        void enableBlend(bool value);
        void setClearColor(glm::vec4 color = glm::vec4(0.0f));
        void clearRenderTarget();
        void end();
        
    protected:
        static GLint fboID;
    };
    
    
    Texture::Dimensions     dimensions;
    Texture::Properties     textureProperties;
    
    FBO():  frameBuffer(DEFAULT_FRAMEBUFFER)
    {
        getDefaultFBODimensions();
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
    inline void     ClearFBO();
    inline Commands& getCommands();
    
    
    void ClearRenderTextures();
    Commands Activate();
    
    virtual GLint AddRenderTarget() = 0;

private:
    inline void getDefaultFBODimensions();
    
protected:
    
    static const GLint MAX_RENDER_TARGETS = 15;
    static const GLint INVALID_FRAME_BUFFER = -1;
    static const GLuint DEFAULT_FRAMEBUFFER = 0;
    
    //keep the variable frameBuffer private as it guarantees that the only classes that can create
    //FBO::Commands objects are the FBO family of classes.  It also forces the clients to call Activate() on
    //the FBO object.  This will make sure the FBO is active on the OpenGL side before opengl commands against this FBO
    //are ran.
    GLuint  frameBuffer, textureColorBuffer, attachment, rbo;
    
    std::vector<Texture*> renderTextures;
    GLint AddRenderTarget(Texture* target);
    
};


void FBO::getDefaultFBODimensions()
{
    GLint dims[4] = {0};
    glGetIntegerv(GL_VIEWPORT, dims);
    
    dimensions.width = dims[2];
    dimensions.height = dims[3];
    dimensions.depth = 0;
}

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
    return renderTextures[index];
};



