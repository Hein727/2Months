#pragma once
#include <random>
#include <string>
#include <vector>
#include <memory>
#include "Graphics/Model.h"
#include "Character.h"

struct Powerup : public Character
{
    std::string name;
    float power = 0.0f;

    std::unique_ptr<Model> model = std::make_unique<Model>("Data/Model/StorageChest.mdl");;
    DirectX::XMFLOAT3 pos = { 0,0,0 };

    float rotationY = 0.0f;

    float lifetime = 30.0f;     // item exists for 30 seconds

    void Update(float elapsedTime)
    {
		rotation = { 0.0f, rotationY, 0.0f };
        position = pos;
		scale = { 0.02f, 0.02f, 0.02f };

		UpdateTransform();
		model->UpdateTransform(transform);
    }
};

struct Book : public Character
{
    std::string name;

    Model* model = nullptr;
    DirectX::XMFLOAT3 position = { 0,0,0 };

    float rotationY = 0.0f;
    float lifetime = 30.0f;     // same lifetime
};

class ItemGenerator
{
public:
    ItemGenerator();
    explicit ItemGenerator(uint32_t seed);

    void Update(float dt);
    void Render(ID3D11DeviceContext* dc, Shader* shader);

private:
    std::mt19937 rng;

    std::vector<std::unique_ptr<Powerup>> items;
    std::vector<std::unique_ptr<Book>> books;

    std::vector<std::string> powerupNames;
    std::vector<std::string> bookNames;

    float spinSpeed = 2.0f;      // rotate speed
    float bobbingTime = 0.0f;    // animation timer
    float bobHeight = 0.25f;     // bob movement

    // ---- generation helpers ----
    void GeneratePowerUp();
    std::string SelectName(const std::vector<std::string>& list);
    float GeneratePower(const std::string& name);
    DirectX::XMFLOAT3 GeneratePosition();

    // ---- lifecycle ----
    void RemoveExpiredItems();

    static constexpr int MAX_ITEMS = 5;
};
