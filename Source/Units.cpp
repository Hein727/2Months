#include "Units.h"
#include "CameraControl.h"
#include "Army.h"

Unit::Unit() 
{ 
	model = std::make_unique<Model>("Data/Model/WhitePawn.mdl");

    hp = 100;
    attack = 50;
    id = 0;
    alive = true;
	unitState = IDLE;

	Character::scale = { 0.01f, 0.01f, 0.01f };
}

void Unit::Update(float elapsedTime)
{
	if(hp <= 0)
	{
		alive = false;
	}

	////ユニットのロジック更新
	switch (unitState)
	{
	case IDLE:
		position = position_in_formation;
		break;
	case MAIN_LOGIC:
		DirectX::XMVECTOR worldPos = DirectX::XMVectorAdd(
			DirectX::XMLoadFloat3(&centerPosition),
			DirectX::XMVectorAdd(
				DirectX::XMVectorScale(right, offsetFromCenter.x),
				DirectX::XMVectorScale(forward, offsetFromCenter.z)
			)
		);
		DirectX::XMStoreFloat3(&position, worldPos);
		break;
	case ATTACK:
		DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
		DirectX::XMVECTOR targetPos = DirectX::XMLoadFloat3(&TargetUnit->position);
		DirectX::XMVECTOR direction = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(targetPos, pos));
		float distance = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMVectorSubtract(targetPos, pos)));
		float sqRadius = pawnRadius * pawnRadius;
		if (distance < sqRadius)
		{
			TargetUnit->TakeDamage(attack);
		}
		else
		{
			DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR newPos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(direction, 4.0f * elapsedTime));
			DirectX::XMStoreFloat3(&position, newPos);
		}
		break;
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