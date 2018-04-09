//
//  VoxelConeTracingRT.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 4/5/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#pragma once

#include "RenderTarget.h"

class VoxelizationConeTracingMaterial;

class VoxelConeTracingRT : public RenderTarget
{
public:
    
    VoxelConeTracingRT();
    
    void Render( Scene& scene) override;
    ~VoxelConeTracingRT() override;
    
private:
    
    std::shared_ptr<VoxelizationConeTracingMaterial> voxConeTracing = nullptr;
};
