#pragma once
#include "UI.h"
#include <vector>

class UIManager
{
	UIManager();
public :
	static UIManager& GetInstance()
	{
		static UIManager instance;
		return instance;
	}

	~UIManager() {};

	void UpdateUI(float elapsedTime);
	void RenderUI(ID3D11DeviceContext* dc);

private :
	std::vector<std::unique_ptr<UI> > playerHp;
	std::vector<std::unique_ptr<UI> > powerUps;
	std::vector<std::unique_ptr<UI> > element;
	float hpbar_width;
};