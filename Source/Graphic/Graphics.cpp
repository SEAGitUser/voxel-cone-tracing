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
#include "Graphic/Material/VoxelizationConeTracingMaterial.h"
#include "Graphic/Material/VoxelizationMaterial.h"
#include "Graphic/Material/WorldPositionMaterial.h"
#include "Graphic/Material/VoxelVisualizationMaterial.h"
#include "Graphic/FBO/FBO_2D.h"

// ----------------------
// Rendering pipeline.
// ----------------------
void Graphics::init(unsigned int viewportWidth, unsigned int viewportHeight)
{
    
	glEnable(GL_MULTISAMPLE); // MSAA. Set MSAA level using GLFW (see Application.cpp).
    voxelConeTracingMaterial = new VoxelizationConeTracingMaterial("voxelization_cone_tracing");
    glError();
	//voxelCamera = OrthographicCamera(viewportWidth / float(viewportHeight));
	initVoxelization();
	initVoxelVisualization(viewportWidth, viewportHeight);
}

void Graphics::render(Scene & renderingScene, unsigned int viewportWidth, unsigned int viewportHeight, RenderingMode renderingMode)
{
	// Voxelize.
	bool voxelizeNow = voxelizationQueued || (automaticallyVoxelize && voxelizationSparsity > 0 && ++ticksSinceLastVoxelization >= voxelizationSparsity);
	if (voxelizeNow) {
		voxelize(renderingScene, true);
		ticksSinceLastVoxelization = 0;
		voxelizationQueued = false;
	}

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
	const Material * material = voxelConeTracingMaterial;
	const GLuint program = material->ProgramID();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(program);

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

	// Upload uniforms.
	uploadCamera(camera, program);
	uploadGlobalConstants(program, viewportWidth, viewportHeight);
	uploadLighting(renderingScene, program);
	uploadRenderingSettings(program);

	// Render.
	renderQueue(renderingScene.renderers, material->ProgramID(), true);
}

void Graphics::uploadLighting(Scene & renderingScene, const GLuint program) const
{
	// Point lights.
	for (unsigned int i = 0; i < renderingScene.pointLights.size(); ++i) renderingScene.pointLights[i].Upload(program, i);

	// Number of point lights.
	glUniform1i(glGetUniformLocation(program, NUMBER_OF_LIGHTS_NAME), renderingScene.pointLights.size());
}

void Graphics::uploadRenderingSettings(const GLuint glProgram) const
{
	glUniform1i(glGetUniformLocation(glProgram, "settings.shadows"), shadows);
	glUniform1i(glGetUniformLocation(glProgram, "settings.indirectDiffuseLight"), indirectDiffuseLight);
	glUniform1i(glGetUniformLocation(glProgram, "settings.indirectSpecularLight"), indirectSpecularLight);
	glUniform1i(glGetUniformLocation(glProgram, "settings.directLight"), directLight);
}

void Graphics::uploadGlobalConstants(const GLuint program, unsigned int viewportWidth, unsigned int viewportHeight) const
{
	//glUniform1i(glGetUniformLocation(program, APP_STATE_NAME), Application::getInstance().state);
	glm::vec2 screenSize(viewportWidth, viewportHeight);
}

void Graphics::uploadCamera(Camera & camera, const GLuint program)
{
	glUniformMatrix4fv(glGetUniformLocation(program, VIEW_MATRIX_NAME), 1, GL_FALSE, glm::value_ptr(camera.viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(program, PROJECTION_MATRIX_NAME), 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
	glUniform3fv(glGetUniformLocation(program, CAMERA_POSITION_NAME), 1, glm::value_ptr(camera.position));
}

void Graphics::renderQueue(RenderingQueue renderingQueue, const GLuint program, bool uploadMaterialSettings) const
{
	for (unsigned int i = 0; i < renderingQueue.size(); ++i) if (renderingQueue[i]->enabled)
		renderingQueue[i]->transform.updateTransformMatrix();

	for (unsigned int i = 0; i < renderingQueue.size(); ++i) if (renderingQueue[i]->enabled) {
		if (uploadMaterialSettings && renderingQueue[i]->materialSetting != nullptr) {
			renderingQueue[i]->materialSetting->Upload(program, false);
		}
		renderingQueue[i]->render(program);
	}
}

// ----------------------
// Voxelization.
// ----------------------
void Graphics::initVoxelization()
{
    voxelizationMaterial = new VoxelizationMaterial("voxelization");

	assert(voxelizationMaterial != nullptr);

	const std::vector<GLfloat> texture3D(4 * voxelTextureSize * voxelTextureSize * voxelTextureSize, 0.0f);
	voxelTexture = new Texture3D(texture3D, voxelTextureSize, voxelTextureSize, voxelTextureSize, true);
}

void Graphics::voxelize(Scene & renderingScene, bool clearVoxelization)
{
    //TODO: YOU'LL HAVE TO CHANGE THIS FUNCTION TO RENDER TO 3D TEXTURE
	if (clearVoxelization) {
		GLfloat clearColor[4] = { 0, 0, 0, 0 };
		voxelTexture->Clear(clearColor);
	}

	Material * material = voxelizationMaterial;

	glUseProgram(material->ProgramID());
    
    static const GLint defaultFrameBuffer = 0;
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFrameBuffer);

	// Settings.
	glViewport(0, 0, voxelTextureSize, voxelTextureSize);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	// Texture.
	voxelTexture->Activate(material->ProgramID(), "texture3D", 0);
    //TODO: YOU NEED TO ATTACH THIS TEXTURE TO THE SHADER!
	//glBindImageTexture(0, voxelTexture->textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);


	// Lighting.
	uploadLighting(renderingScene, material->ProgramID());

	// Render.
	renderQueue(renderingScene.renderers, material->ProgramID(), true);
	if (automaticallyRegenerateMipmap || regenerateMipmapQueued) {
		glGenerateMipmap(GL_TEXTURE_3D);
		regenerateMipmapQueued = false;
	}
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

// ----------------------
// Voxelization visualization.
// ----------------------
void Graphics::initVoxelVisualization(unsigned int viewportWidth, unsigned int viewportHeight)
{
	// Materials.
    worldPositionMaterial = new WorldPositionMaterial("world_position");
    voxelVisualizationMaterial = new VoxelVisualizationMaterial("voxel_visualization");

	assert(worldPositionMaterial != nullptr);
	assert(voxelVisualizationMaterial != nullptr);

	// FBOs.
	vvfbo1 = new FBO_2D(viewportHeight, viewportWidth);
	vvfbo2 = new FBO_2D(viewportHeight, viewportWidth);

	// Rendering cube.
	cubeShape = ObjLoader::loadObjFile("/Assets/Models/cube.obj");
	assert(cubeShape->meshes.size() == 1);
	cubeMeshRenderer = new MeshRenderer(&cubeShape->meshes[0]);

	// Rendering quad.
	quad = StandardShapes::createQuad();
	quadMeshRenderer = new MeshRenderer(&quad);
}

void Graphics::renderVoxelVisualization(Scene & renderingScene, unsigned int viewportWidth, unsigned int viewportHeight)
{
	// -------------------------------------------------------
	// Render cube to FBOs.
	// -------------------------------------------------------
	Camera & camera = *renderingScene.renderingCamera;
	auto program = worldPositionMaterial->ProgramID();
	glUseProgram(program);
	uploadCamera(camera, program);

	// Settings.
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// Back.
	glCullFace(GL_FRONT);
	glBindFramebuffer(GL_FRAMEBUFFER, vvfbo1->frameBuffer);
	glViewport(0, 0, vvfbo1->width, vvfbo1->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cubeMeshRenderer->render(program);

	// Front.
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, vvfbo2->frameBuffer);
	glViewport(0, 0, vvfbo2->width, vvfbo2->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cubeMeshRenderer->render(program);

	// -------------------------------------------------------
	// Render 3D texture to screen.
	// -------------------------------------------------------
	program = voxelVisualizationMaterial->ProgramID();
	glUseProgram(program);
	uploadCamera(camera, program);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Settings.
	uploadGlobalConstants(voxelVisualizationMaterial->ProgramID(), viewportWidth, viewportHeight);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Activate textures.
	vvfbo1->ActivateAsTexture(program, "textureBack", 0);
	vvfbo2->ActivateAsTexture(program, "textureFront", 1);
	voxelTexture->Activate(program, "texture3D", 2);

	// Render.
	glViewport(0, 0, viewportWidth, viewportHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	quadMeshRenderer->render(program);
}

Graphics::~Graphics()
{
	if (vvfbo1) delete vvfbo1;
	if (vvfbo2) delete vvfbo2;
	if (quadMeshRenderer) delete quadMeshRenderer;
	if (cubeMeshRenderer) delete cubeMeshRenderer;
	if (cubeShape) delete cubeShape;
	if (voxelTexture) delete voxelTexture;
    delete voxelConeTracingMaterial;
    delete voxelizationMaterial;
    delete worldPositionMaterial;
}
