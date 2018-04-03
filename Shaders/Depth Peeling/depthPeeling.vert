
// Author:    Rafael Sabino
// Date:    04/01/2018
#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 MVP;
uniform vec3 diffuseColor;

out vec3 diffuseColorFrag;
out vec3 normalFrag;
noperspective out vec4 projectedPosition;


void main()
{
    gl_Position = MVP * vec4(position, 1);
    
    projectedPosition = gl_Position;
    diffuseColorFrag = diffuseColor;
    normalFrag = normal;
}


