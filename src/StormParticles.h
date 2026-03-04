#ifndef STORMPARTICLES_H
#define STORMPARTICLES_H

#include "FallingParticles.h"

class StormParticles : public FallingParticles {
public:
	StormParticles(const Transform & t, const ofShader & shader,
		int numParticles, const ofImage & img, glm::vec3 center = glm::vec3(0, 20, 0), float r = 80.0f, float h = 40.0f)
		: FallingParticles(t, shader, numParticles, img), stormCenter(center), radius(r), height(h) {
	}

	void Setup() override {
		for (int i = 0; i < numParticles; i++) {
			positions[i] = randomStormPoint();
			normals[i] = glm::vec3(0, 1, 0); // upward drift
			phases[i] = ofRandom(0, 1);
			ids[i] = i;
		}

		vbo.setVertexData(positions.data(), numParticles, GL_DYNAMIC_DRAW);
		vbo.setAttributeData(1, &normals[0].x, 3, numParticles, GL_STATIC_DRAW);
		vbo.setAttributeData(2, phases.data(), 1, numParticles, GL_STATIC_DRAW);
		vbo.setAttributeData(3, ids.data(), 1, numParticles, GL_STATIC_DRAW);
	}

	void Update() override {
		
	}

	void Draw(ofCamera & camera) override {
		ofEnableAlphaBlending();
		//ofEnableBlendMode(OF_BLENDMODE_ADD);
		glDepthMask(GL_FALSE);

		auto & shader = GetShader();
		shader.begin();

		shader.setUniform1f("t", ofGetElapsedTimef());
		shader.setUniform1f("pSize", 3.0f);
		shader.setUniformTexture("texture0", tex.getTexture(), 1);
		shader.setUniform1f("t", ofGetElapsedTimef());
		shader.setUniform1f("dt", ofGetLastFrameTime());
		shader.setUniform1f("stormIntensity", 7.0f);
		shader.setUniform3f("stormCenter", stormCenter);
		shader.setUniform1f("stormRadius", radius);

		ofMatrix4x4 model = GetTransform().GetMatrix();
		ofMatrix4x4 viewProj = camera.getModelViewProjectionMatrix();
		ofMatrix4x4 mvp = viewProj * model;
		shader.setUniformMatrix4f("MVP", mvp);

		vbo.draw(GL_POINTS, 0, numParticles);

		shader.end();

		glDepthMask(GL_TRUE);
		ofDisableAlphaBlending();
		//ofDisableBlendMode();
	}

	glm::vec3 GetStormCenter() const { return stormCenter; }
	float GetStormRadius() const { return radius; }
	float GetStormHeight() const { return height; }

private:
	glm::vec3 stormCenter;
	float radius;
	float height;

	glm::vec3 randomStormPoint() {
		float x = ofRandom(stormCenter.x - radius, stormCenter.x + radius);
		float y = ofRandom(stormCenter.y - height, stormCenter.y + height);
		float z = ofRandom(stormCenter.z - radius, stormCenter.z + radius);
		return glm::vec3(x, y, z);
	}
};

#endif
