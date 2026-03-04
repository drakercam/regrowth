#ifndef ENTITY_H
#define ENTITY_H

#include "ofMain.h"
#include "Transform.h"

class Entity
{
    public:
        Entity(const Transform& t, const ofShader& shader)
        : transform(t), shader(shader) {}

        virtual ~Entity() {}

        virtual void Draw(ofCamera& camera) = 0;
        virtual void Update() {}

        void SetTransform(const Transform& t) { transform = t; }
        void SetShader(const ofShader& s) { shader = s; }

        Transform& GetTransform() { return transform; }
        ofShader& GetShader() { return shader; }

    protected:
        Transform transform;
        ofShader shader;
};

#endif