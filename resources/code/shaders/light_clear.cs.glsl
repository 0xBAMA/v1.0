#version 430

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in; //workgroup dimensions

uniform layout(rgba8) image3D current;
uniform layout(r8) image3D lighting;

uniform float intensity;

void main()
{
    imageStore(lighting, ivec3(gl_GlobalInvocationID.xyz), vec4(intensity));
}

