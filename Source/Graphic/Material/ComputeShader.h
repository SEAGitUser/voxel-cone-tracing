//
//  ComputeShader.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 4/10/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#pragma once

#include <OpenCL/OpenCL.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#include "Resource.h"
#include <unordered_map>

class ComputeShader : public Resource
{
public:
    
    ComputeShader(const char* path, const char* methodName);
    
    void setArgument(int index, int value);
    void setArgument(int index, float value );
    void setImageArgument(int index, int value);
    void run();
    
    
    ~ComputeShader();

    static const std::string computeShaderResourcePath;

protected:
    
    void aquireResources();
    
    inline const dispatch_queue_t getDispatchQueue(){ return dispatch_queue; };
    inline const CGLShareGroupObj getShareGroupObj(){ return shareGroup; };
    
    cl_kernel setupComputeKernel(const char* const shaderFilePath, const char* const methodName, const char* const options = nullptr);
    cl_kernel assembleProgram(const char* source, const char* methodName, const char* options = nullptr);
    bool init();
    
    
private:
    
    CGLShareGroupObj shareGroup;
    cl_program program;
    cl_device_id device_id;
    
    
    GLint printDeviceInfo(cl_device_id device);
    bool obtainGPUDevice();
    bool obtainCPUDevice();
    void buildProgram(const char* source, const char* options = nullptr);
    int isExtensionSupported( const char* support_str, const char* ext_string, size_t ext_buffer_size);
    
protected:
    cl_context context;
    dispatch_queue_t dispatch_queue;
    cl_command_queue command_queue;
    size_t workgroup_size;
    cl_kernel kernel;
    
    std::unordered_map<int, cl_image> images;
};
