#pragma once
#include "Stage.h"
<<<<<<< Updated upstream
=======
#include "Army.h"
#include "ItemDrop.h"
#include "ItemGenerator.h""
>>>>>>> Stashed changes
#include <memory>
#include <vector>

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

protected:

	std::unique_ptr<Stage> stage;
<<<<<<< Updated upstream
=======
	Army* player;
	Army* enemy;
	std::unique_ptr<ItemGenerator> itemGenerator;
	std::vector<std::unique_ptr<ItemDrop>> itemDrops;

private:
	void SpawnInitialItems();
>>>>>>> Stashed changes
};
