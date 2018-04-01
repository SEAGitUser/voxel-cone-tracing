// Author:	rafael sabino
// Date:	01/31/2018

#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

//cube dimensions is assumed to be a power of 2
uniform uint cubeDimensions;
uniform sampler2D depthTexture;
uniform vec3 camPosition;

out float totalSlicesGeom;
out vec4 colorGeom;

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
    depth = 1.0f - depth;
    //if "if" statement fails below we are effectively throwing this vertex away
    gl_Position.x = 10000.0f;
    if(depth != 0.0f)
    {
        //here we are transforming the projections of x y planes into a z plane.
        //in the geometry shader, it is assumed that we are looking the camera using a z plane
        //which plane we pick here depends on the position of the camera when rendering the depth buffer sampled above
        
        //projection to z plane ( blue)
        gl_Position.xy = vec2(x,y);
        gl_Position.z = depth;
        gl_Position.w = 1.0f;
        colorGeom = vec4(.0f, 0.0f, 1.0f, 1.0f);
        vec3 n = normalize(camPosition);
        //projection from x plane to z plane ( red )
        if(n.x > n.y && n.x > n.z)
        {
            gl_Position.x = depth;
            gl_Position.y = y;
            gl_Position.z = x;
            colorGeom = vec4(1.0f, .0f, 0.0f, 1.0f);
        }
        //projection from y plane to z plane (green)
        else if(n.y > n.x && n.y > n.z)
        {
            gl_Position.x = y;
            gl_Position.y =  depth;
            gl_Position.z = x;
            colorGeom = vec4(.0f, 1.0f, .0f, 1.0f);
        }
    }
}
