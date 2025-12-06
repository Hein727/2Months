#pragma once
#include "Character.h"
#include "Graphics/Model.h"
#include <DirectXMath.h>
#include <cmath>
#include <memory>

// 見た目付きのドロップアイテム
class ItemDrop 
{
public:
	ItemDrop();
	~ItemDrop() = default;

	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//const GeneratedItem& GetData() const { return itemData; }


	// not needed 
	//bool IsValuable() const { return isValuable; }

private:
	std::unique_ptr<Model> model[2];

	// not needed
	//bool isValuable = false;
};

class ItemDesc : public Character
{
private:
	std::string name;
	float powerup;
	std::unique_ptr<Model> model;	
	float spinSpeed = 1.5f;
	float baseHeight = 0.0f;
	float bobbingTime = 0.0f;

public :
	void setModel(std::unique_ptr<Model> mdl) { model = std::move(mdl); }	

	ItemDesc()
	{
		// かなり拡大しておく
		// Ok
		scale = { 5.0f, 5.0f, 5.0f };

		// 回転速度はそのままでOK
		spinSpeed = 1.5f;
	};
	virtual ~ItemDesc() = default;

	void Update(float elapsedTime) 
	{
		rotation.y += spinSpeed * elapsedTime;
		bobbingTime += elapsedTime;
		position.y = baseHeight + 0.25f * std::sin(bobbingTime * 2.0f);

		UpdateTransform();
		model->UpdateTransform(transform);
	}

	void Render(ID3D11DeviceContext* dc, Shader* shader) 
	{
		shader->Draw(dc, model.get());
	}
};