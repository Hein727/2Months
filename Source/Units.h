#pragma once
#include "Character.h"
#include "Graphics/Model.h"
#include <memory>


class Unit : public Character
{
private:
	int hp, attack;
	uint16_t id;
	bool alive;
	std::unique_ptr<Model> model;
	bool in_formation = false;
	void Initialize();


public:
enum state
{
	IDLE,
	MAIN_LOGIC,
	REGROUP,
	ATTACK,
	DIE
};
	Unit();
	virtual ~Unit() {};
	
	void Update(float elapsedTime);

	void Render(ID3D11DeviceContext* dc, Shader* shader);
	
	//基本的なゲッターとセッター
	int GetAttack() const { return attack; }

	bool IsAlive() const { return alive; }

	uint16_t GetID() const { return id; }	

	void SetID(int newID) { this->id = newID; }

	void SetPositionInFormation(const DirectX::XMFLOAT3& pos) 
	{ 
		this->position_in_formation = pos; 
	}	

	DirectX::XMFLOAT3 GetPositionInFormation() const { return position_in_formation; }

	DirectX::XMFLOAT3 GetPosition() const { return position; }

	void TakeDamage(int damage) 
	{
		hp -= damage;
		if (hp <= 0) {
			alive = false;
			hp = 0;
		}
	}

	void SetState(const state newState) { unitState = newState; }

	void SetDirections(const DirectX::XMVECTOR& forward, const DirectX::XMVECTOR& right) 
	{ 
		this->forward = forward; 
		this->right = right; 
	}

	void SetCenterPosition(const DirectX::XMFLOAT3& centerPos) 
	{ 
		this->centerPosition = centerPos; 
	}

	void SetTargetUnit(Unit* target) { this->TargetUnit = target; }

	int unitState;

	DirectX::XMFLOAT3 offsetFromCenter;
protected:

	DirectX::XMFLOAT3 position_in_formation;

	DirectX::XMFLOAT3 centerPosition;

	DirectX::XMVECTOR forward;

	DirectX::XMVECTOR right;

	Unit* TargetUnit = nullptr;
};