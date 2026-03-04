#ifndef PLANTABLE_AREA_H
#define PLANTABLE_AREA_H

#include "ofMain.h"
#include "Entity.h"

class PlantableArea : public Entity
{
    private:
        ofSpherePrimitive area;
        float radius;
        bool plantedIn;
        ofImage texture;

    public:
        PlantableArea(const Transform& t, const ofShader& shader, float radius, const ofImage& tex)
        : Entity(t, shader), radius(radius), plantedIn(false), texture(tex)
        {
            area.setRadius(radius);
            area.setResolution(60);
            area.setPosition(glm::vec3(0,0,0));
        }

        void Update() override {}

        void Draw(ofCamera& camera) override
        {
            if (!plantedIn)
            {
                auto& shader = GetShader();
                auto& transform = GetTransform();

                shader.begin();

                shader.setUniformMatrix4f("model", transform.GetMatrix());

                ofMatrix4x4 mvp = camera.getModelViewProjectionMatrix();
                shader.setUniformMatrix4f("mvp", mvp);

                shader.setUniformTexture("texture0", texture.getTexture(), 0);
                shader.setUniform3f("viewPos", camera.getPosition());

                area.draw();

                shader.end();
            }
        }

        void SetRadius(const float r) 
        { 
            radius = r; 
            area.setRadius(radius);
        }

        float GetRadius(void) const { return radius; }

        void SetPlantedIn(const bool p) { plantedIn = p; }
        bool GetPlantedIn(void) const { return plantedIn; }
};

#endif