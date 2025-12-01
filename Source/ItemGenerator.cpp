#include "ItemGenerator.h"
#include <chrono>
#include <cmath>

ItemGenerator::ItemGenerator()
{
    const auto now = std::chrono::high_resolution_clock::now();
    const auto seed = static_cast<uint32_t>(now.time_since_epoch().count());
    rng.seed(seed);

    weaponNames = { "Rusty Sword", "Knight Blade", "Arc Saber", "Mythic Spear" };
    armorNames = { "Leather Armor", "Steel Plate", "Dragon Scale", "Eclipse Cloak" };
    consumableNames = { "Herb", "Hi-Potion", "Elixir", "Phoenix Fruit" };
}

ItemGenerator::ItemGenerator(uint32_t seed)
{
    rng.seed(seed);
    weaponNames = { "Rusty Sword", "Knight Blade", "Arc Saber", "Mythic Spear" };
    armorNames = { "Leather Armor", "Steel Plate", "Dragon Scale", "Eclipse Cloak" };
    consumableNames = { "Herb", "Hi-Potion", "Elixir", "Phoenix Fruit" };
}

GeneratedItem ItemGenerator::GenerateItem()
{
    std::uniform_int_distribution<int> categoryDist(0, 2);
    const int categoryIndex = categoryDist(rng);

    GeneratedItem item{};
    if (categoryIndex == 0)
    {
        item.category = "Weapon";
        item.name = SelectName(weaponNames);
    }
    else if (categoryIndex == 1)
    {
        item.category = "Armor";
        item.name = SelectName(armorNames);
    }
    else
    {
        item.category = "Consumable";
        item.name = SelectName(consumableNames);
    }

    item.rarity = SelectRarity();
    item.power = GeneratePower(item.rarity);
    item.isCursed = GenerateCurseFlag(item.rarity);

    return item;
}

bool ItemGenerator::IsValuable(const GeneratedItem& item) const
{
    // レア度と性能が高く、呪われていなければ「価値あり」とみなす
    const bool rarityGood = item.rarity >= 3; // Epic 以上
    const bool powerGood = item.power >= 120.0f;
    const bool curseSafe = !item.isCursed;
    return (rarityGood || powerGood) && curseSafe;
}

std::string ItemGenerator::SelectName(const std::vector<std::string>& candidates)
{
    std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
    return candidates[dist(rng)];
}

int ItemGenerator::SelectRarity()
{
    // 重み付き乱数: ノーマルからレジェンダリーまで
    std::discrete_distribution<int> rarityDist{ 50, 30, 15, 4, 1 };
    return rarityDist(rng);
}

float ItemGenerator::GeneratePower(int rarity)
{
    // レア度に比例して威力を強化しつつ揺らぎを持たせる
    std::normal_distribution<float> variance(1.0f + rarity * 0.2f, 0.1f);
    const float base = 50.0f + rarity * 25.0f;
    return base * std::max(0.7f, variance(rng));
}

bool ItemGenerator::GenerateCurseFlag(int rarity)
{
    // レア度が高いほど呪いの確率は低くする
    std::bernoulli_distribution curseDist(std::max(0.05, 0.25 - rarity * 0.05));
    return curseDist(rng);
}