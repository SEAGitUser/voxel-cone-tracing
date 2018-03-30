//
//  FBO_2D.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#pragma once


#include "OpenGL_Includes.h"
#include <array>
#include <vector>

#include "FBO.h"
#include "Graphic/Material/Texture/Texture2D.h"


// https://www.opengl.org/wiki/Framebuffer_Object_Examples
/// <summary> An FBO. Manages important OpenGL calls. </summary>
class FBO_2D : public FBO
{
public:
    
    //todo: create a commands class for FBO2D
    class Commands : public FBO::Commands
    {
    public:

    private:
    
    };
    
    FBO_2D():FBO(),depthTexture(nullptr)
        {};
    
    FBO_2D(Texture::Dimensions &dimensions,  Texture::Properties &textureProperties);
    ~FBO_2D();
    
    virtual Texture* AddRenderTarget(bool depthTarget = false) override;
    inline Texture* getDepthTexture(){ return static_cast<Texture*>(depthTexture); }
private:
    
    GLuint generateAttachment(GLuint w, GLuint h, GLboolean depth, GLboolean stencil, GLenum magFilter, GLenum minFilter, GLenum wrap);
    
    Texture2D* depthTexture;
    
};
