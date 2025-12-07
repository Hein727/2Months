#include "GameOver.h"
#include "SceneManager.h"
#include "Graphics/Graphics.h"
#include "SceneTitle.h"

void GameOver::Initialize()
{
	gameoverbgm = Audio::Instance().LoadAudioSource("Data/Audio/ゲームオーバーシーン音楽.wav");
	sprite = std::make_unique<Sprite>("Data/Sprite/gameover.png");
	gameoverbgm->Play(true);
}

void GameOver::Finalize()
{
    if (gameoverbgm)
    {
        gameoverbgm->Stop();
        gameoverbgm.reset();
    }
}

void GameOver::Update(float elapsedTime)
{
    if (::GetAsyncKeyState(VK_RETURN) & 0x8000 || ::GetAsyncKeyState(VK_SPACE) & 0x8000 || ::GetAsyncKeyState(VK_LBUTTON) & 0x8000)
        SceneManager::Instance().ChangeScene(std::make_unique<SceneTitle>());
}

void GameOver::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    FLOAT clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    dc->ClearRenderTargetView(rtv, clearColor);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    sprite->Render(
        dc,
        0, 0, 1280, 720,
        0, 0, sprite->GetTextureWidth(), sprite->GetTextureHeight(),
        0,
        1, 1, 1, 1);
}