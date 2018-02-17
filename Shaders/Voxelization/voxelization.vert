// Author:	rafael sabino
// Date:	01/31/2018

#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 worldToUnitCube;
uniform float totalSlices;

out vec3 worldPositionGeom;
out vec3 normalGeom;
out float totalSlicesGeom;

void main(){
    
	worldPositionGeom = vec3(modelMatrix * vec4(position, 1));
	normalGeom = normalize(mat3(transpose(inverse(modelMatrix))) * normal);
    totalSlicesGeom = totalSlices;
    
    gl_Position = worldToUnitCube * modelMatrix * vec4(position,1);
    
	//gl_Position = P * V * vec4(worldPositionGeom, 1);
}
