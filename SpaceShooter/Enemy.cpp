#include "Game.h"

using hlslpp::float2;
using hlslpp::float1;

Enemy::Enemy(void)
{
	Destroyed = false;
	Reset();
}

void Enemy::Reset()
{
	vm.StopMovement();

	delay = rand() % 500;
	timer = delay;

	float x, y;
	y = 0.0f;
	x = (rand() % 200);
	x = x / 100;

	position = float2(x, y);

	float dir = (rand() % 10) - 5;
	dir = dir / 10;

	dir = dir * 0.0002;

	vm.friction = false;
	vm.AddVector(float2(dir, 0.0005));

	sprite.Set(position, 0.0, 2);

	collider.SetPtr(this);
	collider.colliderType = COL_ENEMY;
	collider.Set(position, float1(0.03f));
}

void Enemy::Destroy()
{
	Destroyed = true;
	collider.Skip = true;
	//sprite.Hide = true;
}

int Enemy::Update(float deltaTime)
{
	if (Destroyed)
	{
		timer = timer - deltaTime;
		if (timer < 0.0f) return 1;
		
		if (timer > (delay / 2.0f))
		{
			sprite.Set(3);
		}
		else
		{
			sprite.Set(4);
		}

		return 0;
	}

	sprite.Set(position);

	collider.Set(position, vm.velocity);
	collider.Update();

	position = vm.Update(position, deltaTime, &collider);

	return 0;
}