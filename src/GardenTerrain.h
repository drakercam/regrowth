#ifndef GARDENTERRAIN_H
#define GARDENTERRAIN_H

#include "Terrain.h"

class GardenTerrain : public Terrain
{
    public:
        GardenTerrain(const Transform& t, const ofShader& shader, int w, int h, int res)
        : Terrain(t, shader, w, h, res) 
        {
            noiseScale = 0.01f;
            noiseAmplitude = 15.0f;
            noiseOctaves = 12;
            colorLow = glm::vec3(0.2, 0.5, 0.1);
            colorMid = glm::vec3(0.4, 0.9, 0.3);
            colorHigh = glm::vec3(0.6, 1.0, 0.4);
        }

        virtual void Draw(ofCamera& camera) override
        {
            auto& shader = GetShader();
            shader.begin();

            shader.setUniformMatrix4f("model", GetTransform().GetMatrix());
            shader.setUniformMatrix4f("view", camera.getModelViewMatrix());
            shader.setUniformMatrix4f("projection", camera.getProjectionMatrix());

            shader.setUniform1f("noiseScale", noiseScale);
            shader.setUniform1f("noiseAmplitude", noiseAmplitude);
            shader.setUniform2f("noiseOffset", offset.x, offset.y);
            shader.setUniform1i("noiseOctaves", noiseOctaves);

            shader.setUniform3f("viewPos", camera.getPosition());

            // Set height based colours for shader
            shader.setUniform3f("colorLow", colorLow);
            shader.setUniform3f("colorMid", colorMid);
            shader.setUniform3f("colorHigh", colorHigh);

            GetMesh().draw();

            shader.end();
        }

        void SetOffset(const glm::vec2& off) { offset = off; }

        float GetHeightAt(float x, float z)
        {
            float h = FBM(x * noiseScale + offset.x, z * noiseScale + offset.y, noiseOctaves);
            return h * noiseAmplitude;
        }

    private:
        float noiseScale;
        float noiseAmplitude;
        float noiseOctaves;

        glm::vec3 colorLow;
        glm::vec3 colorMid;
        glm::vec3 colorHigh;

        glm::vec2 offset = {0, 0};
};

#endif