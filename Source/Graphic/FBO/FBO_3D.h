//
//  FBO_3D.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#pragma once

#include <array>
#include "OpenGL_Includes.h"
#include "FBO.h"
#include <vector>



class Texture3D;

class FBO_3D : public FBO
{
public:
    
    FBO_3D(
           GLuint w, GLuint h, GLuint d, GLuint magFilter = GL_NEAREST, GLuint minFilter = GL_NEAREST,
           GLuint _pixelFormat = GL_RGBA, GLuint dataFormat = GL_FLOAT, GLuint wrap = GL_CLAMP_TO_EDGE, GLuint internalFormat = GL_RGBA32F);
    
    void ActivateAsTexture(const int shaderProgram, const std::string glSamplerName, const int textureUnit = GL_TEXTURE0);
    

    inline GLuint getDepth(){return depth;}
    
    ~FBO_3D();
    
    virtual GLint AddRenderTarget() override;
private:
    
    GLuint depth;
    GLuint internalFormat;
    
    GLuint generateAttachment(GLuint w, GLuint h, GLboolean depth, GLboolean stencil, GLenum magFilter, GLenum minFilter, GLenum wrap);
    
    std::vector<Texture3D*> renderTargets;
};



