#pragma once

#include <vector>
#include <set>
#include <SDL.h>
#include <hlslpp/hlsl++.h>

class Enemy;

Enemy* PhysicsQueryEnemy(hlslpp::float2 pos, hlslpp::float2 dir);

enum ColliderType
{
	COL_UNDEFINED,
	COL_ENEMY
};

class Collider
{
public:

	Collider();
	~Collider();

	void Reset();

	void Set(hlslpp::float2 pos, hlslpp::float2 vel = hlslpp::float2(0.0f, 0.0f));
	void Set(hlslpp::float2 pos, hlslpp::float1 radius);

	void Update(void);
	static void UpdateColliders(void);

	hlslpp::float2 position;
	hlslpp::float1 radius;

	hlslpp::float2 velocity;
	hlslpp::float2 CollisionVector;
	ColliderType CollisionType;
	bool CollisionFlag;
	bool Skip;

	void Set(Enemy* e);
	Enemy* enemy;

	ColliderType colliderType;

	bool Free;
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

//extern std::set<EnemyCollider*> EnemyColliders;
extern std::vector<Collider> Colliders;

int GetNewCollider();
Collider* GetCollider(int index);
