//
//  VoxelVisualizationRT.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 12/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "VoxelVisualizationRT.h"
#include "Graphic/Material/MaterialStore.h"
#include "Graphic/Material/Voxelization/VoxelVisualizationMaterial.h"
#include "Graphic/Renderer/MeshRenderer.h"
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
    cubeShape = ObjLoader::loadObjFile("/Assets/Models/cube.obj");
    cubeMeshRenderer = std::make_shared<MeshRenderer>(&cubeShape->meshes[0]);
    
    //this will render to the default frame buffer
    fbo = new FBO_2D();
}

void VoxelVisualizationRT::Render( Scene& scene )
{

    static MaterialSetting::SettingsGroup group;
    MaterialSetting::Sampler3D sampler;
    sampler.texture= voxelTexture;
    
    group["texture3D"] = sampler;
    group["camPosition"] = scene.renderingCamera->position;
    group["focalLength"] = scene.renderingCamera->getFocalLength();
    
    glm::mat4 modelView = scene.renderingCamera->viewMatrix * cubeMeshRenderer->transform.getTransformMatrix();
    group["viewMatrix"] = modelView;
    
    group["M"] = cubeMeshRenderer->transform.getTransformMatrix();

    glm::mat4 mvp = scene.renderingCamera->getProjectionMatrix() * modelView;

    group["MVP"] = mvp;

    FBO::Commands commands = fbo->Activate();
    commands.setClearColor();
    commands.clearRenderTarget();
    commands.enableCullFace(true);
    commands.enableDepthTest(true);
    commands.enableBlend(true);

    std::shared_ptr<Material> material =  std::static_pointer_cast<Material>(voxelVisualizationMaterial);
    cubeMeshRenderer->render(scene, group, material.get());
}

VoxelVisualizationRT::~VoxelVisualizationRT()
{
    delete cubeShape;
    delete fbo;
}

