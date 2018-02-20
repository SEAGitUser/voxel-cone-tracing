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

class Texture2D;


// https://www.opengl.org/wiki/Framebuffer_Object_Examples
/// <summary> An FBO. Manages important OpenGL calls. </summary>
class FBO_2D : public FBO
{
public:
    
    FBO_2D():FBO(){};
    
    FBO_2D(Texture::Dimensions &dimensions,  Texture::Properties &textureProperties);
    ~FBO_2D();
    
    virtual GLint AddRenderTarget() override;
private:
    GLuint generateAttachment(GLuint w, GLuint h, GLboolean depth, GLboolean stencil, GLenum magFilter, GLenum minFilter, GLenum wrap);
    
    std::vector<Texture2D*> renderTargets;
};
