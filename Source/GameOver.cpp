#include "GameOver.h"

void GameOver::Initialize()
{
	gameoverbgm = Audio::Instance().LoadAudioSource("Data/Audio/ゲームオーバーシーン音楽.wav");
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
}

void GameOver::Render()
{
}