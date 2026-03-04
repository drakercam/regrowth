#ifndef GARDENFALLINGPARTICLES_H
#define GARDENFALLINGPARTICLES_H

#include "FallingParticles.h"

class GardenFallingParticles : public FallingParticles 
{
public:
	GardenFallingParticles(const Transform & t, const ofShader & shader, int numParticles, const ofImage & image)
		: FallingParticles(t, shader, numParticles, image)
	{
	}

	void Setup() override 
	{
		// initialize particle attributes
		for (int i = 0; i < numParticles; i++) {
			positions[i] = randomPlanePoint();
			normals[i] = glm::vec3(0, -1, 0);
			ids[i] = i;
			phases[i] = ofRandom(0, 1);
		}

		// initialize VBO
		vbo.setVertexData(positions.data(), numParticles, GL_STATIC_DRAW);
		vbo.setAttributeData(1, &normals[0].x, 3, numParticles, GL_STATIC_DRAW);
		vbo.setAttributeData(2, phases.data(), 1, numParticles, GL_STATIC_DRAW);
		vbo.setAttributeData(3, ids.data(), 1, numParticles, GL_STATIC_DRAW);
	}

	void Update() override 
	{
		Transform t = GetTransform();
		glm::vec3 pos = t.position;

		// re-generate particles that leave the bounds in local-space
		if (pos.x < 0)
		{
			for (size_t i = 0; i < positions.size(); i++) {
				if (std::abs(pos.x - positions[i].x) > edge || std::abs(pos.z - positions[i].z) > edge) {
					positions[i] = randomPlanePoint();
					phases[i] = ofRandom(0, 1);

					vbo.setVertexData(positions.data(), numParticles, GL_STATIC_DRAW);
					vbo.setAttributeData(2, phases.data(), 1, numParticles, GL_STATIC_DRAW);
				}
			}
		}
	}

	void Draw(ofCamera& camera) override 
	{
		glDepthMask(GL_TRUE);
		ofEnableBlendMode(OF_BLENDMODE_ADD);

		ofPushMatrix();

		//camera.begin();

		glm::vec3 pos = GetTransform().position;

		// We will apply the entitie's transform here
		ofPushMatrix();
		ofMultMatrix(GetTransform().GetMatrix());

		if (pos.x <= 0)
		{
			// Draw the particles with shader
			auto& shader = GetShader();
			shader.begin();

			if (!tex.isAllocated()) 
			{
				std::cout << "tex NOT loaded!";
			}

			// uniforms
			shader.setUniform1f("pSize", 2.0f);
			shader.setUniform1f("t", ofGetElapsedTimef());
			shader.setUniformTexture("texture0", tex.getTexture(), 1);

			// MVP
			ofMatrix4x4 mvp = camera.getModelViewProjectionMatrix();
			shader.setUniformMatrix4f("MVP", mvp);

			vbo.draw(GL_POINTS, 0, numParticles);

			shader.end();
		}

		ofPopMatrix();	// remove the entity transform

		//camera.end();

		ofPopMatrix();
		ofDisableBlendMode();
		//glDepthMask(GL_FALSE);
	}

private:
	int edge = 100;
	int up = 50;

	glm::vec3 randomPlanePoint() 
	{
		float x = ofRandom(GetTransform().position.x - edge, GetTransform().position.x + edge);
		float z = ofRandom(GetTransform().position.z - edge, GetTransform().position.z + edge);
		float y = GetTransform().position.y + ofRandom(0, up);

		return glm::vec3(x, y, z);
	}
};

#endif