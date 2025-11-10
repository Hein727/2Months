#pragma once
#include <directXmath.h>
using namespace DirectX;

bool SamePosition(const XMFLOAT3& pos, const XMFLOAT3& otherPos)
{
	return (pos.x == otherPos.x) && (pos.y == otherPos.y) && (pos.z == otherPos.z);
}

