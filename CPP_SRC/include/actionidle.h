#ifndef ACTIONIDLE_H
#define ACTIONIDLE_H

//------------------------------------------------------------------------
//
//  Name:   actionidle.h
//
//  Desc:   Idle action class implementation
//
//  Author: Brett Keir Jones 2008
//
//------------------------------------------------------------------------

#include "actionabstract.h"

class Agent;

class ActionIdle : public ActionAbstract
{
	typedef ActionAbstract super;

	public:

		//my version:
		virtual ENUM_ACTION_TYPE GetActionClassType() const { return kAct_Idle; }

		ActionIdle();

		//ActionAbstract members.

		virtual void	InitAction();
		virtual void	ActivateAction(Agent* pAgent, WorldState& wsGoalState);

		//
	private:


};

#endif