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
#include "Shape/StandardShapes.h"
#include "Renderer/MeshRenderer.h"
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
    quadMeshRenderer= nullptr;
    cubeMeshRenderer = nullptr;
    cubeShape = nullptr;
    
    voxelizeRenderTarget = new VoxelizeRT();
    Texture3D* texture = static_cast<Texture3D*>(voxelizeRenderTarget->getFBO()->getRenderTexture(0) );
    voxVisualizationRT = new VoxelVisualizationRT(texture);

}

void Graphics::render(Scene & renderingScene, unsigned int viewportWidth, unsigned int viewportHeight, RenderingMode renderingMode)
{
    glError();

    voxelizeRenderTarget->Render(renderingScene);

	// Render.
	switch (renderingMode) {
	case RenderingMode::VOXELIZATION_VISUALIZATION:
		renderVoxelVisualization(renderingScene, viewportWidth, viewportHeight);
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
    
    GLint frameBufferWidth, frameBufferHeight;
    GLFWwindow * window = Application::getInstance().currentWindow;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	// GL Settings.
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0);
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
/*
 
 TODO: THIS WILL CODE IS HERE FOR REFERENCE ONLY, WILL BE DELETED ONCE NEW IMPLEMENTATION WORKS
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
*/


void Graphics::renderVoxelVisualization(Scene & renderingScene, unsigned int viewportWidth, unsigned int viewportHeight)
{
    
    voxVisualizationRT->Render(renderingScene);
    /*
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
     */
}

Graphics::~Graphics()
{
	delete quadMeshRenderer;
	delete cubeMeshRenderer;
	delete cubeShape;

    delete voxelConeTracingMaterial;
    delete voxelizationMaterial;
    delete worldPositionMaterial;
    delete voxelTexture;
    delete voxelizeRenderTarget;
    delete voxVisualizationRT;
}
