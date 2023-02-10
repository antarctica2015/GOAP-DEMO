
#include "GoapTest.h"
#include "GoalFollow.h"
#include "GOAPController.h"
#include "Utils.h"

//Constructor
GoalFollow::GoalFollow()
{

}

void GoalFollow::InitGoal(AGOAPController* pAgent, ENUM_GOAL_TYPE eGoalType)
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
void GoalFollow::CalculateGoalRelevance()
{
	// The follow target goal has a constant, 
	// very low relevance. It should always be active if nothing else is.
	m_fGoalRelevance = m_fGoalRelevance = 0.1f;
}

//set the worldstate satisfaction preconditions
void GoalFollow::SetWSSatisfaction(WorldState& WorldState)
{
	WorldState.SetWSProp(KWSKey_DummyWS, kWSType_bool, true);
}

//returns true if the worldstate satsifies the goal
bool GoalFollow::IsWSSatisfied(WorldState* pwsWorldState)
{
	// The test goal is never satisfied in real-time, but the planner should always consider it satisfied.
	return false;
}