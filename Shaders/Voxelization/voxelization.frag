// Lit (diffuse) fragment voxelization shader.
// Author:	Fredrik Präntare <prantare@gmail.com> 
// Date:	11/26/2016
#version 410 core

// Lighting settings.
#define POINT_LIGHT_INTENSITY 1
#define MAX_LIGHTS 1

// Lighting attenuation factors.
#define DIST_FACTOR 1.1f /* Distance is multiplied by this when calculating attenuation. */
#define CONSTANT 1
#define LINEAR 0
#define QUADRATIC 1

// Returns an attenuation factor given a distance.
float attenuate(float dist){ dist *= DIST_FACTOR; return 1.0f / (CONSTANT + LINEAR * dist + QUADRATIC * dist * dist); }

struct PointLight {
	vec3 position;
	vec3 color;
};

struct Material {
	vec3  diffuseColor;
	vec3  specularColor;
	float diffuseReflectivity;
	float specularReflectivity;
    float specularDiffusion;
	float emissivity;
	float transparency;
    float refractiveIndex;
};

uniform Material material;
uniform PointLight pointLights[MAX_LIGHTS];

//uniform int numberOfLights;
//uniform vec3 cameraPosition;
//uniform sampler3D sampler3Dims;

in vec3 worldPositionFrag;
in vec3 normalFrag;
in vec3 colorFrag;
in float totalSlicesFrag;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 normal;

vec3 calculatePointLight(const PointLight light)
{
    
	vec3 direction = normalize(light.position - worldPositionFrag);
	float distanceToLight = distance(light.position, worldPositionFrag);
	float attenuation = attenuate(distanceToLight);
	float d = max(dot(normalize(normalFrag), direction), 0.0f);
	return d * POINT_LIGHT_INTENSITY * attenuation * light.color;
    
}

vec3 scaleAndBias(vec3 p) { return 0.5f * p + vec3(0.5f); }

bool isInsideCube(const vec3 p, float e) { return abs(p.x) < 1 + e && abs(p.y) < 1 + e && abs(p.z) < 1 + e; }

void main()
{
    color = vec4(colorFrag,.4f);
    normal = vec4(normalFrag,.4f);
    
    
//    the code below hasn't been testest, commented out for this reason
//    color = vec4(depth.rrr, 1.0f);
//
//    if(!isInsideCube(worldPositionFrag, 0))
//        discard;
//
//    // Calculate diffuse lighting fragment contribution.
//    //uint maxLights = min(numberOfLights, MAX_LIGHTS);
//    //for(uint i = 0; i < maxLights; ++i)
//
//
//    //color = vec3(1.0f);
//    //TODO: we'll support more than one light eventually
//
//    PointLight pointLight = pointLights[0];
//    color += calculatePointLight(pointLight);
//    vec3 spec = material.specularReflectivity * material.specularColor;
//    vec3 diff = material.diffuseReflectivity * material.diffuseColor;
//    color = (diff + spec) * color + clamp(material.emissivity, 0, 1) * material.diffuseColor;
    
    
    
    
    

    //TODO: following code works on opengl 4.5, it needs to be deleted
    /*
	// Output lighting to 3D texture.
	vec3 voxel = scaleAndBias(worldPositionFrag);
	ivec3 dim = textureSize(sampler3Dims,0);
	float alpha = pow(1 - material.transparency, 4); // For soft shadows to work better with transparent materials.
	vec4 res = alpha * vec4(vec3(color), 1);
    imageStore(sampler3Dims, ivec3(dim * voxel), res);
    */
}
