#include "ItemGenerator.h"
#include <chrono>
#include <cmath>
#include <algorithm>

ItemGenerator::ItemGenerator()
{
    uint32_t seed =
        (uint32_t)std::chrono::high_resolution_clock::now()
        .time_since_epoch().count();

    rng.seed(seed);

    powerupNames = { "beast", "fist", "reload", "runner" };
    bookNames = { "book of earth", "book of fire", "book of wind", "book of wood" };

    // Spawn one item and one book to start
	for (int i = 0; i < 5; ++i)
    GeneratePowerUp();
}

ItemGenerator::ItemGenerator(uint32_t seed)
{
    rng.seed(seed);

    powerupNames = { "beast", "fist", "reload", "runner" };
    bookNames = { "book of earth", "book of fire", "book of wind", "book of wood" };

    GeneratePowerUp();
}

void ItemGenerator::Update(float dt)
{
    bobbingTime += dt;
    float bobOffset = std::sin(bobbingTime * 2.0f) * bobHeight;

    // POWERUPS
    for (auto& item : items)
    {
        item->rotationY += spinSpeed * dt;
        item->lifetime -= dt;

		item->pos.y = bobOffset;

		item->rotationY += spinSpeed * dt;

        item->Update(dt);
    }

    RemoveExpiredItems();
}

void ItemGenerator::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (auto& item : items)
        shader->Draw(dc, item->model.get(), { 0.824, 0.412, 0.118, 1.0f });

}

void ItemGenerator::GeneratePowerUp()
{
    auto item = std::make_unique<Powerup>();

    item->name = SelectName(powerupNames);
    item->power = GeneratePower(item->name);
    item->pos = GeneratePosition();
    item->lifetime = 30.0f;

    items.push_back(std::move(item));
}

std::string ItemGenerator::SelectName(const std::vector<std::string>& list)
{
    std::uniform_int_distribution<size_t> dist(0, list.size() - 1);
    return list[dist(rng)];
}

float ItemGenerator::GeneratePower(const std::string& name)
{
    if (name == "beast")
        return std::uniform_real_distribution<float>(10, 25)(rng);
    if (name == "fist")
        return std::uniform_real_distribution<float>(5, 15)(rng);
    if (name == "reload")
        return std::uniform_real_distribution<float>(5, 10)(rng);
    if (name == "runner")
        return std::uniform_real_distribution<float>(10, 25)(rng);

    return 0.0f;
}

DirectX::XMFLOAT3 ItemGenerator::GeneratePosition()
{
    std::uniform_real_distribution<float> distX(-40.0f, 40.0f);
    std::uniform_real_distribution<float> distZ(-40.0f, 40.0f);

    return { distX(rng), 0.0f, distZ(rng) };
}

void ItemGenerator::RemoveExpiredItems()
{
    for (int i = (int)items.size() - 1; i >= 0; --i)
    {
        if (items[i]->lifetime <= 0.0f)
        {
            items.erase(items.begin() + i);
        }
    }

    while (items.size() < MAX_ITEMS)
    {
        GeneratePowerUp();
    }

}
