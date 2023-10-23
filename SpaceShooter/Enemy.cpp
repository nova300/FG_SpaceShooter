#include "Game.h"

using hlslpp::float2;
using hlslpp::float1;

Enemy::Enemy(void)
{
	Reset();
}

void Enemy::Reset()
{
	vm.StopMovement();

	float x, y;
	x = (rand() % 200);
	x = x / 200;
	y = (rand() % 200);
	y = y / 200;
	position = float2(x, y);

	vm.friction = false;
	vm.AddVector(float2(0.0002, 0.0001));

	sprite.Set(position, 0.0, 3);

	collider.SetPtr(this);
	collider.Set(position, float1(0.03f));
}

void Enemy::Disable()
{
	timer = 500.0f;
	collider.Skip = true;
	sprite.Hide = true;
}

void Enemy::Update(float deltaTime)
{

	if (timer < 0.0f && sprite.Hide)
	{
		Reset();
		sprite.Hide = false;
		collider.Skip = false;
	}
	else if (timer >= 0.0f && sprite.Hide)
	{
		timer = timer - deltaTime;
		return;
	}

	sprite.Set(position);

	collider.Set(position, vm.velocity);
	collider.Update();

	position = vm.Update(position, deltaTime, &collider);
}