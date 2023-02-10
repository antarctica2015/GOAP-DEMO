

#include "goalidle.h"
#include "agent.h"
#include "utils.h"

//Constructor
GoalIdle::GoalIdle()
{
	
}

void GoalIdle::InitGoal(Agent* pAgent, ENUM_GOAL_TYPE eGoalType)
{
	std::cout << "Init Goal : " << s_aszGoalTypes[eGoalType] << std::endl;

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
void GoalIdle::CalculateGoalRelevance()
{
	// The idle goal has a constant, very low relevance.
	// It should always be active if nothing else is.
	m_fGoalRelevance =  m_fGoalRelevance = 0.1f;
}


//set the worldstate satisfaction preconditions
void GoalIdle::SetWSSatisfaction( WorldState& WorldState )
{
	WorldState.SetWSProp( kWSKey_Idling, kWSType_bool, true );
}

//returns true if the worldstate satsifies the goal
bool GoalIdle::IsWSSatisfied( WorldState* pwsWorldState )
{
	// The idle goal is never satisfied in real-time,
	// but the planner should always consider it satisfied.

	return false;
}