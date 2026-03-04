#ifndef RAY_H
#define RAY_H

#include "ofMain.h"

class Ray
{
    private:
        glm::vec3 origin;
        glm::vec3 direction;

    public:

        // definition of a ray
        glm::vec3 PointAt(float t) const
        {
            return origin + direction * t;
        }

        // Setters
        void SetOrigin(glm::vec3 o) { origin = o; }
        void SetDirection(glm::vec3 dir) { direction = glm::normalize(dir); }

        // Getters
        glm::vec3 GetOrigin(void) const { return origin; }
        glm::vec3 GetDirection(void) const { return direction; }

        // takes the sphere's center and radius, and an output paramaeter, hitDist which
        // is the distance t to the hit
        bool RaySphereIntersect(const glm::vec3& sphereCenter, float sphereRadius, float& hitDistance) const
        {
            // vector from the center of the sphere to the ray's origin
            glm::vec3 originCenter = origin - sphereCenter;
            
            // a is 1.0 because the ray direction is normalized, DOT(Dir, Dir) = 1
            float a = 1.0f;

            // projecting the vector from sphere center to ray origin onto the ray direction
            // tells us how far along the ray direction the sphere center lies, tells us if sphere
            // is behind or infront of the ray
            float b = glm::dot(originCenter, direction);

            // if c < 0, it tells us that the ray starts inside the sphere
            float c = glm::dot(originCenter, originCenter) - sphereRadius * sphereRadius;

            // the discriminant tells us if the ray gets close enough to the sphere
            // < 0 means it misses, = 0 ray grazes the sphere, > 0 means ray enters and exits the sphere
            float discriminant = b * b - c;

            // ray missed, no collision
            if (discriminant < 0)
            {
                return false;
            }

            // the two solutions to the quadratic equation
            // t1 is where the ray enters, t2 is where the ray exits
            float t1 = (-b - sqrt(discriminant));
            float t2 = (-b + sqrt(discriminant));

            // we want to track the closest valid hit
            float tMin = std::numeric_limits<float>::infinity();

            // we choose the nearest positive intersection
            if (t1 > 0 && t1 < tMin) tMin = t1;
            if (t2 > 0 && t2 < tMin) tMin = t2;

            // check if we actually hit something
            if (tMin < std::numeric_limits<float>::infinity())
            {
                hitDistance = tMin;
                return true;    // return true if we hit
            }

            return false;   // return false if we did not
        }
};

#endif