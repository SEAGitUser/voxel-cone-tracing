
// Author:    Rafael Sabino
// Date:    04/08/18

#version 410 core

#define NUM_SAMPLING_RAYS 5
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
    vec4 ambient = vec4(0.f);
    
    for(int i = 0; i < NUM_SAMPLING_RAYS; ++i)
    {
        vec3 direction = rotation * samplingRays[4];
        direction = normalize(direction) ;

        vec4 sampleColor = vec4(0.0f);
        float limit = 5.0f * voxelDimensionsInWorldSpace;
        float step = limit / 6.0f;
        float j = voxelDimensionsInWorldSpace ;
        
        int lod = 0;
        while(j < limit && sampleColor.a < 1.0f)
        {
            vec3 worldPos = j * direction + worldPosition;
            vec4 proj = voxViewProjection * vec4(worldPos, 1.0f);
            
            float lambda = 8.0f;
            float attenuation = (1/(1 + j*lambda));
            //if(!outOfBox( vec3(proj.xyz) ))
            {
                proj += 1.0f;
                proj *= .5f;

                vec4 fromLOD = texture(albedoMipMaps[lod], proj.xyz);
                sampleColor.a += (1 - sampleColor.a) * fromLOD.a;
                
                if(sampleColor.a >= 1.0f)
                {
                    break;
                }
            }
            
            lod += 1;
            j +=  step ;
            lod = min(lod, 4);
        }
        sampleColor.a *= float(1.0f)/float(NUM_SAMPLING_RAYS);
        
        ambient += sampleColor;
    }

    return vec4(ambient.aaa, 1.0f);
}


void branchlessONB(vec3 n, out mat3 rotation)
{
    //based off of "Building Orthonormal Basis, Revisited", Pixar Animation Studios
    //https://graphics.pixar.com/library/OrthonormalB/paper.pdf
    float s = sign(n.z) ;
    s = s == 0 ? 1 : s;
    float a = -1.0f / (s + n.z);
    float b = n.x * n.y * a;
    vec3 b1 = vec3(1.0f + s * n.x * n.x * a, s * b, -s * n.x);
    vec3 b2 = vec3(b, s + n.y * n.y * a, -n.y);
    
    rotation[0] = b1;
    rotation[1] = n;
    rotation[2] = b2;
}

void main()
{
    vec4 voxelSpacePos = voxViewProjection * vec4(worldPosition,1.0f);
    
    voxelSpacePos += 1.0f;
    voxelSpacePos *= .5f;
    
    {
        

        mat3 rotation;
        branchlessONB(normalFrag, rotation);
        color = ambientOcclusion(rotation);

    }


}
