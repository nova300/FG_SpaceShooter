#include "Game.h"

std::set<EnemyCollider*> EnemyColliders;
std::set<Collider*> Colliders;
using hlslpp::float2;
using hlslpp::float1;

EnemyCollider::EnemyCollider()
{
	EnemyColliders.insert(this);
	this->enemy = NULL;
}

Collider::Collider()
{
	Colliders.insert(this);
	radius = 1.0f;
	CollisionVector.x = 0.0f;
	CollisionVector.y = 0.0f;
	CollisionFlag = false;
	Skip = false;
}

EnemyCollider::~EnemyCollider()
{
	EnemyColliders.erase(this);
}

Collider::~Collider()
{
	Colliders.erase(this);
}

void Collider::Set(float2 pos, float1 radius)
{
	position = pos;
	this->radius = radius;
	velocity = float2(0.0f, 0.0f);
}

void Collider::Set(float2 pos, float2 vel)
{
	position = pos;
	velocity = vel;
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

void VelocityMovement::StopMovement()
{
	velocity = float2(0.0f, 0.0f);
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

float2 VelocityMovement::Update(float2 pos, float deltaTime, Collider* collider)
{
	if (collider->CollisionFlag)
	{
		collider->CollisionFlag = false;
		velocity = -velocity;
		velocity = velocity + collider->CollisionVector;
	}

	return this->Update(pos, deltaTime);
}

Enemy* PhysicsQueryEnemy(hlslpp::float2 pos, hlslpp::float2 dir)
{
	Enemy* e = NULL;
	float bestDot = 0.997f;
	for (EnemyCollider* c : EnemyColliders)
	{
		if (c->Skip) continue;
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

void Collider::Update()
{

	if (CollisionFlag) return; //skip all if vm has not handled collision;

	for (Collider* c : Colliders)
	{
		if (c == this) continue;
		if (c->Skip) continue;
		float1 distP = hlslpp::distance(c->position, position);
		float1 distC = c->radius + radius;

		display1 = distP.x;

		if (distC > distP)
		{
			CollisionVector = c->velocity;
			CollisionFlag = true;
			break;
		}
	}

}