#include "Army.h"
#include "Units.h"
#include <ctime>
#include <cstdlib>
#include "CameraControl.h"
#include "Graphics/Graphics.h"
#include "Miscellaneous.h"
#include "Stage.h"
#include <algorithm>

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

void Army::SortFormation(bool initial)
{
	formationWidth = (units.size() > 10) ? 10 : units.size();
	formationLength = (units.size() > 10) ? (units.size() + 9) / 10 : 1;

	// Correct half extents (spacing between units)
	float halfWidth = (formationWidth - 1) * spacing * 0.5f;
	float halfLength = (formationLength - 1) * spacing * 0.5f;

	std::vector<XMFLOAT3> localOffsets;
	localOffsets.reserve(units.size());

	for (int i = 0; i < units.size(); ++i)
	{
		int row = i / formationWidth;
        int col = static_cast<int>(i) % static_cast<int>(formationWidth);

		XMFLOAT3 offset;
		offset.x = (col * spacing) - halfWidth;
		offset.y = 0.0f;
		offset.z = (row * spacing) - halfLength;

		localOffsets.push_back(offset);
	}

	if (initial)
		centerPosition = spawnPosition;
	else
		FindCenter();

	XMMATRIX rot = XMMatrixRotationQuaternion(XMLoadFloat4(&orientation));

	for (int i = 0; i < units.size(); ++i)
	{
		XMVECTOR local = XMLoadFloat3(&localOffsets[i]);

		// Rotate offset, no translation!
		XMVECTOR worldOffset = XMVector3TransformNormal(local, rot);

		XMFLOAT3 finalPos;
		XMStoreFloat3(&finalPos, worldOffset);

		units[i]->SetPositionInFormation({
			centerPosition.x + finalPos.x,
			centerPosition.y + finalPos.y,
			centerPosition.z + finalPos.z
			});

		// Save PURE LOCAL offset for regrouping later
		units[i]->offsetFromCenter = localOffsets[i];
	}
}

void Army::OrientationRevaluation()
{
	XMVECTOR AvgDirVec = XMVectorZero();

	for (auto& unit : units)
	{
		AvgDirVec = DirectX::XMVectorAdd(AvgDirVec, DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&centerPosition), XMLoadFloat3(&unit->GetPosition())));
	}
	
	AvgDirVec = XMVector3Normalize(XMVectorScale(AvgDirVec, 1.0f / units.size()));

	forward = AvgDirVec;
	
	XMVECTOR RightVec = XMVector3Normalize(XMVector3Cross(up, forward));

	right = RightVec;

	XMVECTOR OrientationVec = XMQuaternionRotationMatrix(
		XMMatrixSet(
			XMVectorGetX(RightVec), XMVectorGetY(RightVec), XMVectorGetZ(RightVec), 0,
			XMVectorGetX(up), XMVectorGetY(up), XMVectorGetZ(up), 0,
			XMVectorGetX(AvgDirVec), XMVectorGetY(AvgDirVec), XMVectorGetZ(AvgDirVec), 0,
			0, 0, 0, 1
		));

	XMStoreFloat4(&orientation, OrientationVec);
}

void Army::FindCenter()
{
	if (useTargetPositionAsCenter)
		centerPosition = targetPos;
	else
		centerPosition = units[0]->GetCenterPosition();
}

void Army::Update(float elapsedTime)
{
	/////Player army logic/////

	if (!EnemyType)
	{
		///hard state machine for player army behavior///
		if (idle)
		{
			armyState = army_state::IDLE;
			idle = false;
		}
		else if (moving)
		{
			armyState = army_state::MOVE;
			moving = false;
		}
		else if (attacking)
		{
			armyState = army_state::ATTACK;
			attacking = false;
		}
		else if (wait)
		{
			armyState = army_state::WAIT;
			wait = false;
		}
		else if (rotating)
		{
			armyState = army_state::ROTATE;
			rotating = false;
		}

		switch (armyState)
		{
		case START:
		{
			moralCalculation();
			armyState = army_state::IDLE;
			regroupped = true;
		}
			break;

		case IDLE:
			break;

		case MOVE:
		{
			XMVECTOR orientationVec = DirectX::XMLoadFloat4(&orientation);
			XMMATRIX m = XMMatrixRotationQuaternion(orientationVec);
			XMFLOAT4X4 m4x4 = {};
			DirectX::XMStoreFloat4x4(&m4x4, m);
			right = { m4x4._11, m4x4._12, m4x4._13 };
			up = { m4x4._21, m4x4._22, m4x4._23 };
			forward = { m4x4._31, m4x4._32, m4x4._33 };

			XMVECTOR dir = DirectX::XMLoadFloat3(&targetDir);

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
				DirectX::XMStoreFloat4(&orientation, orientationVec);
			}
			distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), DirectX::XMLoadFloat3(&centerPosition))));
			distance -= moveSpeed * elapsedTime;

			if (distance > 0.0f)
			{
				centerPosition.x += targetDir.x * moveSpeed * elapsedTime;
				centerPosition.z += targetDir.z * moveSpeed * elapsedTime;
			}
			else
			{
				if (targetArmy != nullptr)
				{
					attacking = true;
					break;
				}
				else
				{
					idle = true;
					break;
				}
			}
		}
		break;

		case ATTACK:
		{
			if (!inCombat && targetArmy != nullptr)
			{
				UnitTargetting();
				inCombat = !targetArmy->GetUnits().empty();
				if (inCombat)
				{
					for (auto& unit : units)
					{
						unit->SetState(Unit::state::ATTACK);
					}
				}
				regroupped = false;
			}
			wait = true;
		}
			break;
		case WAIT:
		{
			if(targetArmy != nullptr)
			{
				if (!targetArmy->defeated)
				{
					for (auto& unit : units)
					{
						if (unit->GetTargetUnit() == nullptr)
							UnitRetargetting(unit.get());
					}
				}
				else
				{
					idle = true;
				}
			}
		}
		break;
		}

		ArmyMove();

		for (auto& unit : units)
		{
			unit->SetCenterPosition(centerPosition);
			unit->SetDirections(orientation);
			unit->Update(elapsedTime);
		}

		if (targetArmy == nullptr || targetArmy->defeated)
		{
			targetArmy = nullptr;
			targetSet = false;
			inCombat = false;
			for (auto& unit : units)
			{
				unit->clearTargetUnit();
			}
		}

		if (!regroupped)
		{
			int units_not_in_formation = units.size();
			if (!locked)
			{
				for (auto& unit : units)
					unit->LockPositionInFormation();

				locked = true;
			}

			for (auto& unit : units)
			{
				units_not_in_formation = unit->IsInFormation() ? units_not_in_formation - 1 : units_not_in_formation;
			}

			if (units_not_in_formation <= 0)
			{
				units_not_in_formation = units.size();
				regroupped = true;
				AddNewUnits = true;
				for (auto& unit : units)
				{
					unit->SetState(Unit::state::MAIN_LOGIC);
				}
				locked = false;
			}
		}

		AddUnitsPlayer();
	}


	/////Enemy army logic/////
	else
	{
		switch (armyState)
		{
		case START:
			moralCalculation();
			regroupped = true;
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
			XMVECTOR orientationVec = DirectX::XMLoadFloat4(&orientation);
			XMMATRIX m = XMMatrixRotationQuaternion(orientationVec);
			XMFLOAT4X4 m4x4 = {};
			DirectX::XMStoreFloat4x4(&m4x4, m);
			right = { m4x4._11, m4x4._12, m4x4._13 };
			up = { m4x4._21, m4x4._22, m4x4._23 };
			forward = { m4x4._31, m4x4._32, m4x4._33 };

			XMVECTOR dir = DirectX::XMLoadFloat3(&playerArmyDir);

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
				DirectX::XMStoreFloat4(&orientation, orientationVec);
			}
			distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), DirectX::XMLoadFloat3(&centerPosition))));
			distance -= moveSpeed * elapsedTime;

			if (distance > 0.0f)
			{
				centerPosition.x += playerArmyDir.x * moveSpeed * elapsedTime;
				centerPosition.z += playerArmyDir.z * moveSpeed * elapsedTime;
			}
			if (distance <= 0.0f)
			{
				if (targetArmy != nullptr)
					armyState = army_state::ATTACK;
				else
				{
					armyState = army_state::IDLE;
					//move = true;
				}
			}
		case ATTACK:
		{
			if (!inCombat && targetArmy != nullptr)
			{
				UnitTargetting();
				inCombat = !targetArmy->GetUnits().empty();
				if (inCombat)
				{
					for (auto& unit : units)
					{
						unit->SetState(Unit::state::ATTACK);
					}
				}
				regroupped = false;
			}
			wait = true;
		}	
			break;
		case WAIT:
		{
			if (targetArmy != nullptr)
			{
				if (!targetArmy->defeated)
				{
					for (auto& unit : units)
					{
						if (unit->GetTargetUnit() == nullptr)
							UnitRetargetting(unit.get());
					}
				}
			}
		}
		break;
		}

		for (auto& unit : units)
		{
			unit->SetCenterPosition(centerPosition);
			unit->SetDirections(orientation);
			unit->Update(elapsedTime);
		}

		if (targetArmy == nullptr || targetArmy->defeated)
		{
			targetArmy = nullptr;
			targetSet = false;
			inCombat = false;
			for (auto& unit : units)
			{
				unit->clearTargetUnit();
			}
		}

		if (!regroupped)
		{
			int units_not_in_formation = units.size();
			if (!locked)
			{
				for (auto& unit : units)
					unit->LockPositionInFormation();

				locked = true;

			}

			for (auto& unit : units)
			{
				units_not_in_formation = unit->IsInFormation() ? units_not_in_formation - 1 : units_not_in_formation;
			}

			if (units_not_in_formation == 0)
			{
				units_not_in_formation = units.size();
				regroupped = true;
				OrientationRevaluation();
				SortFormation(false);
				for (auto& unit : units)
				{
					unit->SetState(Unit::state::MAIN_LOGIC);
				}
				locked = false;
			}
		}
	}

	if (centerPosition.x < -46.5666f)
	{
		centerPosition.x = -46.5666f;
	}
	if (centerPosition.x > 46.5666f)
	{
		centerPosition.x = 46.5666f;
	}
	if (centerPosition.z < -46.5666f)
	{
		centerPosition.z = -46.5666f;
	}
	if (centerPosition.z > 46.5666f)
	{
		centerPosition.z = 46.5666f;
	}

	RemoveDeadUnits();

	armyHitBox = ComputeAABB();

	moralCalculation();
}

void Army::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for (auto& unit : units)
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
		(mouseZ + 1.0f) / 1.0f, 1.0f
	);

	WVP = world * view * proj;
	invWVP = DirectX::XMMatrixInverse(nullptr, WVP);
	worldPos = DirectX::XMVector3TransformCoord(NDCPosition, invWVP);

	DirectX::XMFLOAT3 rayEnd;
	DirectX::XMStoreFloat3(&rayEnd, worldPos);

	DirectX::XMVECTOR rayOrigin = DirectX::XMLoadFloat3(&rayStart);
	DirectX::XMVECTOR rayEndVec = DirectX::XMLoadFloat3(&rayEnd);
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
		DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), DirectX::XMLoadFloat3(&centerPosition)))
	);
}