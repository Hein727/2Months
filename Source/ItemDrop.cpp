#include "ItemDrop.h"
#include "ItemGenerator.h"
#include "Graphics/Shader.h"
#include <cmath>

ItemDrop::ItemDrop()
{
	model[0] = std::make_unique<Model>("Data/Model/Sword/Sword.mdl");
	model[1] = std::make_unique<Model>("Data/Model/StorageChest.mdl");
}


void ItemDrop::Update(float elapsedTime)
{

}

void ItemDrop::Render(ID3D11DeviceContext* dc, Shader* shader)
{


}