//
//  RenderTarget.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 12/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "RenderTarget.h"
#include "Mesh.h"
#include "Shape.h"


void RenderTarget::setLightingParameters(ShaderParameter::ShaderParamsGroup& settings, std::vector<PointLight> &lights)
{
    GLuint index = 0;
    GLuint argumentIndex = 0;
    for(PointLight &light : lights)
    {
        assert(index < MAX_ARGUMENTS);
        
        sprintf(pointLightArguments[argumentIndex], "pointLights[%d].position", index);
        sprintf(pointLightArguments[argumentIndex+1], "pointLights[%d].color", index);
        
        const GLchar* str = pointLightArguments[argumentIndex];
        settings[str] = light.position;
        str = pointLightArguments[argumentIndex +1];
        settings[str] = light.color;
        ++index;
        argumentIndex += 2;
    }
    
    settings[Material::Commands::NUMBER_OF_LIGHTS_NAME] = (int)lights.size();
    
}
