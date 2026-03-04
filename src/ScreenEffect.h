#ifndef SCREENEFFECT_H
#define SCREENEFFECT_H

#include "Entity.h"

class ScreenEffect : public Entity {
public:
	ScreenEffect(const Transform & t, const ofShader & shader, const ofFbo& fbo, bool doDraw)
	: Entity(t, shader), doDraw(doDraw), fbo(fbo)  {}

	virtual ~ScreenEffect() { }

	virtual void Setup() = 0; // subclass must implement
	virtual void Update() override = 0;
	virtual void Draw(ofCamera & camera) override = 0;

	bool getDoDraw() { return doDraw; }
	void setDoDraw(bool draw) { doDraw = draw; }
	float getTarget() { return target; }
	void setTarget(float t) { target = t; }

	void Activate() { }

protected:
	bool doDraw;
	// current animated value (0 -> 1)
	float level = 0.0f;
	// max fade level
	float target = 1.0f;
	float fadeSpeed = 1.0f / 50.0f;

	const ofFbo& fbo;
};

#endif
