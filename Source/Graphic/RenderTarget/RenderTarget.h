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

class FBO;
class FBO_2D;
class Mesh;

class RenderTarget {
    
public:
    RenderTarget(){};
    
    virtual void Render( Scene& renderScene );
    
    virtual ~RenderTarget(){};
    
    inline FBO* getFBO(){ return voxelFBO;};
private:
    
    
protected:
    using RenderingQueue = const std::vector<Mesh*> &;
    FBO* voxelFBO;
    FBO_2D* defaultFBO;
};

