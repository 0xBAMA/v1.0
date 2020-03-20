#version 450

in vec4 v_color;
out vec4 fragment_output;

uniform int x_resolution;
uniform int y_resolution;

void main()
{
  // fragment_output = v_color;

  // float grey = max(gl_FragCoord.x/x_resolution, gl_FragCoord.y/y_resolution);

  // fragment_output = vec4(vec3(grey),1.0);

  float grey = distance(vec2(x_resolution/2, y_resolution/2), vec2(gl_FragCoord.x, gl_FragCoord.y))/600;

  fragment_output = vec4(vec3(grey),1.0);
}