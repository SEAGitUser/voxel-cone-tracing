#include "FBO_2D.h"
#include "Graphic/Material/Texture/Texture2D.h"
#include <iostream>
#include <assert.h>

std::shared_ptr<FBO_2D> FBO_2D::defaultFBO = nullptr;

FBO_2D::FBO_2D(Texture::Dimensions &dimensions, Texture::Properties &textureProperties)
: FBO(dimensions, textureProperties)
{
    GLint previousFrameBuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);
    
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    
    AddRenderTarget();
    
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    
    assert(dimensions.width != 0 && dimensions.height != 0);
    
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, dimensions.width, dimensions.height); // Use a single rbo for both depth and stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

Texture* FBO_2D::AddRenderTarget(bool depthTarget)
{
    
    //todo: break this function into 2, one for color render targets, and another for depth render targets
    GLint previousFrameBuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    
    assert(renderTextures.size() < MAX_RENDER_TARGETS);
    assert(frameBuffer != DEFAULT_FRAMEBUFFER && "Cannot add a render target to a frame buffer object whose frame buffer is the default");

    Texture2D *target = new Texture2D();
    GLint targetID = FBO::setupRenderTarget(target);
    
    if(!depthTarget)
    {
        renderTextures.push_back(target);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (GLenum)(renderTextures.size() -1), GL_TEXTURE_2D, targetID, 0);
    }
    else
    {
        depthTexture = target;
        target->SetPixelFormat(GL_DEPTH_COMPONENT32);
        target->SaveTextureState();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, targetID, 0);
    }
    
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
    
    return target;

}

std::shared_ptr<FBO_2D>& FBO_2D::getDefault()
{
    if(defaultFBO == nullptr)
    {
        defaultFBO = std::make_shared<FBO_2D>();
    }
    
    return defaultFBO;
}

FBO_2D::~FBO_2D()
{
    glDeleteFramebuffers(1, &frameBuffer);
}

