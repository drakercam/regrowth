#ifndef FERTILIZER_H
#define FERTILIZER_H

#include "ofMain.h"
#include "Entity.h"

class Fertilizer : public Entity
{
    private:
        ofBoxPrimitive body;
		float height;
		float width;
		float depth;
        bool pickedUp;
        ofImage texture;

    public:
		Fertilizer(const Transform & t, const ofShader & shader, const float height, const float width, const float depth, const ofImage & tex)
        : Entity(t, shader), height(height), width(width), depth(depth), pickedUp(false), texture(tex)
        {
            body.set(height, width, depth);
            body.setResolution(60);
            body.setPosition(glm::vec3(0,0,0));
            body.mapTexCoordsFromTexture(texture.getTexture());
        }

        void Update() override {}

        void Draw(ofCamera& camera) override
        {
            if (!pickedUp)
            {
                auto& shader = GetShader();
                auto& transform = GetTransform();

                shader.begin();

                ofMatrix4x4 mvp = camera.getModelViewProjectionMatrix();
                shader.setUniformMatrix4f("mvp", mvp);
                shader.setUniformMatrix4f("model", transform.GetMatrix());
                
                shader.setUniformTexture("texture0", texture.getTexture(), 0);
                shader.setUniform3f("viewPos", camera.getPosition());

                body.draw();

                shader.end();
            }
        }

        void SetSize(const float h, const float w, const float d) 
        { 
            height = h;
			width = w;
			depth = d;
            body.set(h, w, d);
        }

        float GetHeight() { return height; }
		float GetWidth() { return width; }
		float GetDepth() { return depth; }

        void SetPickedUp(const bool p) { pickedUp = true; }
        bool GetPickedUp(void) const { return pickedUp; }
};

#endif
