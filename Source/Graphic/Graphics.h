#pragma once

#include "OpenGL_Includes.h"

#include <vector>



#include "Scene/Scene.h"
#include "Graphic/Camera/OrthographicCamera.h"
#include "Shape/Mesh.h"

class MeshRenderer;
class Material;
class Shape;
class VoxelizationConeTracingMaterial;
class VoxelizationMaterial;
class WorldPositionMaterial;
class VoxelVisualizationMaterial;
class FBO_2D;
class FBO_3D;
class Texture3D;
class VoxelizeRT;
class VoxelVisualizationRT;
class VoxelConeTracingRT;


/// <summary> A graphical context used for rendering. </summary>
class Graphics {
	using RenderingQueue = const std::vector<Mesh*> &;
public:
	enum class RenderingMode {
		VOXELIZATION_VISUALIZATION = 0,
		VOXEL_CONE_TRACING,
        ORTHOGRAPHIC_DEPTH_BUFFER_LAYER_0,
        ORTHOGRAPHIC_DEPTH_BUFFER_LAYER_1,
        ORTHOGRAPHIC_DEPTH_BUFFER_LAYER_2,
        ORTHOGRAPHIC_DEPTH_BUFFER_LAYER_3,
        RENDER_MODE_TOTAL
	};

	/// <summary> Initializes rendering. </summary>
	virtual void init(GLuint viewportWidth, GLuint viewportHeight); // Called pre-render once per run.

	/// <sumamry> Renders a scene using a given rendering mode. </summary>
	virtual void render(
		Scene & renderingScene, unsigned int viewportWidth,
		unsigned int viewportHeight, RenderingMode renderingMode = RenderingMode::VOXEL_CONE_TRACING
	);
    
	~Graphics();
private:

	// ----------------
	// Voxel cone tracing.
	// ----------------
    VoxelizationConeTracingMaterial * voxelConeTracingMaterial = nullptr;
    VoxelizationMaterial * voxelizationMaterial = nullptr;
    
    //TODO: turn these into smart pointers

    Material * worldPositionMaterial;
    VoxelVisualizationMaterial * voxelVisualizationMaterial;
	Mesh quad;

	Shape * cubeShape;
    Texture3D* voxelTexture;
    
    VoxelizeRT* voxelizeRenderTarget = nullptr;
    VoxelVisualizationRT* voxVisualizationRT = nullptr;
    VoxelConeTracingRT* voxConeTracingRT = nullptr;
};
