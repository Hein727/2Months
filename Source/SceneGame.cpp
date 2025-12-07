#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "CameraControl.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Text.h"
#include "UIManager.h"
#include <SceneManager.h>
#include <SceneTitle.h>

// 初期化  
void SceneGame::Initialize()
{
	stage = std::make_unique<Stage>();
	Player::getInstance().clear();
	EnemyManager::GetInstance().clear();
}

// 終了化
void SceneGame::Finalize()
{
	
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	Player* player = &Player::getInstance();
	EnemyManager* enemyManager = &EnemyManager::GetInstance();
	UIManager* uiManager = &UIManager::GetInstance();

	player->Update(elapsedTime);

	enemyManager->Update(elapsedTime);

	uiManager->UpdateUI(elapsedTime);

	stage->Update(elapsedTime);
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

	DirectX::XMFLOAT3 cameraPos = camera_controls::instance().getPosition();

	rc.cameraPosition = DirectX::XMFLOAT4(cameraPos.x, cameraPos.y, cameraPos.z ,0);

	//Calling Instances 
	Player* player = &Player::getInstance();
	EnemyManager* enemyManager = &EnemyManager::GetInstance();
	UIManager* uiManager = &UIManager::GetInstance();


	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		player->Render(dc, shader);
		enemyManager->Render(dc, shader);
		stage->Render(dc, shader);
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
		uiManager->RenderUI(dc);

		Text::Instance().Render("Enemies Defeated: " + std::to_string(enemyManager->totalEnemysDefeated), { 5, 5 }, { 0.5f, 0.5f }, { 1, 1, 1, 0 });
	}

	// 2DデバッグGUI描画
	{
	}
}