#version 450
layout (location = 0) in vec4 i_color;
layout (location = 1) in vec2 i_position;

out vec4 v_color;
out vec3 position;

void main()
{
    v_color = i_color;
    position = vec3( i_position, 0.0);
}
