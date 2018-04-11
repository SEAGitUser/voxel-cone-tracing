//
//  ComputeShader.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 4/10/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#include "ComputeShader.h"
#include <OpenGL/OpenGL.h>
#include <OpenCL/OpenCL.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <fstream>


const std::string ComputeShader::computeShaderResourcePath =  "/Compute Shaders/";

static const char* SEPARATOR = "----------------\n";

ComputeShader::ComputeShader(const char* path, const char* methodName):
dispatch_queue(0),
device_id(0)
{
    init();
    kernel = setupComputeKernel(path, methodName);
}

int ComputeShader::isExtensionSupported(
                                        const char* support_str, const char* ext_string, size_t ext_buffer_size)
{
    size_t offset = 0;
    const char* space_substr = strnstr(ext_string + offset, " ", ext_buffer_size - offset);
    size_t space_pos = space_substr ? space_substr - ext_string : 0;
    while (space_pos < ext_buffer_size)
    {
        if( strncmp(support_str, ext_string + offset, space_pos) == 0 )
        {
            // Device supports requested extension!
            std::cout << "Info: Found extension support " << support_str << std::endl;
            return 1;
        }
        // Keep searching -- skip to next token string
        offset = space_pos + 1;
        space_substr = strnstr(ext_string + offset, " ", ext_buffer_size - offset);
        space_pos = space_substr ? space_substr - ext_string : 0;
    }
    printf("Warning: Extension not supported ‘%s’!\n", support_str);
    return 0;
}


#if defined (__APPLE__) || defined(MACOSX)
static const char* CL_GL_SHARING_EXT = "cl_APPLE_gl_sharing";
#else
static const char* CL_GL_SHARING_EXT = "cl_khr_gl_sharing";
#endif

bool ComputeShader::init()
{
    cl_uint platform_count;
    cl_platform_id* platforms_id;
    
    cl_uint device_count = 0;
    
    cl_device_id devices[8];
    bool result = true;
    
    CGLContextObj kCGLContext = CGLGetCurrentContext();
    shareGroup = CGLGetShareGroup(kCGLContext);
    
    clGetPlatformIDs(0, NULL, &platform_count);
    
    platforms_id = (cl_platform_id*)malloc(platform_count*sizeof(cl_platform_id));
    clGetPlatformIDs(platform_count, platforms_id, NULL);
    
    
    cl_int error = clGetDeviceIDs(platforms_id[0], CL_DEVICE_TYPE_CPU, 1, &device_id, &device_count);
    
    free(platforms_id);
    assert(error == CL_SUCCESS);
    
    
    printDeviceInfo(device_id);
    cl_context_properties properties[] = {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
        (cl_context_properties)shareGroup, 0
    };
    
    size_t length;
    
    context = clCreateContext(properties, 1, &device_id, clLogMessagesToStdoutAPPLE, 0, &error);
    assert(error == CL_SUCCESS);
    
    clGetContextInfo( context, CL_CONTEXT_DEVICES, sizeof(devices), devices, &length);
    
    // Get string containing supported device extensions
    size_t ext_size = 40;
    char* ext_string = (char*)malloc(ext_size);
    clGetDeviceInfo(device_id, CL_DEVICE_IMAGE3D_MAX_DEPTH, sizeof(size_t), &ext_size, NULL);
    
    int supported = isExtensionSupported(CL_GL_SHARING_EXT, ext_string, ext_size);
    assert(supported);
    
    int err = 0;
    command_queue = clCreateCommandQueue(context, device_id, 0, &err);
    assert(err == CL_SUCCESS);
    
    free(ext_string);
    return result;
    
}


GLint ComputeShader::printDeviceInfo(cl_device_id device)
{
    GLchar name[128];
    GLchar vendor[128];
    GLint computeUnits = 0;
    
    clGetDeviceInfo(device, CL_DEVICE_NAME, 128, name, NULL);
    clGetDeviceInfo(device, CL_DEVICE_VENDOR, 128, vendor, NULL);
    clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(computeUnits), &computeUnits, NULL);
    fprintf(stdout, "%s : %s: %d compute units\n", vendor, name, computeUnits);
    return computeUnits;
}

bool ComputeShader::obtainGPUDevice()
{
    bool result = false;
    
    dispatch_queue = gcl_create_dispatch_queue(CL_DEVICE_TYPE_GPU, NULL);
    
    if( dispatch_queue )
    {
        device_id =
        gcl_get_device_id_with_dispatch_queue(dispatch_queue);
        //todo: let's make this pick the most powerful of the devices, right now it just gets the first one.
        fprintf(stdout, "\nAsking for CL_DEVICE_TYPE_GPU gives us:\n");
        printDeviceInfo(device_id);
        
        result = true;
    }
    
    return result;
}


bool ComputeShader::obtainCPUDevice()
{
    
    bool result = true;
    dispatch_queue = gcl_create_dispatch_queue(CL_DEVICE_TYPE_CPU, NULL);
    
    //no null check here, all macs have a opencl device
    device_id = gcl_get_device_id_with_dispatch_queue(dispatch_queue);
    fprintf(stdout, "\nAsking for CL_DEVICE_TYPE_CPU gives us:\n");
    printDeviceInfo(device_id);
    return result;
}

void ComputeShader::buildProgram(const char *source, const char* options)
{
    int err = 0;
    program = clCreateProgramWithSource(context, 1, (const char **) & source, NULL, &err);
    if (!program || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute program! %d\n", err);
        assert(false);
    }
    

    // Build the program executable
    std::cout <<  SEPARATOR;
    std::cout << "Building compute program...\n";
    err = clBuildProgram(program, 1, &device_id, options, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[20000];
        
        std::cout << SEPARATOR;
        //printf("Error: Failed to build program executable!\n");
        std::cerr << "Error: Failed to build program executable!\n";
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        std::cout << SEPARATOR;
        std::cout << SEPARATOR;
        assert(false);
    }
}

cl_kernel ComputeShader::assembleProgram(const char* source, const char* methodName, const char* options)
{
    int err = 0;
    
    // Create the compute kernel from within the program
    std::cout << "Creating kernel " << methodName << "..." << std::endl;
    cl_kernel kernel = clCreateKernel(program, methodName, &err);
    assert(kernel && err == CL_SUCCESS);
    
    
    // Get the maximum work group size for executing the kernel on the device
    err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &workgroup_size, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve kernel work group info! %d\n", err);
        
        assert(false);
    }
    
    printf("Maximum Workgroup Size '%d'\n", int(workgroup_size));
    
    return kernel;
}

cl_kernel ComputeShader::setupComputeKernel(const char *const _path, const char *const methodName, const char* const options)
{
    std::string rawShader;
    
    std::string path = Resource::resourceRoot + ComputeShader::computeShaderResourcePath + _path;

    std::ifstream fileStream(path, std::ios::in);
    if (!fileStream.is_open()) {
        std::cerr << "Couldn't load compute shader '" + std::string(path) + "'." << std::endl;
        fileStream.close();
        assert(false);
    }

    std::cout << SEPARATOR;
    std::cout << "Loading kernel source from file " << path << "..." << std::endl;
    std::string line = "";
    rawShader = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        rawShader.append(line + "\n");
    }
    
    buildProgram(rawShader.c_str(), options);
    
    return assembleProgram(rawShader.c_str(), methodName, options);
}


ComputeShader::~ComputeShader()
{
    if(dispatch_queue)
    {
        dispatch_release(dispatch_queue);
    }
}
