#include "UIManager.h"
#include "Player.h"

UIManager::UIManager()
{
	playerHp.resize(2);
	for (int i = 0; i < 2; i++)
	{
		playerHp[i] = std::make_unique<UI>(DirectX::XMFLOAT2(0, 0));
		switch (i)
		{
		case 0:
			playerHp[i]->loadSprite("Data/Sprite/playerHPbarframe.png");
			playerHp[i]->setPosition(DirectX::XMFLOAT2(375, 0.0f));
			break;
		case 1:
			playerHp[i]->loadSprite("Data/Sprite/playerHPbar.png");
			playerHp[i]->setPosition(DirectX::XMFLOAT2(383, 12.0f));
			break;
		}
		playerHp[i]->setEditTextureSize(true);
	}

	powerUps.resize(4);
	for (int i = 0; i < 4; i++)
	{
		powerUps[i] = std::make_unique<UI>(DirectX::XMFLOAT2(0, 0));
		switch (i)
		{
		case 0:
			powerUps[i]->loadSprite("Data/Sprite/icons_skills_beast.png");
			powerUps[i]->setPosition(DirectX::XMFLOAT2(0.0f, 338.0f));
			break;
		case 1:
			powerUps[i]->loadSprite("Data/Sprite/icons_skills_fist.png");
			powerUps[i]->setPosition(DirectX::XMFLOAT2(0.0f, 248.0f));
			break;
		case 2:
			powerUps[i]->loadSprite("Data/Sprite/icons_skills_reload.png");
			powerUps[i]->setPosition(DirectX::XMFLOAT2(0.0f, 158.0f));
			break;
		case 3:
			powerUps[i]->loadSprite("Data/Sprite/icons_skills_runner.png");
			powerUps[i]->setPosition(DirectX::XMFLOAT2(0.0f, 429));
			break;
		}
	}

	element.resize(4);
	for (int i = 0; i < 4; i++)
	{
		element[i] = std::make_unique<UI>(DirectX::XMFLOAT2(0, 610));
		switch (i)
		{
		case 0:
			element[i]->loadSprite("Data/Sprite/earthicon.png");
			break;
		case 1:
			element[i]->loadSprite("Data/Sprite/fireicon.png");
			break;
		case 2:
			element[i]->loadSprite("Data/Sprite/windicon.png");
			break;
		case 3:
			element[i]->loadSprite("Data/Sprite/woodicon.png");
			break;
		}
	}

	hpbar_width = playerHp[1]->getOriginalTextureSize().x;
}

void UIManager::UpdateUI(float elapsedTime)
{
	Player& player = Player::getInstance();

	switch (player.GetElementType())
	{
		//Earth element
	case 0:
		element[0]->setVisibility(true);
		element[1]->setVisibility(false);
		element[2]->setVisibility(false);
		element[3]->setVisibility(false);
		break;
		//Fire element
	case 1:
		element[0]->setVisibility(false);
		element[1]->setVisibility(true);
		element[2]->setVisibility(false);
		element[3]->setVisibility(false);
		break;
		//Wind element
	case 2:
		element[0]->setVisibility(false);
		element[1]->setVisibility(false);
		element[2]->setVisibility(true);
		element[3]->setVisibility(false);
		break;
		//Wood element
	case 3:
		element[0]->setVisibility(false);
		element[1]->setVisibility(false);
		element[2]->setVisibility(false);
		element[3]->setVisibility(true);
		break;
	}

	// Update player HP UI
	static float currentHpBarWidth = 0;

	currentHpBarWidth = hpbar_width * (player.GetMorale() / static_cast<float>(100));

	playerHp[1]->setEditedTextureSize(DirectX::XMFLOAT2(currentHpBarWidth, playerHp[1]->getOriginalTextureSize().y));
}

void UIManager::RenderUI(ID3D11DeviceContext* dc)
{
	for (auto& i : playerHp)
	{
		i->render(dc);
	}
	for (auto& i : powerUps)
	{
		i->render(dc);
	}
	for (auto& i : element)
	{
		i->render(dc);
	}
}