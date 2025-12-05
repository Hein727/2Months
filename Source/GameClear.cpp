#include "GameClear.h"

void GameClear::Initialize()
{
    gameclearbgm = Audio::Instance().LoadAudioSource("Data/Audio/ゲームクリアシーン音楽.wav");
    gameclearbgm->Play(true);
}

void GameClear::Finalize()
{
    if (gameclearbgm)
    {
        gameclearbgm->Stop();
        gameclearbgm.reset();
    }
}

void GameClear::Update(float elapsedTime)
{
}

void GameClear::Render()
{
}