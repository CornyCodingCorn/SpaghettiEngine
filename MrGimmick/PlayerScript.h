#pragma once
#include "ScriptBase.h"
#include "Macros.h"
#include "Animator.h"
#include "PlayerControl.h"
#include "MoveScript.h"
#include "RigidBody2D.h"
#include "HealthScript.h"
#include "AttackMove.h"
#include "IFrameFlash.h"
#include "DeadAnimationScript.h"

CLASS_FORWARD_DECLARATION(PlayerScript);

class PlayerScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnEnabled() override;
	void OnDisabled() override;
	void OnUpdate() override;
	void OnCollide(CollideEvent& e);
	ScriptBase* Clone() const override;

	void TookDamage(const int& health, const int& delta);
	void Respawn();

	void DisableColliders();
	void EnableColliders();
	void DisableRigidBody();
	void EnableRigidBody();
 
	void Load(nlohmann::json& input);
	[[nodiscard]] static GameObj* GetCurrentPlayer();
private:
	BoolField _isHurted;

	float _respawnDelay = 0;
	float _respawnCounter = 0;

	float _hurtTime = 0;
	float _hurtCounter = 0;
	Vector3 _hurtVel;

	AttackMove* _attackMoveScript	= nullptr;
	PlayerControl* _control			= nullptr;
	Animator* _animator				= nullptr;
	MoveScript* _moveScript			= nullptr;
	RigidBody2D* _rb				= nullptr;
	HealthScript* _healthScript		= nullptr;
	DeadAnimationScript* _deadAnim	= nullptr;

	static inline unsigned _score = 0;
	static inline unsigned _live = 100;

	static inline WPlayerScript __currentInstance;
	REGISTER_START(PlayerScript);
};