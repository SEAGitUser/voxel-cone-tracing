#include "FBO_2D.h"
#include "Graphic/Material/Texture2D.h"
#include <iostream>
#include <assert.h>

FBO_2D::FBO_2D(GLuint w, GLuint h, GLuint minFilter, GLuint magFilter, GLuint pixelFormat, GLuint dataFormat, GLuint wrap)
: FBO(w, h, minFilter, magFilter, pixelFormat, dataFormat, wrap)
{
    GLint previousFrameBuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);
    
    // Init framebuffer.
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    
    textureColorBuffer = AddRenderTarget();

    
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h); // Use a single rbo for both depth and stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { std::cerr << "FBO failed to initialize correctly." << std::endl; }
}
/*
 
THIS CODE IS NOT USED ANYWHERE, NOT SURE THAT I NEED IT, I WILL LEAVE IT HERE FOR REFERENCE PURPOSES, BUT MAY DELETE IN THE FUTURE IF I DON'T FIND
USE FOR IT
 
 
GLuint FBO_2D::generateAttachment(GLuint w, GLuint h, GLboolean depth, GLboolean stencil, GLenum magFilter, GLenum minFilter, GLenum wrap)
{
    GLenum attachment_type = 0;
    if (!depth && !stencil) {
        attachment_type = GL_RGB;
    }
    else if (depth && !stencil) {
        attachment_type = GL_DEPTH_COMPONENT;
    }
    else if (!depth && stencil) {
        attachment_type = GL_STENCIL_INDEX;
    }
    
    assert(attachment_type != 0);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    if (!depth && !stencil) {
        glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, w, h, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, w, h, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return textureID;
}
 */

GLint FBO_2D::AddRenderTarget()
{
    assert(renderTargets.size() < MAX_RENDER_TARGETS);

    Texture2D *target = new Texture2D();
    renderTargets.push_back(target);
    
    GLint targetID = FBO::AddRenderTarget(target);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (renderTargets.size() -1), GL_TEXTURE_2D, targetID, 0);
    return targetID;

}

FBO_2D::~FBO_2D()
{
    for(Texture2D* texture : renderTargets)
    {
        delete texture;
    }
    
    glDeleteFramebuffers(1, &frameBuffer);
    

    
}
