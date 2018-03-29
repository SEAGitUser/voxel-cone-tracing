#pragma once

#include <vector>

#include "Transform.h"
#include "Utility/External/tiny_obj_loader.h"
#include "Graphic/Material/Material.h"
#include "Shape/Transform.h"

class Scene;
class Material;
class Mesh;

class Shape
{
public:
	std::vector<Mesh*> meshes;
    
public:
    void render(Scene& scene, ShaderParameter::ShaderParamsGroup& group, Material* material);
    void render(Scene& scene);
    void render();
    
    Shape(std::vector<tinyobj::shape_t>& shapes);
	Shape() {}
    ~Shape();
    
public:
    Transform transform;
};
