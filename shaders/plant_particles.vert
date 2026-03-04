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
uniform mat4 model; // uses the transform model matrix
uniform mat4 MVP;

// for random value generation per vertex
float hash(float n)
{
    return fract(sin(n) * 43758.5453123);
}

void main() {

    // Animate position using normal and time
	float speed = 0.05;

    float ext = mod(t + 4 * phase, 4.0); // Replay every 4 seconds
    vec3 start = speed * 4 * normal * ext;

    start.y -= ext * 0.5;   // cause the particles to go downwards based on the time-interval

    // Using the hash function and the vertex ids, we'll provide random pos offsets
    vec3 randomOffset = vec3(
        (hash(id + 1.0) - 0.5) * 0.5,
        (hash(id + 2.0) - 0.5) * 0.5,
        (hash(id + 3.0) - 0.5) * 0.5
    );

    // have a random spiral scale using a hash and the particle id to provide some randomness to the spirals
    float spiralScale = 0.5 + hash(id) * 0.5;
    vec3 spiralOffset = vec3(
        cos(ext + phase * 6.28) * spiralScale,  // 6.28 = 2PI
        sin(ext + phase * 6.28) * spiralScale * 0.5,
        sin(ext + phase * 6.28) * spiralScale
    );

    vec3 local = (start + randomOffset + spiralOffset);
    vec4 world = model * vec4(local, 1.0);
	
	vs_out.phase = phase;
	vs_out.id = id; // passing through vertex phase, id to later in the pipeline

	gl_Position = MVP * world;
}