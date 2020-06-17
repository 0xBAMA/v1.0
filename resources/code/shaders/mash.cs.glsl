#version 430

//note that this only effects what the parameters to glDispatchCompute are - by using gl_GlobalInvocationID, you don't need to worry what any of those numbers are
layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;    //specifies the workgroup size

uniform layout(rgba8) image3D current;        //values of the block after the update
uniform layout(r8) image3D lighting;        //values held in the lighting buffer

void main()
{
    vec4 color = imageLoad(current, ivec3(gl_GlobalInvocationID.xyz));    //existing color value (what is the color?)
    vec4 light = imageLoad(lighting, ivec3(gl_GlobalInvocationID.xyz));    //existing light value

    color.rgb *= (5*light.r);  //same scaling as in the display shader

    imageStore(current, ivec3(gl_GlobalInvocationID.xyz), color);
}
