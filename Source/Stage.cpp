#include "Stage.h"

void Stage::Update(float elapsedTime)
{

}

void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	if (model)
	{
		shader->Draw(dc, model.get());
	}
}