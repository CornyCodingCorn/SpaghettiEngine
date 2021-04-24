#pragma once
#include "ScriptBase.h"
#include "CornException.h"
#include "Physic.h"

class Physic;

class RigidBody : public ScriptBase
{
	friend class Physic;
public:
	RigidBody();
	virtual void Start() override;
	virtual void Update() override;
	void SetVelocity(Vector3 v);
	void SetVelocityByDelta(Vector3 delta);
	Vector3 GetVelocity();
protected:
	virtual void Load(const std::string* inputArg) override;
protected:
	Vector3 fall;
	float vx = 0;
	float vy = 0;
	float gravity = 10;
private:
	REGISTER_START(RigidBody);
};