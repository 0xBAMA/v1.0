#version 450
layout (location = 0) in vec3 vPosition;

// out vec4 gl_Position;
out vec4 v_color;

void main()
{
  v_color = vec4( vPosition.x, vPosition.y, 0.0, 1.0);
  gl_Position = vec4(vPosition,1.0);
}
