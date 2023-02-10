
#include "GoapTest.h"
#include "GoalTest.h"
#include "GOAPController.h"
#include "Utils.h"

//Constructor
GoalTest::GoalTest()
{

}

void GoalTest::InitGoal(AGOAPController* pAgent, ENUM_GOAL_TYPE eGoalType)
{
	m_pAgent = pAgent;
	m_eGoalType = eGoalType;
	m_eAIContext = kContext_Invalid;
	m_fRecalcRate = 0.0f;
	m_fActivateChance = 1.0f;
	m_fInterruptPriority = 0.0f;
	m_fFrequency = 0.0f;
	m_bCanReactivateDuringTransitions = false;
	m_bReEvalOnSatisfied = false;
}

//calcuate the current goals relevance
void GoalTest::CalculateGoalRelevance()
{
	// The idle goal has a constant, very low relevance. It should always be active if nothing else is.
	m_fGoalRelevance = m_fGoalRelevance = 0.1f;
}

//set the worldstate satisfaction preconditions
void GoalTest::SetWSSatisfaction(WorldState& WorldState)
{
	WorldState.SetWSProp(KWSKey_DummyWS, kWSType_bool, true);
}

//returns true if the worldstate satsifies the goal
bool GoalTest::IsWSSatisfied(WorldState* pwsWorldState)
{
	// The test goal is never satisfied in real-time, but the planner should always consider it satisfied.
	return false;
}