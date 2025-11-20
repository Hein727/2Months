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

		armyState = START;

		right = DirectX::XMVectorSet(1, 0, 0, 0);
		up = DirectX::XMVectorSet(0, 1, 0, 0);
		forward = DirectX::XMVectorSet(0, 0, 1, 0);

		this->size = size;
		units.reserve(size);
		EnemyType = enemy;
		this->spawnPosition = position;

		srand((unsigned int)time(NULL));

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
	void RemoveUnit()
	{
		int rand = std::rand() % units.size();
		units.erase(units.begin() + rand);
		SortFormation();
	}

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
		}

		if (isDown)
		{
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
	int size = 0;

	float moveSpeed = 3.0f;
	const float spacing = 1.0f;
	float formationWidth = 0.0f; // amount of units in a row 
	float formationLength = 1.0f; // amount of rows 
	float turnSpeed = 3.0f; // will be used to get smooth turning with slerp 
	float armyDetectionRange = 10.0f; // range at which the army will detect enemies
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
	bool GetEnemyArmyMove()
	{
		return move;
	}
	
	
	DirectX::XMFLOAT3 playerArmyPos = { 0,0,0 };
	DirectX::XMFLOAT3 playerArmyDir = { 0,0,0 };
protected:

	bool move = false;

public :
	///////Targeting functions///////
	void FindTargetArmy(const Army* army)
	{
		float distBetweenArmies = 0.0f;
		DirectX::XMVECTOR Enemy = DirectX::XMLoadFloat3(&army->centerPosition);
		DirectX::XMVECTOR Player = DirectX::XMLoadFloat3(&centerPosition);

		distBetweenArmies = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMVectorSubtract(Enemy, Player)));
		float detectionRangeSq = armyDetectionRange * armyDetectionRange;
		if (distBetweenArmies < detectionRangeSq)
		{
			if (!targetSet)
			{
				targetArmy = const_cast<Army*>(army);
				targetSet = true;
				armyState = ATTACK;
			}
		}
		else
		{
			targetArmy = nullptr;
			targetSet = false;
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

	bool targetSet = false;

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

		for (auto& unit : units)
		{
			unit->SetTargetUnit(enemyUnits[targetIndex]);
			targetIndex++;
			if(targetIndex >= numEnemyUnits)
				targetIndex = 0;
		}
	}

	void RemoveDeadUnits()
	{
		units.erase(
			std::remove_if(units.begin(), units.end(),
				[](const std::unique_ptr<Unit>& unit) { return !unit->IsAlive(); }),
			units.end()
		);

		size = static_cast<int>(units.size());

		if(size <= 0)
			defeated = true;	
	}

protected:
	Army* targetArmy = nullptr;

	std::vector<Unit*> enemyUnits;

	bool inCombat = false;

	bool defeated = false;

	bool regroupped = false;
};

