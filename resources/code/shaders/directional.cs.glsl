#version 450

//this is heavily influenced by the display funciton (see main.fs.glsl)

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in; //2d workgroup

uniform layout(rgba8) image3D current;
uniform layout(r8) image3D lighting;

uniform float utheta;
uniform float uphi;

void main()
{

}

