#include "Character.h"

void Character::UpdateTransform()
{
	using namespace DirectX;
	XMMATRIX S = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX T = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX R = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMMATRIX world = S * R * T;
	XMStoreFloat4x4(&transform, world);
}