#pragma once

enum class Attribute
{
    None,
    Wind,
    Fire,
    wood,
    Earth,
    Max
};

class AttributeSystem
{
public:
    static float GetEffectiveness(Attribute attacker, Attribute defender);
};