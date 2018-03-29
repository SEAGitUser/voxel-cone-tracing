// Simple non-conservative voxelization.

#version 410 core

layout(points) in;
layout(points, max_vertices = 1) out;


uniform mat4 inverseProjection;
uniform float farPlane;
//in vec3 worldPositionGeom[];
//in vec3 normalGeom[];
in float totalSlicesGeom[];
in vec4 colorGeom[];




out vec3 normalFrag;
//out vec3 worldPositionFrag;
out vec3 colorFrag;
out float totalSlicesFrag;

void main(){
    
    vec4 normalizedTexCoords =vec4(float(gl_in[0].gl_Position.x), float(gl_in[0].gl_Position.y), float(gl_in[0].gl_Position.z), 1.0f);
    
    //going back to projection space between -1 to 1 on both x and y.
    //For a little background go here: http://www.songho.ca/opengl/gl_projectionmatrix.html
    vec2 inputPos = vec2(
                         (normalizedTexCoords.x * 2.0f) -1.0f,
                         (normalizedTexCoords.y * 2.0f) -1.0f
                         );
    

    gl_Position = vec4(inputPos.x, inputPos.y, 0.0f, 1.0f);
    
    //check these answers for info about why linearly multiplying the depth value times the total number of slices works out.
    //https://stackoverflow.com/questions/7777913/how-to-render-depth-linearly-in-modern-opengl-with-gl-fragcoord-z-in-fragment-sh
    //https://stackoverflow.com/questions/10264949/glsl-gl-fragcoord-z-calculation-and-setting-gl-fragdepth
    
    gl_Layer = int(totalSlicesGeom[0] * normalizedTexCoords.z);
    gl_Position.z = 0.0f;
    colorFrag = colorGeom[0].rgb;

    totalSlicesFrag = totalSlicesGeom[0];
    
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
