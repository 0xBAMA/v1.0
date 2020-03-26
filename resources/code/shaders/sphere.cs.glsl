#version 450

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
uniform layout(rgba8) image3D current;

void main()
{
  if(distance(gl_GlobalInvocationID.xyz, vec3(0)) < 100)
  {
    imageStore(current, ivec3(gl_GlobalInvocationID.xyz), vec4(1));
  }
}
