//
//  VoxelizeRenderTarget.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 12/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "VoxelizeRT.h"
#include "Graphic/Renderer/MeshRenderer.h"
#include "Graphic/Material/Texture/Texture3D.h"
#include "Graphic/FBO/FBO_3D.h"
#include "Graphic/Camera/Camera.h"
#include "Graphic/Material/Voxelization/VoxelizationMaterial.h"

VoxelizeRT::VoxelizeRT( )
{
    Texture::Dimensions dimensions;
    dimensions.width = dimensions.height = dimensions.depth = VoxelizationMaterial::VOXEL_CUBE_SCALE;
    
    //properties will initialize to default values automatically
    Texture::Properties properties;
    
    properties.minFilter = GL_NEAREST;
    properties.magFilter = GL_NEAREST;
    
    fbo = new FBO_3D(dimensions, properties);
    
    worldToUnitCubeNormTex = glm::mat4(1.0f);
}

void VoxelizeRT::createUnitCubeTransform(Camera& camera, glm::mat4 &worldToUnitCube)
{
    glm::vec4 viewVector = glm::vec4(camera.forward, 0);
    
    glm::mat4 unitCubeToWorld = glm::scale(glm::mat4(1.0f), glm::vec3(VoxelizationMaterial::VOXEL_CUBE_SCALE));
    
    const GLfloat offsetScale = -.25f;
    glm::vec4 originalOffset = viewVector * offsetScale;
    glm::vec4 scaledOffset = unitCubeToWorld * originalOffset;
    
    FBO_3D* fbo3D = static_cast<FBO_3D*>(fbo);
    
    //think of the texture texels as cubic world units of measurment, the size of a cellsize wraps in it unit cubed
    //amount of world space, anything that falls within a cell will eventually contain ambient light from the world reaching that cell
    glm::vec3 cellSize( GLfloat(VoxelizationMaterial::VOXEL_CUBE_SCALE/fbo->getDimensions().width),
                       GLfloat(VoxelizationMaterial::VOXEL_CUBE_SCALE/fbo->getDimensions().height),
                       GLfloat(VoxelizationMaterial::VOXEL_CUBE_SCALE / fbo->getDimensions().depth) );
    
    float xTranslate = floorf( ( scaledOffset.x) /cellSize.x) * cellSize.x;
    float yTranslate = floorf( ( scaledOffset.y) / cellSize.y) * cellSize.y;
    float zTranslate = floorf( ( scaledOffset.z) / cellSize.z ) * cellSize.z;
    
    glm::vec3 translate = glm::vec3(xTranslate, yTranslate, zTranslate);
    unitCubeToWorld = glm::translate(glm::mat4(1.0f), translate) * unitCubeToWorld;
    
    worldToUnitCube = glm::inverse(unitCubeToWorld);
    
    float displace = .50f;
    worldToUnitCube = glm::translate(glm::mat4(1.0f), glm::vec3(displace, displace, displace))  * worldToUnitCube;
    
}

void VoxelizeRT::voxelize(Scene& renderScene, glm::mat4 &worldToUnitCube)
{
    RenderingQueue& renderingQueue = renderScene.renderers;
    for (GLuint i = 0; i < renderingQueue.size(); ++i)
    {
        renderingQueue[i]->transform.updateTransformMatrix();
        if (renderingQueue[i]->enabled)
        {
            renderingQueue[i]->voxelize(renderScene, worldToUnitCube);
        }
    }
}


void VoxelizeRT::Render(Scene& renderScene)
{
    
    fbo->ClearRenderTextures();
    fbo->Activate();
    {
        fbo->colorMaskOn(GL_TRUE);

        fbo->activateCulling(GL_FALSE);
        
        glError();
        
        glm::mat4 worldToUnitCube;
        createUnitCubeTransform(*renderScene.renderingCamera, worldToUnitCube);
        
        voxelize(renderScene, worldToUnitCube);
        
        GLuint textureIndex = 0;
        Texture3D* voxelTexture = static_cast<Texture3D*>(fbo->getRenderTexture(textureIndex));
        
        voxelTexture->generateMipMap();
//TODO: optimization oportunity
//        if (automaticallyRegenerateMipmap || regenerateMipmapQueued) {
//            glGenerateMipmap(GL_TEXTURE_3D);
//            regenerateMipmapQueued = false;
//        }
        
        glError();
        ticksSinceLastVoxelization = 0;
        voxelizationQueued = false;

        glError();

        fbo->activateCulling(GL_TRUE);
    }
    
    glError();
    fbo->Deactivate();

    
}

VoxelizeRT::~VoxelizeRT()
{
    
}


