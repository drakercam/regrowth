#ifndef CUSTOMCAMERA_H
#define CUSTOMCAMERA_H

#include "ofCamera.h"
#include "Player.h"

class CustomCamera : public ofCamera 
{
    private:
        float movementSpeed;
        float rotationSpeed;

        glm::vec3 position;
        glm::quat orientation;

        glm::vec3 BASE_UP;
        glm::vec3 BASE_SIDE;
        glm::vec3 BASE_FORWARD;
        
        // Keep track of time
        double currentTime;
        float eyeLevel;

    public:

        CustomCamera() 
        {
            movementSpeed = 150.0f; // units per second
            rotationSpeed = glm::radians(90.0f);   // rads per second
            position =      glm::vec3(0.0f);
            orientation =   glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // identity quat

            BASE_SIDE =     glm::vec3(1.0f, 0.0f, 0.0f);       // s0
            BASE_UP =       glm::vec3(0.0f, 1.0f, 0.0f);       // u0
            BASE_FORWARD =  glm::vec3(0.0f, 0.0f, 1.0f);       // f0

            eyeLevel = 10.0f;
        }

        std::string getClockTime() 
        {
            double time = currentTime;

            int mins = floor(time / 60.0);
            int secs = std::fmod(time, 60.0);

            std::string minutes = std::to_string(mins);
            std::string seconds = std::to_string(secs);

            if (seconds.length() <= 1) {
                seconds = "0" + seconds;
            }

            return minutes + ":" + seconds;
        }

        void Update(Player& player, float deltaTime)
        {
            // update time
            currentTime += deltaTime;

            // --- CAMERA ROTATION ---
            float rotAmount = rotationSpeed * deltaTime;

            // --- Tilt ---
            if (ofGetKeyPressed('i')) pitch(rotAmount);
            if (ofGetKeyPressed('k')) pitch(-rotAmount);

            // --- Pan ---
            if (ofGetKeyPressed('j')) yaw(rotAmount);
            if (ofGetKeyPressed('l')) yaw(-rotAmount);

            // // --- Roll ---
            // if (ofGetKeyPressed('u')) roll(-rotAmount);
            // if (ofGetKeyPressed('o')) roll(rotAmount);

            orientation = glm::normalize(orientation);  // normalize the orientation

            // -----------------------

            // --- CAMERA POSITION ---
            position = player.GetTransform().position;

            // Camera eye height
            position.y += eyeLevel;

            // -----------------------

            // need to set ofCamera parameters using internal position, orientation
            setPosition(position);
            setOrientation(orientation);

        }

        void pitch(float amount) 
        {
            // for pitch, we rotate about camera side (aka x axis)
            glm::vec3 axis = getqSide();
            axis = glm::normalize(axis);

            glm::quat rotQuat = glm::angleAxis(amount, axis);
            orientation = glm::normalize(rotQuat * orientation);
        }

        // ONLY EVER YAW ABOUT THE ACTUAL WORLD UP
        // Creates a realistic FPS camera
        void yaw(float amount) 
        {
            // for yaw, we rotate about camera up (aka y axis)
            glm::vec3 axis = glm::vec3(0,1,0);

            glm::quat rotQuat = glm::angleAxis(amount, axis);
            orientation = glm::normalize(rotQuat * orientation);
        }

        // No need
        // void roll(float amount) 
        // {
        //     // for roll, we rotate about camera forwards (z axis)
        //     glm::vec3 axis = getqForward();
        //     axis = glm::normalize(axis);

        //     glm::quat rotQuat = glm::angleAxis(amount, axis);
        //     orientation = glm::normalize(rotQuat * orientation);
        // }

        glm::vec3 getqForward() 
        {
            glm::vec3 forward = glm::normalize(glm::mat3_cast(orientation) * BASE_FORWARD);

            return -forward;
        }


        glm::vec3 getqSide() 
        {
            glm::vec3 forward = glm::normalize(glm::mat3_cast(orientation) * BASE_FORWARD);
            glm::vec3 up = glm::normalize(glm::mat3_cast(orientation) * BASE_UP);
            glm::vec3 side = glm::normalize(glm::cross(up, forward));

            return side;
        }


        glm::vec3 getqUp() 
        {
            // glm::vec3 forward = glm::normalize(glm::mat3_cast(orientation) * BASE_FORWARD);
            // glm::vec3 side = glm::normalize(glm::mat3_cast(orientation) * BASE_SIDE);
            // glm::vec3 up = glm::normalize(glm::cross(side, forward));

            // return up;
            return glm::normalize(glm::mat3_cast(orientation) * BASE_UP);
        }

        // may prove useful for ray-cast
        float GetEyeLevel() const { return eyeLevel; }
};

#endif