#version 450

//note that this only effects what the parameters to glDispatchCompute are - by using gl_GlobalInvocationID, you don't need to worry what any of those numbers are
layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;    //specifies the workgroup size

uniform layout(rgba8) image3D previous;       //now-current values of the block
uniform layout(r8) image3D previous_mask;  //now-current values of the mask

uniform layout(rgba8) image3D current;        //values of the block after the update
uniform layout(r8) image3D current_mask;   //values of the mask after the update


uniform vec3 location;  //where is this sphere centered?
uniform float radius;   //what is the radius of this sphere?
uniform vec4 color;     //what color should it be drawn with?

uniform bool draw;      //should this shape be drawn?
uniform bool mask;      //this this shape be masked?

void main()
{
  float dist = distance(gl_GlobalInvocationID.xyz, vec3(127));
  bool mask = (imageLoad(previous_mask, ivec3(gl_GlobalInvocationID.xyz)).r > 0.5);   //existing mask value (previous_mask = 0?)
  vec4 pcol = imageLoad(previous, ivec3(gl_GlobalInvocationID.xyz));                 //existing color value (what is the previous color?)

  //there are three outcomes:

  // if mask is true, write color and mask into the current block's data - the drawing operation specified by this shader will not take place for this cell.
  // if mask is false, there are two possiblities - one, it is affected by the current drawing operation, or two, it is not.

  //  in the case that it is affected by the current drawing operation, write the uniform color value to current, and the uniform mask value to current_mask
  //  in the case it is not affected by the current drawing operation, write the color read into pcol to current, and a zero value will be written to current_mask


  //placeholder, garbage data
  if((int(dist) % 15 <= 5) && !mask)
  {
    imageStore(current, ivec3(gl_GlobalInvocationID.xyz), vec4(0.5));
  }
}
