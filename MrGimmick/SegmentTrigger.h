#pragma once
#include "ScriptBase.h"

//
// +-------------------------+
// |						 |
// |						 |			   +----------------------+
// |						 |			   |					  |
// |		SegmentA		 |			   |					  |
// |						 |			   |		SegmentB	  |
// |						 +--+-+--------+					  |
// |PreviousTrigger				|T|					   NextTrigger|
// +-------------------------+--+-+--------+----------------------+
// Use to switch on and off between two segment to make sure that only the needed segment
// is getting update.
// Unused

class SegmentTrigger : public ScriptBase
{
public:
	enum class State
	{
		LoadingSegmentA,
		LoadingSegmentB,
	};
public:
	void OnFixedUpdate() override;

	void Load(nlohmann::json& input) override;
	[[nodiscard]] State GetState() const noexcept;
protected:
	void SetState() noexcept;
protected:
	// to calculate when the player enter;
	bool _playerInside = false;
	bool _shouldTrigger = true;

	State _state = State::LoadingSegmentA;

	GameObj* _segmentA = nullptr;
	GameObj* _segmentB = nullptr;

	Vector3 _center;
	float _width;
	float _height;
private:
	REGISTER_START(SegmentTrigger);
};

