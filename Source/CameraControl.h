#pragma once
#include <DirectXMath.h>
#include <Windows.h>

class camera_controls
{
public:
	static camera_controls& instance()
	{
		static camera_controls instance;
		return instance;
	}

	void Update(HWND hwnd, float elapsedTime);
	void setWheel(float wheel) { this->wheel = wheel; }	
	DirectX::XMFLOAT4X4 get_view() const { return view; }
	DirectX::XMFLOAT4X4 get_projection() const { return projection; }
	POINT get_cursor_position() const { return cursor_position; }
	DirectX::XMFLOAT2 get_cursor_position_hbuse() const
	{
		float scaleX = 1280.0f / 1920.0f;
		float scaleY = 720.0f / 1080.0f;

		// Convert to 720p coordinates
		// Resolution upscale from 1280x720 to 1920x1080
		// used for sprites(2D) hit checking 
		DirectX::XMFLOAT2 cursor720;
		cursor720.x = cursor_position.x * scaleX;
		cursor720.y = cursor_position.y * scaleY;

		return DirectX::XMFLOAT2(static_cast<float>(cursor720.x), static_cast<float>(cursor720.y));
	}
	void SetFocus(const DirectX::XMFLOAT3 new_focus) { focus = new_focus; }	

private :
	camera_controls();
	~camera_controls() {};
	void Update_transform();

	DirectX::XMFLOAT3 position = { 0, 10, -10 };
	DirectX::XMFLOAT3 focus = { 0, 1, 0 };
	DirectX::XMFLOAT3 up = { 0, 1, 0 };
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	POINT cursor_position{ 0, 0 };
	float rotateX{ 0.0f };
	float rotateY{ DirectX::XMConvertToRadians(180.0f) };
	float distance{ 40.0f };
	float wheel{ 0.0f };
};