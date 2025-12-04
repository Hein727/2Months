#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Units.h"
#include <ctime>
#include <cstdlib>
#include <memory>

class Army
{
public : 

	void SetEnemyArmyMoveSpeed(const float speed)
	{
		moveSpeed = speed;
	}


	Army(const int size = 5, const bool enemy = false, const DirectX::XMFLOAT3 position = { 0, 0, 0})
	{
		srand((unsigned int)time(NULL));

		armyState = START;

		right = DirectX::XMVectorSet(1, 0, 0, 0);
		up = DirectX::XMVectorSet(0, 1, 0, 0);
		forward = DirectX::XMVectorSet(0, 0, 1, 0);

		EnemyType = enemy;
		if (!enemy)
		{
			this->initial_size = size;
			units.reserve(size);
			spawnPosition = position;
		}

		else
		{
			int rng = rand() % 3;
			int randomSize = 0;
			int sign = rand() % 2;

			switch (sign)
			{
			case 0: randomSize = (rand() % size + rng); break;
			case 1: randomSize = (rand() % size - rng); break;
			}

			float dirX = (rand() % 2 == 0) ? -1.0f : 1.0f;
			float dirZ = (rand() % 2 == 0) ? -1.0f : 1.0f;

			float distance = static_cast<float>((rand() % 30) + 30);
			spawnPosition = position;
			spawnPosition.x += dirX * distance;
			spawnPosition.z += dirZ * distance;
			spawnPosition.y = 0.0f;

			this->initial_size = randomSize;

			for (auto& unit : units)
			{
				unit->SetHp(75);
			}

		}

		DirectX::XMFLOAT3 n(0, 1, 0);
		float angle = 0 * 0.017452f;

		orientation = {
			sinf(angle / 2) * n.x,
			sinf(angle / 2) * n.y,
			sinf(angle / 2) * n.z,
			cosf(angle / 2)
		};

	};
	virtual ~Army() 
	{
		enemyUnits.clear();
	};
	void FindCenter();
	void Update(float deltaTime);
	void SortFormation(bool initial = false);
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	void GetTarget();

	DirectX::XMFLOAT3 targetPos = { 0, 0, 0 }; // this will get the position of the mouse click in world space
	DirectX::XMFLOAT3 targetDir = { 0, 0, 0 }; // direction from center to targetPos

	//Army's center position
	DirectX::XMFLOAT3 centerPosition = { 0, 0, 0 };

	DirectX::XMVECTOR forward, up, right;

	void SetDetectionRange(const float range) // might be useful for different army types
	{
		armyDetectionRange = range;
	}

private:
	void AddUnit(const int amount = 0);

	void FindOffsetFromCenter();

	void ArmyMove()
	{
		static bool mouseIsDown = false;
		static bool isDown = false;

		isDown = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
		if (isDown && !mouseIsDown)
		{
			GetTarget();

			armyState = army_state::MOVE;
			if (!regroupped)
			{
				for (auto& unit : units)
				{
					unit->SetState(Unit::state::REGROUP);
				}
			}
			else
			{
				for (auto& unit : units)
				{
					unit->SetState(Unit::state::MAIN_LOGIC);
				}
			}
		}
		mouseIsDown = isDown;
	}


	enum army_state
	{
		START,
		IDLE,
		ROTATE,
		MOVE,
		ATTACK,
		REGROUP,
		WAIT,
	}armyState;

protected :
	int initial_size = 0;

	float moveSpeed = 3.0f;
	const float spacing = 1.0f;
	float formationWidth = 0.0f; // amount of units in a row 
	float formationLength = 1.0f; // amount of rows 
	float turnSpeed = 3.0f; // will be used to get smooth turning with slerp 
	float armyDetectionRange = 5.0f; // range at which the army will detect enemies
	DirectX::XMFLOAT3 spawnPosition = { 0,0,0 };
	DirectX::XMFLOAT4 orientation ={};
	float distance = 0.0f;

	bool EnemyType = false; // This will change the whole army behavior 

	int Id = 0;

	// ユニットの配列
	std::vector<std::unique_ptr<Unit>> units;

	friend class Unit;


public :
	///////Enemy army functions///////

	void EnemyFindPlayerArmy(DirectX::XMFLOAT3& playerPos)
	{
		playerArmyPos = playerPos;
		DirectX::XMVECTOR PlayerPosVec = DirectX::XMLoadFloat3(&playerArmyPos);
		DirectX::XMVECTOR CenterPosVec = DirectX::XMLoadFloat3(&centerPosition);
		DirectX::XMVECTOR dirVec = DirectX::XMVectorSubtract(PlayerPosVec, CenterPosVec);
		DirectX::XMStoreFloat3(&playerArmyDir, DirectX::XMVector3Normalize(dirVec));
	}
	
	
	DirectX::XMFLOAT3 playerArmyPos = { 0,0,0 };
	DirectX::XMFLOAT3 playerArmyDir = { 0,0,0 };
protected:

	//bool move = false;

public :
	///////Targeting functions///////
	void EnemyFindTargetArmy(const Army* army)
	{
		targetArmy = nullptr;
		targetSet = false;

		float distBetweenArmies = 0.0f;
		DirectX::XMVECTOR Enemy = DirectX::XMLoadFloat3(&army->centerPosition);
		DirectX::XMVECTOR Player = DirectX::XMLoadFloat3(&centerPosition);

		distBetweenArmies = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMVectorSubtract(Enemy, Player)));
		float detectionRangeSq = armyDetectionRange * armyDetectionRange;
		if (distBetweenArmies < detectionRangeSq)
		{
			targetArmy = const_cast<Army*>(army);
			targetSet = true;
		}
	}

	void PlayerFindTargetArmy(const std::vector<Army*> enemies)
	{
		if (targetArmy != nullptr && !targetArmy->defeated)
		{
			return;
		}
		else
		{
			targetArmy = nullptr;
			targetSet = false;
		}

		for (auto& enemy : enemies)
		{
			if (enemy->defeated) continue;
			float distBetweenArmies = 0.0f;
			DirectX::XMVECTOR Enemy = DirectX::XMLoadFloat3(&enemy->centerPosition);
			DirectX::XMVECTOR Player = DirectX::XMLoadFloat3(&centerPosition);

			distBetweenArmies = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMVectorSubtract(Enemy, Player)));
			float detectionRangeSq = armyDetectionRange * armyDetectionRange;
			if (distBetweenArmies < detectionRangeSq)
			{
				targetArmy = const_cast<Army*>(enemy);
				targetSet = true;
			}
		}
	}

	std::vector<Unit*> GetUnits() const
	{
		if (units.size() > 0)
		{
			std::vector<Unit*> unitPtrs;
			unitPtrs.clear();
			for (const auto& unit : units)
			{
				unitPtrs.push_back(unit.get());
			}
			return unitPtrs;
		}
	}

	bool getDeafeated() const
	{
		return defeated;
	}

	bool targetSet = false;

	int GetElementType() const
	{
		return elementType;
	}

	int setElementType(const int type)
	{
		if (type >= EARTH && type <= WOOD)
		{
			elementType = type;
		}
		return elementType;
	}

private :

	void UnitTargetting()
	{
		if (targetArmy == nullptr) return;

		enemyUnits = targetArmy->GetUnits();

		if (enemyUnits.empty())
		{
			targetArmy = nullptr;
		}
		
		int targetIndex = 0;

		int numEnemyUnits = static_cast<int>(enemyUnits.size());

		if (numEnemyUnits == 0) return;

		for (auto& unit : units)
		{
			unit->SetTargetUnit(enemyUnits[targetIndex]);
			targetIndex++;
			if(targetIndex >= numEnemyUnits)
				targetIndex = 0;
		}
	}

	void UnitRetargetting(Unit* unit)
	{
		if (enemyUnits.empty())
		{
			unit->SetTargetUnit(nullptr);
			return;
		}

		unit->SetTargetUnit(enemyUnits[rand() % enemyUnits.size()]);
	}

	void RemoveDeadUnits()
	{
		units.erase(
			std::remove_if(units.begin(), units.end(),
				[](const std::unique_ptr<Unit>& unit) { return !unit->IsAlive(); }),
			units.end()
		);

		moralCalculation();
	}


protected:
	Army* targetArmy = nullptr;

	std::vector<Unit*> enemyUnits;

	bool inCombat = false;

	bool defeated = false;

	bool regroupped = false;

	int elementType = EARTH;

	enum ElementType
	{
		EARTH,
		FIRE,
		WIND,
		WOOD
	};

	////////Real hp of the army////////
public :
	float getMoral() const
	{
		return moral;
	}

	int getInitialArmySize() const
	{
		return initial_size;
	}

	int getArmySize() const
	{
		return units.size();
	}

private :

	void moralCalculation()
	{
		if(units.size() <= 0)	
		{
			defeated = true;
			return;
		}

		moral = (static_cast<float>(units.size()) / static_cast<float>(initial_size)) * 100.0f;

		if (moral <= 25.0f)
		{
			defeated = true;
		}
	}


protected:
	float moral;
};

