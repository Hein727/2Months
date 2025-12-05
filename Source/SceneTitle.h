#pragma once
#include "Graphics/Sprite.h"
#include "Scene.h"
#include <memory>
#include "Audio/Audio.h"

class SceneTitle : public Scene
{
public:
    SceneTitle() {};
    ~SceneTitle() override {};
    // ‰Šú‰»
    void Initialize() override;

    // I—¹‰»
    void Finalize() override;

    // XVˆ—
    void Update(float elapsedTime) override;

    // •`‰æˆ—
    void Render() override;
private:
    std::unique_ptr<Sprite> sprite;
    std::unique_ptr<Sprite> start;
    std::unique_ptr<Sprite> tutorial;
    std::unique_ptr<AudioSource> titlebgm;
    std::unique_ptr<AudioSource> decisionSe;
    DirectX::XMFLOAT2 cursorPos;
    struct buttonInfo
    {
        DirectX::XMFLOAT2 position;
        DirectX::XMFLOAT2 size;
    }buttoninfos[2];


};