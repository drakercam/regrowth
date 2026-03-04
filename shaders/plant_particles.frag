#version 330
in GS_OUT {
    vec2 uv;
    float phase;
    float id;
} myfrag;

out vec4 outputColor;

uniform float t;
uniform sampler2D texture0;

void main()
{   

    float fade = 1.0-0.25*mod(t+4*myfrag.phase,4.0);

    vec2 uv = myfrag.uv;

    outputColor = texture(texture0,uv/4); // why divide uv by 4?
    outputColor.a = fade;
}
