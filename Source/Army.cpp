#include "Army.h"
#include "Units.h"
#include <ctime>
#include <cstdlib>
#include "CameraControl.h"
#include "Graphics/Graphics.h"
#include "Miscellaneous.h"
#include "Stage.h"

using namespace DirectX;

void Army::FindOffsetFromCenter()
{
	for (auto& unit : units)
	{
		unit->offsetFromCenter =
		{
			unit->GetPositionInFormation().x - centerPosition.x,
			0.0f,
			unit->GetPositionInFormation().z - centerPosition.z
		};
	}
}

void Army::AddUnit(const int amount)
{
	bool isSeeded = units.empty() ? true : false;

    int adjustedAmount = (amount <= 0) ? size : amount;

    for (int i = 0; i < adjustedAmount; ++i)
    {
        auto unit = std::make_unique<Unit>();
        unit->SetID(Id++);
        units.push_back(std::move(unit));
    }

	size += amount;

    SortFormation(isSeeded);
}
void Army::SortFormation(bool initial)
{
	formationWidth = size > 10 ? 10 : size;                    // max 10 units per row
	formationLength = size > 10 ? size / 10 : 1;

	float halfWidth = formationWidth * spacing * 0.5f;
	float halfLength = formationLength * spacing * 0.5f;

	std::vector<XMFLOAT3> offsets;

	for (int i = 0; i < size; ++i)
	{
		int row = i / formationWidth;
		int col = i % static_cast<int>(formationWidth);

		XMFLOAT3 offset;

		offset.x = (col * spacing) - halfWidth;   // left/right from center
		offset.y = 0.0f;
		offset.z = (row * spacing) - halfLength;  // forward/back from center

		offsets.push_back(offset);
	}

	FindCenter();

	if (initial)
	{
		centerPosition = spawnPosition;
	}

	XMMATRIX rotation = XMMatrixRotationQuaternion(XMLoadFloat4(&orientation));

	for (int i = 0; i < size; ++i)
	{
		XMVECTOR offsetVec = XMLoadFloat3(&offsets[i]);
		XMVECTOR rotatedOffset = XMVector3TransformCoord(offsetVec, rotation);

		XMFLOAT3 finalOffset;
		XMStoreFloat3(&finalOffset, rotatedOffset);

		units[i]->SetPositionInFormation({
			centerPosition.x + finalOffset.x,
			centerPosition.y + finalOffset.y,
			centerPosition.z + finalOffset.z
		});
	}

	FindOffsetFromCenter();
}

void Army::FindCenter()
{
	XMFLOAT3 centering = units.empty() ? spawnPosition : units[0]->GetPosition();

	centerPosition.x = centering.x + (formationWidth / 2.0f) * spacing;
	centerPosition.y = centering.y;
	centerPosition.z = centering.z - (formationLength / 2.0f) * spacing;
}

void Army::Update(float elapsedTime)
{
	//Debug
#if 1
	if (GetAsyncKeyState('R') & 0x0001)
	{
		RemoveUnit();
	}

#endif

	static float sign = 0.0f;
	/////Player army logic/////
	if (!EnemyType)
	{

		switch (armyState)
		{
		case START:
			AddUnit();
			armyState = army_state::IDLE;
			regroupped = true;
			break;
		
		case IDLE:
			break;

		case MOVE:
		{
			XMVECTOR orientationVec = XMLoadFloat4(&orientation);
			XMMATRIX m = XMMatrixRotationQuaternion(orientationVec);
			XMFLOAT4X4 m4x4 = {};
			DirectX::XMStoreFloat4x4(&m4x4, m);
			right = { m4x4._11, m4x4._12, m4x4._13 };
			up = { m4x4._21, m4x4._22, m4x4._23 };
			forward = { m4x4._31, m4x4._32, m4x4._33 };

			XMVECTOR dir = XMLoadFloat3(&targetDir);

			float angle;
			angle = XMVectorGetX(XMVector3AngleBetweenVectors(forward, dir));
			XMVECTOR cross = XMVector3Cross(forward, dir);
			sign = XMVectorGetY(cross) >= 0.0f ? 1.0f : -1.0f; // Y = up axis
			angle *= sign;

			if (fabs(angle) > FLT_EPSILON)
			{
				XMVECTOR q = XMQuaternionRotationAxis(up, angle);

				q = XMQuaternionMultiply(orientationVec, q);
				orientationVec = XMQuaternionSlerp(orientationVec, q, turnSpeed * elapsedTime);
				XMStoreFloat4(&orientation, orientationVec);
			}
			distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(XMLoadFloat3(&targetPos), XMLoadFloat3(&centerPosition))));
			distance -= moveSpeed * elapsedTime;

			if (distance > 0.0f)
			{
				centerPosition.x += targetDir.x * moveSpeed * elapsedTime;
				centerPosition.z += targetDir.z * moveSpeed * elapsedTime;
			}
		}
			break;

		case ATTACK:
			if (!inCombat)
			{
				UnitTargetting();
				inCombat = !enemyUnits.empty();
			}

			if (inCombat)
			{
				for (auto& unit : units)
					unit->SetState(Unit::state::ATTACK);

				armyState = army_state::WAIT;
			}
			break;
		case WAIT:
		{
			if (targetArmy == nullptr)
			{
				inCombat = false;
				for (auto& unit : units)
				{
					unit->SetState(Unit::state::REGROUP);
				}
				armyState = army_state::REGROUP;
			}
		}
		break;

		case REGROUP:
		{
			static int units_not_in_formation = size;
			for(auto& unit : units)
			{
				units_not_in_formation = unit->IsInFormation() ? --units_not_in_formation : units_not_in_formation;
			}

			if(units_not_in_formation == 0)
			{
				units_not_in_formation = size;
				armyState = army_state::IDLE;
				regroupped = true;
			}
		}
		break;
		}

		ArmyMove();
	}


	/////Enemy army logic/////
	else
	{
		switch (armyState)
		{
		case START:
			AddUnit();
			armyState = army_state::IDLE;
			break;
		case IDLE:
			if (fabs(playerArmyDir.x) > 0.001f || fabs(playerArmyDir.z) > 0.001f)
			{
				armyState = army_state::ROTATE;
				for (auto& unit : units)
				{
					unit->SetState(Unit::state::MAIN_LOGIC);
				}
			}
			break;
		case ROTATE:
			XMVECTOR orientationVec = XMLoadFloat4(&orientation);
			XMMATRIX m = XMMatrixRotationQuaternion(orientationVec);
			XMFLOAT4X4 m4x4 = {};
			DirectX::XMStoreFloat4x4(&m4x4, m);
			right = { m4x4._11, m4x4._12, m4x4._13 };
			up = { m4x4._21, m4x4._22, m4x4._23 };
			forward = { m4x4._31, m4x4._32, m4x4._33 };

			XMVECTOR dir = XMLoadFloat3(&playerArmyDir);

			float angle;
			angle = XMVectorGetX(XMVector3AngleBetweenVectors(forward, dir));
			XMVECTOR cross = XMVector3Cross(forward, dir);
			sign = XMVectorGetY(cross) >= 0.0f ? 1.0f : -1.0f; // Y = up axis
			angle *= sign;

			if (fabs(angle) > FLT_EPSILON)
			{
				XMVECTOR q = XMQuaternionRotationAxis(up, angle);

				q = XMQuaternionMultiply(orientationVec, q);
				orientationVec = XMQuaternionSlerp(orientationVec, q, turnSpeed * elapsedTime);
				XMStoreFloat4(&orientation, orientationVec);
			}
			distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(XMLoadFloat3(&targetPos), XMLoadFloat3(&centerPosition))));
			distance -= moveSpeed * elapsedTime;

			if (distance > 0.0f)
			{
				centerPosition.x += playerArmyDir.x * moveSpeed * elapsedTime;
				centerPosition.z += playerArmyDir.x * moveSpeed * elapsedTime;
			}
			if (distance <= 0.0f)
			{
				armyState = army_state::IDLE;
				move = true;
			}
			break;
		}
	}



	/////check for enemy army/////
	if (targetArmy != nullptr && targetArmy->defeated)
	{
		targetArmy = nullptr;
	}
	
	for (auto& unit : units)
	{
		unit->SetCenterPosition(centerPosition);
		unit->SetDirections(orientation);
		unit->Update(elapsedTime);
	}

	RemoveDeadUnits();
}

void Army::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for(auto& unit : units)
	{
		unit->Render(dc, shader);
	}
}

void Army::GetTarget()
{
	// マウスのスクリーン座標を取得
	POINT target = camera_controls::instance().get_cursor_position();
	float mouseX = static_cast<float>(target.x);
	float mouseY = static_cast<float>(target.y);
	float mouseZ = 0.0f;

	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(&camera_controls::instance().get_projection());
	DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&camera_controls::instance().get_view());
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();

	DirectX::XMVECTOR NDCPosition = DirectX::XMVectorSet(
		2.0f * mouseX / Graphics::Instance().GetScreenWidth() - 1.0f,
		1.0f - 2.0f * (mouseY / Graphics::Instance().GetScreenHeight()),
		mouseZ / 1.0f, 1.0f
	);

	DirectX::XMMATRIX WVP = world * view * proj;
	DirectX::XMMATRIX invWVP = DirectX::XMMatrixInverse(nullptr, WVP);
	DirectX::XMVECTOR worldPos = DirectX::XMVector3TransformCoord(NDCPosition, invWVP);

	DirectX::XMFLOAT3 rayStart;
	DirectX::XMStoreFloat3(&rayStart, worldPos);

	NDCPosition = DirectX::XMVectorSet(
		2.0f * mouseX / Graphics::Instance().GetScreenWidth() - 1.0f,
		1.0f - 2.0f * (mouseY / Graphics::Instance().GetScreenHeight()),
		(mouseZ + 1.0f)/1.0f, 1.0f
	);

	WVP = world * view * proj;
	invWVP = DirectX::XMMatrixInverse(nullptr, WVP);
	worldPos = DirectX::XMVector3TransformCoord(NDCPosition, invWVP);

	DirectX::XMFLOAT3 rayEnd;
	DirectX::XMStoreFloat3(&rayEnd, worldPos);

	DirectX::XMVECTOR rayOrigin = XMLoadFloat3(&rayStart);
	DirectX::XMVECTOR rayEndVec = XMLoadFloat3(&rayEnd);
	DirectX::XMVECTOR rayDir = DirectX::XMVector3Normalize(
		DirectX::XMVectorSubtract(rayEndVec, rayOrigin)
	);

	float originY = DirectX::XMVectorGetY(rayOrigin);
	float dirY = DirectX::XMVectorGetY(rayDir);

	// Prevent divide by zero if the ray is parallel to the ground
	if (fabsf(dirY) > 1e-6f)
	{
		float t = -originY / dirY; // how far along the ray we hit the ground
		DirectX::XMVECTOR hitPos = DirectX::XMVectorAdd(rayOrigin, DirectX::XMVectorScale(rayDir, t));

		DirectX::XMStoreFloat3(&targetPos, hitPos);
	}
	else
	{
		// If ray is parallel, just ignore or set a default
		targetPos = { 0.0f, 0.0f, 0.0f };
	}

	targetPos.y = 0.0f;

	// Make boundary for the stage //

	DirectX::XMStoreFloat3(
		&targetDir,
			DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(XMLoadFloat3(&targetPos), XMLoadFloat3(&centerPosition)))
	);
}