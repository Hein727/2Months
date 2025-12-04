#include "SceneTitle.h"
#include "SceneGame.h"
#include "Graphics/Graphics.h"
#include "SceneManager.h"
#include "CameraControl.h"


void SceneTitle::Initialize()
{
	sprite = std::make_unique<Sprite>("Data/Sprite/title_sprite.png");
	start = std::make_unique<Sprite>("Data/Sprite/start.png");
	tutorial = std::make_unique<Sprite>("Data/Sprite/tutorial.png");
	buttoninfos[0].position = DirectX::XMFLOAT2(177, 350);
	buttoninfos[0].size = DirectX::XMFLOAT2(start->GetTextureWidth() * 0.35, start->GetTextureHeight() * 0.35);
	buttoninfos[1].position = DirectX::XMFLOAT2(630, 350);
	buttoninfos[1].size = DirectX::XMFLOAT2(start->GetTextureWidth() * 0.35, start->GetTextureHeight() * 0.35);
}

void SceneTitle::Update(float elapsedTime)
{
	cursorPos = camera_controls::instance().get_cursor_position_hbuse();

	if (::GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{	
		//START
		if (cursorPos.x >= buttoninfos[0].position.x && cursorPos.x <= buttoninfos[0].position.x + buttoninfos[0].size.x )
		{
			if (cursorPos.y >= buttoninfos[0].position.y && cursorPos.y <= buttoninfos[0].position.y + buttoninfos[0].size.y )
			{
				SceneManager::Instance().ChangeScene(std::make_unique<SceneGame>());
			}
		}
		//TUTORIAL
		if (cursorPos.x >= buttoninfos[1].position.x && cursorPos.x <= buttoninfos[1].position.x + buttoninfos[1].size.x)
		{
			if (cursorPos.y >= buttoninfos[1].position.y && cursorPos.y <= buttoninfos[1].position.y + buttoninfos[1].size.y)
			{
				SceneManager::Instance().ChangeScene(std::make_unique<SceneGame>());
			}
		}
	}
}

void SceneTitle::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	FLOAT clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	dc->ClearRenderTargetView(rtv, clearColor);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);
	

	//2d sprites here 
	{
		sprite->Render(
			dc,
			0, 0, sprite->GetTextureWidth(), sprite->GetTextureHeight(),
			0, 0, sprite->GetTextureWidth(), sprite->GetTextureHeight(),
			0, 
			1, 1, 1, 1);
		tutorial->Render(
			dc,
			buttoninfos[1].position.x, buttoninfos[1].position.y,
			buttoninfos[1].size.x, buttoninfos[1].size.y,
			0, 0, tutorial->GetTextureWidth(), tutorial->GetTextureHeight(),
			0,
			1, 1, 1, 1);
		start->Render(
			dc,
			buttoninfos[0].position.x, buttoninfos[0].position.y,
			buttoninfos[0].size.x, buttoninfos[0].size.y,
			0, 0, tutorial->GetTextureWidth(), tutorial->GetTextureHeight(),
			0,
			1, 1, 1, 1);
	}

	ImGui::Begin("CursorPos");
	ImGui::SliderFloat2("start", &buttoninfos[0].size.x, 0.0f, 1280.0f);
	ImGui::SliderFloat2("tutorial", &buttoninfos[1].size.x, 0.0f, 1280.0f);
	ImGui::End();
}

void SceneTitle::Finalize()
{

}
