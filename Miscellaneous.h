#pragma once
#include <DirectXMath.h>

bool SamePosition(const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2)
{
	bool sameX = (pos1.x >= pos2.x - 10.0f) && (pos1.x <= pos2.x + 10.0f);

	return sameX && pos1.y == pos2.y && pos1.z == pos2.z;
}

