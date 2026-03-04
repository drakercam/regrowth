#ifndef MESHENTITY_H
#define MESHENTITY_H

#include "Entity.h"

class MeshEntity : public Entity
{
    public:
        MeshEntity(const Transform& t, const ofShader& shader)
        : Entity(t, shader) {}

        void SetMesh(const ofMesh& m) { mesh = m; }
        ofMesh GetMesh() const { return mesh; }

    private:
        ofMesh mesh;
};

#endif