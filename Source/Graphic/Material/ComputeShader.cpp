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

ComputeShader::ComputeShader(const char* path, const char* methodName,
                             glm::vec3 _globalWorkSize, GLuint _dimensions):
dispatch_queue(0),
device_id(0)
{
    globalWorkSize[0] = _globalWorkSize.x;
    globalWorkSize[1] = _globalWorkSize.y;
    globalWorkSize[2] = _globalWorkSize.z;
    dimensions = _dimensions;
    
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
    
    cl_device_id device_id_temp[10];
    cl_int error = clGetDeviceIDs(platforms_id[0], CL_DEVICE_TYPE_CPU, 10, &device_id_temp[0], &device_count);
    
    free(platforms_id);
    assert(error == CL_SUCCESS);
    
    int computeUnits = 0;
    for(int i = 0; i < device_count; ++i)
    {
        int currentUnits = printDeviceInfo(device_id_temp[i]);
        if(computeUnits < currentUnits)
        {
            device_id = device_id_temp[i];
            computeUnits = currentUnits;
        }
    }
    
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


int ComputeShader::printDeviceInfo(cl_device_id device)
{
    GLchar name[128];
    GLchar vendor[128];
    int computeUnits = 0;
    
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

cl_kernel ComputeShader::setupComputeKernel(const char *const _path, const char *const _methodName, const char* const options)
{
    std::string rawShader;
    methodName = _methodName;
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


void ComputeShader::setArgument(int index, int value)
{
    int error = clSetKernelArg(kernel, index, sizeof(value), &value);
    if(error != CL_SUCCESS)
        std::cout << "parameter " << index << " for method " << methodName << " has failed" << std::endl;
}

void ComputeShader::setArgument(int index, float value )
{
    int error = clSetKernelArg(kernel, index, sizeof(value), &value);
    if(error != CL_SUCCESS)
        std::cout << "parameter " << index << " for method " << methodName << " has failed" << std::endl;
}

void ComputeShader::addTexture(int textureID, int textureType)
{
    if(images.count(textureID) == 0)
    {
        int error;
        cl_image image = clCreateFromGLTexture(context, textureType, GL_TEXTURE_3D, 0, textureID, &error);
        assert(error == CL_SUCCESS);
        images[textureID] = image;
    }
}
int ComputeShader::setReadImage3DArgument(int index, int textureID)
{

    addTexture(textureID, CL_MEM_READ_ONLY);
    
    argument_images[index] =images[textureID];
    int error = clSetKernelArg(kernel, index, sizeof(cl_image), &images[textureID]);
    return error;
}

int ComputeShader::setWriteImage3DArgument(int index, int textureID)
{
    addTexture(textureID, CL_MEM_WRITE_ONLY);
    argument_images[index] = images[textureID];
    int error = clSetKernelArg(kernel, index, sizeof(cl_image), &images[textureID]);
    return error;
}

int ComputeShader::setReadWriteImage3DArgument(int index, int textureID)
{
    addTexture(textureID, CL_MEM_READ_WRITE);
    argument_images[index] = images[textureID];
    int error =  clSetKernelArg(kernel, index, sizeof(cl_image), &images[textureID]);
    return error;
}

void ComputeShader::aquireResources()
{
    static int MAX_IMAGES = 9;
    cl_image image_objects[MAX_IMAGES];
    assert(MAX_IMAGES > argument_images.size());
    
    int i = 0;
    for( std::pair<int, cl_image> element : argument_images)
    {
        image_objects[i] = element.second;
        ++i;
    }
    //removing wait event beause it doesn't seem to have any effect...
    //cl_event opengl_get_completion;
    int err = clEnqueueAcquireGLObjects(command_queue, i, image_objects, 0,0, /*&opengl_get_completion*/0);
    //clWaitForEvents(1, &opengl_get_completion);
    //clReleaseEvent(opengl_get_completion);
    assert(err == CL_SUCCESS);
}

void ComputeShader::releaseResources()
{
    int i = 0;
    cl_image image_objects[MAX_IMAGES];
    for( std::pair<int, cl_image> element : argument_images)
    {
        image_objects[i] = element.second;
        ++i;
    }
    
    //removing the event wait here because it has no effect on anything... is not necessary.
    //cl_event opengl_get_completion;
    int err = clEnqueueReleaseGLObjects(command_queue, i, image_objects, 0,0, /*&opengl_get_completion*/ 0);
    //clWaitForEvents(1, &opengl_get_completion);
    //clReleaseEvent(opengl_get_completion);
    assert(err == CL_SUCCESS);
}

void ComputeShader::run()
{
    aquireResources();
    
    cl_event kernel_completion;

    int error = clEnqueueNDRangeKernel(command_queue, kernel, dimensions, NULL, globalWorkSize, nullptr, 0, NULL, &kernel_completion);
    checkError(error);
    error = clWaitForEvents(1, &kernel_completion);
    error |= clReleaseEvent(kernel_completion);
    checkError(error);
    
    releaseResources();
}


ComputeShader::~ComputeShader()
{
    if(dispatch_queue)
        dispatch_release(dispatch_queue);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);
}

bool ComputeShader::checkError(cl_int errMsg)
{
#ifdef DEBUG
    char errorMessage[255];
    
    switch(errMsg)
    {
        case CL_SUCCESS: strcpy(errorMessage, "CL_SUCCESS"); return true;
        case CL_DEVICE_NOT_FOUND: strcpy(errorMessage, "CL_DEVICE_NOT_FOUND"); break;
        case CL_DEVICE_NOT_AVAILABLE: strcpy(errorMessage, "CL_DEVICE_NOT_AVAILABLE"); break;
        case CL_COMPILER_NOT_AVAILABLE: strcpy(errorMessage, "CL_COMPILER_NOT_AVAILABLE"); break;
        case CL_MEM_OBJECT_ALLOCATION_FAILURE: strcpy(errorMessage, "CL_MEM_OBJECT_ALLOCATION_FAILURE"); break;
        case CL_OUT_OF_RESOURCES: strcpy(errorMessage, "CL_OUT_OF_RESOURCES"); break;
        case CL_OUT_OF_HOST_MEMORY: strcpy(errorMessage, "CL_OUT_OF_HOST_MEMORY"); break;
        case CL_PROFILING_INFO_NOT_AVAILABLE: strcpy(errorMessage, "CL_PROFILING_INFO_NOT_AVAILABLE"); break;
        case CL_MEM_COPY_OVERLAP: strcpy(errorMessage, "CL_MEM_COPY_OVERLAP"); break;
        case CL_IMAGE_FORMAT_MISMATCH: strcpy(errorMessage, "CL_IMAGE_FORMAT_MISMATCH"); break;
        case CL_IMAGE_FORMAT_NOT_SUPPORTED: strcpy(errorMessage, "CL_IMAGE_FORMAT_NOT_SUPPORTED"); break;
        case CL_BUILD_PROGRAM_FAILURE: strcpy(errorMessage, "CL_BUILD_PROGRAM_FAILURE"); break;
        case CL_MAP_FAILURE: strcpy(errorMessage, "CL_MAP_FAILURE"); break;
        case CL_MISALIGNED_SUB_BUFFER_OFFSET: strcpy(errorMessage, "CL_MISALIGNED_SUB_BUFFER_OFFSET"); break;
        case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: strcpy(errorMessage, "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST"); break;
        case CL_INVALID_VALUE: strcpy(errorMessage, "CL_INVALID_VALUE"); break;
        case CL_INVALID_DEVICE_TYPE: strcpy(errorMessage, "CL_INVALID_DEVICE_TYPE"); break;
        case CL_INVALID_PLATFORM: strcpy(errorMessage, "CL_INVALID_PLATFORM"); break;
        case CL_INVALID_DEVICE: strcpy(errorMessage, "CL_INVALID_DEVICE"); break;
        case CL_INVALID_CONTEXT: strcpy(errorMessage, "CL_INVALID_CONTEXT"); break;
        case CL_INVALID_QUEUE_PROPERTIES: strcpy(errorMessage, "CL_INVALID_QUEUE_PROPERTIES"); break;
        case CL_INVALID_COMMAND_QUEUE: strcpy(errorMessage, "CL_INVALID_COMMAND_QUEUE"); break;
        case CL_INVALID_HOST_PTR: strcpy(errorMessage, "CL_INVALID_HOST_PTR"); break;
        case CL_INVALID_MEM_OBJECT: strcpy(errorMessage, "CL_INVALID_MEM_OBJECT"); break;
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR: strcpy(errorMessage, "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR"); break;
        case CL_INVALID_IMAGE_SIZE: strcpy(errorMessage, "CL_INVALID_IMAGE_SIZE"); break;
        case CL_INVALID_SAMPLER: strcpy(errorMessage, "CL_INVALID_SAMPLER"); break;
        case CL_INVALID_BINARY: strcpy(errorMessage, "CL_INVALID_BINARY"); break;
        case CL_INVALID_BUILD_OPTIONS: strcpy(errorMessage, "CL_INVALID_BUILD_OPTIONS"); break;
        case CL_INVALID_PROGRAM: strcpy(errorMessage, "CL_INVALID_PROGRAM"); break;
        case CL_INVALID_PROGRAM_EXECUTABLE: strcpy(errorMessage, "CL_INVALID_PROGRAM_EXECUTABLE"); break;
        case CL_INVALID_KERNEL_NAME: strcpy(errorMessage, "CL_INVALID_KERNEL_NAME"); break;
        case CL_INVALID_KERNEL_DEFINITION: strcpy(errorMessage, "CL_INVALID_KERNEL_DEFINITION"); break;
        case CL_INVALID_KERNEL: strcpy(errorMessage, "CL_INVALID_KERNEL"); break;
        case CL_INVALID_ARG_INDEX: strcpy(errorMessage, "CL_INVALID_ARG_INDEX"); break;
        case CL_INVALID_ARG_VALUE: strcpy(errorMessage, "CL_INVALID_ARG_VALUE"); break;
        case CL_INVALID_ARG_SIZE: strcpy(errorMessage, "CL_INVALID_ARG_SIZE"); break;
        case CL_INVALID_KERNEL_ARGS: strcpy(errorMessage, "CL_INVALID_KERNEL_ARGS"); break;
        case CL_INVALID_WORK_DIMENSION: strcpy(errorMessage, "CL_INVALID_WORK_DIMENSION"); break;
        case CL_INVALID_WORK_GROUP_SIZE: strcpy(errorMessage, "CL_INVALID_WORK_GROUP_SIZE"); break;
        case CL_INVALID_WORK_ITEM_SIZE: strcpy(errorMessage, "CL_INVALID_WORK_ITEM_SIZE"); break;
        case CL_INVALID_GLOBAL_OFFSET: strcpy(errorMessage, "CL_INVALID_GLOBAL_OFFSET"); break;
        case CL_INVALID_EVENT_WAIT_LIST: strcpy(errorMessage, "CL_INVALID_EVENT_WAIT_LIST"); break;
        case CL_INVALID_EVENT: strcpy(errorMessage, "CL_INVALID_EVENT"); break;
        case CL_INVALID_OPERATION: strcpy(errorMessage, "CL_INVALID_OPERATION"); break;
        case CL_INVALID_GL_OBJECT: strcpy(errorMessage, "CL_INVALID_GL_OBJECT"); break;
        case CL_INVALID_BUFFER_SIZE: strcpy(errorMessage, "CL_INVALID_BUFFER_SIZE"); break;
        case CL_INVALID_MIP_LEVEL: strcpy(errorMessage, "CL_INVALID_MIP_LEVEL"); break;
        case CL_INVALID_GLOBAL_WORK_SIZE: strcpy(errorMessage, "CL_INVALID_GLOBAL_WORK_SIZE"); break;
        case CL_INVALID_PROPERTY: strcpy(errorMessage, "CL_INVALID_PROPERTY"); break;
        default: strcpy(errorMessage, "unknown error"); break;
    }
    std::cout << errorMessage <<  std::endl;
    assert(errMsg == CL_SUCCESS);
#endif
    return false;
}
