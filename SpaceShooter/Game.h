#pragma once
#include "Graphics.h"
#include "Physics.h"

extern int scoreDisplay;
extern int finalScore;
extern bool nuke;
extern unsigned int gameTick;

class PlayerShip
{
public:
	PlayerShip();

	void Update(float deltaTime);

	void Thruster();

	void Fire();

	void ResetPos();

	static hlslpp::float2 Position;

	double angle;
	Sprite sprite;

private:
	hlslpp::float2 position;
	VelocityMovement vm;
	Collider collider;

	float timer ;
	float clearTimer;
	bool shooting;
	int score;
	bool dead;

	float deadTimer;
	int deadDelay;


};


class Enemy
{
public:
	Enemy(hlslpp::float2 position);
	Enemy(void);
	
	int Update(float deltaTime);
	void Destroy();

private:
	void Reset();
	hlslpp::float2 position;
	VelocityMovement vm;
	Sprite sprite;
	EnemyCollider collider;
	bool Destroyed;
	float timer;
	int delay;
	int tickOffset;
};