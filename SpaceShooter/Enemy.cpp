#include "Game.h"

using hlslpp::float2;
using hlslpp::float1;

Enemy::Enemy(void)
{
	Destroyed = false;
	collider = -1;
	Reset();
}

void Enemy::Reset()
{
	vm.StopMovement();

	delay = rand() % 500;
	timer = delay;

	angle = 0.0f;

	tickOffset = rand() % 10;

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

	Collider* col = GetCollider(collider);
	if (col == NULL)
	{
		collider = GetNewCollider();
		col = GetCollider(collider);
	}

	col->Set(this);
	col->Set(position, float1(0.03f));
}

void Enemy::Destroy()
{
	Destroyed = true;
	GetCollider(collider)->Free = true;
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
			sprite.Set(4);
		}
		else
		{
			sprite.Set(5);
		}

		return 0;
	}

	if ((gameTick + tickOffset) % 10 == 0)
	{
		sprite.Set(3);
		float2 moveVector = PlayerShip::Position - position;
		moveVector = hlslpp::normalize(moveVector);
		float1 dot = hlslpp::dot(moveVector, float2(1.0f, 1.0f));
		angle = dot;
		moveVector *= (0.000001f * deltaTime);
		vm.AddVector(moveVector);
	}
	else
	{
		if ((gameTick + tickOffset) % 10 == 1) tickOffset = rand() % 10;
		sprite.Set(2);
	}

	sprite.Set(position, angle - 0.225);


	Collider* col = GetCollider(collider);
	if (col != NULL)
	{
		position = vm.Update(position, deltaTime, col);
		col->Set(position, vm.velocity);
	}



	return 0;
}