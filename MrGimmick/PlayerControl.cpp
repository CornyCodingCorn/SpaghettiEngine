#include "PlayerControl.h"
#include "InputSystem.h"
#include "InputAll.h"
#include "FieldNames.h"
#include "PlayerSound.h"

REGISTER_FINISH(PlayerControl, ScriptBase) {}

void PlayerControl::OnStart()
{
	_moveLeft = std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_moveLeft));
	_moveRight = std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_moveRight));
	_jump = std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_jump));

	_moveScript = GET_FIRST_SCRIPT_OF_TYPE(MoveScript);

	auto animator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
	_isInTube = animator->GetField<bool>(Fields::Player::_isInTube);
}

void PlayerControl::OnUpdate()
{
	if (_moveLeft->CheckKeyPress())
		_moveScript->StartMoveLeft();
	else if (_moveLeft->CheckKeyRelease())
		_moveScript->StopMoveLeft();

	if (_moveRight->CheckKeyPress())
		_moveScript->StartMoveRight();
	else if (_moveRight->CheckKeyRelease())
		_moveScript->StopMoveRight();

	if (_jump->CheckKeyPress())
	{
		_moveScript->StartJump();

		if (_moveScript->IsAllowJump() && _moveScript->GetGrounded())
		{
			PlayerSound::GetCurrentPlayerSound()->PlayJumpSound();
		}
	}
	else if (_jump->CheckKeyRelease())
		_moveScript->StopJump();
}

void PlayerControl::OnDisabled()
{
	_moveScript->StopMoveLeft();
	_moveScript->StopMoveRight();
	_moveScript->StopJump();
}

void PlayerControl::OnEnabled()
{
	if (_moveLeft->CheckKeyDown())
		_moveScript->StartMoveLeft();
	if (_moveRight->CheckKeyDown())
		_moveScript->StartMoveRight();
}

void PlayerControl::SetIsInTube(bool value)
{
	_isInTube.lock()->SetValue(value);
}

ScriptBase* PlayerControl::Clone() const
{
	auto clone = dynamic_cast<PlayerControl*>(ScriptBase::Clone());

	clone->_moveLeft = _moveLeft;
	clone->_moveRight = _moveRight;
	clone->_jump = _jump;

	return clone;
}
