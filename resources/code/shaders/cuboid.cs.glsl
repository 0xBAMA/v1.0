#version 430

//note that this only effects what the parameters to glDispatchCompute are - by using gl_GlobalInvocationID, you don't need to worry what any of those numbers are
layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;    //specifies the workgroup size

uniform layout(rgba8) image3D previous;       //now-current values of the block
uniform layout(r8) image3D previous_mask;  //now-current values of the mask

uniform layout(rgba8) image3D current;        //values of the block after the update
uniform layout(r8) image3D current_mask;   //values of the mask after the update

uniform vec3 a;             //8 points defining shape
uniform vec3 b;
uniform vec3 c;
uniform vec3 d;
uniform vec3 e;
uniform vec3 f;
uniform vec3 g;
uniform vec3 h;

uniform vec4 color;           //what color should it be drawn with?

uniform bool draw;      //should this shape be drawn?
uniform bool mask;      //this this shape be masked?


bool planetest(vec3 plane_point, vec3 plane_normal, vec3 test_point)
{
	// Determines whether a point is above or below a plane

  //		return false if the point is above the plane
	//		return true if the point is below the plane

	float result = 0.0;

	//equation of plane

	// a (x-x1) + b (y-y1) + c (z-z1) = 0

	float a = plane_normal.x;
	float b = plane_normal.y;
	float c = plane_normal.z;

	float x1 = plane_point.x;
	float y1 = plane_point.y;
	float z1 = plane_point.z;

	float x = test_point.x;
	float y = test_point.y;
	float z = test_point.z;

	result = a * (x - x1) + b * (y - y1) + c * (z - z1);

	return (result < 0) ? true : false;

}


bool in_shape()
{
  //QUADRILATERAL HEXAHEDRON (CUBOID)

	// 	point location reference - it can be rotated, but the faces must be
	//		ACEG, CGDH, EGFH, ABEF, ABCD and BDFH - axes just for reference
	//
	// 	   e-------g    +y
	// 	  /|      /|		 |
	// 	 / |     / |     |___+x
	// 	a-------c  |    /
	// 	|  f----|--h   +z
	// 	| /     | /
	//  |/      |/
	// 	b-------d

	//		the four points making up each of the 6 faces must be coplanar - if not,
	// 			the shape will not come out as intended (there would be two potential
	//			planes for each face, and only one of them is used to represent that face)

	//		that being said, there is still a degree of freedom allowing a lot of
	//			potential for non-cube shapes, making use of trapezoidal or
	//			rhombus-shaped faces which need not be parallel to one another.

	//		the algorithm is very similar to the one used for the triangle - I have
	//			seen it said that it can be generalized to any convex polyhedron -
	//			concave shapes do not work as there are areas that should be within the
	//			shape that will not pass all the requisite plane tests, which will
	//			exclude some of the area that should lie within the shape



  // note (3/30/2020): this function is only really being included for legacy purposes
  //    as it is very difficult to configure and use in any real way. There is a requirement
  //    that the four points defining any given face are coplanar - this is not easy to
  //    enforce when you're laying it out by hand. While it is very flexible, as a primitive,
  //    there is very little value in it due to how hard it is to use, and how easy it is
  //    to have the case where the four points on a face are not coplanar.

  vec3 quad_hex_center;

  vec3 quad_hex_top_normal;
  vec3 quad_hex_bottom_normal;
  vec3 quad_hex_left_normal;
  vec3 quad_hex_right_normal;
  vec3 quad_hex_front_normal;
  vec3 quad_hex_back_normal;

  bool draw_cuboid;

  quad_hex_center = (a + b + c + d + e + f + g + h) / 8.0f;

  // "TOP" (ACEG) - using ACE
  quad_hex_top_normal = normalize( cross( a - c, e - c ) ); //compute a normal cantidate
  quad_hex_top_normal = planetest( a, quad_hex_top_normal, quad_hex_center) ? quad_hex_top_normal : ( quad_hex_top_normal * -1.0f ); //valid if center is below plane, else invert normal

  // "BOTTOM" (BDFH) - using BFD
  quad_hex_bottom_normal = normalize( cross( b - f, d - f ) );
  quad_hex_bottom_normal = planetest( b, quad_hex_bottom_normal, quad_hex_center) ? quad_hex_bottom_normal : ( quad_hex_bottom_normal * -1.0f );

  // "LEFT" (ABEF) - using FEA
  quad_hex_left_normal = normalize( cross( f - e, a - e ) );
  quad_hex_left_normal = planetest( f, quad_hex_left_normal, quad_hex_center) ? quad_hex_left_normal : ( quad_hex_left_normal * -1.0f );

  // "RIGHT" (CDGH) - using CGH
  quad_hex_right_normal = normalize( cross( c - g, h - g ) );
  quad_hex_right_normal = planetest( c, quad_hex_right_normal, quad_hex_center) ? quad_hex_right_normal : ( quad_hex_right_normal * -1.0f );

  // "FRONT" (ABCD) - using ABD
  quad_hex_front_normal = normalize( cross( a - b, d - b ) );
  quad_hex_front_normal = planetest( a, quad_hex_front_normal, quad_hex_center) ? quad_hex_front_normal : ( quad_hex_front_normal * -1.0f );

  // "BACK" (EFGH) - using GHF
  quad_hex_back_normal = normalize( cross( g - h, f - h ) );
  quad_hex_back_normal = planetest( g, quad_hex_back_normal, quad_hex_center) ? quad_hex_back_normal : ( quad_hex_back_normal * -1.0f );


  draw_cuboid =  planetest(a, quad_hex_top_normal, gl_GlobalInvocationID.xyz) &&
                planetest( b, quad_hex_bottom_normal, gl_GlobalInvocationID.xyz) &&
                planetest( f, quad_hex_left_normal, gl_GlobalInvocationID.xyz) &&
                planetest( c, quad_hex_right_normal, gl_GlobalInvocationID.xyz) &&
                planetest( a, quad_hex_front_normal, gl_GlobalInvocationID.xyz) &&
                planetest( g, quad_hex_back_normal, gl_GlobalInvocationID.xyz);


  if(draw_cuboid)
  {
    return true;
  }

  return false;
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
