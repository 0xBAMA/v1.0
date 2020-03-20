#version 450

in vec4 v_color;
out vec4 fragment_output;

void main()
{
  int fcymod3 = int(gl_FragCoord.y) % 3;

  if(fcymod3 == 0)
    discard;
  else
    fragment_output = v_color;
}
