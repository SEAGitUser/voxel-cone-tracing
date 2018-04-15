// Author:    Rafael Sabino
// Date:    04/11/2018

//this shader will down sample a texture by taking the average of surrounding texels and writing this average to destination

const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

static float4 getAverage(read_only image3d_t source, const sampler_t sampler, int4 coord)
{
        float4 value = read_imagef(source, sampler, (int4)(  coord.x * 2,    coord.y * 2,    coord.z * 2,    1)) +
        read_imagef(source, sampler, (int4)(coord.x * 2 + 1, coord.y * 2,    coord.z * 2,    1)) +
        read_imagef(source, sampler, (int4)(coord.x * 2,     coord.y * 2 + 1,coord.z * 2,    1)) +
        read_imagef(source, sampler, (int4)(coord.x * 2 + 1, coord.y * 2 + 1,coord.z * 2,    1)) +
        read_imagef(source, sampler, (int4)(coord.x * 2,     coord.y * 2,    coord.z * 2+ 1,1)) +
        read_imagef(source, sampler, (int4)(coord.x * 2 + 1, coord.y * 2,    coord.z * 2+ 1,1)) +
        read_imagef(source, sampler, (int4)(coord.x * 2,     coord.y * 2 + 1,coord.z * 2+ 1,1)) +
        read_imagef(source, sampler, (int4)(coord.x * 2 + 1, coord.y * 2 + 1,coord.z * 2+ 1,1));
        value *= 0.125f;
    
    return value;
}

kernel void downsample
(
    read_only image3d_t albedo,
    read_only image3d_t normal,

    write_only image3d_t albedoDest,
    write_only image3d_t normalDest

)
{
    int4 coord;
    coord.x = (int)get_global_id(0);
    coord.y = (int)get_global_id(1);
    coord.z = (int)get_global_id(2);
    coord.w = 1;


    float4 albedoAvg = getAverage(albedo, sampler, coord);
    float4 normalAvg = getAverage(normal, sampler, coord);
    
    write_imagef(albedoDest, (int4)coord, convert_float4(albedoAvg));
    write_imagef(normalDest, (int4)coord, convert_float4(normalAvg));
}
