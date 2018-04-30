#include "Application.h"

// Standard library.
#include <iostream>
#include <iomanip>
#include <time.h>

// External.
#include "OpenGL_Includes.h"

// Internal.
#include "Scene/Scene.h"
#include "Scene/ScenePack.h"
#include "Graphic/Graphics.h"
#include "Graphic/Material/MaterialStore.h"
#include "Time/FrameRate.h"
#include "Shape/TextQuad.h"

#define __LOG_INTERVAL 1 /* How often we should log frame rate info to the console. = 0 means don't log. */
#if __LOG_INTERVAL > 0
constexpr float __LOG_INTERVAL_TIME_GUARD = 1.0f;
#endif

using __DEFAULT_LEVEL = GlassScene; // The scene that will be loaded on startup.
// (see ScenePack.h for more scenes)

Application & Application::getInstance() {
	static Application application;
	return application;
}

void Application::init() {
	std::cout << "Initialization started." << std::endl;

	// -------------------------------------
	// Initialize GLFW.
	// -------------------------------------
	if (!glfwInit()) {
		std::cerr << "GLFW failed to initialize." << std::endl;
	}
	double timeElapsed = glfwGetTime();

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#else
	// OpenGL version 4.5 with core profile.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
#endif
	glfwWindowHint(GLFW_SAMPLES, MSAA_SAMPLES);

	// Create a new window and set window mode accordingly (including title, size and mode).
#if DEFAULT_FULLSCREEN == 2 
	// Borderless fullscreen.
	SetBorderlessFullscreenMode();
#else
	SetWindowMode(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_FULLSCREEN == 1);
#endif
	std::cout << "[0] : GLFW initialized." << std::endl;

#ifndef __APPLE__
	// -------------------------------------
	// Initialize GLEW.
	// -------------------------------------
	glewExperimental = GL_TRUE;
	if (glewInit() == GLEW_OK) {
		std::cout << "[1] : GLEW initialized." << std::endl;
	}
	else {
		std::cerr << "GLEW failed to initialize (glewExperimental might not be supported)." << std::endl;
		std::cerr << "Press any key to exit ... " << std::endl;
		getchar();
		return;
	}

	std::vector<std::pair<GLuint, std::string>> requiredGLEWExtensions = {
		{ GLEW_ARB_shader_image_load_store,		"ARB_shader_image_load_store"},
		{ GLEW_VERSION_4_5,						"GLEW_VERSION_4_5 (OpenGL 4.5)"},
		{ GL_ARB_multisample,					"GLFW MSAA" }
	};

	for (const auto & ext : requiredGLEWExtensions) {
		if (!ext.first) {
			std::cerr << "ERROR: " << ext.second << " not supported! Expect unexpected behaviour." << std::endl;
			std::cerr << "Press any key to continue ... " << std::endl;
			getchar();
		}
	}
#endif
	// -------------------------------------
	// Initialize graphics.
	// -------------------------------------
	int w, h;
	glfwGetWindowSize(currentWindow, &w, &h);
	MaterialStore::getInstance(); // Initialize material store.
    glError();
    graphics.init(w, h);
    glError();
	glfwSetWindowSizeCallback(currentWindow, Application::OnWindowResize);
	glfwSwapInterval(DEFAULT_VSYNC); // vSync.
	std::cout << "[2] : Graphics initialized." << std::endl;

	// -------------------------------------
	// Initialize scene.
	// -------------------------------------
    scene = new __DEFAULT_LEVEL();
	scene->init(w, h);
	std::cout << "[3] : Scene initialized." << std::endl;


	// -------------------------------------
	// Initialize input.
	// -------------------------------------
	glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetMouseButtonCallback(currentWindow, GLFWMouseButtonCallback);
	glfwSetCursorPosCallback(currentWindow, GLFWMousePositionCallback);
	glfwSetKeyCallback(currentWindow, GLFWKeyCallback);
	std::cout << "[5] : Input initialized." << std::endl;

	// -------------------------------------
	// Finalize initialization.
	// -------------------------------------
	srand(0);
	FrameRate::time = 0;
	initialized = FrameRate::initialized = true;

	timeElapsed = glfwGetTime() - timeElapsed;
	std::cout << "Initialization finished (" << timeElapsed << " seconds)!" << std::endl;
	glfwSetTime(0);

	std::cout << "Using OpenGL version " << glGetString(GL_VERSION) << std::endl;
    
    glm::vec2 dimensions( w, h);
    text = new TextQuad(dimensions);
}



void Application::run()
{
	std::cout << "Application is now running.\n" << std::endl;
	std::cout << " :: Use R to switch between rendering modes.\n";
	// std::cout << " :: Use T to switch between interaction modes." << std::endl;

	double smoothedDeltaTimeAccumulator = 0;
#if __LOG_INTERVAL > 0
	double updateCost = 0;
	double renderCost = 0;
	double timestampCost = 0;
	double timestampLog = 0;
#endif

    std::string frameRate = "";
	// Start the update loop.
	while (!glfwWindowShouldClose(currentWindow) && !exitQueued)
	{
		// --------------------------------------------------
		// Update input and timers.
		// --------------------------------------------------
		UpdateGlobalInputParameters();

		// Update time vals.
		double currentTime = glfwGetTime();
		FrameRate::deltaTime = currentTime - FrameRate::time;
		FrameRate::time = currentTime;
		FrameRate::framesPerSecond = 1.0 / (FrameRate::deltaTime + 0.0000001);

		// --------------------------------------------------
		// Update world.
		// --------------------------------------------------
#if __LOG_INTERVAL > 0
		{
			timestampCost = glfwGetTime();
		}
#endif
		if (!paused) scene->update();
#if __LOG_INTERVAL > 0 
		{
			updateCost += glfwGetTime() - timestampCost;
		}
#endif

		// --------------------------------------------------
		// Rendering.
		// --------------------------------------------------
#if __LOG_INTERVAL > 0
		{
			timestampCost = glfwGetTime();
		}
#endif
		int viewportWidth, viewportHeight;
		glfwGetWindowSize(currentWindow, &viewportWidth, &viewportHeight);
		if (!paused) {
			graphics.render(*scene, viewportWidth, viewportHeight, currentRenderingMode);

		}


		// --------------------------------------------------
		// Swap buffers and update timers.
		// --------------------------------------------------
#if __LOG_INTERVAL > 0
		{
			renderCost += glfwGetTime() - timestampCost;
		}
#endif

        
        // Update frame count.
        FrameRate::frameCount++;
        smoothedDeltaTimeAccumulator += FrameRate::deltaTime;
        
        // Update smoothed delta time.
        if (FrameRate::smoothedDeltaTimeFrameCount > 0 && FrameRate::frameCount % FrameRate::smoothedDeltaTimeFrameCount == 0) {
            FrameRate::smoothedDeltaTime = smoothedDeltaTimeAccumulator / FrameRate::smoothedDeltaTimeFrameCount;
            smoothedDeltaTimeAccumulator = 0;
        }
        
        glError();
        
        char buf[100];
        float f = floorf(FrameRate::framesPerSecond * 100.0f)/100.0f;
        static std::string frameRate;
        
        std::sprintf(buf, "Frame Rate: %.2f", f);
        
        frameRate = buf;
        glm::vec2 pos(50.0f, 50.0f);
        text->setScale(.5f);
        text->print(frameRate, pos);
        
		// Swap front and back buffers.
		if (!paused)
            glfwSwapBuffers(currentWindow);

		// Poll for and process events.
		glfwPollEvents();
	}

	glfwDestroyWindow(currentWindow);
	glfwTerminate();
	std::cout << "Application has now terminated." << std::endl;
}

void Application::UpdateGlobalInputParameters() {
	if (glfwGetKey(currentWindow, GLFW_KEY_ESCAPE)) {
		exitQueued = true;
		return;
	}

	int button_state_x = glfwGetKey(currentWindow, GLFW_KEY_X);
	int button_state_z = glfwGetKey(currentWindow, GLFW_KEY_Z);

	if (button_state_x == GLFW_PRESS && button_state_x != previous_state_x) {
		std::cout << "Application state: " << ++state << std::endl;
	}
	if (button_state_z == GLFW_PRESS && button_state_z != previous_state_z) {
		std::cout << "Application state: " << --state << std::endl;
	}

	previous_state_x = button_state_x;
	previous_state_z = button_state_z;
}

void Application::SetBorderlessFullscreenMode() {
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	GLFWwindow* newWindow = glfwCreateWindow(mode->width, mode->height, DEFAULT_TITLE, monitor, NULL);
	if (newWindow) {
		currentWindow = newWindow;
		glfwMakeContextCurrent(currentWindow);
	}
	else {
		glfwTerminate();
		std::cerr << "GLFW failed setting borderless fullscreen window mode." << std::endl;
	}
}

void Application::SetWindowMode(int quadWidth, int quadHeight, bool fullscreen = true) {
	GLFWwindow* newWindow = glfwCreateWindow(quadWidth, quadHeight, DEFAULT_TITLE, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if (newWindow) {
		currentWindow = newWindow;
		glfwMakeContextCurrent(currentWindow);
	}
	else {
		glfwTerminate();
		std::cerr << "GLFW failed setting new window mode." << std::endl;
	}
}

Application::~Application() {
	delete scene;
    delete text;
}

Application::Application() : exitQueued(false) {

}

void Application::OnWindowResize(GLFWwindow* window, int quadWidth, int quadHeight)
{

}

void Application::GLFWMousePositionCallback(GLFWwindow * window, double x, double y)
{

}

void Application::GLFWMouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{

}

void Application::GLFWKeyCallback(GLFWwindow * window, int key, int scanmode, int action, int mods)
{
	auto & app = Application::getInstance();

    using GRM = Graphics::RenderingMode;

    
	// Button was pressed down this frame.
	if (action == 1) {
		// Change rendering mode.
		if (key == GLFW_KEY_R) {

            uint mode = (static_cast<int>(app.currentRenderingMode) + 1) % static_cast<int>(GRM::RENDER_MODE_TOTAL);
            app.currentRenderingMode = static_cast<GRM>(mode);
		}

		// Change viewing mode.
		if (key == GLFW_KEY_M) {
			if (app.currentInputState == InputState::FREE_LOOK) {
				glfwSetInputMode(app.currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				app.currentInputState = InputState::TWEAK_BAR;
			}
			else {
				app.currentInputState = InputState::FREE_LOOK;
				glfwSetInputMode(app.currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			// Update camera.
			int xwidth, yheight;
			glfwGetWindowSize(window, &xwidth, &yheight);
			glfwSetCursorPos(window, xwidth / 2, yheight / 2); // Reset mouse position for next update iteration.
		}

		// Pause / unpause.
		if (key == GLFW_KEY_P) {
			app.paused = !app.paused;
		}
	}
}
