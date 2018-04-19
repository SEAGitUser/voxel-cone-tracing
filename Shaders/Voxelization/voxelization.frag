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



struct PointLight {
	vec3 position;
	vec3 color;
};

uniform PointLight pointLights[MAX_LIGHTS];

uniform uint numberOfLights;


in vec3 worldPositionFrag;
in vec3 normalFrag;
in vec3 colorFrag;
in float totalSlicesFrag;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 normal;

// Returns an attenuation factor given a distance.
float attenuate(float dist){ dist *= DIST_FACTOR; return 1.0f / (CONSTANT + LINEAR * dist + QUADRATIC * dist * dist); }

vec3 calculatePointLight( PointLight light)
{
    vec3 direction = light.position - worldPositionFrag;
    direction = normalize(direction);
    float distanceToLight = distance(light.position, worldPositionFrag);
    float attenuation = attenuate(distanceToLight);
    float d = max(dot(normalize(normalFrag), direction), 0.0f);
    return d * POINT_LIGHT_INTENSITY * attenuation * light.color;
}


void main()
{
    vec3 albedo = colorFrag;
    normal = vec4(normalFrag,1.f);
    
    // Calculate diffuse lighting fragment contribution.
    uint maxLights = min(numberOfLights, MAX_LIGHTS);
    for(uint i = 0; i < maxLights; ++i)
    {
        PointLight pointLight = pointLights[i];
        vec3 lightColor = calculatePointLight(pointLight);

        color.xyz = albedo * lightColor;
        color.a = 1.0f;
    }
}
