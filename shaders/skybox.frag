#version 150

uniform sampler2D texture0;
in vec2 vTexcoord;

out vec4 FragColor;

void main()
{
    vec4 color = texture(texture0, vTexcoord);
    FragColor = color;
}