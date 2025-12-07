#pragma once
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Audio/Audio.h"

class GameOver : public Scene
{
public:
    GameOver() {};
    ~GameOver() override {};
    // ‰Šú‰»
    void Initialize() override;

    // I—¹‰»
    void Finalize() override;

    // XVˆ—
    void Update(float elapsedTime) override;

    // •`‰æˆ—
    void Render() override;

private:
    std::unique_ptr<AudioSource> gameoverbgm;

	std::unique_ptr<Sprite> sprite;

};