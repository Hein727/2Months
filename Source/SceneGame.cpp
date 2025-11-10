#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "CameraControl.h"

// 初期化  
void SceneGame::Initialize()
{
	stage = std::make_unique<Stage>();

	player = new Army(5, false); // 味方軍

	enemy = new Army(3, true, {10.0f , 0.0f, 10.0f}); // 敵軍
}

// 終了化
void SceneGame::Finalize()
{
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	stage->Update(elapsedTime);

	player->Update(elapsedTime);

	if (enemy != nullptr)
	{
		enemy->EnemyFindPlayerArmy(player->centerPosition);
	}

	//enemy->Update(elapsedTime);
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
		player->Render(dc, shader);
		enemy->Render(dc, shader);
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
		ImGui::Begin("Player");
        ImGui::SliderFloat3("Target :", reinterpret_cast<float*>(&player->targetPos.x), -100.0f, 100.0f);
        ImGui::SliderFloat3("TargetDir :", reinterpret_cast<float*>(&player->targetDir.x), -100.0f, 100.0f);
        ImGui::SliderFloat3("CenterPos :", reinterpret_cast<float*>(&player->centerPosition.x), -100.0f, 100.0f);
        ImGui::SliderFloat3("Forward :", reinterpret_cast<float*>(&player->forward), -100.0f, 100.0f);
		ImGui::End();

		ImGui::Begin("Enemy");
		ImGui::SliderFloat3("Target :", reinterpret_cast<float*>(&enemy->playerArmyPos.x), -100.0f, 100.0f);
		ImGui::SliderFloat3("TargetDir :", reinterpret_cast<float*>(&enemy->playerArmyDir.x), -100.0f, 100.0f);
		ImGui::SliderFloat3("CenterPos :", reinterpret_cast<float*>(&enemy->centerPosition.x), -100.0f, 100.0f);
		ImGui::SliderFloat3("Forward :", reinterpret_cast<float*>(&enemy->forward), -100.0f, 100.0f);
		ImGui::End();
	}
}
