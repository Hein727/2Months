#pragma once
#include "Stage.h"
#include "Scene.h"
#include "Army.h"
#include <memory>
#include <vector>
#include "UI.h"
#include "Audio/Audio.h"

// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;
	
	void EnemyDefeated()
	{
		if (enemies.empty()) return;

		enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Army* enemy) {
			if (enemy->getDeafeated())
			{
				delete enemy;
				return true;
			}
			else
			{
				return false;
			}
			}), enemies.end());
	}

	void SpawnEnemy(float elapsedTime)
	{
		static float spawnTimer = 0.0f;
		spawnTimer += elapsedTime;
		if (spawnTimer < 5.0f)
		{
			return;
		}
		if (enemies.size() < enemyLimit)
		{
			Army* enemy = new Army(player->getArmySize(), true, player->centerPosition);
			enemies.push_back(std::move(enemy));
			spawnTimer = 0.0f;
		}
	}


protected:

	std::unique_ptr<Stage> stage;
	Army* player;
	Army* enemy;
	std::vector<Army*> enemies;
	int enemyLimit = 5;

	std::vector<std::unique_ptr<UI> > playerHp;
	std::vector<std::unique_ptr<UI> > powerUps;
	std::vector<std::unique_ptr<UI> > element;
	std::unique_ptr<AudioSource> gamebgm;
};
