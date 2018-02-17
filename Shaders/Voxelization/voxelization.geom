// Simple non-conservative voxelization.

#version 410 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 worldPositionGeom[];
in vec3 normalGeom[];

in float totalSlicesGeom[];

out vec3 normalFrag;
out vec3 worldPositionFrag;

void main(){
    
    for(uint i = 0; i < 3; ++i)
    {
        //TODO: if you push the the coordinates half a voxel along the normal you may achieve the same effect as
        //conservative raterization, look into this.
        vec4 normalizedTexCoords =vec4(float(gl_in[i].gl_Position.x), float(gl_in[i].gl_Position.y), float(gl_in[i].gl_Position.z), 1.0f);
        
        //going back to projection space between -1 to 1 on both x and y.
        //For a little background go here: http://www.songho.ca/opengl/gl_projectionmatrix.html
        vec2 inputPos = vec2(
                             (normalizedTexCoords.x * 2.0f) -1.0f,
                             (normalizedTexCoords.y * 2.0f) -1.0f
                             );
        
        gl_Layer = int(floor(normalizedTexCoords.z * totalSlicesGeom[i]));
        //z coordinate is represented by the layer above,  that's why there is a 0 on the z axis here
        gl_Position = vec4(inputPos.x, inputPos.y, 0.0f, 1.0f);
        
        normalFrag = normalGeom[i];
        worldPositionFrag = worldPositionGeom[i];
        EmitVertex();
    }
    EndPrimitive();
    
    /*
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
