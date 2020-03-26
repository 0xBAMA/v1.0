#version 450

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
uniform layout(rgba8) image3D current;

void main()
{
  float dist = distance(gl_GlobalInvocationID.xyz, vec3(0));
  if((dist < 100 && dist > 90) || (dist < 80 && dist > 70))
  {
    imageStore(current, ivec3(gl_GlobalInvocationID.xyz), vec4(1));
  }
}
