#include "AttributeSystem.h"

// 属性ごとの効果を計算する関数
float AttributeSystem::GetEffectiveness(Attribute attacker, Attribute defender)
{
    // 属性相性表（5×5の2次元配列）
    static float table[(int)Attribute::Max][(int)Attribute::Max] =
    {
        // None, Wind, Fire, Wood, Earth
        {1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, // None
        {1.0f, 1.0f, 1.5f, 1.0f, 0.5f}, // Wind → Fireに強く、Earthに弱い
        {1.0f, 0.5f, 1.0f, 1.5f, 1.0f}, // Fire → Woodに強く、Windに弱い
        {1.0f, 1.0f, 0.5f, 1.0f, 1.5f}, // Wood → Earthに強く、Fireに弱い
        {1.0f, 1.5f, 1.0f, 0.5f, 1.0f}, // Earth → Windに強く、Woodに弱い
    };

    // 相性倍率を返す
    return table[(int)attacker][(int)defender];
}