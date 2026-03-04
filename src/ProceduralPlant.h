#ifndef PROCEDURALPLANT_H
#define PROCEDURALPLANT_H

#include "ofMain.h"
#include "Entity.h"
#include "LSystem.h"

class ProceduralPlant : public Entity
{
    private:
        ofMesh mesh;
        ofMesh leafMesh;
        LSystem lsys;
        size_t iterations;
        float segmentLength;
        float angle;

        glm::vec4 baseColor;
		glm::vec4 leafColor;

        bool inGarden;

        struct State
        {
            glm::vec3 pos;
            glm::vec3 forward;
            glm::vec3 right;
            glm::vec3 up;
        };

        void NormalizeState(State& s)
        {
            s.forward   = glm::normalize(s.forward);
            s.right     = glm::normalize(s.right);
            s.up        = glm::normalize(s.up);
        }

    public:
        ProceduralPlant(const Transform & t, const ofShader & shader, const std::string & axiom, const RuleMap & rules, size_t iter, float segLen, float angle,
			glm::vec4 baseCol = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4 leafCol = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), bool inGarden = true)
        : Entity(t, shader), iterations(iter), segmentLength(segLen), angle(angle), baseColor(baseCol), leafColor(leafCol), inGarden(inGarden)
        {
            lsys = LSystem(axiom, rules);
            lsys.SetLSysString(lsys.ExpandString(iter));
        }

        void GenerateGeometry()
        {
            mesh.clear();
            leafMesh.clear();
            mesh.setMode(OF_PRIMITIVE_LINES);
            leafMesh.setMode(OF_PRIMITIVE_TRIANGLES);

            std::vector<State> stack;

            State state;
            // state.pos = GetTransform().position; // the start position is based of the Entity transform
            state.pos = glm::vec3(0.0f);
            state.forward = glm::vec3(0, 1, 0);  // up
            state.up      = glm::vec3(0, 0, 1);
            state.right   = glm::vec3(1, 0, 0);

            const std::string str = lsys.GetLSysString();

            float radians = glm::radians(angle);

            for (char c : str)
            {
                switch (c)
                {
                    case 'F':
                    {
                        glm::vec3 start = state.pos;
                        state.pos += state.forward * segmentLength;
                        glm::vec3 end = state.pos;

                        mesh.addVertex(start);
                        mesh.addVertex(end);
                        break;
                    }

                    case 'L':
                    {
                        // Leaf logic
                        float leafSize = segmentLength * 0.5f;

                        // center of the lead
                        glm::vec3 pos = state.pos;

                        // will we use a randomly rotate right vector to provide
                        // some variety to the leaves
                        float theta = ofRandom(-PI/4, PI/4);

                        // We will build a flat quad using right/up axes

                        glm::vec3 right = glm::rotate(state.right, theta, state.forward);
                        glm::vec3 up = state.up * leafSize;

                        glm::vec3 v0 = pos - right - up;
                        glm::vec3 v1 = pos + right - up;
                        glm::vec3 v2 = pos + right + up;
                        glm::vec3 v3 = pos - right + up;

                        leafMesh.addVertex(v0);
                        leafMesh.addVertex(v1);
                        leafMesh.addVertex(v2);

                        leafMesh.addVertex(v2);
                        leafMesh.addVertex(v3);
                        leafMesh.addVertex(v0);

                        break;
                    }

                    case '+':   // yaw right
                    {
                        state.forward = glm::rotate(state.forward, radians, state.up);
                        state.right = glm::rotate(state.right, radians, state.up);
                        break;
                    }

                    case '-':   // yaw left
                    {
                        state.forward = glm::rotate(state.forward, -radians, state.up);
                        state.right = glm::rotate(state.right, -radians, state.up);
                        break;
                    }

                    case '&':   // pitch down
                    {
                        state.forward = glm::rotate(state.forward, radians, state.right);
                        state.up = glm::rotate(state.up, radians, state.right);
                        break;
                    }

                    case '^':   // pitch up
                    {
                        state.forward = glm::rotate(state.forward, -radians, state.right);
                        state.up = glm::rotate(state.up, -radians, state.right);
                        break;
                    }

                    case '@':  // roll left
                    {
                        state.right = glm::rotate(state.right, radians, state.forward);
                        state.up = glm::rotate(state.up, radians, state.forward);
                        break;
                    }

                    case '*':   // roll right
                    {
                        state.right = glm::rotate(state.right, -radians, state.forward);
                        state.up = glm::rotate(state.up, -radians, state.forward);
                        break;
                    }

                    case '[':   // push state
                    {
                        stack.push_back(state);
                        break;
                    }

                    case ']':   // pop state
                    {
                        if (!stack.empty())
                        {
                            state = stack.back();
                            stack.pop_back();
                        }
                        break;
                    }

                    default:
                        break;
                }

                NormalizeState(state);
            }
        }

        virtual void Update() override
        {
            // add update functionality here
        }

        virtual void Draw(ofCamera& camera) override
        {
            // We will apply the entity's transform here
		    ofPushMatrix();
		    ofMultMatrix(GetTransform().GetMatrix());

                glm::mat4 model = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);
                glm::mat4 mvp = camera.getProjectionMatrix() * model;

                GetShader().begin();

                    GetShader().setUniformMatrix4f("mvp", mvp);
                    GetShader().setUniform4f("uColor", baseColor);

                    mesh.draw();

                    GetShader().setUniform4f("uColor", leafColor);
                    leafMesh.draw();

                GetShader().end();

            ofPopMatrix();
        }

        void DrawWithoutModelTransform(ofCamera& camera)
        {
            glm::mat4 model = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);
            glm::mat4 mvp = camera.getProjectionMatrix() * model;

            GetShader().setUniformMatrix4f("mvp", mvp);

            GetShader().setUniform4f("uColor", baseColor);

            mesh.draw();

            GetShader().setUniform4f("uColor", leafColor);
            leafMesh.draw();
        }

        void SetSegmentLen(float length) { segmentLength = length; }
        void SetAngle(float theta) { angle = theta; }

        void SetInGarden(bool ig) { inGarden = ig; }
        bool GetInGarden(void) const { return inGarden; }
};

// Helper to obtain a random 3D symbols
inline std::string Random3DSymbol()
{
    static const std::vector<char> ops =
    {
        '+', '-', '&',
        '^', '@', '*'
    };

    return std::string(1, ops[rand() % ops.size()]);
}

#endif