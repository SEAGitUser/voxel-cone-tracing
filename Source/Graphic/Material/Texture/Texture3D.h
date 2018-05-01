#pragma once


#include <vector>

#include "OpenGL_Includes.h"
#include "Texture.h"
#include "SOIL/SOIL.h"

/// <summary> A 3D texture wrapper class. Handles important OpenGL calls. </summary>
class Texture3D : public Texture{
public:

    class Commands: public Texture::Commands
    {
    public:
        explicit Commands(Texture3D* texture);
        void setWrapMode(unsigned int mode) override;
        void setMinFiltering(unsigned int filter) override;
        void setMagFiltering(unsigned int filter) override;
        void enableMipMaps() override;
        void generateMipmaps() override;
        void allocateOnGPU() override;
        void end() override;
        ~Commands();
    private:
        
        Commands(Commands& rhs);
        
#ifdef __APPLE__
        
        //these functions are here because Apple supports up to OpenGL 4.1, so I will implement them myself in hopes one day
        //I won't need this.  These implementations are very specific to how it's called in this class, do not use it anywhere else
        
        void glTexStorage3D(    GLenum target,
                                GLsizei levels,
                                GLenum internalformat);
        
        
        virtual void glClearTexImage(    unsigned int texture,
                             unsigned int levels,
                             GLenum format,
                             GLenum type,
                             const void * data) override;
        
#endif
        Texture3D* texture;
    };
    
    
	Texture3D(
		const std::vector<float> & textureBuffer,
		const unsigned int width, const unsigned int height, const unsigned int depth,
		const bool generateMipmaps = GL_TRUE,
        unsigned int internalFormat = GL_RGBA32F
	);
    
    Texture3D();
    ~Texture3D();
    
    inline void SetInternalFormat(unsigned int value){ internalFormat = value; }
    inline void SetDepth(unsigned int value){ depth = value; }
    
    virtual void SaveTextureState(bool generateMipmaps = false, bool loadTexture = GL_FALSE) override;
    
    
private:

    
    unsigned int depth;
    unsigned int internalFormat;
    //TODO: THIS ARRAY IS USED TO CLEAR MEMORY, WILL NOT BE NEEDED IN OPENGL 4.4 AND ABOVE
    //ANOTHER WAY TO DO THIS IN 4.1 IS TO USE OPENCL.  I WILL IMPLEMENT THIS IN A MEMORY PASS.
    
#if __APPLE__
	static std::vector<float> clearData;
#endif
};
