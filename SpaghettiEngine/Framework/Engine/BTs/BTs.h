#pragma once
#include "Node.h"
#include "BlackBoard.h"

class Root : public DecoratorNode
{
public:
	Node::State Tick() override;
private:
	NODE_REGISTER(Root);
};

class BTs;
typedef std::shared_ptr<BTs> SBTs;
typedef std::weak_ptr<BTs> WBTs;

class BTs
{
public:
	[[nodiscard]] static SBTs CreateBehaviorTree(); // Use this to create behavior tree

	void Load(const std::string& path);
	void AssignPtr(WBTs ptr);
	BTs();
	Node::State Tick();
	WBlackBoard GetBlackBoard();
	virtual SBTs Clone();
protected:
	SBlackBoard _bb;
	WBTs _this;
	SNode _root;
};


