// Simple non-conservative voxelization.

#version 410 core

layout(points) in;
layout(points, max_vertices = 1) out;


uniform mat4 inverseProjection;
uniform float farPlane;

in vec3 normalGeom[];
in vec3 albedoGeom[];
in float totalSlicesGeom[];
in vec3 worldPositionGeom[];


out vec3 normalFrag;
out vec3 colorFrag;
out float totalSlicesFrag;
out vec3 worldPositionFrag;

void main(){
    
    vec4 normalizedTexCoords =vec4(float(gl_in[0].gl_Position.x), float(gl_in[0].gl_Position.y), float(gl_in[0].gl_Position.z), 1.0f);

    gl_Position =normalizedTexCoords;

    
    //check these answers for info about why linearly multiplying the depth value times the total number of slices works out.
    //https://stackoverflow.com/questions/7777913/how-to-render-depth-linearly-in-modern-opengl-with-gl-fragcoord-z-in-fragment-sh
    //https://stackoverflow.com/questions/10264949/glsl-gl-fragcoord-z-calculation-and-setting-gl-fragdepth
    
    //scale from [-1, 1] to [0, 1]
    float depth = (gl_Position.z + 1.0f) * .5f;
    gl_Layer = int((totalSlicesGeom[0] * depth));
    //gl_Layer -= 1;

    colorFrag = albedoGeom[0].rgb;
    normalFrag = normalGeom[0].rgb;

    totalSlicesFrag = totalSlicesGeom[0];
    worldPositionFrag = worldPositionGeom[0];
    
    EmitVertex();
    EndPrimitive();
    
    /*
    //this is the original shader written against glsl 4.5 core, will leave here for reference.  Code
    //commented out because apple doesn't support 4.5.
     
	vec3 p1 = worldPositionGeom[1] - worldPositionGeom[0];
	vec3 p2 = worldPositionGeom[2] - worldPositionGeom[0];
	vec3 p = abs(cross(p1, p2)); 
	for(uint i = 0; i < 3; ++i){
		worldPositionFrag = worldPositionGeom[i];
		normalFrag = normalGeom[i];
		if(p.z > p.x && p.z > p.y){
			gl_Position = vec4(worldPositionFrag.x, worldPositionFrag.y, 0, 1);
		} else if (p.x > p.y && p.x > p.z){
			gl_Position = vec4(worldPositionFrag.y, worldPositionFrag.z, 0, 1);
		} else {
			gl_Position = vec4(worldPositionFrag.x, worldPositionFrag.z, 0, 1);
		}
		EmitVertex();
	}
    EndPrimitive();
    */
}
