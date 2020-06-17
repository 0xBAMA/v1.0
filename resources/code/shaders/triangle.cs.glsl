#version 430

//note that this only effects what the parameters to glDispatchCompute are - by using gl_GlobalInvocationID, you don't need to worry what any of those numbers are
layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;    //specifies the workgroup size

uniform layout(rgba8) image3D previous;       //now-current values of the block
uniform layout(r8) image3D previous_mask;  //now-current values of the mask

uniform layout(rgba8) image3D current;        //values of the block after the update
uniform layout(r8) image3D current_mask;   //values of the mask after the update

uniform vec3 point1;        //where are the three points of the triangle?
uniform vec3 point2;
uniform vec3 point3;


uniform float thickness;      //what is the thickness of this triangle?
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
  //code to see if gl_GlobalInvocationID.xyz is inside the shape

  bool draw_triangle = false;

  //calculate the center of the triangle
  vec3 calculated_triangle_center = ( point1 + point2 + point3 ) / 3.0f;


  //calculate the top normal vector of the triangle

  //    ^  < - - -normal
  //		|
  //		|
  //	1 .______. 2
  //		\							taking the cross product of the two sides (1-2 and 1-3)
  //		 \							will give either the normal or the inverse of the normal
  //      \								check this against the center point of the triangle to determine
  //			 * 3							and invert it if neccesary (depending on the relative positions
  //													of these three points)

  vec3 calculated_top_normal = normalize( cross( point1 - point2, point1 - point3 ) );
  calculated_top_normal = planetest( point1 + thickness * calculated_top_normal, calculated_top_normal, calculated_triangle_center ) ? calculated_top_normal : ( calculated_top_normal * -1.0f );

  //calculate the side normal vectors

  //			   ^
  //			   |  < - - top normal
  //       _________
  //      |\       /| ^
  //      | \ top / |	| thickness
  //			|  \   /  | v
  //      \   \ /  /
  //       \   |  /
  //        \  | /
  //         \ |/
  //          \/
  //
  //	looking at this from one of the edges:
  //
  //   ^
  //   | < - - - - the triangle's top normal
  //   *-------> < - - - vector representing the side being considered
  //
  //   take the cross product of these two vectors, then do a similar test involving the center point of the triangle to invert it if neccesary

  vec3 calculated_side_1_2_normal = normalize( cross( calculated_top_normal, point2 - point1 ) );
  calculated_side_1_2_normal = planetest( point1, calculated_side_1_2_normal, calculated_triangle_center) ? calculated_side_1_2_normal : ( calculated_side_1_2_normal * -1.0f );

  vec3 calculated_side_2_3_normal = normalize( cross( calculated_top_normal, point3 - point2 ) );
  calculated_side_2_3_normal = planetest( point2, calculated_side_2_3_normal, calculated_triangle_center) ? calculated_side_2_3_normal : ( calculated_side_2_3_normal * -1.0f );

  vec3 calculated_side_3_1_normal = normalize( cross( calculated_top_normal, point1 - point3 ) );
  calculated_side_3_1_normal = planetest( point3, calculated_side_3_1_normal, calculated_triangle_center) ? calculated_side_3_1_normal : ( calculated_side_3_1_normal * -1.0f );


  // do the tests - for each of the normals, top, bottom, and the three sides,
  //	use the planetest function to determine whether the current point is
  //	'below' all 5 planes - if it is, it is inside this triangular prism


  draw_triangle = planetest( point1 + ( thickness / 2.0f ) * calculated_top_normal, calculated_top_normal, gl_GlobalInvocationID.xyz ) &&
  planetest( point1 - ( thickness / 2.0f ) * calculated_top_normal, -1.0f * calculated_top_normal, gl_GlobalInvocationID.xyz ) &&
  planetest( point1, calculated_side_1_2_normal, gl_GlobalInvocationID.xyz ) &&
  planetest( point2, calculated_side_2_3_normal, gl_GlobalInvocationID.xyz ) &&
  planetest( point3, calculated_side_3_1_normal, gl_GlobalInvocationID.xyz );

  if(draw_triangle)
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
