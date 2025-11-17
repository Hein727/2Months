#include "Units.h"
#include "CameraControl.h"
#include "Army.h"

Unit::Unit() 
{ 
	model = std::make_unique<Model>("Data/Model/WhitePawn.mdl");

    hp = 100;
    attack = 10;
    id = 0;
    alive = true;
	unitState = IDLE;

	Character::scale = { 0.01f, 0.01f, 0.01f };
}

void Unit::Update(float elapsedTime)
{
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
	case TARGETING:
		
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