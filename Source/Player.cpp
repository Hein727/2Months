#include "Player.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "CameraControl.h"
#include "EnemyManager.h"

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

	if (army->getDeafeated())
	{
		SceneManager::Instance().ChangeScene(std::make_unique<SceneTitle>());
	}
}

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	army->Render(dc, shader);

	itemGenerator->Render(dc, shader);
}