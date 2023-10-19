#pragma once
#include "Graphics.h"
#include "Physics.h"

class PlayerShip
{
public:
	PlayerShip();

	void Update(float deltaTime);

	void Thruster();

	void Fire();

	double angle;
	Sprite sprite;

private:
	hlslpp::float2 position;
	VelocityMovement vm;

	float timer ;
	float clearTimer;
	bool shooting;

};


class Enemy
{
public:
	Enemy(hlslpp::float2 position);
	Enemy(void);
	
	void Update(float deltaTime);
	void Disable();

private:
	hlslpp::float2 position;
	VelocityMovement vm;
	Sprite sprite;
	EnemyCollider collider;
	float timer;
};