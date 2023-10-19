#include "Graphics.h"
#include "Physics.h"
#include "Game.h"
#include <stdio.h>
#include <queue>

using hlslpp::float2;

std::queue<Line> FrameLines;

PlayerShip::PlayerShip()
{
	position = float2(1.0f, 1.0f);
	angle = -0.25;
	sprite.Set(float2(1.0f, 1.0f), 0.0, 0);
	shooting = false;
}

void PlayerShip::Fire()
{
	if (!shooting)
	{
		timer = 25.0f;
		shooting = true;
		FrameLines.emplace();
		Line* f = &FrameLines.back();

		float ang = angle + ((rand() % 50) - 25);

		float2 fireVec = float2(cos(angle * 6.28), sin(angle * 6.28));
		fireVec = hlslpp::normalize(fireVec);

		Enemy* e = PhysicsQueryEnemy(position, fireVec);

		if (e != NULL)
		{
			e->Disable();
		}


		fireVec = fireVec * 3.0f;
		fireVec = position + fireVec;
		f->Set(position, fireVec);
	}
}

void PlayerShip::Thruster()
{
	float2 fireVec = float2(cos(angle * 6.28), sin(angle * 6.28));

	fireVec = hlslpp::normalize(fireVec);

	fireVec /= 1000;

	fireVec *= (0.005f * deltaTime);

	vm.AddVector(fireVec);
}

void PlayerShip::Update(float deltaTime)
{
	if (clearTimer < 0.0f)
	{
		if (!FrameLines.empty())
		{
			FrameLines.pop();
		}
		clearTimer = 50.0f;
	}
	else if (clearTimer >= 0.0f)
	{
		clearTimer = clearTimer - deltaTime;
	}

	if (timer < 0.0f && shooting)
	{
		if (FrameLines.size() < 3) shooting = false;
	}
	else if (timer >= 0.0f && shooting)
	{
		timer = timer - deltaTime;
	}

	sprite.Set(position, angle);

	position = vm.Update(position, deltaTime);
}