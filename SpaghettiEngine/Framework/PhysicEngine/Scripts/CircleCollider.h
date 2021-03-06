#pragma once
#include "Collider2DBase.h"

class Circle;

class CircleCollider : public Collider2DBase
{
public:
	void OnFixedUpdate() override;
	void Load(nlohmann::json& input) override;
private:
	REGISTER_START(CircleCollider);
};