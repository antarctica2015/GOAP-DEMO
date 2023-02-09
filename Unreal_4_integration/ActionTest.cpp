
#include "GoapTest.h"
#include "ActionTest.h"
#include "GOAPController.h"

//constructor
ActionTest::ActionTest()
{

}

//handle initialising the action, setting the action record to use as well as any preconditions and effects
void ActionTest::InitAction()
{
	super::InitAction();

	m_eActionType = kAct_Test;
	m_fActionCost = 2.0f;
	m_fActionPrecedence = 1.0f;
	m_bActionIsInterruptible = true;
	m_fActionProbability = 1.0f;

	m_wsWorldStatePreconditions.SetWSProp(KWSKey_DummyWS, kWSType_bool, true);

	//set effects
	m_wsWorldStateEffects.SetWSProp(KWSKey_DummyWS, kWSType_bool, true);
}

void ActionTest::ActivateAction(AGOAPController* pAgent, WorldState& wsGoalState)
{
	super::ActivateAction(pAgent, wsGoalState);

}

