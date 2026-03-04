#ifndef SKYBOX_H
#define SKYBOX_H

#include "ofMain.h"
#include "Entity.h"

class Skybox : public Entity
{
    private:
        ofImage texture;
        ofColor color;
        float radius;
        ofSpherePrimitive sphere;

    public:
        Skybox(const Transform& t, const ofShader& shader, float radius, const ofImage& tex)
        : Entity(t, shader), radius(radius), texture(tex)
        {
            color = ofColor::skyBlue;
            texture.rotate90(2);

            sphere.setRadius(radius);
            sphere.setResolution(60);
            sphere.setPosition(glm::vec3(0,0,0));
            sphere.mapTexCoordsFromTexture(texture.getTexture());
        }

        void Update() override
        {

        }

        void Draw(ofCamera& camera) override
        {
            auto& shader = GetShader();

            // ofEnableDepthTest();

            ofPushMatrix();
		    ofMultMatrix(GetTransform().GetMatrix());

                shader.begin();

                ofMatrix4x4 mvp = camera.getModelViewProjectionMatrix();
                shader.setUniformMatrix4f("mvp", mvp);
                shader.setUniformTexture("texture0", texture.getTexture(), 0);
                // shader.setUniform4f("uColor", color);

                // ofSetColor(color);
                sphere.draw();

                shader.end();

            ofPopMatrix();

            // ofDisableDepthTest();
        }
};

#endif