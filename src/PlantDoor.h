#ifndef PLANTDOOR_H
#define PLANTDOOR_H

#include "ofMain.h"
#include "Entity.h"
#include "ProceduralPlant.h"

class PlantDoor : public Entity
{
    private:
        PlantType2 pt;
        std::unique_ptr<ProceduralPlant> leftPlant;
        std::unique_ptr<ProceduralPlant> rightPlant;
        std::unique_ptr<ProceduralPlant> leftAnglePlant;
        std::unique_ptr<ProceduralPlant> rightAnglePlant;

        bool opening = false;
        float openAmount = 0.0f;

        void createProceduralPlant(std::unique_ptr<ProceduralPlant>& plant)
        {
            glm::vec4 baseCol = glm::vec4(0.0f, ofRandom(0.3f, 1.0f), 0.0f, 1.0f);
            glm::vec4 leafCol = glm::vec4(ofRandom(0.3f, 1.0f), 0.0f, ofRandom(0.3f, 1.0f), 1.0f);

            Transform t = Transform();

            plant = std::make_unique<ProceduralPlant>(
                t, shader, pt.axiom, pt.rules,
                pt.iterations, pt.segLen, pt.angle,
                baseCol, leafCol, true);

            plant->GenerateGeometry();
        }

    public:
        PlantDoor(const Transform& t, const ofShader& shader)
        : Entity(t, shader)
        {
            createProceduralPlant(leftPlant);
            createProceduralPlant(rightPlant);
            createProceduralPlant(leftAnglePlant);
            createProceduralPlant(rightAnglePlant);
        }

        void Update() override 
        {
            float openSpeed = 1.0f;

            if (opening)
            {
                openAmount = glm::min(openAmount + openSpeed * float(ofGetLastFrameTime()), 1.0f);
            }
            else
            {
                openAmount = glm::max(openAmount - openSpeed * float(ofGetLastFrameTime()), 0.0f);
            }
        }

        void Draw(ofCamera& camera) override
        {
            auto& shader = GetShader();
            float openAngle = 90.0f * openAmount;

            ofPushMatrix();
            ofMultMatrix(GetTransform().GetMatrix());

                shader.begin();

                // --- LEFT PLANT ---
                ofPushMatrix();
                ofTranslate(glm::vec3(0, 0, -30));
                ofRotateYDeg(openAngle);
                leftPlant->DrawWithoutModelTransform(camera);

                    // --- LEFT ANGLED PLANT ---
                    ofPushMatrix();
                    ofTranslate(glm::vec3(0, 5, 0));
                    ofRotateXDeg(35.0f);
                    leftAnglePlant->DrawWithoutModelTransform(camera);

                    ofPopMatrix();
                    // -------------------------

                ofPopMatrix();
                // ------------------

                // --- RIGHT PLANT ---
                ofPushMatrix();
                ofTranslate(glm::vec3(0, 0, 30));
                ofRotateYDeg(-openAngle);
                rightPlant->DrawWithoutModelTransform(camera);

                    // --- RIGHT ANGLED PLANT ---
                    ofPushMatrix();
                    ofTranslate(glm::vec3(0, 5, -5));
                    ofRotateXDeg(-35.0f);
                    rightAnglePlant->DrawWithoutModelTransform(camera);

                    ofPopMatrix();
                    // --------------------------

                ofPopMatrix();
                // -------------------

                shader.end();

            ofPopMatrix();
        }

        bool GetIsOpening() const { return opening; }
        void SetIsOpening(bool o) { opening = o; }
};

#endif