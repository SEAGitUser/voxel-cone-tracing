//(1) This implementation is based on the one found here: http://prideout.net/blog/?p=64 "The Little Grasshoper"
//(2) pretty good explanation of AABB/ray intersection can be found here:
    //https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection


#version 410 core
/*
#define INV_STEP_LENGTH (1.0f/STEP_LENGTH)
#define STEP_LENGTH 0.005f
*/


uniform mat4    modelView;
uniform float   focalLength;
uniform vec2    windowSize;
uniform vec3    rayOrigin;
uniform sampler3D texture3D;

uniform float stepSize = 0.01f;
uniform float maxSamples = 5;


out vec4 fragColor;

struct Ray {
    vec3 Origin;
    vec3 Dir;
};

struct AABB {
    vec3 Min;
    vec3 Max;
};

bool IntersectBox(Ray r, AABB aabb, out float t0, out float t1)
{
    vec3 invR = 1.0 / r.Dir;
    //these are equation 2 of the (2) article mentioned on top of page
    vec3 tbot = invR * (aabb.Min-r.Origin);
    vec3 ttop = invR * (aabb.Max-r.Origin);
    vec3 tmin = min(ttop, tbot);
    vec3 tmax = max(ttop, tbot);
    //find the minimum ray t value between x, y, and z
    vec2 t = max(tmin.xx, tmin.yz);
    t0 = max(t.x, t.y);
    //find maximum ray t value between x, y, and z
    t = min(tmax.xx, tmax.yz);
    t1 = min(t.x, t.y);

    return t0 <= t1;
}

void main()
{
    vec3 rayDirection;
    rayDirection.xy = 2.0 * gl_FragCoord.xy / windowSize - 1.0;
    rayDirection.z = -focalLength;
    rayDirection = (vec4(rayDirection, 0) * modelView).xyz;
    
    Ray eye = Ray( rayOrigin, normalize(rayDirection) );
    AABB aabb = AABB(vec3(-1.0), vec3(+1.0));
    
    float tnear, tfar;
    IntersectBox(eye, aabb, tnear, tfar);
    if (tnear < 0.0) tnear = 0.0;
    
    vec3 rayStart = eye.Origin + eye.Dir * tnear;
    vec3 rayStop = eye.Origin + eye.Dir * tfar;
    
    // Transform from object space to texture coordinate space:
    // note that the box we are intersecting against has dimension ranges from -1 to 1 in all axis
    rayStart = 0.5 * (rayStart + 1.0);
    rayStop = 0.5 * (rayStop + 1.0);
    
    // Perform the ray marching:
    vec3 pos = rayStart;
    vec3 step = normalize(rayStop-rayStart) * stepSize;
    float travel = distance(rayStop, rayStart);
    fragColor = vec4(0);
    for (int i=0; i < maxSamples && travel > 0.0; ++i, pos += step, travel -= stepSize)
    {
        fragColor += texture(texture3D, pos);
    }
}

 
 /*
 uniform sampler2D textureBack; // Unit cube back FBO.
 uniform sampler2D textureFront; // Unit cube front FBO.
 
 uniform sampler3D texture3D; // Texture in which voxelization is stored.
 uniform vec3 cameraPosition; // World camera position.
 uniform int state = 0; // Decides mipmap sample level.
 
in vec2 textureCoordinateFrag; 
out vec4 color;

// Scales and bias a given vector (i.e. from [-1, 1] to [0, 1]).
vec3 scaleAndBias(vec3 p) { return 0.5f * p + vec3(0.5f); }


// Returns true if p is inside the unity cube (+ e) centered on (0, 0, 0).
bool isInsideCube(vec3 p, float e) { return abs(p.x) < 1 + e && abs(p.y) < 1 + e && abs(p.z) < 1 + e; }

void main() {
	float mipmapLevel = state;

	// Initialize ray.
	vec3 origin = isInsideCube(cameraPosition, 0.2f) ?
		cameraPosition : texture(textureFront, textureCoordinateFrag).xyz;
	vec3 direction = texture(textureBack, textureCoordinateFrag).xyz - origin;
	uint numberOfSteps = uint(INV_STEP_LENGTH * length(direction));
	direction = normalize(direction);
     
	// Trace.
	color = vec4(0.0f);
	for(uint step = 0; step < numberOfSteps && color.a < 0.99f; ++step) {
		vec3 currentPoint = origin + STEP_LENGTH * step * direction;
		vec3 coordinate = scaleAndBias(currentPoint);
		vec4 currentSample = textureLod(texture3D, scaleAndBias(currentPoint), mipmapLevel);
		color += (1.0f - color.a) * currentSample;
	} 
	color.rgb = pow(color.rgb, vec3(1.0 / 2.2));
}
 */
