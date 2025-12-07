#pragma once
#include <memory>
#include "Army.h"
#include "ItemGenerator.h"

class Player
{
private:
	Player()
	{
		army = new Army(5);
		itemGenerator = std::make_unique<ItemGenerator>();
	};

public:
	static Player& getInstance()
	{
		static Player instance;
		return instance;
	};
	~Player() 
	{
		delete army;
	};

	void clear()
	{
		delete army;
		itemGenerator.release();
		army = new Army(5);
		itemGenerator = std::make_unique<ItemGenerator>();
	}

	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	int GetElementType() const { return army->GetElementType(); }
	int GetMorale() const { return army->getMoral(); }
	DirectX::XMFLOAT3 GetPosition() const { return army->centerPosition; }
	Army* GetArmy() const { return army; }	
	int GetArmySize() const { return army->getArmySize(); }
	float reductionAmount() const { return army->moralReduction; }
	bool isBeasting() const { return army->gotBeastPowerup; }


private:
	Army* army;
	std::unique_ptr<ItemGenerator> itemGenerator;

	float timer = 1.0f;
};
