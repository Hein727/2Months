#include "SceneTitle.h"
#include "SceneGame.h"
#include "Graphics/Graphics.h"
#include "SceneManager.h"
<<<<<<< Updated upstream
#include <imgui.h>
=======
#include "Text.h"
>>>>>>> Stashed changes

void SceneTitle::Initialize()
{
	sprite = std::make_unique<Sprite>("Data/Sprite/title_sprite.png");
	start = std::make_unique<Sprite>("Data/Sprite/start.png");
	tutorial = std::make_unique<Sprite>("Data/Sprite/tutorial.png");
}

void SceneTitle::Update(float elapsedTime)
{
	::GetCursorPos(&cursorPos);
	HWND hwnd = GetForegroundWindow();
	ScreenToClient(hwnd, &cursorPos);

	if (::GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		//START
		if (cursorPos.x >= 230 && cursorPos.x <= 600)
		{
			if (cursorPos.y >= 520 && cursorPos.y <= 680)
			{
				SceneManager::Instance().ChangeScene(new SceneGame);
			}
		}
		//TUTORIAL
		if (cursorPos.x >= 690 && cursorPos.x <= 1150)
		{
			if (cursorPos.y >= 520 && cursorPos.y <= 680)
			{
				SceneManager::Instance().ChangeScene(new SceneGame);
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
			0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
			0, 0, sprite->GetTextureWidth(), sprite->GetTextureHeight(),
			0, 
			1, 1, 1, 1);
<<<<<<< Updated upstream
		tutorial->Render(
			dc,
			590, 350, 750, 500,
			0, 0, tutorial->GetTextureWidth(), tutorial->GetTextureHeight(),
			0,
			1, 1, 1, 1);
		start->Render(
			dc,
			50, 350, 750, 500,
			0, 0, start->GetTextureWidth(), start->GetTextureHeight(),
			0,
			1, 1, 1, 1);
		
	}

	

	
=======
	Text::Instance().Render("enter", { graphics.GetScreenWidth() / 2, graphics.GetScreenHeight() / 2 }, { 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, Text::Instance().CENTER);
	}


	if(::GetAsyncKeyState(VK_RETURN) & 0x0001)
	SceneManager::Instance().ChangeScene(new SceneGame);
>>>>>>> Stashed changes
}

void SceneTitle::Finalize()
{

}
