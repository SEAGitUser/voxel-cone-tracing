//
//  ScreenQuad.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 3/30/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#include "ScreenQuad.h"


ScreenQuand::ScreenQuand()
{
    indices.reserve(4);
    vertexData.reserve(4);
   
    VertexData data;
    data.position = glm::vec3(-1.0f, -1.0f, 0.0f);
    data.texCoord = glm::vec2(0.0f, 0.0f);
    vertexData.push_back(data);
    
    data.position = glm::vec3(1.0f, -1.0f, 0.0f);
    data.texCoord = glm::vec2(1.0f, 0.0f);
    vertexData.push_back(data);
    
    data.position = glm::vec3(1.0f, 1.0f, 0.0f);
    data.texCoord = glm::vec2(1.0f, 1.0f);
    vertexData.push_back(data);
    
    data.position = glm::vec3(-1.0f, 1.0f,0.0f);
    data.texCoord = glm::vec2( 0.0f, 1.0f);
    vertexData.push_back(data);


    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(2);
    
    ScreenQuand::Commands commands(this);
    
    commands.uploadGPURenderingData();
    
}

///Commands

ScreenQuand::Commands::Commands(ScreenQuand* _quad):
Primitive::Commands(_quad)
{
    quad = _quad;
}


void ScreenQuand::Commands::render()
{
    glDrawElements(GL_TRIANGLE_STRIP, quad->indices.size(), GL_UNSIGNED_INT, 0);
}
