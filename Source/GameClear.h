#pragma once
#include "Scene.h"
#include "Audio/Audio.h"

class GameClear : public Scene
{
public:
    GameClear() {};
    ~GameClear() override {};
    // ‰Šú‰»
    void Initialize() override;

    // I—¹‰»
    void Finalize() override;

    // XVˆ—
    void Update(float elapsedTime) override;

    // •`‰æˆ—
    void Render() override;

private:
    std::unique_ptr<AudioSource> gameclearbgm;

};
