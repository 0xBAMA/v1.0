#version 450

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

uniform layout(rgba8) image3D previous;       //now-current value of the block
uniform layout(r8) image3D previous_mask;  //now-current value of the mask

uniform layout(rgba8) image3D current;        //value of the block after the update
uniform layout(r8) image3D current_mask;   //value of the mask after the update

void main()
{
  float dist = distance(gl_GlobalInvocationID.xyz, vec3(127));
  bool mask = (imageLoad(previous_mask, ivec3(gl_GlobalInvocationID.xyz)).r > 0.5);



  if(((dist < 100 && dist > 90) || (dist < 80 && dist > 70)) && !mask)
  {
    imageStore(current, ivec3(gl_GlobalInvocationID.xyz), vec4(0.5));
  }
}
