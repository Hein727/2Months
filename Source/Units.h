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

	float pawnRadius = 50.4f * 0.01f;

public:
	enum state
	{
		IDLE = 0,
		MAIN_LOGIC,
		REGROUP,
		LOCK_FORMATION,
		ATTACK,
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

	void SetAttack(int attack) { this->attack = attack; }

	void SetHp(int hp) { this->hp = hp; }

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

	void SetDirections(const DirectX::XMFLOAT4& orientation)
	{
		DirectX::XMVECTOR orientVec = DirectX::XMLoadFloat4(&orientation);
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(orientVec);
		DirectX::XMFLOAT4X4 oteint4x4;
		DirectX::XMStoreFloat4x4(&oteint4x4, rotationMatrix);

		forward = DirectX::XMFLOAT4(oteint4x4._31, oteint4x4._32, oteint4x4._33, 0.0f);
		right = DirectX::XMFLOAT4(oteint4x4._11, oteint4x4._12, oteint4x4._13, 0.0f);
		up = DirectX::XMFLOAT4(oteint4x4._21, oteint4x4._22, oteint4x4._23, 0.0f);
	}

	void SetCenterPosition(const DirectX::XMFLOAT3& centerPos)
	{
		this->centerPosition = centerPos;
	}

	void LockPositionInFormation()
	{
		regroupTarget = position_in_formation;
	}

	void clearTargetUnit() { this->TargetUnit = nullptr; }

	void SetTargetUnit(Unit* target) { this->TargetUnit = target; }

	Unit* GetTargetUnit() const { return TargetUnit; }

	bool IsInFormation() const { return in_formation; }

	DirectX::XMFLOAT3 GetCenterPosition() const { return centerPosition; }	

	int unitState;

	DirectX::XMFLOAT3 offsetFromCenter;
protected:

	DirectX::XMFLOAT3 position_in_formation;

	DirectX::XMFLOAT3 centerPosition;

	DirectX::XMFLOAT4 forward;

	DirectX::XMFLOAT4 right;

	DirectX::XMFLOAT4 up;

	Unit* TargetUnit = nullptr;

	bool in_formation_check = false;

	DirectX::XMFLOAT3 regroupTarget;

	float attackCooldown = 0.5f;
};