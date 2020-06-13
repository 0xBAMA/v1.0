#version 450

in vec4 v_color;
out vec4 fragment_output;

uniform layout(rgba8) image3D current;
uniform layout(r8) image3D lighting;

uniform vec4 clear_color;     //bg color
uniform int x_resolution;    //width, in pixels
uniform int y_resolution;   //height, in pixels

uniform float utheta;     //rotation about the y axis
uniform float uphi;      //rotation up from the xz plane

uniform float scale;   //scales the xy offsets

//thanks to Neil Mendoza via http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/
mat3 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat3(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c);
}

double tmin, tmax; //global scope, set in hit() to tell min and max parameters

// #define NUM_STEPS 2000
//#define NUM_STEPS 165

#define NUM_STEPS 500
#define MIN_DISTANCE 0.0
#define MAX_DISTANCE 10.0

bool hit(vec3 org, vec3 dir)
{
  // hit() code adapted from:
  //
  //    Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
  //    "An Efficient and Robust Ray-Box Intersection Algorithm"
  //    Journal of graphics tools, 10(1):49-54, 2005

  //the bounding box
  vec3 min = vec3(-1,-1,-1);
  vec3 max = vec3(1,1,1);

  int sign[3];

  vec3 inv_direction = vec3(1/dir.x, 1/dir.y, 1/dir.z);

  sign[0] = (inv_direction[0] < 0)?1:0;
  sign[1] = (inv_direction[1] < 0)?1:0;
  sign[2] = (inv_direction[2] < 0)?1:0;

  vec3 bbox[2] = {min,max};

  tmin = (bbox[sign[0]][0] - org[0]) * inv_direction[0];
  tmax = (bbox[1-sign[0]][0] - org[0]) * inv_direction[0];

  double tymin = (bbox[sign[1]][1] - org[1]) * inv_direction[1];
  double tymax = (bbox[1-sign[1]][1] - org[1]) * inv_direction[1];

  if ( (tmin > tymax) || (tymin > tmax) )
    return false;
  if (tymin > tmin)
    tmin = tymin;
  if (tymax < tmax)
    tmax = tymax;

  double tzmin = (bbox[sign[2]][2] - org[2]) * inv_direction[2];
  double tzmax = (bbox[1-sign[2]][2] - org[2]) * inv_direction[2];

  if ( (tmin > tzmax) || (tzmin > tmax) )
    return false;
  if (tzmin > tmin)
    tmin = tzmin;
  if (tzmax < tmax)
    tmax = tzmax;
  return ( (tmin < MAX_DISTANCE) && (tmax > MIN_DISTANCE) );

  return true;
}

vec4 get_color_for_pixel(vec3 org, vec3 dir)
{
  float current_t = float(tmax);
  // vec4 t_color = vec4(0);
  vec4 t_color = clear_color;

  float step = float((tmax-tmin))/NUM_STEPS;
  if(step < 0.001f)
    step = 0.001f;

  ivec3 samp = ivec3((vec3(imageSize(current))/2.0f)*(org+current_t*dir+vec3(1)));

  vec4 new_read = imageLoad(current,samp);
  vec4 new_light_read = imageLoad(lighting,samp);

  for(int i = 0; i < NUM_STEPS; i++)
  {
    if(current_t>=tmin)
    {
      current_t -= step;
      samp = ivec3((vec3(imageSize(current))/2.0f)*(org+current_t*dir+vec3(1)));

      new_read = imageLoad(current,samp);
      new_light_read = imageLoad(lighting,samp);


      new_read.rgb *= (5*new_light_read.r);
          
          
      // it's a over b, where a is the new sample and b is the current color, t_color
      t_color.rgb = new_read.rgb * new_read.a + t_color.rgb * t_color.a * ( 1 - new_read.a );
      t_color.a = new_read.a + t_color.a * ( 1 - new_read.a );
    }
    else
    {

    }
  }

  return t_color;
}


void main()
{
  float xoff = scale*(((gl_FragCoord.x + gl_SamplePosition.x)/float(x_resolution)) - 0.5f);
  float yoff = scale*(((gl_FragCoord.y + gl_SamplePosition.y)/float(y_resolution)) - 0.5f)*(float(y_resolution)/float(x_resolution));

  //start with a vector pointing down the z axis (greater than half the corner to corner distance, i.e. > ~1.75)
  vec3 org = vec3(xoff, yoff,  2); //add the offsets in x and y
  vec3 dir = vec3(   0,    0, -2); //simply a vector pointing in the opposite direction, no xy offsets

  //rotate both vectors 'up' by phi, e.g. about the x axis
  mat3 rotphi = rotationMatrix(vec3(1,0,0), uphi);
  org *= rotphi;
  dir *= rotphi;

  //rotate both about the y axis by theta
  mat3 rottheta = rotationMatrix(vec3(0,1,0), utheta);
  org *= rottheta;
  dir *= rottheta;

  //does a ray with that origin and that direction hit the cube?
  if(hit(org,dir))
  { //if yes, trace the ray

    //this colors based on screen position
    // fragment_output = vec4(abs(xoff),abs(yoff),0,1) * float(1.75f-distance((org+tmin*dir),org));

    //this colors based on a texture read at tmin
    // ivec3 sample_location = ivec3((org+tmin*dir+vec3(1))*127.5);
    // fragment_output = imageLoad(current,sample_location) * float(1.75f-distance((org+tmin*dir),org));

    //this does the full raycasting operation, with alpha blending
    fragment_output = get_color_for_pixel(org, dir);

  }
  else
  { //if no, discard the fragment
    discard;
  }
}
