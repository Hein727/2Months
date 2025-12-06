#pragma once
#include <memory>
#include "Army.h"

class Player
{
private:
	Player()
	{
		army = new Army(5);
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

	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	int GetElementType() const { return army->GetElementType(); }
	int GetMorale() const { return army->getMoral(); }
	DirectX::XMFLOAT3 GetPosition() const { return army->centerPosition; }
	Army* GetArmy() const { return army; }	
	int GetArmySize() const { return army->getArmySize(); }

private:
	Army* army;
};
