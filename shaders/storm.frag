#version 330

in GS_OUT {
    vec2 uv;
    float phase;
    float id;
} fs;

out vec4 outputColor;

uniform sampler2D texture0;
uniform float t;

void main() {

    vec4 col = texture(texture0, fs.uv);

    // flicker like storm lightning
    float flicker = 0.8 + 0.2 * sin(t * 20.0 + fs.id);

    outputColor = vec4(col.rgb * flicker, col.a);
}
