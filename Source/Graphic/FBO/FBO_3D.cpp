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
    addRenderTarget();
}


Texture* FBO_3D::addRenderTarget()
{
    assert(renderTextures.size() < MAX_RENDER_TARGETS);
    FBO_3D::Commands commands(this);
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
    
    renderTextures.push_back(target);

    bool threeDimensions = true;
    commands.setupTargetsForRendering(threeDimensions);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
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

//Commands

FBO_3D::Commands::Commands(FBO_3D* _fbo3d):
fbo3d(nullptr),
FBO::Commands()
{
    fbo3d = _fbo3d;
    if(fbo3d->frameBuffer == 0)
    {
        glGenFramebuffers(1, &fbo3d->frameBuffer);
    }
    init(_fbo3d);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo3d->frameBuffer);
}

FBO_3D::Commands::~Commands()
{
    
}

