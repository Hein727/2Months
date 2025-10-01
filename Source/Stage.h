#pragma once
#include "Graphics/Model.h"
#include "Graphics/Shader.h"
#include <memory>

class Stage
{
public :
	Stage()
	{
		model = std::make_unique<Model>("Data/Model/ExampleStage/ExampleStage.mdl");
	};
	~Stage() {};

	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext* dc, Shader* shader);

protected :

	std::unique_ptr<Model> model;
};