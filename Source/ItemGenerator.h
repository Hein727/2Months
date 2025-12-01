#pragma once
#include <random>
#include <string>
#include <vector>

// アイテムを表現する軽量なデータ構造
struct GeneratedItem
{
    std::string category;
    std::string name;
    int rarity;             // 0:ノーマル?4:レジェンダリー
    float power;            // 攻撃力や回復量の目安
    bool isCursed;          // デメリット付きかどうか
};

// ランダム生成と簡易評価を担当するユーティリティ
class ItemGenerator
{
public:
    ItemGenerator();
    explicit ItemGenerator(uint32_t seed);

    // 新しいアイテムをランダムに生成
    GeneratedItem GenerateItem();

    // 生成されたアイテムが「価値あり」かどうかを判定
    bool IsValuable(const GeneratedItem& item) const;

private:
    std::mt19937 rng;
    std::vector<std::string> weaponNames;
    std::vector<std::string> armorNames;
    std::vector<std::string> consumableNames;

    std::string SelectName(const std::vector<std::string>& candidates);
    int SelectRarity();
    float GeneratePower(int rarity);
    bool GenerateCurseFlag(int rarity);
};