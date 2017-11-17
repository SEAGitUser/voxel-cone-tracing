

#include "FBO.h"
#include "Texture.h"


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
    
    
    return target->GetTextureID();
}
