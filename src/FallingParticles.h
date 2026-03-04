#ifndef FALLINGPARTICLES_H
#define FALLINGPARTICLES_H

#include "Entity.h"

class FallingParticles : public Entity {
public:
	FallingParticles(const Transform& t, const ofShader& shader, int numParticles, const ofImage& image)
		: Entity(t, shader), numParticles(numParticles), tex(image) {
		positions.resize(numParticles);
		normals.resize(numParticles);
		phases.resize(numParticles);
		ids.resize(numParticles);
	}

	virtual ~FallingParticles() { }

	virtual void Setup() = 0; // subclass must implement
	virtual void Update() override = 0;
	virtual void Draw(ofCamera & camera) override = 0;

	void SetImage(const ofImage & img) { tex = img; }

protected:
	int numParticles;

	// particle data
	ofVbo vbo;
	vector<glm::vec3> positions;
	vector<glm::vec3> normals;
	vector<float> phases;
	vector<float> ids;

	// optional texture
	ofImage tex;
};

#endif
