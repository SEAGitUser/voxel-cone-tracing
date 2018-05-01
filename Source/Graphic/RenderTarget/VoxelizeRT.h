//
//  VoxelizeRenderTarget.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 12/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#pragma once

#include "RenderTarget.h"
#include "Graphic/Camera/OrthographicCamera.h"
#include "Graphic/Material/ShaderParameter.h"
#include "Graphic/Material/Voxelization/VoxelizationMaterial.h"
#include "Texture2D.h"
#include "ScreenQuad.h"
#include <array>
#include "ComputeShader.h"

class OrthographicCamera;
class Material;
class Points;
class VoxelizationMaterial;
class FBO_3D;
class Texture3D;

class VoxelizeRT : public RenderTarget
{
public:
    VoxelizeRT(GLfloat worldSpaceWidth, GLfloat worldSpaceHeight, GLfloat worldSpaceDepth );
    void presentOrthographicDepth( Scene& scene, int layer);
    
    virtual void Render( Scene& scene ) override;
    virtual ~VoxelizeRT();
    
    inline std::shared_ptr<FBO_3D> getFBO(){ return voxelFBO;};
    inline glm::mat4 getVoxViewProjection(){ return voxViewProjection; }
    inline std::shared_ptr<Texture3D> getAlbedoMipMapLevel( int index) { assert(index < albedoMipMaps.size()); return albedoMipMaps[index];}
    inline std::shared_ptr<Texture3D> getNormalMipMapLevel( int index ){ assert(index < normalMipMaps.size()); return normalMipMaps[index];}
    
    std::vector<std::shared_ptr<Texture3D>>& getNormalMipMaps(){ return normalMipMaps; }
    std::vector<std::shared_ptr<Texture3D>>& getAlbedoMipMaps(){ return albedoMipMaps; }
    
    static const float VOXELS_WORLD_SCALE;
    
private:
    void fillUpVoxelTexture( Scene& renderScene);
    void voxelize(Scene& renderScene);
    void generateDepthPeelingMaps(Scene& renderScene);
    void initDepthBuffer(int index, Texture::Dimensions &dimensions, Texture::Properties& properties);
    void generateMipMaps();
    void initMipMaps(Texture::Properties& properties);
    void initDepthPeelingBuffers(Texture::Dimensions& dimensions, Texture::Properties& properties);
    
private:
    bool automaticallyRegenerateMipmap = true;
    bool regenerateMipmapQueued = true;
    bool automaticallyVoxelize = true;
    bool voxelizationQueued = true;
    int voxelizationSparsity = 1; // Number of ticks between mipmap generation.
    int ticksSinceLastVoxelization = voxelizationSparsity;
    
    
    //state variables we will be modifying
    int colorMask[4];
    bool cullFace;
    bool depthTest;
    bool blend;
    int frameBuffer;

    std::shared_ptr<Material> positionsMaterial = nullptr;
    std::shared_ptr<Points> points = nullptr;
    std::shared_ptr<VoxelizationMaterial> voxMaterial = nullptr;
    std::shared_ptr<Material> textureDisplayMat = nullptr;
    std::shared_ptr<Material> depthPeelingMat = nullptr;
    
    OrthographicCamera orthoCamera;
    ScreenQuand screenQuad;
    std::shared_ptr<FBO_3D> voxelFBO;
    glm::mat4 voxViewProjection;
    ComputeShader downSample;
    
    
    std::vector< std::shared_ptr<Texture3D> > albedoMipMaps;
    std::vector< std::shared_ptr<Texture3D> > normalMipMaps;
    
    std::array<std::shared_ptr<FBO_2D>, 5> depthFBOs {nullptr, nullptr, nullptr, nullptr};
};
