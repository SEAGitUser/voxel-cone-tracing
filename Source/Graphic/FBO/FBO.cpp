

#include "FBO.h"
#include "Texture.h"


GLint FBO::AddRenderTarget(Texture* target)
{
    target->SetWrap(GL_CLAMP_TO_EDGE);
    
    target->SetWidth(width);
    target->SetHeight(height);
    target->SetMinFilter(GL_NEAREST);
    target->SetMagFilter(GL_NEAREST);
    target->SetPixelFormat(GL_RGBA);
    target->SetDataType(GL_FLOAT);
    
    target->SaveTextureState();
    
    
    return target->GetTextureID();
}
