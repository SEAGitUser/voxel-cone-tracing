//
//  FBO_3D.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include <iostream>
#include "FBO_3D.h"
#include "Graphic/Material/Texture3D.h"

FBO_3D::FBO_3D(GLuint w, GLuint h, GLuint d, GLenum magFilter, GLenum minFilter, GLint internalFormat, GLint format, GLint wrap)
: FBO(w, h), depth(d)
{
    GLint previousFrameBuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);
    
    // Init framebuffer.
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    
    AddRenderTarget();

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
    
    GLint textureID = FBO::AddRenderTarget(target);
    
    FBO::AddRenderTarget(target);
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (renderTargets.size() -1), target->GetTextureID(), 0);
    
    return textureID;
}

FBO_3D::~FBO_3D()
{
    
}
