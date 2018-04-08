//
//  FBO_3D.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include <iostream>
#include "FBO_3D.h"
#include "Graphic/Material/Texture/Texture3D.h"

FBO_3D::FBO_3D(Texture::Dimensions &_dimensions, Texture::Properties &_properties)
: FBO(_dimensions, _properties)
{
    
    GLint previousFrameBuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);
    
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glError();
    addRenderTarget();
    glError();
    GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert( e == GL_FRAMEBUFFER_COMPLETE);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);

}


Texture* FBO_3D::addRenderTarget()
{
    GLint previousFrameBuffer = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);
    
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    assert(renderTextures.size() < MAX_RENDER_TARGETS);
    
    Texture3D *target = new Texture3D();
    
    target->SetWrap(textureProperties.wrap);
    
    target->SetWidth(dimensions.width);
    target->SetHeight(dimensions.height);
    target->SetDepth(dimensions.depth);
    target->SetMinFilter(textureProperties.minFilter);
    target->SetMagFilter(textureProperties.magFilter);
    target->SetPixelFormat(textureProperties.pixelFormat);
    target->SetDataType(textureProperties.dataFormat);
    target->SetInternalFormat(textureProperties.internalFormat);
    
    target->SaveTextureState();

    glError();
    glBindTexture(GL_TEXTURE_3D, target->GetTextureID());
    glError();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (unsigned int)renderTextures.size(), target->GetTextureID(), 0);
    GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glError();
    assert(e == GL_FRAMEBUFFER_COMPLETE);
    glError();
    
    renderTextures.push_back(target);
    
    glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
    return target;
}

void FBO_3D::ClearRenderTextures()
{
    for(Texture* texture : renderTextures)
    {
        Texture3D* texture2D = static_cast<Texture3D*>(texture);
        Texture3D::Commands commands(texture2D);
        
        commands.clear();
    }
}
FBO_3D::~FBO_3D()
{
    glDeleteFramebuffers(1, &frameBuffer);
}
