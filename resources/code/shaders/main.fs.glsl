#version 450

in vec4 v_color;
out vec4 fragment_output;

uniform int x_resolution;    //width, in pixels
uniform int y_resolution;   //height, in pixels

uniform float theta;      //rotation about the y axis
uniform float phi;       //rotation up from the xz plane

void main()
{
  //start with a unit vector pointing down the z axis
  vec3 org = vec3(0,0,2); //will have to compute the offsets

  //rotate that vector 'up' by phi

  //rotate about the y axis by theta

  //does a ray with that origin and that direction hit the cube?

  //if yes, trace the ray

  //if no, discard the fragment

  fragment_output = v_color;
}
