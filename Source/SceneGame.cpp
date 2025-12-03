#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "CameraControl.h"

// 初期化  
void SceneGame::Initialize()
{
	stage = std::make_unique<Stage>();

	player = new Army(20, false); // 味方軍

	enemy = new Army(player->getArmySize(), true, player->centerPosition); // 敵軍

	playerHp.resize(2);
	for(int i = 0; i < 2; i++)
	{
		playerHp[i] = std::make_unique<UI>(DirectX::XMFLOAT2(0, 0));

		switch (i)
		{
		case 0:
			playerHp[i]->loadSprite("Data/Sprite/playerHPbarframe.png");
			break;
		case 1:
			playerHp[i]->loadSprite("Data/Sprite/playerHPbar.png");
			break;
		}
	}

	powerUps.resize(4);
	for(int i = 0; i < 4; i++)
	{
		powerUps[i] = std::make_unique<UI>(DirectX::XMFLOAT2(0, 0));

		switch (i)
		{
		case 0:
			powerUps[i]->loadSprite("Data/Sprite/icons_skills_beast.png");
			powerUps[i]->setPosition(DirectX::XMFLOAT2(0.0f, 338.0f));
			break;
		case 1:
			powerUps[i]->loadSprite("Data/Sprite/icons_skills_fist.png");
			powerUps[i]->setPosition(DirectX::XMFLOAT2(0.0f, 248.0f));
			break;
		case 2:
			powerUps[i]->loadSprite("Data/Sprite/icons_skills_reload.png");
			powerUps[i]->setPosition(DirectX::XMFLOAT2(0.0f, 158.0f));
			break;
		case 3:
			powerUps[i]->loadSprite("Data/Sprite/icons_skills_runner.png");
			powerUps[i]->setPosition(DirectX::XMFLOAT2(0.0f, 429));
			break;
		}
	}

	element.resize(4);
	for(int i = 0; i < 4; i++)
	{
		element[i] = std::make_unique<UI>(DirectX::XMFLOAT2(0, 610));
		switch (i)
		{
		case 0:
			element[i]->loadSprite("Data/Sprite/earthicon.png");
			break;
		case 1:
			element[i]->loadSprite("Data/Sprite/fireicon.png");	
			break;
		case 2:
			element[i]->loadSprite("Data/Sprite/windicon.png");	
			break;
		case 3:
			element[i]->loadSprite("Data/Sprite/woodicon.png");	
			break;
		}
	}
}

// 終了化
void SceneGame::Finalize()
{
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	stage->Update(elapsedTime);

	if (player->getArmySize() > 0)
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

		switch (player->GetElementType())
		{
			//Earth element
		case 0:
			element[0]->setVisibility(true);	
			element[1]->setVisibility(false);
			element[2]->setVisibility(false);
			element[3]->setVisibility(false);
			break;
			//Fire element
		case 1:
			element[0]->setVisibility(false);
			element[1]->setVisibility(true);
			element[2]->setVisibility(false);
			element[3]->setVisibility(false);
			break;
			//Wind element
		case 2:
			element[0]->setVisibility(false);
			element[1]->setVisibility(false);
			element[2]->setVisibility(true);
			element[3]->setVisibility(false);
			break;
			//Wood element
		case 3:
			element[0]->setVisibility(false);
			element[1]->setVisibility(false);
			element[2]->setVisibility(false);
			element[3]->setVisibility(true);
			break;
		}
	}
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
		/*for (auto& i : playerHp)
		{
			i->render(dc);
		}*/
		for (auto& i : powerUps)
		{
			i->render(dc);
		}
		for (auto& i : element)
		{
			i->render(dc);
		}
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