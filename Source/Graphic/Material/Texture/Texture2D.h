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
        Commands(Texture2D* texture);
        void setWrapMode(unsigned int wrap) override;
        void setMinFiltering(unsigned int filter) override;
        void setMagFiltering(unsigned int filter) override;
        void generateMipmaps() override;
        void enableMipMaps() override;
        void allocateOnGPU() override;
        void end() override;
        void clear(glm::vec4 clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        
        ~Commands();
        
    private:
        
#if __APPLE__
        void glClearTexImage(unsigned int texture,
                        unsigned int levels,
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
    void SaveTextureState(bool generateMipmaps = false, bool loadTexture = GL_FALSE) override;
    
private:
    std::string shaderTextureSamplerName;
};
