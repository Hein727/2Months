#include "Character.h"

void Character::UpdateTransform()
{
	using namespace DirectX;
	XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX translation = XMMatrixTranslation(position.x, position.y, position.z);

}