//
//  RenderTarget.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 12/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#pragma once

#include "OpenGL_Includes.h"
#include "Scene/Scene.h"
#include "Graphic/Material/ShaderParameter.h"

class FBO;
class FBO_2D;
class Mesh;

class RenderTarget {
    
public:
    RenderTarget(){};
    
    virtual void Render( Scene& renderScene ) = 0;
    
    virtual ~RenderTarget(){};

    
protected:
    void setLightingParameters(ShaderParameter::ShaderParamsGroup& settings, std::vector<PointLight> &lights);
private:
    
    
protected:
    using RenderingQueue = const std::vector<Mesh*> &;
    
    static const int MAX_ARGUMENTS = 80;
    char arguments[MAX_ARGUMENTS ][MAX_ARGUMENTS ];

};

