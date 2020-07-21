#version 430

//note that this only effects what the parameters to glDispatchCompute are - by using gl_GlobalInvocationID, you don't need to worry what any of those numbers are
layout(local_size_x = 8, local_size_y = 1, local_size_z = 8) in;    //specifies the workgroup size

uniform layout(rgba8) image3D current;        //values of the block after the update
uniform layout(r8) image3D lighting;        //values held in the lighting buffer


uniform int y_index;   // what plane are we in
uniform float scale_factor; // how much should you scale the hit cell's lighting by
uniform float alpha_thresh; // what is the minimum threshold considered a 'hit' when tracing the GI rays
uniform float sky_intensity; // if the ray escapes, how much light should it get?

  // This has a sequential dependence, handled by how the shader is invoked - we don't need to worry about it in the shader
  // Context, from the same guy who did the Voxel Automata Terrain, Brent Werness:
  //   "Totally faked the GI!  It just casts out 9 rays in upwards facing the lattice directions.
  //    If it escapes it gets light from the sky, otherwise it gets some fraction of the light
  //    from whatever cell it hits.  Run from top to bottom and you are set!"

void main()
{
    ivec3 my_loc = ivec3(gl_GlobalInvocationID.x, y_index, gl_GlobalInvocationID.z);

    vec4 prev = imageLoad(lighting, my_loc);    //existing color value (what is the color?)

    // for the 9 grid cells above the cell being considered
    // consider a vector between the current cell and the above grid cell
    // trace it out, through the current color buffer
    //   if you find a cell with alpha > alpha_thresh, you have hit something - take some portion of its light,
    //     specified by scale_factor

    //otherwise, if you trace it out to beyond the extent of the image dimensions, you have 'escaped', and take light from the sky

    imageStore(lighting, ivec3(gl_GlobalInvocationID.xyz), vec4(new));
}

