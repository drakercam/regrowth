#version 150

in vec4 position;
in vec2 texcoord;

out vec2 vTexcoord;

void main()
{
    vTexcoord = texcoord;
    gl_Position = position;
}
