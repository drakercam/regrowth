#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in float phase;
layout(location = 3) in float id;

out VS_OUT {
    float phase;
    float id;
} vs_out;

uniform float t;
uniform mat4 MVP;

void main() {
    float speed = 0.25;

    vec3 zeroposition = position;

    // total lifecycle of a particle
    float lifetime = 60.0; // in seconds
    float ext = mod(t + lifetime * phase, lifetime);

    // base downward movement
    vec3 animatedPos = zeroposition + normal * speed * 6.0 * ext;

    // add sway 
    float swayAmplitude = 1.0;
    float swaySpeed = 1.0;
    animatedPos.x += swayAmplitude * sin(swaySpeed * t + id);
    animatedPos.z += swayAmplitude * cos(swaySpeed * t + id);

    vec4 a_final = MVP * vec4(animatedPos, 1.0);

    vs_out.phase = phase;
    vs_out.id = id;

    gl_Position = a_final;
}
