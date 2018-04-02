
// Author:    Rafael Sabino
// Date:    04/01/2018
#version 410 core

layout(location = 0) in vec3 position;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 worldPosition;
noperspective out vec4 projectedPosition;


void main()
{
    worldPosition = position;
    gl_Position = P * V * M * vec4(worldPosition, 1);
    
    projectedPosition = gl_Position;
}


