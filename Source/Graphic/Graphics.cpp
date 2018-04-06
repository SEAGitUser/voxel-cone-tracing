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

    Texture3D* texture = static_cast<Texture3D*>(voxelizeRenderTarget->getFBO()->getRenderTexture(1) );
    voxVisualizationRT = new VoxelVisualizationRT(texture);
}

void Graphics::render(Scene & renderingScene, unsigned int viewportWidth, unsigned int viewportHeight, RenderingMode renderingMode)
{
    voxelizeRenderTarget->Render(renderingScene);

	switch (renderingMode) {
	case RenderingMode::VOXELIZATION_VISUALIZATION:
        voxVisualizationRT->Render(renderingScene);
		break;
	case RenderingMode::VOXEL_CONE_TRACING:
		renderScene(renderingScene, viewportWidth, viewportHeight);
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

// ----------------------
// Scene rendering.
// ----------------------
void Graphics::renderScene(Scene & renderingScene, unsigned int viewportWidth, unsigned int viewportHeight)
{
    
    
	// Fetch references.
	auto & camera = *renderingScene.renderingCamera;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    GLint frameBufferWidth, frameBufferHeight;
    GLFWwindow * window = Application::getInstance().currentWindow;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    
    FBO::Commands commands(FBO_2D::getDefault().get());
    
    commands.setViewport(frameBufferWidth, frameBufferHeight);
    commands.setClearColor();
    commands.clearRenderTarget();
    commands.enableDepthTest(true);
    commands.backFaceCulling(true);
    commands.blendSrcAlphaOneMinusSrcAlpha();
    
	renderQueue(renderingScene, true);
    
    commands.end();
    
}


void Graphics::renderQueue(Scene& renderingScene, bool uploadMaterialSettings) const
{
    RenderingQueue &renderingQueue = renderingScene.renderers;
    
    for(Shape* shape : renderingScene.shapes)
    {
        shape->render(renderingScene);
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
}
