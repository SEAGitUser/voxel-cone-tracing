

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
    assert(previousFrameBuffer == INVALID_FRAME_BUFFER && "Cannot clear while the framebuffer object is active");
    for(Texture* texture : renderTextures)
    {
        texture->Clear();
    }
}

void FBO::Activate()
{
    assert(previousFrameBuffer == INVALID_FRAME_BUFFER && "Cannot call Activate() without first calling Deactivate()");
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glViewport(0, 0, dimensions.width, dimensions.height);

}

void FBO::Deactivate()
{
    assert(previousFrameBuffer != INVALID_FRAME_BUFFER && "Cannot call Deactivate() without first calling Activate()");
    glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
    previousFrameBuffer = INVALID_FRAME_BUFFER;
}

void FBO::colorMaskOn(GLboolean value)
{
    glColorMask(value, value, value, value);
}

void FBO::activateCulling(GLboolean value)
{
    if(value == GL_FALSE)
    {
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
    else
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
    }

}
