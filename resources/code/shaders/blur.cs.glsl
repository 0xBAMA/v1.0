#version 430

//note that this only effects what the parameters to glDispatchCompute are - by using gl_GlobalInvocationID, you don't need to worry what any of those numbers are
layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;    //specifies the workgroup size

uniform layout(rgba8) image3D previous;       //now-current values of the block
uniform layout(r8) image3D previous_mask;  //now-current values of the mask

uniform layout(rgba8) image3D current;        //values of the block after the update
uniform layout(r8) image3D current_mask;   //values of the mask after the update

uniform int radius;
uniform bool respect_mask;
uniform bool touch_alpha;

vec4 mask_true = vec4(1.0,0.0,0.0,0.0);
vec4 mask_false = vec4(0.0,0.0,0.0,0.0);

void main()
{
  bool pmask = (imageLoad(previous_mask, ivec3(gl_GlobalInvocationID.xyz)).r > 0.5);  //existing mask value (previous_mask = 0?)
  vec4 pcol = imageLoad(previous, ivec3(gl_GlobalInvocationID.xyz));                 //existing color value (what is the previous color?)

  if(pmask && respect_mask)
  {
    imageStore(current_mask, ivec3(gl_GlobalInvocationID.xyz), mask_true);
    imageStore(current, ivec3(gl_GlobalInvocationID.xyz), pcol);
  }
  else
  {
    int num = 0;
    vec4 csum = vec4(0,0,0,0);
    float msum = 0.0;

    for(int x = (-1 * radius); x <= radius; x++)
    {
      for(int y = (-1 * radius); y <= radius; y++)
      {
        for(int z = (-1 * radius); z <= radius; z++)
        {
          csum += imageLoad(previous, ivec3(gl_GlobalInvocationID.xyz) + ivec3(x,y,z));
          msum += (imageLoad(previous_mask, ivec3(gl_GlobalInvocationID.xyz) + ivec3(x,y,z)).r > 0.5) ? 1.0 : 0.0;
          num++;
        }
      }
    }

    //divide csum and msum by num
    csum /= num;
    msum /= num;

    imageStore(current_mask, ivec3(gl_GlobalInvocationID.xyz), (msum > 0.5) ? mask_true : mask_false);

    if(touch_alpha)
    { //alpha will change to the average of the neighboring cells
      imageStore(current, ivec3(gl_GlobalInvocationID.xyz), csum);
    }
    else
    { //don't touch alpha, get the value from pcol
      imageStore(current, ivec3(gl_GlobalInvocationID.xyz), vec4(csum.rgb, pcol.a));
    }
  }
}
