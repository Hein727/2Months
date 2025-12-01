#pragma once

#include "Graphics/Sprite.h"
#include <string>
#include <memory>

class Text
{
private :
	Text() : font_sprite(std::make_unique<Sprite>("Data/Font/font.png")) {};
public :
	enum Alignments
	{
		LEFT,
		CENTER,
		RIGHT
	};
	static Text& Instance()
	{
		static Text instance;
		return instance;
	}
	~Text() {}
	void Render(std::string text, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT4 color, Alignments align = LEFT);

private:
	std::unique_ptr<Sprite> font_sprite;
};