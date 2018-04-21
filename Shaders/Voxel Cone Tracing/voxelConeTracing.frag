
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
uniform float   coneVariances[NUM_MIP_MAPS]; //new
uniform float   coneApertures[NUM_MIP_MAPS]; //new
uniform Material material; //new


float dimensionInverse = 1/voxelDimensionsInWorldSpace;

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

vec3 ambientOcclusion(vec4 projection, uint mipMapIndex, float step, float attenuation, inout vec4 sampleColor)
{
    vec4 fromLOD = texture(albedoMipMaps[ mipMapIndex ], projection.xyz);
    fromLOD.a = pow((1 -(1 - fromLOD.a)), step * dimensionInverse);
    sampleColor.a += (1 - sampleColor.a) * fromLOD.a * attenuation;
    sampleColor.a = 1 - sampleColor.a;
    
    //the paper mentions to do an attenuation between the samples of the LOD's, I've chosen not to do this
    //because I think it looks good enough, no need to add more computation.
    
    return fromLOD.xyz;
}

//section 7 and 8.1 of the paper
vec3 indirectIllumination(vec3 sampleFromLOD, vec3 geometryNormal, uint rayIndex,
                          uint mipMapIndex, vec4 projection, vec3 samplingPos)
{
    vec3 avgNormal = texture(normalMipMaps[mipMapIndex], projection.xyz).xyz;
    vec3 result = vec3(0.0f);
    vec3 fullSizeNormal = normalize(avgNormal.xyz);

    float dotProduct = dot(fullSizeNormal, geometryNormal);
    bool normalsFacingEachOther = dotProduct <= 0;
    if( normalsFacingEachOther )
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
        float gauss = gaussianLobeDistribution( up * length(avgNormal), coneVariances[mipMapIndex]);
        


        vec3 lightDirection = view;
        //Blinn Phong
        // this is equivalent to normalize(view + lightDirection);
        vec3 h = view;
        float ndoth = clamp(dot(up, h), 0.0f, 1.0f);
        //todo: we need a specular map where this power comes from, for now it is constant
        float s = 800.0f;
        float gloss = toksvigFactor(avgNormal, s);

        float p = s * gloss;
        float spec = pow(ndoth, p)* (1 + gloss*s)/(1 + s);

        float ndotl = dot(up, lightDirection);
        float scaleUp = 40.0f;
        result =  (sampleFromLOD + spec * vec3(1.0f, 1.0f, 1.0f)) * scaleUp * ndotl * gauss;
    }
    return result;
}

vec4 voxelConeTracing( mat3 rotation,vec3 incomingNormal )
{
    vec4 ambient = vec4(0.f);
    
    float numSamplingRaysInverse = float(1.0f)/float(NUM_SAMPLING_RAYS);
    float limit = 8.f * voxelDimensionsInWorldSpace;
    float step = limit / 5.0f;
    
    for(uint i = 0; i < NUM_SAMPLING_RAYS; ++i)
    {
        vec3 direction = rotation * samplingRays[i];
        //typically you would want to normalize because rotating a normal does not guarantee that it remains
        //unit length one.  In this case, I've commented the line below because it doesn't have much of an effect on the final result.
        //direction = normalize(direction) ;

        vec4 sampleColor = vec4(0.0f);
        float j = voxelDimensionsInWorldSpace ;

        uint lod = 0;
        while(j < limit &&  sampleColor.a < 1.0f)
        {
            vec3 worldPos = j * direction + worldPosition;
            vec4 proj = voxViewProjection * vec4(worldPos, 1.0f);
    
            float lambda = 20.0f;
            float attenuation = (1/(1 + j*lambda));
            if(withinBounds( vec3(proj.xyz) ))
            {
                proj += 1.0f;
                proj *= .5f;
                
                vec3 colorFromLOD =  ambientOcclusion(proj, lod, step, attenuation, sampleColor);

                sampleColor.xyz += indirectIllumination(colorFromLOD, incomingNormal, i, lod, proj, worldPos);
            }
            else
                break;
            
            lod += 1;
            j +=  step ;
            lod = min(lod, 5);
        }

        ambient += sampleColor;
        ambient.a = 1.0f;
    }
    return ambient;
}


void main()
{
    mat3 rotation;
    branchlessONB(normalFrag, rotation);
    vec3 incomingNormal = normalize(normalFrag);
    color = voxelConeTracing(rotation, incomingNormal);
}
