OF_GLSL_SHADER_HEADER

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float noiseScale;       // How stretched the noise is
uniform float noiseAmplitude;   // Height Multiplier
uniform vec2 noiseOffset;       // Scrolling or moving terrain
uniform int noiseOctaves;       // Number of FBM layers

in vec3 position;
in vec2 texcoord;
in vec3 normal;

out vec3 vWorldPos;
out vec3 vNormal;

// Hash-based pseudorandom gradient generation
vec2 RandomGradient(int ix, int iy)
{
    uint w = 32u;
    uint s = w / 2u;

    uint a = uint(ix);
    uint b = uint(iy);

    a *= 3284157443u;
    b ^= (a << s) | (a >> (w - s));
    b *= 1911520717u;
    a ^= (b << s) | (b >> (w - s));
    a *= 2048419235u;

    float random = float(a) * (3.14159265 / float(0xffffffffu));
    return vec2(sin(random), cos(random));
}

// Dot product of distance and gradient
float DotGridGradient(int ix, int iy, float x, float y)
{
    vec2 gradient = RandomGradient(ix, iy);
    vec2 distance = vec2(x - float(ix), y - float(iy));
    return dot(distance, gradient);
}

// cubic interpolation curve
float Interpolate(float a0, float a1, float w)
{
    return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}

float Perlin(float x, float y)
{
    int x0 = int(floor(x));
    int y0 = int(floor(y));
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float wx = x - float(x0);
    float wy = y - float(y0);

    float n0 = DotGridGradient(x0, y0, x, y);
    float n1 = DotGridGradient(x1, y0, x, y);
    float ix0 = Interpolate(n0, n1, wx);

    n0 = DotGridGradient(x0, y1, x, y);
    n1 = DotGridGradient(x1, y1, x, y);
    float ix1 = Interpolate(n0, n1, wx);

    float value = Interpolate(ix0, ix1, wy);
    return value;
}

// Multi-Octave Noise
float FBM(float x, float y, int octaves)
{
    float total = 0.0;
    float freq = 1.0;
    float amp = 1.0;

    for (int i = 0; i < octaves; ++i)
    {
        total += Perlin(x * freq, y * freq) * amp;
        freq *= 2.0;
        amp *= 0.5;
    }

    return total;
}

// Displaced position function
vec3 GetDisplacedPosition(vec3 pos) {
    float h = FBM(pos.x * noiseScale + noiseOffset.x, pos.y * noiseScale + noiseOffset.y, noiseOctaves);
    float height = h * noiseAmplitude;
    return vec3(pos.x, pos.y, height);
}

void main()
{
    vec3 p = position;
    vec3 px = GetDisplacedPosition(vec3(p.x + 0.01, p.y, p.z));
    vec3 py = GetDisplacedPosition(vec3(p.x, p.y + 0.01, p.z));
    vec3 displacePos = GetDisplacedPosition(p);

    vec3 tangentX = px - displacePos;
    vec3 tangentY = py - displacePos;
    vec3 displacedNormal = normalize(cross(tangentX, tangentY));

    vec4 worldPos = model * vec4(displacePos, 1.0);
    
    vWorldPos = worldPos.xyz;

    vNormal = normalize((model * vec4(displacedNormal, 0.0)).xyz);

    gl_Position = projection * view * worldPos;
}