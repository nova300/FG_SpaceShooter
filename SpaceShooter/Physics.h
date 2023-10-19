#pragma once

#include <vector>
#include <set>
#include <SDL.h>
#include <hlslpp/hlsl++.h>

class Enemy;

Enemy* PhysicsQueryEnemy(hlslpp::float2 pos, hlslpp::float2 dir);

class Collider
{
public:

	Collider();

	void Set(hlslpp::float2 pos);
	void Set(hlslpp::float2 pos, int radius);

	hlslpp::float2 position;
	int radius;
};

class EnemyCollider : public Collider
{
public:

	EnemyCollider();
	~EnemyCollider();

	void SetPtr(Enemy* e);
	
	Enemy* enemy;
};

class VelocityMovement
{
public:

	VelocityMovement();

	void AddVector(hlslpp::float2 v);
	void StopMovement(void);
	hlslpp::float2 Update(hlslpp::float2 pos, float deltaTime);
	bool friction;

private:
	hlslpp::float2 velocity;


};

extern std::set<EnemyCollider*> Colliders;