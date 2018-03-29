// Author:	Fredrik Präntare <prantare@gmail.com>
// Date:	11/26/2016
#version 410 core

layout(location = 0) in vec3 position;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 worldPosition;
out vec3 projectedPosition;


void main()
{
    worldPosition = position;
    //todo: optimize this
    gl_Position = P * V * M * vec4(worldPosition, 1);
    
    projectedPosition = gl_Position.xyz;
}


