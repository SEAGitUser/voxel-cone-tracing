//
//  VoxelConeTracingRT.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 4/5/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#pragma once

#include "RenderTarget.h"
#include "Graphic/Camera/Camera.h"

class VoxelizationConeTracingMaterial;
class Texture3D;


class VoxelConeTracingRT : public RenderTarget
{
public:
    
    VoxelConeTracingRT(Texture3D* albedoVoxels, Texture3D* normalVoxels, glm::mat4& voxViewProjection);
    
    void Render( Scene& scene) override;
    ~VoxelConeTracingRT() override;
    
private:
    void getVoxParameters(ShaderParameter::ShaderParamsGroup &settings, VoxProperties &voxProperties);
    void setLightingParameters(ShaderParameter::ShaderParamsGroup& settings, std::vector<PointLight> &lights);
    void setCameraParameters(ShaderParameter::ShaderParamsGroup& params, Camera &camera);
    void uploadRenderingSettings(ShaderParameter::ShaderParamsGroup& params, std::shared_ptr<VoxelizationConeTracingMaterial> &material );
private:
    
    Texture3D* albedoVoxels = nullptr;
    Texture3D* normalVoxels = nullptr;

    glm::mat4 voxViewProjection;
    
    static const int MAX_ARGUMENTS = 80;
    char arguments[MAX_ARGUMENTS ][MAX_ARGUMENTS ];
    
    std::shared_ptr<VoxelizationConeTracingMaterial> voxConeTracing = nullptr;
};
