#pragma once

#include <vector>
#include <set>
#include <SDL.h>
#include <hlslpp/hlsl++.h>

enum ColliderChannel
{
	CH_PLAYER,
	CH_ENEMY,
	CH_PROJECTILE
};

class Collider
{
public:

	Collider(ColliderChannel ch);
	~Collider();

	void Set(int x, int y);

private:
	ColliderChannel channel;
	int x;
	int y;

};

class VelocityMovement
{
public:

	VelocityMovement(void);

	void AddVector(hlslpp::float2 v);
	void StopMovement(void);
	hlslpp::float2 Update(hlslpp::float2 pos, float deltaTime);

private:
	hlslpp::float2 velocity;

};

extern std::set<Collider*> Colliders;