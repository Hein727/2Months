#include "Player.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "CameraControl.h"
#include "ItemGenerator.h"
#include "EnemyManager.h"
#include "GameClear.h"

void Player::Update(float elapsedTime) 
{
	EnemyManager& enemyManager = EnemyManager::GetInstance();

	if (army->getInitialArmySize() > 0)
	{
		army->Update(elapsedTime);

#if !_DEBUG
		camera_controls::instance().SetFocus(army->centerPosition);
#endif
		army->PlayerFindTargetArmy(enemyManager.GetEnemyVector());
	}

	itemGenerator->Update(elapsedTime);

	static float count = 0.0f;

	count += elapsedTime;

	if (count >= timer)
		army->getPowerups(itemGenerator->GetPowerups());

	if (army->getDeafeated())
	{
		SceneManager::Instance().ChangeScene(std::make_unique<GameClear>());
	}
}

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	army->Render(dc, shader);

	itemGenerator->Render(dc, shader);
}