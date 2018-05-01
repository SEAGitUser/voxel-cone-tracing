

#include "FBO.h"
#include "Texture.h"
#include "OpenGL_Includes.h"


int FBO::setupRenderTarget(Texture* target)
{
    target->SetWrap(textureProperties.wrap);
    
    target->SetWidth(dimensions.width);
    target->SetHeight(dimensions.height);
    target->SetMinFilter(textureProperties.minFilter);
    target->SetMagFilter(textureProperties.magFilter);
    target->SetPixelFormat(textureProperties.pixelFormat);
    target->SetDataType(textureProperties.dataFormat);
    
    target->SaveTextureState();
    
    return target->GetTextureID();
}

FBO::~FBO()
{
    for(Texture* texture: renderTextures)
    {
        delete texture;
    }
}

///////////////////////////////////////////////////FBO::Commands

FBO* FBO::Commands::fbo = nullptr;

FBO::Commands::Commands(FBO* _fbo):
previousViewportWidth(0),
previousViewportHeight(0),
previousFBO(0)
{
    init(_fbo);
}


FBO::Commands::Commands():
previousViewportWidth(0),
previousViewportHeight(0),
previousFBO(0)
{
    //glBindBuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Commands::init(FBO* _fbo)
{
    assert(fbo == nullptr && "You must call FBO::Commands::end() to indicate end of frame rendering before starting a new frame commands");
    fbo = _fbo;
    glBindFramebuffer(GL_FRAMEBUFFER, fbo->frameBuffer);

    unsigned int colorAttachment[50];

    if(fbo->renderTextures.size() > 0)
    {
        for(int i = 0; i < fbo->renderTextures.size(); ++i)
        {
            colorAttachment[i] = GL_COLOR_ATTACHMENT0 + i;
        }

        glDrawBuffers(fbo->renderTextures.size(), colorAttachment);
    }

    getPreviousViewportSize();

    fbo->dimensions.width = fbo->dimensions.width == 0 ? previousViewportWidth : fbo->dimensions.width;
    fbo->dimensions.height= fbo->dimensions.height == 0 ? previousViewportHeight : fbo->dimensions.height;

    setViewport(fbo->dimensions.width, fbo->dimensions.height);
}
void FBO::Commands::setViewport(int width, int height)
{
     glViewport(0, 0, width, height);
    
}

void FBO::Commands::getPreviousViewportSize()
{
    int dims[4];
    glGetIntegerv(GL_VIEWPORT, dims);
    previousViewportWidth = dims[2];
    previousViewportHeight = dims[3];
}

void FBO::Commands::colorMask(bool _value)
{
    GLboolean value = _value ? GL_TRUE : GL_FALSE;
    glColorMask(value, value, value, value);
}

void FBO::Commands::depthMask(bool _value)
{
    GLboolean value = _value ? GL_TRUE : GL_FALSE;
    glDepthMask(value);
}

void FBO::Commands::activateCulling(bool value)
{
    value ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
}

void FBO::Commands::enableDepthTest(bool value)
{
    value ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void FBO::Commands::enableBlend(bool value)
{
    value ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}

void FBO::Commands::enableAdditiveBlending()
{
    enableBlend(true);
    glBlendFunc(GL_ONE, GL_ONE);
}

void FBO::Commands::backFaceCulling(bool _value)
{
    if(_value)
    {
        glEnable(GL_CULL_FACE); glCullFace(GL_BACK);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
}

void FBO::Commands::end()
{
    glBindFramebuffer(GL_FRAMEBUFFER, previousFBO);
    setViewport(previousViewportWidth, previousViewportHeight);
    fbo = nullptr;
}

void FBO::Commands::clearRenderTarget()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
}

void FBO::Commands::setClearColor(glm::vec4 color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void FBO::Commands::setDetphClearValue(float depth)
{
    glClearDepth(depth);
}

void FBO::Commands::blendSrcAlphaOneMinusSrcAlpha()
{
    enableBlend(true);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void FBO::Commands::setupTargetsForRendering(bool threeDimensions)
{
    unsigned int colorAttachment[50];
    
    unsigned int dimensions = threeDimensions ? GL_TEXTURE_3D :GL_TEXTURE_2D ;
    if(fbo->renderTextures.size() > 0)
    {
        int size = fbo->renderTextures.size();
        
        for(int i = 0; i < size; ++i)
        {
            colorAttachment[i] = GL_COLOR_ATTACHMENT0 + i;
            if(!threeDimensions)
            {
                glBindTexture(GL_TEXTURE_2D, fbo->renderTextures[i]->GetTextureID());
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (GLenum)(i), GL_TEXTURE_2D, fbo->renderTextures[i]->GetTextureID(), 0);
            }
            else
            {
                glBindTexture(GL_TEXTURE_3D, fbo->renderTextures[i]->GetTextureID());
                glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (GLenum)i, fbo->renderTextures[i]->GetTextureID(), 0);
            }
        }
        
        glError();
        glDrawBuffers(fbo->renderTextures.size(), colorAttachment);
    }
}

FBO::Commands::~Commands()
{
    end();
}






