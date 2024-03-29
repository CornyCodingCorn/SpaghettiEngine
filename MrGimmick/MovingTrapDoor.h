#pragma once
#include "ScriptBase.h"
#include "CRectangle.h"
#include "Animator.h"
#include "MovableScript.h"
#include "Polygon2DCollider.h"
#include "TrapDoorSound.h"

class MovingTrapDoor : public ScriptBase
{
public:
	void OnFixedUpdate() override;
	void OnStart() override;
	void Load(nlohmann::json& input) override;

	ScriptBase* Clone() const override;
	void Arrived();
private:
	CRectangle _triggerZone;
	BoolField _dispatchedField;

	unsigned _dispatched = 1;
	float _delay = 0;
	float _delayCD = 0;

	float _tick = 0;
	bool _isStartTick = false;

	MovableScript* _movableScript = nullptr;
	Polygon2DCollider* _polygonCollider = nullptr;
	TrapDoorSound* _sound = nullptr;

	REGISTER_START(MovingTrapDoor);
};

