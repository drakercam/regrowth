#ifndef PLANTTYPES_H
#define PLANTTYPES_H

#include "ProceduralPlant.h"

struct PlantType1
{
    std::string axiom = "F";
    RuleMap rules = 
    {
        {'F', "F" + Random3DSymbol() + "FL[" + Random3DSymbol() + "FL]L"}
    };

    float angle = 25.7f;
    float segLen = 1.5f;
    int iterations = 5;
};

struct PlantType2
{
    std::string axiom = "F";
    RuleMap rules =
    {
        {'F', "F[" + Random3DSymbol() + "FL]F[" + Random3DSymbol() + "FL][FL]"}  
    };

    float angle = 20.0f;
    float segLen = 1.5f;
    int iterations = 5;
};

struct PlantType3
{
    std::string axiom = "X";
    RuleMap rules =
    {
        {'X', "FL[" + Random3DSymbol() + "X]FL[" + Random3DSymbol() + "X]" + Random3DSymbol() + "X"},
        {'F', "FF"}
    };

    float angle = 20.0f;
    float segLen = 0.3f;
    int iterations = 7;
};


#endif