#pragma once

#include <string>

#include "OpenGL_Includes.h"
#include "Texture.h"
#include "SOIL/SOIL.h"
#include "glm.hpp"

/// <summary> A 2D texture wrapper class. Handles important OpenGL calls. </summary>
class Texture2D : public Texture {
public:

    class Commands: public Texture::Commands
    {
    public:
        Commands(Texture2D* texture, GLuint textureUnit = 0);
        void setWrapMode(GLuint wrap) override;
        void setMinFiltering(GLuint filter) override;
        void setMagFiltering(GLuint filter) override;
        void generateMipmaps() override;
        void allocateOnGPU() override;
        void end() override;
        void clear(glm::vec4 clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        
        ~Commands();
        
    private:
        
#if __APPLE__
        void glClearTexImage(GLuint texture,
                        GLuint levels,
                        GLenum format,
                        GLenum type,
                        const void * data) override;
#endif
        
    private:
        Texture2D* texture;
    };

    Texture2D(bool dummyTexture = false);
	Texture2D(const std::string shaderTextureSamplerName, const std::string &path,
              const bool generateMipmaps = true, const int force_channels = SOIL_LOAD_RGB);
	
    
    ~Texture2D();
    
    ///<summary> This saves the texture state in the GPU using the platform's graphics library based on arguments passed in setter functions and constructor arguments </summary>
    void SaveTextureState(GLboolean generateMipmaps = false, GLboolean loadTexture = GL_FALSE) override;
    
private:
    std::string shaderTextureSamplerName;
};
