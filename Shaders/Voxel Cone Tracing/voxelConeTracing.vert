
// Author:    Rafael Sabino
// Date:    04/08/18

#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;



out vec3 voxViewPosFrag;
out vec3 normalFrag;

void main(){
    voxViewPosFrag = (  M * vec4(position, 1)).xyz;
    
    normalFrag = normalize(mat3(transpose(inverse(M))) * normal);
    gl_Position = P * V * M * vec4(position, 1);
}
