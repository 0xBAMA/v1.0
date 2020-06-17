#version 430

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in; //2d workgroup

uniform layout(rgba8) image3D current;
uniform layout(r8) image3D lighting;

uniform vec3 light_position;
uniform float light_intensity;

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

void main()
{
    //is this voxel close enough to consider?
        //equivalently, is (intensity/pow(distance(my_position, light_position)), 2) >= 1.0/256.0
        
    //if close enough, trace a ray from light_position to my_position, use same logic as in directional.cs.glsl 
}

