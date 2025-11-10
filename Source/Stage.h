#pragma once
#include "Graphics/Model.h"
#include "Graphics/Shader.h"
#include <memory>

class Stage
{
public :
	Stage()
	{
		model = std::make_unique<Model>("Data/Model/Board.mdl");
	};
	~Stage() {};

	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	float stageBoundaryX[2] = { -46.5666f , 46.5666f };
	float stageBoundaryZ[2] = { -46.5288f , 46.5288f };

protected :

	std::unique_ptr<Model> model;

	DirectX::XMFLOAT3 position = { 0.0f, -5372.0f * 0.0018f, 0.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 scale = { 0.0018f, 0.0018f, 0.0018f };
	DirectX::XMFLOAT4X4 transform =
	{
		1, 0, 0, 1,
		0, 1, 0, 1,
		0, 0, 1, 1,
		1, 1, 1, 1,
	};
};