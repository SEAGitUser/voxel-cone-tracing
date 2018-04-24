
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


//uniform Settings settings;
//uniform PointLight pointLights[1];
//uniform int numberOfLights; // Number of lights currently uploaded.
//uniform vec3 cameraPosition; // World campera position.
//uniform int state; // Only used for testing / debugging.


uniform sampler3D normalMipMaps[NUM_MIP_MAPS];
uniform sampler3D albedoMipMaps[NUM_MIP_MAPS];
uniform vec3      samplingRays[NUM_SAMPLING_RAYS];

uniform mat4    toVoxelSpace;
uniform mat4    voxViewProjection;
uniform float   voxelDimensionsInWorldSpace;

uniform vec3    view; //new: make sure is normalized
uniform vec3    lightPos; //new
uniform float   coneVariances[NUM_MIP_MAPS];
uniform uint   numberOfLods;

uniform Material material;


float dimensionInverse = 1.0f/voxelDimensionsInWorldSpace;
float distanceLimit = 15.0f * voxelDimensionsInWorldSpace;
float oneOverDistanceLimit = 1.0f/distanceLimit;
float oneOverNumLODs = 1.0f/numberOfLods;
float distanceBetweenLods = distanceLimit / numberOfLods;
vec4  albedoLODColors[NUM_MIP_MAPS];
vec4  normalLODColors[NUM_MIP_MAPS];
float rayWeight = 1.0f/float(NUM_SAMPLING_RAYS);


in vec3 worldPosition;
in vec3 normalFrag;

out vec4 color;


bool withinBounds(vec3 pos)
{
    
    return pos.x > -1.0f && pos.x < 1.0f &&
           pos.y > -1.0f && pos.y < 1.0f &&
        pos.z > -1.0f && pos.z < 1.0f;
}

void branchlessONB(vec3 n, out mat3 rotation)
{
    //based off of "Building Orthonormal Basis, Revisited", Pixar Animation Studios
    //https://graphics.pixar.com/library/OrthonormalB/paper.pdf
    float s = int(n.z >= 0) - int(n.z < 0);
    float a = -1.0f / (s + n.z);
    float b = n.x * n.y * a;
    vec3 b1 = vec3(1.0f + s * n.x * n.x * a, s * b, -s * n.x);
    vec3 b2 = vec3(b, s + n.y * n.y * a, -n.y);
    
    rotation[0] = b1;
    rotation[1] = n;
    rotation[2] = b2;
}


float gaussianLobeDistribution(vec3 normal, float variance)
{
    //based off of: https://math.stackexchange.com/questions/434629/3-d-generalization-of-the-gaussian-point-spread-function
    float sigma = variance;
    float e = 2.71828f;
    float pi = 3.14159f;
    float N = pow(2.0f, 3.0f) * pow(sigma, 2.0 * 3.0f) * pow(pi, 3.0f);
    N = 1.0f/sqrt(N);
    float power = -(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z)/(sigma * sigma);
    float gauss = N * pow(e, power);
    
    return gauss;
}

float toksvigFactor(vec3 normal, float s)
{
    
    //based off of "Mipmapping Normal Maps", Nvidia
    //https://developer.download.nvidia.com/whitepapers/2006/Mipmapping_Normal_Maps.pdf
    float rlen = 1.0f/clamp(length(normal), 0.0f, 1.0f);
    //sigma = |N|/(|N| + s(1 - |N|)).  Below, we just devided numerator and denominator by |N|
    return 1.0f/(1.0f + s * (rlen - 1.0f));
}

vec4 getLODColor(float distance, vec4 lodColors[NUM_MIP_MAPS])
{
    
    float travel = distance * oneOverDistanceLimit;
    
    float fractional = float(numberOfLods) * travel;
    uint lod = uint(floor(fractional));
    
    fractional = fract(fractional);
    lod = min(lod, uint(numberOfLods - 1.0));
    
    uint next = uint((lod == (numberOfLods-1)));
    uint lod2 = lod + next;
    
    vec4 blendedColor = mix(lodColors[lod], lodColors[lod2], fractional);

    return blendedColor;
}


void ambientOcclusion(vec3 direction, float j, float step, vec3 worldPos, inout vec4 sampleColor )
{
    float lambda = 10.0f;

    vec4 projection = voxViewProjection * vec4(worldPos, 1.0f);
    
    if(withinBounds( vec3(projection.xyz) ))
    {
        float attenuation = (1/(1 + j*lambda));
        vec4 fromLOD = getLODColor(j, albedoLODColors);
        fromLOD.a = pow((1 -(1 - fromLOD.a)), step * dimensionInverse);
        sampleColor.a += (1 - sampleColor.a) * fromLOD.a * attenuation;
        sampleColor.a = 1 - sampleColor.a;
    }
    
}

float getVariance(float distance)
{
    float travel = distance * oneOverDistanceLimit;
    
    float fractional = float(NUM_MIP_MAPS) * travel;
    uint lod = uint(floor(fractional));
    
    fractional = fract(fractional);
    lod = min(lod, NUM_MIP_MAPS);
    
    uint next = uint((lod == (NUM_MIP_MAPS-1)));
    uint lod2 = lod + next;
    
    float variance = mix(coneVariances[lod], coneVariances[lod2], fractional);
    return variance;
}

//section 7 and 8.1 of the paper
vec3 indirectIllumination( vec3 geometryNormal,  float j, vec3 samplingPos)
{
    vec3 avgNormal =  getLODColor(j, normalLODColors).xyz;// texture(normalMipMaps[mipMapIndex], projection.xyz).xyz;
    vec3 sampleFromLOD = getLODColor(j, albedoLODColors).xyz;
    vec3 result = vec3(0.0f);
    vec3 fullSizeNormal = normalize(avgNormal.xyz);
    
    float dotProduct = dot(fullSizeNormal, geometryNormal);
    bool normalsFacingEachOther = dotProduct <= 0;
    //if( normalsFacingEachOther )
    {
        mat3 normalRotation;
        branchlessONB(fullSizeNormal, normalRotation );
        
        //from here on out, we work on the sampling point space.
        geometryNormal = geometryNormal * normalRotation;
        vec3 worldPosInSampleSpace = worldPosition.xyz - samplingPos.xyz;
        worldPosInSampleSpace = worldPosInSampleSpace * normalRotation ;
        
        vec3 view =  normalize(worldPosInSampleSpace);
        
        //in sampling point space, the normal is always up
        vec3 up = vec3(0.f, 1.0f, 0.0f);
        
        float variance = getVariance(j);
        float gauss = gaussianLobeDistribution( view, variance);
        
        vec3 lightDirection = view;
        //Blinn Phong
        // this is equivalent to normalize(view + lightDirection); because view = lightDirection
        vec3 h = view;
        float ndoth = clamp(dot(up, h), 0.0f, 1.0f);
        //todo: we need a specular map where this power comes from, for now it is constant
        float s = .0005f;
        float gloss = toksvigFactor(avgNormal, s);
        
        float p = s * gloss;
        float spec = pow(ndoth, p)* (1 + gloss*s)/(1 + s);
        
        float ndotl = dot(up, lightDirection);
        result =  (sampleFromLOD + sampleFromLOD * spec)  * ndotl * gloss;
    }
    return result;
}

vec4 directIllumination(vec4 lightPos, vec4 sampleColor)
{
    return vec4(0.0f);
}

void collectLODColors(vec3 direction)
{
    float j = voxelDimensionsInWorldSpace;
    float step = distanceLimit / numberOfLods;
    uint lod = 0;
    while(j < distanceLimit && (lod != (numberOfLods) ))
    {
        vec3 worldPos = j * direction + worldPosition;
        vec4 proj = voxViewProjection * vec4(worldPos, 1.0f);
        
        if(withinBounds( vec3(proj.xyz) ))
        {
            proj += 1.0f;
            proj *= .5f;
            
            albedoLODColors[ lod ] = texture(albedoMipMaps[ lod ], proj.xyz);
            normalLODColors[ lod ] = texture(normalMipMaps[ lod ], proj.xyz);
        }
        else
            break;
        lod += 1;
        j +=  step ;
        lod = min(lod, (numberOfLods));
    }
}

vec4 voxelConeTracing( mat3 rotation,vec3 incomingNormal )
{
    vec4 ambient = vec4(0.f);
    float step = distanceLimit / 7.0f;
    
    for(uint i = 0; i < NUM_SAMPLING_RAYS; ++i)
    {
        vec3 direction = rotation * samplingRays[i];
        //typically you would want to normalize because rotating a normal does not guarantee that it remains
        //unit length one.  In this case, I've commented the line below because it doesn't have much of an effect on the final result.
        //direction = normalize(direction) ;
        
        vec4 sampleColor = vec4(0.0f);
        collectLODColors(direction);
        
        float j = voxelDimensionsInWorldSpace * 1.5f;

        while(j < distanceLimit)
        {
            vec3 worldPos = j * direction + worldPosition;
            ambientOcclusion(direction, j, step, worldPos, sampleColor);
            sampleColor.xyz += indirectIllumination(incomingNormal, j, worldPos);
            
            j += step;
        }

        
        ambient.xyz += sampleColor.xyz;
        ambient.a += sampleColor.a * rayWeight;
    }
    
    return vec4(ambient.xyz, 1.0f);
}



void main()
{
    mat3 rotation;
    branchlessONB(normalFrag, rotation);
    vec3 incomingNormal = normalize(normalFrag);
    color = voxelConeTracing(rotation, incomingNormal);
    //todo: perform direct illumination here
    
}
