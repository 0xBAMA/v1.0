#version 430

//note that this only effects what the parameters to glDispatchCompute are - by using gl_GlobalInvocationID, you don't need to worry what any of those numbers are
layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;    //specifies the workgroup size

uniform layout(rgba8) image3D previous;       //now-current values of the block
uniform layout(r8) image3D previous_mask;  //now-current values of the mask

uniform layout(rgba8) image3D current;        //values of the block after the update
uniform layout(r8) image3D current_mask;   //values of the mask after the update

uniform vec3 tvec;            //location of center of top
uniform vec3 bvec;            //location of center of bottom
uniform float iradius;        //inside radius
uniform float oradius;        //outside radius

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

  vec3 cylinder_center = ( bvec + tvec ) / 2.0f;

  vec3 cylinder_tvec_normal = bvec - tvec;
  cylinder_tvec_normal = planetest( tvec, cylinder_tvec_normal, cylinder_center) ? cylinder_tvec_normal : (cylinder_tvec_normal * -1.0f);

  vec3 cylinder_bvec_normal = bvec - tvec;
  cylinder_bvec_normal = planetest( bvec, cylinder_bvec_normal, cylinder_center) ? cylinder_bvec_normal : (cylinder_bvec_normal * -1.0f);


  if( planetest(bvec, cylinder_bvec_normal, gl_GlobalInvocationID.xyz) && planetest(tvec, cylinder_tvec_normal, gl_GlobalInvocationID.xyz) )
  {
    float len = length( cross( tvec - bvec, bvec - gl_GlobalInvocationID.xyz ) ) / length( tvec - bvec );
    if(len < oradius && len > iradius)
    {
      //distance from point to line from http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
      return true;
    }
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
