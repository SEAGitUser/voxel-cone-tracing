

#include "FBO.h"
#include "Texture.h"
#include "OpenGL_Includes.h"


GLint FBO::AddRenderTarget(Texture* target)
{
    target->SetWrap(textureProperties.wrap);
    
    target->SetWidth(dimensions.width);
    target->SetHeight(dimensions.height);
    target->SetMinFilter(textureProperties.minFilter);
    target->SetMagFilter(textureProperties.magFilter);
    target->SetPixelFormat(textureProperties.pixelFormat);
    target->SetDataType(textureProperties.dataFormat);
    
    target->SaveTextureState();
    
    renderTextures.push_back(target);
    
    return target->GetTextureID();
}

void FBO::ClearRenderTextures()
{
    for(Texture* texture : renderTextures)
    {
        texture->Clear();
    }
}

FBO::Commands FBO::Activate()
{
    FBO::Commands commands(frameBuffer);
    commands.setViewport(dimensions.width, dimensions.height);
    return commands;
}

///////////////////////////////////////////////////FBO::Commands

GLint FBO::Commands::fboID = FBO::DEFAULT_FRAMEBUFFER;

FBO::Commands::Commands(GLint _fboID)
{
    //you cannot manipulate two FBO objects at the same time in OpenGL.
    //call the end() function first, then call Activate() on another FBO object.
    assert(fboID == DEFAULT_FRAMEBUFFER && "Only one FBO::Commands is allowed at a time");
    fboID = _fboID;
    
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void FBO::Commands::setViewport(GLint width, GLint height)
{
     glViewport(0, 0, width, height);
}

void FBO::Commands::colorMask(bool _value)
{
    GLboolean value = _value ? GL_TRUE : GL_FALSE;
    glColorMask(value, value, value, value);
}

void FBO::Commands::activateCulling(bool value)
{
    value ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
}

void FBO::Commands::enableDepthTest(bool value)
{
    value ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void FBO::Commands::enableBlend(bool value)
{
    value ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}

void FBO::Commands::enableCullFace(bool _value)
{
    _value ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
}

FBO::Commands::Commands( const FBO::Commands& rhs)
{
    fboID = rhs.fboID;
}

void FBO::Commands::end()
{
    if(fboID != DEFAULT_FRAMEBUFFER)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, DEFAULT_FRAMEBUFFER);
        fboID = DEFAULT_FRAMEBUFFER;
    }
}

void FBO::Commands::clearRenderTarget()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

FBO::Commands::~Commands()
{
    end();
}





