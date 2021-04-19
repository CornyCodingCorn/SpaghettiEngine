#include "PhysicMath.h"

PhysicMath::Rect PhysicMath::getSweptBroadphaseRect(const Rect& object)
{
	float x = object.vx > 0 ? object.x : object.x + object.vx;
	float y = object.vy > 0 ? object.y : object.y + object.vy;
	float w = object.vx > 0 ? object.vx + object.width : object.width - object.vx;
	float h = object.vy > 0 ? object.vy + object.height : object.height - object.vy;

	return Rect(x, y, w, h);
}

float PhysicMath::sweptAABB(const Rect& object, const Rect& block, Vector3*& result)
{
	float dxEntry, dxExit;
	float dyEntry, dyExit;

	if (object.vx > 0.0f)
	{
		dxEntry = block.x - (object.x + object.width);
		dxExit = (block.x + block.width) - object.x;
	}
	else
	{
		dxEntry = (block.x + block.width) - object.x;
		dxExit = block.x - (object.x + object.width);
	}

	if (object.vy > 0.0f)
	{
		dyEntry = block.y - (object.y + object.height);
		dyExit = (block.y + block.height) - object.y;
	}
	else
	{
		dyEntry = (block.y + block.height) - object.y;
		dyExit = block.y - (object.y + object.height);
	}

	float txEntry, txExit;
	float tyEntry, tyExit;

	if (object.vx == 0.0f)
	{
		txEntry = -std::numeric_limits<float>::infinity();
		txExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		txEntry = dxEntry / object.vx;
		txExit = dxExit / object.vx;
	}

	if (object.vy == 0.0f)
	{
		tyEntry = -std::numeric_limits<float>::infinity();
		tyExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		tyEntry = dyEntry / object.vy;
		tyExit = dyExit / object.vy;
	}

	float entryTime = max(txEntry, tyEntry);
	float exitTime = min(txExit, tyExit);

	if (entryTime > exitTime || (txEntry < 0.0f && tyEntry < 0.0f) || txEntry > 1.0f || tyEntry > 1.0f)
	{
		return 1.0f;
	}

	if (txEntry > tyEntry)
	{
		if (dxEntry > 0.0f)
		{
			result = new Vector3(1, 0, 0);
		}
		else
		{
			result = new Vector3(-1, 0, 0);
		}
	}
	else
	{
		if (dyEntry > 0.0f)
		{
			result = new Vector3(0, 1, 0);
		}
		else
		{
			result = new Vector3(0, -1, 0);
		}
	}

	return entryTime;
}

bool PhysicMath::AABBCheck(Rect object, Rect block)
{
	return !(object.x + object.width < block.x || object.x > block.x + block.width || object.y + object.height < block.y || object.y > block.y + block.height);
}
