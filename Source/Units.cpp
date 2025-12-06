#include "Units.h"
#include "CameraControl.h"
#include "Army.h"

Unit::Unit()
{
	model = std::make_unique<Model>("Data/Model/WhitePawn.mdl");

	hp = 100;
	attack = 25;
	id = 0;
	alive = true;
	unitState = IDLE;

	Character::scale = { 0.01f, 0.01f, 0.01f };
}

void Unit::Update(float elapsedTime)
{
	

	if (hp <= 0)
	{
		alive = false;
	}

	////ユニットのロジック更新
	switch (unitState)
	{
	case IDLE:
		position = position_in_formation;
		in_formation = true;
		break;
	case MAIN_LOGIC:
	{
		DirectX::XMVECTOR worldPos = DirectX::XMVectorAdd(
			DirectX::XMLoadFloat3(&centerPosition),
			DirectX::XMVectorAdd(
				DirectX::XMVectorScale(DirectX::XMLoadFloat4(&right), offsetFromCenter.x),
				DirectX::XMVectorScale(DirectX::XMLoadFloat4(&forward), offsetFromCenter.z)
			)
		);
		DirectX::XMStoreFloat3(&position, worldPos);
		position.y = 0.0f;
		break;
	}
	case ATTACK:
	{
		if (TargetUnit == nullptr)
		{
			unitState = REGROUP;
			return;
		}
		else if(!TargetUnit->alive)
		{
			unitState = REGROUP;
			return;
		}
		DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
		DirectX::XMVECTOR targetPos = DirectX::XMLoadFloat3(&TargetUnit->position);
		DirectX::XMVECTOR direction = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(targetPos, pos));
		float distance = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMVectorSubtract(targetPos, pos)));
		float sqRadius = pawnRadius * pawnRadius;
		if (distance < sqRadius)
		{
			if (attackCooldown < 0.0f)
			{
				TargetUnit->TakeDamage(attack);
				attackCooldown = 0.5f;
			}
			attackCooldown -= elapsedTime;
		}
		else
		{
			DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR newPos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(direction, 4.0f * elapsedTime));
			DirectX::XMStoreFloat3(&position, newPos);
			position.y = 0.0f;
		}
		in_formation = false;
		break;
	}
	case REGROUP:
	{
		DirectX::XMVECTOR dir = DirectX::XMVectorSubtract(
			DirectX::XMLoadFloat3(&regroupTarget),
			DirectX::XMLoadFloat3(&position)
		);

		float distanceSq = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(dir));

		if (distanceSq < 0.05f)
		{
			unitState = IDLE;
			in_formation = true;
		}
		else
		{
			dir = DirectX::XMVector3Normalize(dir);
			DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR newPos = DirectX::XMVectorAdd(
				pos, DirectX::XMVectorScale(dir, 4.0f * elapsedTime)
			);
			DirectX::XMStoreFloat3(&position, newPos);
			position.y = 0.0f;
		}
	}
	break;
	}

	if (hp <= 0)
	{
		alive = false;
	}

	if (unitState != REGROUP)
	{
		DirectX::XMVECTOR centerVec = DirectX::XMLoadFloat3(&centerPosition);
		DirectX::XMVECTOR rightVec = DirectX::XMLoadFloat4(&right);
		DirectX::XMVECTOR upVec = DirectX::XMLoadFloat4(&up);
		DirectX::XMVECTOR forwardVec = DirectX::XMLoadFloat4(&forward);
		DirectX::XMVECTOR worldPos;
		float localX = offsetFromCenter.x;
		float localY = offsetFromCenter.y;
		float localZ = offsetFromCenter.z;
		{
			using namespace DirectX;
			XMVECTOR worldOffset = XMVectorScale(rightVec, localX) +
				XMVectorScale(upVec, localY) +
				XMVectorScale(forwardVec, localZ);

			worldPos = centerVec + worldOffset;
		}
		DirectX::XMStoreFloat3(&position_in_formation, worldPos);
		position_in_formation.y = 0.0f;
	}

	//モデルの更新
	UpdateTransform();
	model->UpdateTransform(transform);
}

void Unit::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	if (model)
	{
		shader->Draw(dc, model.get());
	}
}