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
class RenderTarget {
    
public:
    RenderTarget(){};
    
    virtual void SaveRenderState(){};
    virtual void RestoreRenderState(){};
    virtual void Render( Scene& renderScene ) = 0;
    
    virtual ~RenderTarget(){};
private:
    
    
protected:
    using RenderingQueue = const std::vector<MeshRenderer*> &;

};

