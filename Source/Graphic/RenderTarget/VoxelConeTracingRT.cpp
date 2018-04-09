//
//  VoxelConeTracingRT.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 4/5/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#include "Graphic/Material/Material.h"
#include "Graphic/Material/Voxelization/VoxelizationConeTracingMaterial.h"
#include "VoxelConeTracingRT.h"
#include "Graphic/Material/MaterialStore.h"
#include "Shape/Mesh.h"
#include "Shape/Shape.h"
#include "Graphic/FBO/FBO.h"
#include "Graphic/FBO/FBO_2D.h"


VoxelConeTracingRT::VoxelConeTracingRT()
{
    voxConeTracing = MaterialStore::GET_MAT<VoxelizationConeTracingMaterial>("voxelization-cone-tracing");
}

void VoxelConeTracingRT::Render(Scene& scene)
{
    FBO::Commands commands(FBO_2D::getDefault().get());
    
    commands.setClearColor();
    commands.clearRenderTarget();
    commands.enableDepthTest(true);
    commands.backFaceCulling(true);
    commands.blendSrcAlphaOneMinusSrcAlpha();

    
    for(Shape* shape: scene.shapes)
    {
        size_t numberOfProperties = shape->getMeshProperties().size();
        glm::mat4 trans = shape->transform.getTransformMatrix();
        voxConeTracing->SetModelMatrix(trans);
        
        GLint i = 0;
        for(Mesh* mesh : shape->meshes)
        {
            VoxProperties prop = i < shape->meshProperties.size()  ? shape->meshProperties[i] : shape->defaultVoxProperties;
            voxConeTracing->uploadVoxParametersToGPU(shape->transform, scene, prop);
            mesh->render();
            ++i;
        }
    }
    
    commands.end();
}

VoxelConeTracingRT::~VoxelConeTracingRT()
{
    
}
