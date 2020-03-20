#version 450

in vec4 v_color;
out vec4 fragment_output;

uniform int x_resolution;
uniform int y_resolution;

void main()
{
  float grey = distance(vec2(x_resolution/2, y_resolution/2), vec2(gl_FragCoord.x, gl_FragCoord.y))/600;

  if(int(gl_FragCoord.x)%5==0||int(gl_FragCoord.y)%5==0)
    fragment_output = vec4(0,0,0,1);
  else if(int(gl_FragCoord.x)%2==0||int(gl_FragCoord.y)%2==0)
    fragment_output = vec4(vec3(grey),1.0);
  else
    discard;
}
