//Author:  Rafael Sabino
// Date:    02/28/2018

#version 410 core

uniform mat4 MVP;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec2 textureCoordinateFrag; 

// Scales and bias a given vector (i.e. from [-1, 1] to [0, 1]).
vec2 scaleAndBias(vec2 p) { return 0.5f * p + vec2(0.5f); }

void main()
{
    gl_Position = MVP * vec4(position,1.0f);
}
