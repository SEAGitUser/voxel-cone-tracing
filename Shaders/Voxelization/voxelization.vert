// Author:	rafael sabino
// Date:	01/31/2018

#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

//cube dimensions is assumed to be a power of 2
uniform uint cubeDimensions;
uniform sampler2D depthTexture;

out float totalSlicesGeom;
out vec4 colorGeom;

void throwAwayVertex()
{
    gl_Position.x = 10000.0f;
}
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
    
    if(depth == 1.0f)
    {
        throwAwayVertex();
    }
    else
    {
        colorGeom = vec4(0.0f, 1.0f, 0.0f, 1.0f);

        gl_Position.xy = vec2(x,y);
        gl_Position.z = depth;
        gl_Position.w = 1.0f;
    }

}
