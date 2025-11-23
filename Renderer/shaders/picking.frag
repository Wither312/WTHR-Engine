#version 460

uniform uint DrawIndex;
uniform uint ObjectIndex;

out uvec3 FragColor;

void main()
{
    FragColor = uvec3(ObjectIndex, DrawIndex, gl_PrimitiveID);
}