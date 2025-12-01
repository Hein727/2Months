#pragma once

#include "Scene.h"

class SceneManager
{
private :
	SceneManager() {};
public:
	~SceneManager() {};

	static SceneManager& Instance()
	{
		static SceneManager Instance;
		return Instance;
	}
	
	void Update(float elapsedTime)
	{
		if (nextScene != nullptr)
		{
			currentScene->Finalize();

			currentScene = nextScene;
			nextScene = nullptr;

			currentScene->Initialize();
		}

		if (currentScene != nullptr)
		{
			currentScene->Update(elapsedTime);
		}
	}

	void Render()
	{
		if(currentScene != nullptr)
		{
			currentScene->Render();
		}
	}

	void Clear()
	{
		if (currentScene != nullptr)
		{
			currentScene->Finalize();
			delete currentScene;
			currentScene = nullptr;
		}
	}

	void ChangeScene(Scene* newScene)
	{
		nextScene = newScene;
		if(currentScene == nullptr)
		{
			currentScene = nextScene;
		}
	}

private:
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;
};