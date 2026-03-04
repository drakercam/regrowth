#ifndef PLAYER_H
#define PLAYER_H

#include "ofMain.h"
#include "Entity.h"

class Player : public Entity
{
    private:
        float speed;
        glm::vec3 velocity;
        float gravity;
        bool isGrounded;

    public:
        Player(const Transform& t, const ofShader& shader, float speed)
        : Entity(t, shader), speed(speed) 
        {
            velocity = glm::vec3(0.0f);
            gravity = -9.8f;
            isGrounded = true;
        }

        void Update() override {}

        void Update(const glm::vec3& forward, const glm::vec3& right, float dt)
        {
            // --- APPLY GRAVITY ---
            if (!isGrounded)
            {
                velocity.y += gravity * dt;
            }

            if (ofGetKeyPressed(OF_KEY_SPACE) && isGrounded)
            {
                velocity.y = 15.0f;
                isGrounded = false;
            }

            // ---------------------

            // --- WASD MOVEMENT ---
            glm::vec3 flatForward = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));
            glm::vec3 flatRight   = glm::normalize(glm::vec3(right.x, 0.0f, right.z));

            glm::vec3 move = glm::vec3(0.0f);

            if (ofGetKeyPressed('w')) move += flatForward;
            if (ofGetKeyPressed('s')) move -= flatForward;
            if (ofGetKeyPressed('a')) move -= flatRight;
            if (ofGetKeyPressed('d')) move += flatRight;

            if (glm::length(move) > 0.001f)
            {
                move = glm::normalize(move);
            }

            velocity.x = move.x * speed;
            velocity.z = move.z * speed;

            // ---------------------

            // --- APPLY VELOCITY --
            GetTransform().SetPosition(GetTransform().position + velocity * dt);

            if (GetTransform().position.y <= 5.0f)
            {
                GetTransform().SetPosition(glm::vec3(GetTransform().position.x, 0, GetTransform().position.z));
                velocity.y = 0;
                GetTransform().position.y = 5.0f;
                isGrounded = true;
            }

            // ---------------------

            // --- PLANE WALL CLAMP Z ---
            float leftWallBackZ = -1000.0f; // farthest back
            float rightWallBackZ = 1000.0f; // farthest back

            // full span of walls along Z
            float minZ = leftWallBackZ + GetCollisionRadius();   // -990 + radius
            float maxZ = rightWallBackZ - GetCollisionRadius();  // 990 - radius

            glm::vec3 pos = GetTransform().position;
            pos.z = glm::clamp(pos.z, minZ, maxZ);
            GetTransform().SetPosition(pos);

            // ------------------------

            // --- PLANE WALL CLAMP X ---
            float leftWallBackX = -2000.0f; // farthest back
            float rightWallBackX = 2000.0f; // farthest back

            // full span of walls along X
            float minX = leftWallBackX + GetCollisionRadius();   // -990 + radius
            float maxX = rightWallBackX - GetCollisionRadius();  // 990 - radius

            pos = GetTransform().position;
            pos.x = glm::clamp(pos.x, minX, maxX);
            GetTransform().SetPosition(pos);

            // ------------------------

            // --- PLANT WALL COLLISION ---
            float wallX = 0.0f;             // plant wall runs along x = 0
            float wallZMin = -1000.0f;      // plane starts at z = -1000
            float wallZMax =  1000.0f;      // plant ends at z = 1000

            // Door opening
            float doorMinZ = -30.0f;        // door area spans from z = -30 to z = 30
            float doorMaxZ =  30.0f;

            pos = GetTransform().position;

            // Only collide when player is within the span of the plant wall along z-axis
            if (pos.z >= wallZMin && pos.z <= wallZMax
                && !(pos.z >= doorMinZ && pos.z <= doorMaxZ))   // only collide if the player isn't in the range of the plant door
            {
                // Prevent crossing from left to right
                if (pos.x > wallX - GetCollisionRadius() && pos.x < wallX)
                {
                    pos.x = wallX - GetCollisionRadius();
                    velocity.x = 0.0f;
                }

                // Prevent crossing from right to left
                if (pos.x < wallX + GetCollisionRadius() && pos.x > wallX)
                {
                    pos.x = wallX + GetCollisionRadius();
                    velocity.x = 0.0f;
                }
            }

            GetTransform().SetPosition(pos);
            // ----------------------------
        }

        void Draw(ofCamera& camera) override {}
        
        float GetCollisionRadius() const { return 10.0f; }

        float GetSpeed() const { return speed; }
        void SetSpeed(const float s) { speed = s; }

        glm::vec3 GetVelocity() const { return velocity; }
        void SetVelocity(const glm::vec3 v) { velocity = v; }
};

#endif