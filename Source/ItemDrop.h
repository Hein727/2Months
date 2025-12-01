#pragma once

#include "Character.h"
#include "Graphics/Shader.h"
#include "ItemGenerator.h"
#include <DirectXMath.h>
#include <memory>

// 見た目付きのドロップアイテム
class ItemDrop : public Character
{
public:
	ItemDrop(const GeneratedItem& data, const DirectX::XMFLOAT3& spawnPos, bool valuable);
	~ItemDrop() = default;

	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	const GeneratedItem& GetData() const { return itemData; }
	bool IsValuable() const { return isValuable; }

private:
	GeneratedItem itemData;
	std::unique_ptr<Model> model;
	float spinSpeed = 1.5f;
	float baseHeight = 0.0f;
	float bobbingTime = 0.0f;
	bool isValuable = false;
};