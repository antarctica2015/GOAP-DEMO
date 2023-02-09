

#include "goalabstract.h"
#include "agent.h"
#include "actionstorage.h"
#include "aiplanner.h"
#include "goalstorage.h"
#include "utils.h"
#include "blackboard.h"


//static func that returns the name of a passed in goal
const char* const GoalAbstract::GetGoalTypeName(ENUM_GOAL_TYPE eGoalType)
{
	if (eGoalType < 0 || eGoalType >= kGoal_Count)
	{
		return "";
	}

	return s_aszGoalTypes[eGoalType];
}

//Convert the passed in name to a goal type
ENUM_GOAL_TYPE GoalAbstract::GetGoalType(const char* const pszGoalTypeName)
{
	if (NULL == pszGoalTypeName)
	{
		return kGoal_InvalidType;
	}

	for (int i = 0; i < kGoal_Count; ++i)
	{
		//need a string comparison func (ogre or <string> ?)e))
		if( strcmp(s_aszGoalTypes[i], pszGoalTypeName) )
		{
			return (ENUM_GOAL_TYPE)i;
		}
	}

	return kGoal_InvalidType;
}


GoalAbstract::GoalAbstract()
{
	m_pAgent = NULL;
	m_eGoalType = kGoal_InvalidType;

	m_fGoalRelevance = 0.0f;
	m_fNextRecalcTime = 0.0f;
	m_fActivationTime = 0.0f;

	m_eAIContext = kContext_Invalid;
	m_fRecalcRate = 0.0f;
	m_fActivateChance = 0.0f;
	m_fInterruptPriority = 0.0f;
	m_fFrequency = 0.0f;
	m_bCanReactivateDuringTransitions = false;
	m_bReEvalOnSatisfied = false;

	
}

GoalAbstract::~GoalAbstract()
{

}




//initialses the goal
void GoalAbstract::InitGoal(Agent* pAgent, ENUM_GOAL_TYPE eGoalType)
{
	m_pAgent  = pAgent;
	m_eGoalType = eGoalType;

	if( m_fRecalcRate > 0.0f )
	{
		SetNextRecalcTime();
	}

	//sensor set up goes here
}

//
void GoalAbstract::TerminateGoal()
{

}

//
void GoalAbstract::ActivateGoal()
{
	m_fActivationTime = timeGetTime()/1000; // get the time in seconds

	// Don't allow two goals to use the same path.  Always recalculate, even if the destination is the same.
	m_pAgent->GetBlackBoard()->SetBBInvalidatePath(true);
}

void GoalAbstract::DeactivateGoal()
{
	if( m_fFrequency > 0.0f )
	{
		m_fNextRecalcTime = timeGetTime()/1000;
	}
}

//
void GoalAbstract::SetNextRecalcTime()
{
	if( m_fRecalcRate > 0.0f )
	{
		m_fNextRecalcTime = (timeGetTime()/1000) + m_fRecalcRate;
	}
}


//tell the planner to make a plan
bool GoalAbstract::BuildPlan()
{
	return m_pAgent->GetGoalPlanner()->BuildPlan(this);
	//return m_pAgent->GetGoalPlanner()->BuildPlan(m_pAgent, this);
}

//set this goals plan to the passed in plan
void GoalAbstract::SetPlan( Plan* pPlan )
{
	if( !pPlan )
	{
		return;
	}

	m_pAgent->SetPlan( pPlan );
}


//retrieve and activate the goals plan
void GoalAbstract::ActivatePlan()
{
	Plan* pPlan = m_pAgent->GetPlan();

	if( pPlan )
	{
		pPlan->ActivatePlan( m_pAgent );
	}
}

//check if a plan step is completed- then move to next one
bool GoalAbstract::UpdateGoal()
{
	Plan* pPlan = m_pAgent->GetPlan();
	
	if( !pPlan )
	{
		return false;
	}

	if( pPlan->IsPlanStepComplete() )
	{
		return pPlan->AdvancePlan();
	}

	return true;
}


//validate plan
bool GoalAbstract::IsPlanValid()
{
	Plan* pPlan = m_pAgent->GetPlan();
	
	if( !pPlan )
	{
		return false;
	}

	return pPlan->IsPlanValid();
}


//clear the plan
void GoalAbstract::ClearPlan()
{
	m_pAgent->SetPlan( NULL );
}


//returns whether the plan can be interrupted or not
bool GoalAbstract::IsPlanInterruptible()
{
	Plan* pPlan = m_pAgent->GetPlan();
	
	if( pPlan )
	{
		return pPlan->IsPlanStepInterruptible();
	}

	return true;
}

//returns the current action
ActionAbstract* GoalAbstract::GetCurrentAction()
{
	Plan* pPlan = m_pAgent->GetPlan();
	
	if( pPlan )
	{
		return pPlan->GetCurrentPlanStepAction();
	}

	return NULL;
}

//FROM FEAR SDK DOCS:
//Returns the ENUM_AI_CONTEXT associated with this goal.
//This is guaranteed to be either valid eAIContext or kContext_Invalid.
//This function is only valid to call on Active goals.

ENUM_AI_CONTEXT GoalAbstract::GetContext() const
{
	//BKJ:TODO
	// Verify the goal is currently active.
	if( !m_pAgent->GetGoalStorage()->IsCurrentGoal( this ) )
	{
	//log error to file "GoalAbstract::GetContext: This function is only valid while a goal is active."
	std::cout <<  "GoalAbstract::GetContext: This function is only valid while a goal is active." << std::endl; 
	}

	ENUM_AI_CONTEXT eContext = OnGetContext();

	// Verify the context to be returned is valid.
	if ( eContext == kContext_Invalid )
	{
		//AIASSERT1( 0, NULL, "GoalAbstract::GetAIContext: goal %s returned an out of range AIContext ID.", GetGoalTypeName( GetGoalType() ) );
		std::cout<< "GoalAbstract::GetContext: out of range ID for goal: " << GetGoalTypeName( GetGoalType() ) << std::endl;
		return kContext_Invalid;
	}

	return eContext;
}
        
//This is a template method, overridable by derived classes,for returning the ENUM_AI_CONTEXT for this goal.
//This base returns the m_eAIContext specified in this goal.
ENUM_AI_CONTEXT GoalAbstract::OnGetContext() const
{
	return m_eAIContext;
}
