#version 150

uniform sampler2D sceneTex;        // normalized sampler2D
uniform vec2 u_resolution;         // width, height in pixels
uniform vec2 u_center;             // center in pixel coords
uniform float u_radius;
uniform float u_level;

in vec2 texcoordVarying;
out vec4 outputColor;

void main()
{
    // normalized UV
    vec2 uv = vec2(gl_FragCoord.x / u_resolution.x, gl_FragCoord.y / u_resolution.y);

    // sample scene texture
    vec4 sceneColor = texture(sceneTex, uv);

    // distance from center
    float dx = gl_FragCoord.x - u_center.x;
    float dy = gl_FragCoord.y - u_center.y;
    float d = sqrt(dx*dx + dy*dy);

    float edgeFeather = 400.0;
	float baseAlpha = smoothstep(u_radius - edgeFeather, u_radius + edgeFeather, d);
	float finalAlpha = baseAlpha * u_level;

    vec4 tanColor = vec4(0.75, 0.52, 0.21, 1.0);
	outputColor = mix(sceneColor, tanColor, finalAlpha);
}
