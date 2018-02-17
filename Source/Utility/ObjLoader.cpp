

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



#include "External/tiny_obj_loader.h"
#include "Shape/VertexData.h"
#include "Shape/Mesh.h"

Shape * ObjLoader::loadObjFile(const std::string &path)
{
    std::string assetPath = AssetStore::resourceRoot + path;
#if __UTILITY_LOG_LOADING_TIME
	double logTimestamp = glfwGetTime();
	double took;
	std::cout << "Loading obj '" << assetPath << "'..." << std::endl;
#endif

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	Shape * result = new Shape();

	std::string err;
	if (!tinyobj::LoadObj(shapes, materials, err, assetPath.c_str()) || shapes.size() == 0) {
#if __UTILITY_LOG_LOADING_TIME
		std::cerr << "Failed to load object with path '" << assetPath << "'. Error message:" << std::endl << err << std::endl;
#endif
		return nullptr;
	}

#if __UTILITY_LOG_LOADING_TIME
	took = glfwGetTime() - logTimestamp;
	std::cout << std::setprecision(4) << " - Parsing '" << assetPath << "' took " << took << " seconds (by tinyobjloader)." << std::endl;
	logTimestamp = glfwGetTime();
#endif

	// Load all shapes.
	for (const auto & shape : shapes) {

		// Create a new mesh.
		Mesh newMesh;
		auto & vertexData = newMesh.vertexData;
		auto & indices = newMesh.indices;

		indices.reserve(shape.mesh.indices.size());

		// Push back all indices.
		for (const auto index : shape.mesh.indices) {
			indices.push_back(index);
		}

		vertexData.reserve(shape.mesh.positions.size());

		// Positions.
		for (unsigned int i = 0, j = 0; i < shape.mesh.positions.size(); i += 3, ++j) {
			if (j >= vertexData.size()) {
				vertexData.push_back(VertexData());
			}
			vertexData[j].position.x = shape.mesh.positions[i + 0];
			vertexData[j].position.y = shape.mesh.positions[i + 1];
			vertexData[j].position.z = shape.mesh.positions[i + 2];
		}

		// Normals.
		for (unsigned int i = 0, j = 0; i < shape.mesh.normals.size(); i += 3, ++j) {
			if (j >= vertexData.size()) {
				vertexData.push_back(VertexData());
			}
			vertexData[j].normal.x = shape.mesh.normals[i + 0];
			vertexData[j].normal.y = shape.mesh.normals[i + 1];
			vertexData[j].normal.z = shape.mesh.normals[i + 2];
		}

		// Texture coordinates.
		for (unsigned int i = 0, j = 0; i < shape.mesh.texcoords.size(); i += 2, ++j) {
			if (j >= vertexData.size()) {
				vertexData.push_back(VertexData());
			}
			vertexData[j].texCoord.x = shape.mesh.texcoords[i + 0];
			vertexData[j].texCoord.y = shape.mesh.texcoords[i + 1];
		}

		result->meshes.push_back(newMesh);
	}

#if __UTILITY_LOG_LOADING_TIME
	took = glfwGetTime() - logTimestamp;
	std::cout << std::setprecision(4) << " - Loading '" << assetPath << "' took " << took << " seconds." << std::endl;
#endif
	return result;
}
