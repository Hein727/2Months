#include "Text.h"
#include "Graphics/Graphics.h"

void Text::Render(std::string text, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT4 color, Alignments align)
{
	float tw = static_cast<float>(font_sprite->GetTextureWidth() / 16.0f);
	float th = static_cast<float>(font_sprite->GetTextureHeight() / 16.0f);
	float cursor = 0.0f;
	// Handle alignment
	float text_width = tw * static_cast<float>(text.length()) * scale.x;
	float text_height = th * scale.y;
	switch (align)
	{
		case LEFT:
			pos.y -= text_height / 2;
			break;
		case CENTER:
		{
			pos.x -= text_width / 2;
			pos.y -= text_height / 2;
			break;
		}
		case RIGHT:
		{
			pos.y -= text_height / 2;
			pos.x -= text_width;
			break;
		}
	}
	for (const auto& c : text)
	{
		float sx = tw * (c & 0x0F);
		float sy = th * (c >> 4);

		font_sprite->Render(
			Graphics::Instance().GetDeviceContext(),
			pos.x + cursor, pos.y,
			tw * scale.x, th * scale.y,   
			sx, sy, tw, th,           
			0.0f,
			color.x, color.y, color.z, color.w
		);

		cursor += tw * scale.x;
	}
}