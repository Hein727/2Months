#include "SceneTitle.h"
#include "SceneGame.h"
#include "Graphics/Graphics.h"
#include "SceneManager.h"

void SceneTitle::Initialize()
{
	sprite = std::make_unique<Sprite>("Data/Sprite/title_sprite.png");
}

void SceneTitle::Update(float elapsedTime)
{

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
	}

	if(::GetAsyncKeyState(VK_RETURN) & 0x0001)
	SceneManager::Instance().ChangeScene(new SceneGame);
}

void SceneTitle::Finalize()
{

}
