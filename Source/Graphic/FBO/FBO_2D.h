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
    
    class Commands : public FBO::Commands
    {
    public:
        Commands(FBO_2D* fbo);
        void allocateOnGPU(Texture::Dimensions &dimensions);
        void addDepthTarget(GLint targetID);
        void deleteFrameBuffer();
        
        ~Commands() override;
        
    private:
        FBO_2D* fbo2d;
    };
    
    
    static std::shared_ptr<FBO_2D>& getDefault();
    
    FBO_2D():FBO(),depthTexture(nullptr)
        {};
    
    FBO_2D(Texture::Dimensions &dimensions,  Texture::Properties &textureProperties);
    ~FBO_2D() override;
    
    virtual Texture* addRenderTarget() override;
    virtual Texture* addDepthTarget();
    inline Texture* getDepthTexture(){ return static_cast<Texture*>(depthTexture); }
    void ClearRenderTextures() override;
    
private:
    
    GLuint generateAttachment(GLuint w, GLuint h, GLboolean depth, GLboolean stencil, GLenum magFilter, GLenum minFilter, GLenum wrap);
    void getPreviousViewportDimensions();
    
private:
    static std::shared_ptr<FBO_2D> defaultFBO;
    Texture2D* depthTexture;
    
};
