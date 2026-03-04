#ifndef TERRAIN_H
#define TERRAIN_H

#include "MeshEntity.h"

class Terrain : public MeshEntity
{
    public:
        Terrain(const Transform& t, const ofShader& shader, int w, int h, int res)
        : MeshEntity(t, shader), width(w), height(h), res(res)
        {
            GeneratePlaneMesh();
        }

    protected:
        int width, height;
        int res;

        void GeneratePlaneMesh()
        {   
            ofPlanePrimitive plane;
            plane.set(width, height, res, res, OF_PRIMITIVE_TRIANGLES);
            SetMesh(plane.getMesh());
        }

        glm::vec2 RandomGradient(int ix, int iy)
        {
            uint32_t w = 32u;
            uint32_t s = w / 2u;
            uint32_t a = ix;
            uint32_t b = iy;

            a *= 3284157443u;
            b ^= (a << s) | (a >> (w - s));
            b *= 1911520717u;
            a ^= (b << s) | (b >> (w - s));
            a *= 2048419235u;

            float random = float(a) * (3.14159265f / float(0xffffffffu));
            return glm::vec2(sin(random), cos(random)); // just one dimension
        }
        
        float DotGridGradient(int ix, int iy, float x, float y)
        {
            glm::vec2 gradient = RandomGradient(ix, iy);
            glm::vec2 distance = glm::vec2(x - float(ix), y - float(iy));

            return glm::dot(distance, gradient);
        }

        float Interpolate(float a0, float a1, float w)
        {
            return (a1 - a0) * (3.0f - w * 2.0f) * w * w + a0;
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

            return Interpolate(ix0, ix1, wy);
        }

        float FBM(float x, float y, int octaves)
        {
            float total = 0.0f;
            float freq = 1.0f;
            float amp = 1.0f;

            for (int i = 0; i < octaves; ++i)
            {
                total += Perlin(x * freq, y * freq) * amp;
                freq *= 2.0f;
                amp *= 0.5f;
            }
            
            return total;
        }
};

#endif