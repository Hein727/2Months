#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "CameraControl.h"
#include <random>

// 初期化
void SceneGame::Initialize()
{
	stage = std::make_unique<Stage>();
<<<<<<< Updated upstream
=======

	player = new Army(20, false); // 味方軍

	enemy = new Army(3, true, { 30.0f , 0.0f, 30.0f }); // 敵軍

	enemy->SetEnemyArmyMoveSpeed(0.5f);

	itemGenerator = std::make_unique<ItemGenerator>();
	SpawnInitialItems();
>>>>>>> Stashed changes
}

// 終了化
void SceneGame::Finalize()
{
	itemDrops.clear();

	delete player;
	player = nullptr;

	if (enemy != nullptr)
	{
		delete enemy;
		enemy = nullptr;
	}
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	stage->Update(elapsedTime);
<<<<<<< Updated upstream
=======

	player->Update(elapsedTime);

	for (auto& drop : itemDrops)
	{
		drop->Update(elapsedTime);
	}


#if !_DEBUG
	camera_controls::instance().SetFocus(player->centerPosition);
#endif
	if (enemy != nullptr)
	{
		enemy->EnemyFindPlayerArmy(player->centerPosition);
	}
	
	static float timer = 3.0f;

	if (timer <= 0.01f)
	{
		player->FindTargetArmy(enemy);

		enemy->FindTargetArmy(player);

		if (enemy != nullptr)
		{
			enemy->FindTargetArmy(player);
		}

	}

	enemy->Update(elapsedTime);

	timer -= elapsedTime;
>>>>>>> Stashed changes
}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };     // RGBA(0.0`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };        // Cgi)

	rc.view = camera_controls::instance().get_view();
	rc.projection = camera_controls::instance().get_projection();

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		stage->Render(dc, shader);
<<<<<<< Updated upstream
=======

		for (auto& drop : itemDrops)
		{
			drop->Render(dc, shader);
		}

		player->Render(dc, shader);
		enemy->Render(dc, shader);
		if (enemy)
		{
			enemy->Render(dc, shader);
		}
>>>>>>> Stashed changes
		shader->End(dc);

	}

	// 3Dデバッグ描画
	{
		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2Dスプライト描画
	{

	}

	// 2DデバッグGUI描画
	{
<<<<<<< Updated upstream

	}
}
=======
		ImGui::Begin("Player");
		ImGui::SliderFloat3("Target :", reinterpret_cast<float*>(&player->targetPos.x), -100.0f, 100.0f);
		ImGui::SliderFloat3("TargetDir :", reinterpret_cast<float*>(&player->targetDir.x), -100.0f, 100.0f);
		ImGui::SliderFloat3("CenterPos :", reinterpret_cast<float*>(&player->centerPosition.x), -100.0f, 100.0f);
		ImGui::SliderFloat3("Forward :", reinterpret_cast<float*>(&player->forward), -100.0f, 100.0f);
		ImGui::Text("TargetSet :%s", player->targetSet ? "true" : "false");
		ImGui::End();

		ImGui::Begin("Enemy");
		if (enemy)
		{
			ImGui::SliderFloat3("Target :", reinterpret_cast<float*>(&enemy->playerArmyPos.x), -100.0f, 100.0f);
			ImGui::SliderFloat3("TargetDir :", reinterpret_cast<float*>(&enemy->playerArmyDir.x), -100.0f, 100.0f);
			ImGui::SliderFloat3("CenterPos :", reinterpret_cast<float*>(&enemy->centerPosition.x), -100.0f, 100.0f);
			ImGui::SliderFloat3("Forward :", reinterpret_cast<float*>(&enemy->forward), -100.0f, 100.0f);
			ImGui::Text("TargetSet :%s", enemy->targetSet ? "true" : "false");
		}
		else
		{
			ImGui::Text("Enemy defeated");
		}
		ImGui::End();

		ImGui::Begin("Items");
		for (size_t i = 0; i < itemDrops.size(); ++i)
		{
			const auto& data = itemDrops[i]->GetData();
			ImGui::Text("[%zu] %s / %s", i + 1, data.category.c_str(), data.name.c_str());
			ImGui::Text("Rarity:%d  Power:%.1f  %s", data.rarity, data.power, data.isCursed ? "Cursed" : "Safe");
			ImGui::Text("Valuable : %s", itemDrops[i]->IsValuable() ? "Yes" : "No");
			ImGui::Separator();
		}
		ImGui::End();
	}
}

void SceneGame::EnemyDefeated()
{
	delete enemy;
	enemy = nullptr;
}

void SceneGame::SpawnInitialItems()
{
	if (!stage)
	{
		return;
	}

	std::mt19937 rng(static_cast<uint32_t>(std::random_device{}() & 0xFFFFFFFF));
	std::uniform_real_distribution<float> xDist(stage->stageBoundaryX[0] * 0.4f, stage->stageBoundaryX[1] * 0.4f);
	std::uniform_real_distribution<float> zDist(stage->stageBoundaryZ[0] * 0.4f, stage->stageBoundaryZ[1] * 0.4f);

	itemDrops.clear();

	for (int i = 0; i < 3; ++i)
	{
		GeneratedItem item = itemGenerator->GenerateItem();
		bool valuable = itemGenerator->IsValuable(item);
		DirectX::XMFLOAT3 pos{ xDist(rng), 0.5f, zDist(rng) };
		itemDrops.push_back(std::make_unique<ItemDrop>(item, pos, valuable));
	}

}
>>>>>>> Stashed changes
