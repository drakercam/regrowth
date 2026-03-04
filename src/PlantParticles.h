#ifndef PLANTPARTICLES_H
#define PLANTPARTICLES_H

#include "ofMain.h"
#include "Entity.h"

class PlantParticles : public Entity
{
    private:
        ofVbo VBO;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<float> phases;
        std::vector<float> ids;
        ofImage texture;

        // helper to sample particle positions for a cone shape
        glm::vec3 cone_sample()
        {
            float height = ofRandom(0.0f, 5.0f);
            float radius = 1.0f - height;

            glm::vec2 disk;

            do
            {
                disk = glm::vec2(
                    ofRandom(-1.0f, 1.0f),
                    ofRandom(-1.0f, 1.0f)
                );

            } while (glm::length2(disk) > 1.0f);

            disk *= radius;

            return glm::vec3(GetTransform().position.x + disk.x, GetTransform().position.y + height, GetTransform().position.z + disk.y);
        }

        void PlantParticlesSetup()
        {
            int numParticles = 1000;
            positions.resize(numParticles);
            normals.resize(numParticles);
            phases.resize(numParticles);
            ids.resize(numParticles);

            for (int i = 0; i < numParticles; i++) 
            {
                positions[i] = cone_sample() - GetTransform().position; // using cone sampling rather than sphere sampling
                glm::vec3 dir = glm::normalize(glm::vec3(positions[i].x, 0, positions[i].z));
                normals[i] = glm::vec3(dir.x, 1.0f, dir.z);
                ids[i] = i;
                phases[i] = ofRandom(0, 1);
            }

            ofDisableArbTex(); // may not do anything, here for compatibility
            VBO.setVertexData(positions.data(), numParticles, GL_STATIC_DRAW);
            VBO.setAttributeData(1, &normals[0].x, 3, numParticles, GL_STATIC_DRAW);
            VBO.setAttributeData(2, phases.data(), 1, numParticles, GL_STATIC_DRAW);
            VBO.setAttributeData(3, ids.data(), 1, numParticles, GL_STATIC_DRAW);
        }

    public:
        PlantParticles(const Transform& t, const ofShader& shader, const ofImage& tex)
        : Entity(t, shader), texture(tex)
        {
            PlantParticlesSetup();
        }

        void Draw(ofCamera& camera) override
        {
            glDepthMask(GL_FALSE);

            ofEnableBlendMode(OF_BLENDMODE_ADD);

            auto& shader = GetShader();

            shader.begin();
        
            shader.setUniform1f("pSize",0.12f); // particle point size
            shader.setUniform1f("t", ofGetElapsedTimef()); // time

            shader.setUniformTexture("texture0", texture.getTexture(), 1);

            shader.setUniformMatrix4f("model", GetTransform().GetMatrix());
            ofMatrix4x4 MVP = camera.getModelViewProjectionMatrix();

            shader.setUniformMatrix4f("MVP", MVP);

            VBO.draw(GL_POINTS, 0, positions.size());

            shader.end();

            glDepthMask(GL_TRUE);
        }

        void Update() override {}
};

#endif