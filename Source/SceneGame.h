#pragma once
#include "Stage.h"
#include "Scene.h"
#include <memory>

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

	void Render() override;

protected:
	std::unique_ptr<Stage> stage;
};