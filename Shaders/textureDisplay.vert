
// Author:    Rafael Sabino
// Date:    03/29/2018

#version 410 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 textureCoord;

out vec3 texCoord;

void main()
{
    gl_Position = vec4(position,1);
    texCoord = textureCoord;
}


