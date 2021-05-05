#include "Rigidbody2D.h"
#include "GameTimer.h"

REGISTER_FINISH(RigidBody2D);

RigidBody2D::RigidBody2D(PScene owner) : ScriptBase(owner)
{
	_name = TYPE_NAME(RigidBody2D);
}

void RigidBody2D::OnStart()
{
	velocity = constVelocity;
}

void RigidBody2D::SetConstVelocity(Vector3 v)
{
	constVelocity = v;
}

void RigidBody2D::SetVelocity(Vector3 v)
{
	velocity = v;
}

void RigidBody2D::AddForce(Vector3 delta)
{
	velocity += delta;
}

void RigidBody2D::SetMass(float m)
{
	mass = m;
}

Vector3 RigidBody2D::GetVelocity()
{
	return velocity;
}

Vector3 RigidBody2D::GetConstVelocity()
{
	return constVelocity;
}

float RigidBody2D::GetMass()
{
	return mass;
}

void RigidBody2D::Load(json& inputObject)
{
	try
	{
		//Physic::GetInstance()->AddRigidBody(this);
	}
	catch (CornException& e)
	{
		throw e;
	}
}
