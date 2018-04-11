
// Author:    Rafael Sabino
// Date:    04/08/18

#version 410 core


struct PointLight {
    vec3 position;
    vec3 color;
};

// Basic material.
struct Material {
    vec3 diffuseColor;
    float diffuseReflectivity;
    vec3 specularColor;
    float specularDiffusion; // "Reflective and refractive" specular diffusion.
    float specularReflectivity;
    float emissivity; // Emissive materials uses diffuse color as emissive color.
    float refractiveIndex;
    float transparency;
};

struct Settings {
    bool indirectSpecularLight; // Whether indirect specular light should be rendered or not.
    bool indirectDiffuseLight; // Whether indirect diffuse light should be rendered or not.
    bool directLight; // Whether direct light should be rendered or not.
    bool shadows; // Whether shadows should be rendered or not.
};

//uniform Material material;
//uniform Settings settings;
//uniform PointLight pointLights[1];
//uniform int numberOfLights; // Number of lights currently uploaded.
//uniform vec3 cameraPosition; // World campera position.
//uniform int state; // Only used for testing / debugging.
uniform sampler3D albedoVoxels; // Voxelization texture.
uniform sampler3D normalVoxels;
uniform mat4 toVoxelSpace;
uniform mat4 voxViewProjection;

in vec3 voxViewPosFrag;
in vec3 normalFrag;


out vec4 color;


float ambientOcclusion()
{
    
    return 0;
}

void main()
{
    vec4 voxelSpacePos = voxViewProjection * vec4(voxViewPosFrag,1.0f);
    
    voxelSpacePos += 1.0f;
    voxelSpacePos *= .5f;
    
    vec4 albedo = texture(albedoVoxels, voxelSpacePos.xyz);
    color = vec4(albedo.xyz, 1.0f);
    
    
}
