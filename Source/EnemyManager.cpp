#include "EnemyManager.h"
#include "Player.h"

void EnemyManager::EnemyDefeated()
{
	if (enemies.empty()) return;

	std::vector<Army*> toDelete;

	// First collect defeated armies
	enemies.erase(
		std::remove_if(enemies.begin(), enemies.end(),
			[&](Army* enemy)
			{
				if (enemy->getDeafeated())
				{
					toDelete.push_back(enemy);
					return true;    // remove from vector only
				}
				return false;
			}),
		enemies.end()
	);

	// Delete after vector erase (SAFE)
	for (Army* e : toDelete)
	{
		delete e;
	}
}

void EnemyManager::SpawnEnemy(float elapsedTime)
{
	Player& player = Player::getInstance();

	static float spawnTimer = 1.0f;
	spawnTimer -= elapsedTime;
	//if (::GetAsyncKeyState(VK_SPACE) & 0x8000)
	if (enemies.size() < enemyLimit && spawnTimer <= 0.0f)
	{
		Army* enemy = new Army(player.GetArmySize(), true, player.GetPosition());
		enemies.push_back(std::move(enemy));
		spawnTimer = spawnInterval;
	}
}

void EnemyManager::Update(float elapsedTime)
{
	Player& player = Player::getInstance();
	SpawnEnemy(elapsedTime);
	if (enemies.size() > 0)
	{
		for (auto& enemy : enemies)
		{
			enemy->EnemyFindPlayerArmy(player.GetPosition());
			enemy->EnemyFindTargetArmy(player.GetArmy());
			enemy->Update(elapsedTime);
		}
	}
	EnemyDefeated();
}


void EnemyManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for(auto& enemy : enemies)
	{
		enemy->Render(dc, shader);
	}
}
