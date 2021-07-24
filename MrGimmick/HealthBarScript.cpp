#include "HealthBarScript.h"
#include "LoadingJson.h"
#include "Canvas.h"

REGISTER_FINISH(HealthBarScript, ScriptBase) {}

void HealthBarScript::OnStart()
{
	auto canvas = GET_FIRST_SCRIPT_OF_TYPE(Canvas);
	_healthBar = dynamic_cast<HealthUIComponent*>(canvas->GetComponent("HealthBar"));
}

void HealthBarScript::OnLateUpdate()
{
	_healthBar->SetMaxHealth(_healthScript->GetMaxHealth());
	_healthBar->SetHealth(_healthScript->GetHealth());
}

void HealthBarScript::Load(nlohmann::json& input)
{
	_healthScript = dynamic_cast<HealthScript*>(
		GetOwner()->GetComponent(input[LoadingJson::Field::scriptsField][0][LoadingJson::Field::idField].get<CULL>())
	);
}
