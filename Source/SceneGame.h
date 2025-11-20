#pragma once
#include "Stage.h"
#include "Army.h"
#include <memory>

// ゲームシーン
class SceneGame
{
public:
	SceneGame() {}
	~SceneGame() {}

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render();
	
	void EnemyDefeated();

protected:

	std::unique_ptr<Stage> stage;
	Army* player;
	Army* enemy;
};
