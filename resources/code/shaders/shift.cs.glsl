#version 430

//note that this only effects what the parameters to glDispatchCompute are - by using gl_GlobalInvocationID, you don't need to worry what any of those numbers are
layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;    //specifies the workgroup size

uniform layout(rgba8) image3D previous;       //now-current values of the block
uniform layout(r8) image3D previous_mask;  //now-current values of the mask

uniform layout(rgba8) image3D current;        //values of the block after the update
uniform layout(r8) image3D current_mask;   //values of the mask after the update

uniform ivec3 movement;     //how much are you moving this current cell by? 
uniform bool loop;          //does the data loop off the sides (toroid style)
uniform int mode;           //will you respect the current value of the mask? this could get ambiguous but we'll deal

vec4 mask_true = vec4(1.0,0.0,0.0,0.0);
vec4 mask_false = vec4(0.0,0.0,0.0,0.0);

void main()
{
    ivec3 regular_pos = ivec3(gl_GlobalInvocationID.xyz);
    ivec3 shifted_pos = regular_pos - movement;

    ivec3 image_size = imageSize(current);

    if(loop)
    {
        shifted_pos.x = shifted_pos.x % image_size.x;
        shifted_pos.y = shifted_pos.y % image_size.y;
        shifted_pos.z = shifted_pos.z % image_size.z;
    }

  bool pmask = (imageLoad(previous_mask, regular_pos).r > 0.5);  //existing mask value (previous_mask = 0?)
  bool psmask = (imageLoad(previous_mask, shifted_pos).r > 0.5);  //existing mask value (shifted)
  vec4 pcol = imageLoad(previous, regular_pos);    //existing color value (what is the previous color?)
  vec4 pscol = imageLoad(previous, shifted_pos);   //existing color value (shifted)

    if(mode == 1)       //ignore mask buffer, move color data only (current_mask takes value of previous_mask)
    {
        //do the color shift
        imageStore(current, regular_pos, pscol);
        //write the same value of mask back to current_mask
        imageStore(current_mask, regular_pos, pmask?mask_true:mask_false);
    }
    else if(mode == 2)  //respect mask buffer, if pmask is true, current takes value of previous, if false, do the shift
    {
        //is the cell masked?
        if(pmask)
        {
            //if yes, write previous color and mask_true
            imageStore(current, regular_pos, pcol);
            imageStore(current_mask, regular_pos, mask_true);
        }
        else
        {
            //if no, write shifted color, and mask_false 
            imageStore(current, regular_pos, pscol);
            imageStore(current_mask, regular_pos, mask_false);
        }
    }
    else if(mode == 3)  //carry mask buffer, mask comes along for the ride with the color values
    {
        //do the color shift
        imageStore(current, regular_pos, pscol);
        //do the mask shift
        imageStore(current_mask, regular_pos, psmask?mask_true:mask_false);
    }
}
