
// Author:    Rafael Sabino
// Date:    03/30/2018
#version 410 core


in vec3 diffuseColorFrag;
in vec3 normalFrag;
noperspective in vec4 projectedPosition;

uniform sampler2D depthTexture;

layout(location = 0) out vec4 albedo;
layout(location = 1) out vec4 normal;


void main()
{
    vec2 fragPosition = vec2(projectedPosition.x, projectedPosition.y);
    
    //scale to [0,1] range
    fragPosition += vec2(1.0f, 1.0f);
    fragPosition *= 0.5f;
    
    vec4 depth = texture(depthTexture, fragPosition.st);
    
    float previousLayerDepth = depth.r;
    
    albedo = vec4(diffuseColorFrag,1.0f);
    normal = vec4(normalFrag, 1.0f);
    if(previousLayerDepth > gl_FragCoord.z ||
       abs(gl_FragCoord.z - previousLayerDepth) < 0.001f)
    {
        discard;
    }
}
