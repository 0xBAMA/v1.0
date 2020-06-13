#version 450

//note that this only effects what the parameters to glDispatchCompute are - by using gl_GlobalInvocationID, you don't need to worry what any of those numbers are
layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;    //specifies the workgroup size

uniform layout(rgba8) image3D current;        //values of the block after the update
uniform layout(r8) image3D lighting;        //values held in the lighting buffer

uniform int radius; //how big a neighborhood should be considered for neighborhood occupancy?

vec4 mask_true = vec4(1.0,0.0,0.0,0.0);
vec4 mask_false = vec4(0.0,0.0,0.0,0.0);

void main()
{
  vec4 col = imageLoad(current, ivec3(gl_GlobalInvocationID.xyz));    //existing color value (what is the color?)

  //int num = 0;
  //vec4 csum = vec4(0,0,0,0);
  //float msum = 0.0;
  //for(int x = (-1 * radius); x <= radius; x++)
  //{
  //for(int y = (-1 * radius); y <= radius; y++)
  //{
  //for(int z = (-1 * radius); z <= radius; z++)
  //{
  //csum += imageLoad(previous, ivec3(gl_GlobalInvocationID.xyz) + ivec3(x,y,z));
  //msum += (imageLoad(previous_mask, ivec3(gl_GlobalInvocationID.xyz) + ivec3(x,y,z)).r > 0.5) ? 1.0 : 0.0;
  //num++;
  //}
  //}
  //}

    float alpha_sum = 0.0f;

    for(int x = -1; x <= 1; x++)
        for(int y = -1; y <= 1; y++)
            for(int z = -1; z <= 1; z++)
            {
                alpha_sum += imageLoad(previous, ivec3(gl_GlobalInvocationID.xyz)+ivec3(x,y,z)).a;
            }

    //sum up alpha values across the 27 cells being considered
    //compute a ratio of that alpha sum over the number of cells considered
    //scale the rgb components of pcol by this ratio





  imageStore(current_mask, ivec3(gl_GlobalInvocationID.xyz), pmask);
  imageStore(current, ivec3(gl_GlobalInvocationID.xyz), vec4(alpha_sum/27);
}
