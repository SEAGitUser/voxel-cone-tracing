#pragma once



class FBO {
    
public:
    
    FBO(GLuint w, GLuint h )
    {
        width = w;
        height = h;
    }
    
    
protected:
    static const GLint MAX_RENDER_TARGETS = 10;
    
    GLuint width, height, frameBuffer, textureColorBuffer, attachment, rbo;
};
