#pragma once

class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};

	virtual void Initialize() = 0;

	virtual void Update(float elapsedTime) = 0;

	virtual void Render() = 0;

	virtual void Finalize() = 0;	
};