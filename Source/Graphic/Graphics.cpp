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
#include "Time/Time.h"
#include "Shape/Mesh.h"
#include "Shape/StandardShapes.h"
#include "Renderer/MeshRenderer.h"
#include "Utility/ObjLoader.h"
#include "Shape/Shape.h"
#include "Application.h"
#include "Graphic/Graphics.h"
#include "Graphic/Material/Voxelization/VoxelizationConeTracingMaterial.h"
#include "Graphic/Material/Voxelization/VoxelizationMaterial.h"
#include "Graphic/Material/WorldPositionMaterial.h"
#include "Graphic/Material/Voxelization/VoxelVisualizationMaterial.h"
#include "Graphic/RenderTarget/VoxelizeRenderTarget.h"
#include "Graphic/FBO/FBO_2D.h"
#include "Graphic/FBO/FBO_3D.h"
#include "Texture3D.h"

// ----------------------
// Rendering pipeline.
// ----------------------
void Graphics::init(unsigned int viewportWidth, unsigned int viewportHeight)
{
    voxelFBO = nullptr;
    vvfbo1 = nullptr;
    vvfbo2 = nullptr;
    quadMeshRenderer= nullptr;
    cubeMeshRenderer = nullptr;
    cubeShape = nullptr;

    voxelFBO = new FBO_3D(VoxelizationMaterial::voxelTextureSize, VoxelizationMaterial::voxelTextureSize, VoxelizationMaterial::voxelTextureSize,
                          GL_NEAREST,GL_NEAREST, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_RGBA32F);
    
    glError();
    //TODO:: we can likely get rid of this vector
    const std::vector<GLfloat> initTextureBuffer = std::vector<GLfloat>(4 * VoxelizationMaterial::voxelTextureSize *
                                                                                              VoxelizationMaterial::voxelTextureSize * VoxelizationMaterial::voxelTextureSize, 0.0f);
    
    voxelTexture = new Texture3D(initTextureBuffer, VoxelizationMaterial::voxelTextureSize, VoxelizationMaterial::voxelTextureSize,
                                 VoxelizationMaterial::voxelTextureSize, GL_TRUE, GL_RGBA32F);
    voxelTexture->SaveTextureState(GL_FALSE, GL_FALSE);
    
    voxelizeRenderTarget = new VoxelizeRenderTarget(voxelTexture);

}

void Graphics::render(Scene & renderingScene, unsigned int viewportWidth, unsigned int viewportHeight, RenderingMode renderingMode)
{
    glError();
    voxelizeRenderTarget->SaveRenderState();
    voxelizeRenderTarget->Render(renderingScene);
    voxelizeRenderTarget->RestoreRenderState();

	// Render.
	switch (renderingMode) {
	case RenderingMode::VOXELIZATION_VISUALIZATION:
		//renderVoxelVisualization(renderingScene, viewportWidth, viewportHeight);
		break;
	case RenderingMode::VOXEL_CONE_TRACING:
		renderScene(renderingScene, viewportWidth, viewportHeight);
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
	//glUseProgram(program);

    GLint frameBufferWidth, frameBufferHeight;
    GLFWwindow * window = Application::getInstance().currentWindow;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	// GL Settings.
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Render.
	renderQueue(renderingScene, true);
}


void Graphics::renderQueue(Scene& renderingScene, bool uploadMaterialSettings) const
{
    RenderingQueue &renderingQueue = renderingScene.renderers;
    
	for (unsigned int i = 0; i < renderingQueue.size(); ++i)
    {
        if (renderingQueue[i]->enabled)
        {
            renderingQueue[i]->render(renderingScene);
        }
    }
}

// ----------------------
// Voxelization visualization.
// ----------------------
void Graphics::initVoxelVisualization(unsigned int viewportWidth, unsigned int viewportHeight)
{
	// Materials.
    worldPositionMaterial = static_cast<Material*>( MaterialStore::getInstance().getMaterial("world-position"));
    voxelVisualizationMaterial = static_cast<VoxelVisualizationMaterial*>( MaterialStore::getInstance().getMaterial("voxel-visualization"));

	assert(worldPositionMaterial != nullptr);
	assert(voxelVisualizationMaterial != nullptr);

	// FBOs.
	vvfbo1 = new FBO_2D(viewportWidth, viewportHeight, GL_NEAREST, GL_NEAREST);
	vvfbo2 = new FBO_2D(viewportWidth, viewportHeight, GL_NEAREST, GL_NEAREST);

	// Rendering cube.
	cubeShape = ObjLoader::loadObjFile("/Assets/Models/cube.obj");
	assert(cubeShape->meshes.size() == 1);
	cubeMeshRenderer = new MeshRenderer(&cubeShape->meshes[0]);

	// Rendering quad.
	quad = StandardShapes::createQuad();
	quadMeshRenderer = new MeshRenderer(&quad);
}
/*
 
 FOLLOWING CODE IS COMMENTED OUT BECAUSE IS CURRENTLY NOT WORKING, WILL FIX IN A LATER SUBMISSION
void Graphics::renderVoxelVisualization(Scene & renderingScene, unsigned int viewportWidth, unsigned int viewportHeight)
{
    Camera & camera = *renderingScene.renderingCamera;
    auto program = worldPositionMaterial->ProgramID();
    
    worldPositionMaterial->Activate();
    
    glError();
	// -------------------------------------------------------
	// Render cube to FBOs.
	// -------------------------------------------------------

	//glUseProgram(program);
	uploadCamera(camera, program);

	// Settings.
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// Back.
	glCullFace(GL_FRONT);
	glBindFramebuffer(GL_FRAMEBUFFER, vvfbo1->getFrameBufferID());
	glViewport(0, 0, vvfbo1->getWidth(), vvfbo1->getHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cubeMeshRenderer->render(program);

    glError();
	// Front.
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, vvfbo2->getFrameBufferID());
	glViewport(0, 0, vvfbo2->getWidth(), vvfbo2->getHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cubeMeshRenderer->render(program);
    
    glError();

	// -------------------------------------------------------
	// Render 3D texture to screen.
	// -------------------------------------------------------
	program = voxelVisualizationMaterial->ProgramID();
    voxelVisualizationMaterial->Activate();
    
	uploadCamera(camera, program);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Settings.
    //following function doesn't do anything
	uploadGlobalConstants(voxelVisualizationMaterial->ProgramID(), viewportWidth, viewportHeight);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Activate textures.
    GLint textureBack = vvfbo1->getColorBufferTextureName();
    GLint textureFront = vvfbo1->getColorBufferTextureName();
    
    voxelVisualizationMaterial->ActivateTexture2D("textureBack", textureBack, 0);
    voxelVisualizationMaterial->ActivateTexture2D("textureFront", textureFront, 1);
    
    Texture3D* voxelTexture = voxelizationMaterial->GetVoxelTexture();
    voxelVisualizationMaterial->ActivateTexture3D("texture3D", voxelTexture, 2);

	// Render.
	glViewport(0, 0, viewportWidth, viewportHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	quadMeshRenderer->render(program);
    
    glError();
}
*/
Graphics::~Graphics()
{
	delete vvfbo1;
	delete vvfbo2;
    delete voxelFBO;
	delete quadMeshRenderer;
	delete cubeMeshRenderer;
	delete cubeShape;

    delete voxelConeTracingMaterial;
    delete voxelizationMaterial;
    delete worldPositionMaterial;
    delete voxelTexture;
    delete voxelizeRenderTarget;
}
