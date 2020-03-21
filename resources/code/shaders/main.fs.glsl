#version 450

in vec4 v_color;
out vec4 fragment_output;

uniform int x_resolution;    //width, in pixels
uniform int y_resolution;   //height, in pixels

uniform float utheta;     //rotation about the y axis
uniform float uphi;      //rotation up from the xz plane

//thanks to Neil Mendoza via http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/
mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
  float xoff = ((gl_FragCoord.x/x_resolution) - 0.5f);  //going to need to scale this, probably
  float yoff = ((gl_FragCoord.y/y_resolution) - 0.5f);  //same here

  //start with a vector pointing down the z axis (greater than half the corner to corner distance, i.e. > ~1.75)
  vec4 org = vec4(xoff, yoff,  2,  0); //add the offsets in x and y
  vec4 dir = vec4(   0,    0, -2,  0); //simply a vector pointing in the opposite direction, no xy offsets

  //rotate both vectors 'up' by phi, e.g. about the x axis
  mat4 rotphi = rotationMatrix(vec3(1,0,0), uphi);
  org *= rotphi;
  dir *= rotphi;

  //rotate both about the y axis by theta
  mat4 rottheta = rotationMatrix(vec3(0,1,0), utheta);
  org *= rottheta;
  dir *= rottheta;

  //does a ray with that origin and that direction hit the cube?

  //if yes, trace the ray

  //if no, discard the fragment

  // fragment_output = v_color;
  fragment_output = vec4(abs(xoff),abs(yoff),0,1);
}
