#ifndef WITHERER_H
#define WITHERER_H

#include "Entity.h"
#include "ofMain.h"

class Witherer : public Entity {
public:
	Witherer(const Transform & t, const ofShader & shader, const ofImage & coneTex, const ofImage & sphereTex)
		: Entity(t, shader)
		, coneTexture(coneTex)
		, sphereTexture(sphereTex) {
		cone.set(5, 30);
		cone.rotate(180, 1.0, 0.0, 0.0);

		sphere.setRadius(5);

		cylinder.set(1, 40);

		// set the position and scale using transforms
		GetTransform().SetScale(glm::vec3(5.0f, 5.0f, 5.0f));

		isHealed = false;

		damagedTexture.load("images/damaged_texture.png");
		healedTexture.load("images/healed_texture.png");
	}

	void Update() override { }

	void Draw(ofCamera & camera) override {
		auto & shader = GetShader();
		auto & transform = GetTransform();

		shader.begin();

		ofMatrix4x4 mvp = camera.getModelViewProjectionMatrix();
		shader.setUniformMatrix4f("mvp", mvp);
		shader.setUniformMatrix4f("model", transform.GetMatrix());

		shader.setUniform3f("viewPos", camera.getPosition());

		shader.setUniformTexture("texture0", coneTexture.getTexture(), 0);

		// Draw shapes centered at origin, shader will handle position & scale
		cone.draw();

		shader.setUniformTexture("texture0", sphereTexture.getTexture(), 0);

		sphere.draw();

		if (!isHealed) {
			shader.setUniformTexture("texture0", damagedTexture.getTexture(), 0);
		} else {
			shader.setUniformTexture("texture0", healedTexture.getTexture(), 0);
		}

		cylinder.draw();

		shader.end();
	}

	float GetEffectRadius() const { return 300.0f; }
	float GetInteractRadius() const { return 50.0f; }
	bool GetIsHealed() const { return isHealed; }
	void SetIsHealed(bool healed) { isHealed = healed; }

private:
	ofConePrimitive cone;
	ofSpherePrimitive sphere;
	ofCylinderPrimitive cylinder;

	ofImage coneTexture;
	ofImage sphereTexture;
	ofImage damagedTexture;
	ofImage healedTexture;

	bool isHealed;
};

#endif
