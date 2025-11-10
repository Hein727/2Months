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
	float distance{ 10.0f };
	float wheel{ 0.0f };
};