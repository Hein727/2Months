#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Graphics/Sprite.h"

class UI
{
public:
	struct options
	{
		DirectX::XMFLOAT2 position;
		DirectX::XMFLOAT2 scale;
		bool visible;
		bool display_textBox;
		DirectX::XMFLOAT4 color; // RGBA
		DirectX::XMFLOAT2 hitbox_size;
	} option;

	UI(DirectX::XMFLOAT2 position, bool visible = true, bool textBox = false)
	{
		option.position = position;
		option.visible = visible;
		option.display_textBox = textBox;
		option.scale = DirectX::XMFLOAT2(1.0f, 1.0f);
		option.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		option.hitbox_size = DirectX::XMFLOAT2();
	};
	~UI() {};

	// all the setters
	void loadSprite(const char* filePath) {
		sprite = std::make_unique<Sprite>(filePath);
	}

	void setPosition(const DirectX::XMFLOAT2 position) {
		option.position = position;
	}

	void setScale(const DirectX::XMFLOAT2 scale) {
		option.scale = scale;
	}

	void setVisibility(bool isVisible) {
		option.visible = isVisible;
	}

	void setDisplayTextBox(bool display) {
		option.display_textBox = display;
	}

	// all the getters

	DirectX::XMFLOAT2 getPosition() const {
		return option.position;
	}

	DirectX::XMFLOAT2 getScale() const {
		return option.scale;
	}

	bool isVisible() const {
		return option.visible;
	}

	bool isTextBoxDisplayed() const {
		return option.display_textBox;
	}

	void Update(float elapsedTime)
	{
		// Update logic can be added here if needed
	}

	void render(ID3D11DeviceContext* dc)
	{
		if (option.visible && sprite != nullptr)
		{
			sprite->Render(dc,
				option.position.x, option.position.y,
				sprite->GetTextureWidth() * option.scale.x, sprite->GetTextureHeight() * option.scale.y,
				0, 0,
				sprite->GetTextureWidth(), sprite->GetTextureHeight(),
				0,
				option.color.x, option.color.y, option.color.z, option.color.w
			);
		}
	}

private:

	std::unique_ptr<Sprite> sprite;
};