OF_GLSL_SHADER_HEADER

#ifndef PERLIN_GLSL
#define PERLIN_GLSL

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

#endif