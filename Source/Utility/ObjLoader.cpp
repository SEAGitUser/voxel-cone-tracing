

#define __UTILITY_LOG_LOADING_TIME true

#if __UTILITY_LOG_LOADING_TIME

#include <iostream>
#include <iomanip>
#include "OpenGL_Includes.h"

#include "Time/FrameRate.h"
#endif

#include "ObjLoader.h"

#include <fstream>
#include <vector>
#include <algorithm>



#include "External/tiny_obj/tiny_obj_loader.h"
#include "Shape/VertexData.h"
#include "Shape/Mesh.h"

Shape * ObjLoader::loadShapeFromObj(const std::string &path)
{
    
    std::string assetPath = AssetStore::resourceRoot + path;
#if __UTILITY_LOG_LOADING_TIME
    double logTimestamp = glfwGetTime();
    double took;
    std::cout << "Loading obj '" << assetPath << "'..." << std::endl;
#endif
    
    
    RawObjData rawObjData;
	

    loadRawObjData(path, rawObjData);

#if __UTILITY_LOG_LOADING_TIME
    took = glfwGetTime() - logTimestamp;
    std::cout << std::setprecision(4) << " - Parsing '" << assetPath << "' took " << took << " seconds (by tinyobjloader)." << std::endl;
    logTimestamp = glfwGetTime();
#endif
    
    Shape * result = new Shape(rawObjData.shapes);

#if __UTILITY_LOG_LOADING_TIME
	took = glfwGetTime() - logTimestamp;
	std::cout << std::setprecision(4) << " - Loading '" << assetPath << "' took " << took << " seconds." << std::endl;
#endif
	return result;
}

void ObjLoader::loadRawObjData(const std::string &path, ObjLoader::RawObjData &rawObjData)
{
    std::string assetPath = AssetStore::resourceRoot + path;

    
    std::string err;
    if (!tinyobj::LoadObj(rawObjData.shapes, rawObjData.materials, err, assetPath.c_str()) || rawObjData.shapes.size() == 0) {
#if __UTILITY_LOG_LOADING_TIME
        std::cerr << "Failed to load object with path '" << assetPath << "'. Error message:" << std::endl << err << std::endl;
#endif
        //return nullptr;
    }
    
}
