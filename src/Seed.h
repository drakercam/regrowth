#ifndef SEED_H
#define SEED_H

#include "ofMain.h"
#include "Entity.h"

class Seed : public Entity
{
    private:
        ofSpherePrimitive body;
        float radius;
        bool pickedUp;
        ofImage texture;

    public:
        Seed(const Transform& t, const ofShader& shader, const float radius, const ofImage& tex)
        : Entity(t, shader), radius(radius), pickedUp(false), texture(tex)
        {
            body.setRadius(radius);
            body.setResolution(60);
            body.setPosition(glm::vec3(0,0,0));
            body.mapTexCoordsFromTexture(texture.getTexture());
        }

        void Update() override {}

        void Draw(ofCamera& camera) override
        {
            if (!pickedUp)
            {
                auto& shader = GetShader();
                auto& transform = GetTransform();

                shader.begin();

                ofMatrix4x4 mvp = camera.getModelViewProjectionMatrix();
                shader.setUniformMatrix4f("mvp", mvp);
                shader.setUniformMatrix4f("model", transform.GetMatrix());
                
                shader.setUniformTexture("texture0", texture.getTexture(), 0);
                shader.setUniform3f("viewPos", camera.getPosition());

                body.draw();

                shader.end();
            }
        }

        void SetRadius(const float r) 
        { 
            radius = r; 
            body.setRadius(radius);
        }

        float GetRadius(void) const { return radius; }

        void SetPickedUp(const bool p) { pickedUp = true; }
        bool GetPickedUp(void) const { return pickedUp; }
};

#endif