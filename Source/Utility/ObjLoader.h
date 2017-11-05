#pragma once
#include "Shape/Shape.h"
#include "Utility/AssetStore.h"

#include <string>
class ObjLoader : public AssetStore{
public:
	/// <summary> Loads an .obj-file into a Shape object. </summary>
	static Shape * loadObjFile(const std::string &path = "Assets\\Models\\teapot.obj");
};
