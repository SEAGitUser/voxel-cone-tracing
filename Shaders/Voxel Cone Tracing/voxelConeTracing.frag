
// Author:    Rafael Sabino
// Date:    04/08/18

#version 410 core

#define NUM_SAMPLING_RAYS 6
#define NUM_MIP_MAPS 7

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


uniform sampler3D normalMipMaps[NUM_MIP_MAPS];
uniform sampler3D albedoMipMaps[NUM_MIP_MAPS];
uniform vec3      samplingRays[NUM_SAMPLING_RAYS];

uniform mat4 toVoxelSpace;
uniform mat4 voxViewProjection;
uniform float voxelDimensionsInWorldSpace;

in vec3 worldPosition;
in vec3 normalFrag;

out vec4 color;


bool outOfBox(vec3 pos)
{
    
    return pos.x > 0.0f && pos.x < 1.0f &&
           pos.y > 0.0f && pos.y < 1.0f &&
        pos.z > 0.0f && pos.z < 1.0f;
}

vec4 ambientOcclusion( mat3 rotation )
{
    vec4 color = vec4(0.f, 0.f, 0.f, 0.f);
    for(int i = 0; i < 1; ++i)
    {
        vec3 direction = rotation * samplingRays[i];
        direction = normalize(direction);

        float j = voxelDimensionsInWorldSpace;
        while(j < 4.0f && color.a != 1)
        {
            vec3 worldPos = j * direction + worldPosition;
            vec4 proj = voxViewProjection * vec4(worldPos, 1.0f);

            if(!outOfBox(worldPos))
            {
                proj += 1.0f;
                proj *= .5f;

                color += texture(albedoMipMaps[0], proj.xyz);
            }

            j +=  voxelDimensionsInWorldSpace;
        }
    }
    
    return color;
}

void generateRotationMatrix( out mat3 rot, vec3 normal)
{
    vec3 yDimension = normal;
    vec3 xDimension = vec3(1.0f, 0.f, 0.f);
    xDimension = (normal == xDimension) ? vec3(1.0f, 0.001f, 0.0f) : xDimension;
    
    vec3 zDimension = cross(xDimension, yDimension);
    xDimension = cross(zDimension, yDimension);
    
    rot[0] = normalize(xDimension);
    rot[1] = normalize(yDimension);
    rot[2] = normalize(zDimension);
}

void main()
{
    vec4 voxelSpacePos = voxViewProjection * vec4(worldPosition,1.0f);
    
    voxelSpacePos += 1.0f;
    voxelSpacePos *= .5f;
    
    mat3 rotation;
    generateRotationMatrix(rotation, normalFrag);
    color = ambientOcclusion(rotation);

}
