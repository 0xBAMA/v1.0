#version 430

//note that this only effects what the parameters to glDispatchCompute are - by using gl_GlobalInvocationID, you don't need to worry what any of those numbers are
layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;    //specifies the workgroup size

uniform layout(rgba8) image3D previous;       //now-current values of the block
uniform layout(r8) image3D previous_mask;  //now-current values of the mask

uniform layout(rgba8) image3D current;        //values of the block after the update
uniform layout(r8) image3D current_mask;   //values of the mask after the update

uniform ivec3 spacing;  //distance between gridlines, xyz
uniform ivec3 offsets;  //distance between gridlines, xyz
uniform ivec3 width;    //width of grid lines, xyz
uniform vec4 color;     //what color should it be drawn with?

uniform bool draw;      //should this shape be drawn?
uniform bool mask;      //this this shape be masked?

bool in_shape()
{
  bool x = ((gl_GlobalInvocationID.x + offsets.x) % spacing.x) <= width.x; 
  bool y = ((gl_GlobalInvocationID.y + offsets.y) % spacing.y) <= width.y; 
  bool z = ((gl_GlobalInvocationID.z + offsets.z) % spacing.z) <= width.z; 

  return ((x && y) || (x && z) || (y && z));
}

vec4 mask_true = vec4(1.0,0.0,0.0,0.0);
vec4 mask_false = vec4(0.0,0.0,0.0,0.0);

void main()
{
  bool pmask = (imageLoad(previous_mask, ivec3(gl_GlobalInvocationID.xyz)).r > 0.5);  //existing mask value (previous_mask = 0?)
  vec4 pcol = imageLoad(previous, ivec3(gl_GlobalInvocationID.xyz));                 //existing color value (what is the previous color?)

  if(pmask) //the cell was masked
  {
    imageStore(current, ivec3(gl_GlobalInvocationID.xyz), pcol);  //color takes on previous color
    imageStore(current_mask, ivec3(gl_GlobalInvocationID.xyz), mask_true);  //mask is set true
  }
  else if(!in_shape())  //the cell was not masked, but is outside the shape
  {
    imageStore(current, ivec3(gl_GlobalInvocationID.xyz), pcol);  //color takes previous color
    imageStore(current_mask, ivec3(gl_GlobalInvocationID.xyz), mask_false); //mask is set false
  }
  else  //the cell was not masked, and is inside the shape
  {
    if(mask)  //uniform value telling whether or not to mask
      imageStore(current_mask, ivec3(gl_GlobalInvocationID.xyz), mask_true);
    else
      imageStore(current_mask, ivec3(gl_GlobalInvocationID.xyz), mask_false);

    if(draw)  //uniform value telling whether or not to draw
      imageStore(current, ivec3(gl_GlobalInvocationID.xyz), color); //uniform color
    else
      imageStore(current, ivec3(gl_GlobalInvocationID.xyz), pcol);  //previous color
  }
}
