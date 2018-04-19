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
        
        sprintf(arguments[argumentIndex], "pointLights[%d].position", index);
        sprintf(arguments[argumentIndex+1], "pointLights[%d].color", index);
        
        const GLchar* str = arguments[argumentIndex];
        settings[str] = light.position;
        str = arguments[argumentIndex +1];
        settings[str] = light.color;
        ++index;
        argumentIndex += 2;
    }
    
    settings[Material::Commands::NUMBER_OF_LIGHTS_NAME] = (unsigned int)lights.size();
    
}
