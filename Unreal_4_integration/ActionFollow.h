
//------------------------------------------------------------------------
//
//  Name:   ActionFollow.h
//
//  Desc:   Move to position action class implementation
//
//  Author: Brett Keir Jones 
//
//------------------------------------------------------------------------

#include "ActionAbstract.h"

class AGOAPController;

class ActionFollow: public ActionAbstract
{
	typedef ActionAbstract super;

public:

	//my version:
	virtual ENUM_ACTION_TYPE GetActionClassType() const { return kAct_Follow; }

	ActionFollow();

	//ActionAbstract members.
	virtual void	InitAction();
	virtual void	ActivateAction(AGOAPController* pAgent, WorldState& wsGoalState);

	//
private:


};