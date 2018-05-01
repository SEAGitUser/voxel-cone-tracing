#include "Graphics.h"

// Stdlib.
#include <queue>
#include <algorithm>
#include <vector>

// External.
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

// Internal.
#include "Material/Material.h"
#include "Camera/OrthographicCamera.h"
#include "Material/MaterialStore.h"
#include "Time/FrameRate.h"
#include "Shape/Mesh.h"
#include "Utility/ObjLoader.h"
#include "Shape/Shape.h"
#include "Application.h"
#include "Graphic/Graphics.h"
#include "Graphic/Material/Voxelization/VoxelizationConeTracingMaterial.h"
#include "Graphic/Material/Voxelization/VoxelizationMaterial.h"
#include "Graphic/Material/Voxelization/VoxelVisualizationMaterial.h"
#include "Graphic/RenderTarget/VoxelizeRT.h"
#include "Graphic/RenderTarget/VoxelVisualizationRT.h"
#include "Graphic/RenderTarget/VoxelConeTracingRT.h"
#include "Graphic/FBO/FBO_2D.h"
#include "Graphic/FBO/FBO_3D.h"
#include "Texture3D.h"

// ----------------------
// Rendering pipeline.
// ----------------------
void Graphics::init(GLuint viewportWidth, GLuint viewportHeight)
{
    cubeShape = nullptr;
    
    GLfloat const worldCubeDimensions = 10.0f;
    voxelizeRenderTarget = new VoxelizeRT(worldCubeDimensions, worldCubeDimensions,worldCubeDimensions);

    Texture3D* albedoVoxels = static_cast<Texture3D*>(voxelizeRenderTarget->getFBO()->getRenderTexture(0));
    Texture3D* normalVoxels = static_cast<Texture3D*>(voxelizeRenderTarget->getFBO()->getRenderTexture(1));
    
    //std::shared_ptr<Texture3D> mipMap = voxelizeRenderTarget->getNormalMipMapLevel(5);
    voxVisualizationRT = new VoxelVisualizationRT(albedoVoxels);
    
    glm::mat4 voxViewProj = voxelizeRenderTarget->getVoxViewProjection();
    voxConeTracingRT = new VoxelConeTracingRT(albedoVoxels, normalVoxels, voxelizeRenderTarget->getAlbedoMipMaps(), voxelizeRenderTarget->getNormalMipMaps(),
                                              voxViewProj);
}

void Graphics::render(Scene & renderingScene, unsigned int viewportWidth, unsigned int viewportHeight, RenderingMode renderingMode)
{
    voxelizeRenderTarget->Render(renderingScene);

    switch (renderingMode) {
    case RenderingMode::VOXELIZATION_VISUALIZATION:
        voxVisualizationRT->Render(renderingScene);
        break;
    case RenderingMode::VOXEL_CONE_TRACING:
        voxConeTracingRT->Render(renderingScene);
        break;
    case RenderingMode::ORTHOGRAPHIC_DEPTH_BUFFER_LAYER_0:
        voxelizeRenderTarget->presentOrthographicDepth(renderingScene, 0);
        break;
    case RenderingMode::ORTHOGRAPHIC_DEPTH_BUFFER_LAYER_1:
        voxelizeRenderTarget->presentOrthographicDepth(renderingScene, 1);
        break;
    case RenderingMode::ORTHOGRAPHIC_DEPTH_BUFFER_LAYER_2:
        voxelizeRenderTarget->presentOrthographicDepth(renderingScene, 2);
        break;
    case RenderingMode::ORTHOGRAPHIC_DEPTH_BUFFER_LAYER_3:
        voxelizeRenderTarget->presentOrthographicDepth(renderingScene, 3);
        break;
    }
    
}


Graphics::~Graphics()
{
	delete cubeShape;

    delete voxelConeTracingMaterial;
    delete voxelizationMaterial;
    delete worldPositionMaterial;
    delete voxelTexture;
    delete voxelizeRenderTarget;
    delete voxVisualizationRT;
    delete voxConeTracingRT;
}
