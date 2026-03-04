#version 150

in vec4 position;
in vec2 texcoord;

uniform mat4 modelViewProjectionMatrix; // auto-set by oF

out vec2 uv0;

void main() {
    uv0 = texcoord;
    gl_Position = modelViewProjectionMatrix*position; // transformed vertex postion, as usual
}
