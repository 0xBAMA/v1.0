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
    ivec3 check_loc = ivec3(0);

    vec4 prev_light_val = imageLoad(lighting, my_loc);    //existing light value (how much light, stored in r)
    vec4 prev_color_val = imageLoad(current, my_loc);    //existing color value (what is the color?)

    float new_light_val = prev_light_val.r;

    ivec3 imsize = imageSize(lighting);

    if(prev_color_val.a >= alpha_thresh) // this cell is opaque enough to participate
    {
        for(int dx = -1; dx <= 1; dx++)
        {
            for(int dz = -1; dz <= 1; dz++)
            {
                check_loc = ivec3(my_loc.x + dx, my_loc.y + 1, my_loc.z + dz);
                bool hit = false;

                while(check_loc.x >= 0 && check_loc.x < imsize.x && check_loc.z >= 0 && check_loc.z < imsize.z && check_loc.y < imsize.y)
                {
                    if(imageLoad(current, check_loc).a >= alpha_thresh)
                    {
                        // take some portion (determined by scale_factor) of the light from that location
                        new_light_val = new_light_val + imageLoad(lighting, check_loc).r * scale_factor;

                        hit = true;
                        break;
                    }
                    check_loc.x += dx;
                    check_loc.y += 1;
                    check_loc.z += dz;
                }

                if(!hit)  // ray escaped the volume, use sky_intensity instead
                {
                    // take the light from the sky
                    new_light_val = prev_light_val.r + sky_intensity; // should this take scale_factor as well?
                }
            }
        }

        imageStore(lighting, my_loc, vec4(new_light_val));
    }
}
