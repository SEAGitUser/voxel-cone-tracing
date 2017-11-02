#pragma once

#include "OpenGL_Includes.h"

#include "Shape/Transform.h"
#include "Graphic/Material/MaterialSetting.h"


#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

#include <string>

class Mesh;

/// <summary> A renderer that can be used to render a mesh. </summary>
class MeshRenderer {
public:
	bool enabled = true;
	bool tweakable = false; // Automatically adds a window for this mesh renderer.
	std::string name = "Mesh renderer"; // Is displayed in the tweak bar.

	Transform transform;
	Mesh * mesh;

	// Constr/destr.
	MeshRenderer(Mesh *, MaterialSetting * = nullptr);
	~MeshRenderer();

	// Rendering.
	MaterialSetting * materialSetting = nullptr;
	void render(const GLuint program);
private:
	void setupMeshRenderer();
	void reuploadIndexDataToGPU();
	void reuploadVertexDataToGPU();
};
