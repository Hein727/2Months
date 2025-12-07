#pragma once
#include "Army.h"
#include "Graphics/Shader.h"

class EnemyManager {
private:
	EnemyManager() {};

public:
	static EnemyManager& GetInstance() 
	{
		static EnemyManager instance;
		return instance;
	}

	~EnemyManager()
	{
		for (Army* enemy : enemies) {
			delete enemy;
		}
		enemies.clear();
	};

	void clear()
	{
		for (Army* enemy : enemies) {
			delete enemy;
		}
		enemies.clear();
	}

	void Update(float deltaTime);
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	void EnemyDefeated();
	void SpawnEnemy(float elapsedTime);
	std::vector<Army*> GetEnemyVector() { return enemies; }
	bool AABBvsAABB(const Army::HitBox& a, const Army::HitBox& b)
	{
		return
			a.min.x <= b.max.x && a.max.x >= b.min.x &&
			a.min.z <= b.max.z && a.max.z >= b.min.z;
	}

	int totalEnemysDefeated = 0;

private:
	std::vector<Army*> enemies;
	int enemyLimit = 5;
	float spawnInterval = 10.0f;

};