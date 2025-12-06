#include "ItemGenerator.h"
#include <chrono>
#include <cmath>
#include <algorithm>
#include "AttributeSystem.h"

ItemGenerator::ItemGenerator()
{
    const auto now = std::chrono::high_resolution_clock::now();
    const auto seed = static_cast<uint32_t>(now.time_since_epoch().count());
    rng.seed(seed);

    powerupNames = { "beast", "fist", "reload", "runner" };
    bookNames = { "book of earth", "book of fire", "book of wind", "book of wood" };
}

ItemGenerator::ItemGenerator(uint32_t seed)
{
    rng.seed(seed);
    
    powerupNames = { "beast", "fist", "reload", "runner" };
    bookNames = { "book of earth", "book of fire", "book of wind", "book of wood" };
}

void ItemGenerator::GeneratePowerUps()
{
	std::unique_ptr<Powerup> item = std::make_unique<Powerup>();

    item->name = SelectName(powerupNames);
    item->power = GeneratePower(item->name);

	items.push_back(std::move(item));

    //item.rarity = SelectRarity();
    //item.isCursed = GenerateCurseFlag(item.rarity);
}

void ItemGenerator::GenerateBooks()
{
    std::unique_ptr<Books> book = std::make_unique<Books>();

	book->name = SelectName(bookNames);

	books.push_back(std::move(book));
}


std::string ItemGenerator::SelectName(const std::vector<std::string>& candidates)
{
    std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
    return candidates[dist(rng)];
}

float ItemGenerator::GeneratePower(const std::string name)
{
    // レア度に比例して威力を強化しつつ揺らぎを持たせる really cool 
    //std::normal_distribution<float> variance(1.0f * 0.2f, 0.1f);
    //const float base = 50.0f * 25.0f;
    //return base * std::max(0.7f, variance(rng));
    
    if (name == powerupNames[0])
    {
        std::uniform_real_distribution<float> powerDist(10.0f , 25.0f);
        return powerDist(rng);
    }
    else if (name == powerupNames[1])
    {
        std::uniform_real_distribution<float> powerDist(5.0f, 15.0f);
        return powerDist(rng);
    }
    else if(name == powerupNames[2])
    {
        std::uniform_real_distribution<float> powerDist(5.0f, 10.0f);
        return powerDist(rng);
    }
    else if (name == powerupNames[3])
    {
        std::uniform_real_distribution<float> powerDist(10.0f, 25.0f);
        return powerDist(rng);
    }
    else
    {
        return 0.0f;
    }
}


//bool ItemGenerator::IsValuable(const GeneratedItem& item) const
//{
//    // レア度と性能が高く、呪われていなければ「価値あり」とみなす
//    const bool rarityGood = item.rarity >= 3; // Epic 以上
//    const bool powerGood = item.power >= 120.0f;
//    const bool curseSafe = !item.isCursed;
//    return (rarityGood || powerGood) && curseSafe;
//}

//int ItemGenerator::SelectRarity()
//{
//    // 重み付き乱数: ノーマルからレジェンダリーまで
//    std::discrete_distribution<int> rarityDist{ 50, 30, 15, 4, 1 };
//    return rarityDist(rng);
//}

//bool ItemGenerator::GenerateCurseFlag(int rarity)
//{
//    // レア度が高いほど呪いの確率は低くする
//    std::bernoulli_distribution curseDist(std::max(0.05, 0.25 - rarity * 0.05));
//    return curseDist(rng);
//}