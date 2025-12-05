#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "CameraControl.h"
#include <SceneManager.h>
#include <SceneTitle.h>

// 初期化  
void SceneGame::Initialize()
{
	stage = std::make_unique<Stage>();

	gamebgm = Audio::Instance().LoadAudioSource("Data/Audio/シーンゲーム音楽２.wav");
	gamebgm->Play(true);

	player = new Army(20, false); // 味方軍
	
	InitUI();
}

// 終了化
void SceneGame::Finalize()
{
	if (gamebgm)
	{
		gamebgm->Stop();
		//gamebgm.reset();
	}
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	stage->Update(elapsedTime);

	if (player->getInitialArmySize() > 0)
	{
		player->Update(elapsedTime);

		SpawnEnemy(elapsedTime);
#if !_DEBUG
		camera_controls::instance().SetFocus(player->centerPosition);
#endif
		if (enemies.size() > 0)
		{
			for (auto& enemy : enemies)
			{
				enemy->EnemyFindPlayerArmy(player->centerPosition);
				enemy->EnemyFindTargetArmy(player);
				enemy->Update(elapsedTime);
			}
		}

		player->PlayerFindTargetArmy(enemies);

		EnemyDefeated();
	}

	if (player->getDeafeated())
	{
		SceneManager::Instance().ChangeScene(std::make_unique<SceneTitle>());
	}

	UpdateUI(elapsedTime);
}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向)

	rc.view = camera_controls::instance().get_view();
	rc.projection = camera_controls::instance().get_projection();

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		stage->Render(dc, shader);
		for(auto& enemy : enemies)
		{
			enemy->Render(dc, shader);
		}
		player->Render(dc, shader);
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
		RenderUI(dc);
	}

	// 2DデバッグGUI描画
	{
		ImGui::Begin("Player");
        ImGui::SliderFloat3("Target :", reinterpret_cast<float*>(&player->targetPos.x), -100.0f, 100.0f);
        ImGui::SliderFloat3("TargetDir :", reinterpret_cast<float*>(&player->targetDir.x), -100.0f, 100.0f);
        ImGui::SliderFloat3("CenterPos :", reinterpret_cast<float*>(&player->centerPosition.x), -100.0f, 100.0f);
        ImGui::SliderFloat3("Forward :", reinterpret_cast<float*>(&player->forward), -100.0f, 100.0f);
		ImGui::Text("TargetSet :%s", player->targetSet ? "true" : "false");
		ImGui::End();

		DirectX::XMFLOAT2 pos[2] = { playerHp[0]->getPosition(), playerHp[1]->getPosition() };

		ImGui::Begin("UI pos");
		ImGui::SliderFloat("bar pos", reinterpret_cast<float*>(&pos[0].y), 0, 720.0f);
		ImGui::SliderFloat("frame pos", reinterpret_cast<float*>(&pos[1].y), 0, 720.0f);
		ImGui::End();
		playerHp[0]->setPosition(pos[0]);
		playerHp[1]->setPosition(pos[1]);

	}
}