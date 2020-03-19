#version 450
layout (location = 0) in vec3 vPosition;

out vec4 v_color;
out vec3 position;

void main()
{
    v_color = vec4( vPosition.x, vPosition.y, 0.0, 1.0);
    position = vPosition;
}
