
#include "GoapTest.h"
#include "ActionFollow.h"
#include "GOAPController.h"

//constructor
ActionFollow::ActionFollow()
{

}

//handle initialising the action, setting the action record to use as well as any preconditions and effects
void ActionFollow::InitAction()
{
	super::InitAction();

	m_eActionType = kAct_Follow;
	m_fActionCost = 2.0f;
	m_fActionPrecedence = 1.0f;
	m_bActionIsInterruptible = true;
	m_fActionProbability = 1.0f;

	m_wsWorldStatePreconditions.SetWSProp(KWSKey_DummyWS, kWSType_bool, true);

	//set effects
	m_wsWorldStateEffects.SetWSProp(KWSKey_DummyWS, kWSType_bool, true);
}

void ActionFollow::ActivateAction(AGOAPController* pAgent, WorldState& wsGoalState)
{
	super::ActivateAction(pAgent, wsGoalState);

}

