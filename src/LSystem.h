#ifndef LSYSTEM_H
#define LSYSTEM_H

#include "ofMain.h"

using RuleMap = std::unordered_map<char, std::string>;

class LSystem
{
    private:
        std::string axiom;
        RuleMap rules;

    public:
        LSystem() {}

        LSystem(const std::string& axiom, const RuleMap& rules)
        : axiom(axiom), rules(rules) {}

        std::string ExpandString(size_t iterations)
        {
            std::string result = this->axiom;

            for (size_t i = 0; i < iterations; ++i)
            {
                std::string next;

                for (char c : result)
                {
                    auto iterator = rules.find(c);

                    if (iterator != rules.end())
                    {
                        next += iterator->second;   // use the rule
                    }
                    else
                    {
                        next += c;                  // no rule to use, keep the character
                    }
                }

                result = next;
            }

            return result;
        }

        void SetLSysString(const std::string s) { axiom = s; }
        const std::string& GetLSysString() const { return axiom; }
};

#endif