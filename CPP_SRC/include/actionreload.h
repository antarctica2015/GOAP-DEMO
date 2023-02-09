
#ifndef ACTIONRELOAD_H
#define ACTIONRELOAD_H

//------------------------------------------------------------------------
//
//  Name:   actionreload.h
//
//  Desc:  
//
//  Author: Brett Keir Jones 2008
//
//------------------------------------------------------------------------

#include "actionabstract.h"

//forward decs
class Agent;

class ActionReload : public ActionAbstract
{
	typedef ActionAbstract super;

	public:
		virtual ENUM_ACTION_TYPE GetActionClassType() const { return kAct_Reload; }

		ActionReload();

		//ActionAbstract members.

		virtual void	InitAction();
		virtual bool	ValidateContextPreconditions( Agent* pAgent, WorldState& wsWorldStateGoal, bool bIsPlanning );
		virtual void	ActivateAction( Agent* pAgent, WorldState& wsWorldStateGoal );
		virtual	bool	IsActionComplete( Agent* pAgent );

	protected:
		
			//

};


#endif
