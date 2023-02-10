
#include "actionidle.h"
#include "agent.h"
#include <iostream>


//constructor
ActionIdle::ActionIdle()
{

}

//handle initialising the action, setting the action record to use
//as well as any preconditions and effects
void ActionIdle::InitAction()
{
	std::cout << "Init Action: " << "kAct_Idle" << std::endl;

	super::InitAction();

	m_eActionType = kAct_Idle;
	m_fActionCost = 2.0f;
	m_fActionPrecedence = 1.0f;
	m_bActionIsInterruptible = true;
	m_fActionProbability = 1.0f;

	//dont want to idle if out weapon is not loaded (want it to reload then idle)
	//m_wsWorldStatePreconditions.SetWSProp(key_name, prop_key_type, key_value )
	m_wsWorldStatePreconditions.SetWSProp(kWSKey_WeaponLoaded, kWSType_bool, true);

	//set effects
	//Effect = ai agent is idling.
	m_wsWorldStateEffects.SetWSProp(kWSKey_Idling, kWSType_bool, true);
}

void ActionIdle::ActivateAction(Agent* pAgent, WorldState& wsGoalState)
{
	super::ActivateAction(pAgent, wsGoalState);

	//check swat4 actions for ideas!
	//we only use the idle aim around behavior if the officer isn't moving and clearing


}

