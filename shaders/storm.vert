#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in float phase;
layout(location = 3) in float id;

out VS_OUT {
    float phase;
    float id;
} vs_out;

uniform mat4 MVP;
uniform float t;
uniform float dt;

uniform float stormIntensity;
uniform vec3 stormCenter;
uniform float stormRadius;

void main() {

    vec3 p = position;

    float base = id * 0.123;

    // --- Turbulence ---
    vec3 turbulence;
    turbulence.x = sin(t * 14.7 + base * 5.0) * 40.0 + cos(t * 32.1 + base * 11.0) * 35.0;
    turbulence.y = sin(t * 25.9 + base * 17.0) * 20.0 + cos(t * 18.4 + base * 9.0) * 25.0;
    turbulence.z = cos(t * 21.3 + base * 13.0) * 40.0 - sin(t * 27.6 + base * 7.0) * 30.0;

    // --- Shock Impulses ---
    float shockNoise = fract(sin((t*8.0) + base) * 43758.5453);
    float shock = shockNoise > 0.8 ? 1.0 : 0.0;

    vec3 shockDir = normalize(vec3(
        fract(sin(id * 12.34) * 12345.678),
        fract(sin(id * 98.12) * 87654.321),
        fract(sin(id * 45.67) * 54321.987)
    ));

    vec3 impulse = shockDir * shock * 120.0;

    // --- Velocity ---
    vec3 v = (turbulence + impulse) * stormIntensity;

    p += v * dt;

    // --- Boundary ---
    vec3 d = p - stormCenter;
    float dist = length(d);
    if (dist > stormRadius) {
        p = stormCenter + normalize(-d) * (stormRadius * 0.9);
    }

    gl_Position = MVP * vec4(p, 1.0);

    vs_out.phase = phase;
    vs_out.id = id;
}
