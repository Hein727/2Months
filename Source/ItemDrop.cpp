#include "ItemDrop.h"
#include <cmath>

ItemDrop::ItemDrop(const GeneratedItem& data, const DirectX::XMFLOAT3& spawnPos, bool valuable)
	: itemData(data), isValuable(valuable)
{
	model = std::make_unique<Model>("Data/Model/Sword/Sword.mdl");

	position = spawnPos;

	// ‚©‚È‚èŠg‘å‚µ‚Ä‚¨‚­
	scale = { 5.0f, 5.0f, 5.0f };

	baseHeight = spawnPos.y;

	// ‰ñ“]‘¬“x‚Í‚»‚Ì‚Ü‚Ü‚ÅOK
	spinSpeed = 1.5f;
}


void ItemDrop::Update(float elapsedTime)
{
	rotation.y += spinSpeed * elapsedTime;
	bobbingTime += elapsedTime;
	position.y = baseHeight + 0.25f * std::sin(bobbingTime * 2.0f);
}

void ItemDrop::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	if (!model)
	{
		return;
	}

	UpdateTransform();
	model->UpdateTransform(transform);
	shader->Draw(dc, model.get());
}