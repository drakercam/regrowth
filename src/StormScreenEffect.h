#ifndef STORMSCREENEFFECT_H
#define STORMSCREENEFFECT_H

#include "ScreenEffect.h"

class StormScreenEffect : public ScreenEffect {
public:
	StormScreenEffect(const Transform & t, const ofShader & shader, const ofFbo & fbo, bool doDraw = false)
		: ScreenEffect(t, shader, fbo, doDraw) { }

	void Setup() override {
	}

	void Update() override {
		// Animate level -> target
		level += (target - level) * fadeSpeed;
		level = ofClamp(level, 0.0f, 1.0f);

		// auto-disable drawing when invisible
		if (level < 0.1f) {
			doDraw = false;
		}
	}

	void Draw(ofCamera& camera) override {
		auto shader = GetShader();
		// apply visibility shader
		shader.begin();

		shader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
		shader.setUniform2f("u_center", ofGetWidth() / 2, ofGetHeight() / 2);
		shader.setUniform1f("u_radius", radius);
		shader.setUniform1f("u_level", level);
		shader.setUniformTexture("sceneTex", fbo.getTexture(), 0);

		fbo.draw(-1, -1, ofGetWidth(), ofGetHeight());

		shader.end();
	}

private:
	float radius = 200.0f;
};

#endif