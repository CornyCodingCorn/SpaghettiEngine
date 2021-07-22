#include "PlayerScript.h"
#include "SegmentScript.h"
#include "SpawnPoint.h"
#include "SceneManager.h"
#include "GameTimer.h"
#include "FieldNames.h"
#include "Polygon2DCollider.h"

REGISTER_FINISH(PlayerScript, ScriptBase) {
};

void PlayerScript::OnStart()
{
    auto animator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
    _healthScript = GET_FIRST_SCRIPT_OF_TYPE(HealthScript);
    _rb = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);
    _moveScript = GET_FIRST_SCRIPT_OF_TYPE(MoveScript);

    _healthScript->AddToEvent([&](const int& health, const int& delta) {
        this->TookDamage(health, delta);
    });
    _control = GET_FIRST_SCRIPT_OF_TYPE(PlayerControl);
    _isHurted = animator->GetField<bool>(Fields::Player::_isHurt);
}

void PlayerScript::OnEnabled()
{
    __currentInstance = std::dynamic_pointer_cast<PlayerScript>(GetSharedPtr());
}

void PlayerScript::OnDisabled()
{
    __currentInstance.reset();
}

void PlayerScript::OnUpdate()
{
    if (_respawnCounter > 0)
    {
        _respawnCounter -= GameTimer::GetDeltaTime();
        if (_respawnCounter <= 0)
            Respawn();
        return;
    }

    if (_hurtCounter > 0)
    {
        _hurtCounter -= GameTimer::GetDeltaTime();
        if (_hurtCounter <= 0)
        {
            _control->Enable();
            _isHurted.lock()->SetValue(false);
        }
    }
}

ScriptBase* PlayerScript::Clone() const
{
    auto clone = dynamic_cast<PlayerScript*>(ScriptBase::Clone());

    clone->_hurtTime = _hurtTime;
    clone->_hurtVel = _hurtVel;
    clone->_respawnDelay = _respawnDelay;

    return clone;
}

void PlayerScript::TookDamage(const int& health, const int& delta)
{
    if (health > 0)
    {
        if (delta < 0)
        {
            _isHurted.lock()->SetValue(true);
            _hurtCounter = _hurtTime;
            _control->Disable();

            auto vel = _rb->GetVelocity();
            vel += _hurtVel * (_moveScript->IsFlipped() ? -1 : 1);
            _rb->SetVelocity(vel);
        }
        else if (delta > 0)
        {
            // TO DO
        }
    }
    else
    {
        _respawnCounter = _respawnDelay;
        DisableColliders();
        DisableRigidBody();
    }
}

void PlayerScript::Respawn()
{
    SegmentScript::DisableAllSegment();
    SegmentScript::SpawnAll();
    GetGameObject()->GetTransform().SetWorldTransform(SpawnPoint::GetSpawnPointScript()->GetSpawnPosition());
    auto segment = SpawnPoint::GetSpawnPointScript()->GetSegment();
    segment->Enable();

    EnableColliders();
    EnableRigidBody();

    _healthScript->SetHealth(_healthScript->GetMaxHealth());
    _live--;
    if (_live == 0)
    {
        _score = 0;
        SceneManager::CallReloadScene();
    }
}

void PlayerScript::DisableColliders()
{
    GetGameObject()->GetScriptContainer().IteratingWithLamda(
        [&](PScriptBase script) {
            if (script->GetType() == TYPE_NAME(Polygon2DCollider))
            {
                script->Disable();
            }
        }
    );
}

void PlayerScript::EnableColliders()
{
    GetGameObject()->GetScriptContainer().IteratingWithLamda(
        [&](PScriptBase script) {
            if (script->GetType() == TYPE_NAME(Polygon2DCollider))
            {
                script->Enable();
            }
        }
    );
}

void PlayerScript::DisableRigidBody()
{
    auto script = GetGameObject()->GetPhysicComponent().GetRigidBody2DScript();
    if (script)
        script->Disable();
}

void PlayerScript::EnableRigidBody()
{
    GetGameObject()->GetScriptContainer().IteratingWithLamda(
        [&](PScriptBase script) {
            if (script->GetType() == TYPE_NAME(RigidBody2D))
            {
                script->Enable();
            }
        }
    );
}



void PlayerScript::Load(nlohmann::json& input)
{
    _hurtTime = input[Fields::PlayerScript::_hurtTime].get<float>();
    _hurtVel = input[Fields::PlayerScript::_hurtVel];
    _respawnDelay = input[Fields::PlayerScript::_respawnDelay].get<float>();
}

GameObj* PlayerScript::GetCurrentPlayer()
{
    return __currentInstance.expired() ? nullptr : __currentInstance.lock()->GetGameObject();
}
