#ifndef PLANTWALL_H
#define PLANTWALL_H

#include "ofMain.h"
#include "Entity.h"
#include "ProceduralPlant.h"
#include "PlantTypes.h"

class PlantWall : public Entity
{
    private:
        std::vector<std::unique_ptr<ProceduralPlant>> leftSidePlants;
        std::vector<std::unique_ptr<ProceduralPlant>> rightSidePlants;
        size_t numPlantsEachSide = 96;

        void createProceduralPlantPT2(std::unique_ptr<ProceduralPlant>& plant, const glm::vec3 position)
        {
            PlantType2 pt;

            glm::vec4 baseCol = glm::vec4(0.0f, ofRandom(0.3f, 1.0f), 0.0f, 1.0f);
            glm::vec4 leafCol = glm::vec4(ofRandom(0.3f, 1.0f), 0.0f, ofRandom(0.3f, 1.0f), 1.0f);

            Transform t = Transform();
            t.SetPosition(position);

            plant = std::make_unique<ProceduralPlant>(
                t, shader, pt.axiom, pt.rules,
                pt.iterations, pt.segLen, pt.angle,
                baseCol, leafCol, true);

            plant->GenerateGeometry();
        }

        void createProceduralPlantPT3(std::unique_ptr<ProceduralPlant>& plant, const glm::vec3 position)
        {
            PlantType3 pt;

            glm::vec4 baseCol = glm::vec4(0.0f, ofRandom(0.3f, 1.0f), 0.0f, 1.0f);
            glm::vec4 leafCol = glm::vec4(ofRandom(0.3f, 1.0f), 0.0f, ofRandom(0.3f, 1.0f), 1.0f);

            Transform t = Transform();
            t.SetPosition(position);

            plant = std::make_unique<ProceduralPlant>(
                t, shader, pt.axiom, pt.rules,
                pt.iterations, pt.segLen, pt.angle,
                baseCol, leafCol, true);

            plant->GenerateGeometry();
        }

    public:
        PlantWall(const Transform& t, const ofShader& shader)
        : Entity(t, shader)
        {
            leftSidePlants.resize(numPlantsEachSide);
            rightSidePlants.resize(numPlantsEachSide);

            // Create the plants on the left side
            for (size_t i = 0; i < leftSidePlants.size(); ++i)
            {
                static float offsetZ = -10.0f;
                size_t randomChoice = rand() % 2;
                glm::vec3 position = glm::vec3(0, 0, -30 + offsetZ);

                if (randomChoice == 0)
                {
                    createProceduralPlantPT2(leftSidePlants.at(i), position);
                }
                else if (randomChoice == 1)
                {
                    createProceduralPlantPT3(leftSidePlants.at(i), position);
                }

                offsetZ -= 10.0f;
            }

            // Create the plants on the right side
            for (size_t i = 0; i < rightSidePlants.size(); ++i)
            {
                static float offsetZ = 10.0f;
                size_t randomChoice = rand() % 2;
                glm::vec3 position = glm::vec3(0, 0, 30 + offsetZ);

                if (randomChoice == 0)
                {
                    createProceduralPlantPT2(rightSidePlants.at(i), position);
                }
                else if (randomChoice == 1)
                {
                    createProceduralPlantPT3(rightSidePlants.at(i), position);
                }

                offsetZ += 10.0f;
            }
        }

        void Update() override 
        {

        }

        void Draw(ofCamera& camera) override
        {
            for (size_t i = 0; i < leftSidePlants.size(); ++i)
            {
                leftSidePlants.at(i)->Draw(camera);
            }

            for (size_t i = 0; i < rightSidePlants.size(); ++i)
            {
                rightSidePlants.at(i)->Draw(camera);
            }
        }
};

#endif