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
	Army(const int size = 5, const bool enemy = false, const DirectX::XMFLOAT3 centerPosition = {0, 0, 0})
	{
		this->size = size;
		units.reserve(size);
		EnemyType = enemy;
		this->centerPosition = centerPosition;
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
	virtual ~Army() {};
	void FindCenter();
	void Update(float deltaTime);
	void SortFormation(const DirectX::XMFLOAT3 pos = {0.0f, 0.0f, 0.0f});
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
private:
	void AddUnit(const int amount = 0);

	void FindOffsetFromCenter();

	enum army_state
	{
		START,
		IDLE,
		ROTATE,
		MOVE,
		ATTACK,
	}armyState;

protected :
	int size = 0;

	float moveSpeed = 3.0f;
	const float spacing = 1.0f;
	float formationWidth = 0.0f; // amount of units in a row 
	float formationLength = 1.0f; // amount of rows 
	float turnSpeed = 3.0f; // will be used to get smooth turning with slerp 

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
};

