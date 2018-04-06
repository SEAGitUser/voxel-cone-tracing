//
//  VoxelVisualizationRT.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 12/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "VoxelVisualizationRT.h"
#include "Graphic/Material/MaterialStore.h"
#include "Graphic/Material/Voxelization/VoxelizationMaterial.h"
#include "Graphic/Material/Voxelization/VoxelVisualizationMaterial.h"
#include "Graphic/Material/Texture/Texture3D.h"
#include "Graphic/Camera/PerspectiveCamera.h"
#include "Utility/ObjLoader.h"
#include "Utility/Logger.h"
#include "Graphic/FBO/FBO_2D.h"


VoxelVisualizationRT::VoxelVisualizationRT(Texture3D* _voxelTexture)
{
    voxelVisualizationMaterial = MaterialStore::GET_MAT<VoxelVisualizationMaterial>("voxel-visualization");
    voxelTexture = _voxelTexture;
    
    //todo: ObjLoader should return a shared pointer
    cubeShape = ObjLoader::loadShapeFromObj("/Assets/Models/cube.obj");


}

void VoxelVisualizationRT::Render( Scene& scene )
{

    static ShaderParameter::ShaderParamsGroup group;
    ShaderParameter::Sampler3D sampler;
    sampler.texture= voxelTexture;
    
    group["texture3D"] = sampler;
    group["camPosition"] = scene.renderingCamera->position;
    
    glm::mat4 modelView = scene.renderingCamera->viewMatrix * cubeShape->transform.getTransformMatrix();
    group["M"] = cubeShape->transform.getTransformMatrix();
    group["stepSize"] = 1.0f/(GLfloat)VoxelizationMaterial::VOXEL_TEXTURE_DIMENSIONS;
    
    glm::mat4 mvp = scene.renderingCamera->getProjectionMatrix() * modelView;

    group["MVP"] = mvp;

    
    FBO::Commands commands(FBO_2D::getDefault().get());
    commands.setClearColor();
    commands.clearRenderTarget();
    commands.backFaceCulling(false);
    commands.enableDepthTest(false);
    commands.enableBlend(true);

    std::shared_ptr<Material> material =  std::static_pointer_cast<Material>(voxelVisualizationMaterial);
    cubeShape->render(scene, group, material.get());
    commands.enableDepthTest(true);
    commands.end();
}

VoxelVisualizationRT::~VoxelVisualizationRT()
{
    delete cubeShape;
}

