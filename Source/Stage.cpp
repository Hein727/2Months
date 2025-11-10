#include "Stage.h"

void Stage::Update(float elapsedTime)
{
	using namespace DirectX;
	XMMATRIX S = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX T = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX R = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMMATRIX world = S * R * T;
	XMStoreFloat4x4(&transform, world);

	model->UpdateTransform(transform);
}

void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	if (model)
	{
		shader->Draw(dc, model.get());
	}
}