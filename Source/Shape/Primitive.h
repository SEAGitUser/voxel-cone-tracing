//
//  Primitive.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 3/14/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#pragma once

#include "OpenGL_Includes.h"
#include <vector>
#include "VertexData.h"

//Primitive family of classes holds data and makes use of the Primitive::Commands family of classes
//to render it's data
class Primitive
{
public:

    //Primitive::Commands family of classes wraps graphics API calls to communicate with the GPU
    class Commands
    {
    public:
        explicit Commands(Primitive* primitive);
        Commands(){};
        virtual void render();
        virtual void end();
        
        virtual void uploadGPURenderingData();
        virtual void destroyBuffers();
        
        static const int POSITION_LOCATION = 0;
        static const unsigned int NORMALS_LOCATION = 1;
        static const int TEXTURE_LOCATION = 2;
        
        virtual void uploadGPUVertexData();
        virtual void uploadGPUIndexData();
        
        virtual ~Commands();
        
    protected:

        virtual void initBuffers();
    protected:
        Primitive* primitive = nullptr;
    };
    
    Primitive():vao(0), vbo(0), ebo(0){}
    
protected:
    bool staticMesh = true;
    unsigned int vao;
    unsigned int vbo, ebo; // Vertex Buffer Object, Vertex Array Object, Element Buffer Object.
    std::vector<VertexData> vertexData;
    std::vector<unsigned int> indices;

};
