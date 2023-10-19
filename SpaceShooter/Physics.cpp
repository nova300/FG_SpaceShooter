#include "Game.h"

std::set<EnemyCollider*> EnemyColliders;
using hlslpp::float2;
using hlslpp::float1;

EnemyCollider::EnemyCollider()
{
	EnemyColliders.insert(this);
	this->enemy = NULL;
}

Collider::Collider()
{
	radius = 32;
}

EnemyCollider::~EnemyCollider()
{
	EnemyColliders.erase(this);
}

void Collider::Set(float2 pos, int radius)
{
	position = pos;
	this->radius = radius;
}

void Collider::Set(float2 pos)
{
	position = pos;
}

void EnemyCollider::SetPtr(Enemy* e)
{
	this->enemy = e;
}

VelocityMovement::VelocityMovement()
{
	velocity = float2(0.0f, 0.0f);
	friction = true;
}

void VelocityMovement::AddVector(float2 v)
{
	velocity += v;
}

float2 VelocityMovement::Update(float2 pos, float deltaTime)
{
	const float speedlimit = 0.005;
	int d = 70;
	if (velocity.x > speedlimit)
	{
		velocity.x = speedlimit;
	}
	if (velocity.x < -speedlimit)
	{
		velocity.x = -speedlimit;
	}
	if (velocity.y > speedlimit)
	{
		velocity.y = speedlimit;
	}
	if (velocity.y < -speedlimit)
	{
		velocity.y = -speedlimit;
	}
	if (pos.x < 0)
	{
		velocity = float2(-velocity.x, velocity.y);
		pos.x = 0.0f;
		d = 10;
	}
	else if (pos.x > 2.0f)
	{
		velocity = float2(-velocity.x, velocity.y);
		pos.x = 2.0f;
		d = 10;
	}
	else if (pos.y < 0)
	{
		velocity = float2(velocity.x, -velocity.y);
		pos.y = 0;
		d = 10;
	}
	else if (pos.y > 2.0f)
	{
		velocity = float2(velocity.x, -velocity.y);
		pos.y = 2.0f;
		d = 10;
	}

	if (friction) velocity = velocity - (velocity / d);

	float2 result = pos + (velocity * deltaTime);

	return pos + (velocity * deltaTime);
}

Enemy* PhysicsQueryEnemy(hlslpp::float2 pos, hlslpp::float2 dir)
{
	Enemy* e = NULL;
	float bestDot = 0.997f;
	for (EnemyCollider* c : EnemyColliders)
	{
		float2 diff = c->position - pos;
		diff = hlslpp::normalize(diff);
		float dot = hlslpp::dot(dir, diff);
		display1 = dot;
		if (dot > bestDot)
		{
			bestDot = dot;
			e = c->enemy;
		}
	}
	//display1 = bestDot;
	return e;
}