
// Author:    Rafael Sabino
// Date:    03/30/2018
#version 410 core


//this shader is tied to Positions/world_position.vert

in vec3 worldPosition;
in vec3 projectedPosition;

uniform sampler2D depthTexture;

out vec4 color;

void main()
{
    vec2 fragPosition = vec2(projectedPosition.x, projectedPosition.y);
    
    //scale to [0,1] range
    fragPosition += vec2(1.0f, 1.0f);
    fragPosition *= 0.5f;
    
    color = texture(depthTexture, fragPosition.st);
    color.a = 1;
    
    float previousLayerDepth = color.r;
    if(previousLayerDepth > gl_FragCoord.z ||
       abs(gl_FragCoord.z - previousLayerDepth) < 0.001f)
    {
        discard;
    }
}
