#pragma once

#include "Scene.h"
#include <memory>

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

			currentScene = std::move(nextScene);
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
			currentScene = nullptr;
		}
	}

    void ChangeScene(std::unique_ptr<Scene> newScene)
    {
		// First scene ever
		if (!currentScene)
		{
			currentScene = std::move(newScene);
			currentScene->Initialize();
			return;
		}

		// Otherwise queue for change
		nextScene = std::move(newScene);
    }

private:
	//Scene* currentScene = nullptr;
	//Scene* nextScene = nullptr;

	std::unique_ptr<Scene> currentScene;
	std::unique_ptr<Scene> nextScene;
};