#pragma once
#include "Shape/Shape.h"
#include "Utility/AssetStore.h"

#include <string>
class ObjLoader : public AssetStore{
public:
	/// <summary> Loads an .obj-file into a Shape object. </summary>
	static Shape * loadShapeFromObj(const std::string &path = "Assets\\Models\\teapot.obj");

    
    struct RawObjData
    {
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
    };
    
    static void loadRawObjData(const std::string &path, RawObjData& rawObjData);
};
