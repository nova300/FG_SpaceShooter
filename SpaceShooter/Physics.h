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
	~Collider();

	void Set(hlslpp::float2 pos, hlslpp::float2 vel = hlslpp::float2(0.0f, 0.0f));
	void Set(hlslpp::float2 pos, hlslpp::float1 radius);

	void Update(void);

	hlslpp::float2 position;
	hlslpp::float1 radius;

	hlslpp::float2 velocity;
	hlslpp::float2 CollisionVector;
	bool CollisionFlag;
	bool Skip;
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
	hlslpp::float2 Update(hlslpp::float2 pos, float deltaTime, Collider* collider);
	bool friction;
	hlslpp::float2 velocity;

};

extern std::set<EnemyCollider*> EnemyColliders;
extern std::set<Collider*> Colliders;