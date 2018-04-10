#include "FBO_2D.h"
#include "Graphic/Material/Texture/Texture2D.h"
#include <iostream>
#include <assert.h>

std::shared_ptr<FBO_2D> FBO_2D::defaultFBO = nullptr;

FBO_2D::FBO_2D(Texture::Dimensions &dimensions, Texture::Properties &textureProperties)
: FBO(dimensions, textureProperties)
{
    assert(dimensions.width != 0 && dimensions.height != 0);
    addRenderTarget();
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}


Texture* FBO_2D::addDepthTarget()
{
    FBO_2D::Commands commands(this);
    
    depthTexture = new Texture2D();
    FBO::setupRenderTarget(depthTexture);
    
    depthTexture->SetPixelFormat(GL_DEPTH_COMPONENT32);
    depthTexture->SaveTextureState();
    
    commands.addDepthTarget(depthTexture->GetTextureID());
    
    return depthTexture;
}
Texture* FBO_2D::addRenderTarget()
{

    FBO_2D::Commands commands(this);

    Texture2D *target = new Texture2D();
    GLint targetID = FBO::setupRenderTarget(target);
    renderTextures.push_back(target);

    commands.setupTargetsForRendering();

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    return target;

}

void FBO_2D::ClearRenderTextures()
{
    for(Texture* texture : renderTextures)
    {
        Texture2D* texture2D = static_cast<Texture2D*>(texture);
        Texture2D::Commands commands(texture2D);
        commands.clear();
    }
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


//Commands

FBO_2D::Commands::Commands(FBO_2D* _fbo2D):
FBO::Commands()
{
    fbo2d = _fbo2D;
    if(fbo2d->frameBuffer == 0)
    {
        glGenFramebuffers(1, &fbo2d->frameBuffer);
        glGenRenderbuffers(1, &fbo2d->rbo);
    }
    
    init(_fbo2D);
    
    glBindFramebuffer(GL_FRAMEBUFFER, fbo2d->frameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, fbo2d->rbo);
}

void FBO_2D::Commands::allocateOnGPU(Texture::Dimensions& dimensions)
{
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, dimensions.width, dimensions.height); // Use a single rbo for both depth and stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo2d->rbo);
}

void FBO_2D::Commands::addDepthTarget(GLint targetID)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, targetID, 0);
}

FBO_2D::Commands::~Commands()
{
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

