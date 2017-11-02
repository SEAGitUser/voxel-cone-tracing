#pragma once

#include <string>

#include "OpenGL_Includes.h"

#include "SOIL/SOIL.h"

/// <summary> A 2D texture wrapper class. Handles important OpenGL calls. </summary>
class Texture2D {
public:
	std::string shaderTextureSamplerName;
	GLuint textureID;

	/// <summary> Activates this texture and passes it on to a texture unit on the GPU. </summary>
	void Activate(int shaderProgram, int textureUnit = 0);

	Texture2D(const std::string shaderTextureSamplerName, const std::string path, const bool generateMipmaps = true, const int force_channels = SOIL_LOAD_RGB);
	~Texture2D();
private:
	int width, height, channels;
};
