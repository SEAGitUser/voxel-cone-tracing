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

    void ActivateAsTexture(const int shaderProgram, const std::string glSamplerName, const int textureUnit = GL_TEXTURE0);
    
    FBO_3D(
        GLuint w, GLuint h, GLuint d, GLenum magFilter = GL_NEAREST, GLenum minFilter = GL_NEAREST,
        GLint internalFormat = GL_RGB16F, GLint format = GL_FLOAT, GLint wrap = GL_REPEAT);
    
    ~FBO_3D();
    
    virtual GLint AddRenderTarget() override;
private:
    
    GLuint depth;
    GLuint generateAttachment(GLuint w, GLuint h, GLboolean depth, GLboolean stencil, GLenum magFilter, GLenum minFilter, GLenum wrap);
    
    std::vector<Texture3D*> renderTargets;
};



