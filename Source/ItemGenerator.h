#pragma once
#include <random>
#include <string>
#include <vector>
#include <memory>

// アイテムを表現する軽量なデータ構造
struct Powerup
{
    std::string name;
    //int rarity;             // 0:ノーマル?4:レジェンダリー Nice
    float power;            // 攻撃力や回復量の目安
    //bool isCursed;          // デメリット付きかどうか
};

struct Books
{
    std::string name;
};
// ランダム生成と簡易評価を担当するユーティリティ
class ItemGenerator
{
public:
    ItemGenerator();
    explicit ItemGenerator(uint32_t seed);

    // 新しいアイテムをランダムに生成

    // 生成されたアイテムが「価値あり」かどうかを判定
    //bool IsValuable(const GeneratedItem& item) const;

private:
    std::mt19937 rng;
    std::vector<std::string> powerupNames;
	std::vector<std::string> bookNames;

    std::string SelectName(const std::vector<std::string>& candidates);
    float GeneratePower(const std::string name);
    void GeneratePowerUps();
	void GenerateBooks();
    //int SelectRarity();
    //bool GenerateCurseFlag(int rarity);

    std::vector<std::unique_ptr<Powerup>> items;
    std::vector<std::unique_ptr<Books>> books;

};