

#include "FBO.h"
#include "Texture.h"
#include "OpenGL_Includes.h"

GLint FBO::AddRenderTarget(Texture* target)
{
    target->SetWrap(wrap);
    
    target->SetWidth(width);
    target->SetHeight(height);
    target->SetMinFilter(minFilter);
    target->SetMagFilter(magFilter);
    target->SetPixelFormat(pixelFormat);
    target->SetDataType(dataFormat);
    
    target->SaveTextureState();
    
    renderTextures.push_back(target);
    
    return target->GetTextureID();
}

void FBO::Clear()
{
    for(Texture* texture : renderTextures)
    {
        texture->Clear();
    }
}

void FBO::Activate()
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);
}

void FBO::Deactivate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
}
