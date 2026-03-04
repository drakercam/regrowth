#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform float pSize;

in VS_OUT {
    float phase;
    float id;
} gs_in[];

out GS_OUT {
    vec2 uv;
    float phase;
    float id;
} gs_out;

void main() {

    vec4 center = gl_in[0].gl_Position;

    vec2 offsets[4] = vec2[](
        vec2(-pSize, -pSize),
        vec2(pSize, -pSize),
        vec2(-pSize,  pSize),
        vec2(pSize,  pSize)
    );

    vec2 uv_set[4] = vec2[](
        vec2(0,0),
        vec2(1,0),
        vec2(0,1),
        vec2(1,1)
    );

    for (int i = 0; i < 4; i++) {
        gl_Position = center + vec4(offsets[i], 0, 0);
        gs_out.uv = uv_set[i];
        gs_out.phase = gs_in[0].phase;
        gs_out.id = gs_in[0].id;
        EmitVertex();
    }
    EndPrimitive();
}
