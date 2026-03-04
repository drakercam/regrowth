#version 150

in vec2 uv0;

uniform sampler2D tex0;
uniform float timer;
uniform float staticAmount; // 0 = off, 1 = full strength
uniform vec2 screenSize;

out vec4 fragColor;

// hash-based pseudo-random generator
float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    vec4 scene = texture(tex0, uv0);

    // static noise
	float noise = rand(uv0 * 800.0 + timer * 5.0);

    // track distance to nearest screen edge
    float left   = uv0.x;
    float right  = 1.0 - uv0.x;
    float top    = uv0.y;
    float bottom = 1.0 - uv0.y;

    float edgeDistance = min(min(left, right), min(top, bottom));

    // make the static thicker on the outside and thinner on the inside
    float thickness = 0.6;
    float falloff = 1.0 - smoothstep(0.0, thickness, edgeDistance);
	
	// darken the edges of the screen
	float darkStrength = 0.8;
	float darkFactor = 1.0 - (falloff * darkStrength * staticAmount);

	scene.rgb *= darkFactor;

	// add static effect
	float noiseAmount = noise * falloff * staticAmount;
    vec3 staticColor = vec3(noiseAmount);

    // make it see-through background
    float alpha = noiseAmount * 0.6;  // adjust 0.6 to taste (0–1)
    
    vec3 finalColor = mix(scene.rgb, staticColor, alpha);
	
    fragColor = vec4(finalColor, 1.0);
}
