#ifndef WITHEREDSCREENEFFECT_H
#define WITHEREDSCREENEFFECT_H

#include "ScreenEffect.h"

class WitheredScreenEffect : public ScreenEffect {
public:
	WitheredScreenEffect(const Transform & t, const ofShader & shader, const ofFbo & fbo, bool doDraw = false)
		: ScreenEffect(t, shader, fbo, doDraw) { }

	void Setup() override {
	}

	void Update() override {
		// Animate level -> target
		level += (target - level) * fadeSpeed;
		level = ofClamp(level, 0.0f, 1.0f);

		// auto-disable drawing when invisible
		if (level < 0.001f) {
			doDraw = false;
		}
	}

	void Draw(ofCamera& camera) override {
		auto shader = GetShader();
		// apply visibility shader
		shader.begin();

		shader.setUniform1f("timer", ofGetElapsedTimef());
		shader.setUniform1f("staticAmount", level);
		shader.setUniformTexture("tex0", fbo.getTexture(), 0);

		ofPushMatrix();
		ofTranslate(0, ofGetHeight());
		ofScale(1, -1);
		fbo.draw(-1, -1, ofGetWidth(), ofGetHeight());
		ofPopMatrix();

		shader.end();
	}

private:
};

#endif
