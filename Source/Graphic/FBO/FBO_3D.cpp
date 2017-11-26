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

FBO_3D::FBO_3D(GLuint w, GLuint h, GLuint d, GLuint _minFilter, GLuint _magFilter, GLuint _pixelFormat, GLuint _dataFormat, GLuint _wrap,GLuint _internalFormat)
: FBO(w, h, _minFilter, _magFilter, _pixelFormat, _dataFormat, _wrap), depth(d), internalFormat(_internalFormat)
{
    
    GLint previousFrameBuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);
    
    // Init framebuffer.
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glError();
    AddRenderTarget();
    glError();
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h); // Use a single rbo for both depth and stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { std::cerr << "FBO failed to initialize correctly." << std::endl; }
}


GLint FBO_3D::AddRenderTarget()
{
    assert(renderTargets.size() < MAX_RENDER_TARGETS);
    
    Texture3D *target = new Texture3D();
    renderTargets.push_back(target);
    
    //GLint textureID = FBO::AddRenderTarget(target);
    
    target->SetWrap(wrap);
    
    target->SetWidth(width);
    target->SetHeight(height);
    target->SetDepth(depth);
    target->SetMinFilter(minFilter);
    target->SetMagFilter(magFilter);
    target->SetPixelFormat(pixelFormat);
    target->SetDataType(dataFormat);
    target->SetInternalFormat(internalFormat);
    
    target->SaveTextureState();

    glError();
    glBindTexture(GL_TEXTURE_3D, target->GetTextureID());
    glError();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , target->GetTextureID(), 0);
    GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert(e = GL_FRAMEBUFFER_COMPLETE);
    glError();
    return target->GetTextureID();
}

FBO_3D::~FBO_3D()
{
    for(Texture3D* texture : renderTargets)
    {
        delete texture;
    }
    
    glDeleteFramebuffers(1, &frameBuffer);
}
