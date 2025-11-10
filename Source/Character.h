#pragma once
#include "Graphics/Model.h"
#include "Graphics/Shader.h"
#include <DirectXMath.h>

class Character
{
public:

	Character() {};
	virtual ~Character() {};
	void UpdateTransform();

protected :

	DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT4X4 transform =
	{
		1, 0, 0, 1,
		0, 1, 0, 1,
		0, 0, 1, 1,
		1, 1, 1, 1,
	};
};