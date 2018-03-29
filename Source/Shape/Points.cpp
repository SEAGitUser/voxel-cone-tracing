//
//  PointRenderer.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 3/13/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#include "Points.h"



Points::Points(GLuint _count):
count(_count)
{
    indices.reserve(1);
    vertexData.reserve(1);
    
    indices.push_back(0);
    vertexData.push_back(glm::vec3(0.0f));
    
    Points::Commands commands(this);
    commands.uploadGPURenderingData();
    glError();
}

Points::~Points()
{
    Primitive::Commands commands(this);
    commands.destroyBuffers();
    commands.end();
}




/////////////Point::Commands

Points::Commands::Commands(Points* _point):
Primitive::Commands(_point),points(_point)
{
    
}

//
//void Points::Commands::destroyBuffers()
//{
//    assert(false);
////    glDeleteBuffers(1, &screenPoint->vao);
//}
//void Points::Commands::initBuffers()
//{
//    assert(false);
////    glGenBuffers(1, &(screenPoint->vbo));
//    //TODO: MAY HAVE TO DO THIS EVERY FRAME?
//    glBufferData(GL_ARRAY_BUFFER, sizeof(screenPoint->singleVertex), singleVertex, GL_STATIC_DRAW);
//}
//
//void Points::Commands::drawPoints(GLint count)
//{
//
//}
//
//void Points::Commands::end()
//{
//    glDisableVertexAttribArray(Primitive::Commands::POSITION_LOCATION);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//}
//
void Points::Commands::render()
{
    glBindBuffer(GL_ARRAY_BUFFER, points->vbo);
    glEnableVertexAttribArray(Primitive::Commands::POSITION_LOCATION);
    glVertexAttribPointer(Primitive::Commands::POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)offsetof(VertexData, position));
    glDrawArraysInstanced(GL_POINTS, 0, 3, points->count);
    glError();
}

Points::Commands::~Commands()
{
    //glDisableVertexAttribArray(Primitive::Commands::POSITION_LOCATION);
}



