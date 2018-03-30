//
//  Primitive.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 3/14/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#include "Primitive.h"


Primitive::Commands::Commands(Primitive* _primitive)
{
    primitive = _primitive;
    initBuffers();
    glBindVertexArray(primitive->vao);
}

void Primitive::Commands::uploadGPUVertexData()
{
    
    if(primitive->vertexData.size() != 0)
    {
        auto dataSize = sizeof(VertexData);
        static const int NUMBER_OF_ELEMENTS = 3;
        glBindBuffer(GL_ARRAY_BUFFER, primitive->vbo);
        glBufferData(GL_ARRAY_BUFFER, primitive->vertexData.size() * dataSize, primitive->vertexData.data(),
                     primitive->staticMesh ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(POSITION_LOCATION);
        glVertexAttribPointer(POSITION_LOCATION, NUMBER_OF_ELEMENTS, GL_FLOAT, GL_FALSE, dataSize, (GLvoid*)offsetof(VertexData, position));
        glEnableVertexAttribArray(TEXTURE_LOCATION);
        static const int NUMBER_OF_TEXTURE_ELEMENTS = 2;
        glVertexAttribPointer(TEXTURE_LOCATION, NUMBER_OF_TEXTURE_ELEMENTS, GL_FLOAT, GL_FALSE, dataSize, (GLvoid*)offsetof(VertexData, texCoord));
    }

}

void Primitive::Commands::uploadGPUIndexData()
{
    if(primitive->indices.size() != 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, primitive->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, primitive->indices.size() * sizeof(GLuint), primitive->indices.data(), primitive->staticMesh ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    }

}

void Primitive::Commands::uploadGPURenderingData()
{
    uploadGPUVertexData();
    uploadGPUIndexData();
    glError();
}


void Primitive::Commands::render()
{
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glDrawElements(GL_TRIANGLES, primitive->indices.size(), GL_UNSIGNED_INT, 0);
    glError();
}

void Primitive::Commands::initBuffers()
{
    if(primitive->vao == 0)
    {
        glGenVertexArrays(1, &(primitive->vao));
        glGenBuffers(1, &primitive->vbo);
        glGenBuffers(1, &primitive->ebo);
    }
}

void Primitive::Commands::destroyBuffers()
{
    glDeleteBuffers(1, &primitive->vbo);
    glDeleteBuffers(1, &primitive->ebo);
    glDeleteVertexArrays(1, &primitive->vao);
}

void Primitive::Commands::end()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Primitive::Commands::~Commands()
{
    end();
}
