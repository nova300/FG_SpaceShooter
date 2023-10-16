#include "Physics.h"

std::set<Collider*> Colliders;
using hlslpp::float2;
using hlslpp::float1;

Collider::Collider(ColliderChannel ch)
{
	this->channel = ch;
	Colliders.insert(this);
}

Collider::~Collider()
{
	Colliders.erase(this);
}

VelocityMovement::VelocityMovement()
{
	velocity = float2(0.0f, 0.0f);
}

void VelocityMovement::AddVector(float2 v)
{
	velocity += v;
}

float2 VelocityMovement::Update(float2 pos, float deltaTime)
{
	const float speedlimit = 0.005;
	int drag = 70;
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
		drag = 10;
	}
	else if (pos.x > 2.0f)
	{
		velocity = float2(-velocity.x, velocity.y);
		pos.x = 2.0f;
		drag = 10;
	}
	else if (pos.y < 0)
	{
		velocity = float2(velocity.x, -velocity.y);
		pos.y = 0;
		drag = 10;
	}
	else if (pos.y > 2.0f)
	{
		velocity = float2(velocity.x, -velocity.y);
		pos.y = 2.0f;
		drag = 10;
	}

	velocity = velocity - (velocity / drag);

	float2 result = pos + (velocity * deltaTime);

	return pos + (velocity * deltaTime);
}