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


class ComputeShader : public Resource
{
public:
    
    ComputeShader(const char* path, const char* methodName);
    ~ComputeShader();
    
    inline const dispatch_queue_t getDispatchQueue(){ return dispatch_queue; };
    inline const CGLShareGroupObj getShareGroupObj(){ return shareGroup; };
    
    int fileToString(const char *file_name, char **result_string, size_t *string_len);
    
    static const std::string computeShaderResourcePath;
    
    
protected:
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
};
