
// Author:    Rafael Sabino
// Date:    03/30/2018
#version 410 core

uniform sampler2D displayTexture;

in vec3 texCoord;
out vec4 color;

void main()
{
    color = texture(displayTexture, texCoord.st);
}
