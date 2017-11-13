#pragma once

#include <string>

#include "OpenGL_Includes.h"
#include "Texture.h"
#include "SOIL/SOIL.h"

/// <summary> A 2D texture wrapper class. Handles important OpenGL calls. </summary>
class Texture2D : public Texture {
public:

	/// <summary> Activates this texture and passes it on to a texture unit on the GPU. </summary>
	void Activate(int shaderProgram, int textureUnit = 0);

    Texture2D();
	Texture2D(const std::string shaderTextureSamplerName, const std::string &path,
              const bool generateMipmaps = true, const int force_channels = SOIL_LOAD_RGB);
	
    
    virtual void Clear() override;
    ~Texture2D();
    
    ///<summary> This saves the texture state in the GPU using the platform's graphics library based on arguments passed in setter functions and constructor arguments </summary>
    void SaveTextureState(GLboolean generateMipmaps = false, GLboolean loadTexture = GL_FALSE) override;
    

#if __APPLE__
    
    virtual void glClearTexImage(	GLuint texture,
                                 GLint level,
                                 GLenum format,
                                 GLenum type,
                                 const void * data) override;
    
#endif
    
private:
    std::string shaderTextureSamplerName;
};
