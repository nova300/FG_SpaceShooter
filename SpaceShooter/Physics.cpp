#include "Game.h"

//std::set<EnemyCollider*> EnemyColliders;
//std::set<Collider*> Colliders;
std::vector<Collider> Colliders;
using namespace hlslpp;

Collider::Collider()
{
	Reset();
}

void Collider::Reset()
{
	float1 zero = float1(0.0f);
	colliderType = COL_UNDEFINED;
	//Colliders.insert(this);
	radius = 1.0f;
	position.xy = zero.xx;
	CollisionVector.xy = zero.xx;
	velocity.xy = zero.xx;
	CollisionFlag = false;
	Skip = false;
	enemy = NULL;
	Free = false;
}

int GetNewCollider()	//add new collider to vector and then return pointer
{
	for (int i = 0; i < Colliders.size(); i++)
	{
		if (Colliders[i].Free)
		{
			Colliders[i].Reset();
			return i;
		}
	}
	Colliders.emplace_back();
	return Colliders.size() - 1;
}

Collider* GetCollider(int index)
{
	if (index > Colliders.size()) return NULL;
	return &Colliders[index];
}

Collider::~Collider()
{
	//Colliders.erase(this);
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

void Collider::Set(Enemy* e)
{
	this->enemy = e;
	this->colliderType = COL_ENEMY;
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
	const float speedlimit = 0.001;
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
		velocity = collider->CollisionVector;
		velocity = velocity - (velocity / 10);
		collider->CollisionFlag = false;
	}

	return this->Update(pos, deltaTime);
}

Enemy* PhysicsQueryEnemy(hlslpp::float2 pos, hlslpp::float2 dir)
{
	Enemy* e = NULL;
	float bestDot = 0.996f;
	for (Collider c : Colliders)
	{
		if (c.Skip) continue;
		if (c.colliderType != COL_ENEMY) continue;
		float2 diff = c.position - pos;
		diff = hlslpp::normalize(diff);
		float dot = hlslpp::dot(dir, diff);
		//display1 = dot;
		if (dot > bestDot)
		{
			bestDot = dot;
			e = c.enemy;
		}
	}
	//display1 = bestDot;
	return e;
}

void Collider::Update()	//pre-optimization update
{
	/*
	if (CollisionFlag) return; //skip all if vm has not handled collision;

	for (Collider* c : Colliders)
	{
		if (c == this) continue;
		if (c->Skip) continue;
		float1 distP = hlslpp::distance(c->position, position);
		float1 distC = c->radius + radius;

		//display1 = distP.x;

		if (distC > distP)
		{
			float1 disO = distC - distP;
			disO = disO / 100;
			float2 dir = c->position - position;
			dir = normalize(dir);
			dir = dir * disO;
			float2 vel = -velocity;
			vel = vel / 2;
			vel = vel + (c->velocity / 2);
			CollisionVector = vel - dir;
			CollisionFlag = true;
			CollisionType = c->colliderType;
			break;
		}
	}
	*/
}

void Collider::UpdateColliders() //optimized update
{
	for (int i = 0; i < Colliders.size(); i++)
	{

		Collider* near = &Colliders[i];

		if (near->Skip) continue;
		if (near->CollisionFlag) continue;
		for (int j = 0; j < Colliders.size(); j++)
		{
			Collider* far = &Colliders[j];
			if (far == near) continue;
			if (far->Skip) continue;
			if (far->CollisionFlag) continue;
			float2 dist = far->position - near->position;
			float1 distP = (dist.x * dist.x) + (dist.y * dist.y);
			float1 distC = (far->radius + near->radius) * (far->radius + near->radius);

			//display1 = distP.x;

			if (distC > distP)
			{
				float1 disO = sqrt(distC) - sqrt(distP);
				disO = disO / 100;
				float2 dir = far->position - near->position;
				dir = normalize(dir);
				dir = dir * disO;
				float2 vel = -near->velocity;
				vel = vel / 2;
				vel = vel + (far->velocity / 2);
				near->CollisionVector = vel - dir;
				near->CollisionFlag = true;
				near->CollisionType = far->colliderType;

				//disO = distP - distC;
				//disO = disO / 100;
				dir = near->position - far->position;
				dir = normalize(dir);
				dir = dir * disO;
				vel = -far->velocity;
				vel = vel / 2;
				vel = vel + (near->velocity / 2);
				far->CollisionVector = vel - dir;
				far->CollisionFlag = true;
				far->CollisionType = near->colliderType;
				break;
			}
		}
	}
}