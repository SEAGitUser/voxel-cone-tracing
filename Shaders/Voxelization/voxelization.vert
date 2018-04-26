// Author:	rafael sabino
// Date:	01/31/2018

#version 410 core

layout(location = 0) in vec3 position;

//cube dimensions is assumed to be a power of 2
uniform uint cubeDimensions;
uniform sampler2D depthTexture;
uniform sampler2D albedoTexture;
uniform sampler2D normalTexture;


//these are assumed to be from orthographic projections
uniform mat4 zPlaneProjection;
uniform mat4 toWorldSpace;

uniform float voxelSizeInWorldSpace;


out float totalSlicesGeom;
out vec3 normalGeom;
out vec3 albedoGeom;
out vec3 worldPositionGeom;

void main(){
    
    //this is the same thing as the remainder or mod operator, but way faster. total slices is always a power of 2, causing
    //this "mod" to always give us the right answer.
    float x = uint(gl_InstanceID) & uint(cubeDimensions - 1);
    float y = int(float(gl_InstanceID) / float(cubeDimensions));
    
    float OFFSET_TO_PIXEL_CENTER = 0.5f;
    x += OFFSET_TO_PIXEL_CENTER;
    y += OFFSET_TO_PIXEL_CENTER;
    
    x /= float(cubeDimensions);
    y /= float(cubeDimensions);

    totalSlicesGeom = float(cubeDimensions);
    float depth = texture(depthTexture, vec2(x,y)).r;
    
    //if "if" statement fails below we are effectively throwing this vertex away
    gl_Position.x = 10000.0f;
    if(depth != 1.0f)
    {
        albedoGeom = texture(albedoTexture, vec2(x,y)).xyz;
        normalGeom = texture(normalTexture, vec2(x,y)).xyz;
        
        vec4 clipSpacePos = vec4(x, y, depth, 1.f );
        
        clipSpacePos.xyz *= 2.0f;
        clipSpacePos.xyz -= 1.0f;
        
        vec4 worldSpacePos = toWorldSpace * clipSpacePos;
        
        //worldSpacePos.xyz += normalGeom * voxelSizeInWorldSpace;
        
        //here we are transforming the projections of x y planes into a z plane.
        //in the geometry shader, it is assumed that we are looking the camera using a z plane
        //which plane we pick here depends on the position of the camera when rendering the depth buffer sampled above
        gl_Position =  zPlaneProjection * worldSpacePos;
        
        worldPositionGeom = worldSpacePos.xyz;
    }
}
