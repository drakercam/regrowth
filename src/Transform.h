#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "ofMain.h"

struct Transform
{
    glm::vec3 position;
    glm::vec3 rotation; // might use quaternions, we will see
    glm::vec3 scale;

    Transform()
    {
        this->position = glm::vec3(0.0f);
        this->rotation = glm::vec3(0.0f);
        this->scale = glm::vec3(1.0f);
    }
    
    Transform(const Transform& t)
    {
        this->position = t.position;
        this->rotation = t.rotation;
        this->scale    = t.scale;
    }

    Transform(const glm::vec3 pos, const glm::vec3 rot, const glm::vec3 scale)
    {
        this->position = pos;
        this->rotation = rot;
        this->scale = scale;
    }

    void SetPosition(const glm::vec3 p)
    {
        this->position = p;
    }   

    void SetRotation(const glm::vec3 r)
    {
        this->rotation = r;
    }

    void SetScale(const glm::vec3 s)
    {
        this->scale = s;
    }

    glm::mat4 GetMatrix() const
    {
        glm::mat4 m = glm::mat4(1.0f);

        // Translate
        m = glm::translate(m, position);

        // Rotate
        m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1,0,0));
        m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0,1,0));
        m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0,0,1));

        // Scale
        m = glm::scale(m, scale);

        return m;
    }
};

#endif