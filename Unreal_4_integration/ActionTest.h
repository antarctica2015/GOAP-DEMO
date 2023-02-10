
//------------------------------------------------------------------------
//
//  Name:   ActionTest.h
//
//  Desc:   Test action class implementation
//
//  Author: Brett Keir Jones 
//
//------------------------------------------------------------------------

#include "ActionAbstract.h"

class AGOAPController;

class ActionTest: public ActionAbstract
{
	typedef ActionAbstract super;

public:

	//my version:
	virtual ENUM_ACTION_TYPE GetActionClassType() const { return kAct_Test; }

	ActionTest();

	//ActionAbstract members.
	virtual void	InitAction();
	virtual void	ActivateAction(AGOAPController* pAgent, WorldState& wsGoalState);

	//
private:


};