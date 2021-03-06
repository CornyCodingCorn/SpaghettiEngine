#include "RidableScript.h"
#include "FieldNames.h"
#include "GameTimer.h"

REGISTER_FINISH(RidableScript, ScriptBase) {}

void RidableScript::OnEnabled()
{
	_oldPos = GetWorldTransform();
}

void RidableScript::OnFixedUpdate()
{
	_delta = GetWorldTransform() - _oldPos;
	for (auto& obj : _objects)
		obj->GetTransform().Translate(_delta);
	_oldPos = GetWorldTransform();
	_objects.clear();
}

void RidableScript::OnCollide(CollideEvent& e)
{
	if (e.GetEventOwner() != GetGameObject())
		return;

	if (e.GetGameObject()->GetPhysicComponent().GetRigidBody2DScript() != nullptr && e.GetNormal().y > 0.75f)
		_objects.insert(e.GetGameObject());
}