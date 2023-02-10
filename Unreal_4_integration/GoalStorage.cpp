
#include "GoapTest.h"
#include "GoalStorage.h"
#include "GOAPController.h"
#include "BlackBoard.h"
#include "Utils.h"

GoalStorage::GoalStorage(AGOAPController* pAgent)
{
	m_pAgent = pAgent;
	Init();
}

GoalStorage::~GoalStorage()
{
	Terminate(true);
}

//initialise the goal manager
void GoalStorage::Init()
{
	m_pCurrentGoal = NULL;

	m_eGoalSet = kGSetID_Invalid;
	m_fGoalSetTime = 0.0f;

	//should be adding goals?
	AddGoal(kGoal_Test);

}

//terminate/shutdown goal manager
void GoalStorage::Terminate(bool bDestroyAll)
{
	// Delete all goals.
	// Use RemoveGoal so that all goal removals follow the same code path.
	// When SetGoalSet is called, it may call Terminate()to clear the goals. 
	// Calling RemoveGoal ensures that there are no dangling pointers.

	GoalAbstract* pGoal;
	GOAL_LIST::TIterator itGoal = m_plistGoals.CreateIterator() ;
	//BKJ need to fix
	while (itGoal) {
		pGoal = (*itGoal);

		if (!bDestroyAll && pGoal->IsPermanentGoal()) {
			++itGoal;
		} else {
			//AITRACE( AIShowGoals, ( m_pAI->m_hObject, "Removing Goal %s.", s_aszGoalTypes[pGoal->GetGoalType()] ) );
			RemoveGoal(pGoal->GetGoalType());

			itGoal.Reset();

		}
	}

	for (auto itGoal = m_plistGoals.CreateIterator(); itGoal; ++itGoal) {
		pGoal = (*itGoal);
		//AITRACE( AIShowGoals, ( m_pAI->m_hObject, "Goal %s is Permanent. NOT removing.", s_aszGoalTypes[pGoal->GetGoalType()] ) );
	}
}

//create a new goal of the given type
GoalAbstract* GoalStorage::CreateNewGoal(ENUM_GOAL_TYPE eGoalType)
{
	switch (eGoalType)
	{
	case kGoal_Test: { return new GoalTest(); break; }

	default: break;   //AIASSERT( 0, m_pAI->m_hObject, "CAIGoalMgr::CreateFactoryGoal: Unrecognized goal type.");
	}

	return NULL;
}

//add a new goal to the goal list
GoalAbstract* GoalStorage::AddGoal(ENUM_GOAL_TYPE eGoalType)
{
	GoalAbstract* pGoal = NULL;

	if (eGoalType != kGoal_InvalidType && eGoalType < kGoal_Count) {
		// Check if AI already has this goal.
		GOAL_LIST::TIterator itGoal  = m_plistGoals.CreateIterator();

		for (; itGoal; ++itGoal) {

			pGoal = *itGoal;

			// If AI has the goal, return it.
			if (pGoal->GetGoalType() == eGoalType) {
				break;
			}
		}

		if (pGoal == m_plistGoals.Top()) {
			if (eGoalType != kGoal_InvalidType) {
				// Add goal to list of goals.
				pGoal = NULL;

				pGoal = CreateNewGoal(eGoalType);

				if (pGoal == NULL) {
					//AIASSERT( 0, m_pAI->m_hObject, "CAIGoalMgr::AddGoal: Failed to create goal.");
					return NULL;
				}

				// Initialize goal.
				pGoal->InitGoal(m_pAgent, eGoalType);

				//add to TArray
				m_plistGoals.Emplace(pGoal);
			} else {
				//AIASSERT1( 0, m_pAI->m_hObject, "CAIGoalMgr::AddGoal: Unable to create goal '%s'", s_aszGoalTypes[eGoalType] );
			}
		}
	}

	return pGoal;
}

//removes a goal from goal list
void GoalStorage::RemoveGoal(ENUM_GOAL_TYPE eGoalType)
{
	if (eGoalType == kGoal_InvalidType) {
		//error message here!
	}

	// Find goal of specified type.
	GoalAbstract* pGoal;

	GOAL_LIST::TIterator itGoal = m_plistGoals.CreateIterator();

	for (; itGoal; ++itGoal) {
		pGoal = *itGoal;

		if (pGoal->GetGoalType() == eGoalType) {
			if (m_pCurrentGoal == pGoal) {
				m_pCurrentGoal = NULL;
			}

			// Delete the goal.
			pGoal->TerminateGoal();

			if (pGoal) {
				delete pGoal;
				pGoal = NULL;
			}

			m_plistGoals.Remove(*itGoal);

			return;
		}
	}
}

//find a goal of given type
GoalAbstract* GoalStorage::FindGoalByType(ENUM_GOAL_TYPE eGoalType)
{
	GOAL_LIST::TIterator itGoal = m_plistGoals.CreateIterator();

	GoalAbstract* pGoal;

	for (; itGoal; ++itGoal) {
		pGoal = (*itGoal);

		if (pGoal->GetGoalType() == eGoalType) {
			return pGoal;
		}
	}

	return NULL;
}

//selects most relevant goal
void GoalStorage::UpdateGoalRelevances(bool bReplan)
{
	bool bRequiresHigherInterruptPriority = false;
	bool bGoalChanged = false;

	GoalAbstract* pGoalMax = FindMostRelevantGoal(true);

	// Set the new current goal, which is the goal with the highest relevance that can formulate a valid plan of actions.
	while (pGoalMax) {
		// If the pGoalMax has the same relevance as the current goal, prefer the current to avoid abandoning a goal prematurely when there isn't anything better to do.
		if ((m_pCurrentGoal && pGoalMax) && (m_pCurrentGoal != pGoalMax) && (m_pCurrentGoal->GetGoalRelevance() == pGoalMax->GetGoalRelevance())) {
			pGoalMax = m_pCurrentGoal;
		}

		// Goal has not changed. No planning necessary.
		if ((pGoalMax == m_pCurrentGoal) && (!bReplan) && (!m_pCurrentGoal->ReplanRequired())) {
			break;
		}

		// Goal must interrupt to be valid (ie damage response which must be played).
		if (((bRequiresHigherInterruptPriority && pGoalMax && m_pCurrentGoal) && (pGoalMax != m_pCurrentGoal)) && (pGoalMax->GetInterruptPriority() <= m_pCurrentGoal->GetInterruptPriority())) {
			pGoalMax->ClearGoalRelevance();
		} else if ((pGoalMax->GetActivationChance() < 1.0f) && (pGoalMax->GetActivationChance() < GetRandom(0.0f, 1.0f))) {
			// Failed to meet random probability of activation.
			pGoalMax->ClearGoalRelevance();

		} else if (pGoalMax->BuildPlan()) {
			// Activate the goal if we formulated a valid plan.
			if (m_pCurrentGoal) {
				m_pCurrentGoal->DeactivateGoal();
				//AITRACE( AIShowGoals, ( m_pAI->m_hObject, "Deactivating Goal: %s", s_aszGoalTypes[m_pCurGoal->GetGoalType()] ) );
			}

			m_pCurrentGoal = pGoalMax;
			//AITRACE( AIShowGoals, ( m_pAI->m_hObject, "Activating Goal: %s (%.2f)", s_aszGoalTypes[m_pCurGoal->GetGoalType()], m_pCurGoal->GetGoalRelevance() ) );
			m_pCurrentGoal->ActivateGoal();
			m_pCurrentGoal->ActivatePlan();
			bGoalChanged = true;
			break;

		} else {	
			// Could not build a valid plan.
			pGoalMax->HandleBuildPlanFailure();
		}

		pGoalMax = FindMostRelevantGoal(false);
	}

	// No max goal was found, so the AI has no current goal or state.
	if (bReplan && !pGoalMax && !bRequiresHigherInterruptPriority) {
		m_pCurrentGoal = NULL;
	}

	// Clear the SelectAction flag from the blackboard.
	// If the Goal required a interrupt, and the Goal did not change,
	// then do not clear the SelectAction flag.
	if (bRequiresHigherInterruptPriority && !bGoalChanged) {
		return;
	} else {
		m_pAgent->GetBlackBoard()->SetBBSelectAction(false);
	}
}

//returns the most relevent goal
GoalAbstract* GoalStorage::FindMostRelevantGoal(bool bRecalculate)
{
	float fRelevance;
	float fMaxRelevance = 0.0f;

	double fNextRecalcTime;

	GoalAbstract* pGoal;
	GoalAbstract* pGoalMax = NULL;

	GOAL_LIST::TIterator itGoal = m_plistGoals.CreateIterator();

	for (; itGoal; ++itGoal) {
		pGoal = *itGoal;

		// Recalculate the current relevance.
		if (bRecalculate) {
			// Do not re-evaluate this goal if it is not time yet.
			fNextRecalcTime = pGoal->GetNextRecalcTime();

			if ((pGoal != m_pCurrentGoal) && (fNextRecalcTime > 0.0f) && (fNextRecalcTime > GetCurrentTime() )) {
				pGoal->ClearGoalRelevance();
			} else if ((!pGoal->GetReEvalOnSatisfied()) && pGoal->IsWSSatisfied(m_pAgent->GetWorldState())) {
				// Do not re-evaluate this goal if it is currently satisfied.
				pGoal->ClearGoalRelevance();
			} else {
				// Goal is unsatisfied, or should always be re-evaluated.
				pGoal->CalculateGoalRelevance();

				if (fNextRecalcTime > 0.0f) {
					pGoal->SetNextRecalcTime();
				}
			}
		}

		fRelevance = pGoal->GetGoalRelevance();

		// Select a new max.
		if (fRelevance > fMaxRelevance) {
			fMaxRelevance = fRelevance;
			pGoalMax = pGoal;
		}
	}

	return pGoalMax;
}

//updates the goals
void GoalStorage::SelectRelevantGoal()
{
	// need to determine if the goals should be updated, and if so, if replanning is required.
	bool bUpdateGoals = false;
	bool bForceReplanning = false;

	if (m_pCurrentGoal) {
		// Test for an invalid plan.
		if (!m_pCurrentGoal->IsPlanValid()) {
			//AITRACE( AIShowGoals, ( m_pAI->m_hObject, "Plan is invalid for Goal: %s.", s_aszGoalTypes[m_pCurGoal->GetGoalType()] ) );
			bUpdateGoals = true;
			bForceReplanning = true;
		} else if (m_pCurrentGoal->IsWSSatisfied(m_pAgent->GetWorldState())) {
			// Test for a satisfied plan.
			//AITRACE( AIShowGoals, ( m_pAI->m_hObject, "Goal is satisfied: %s.", s_aszGoalTypes[m_pCurGoal->GetGoalType()] ) );
			bUpdateGoals = true;
			bForceReplanning = true;
		}
	}

	// Test for a request to reselect actions.  
	if (m_pAgent->GetBlackBoard()->GetBBSelectAction()) {
		bUpdateGoals = true;
	}

	// Plan has been invalidated, so immediately force a replan.
	if (m_pAgent->GetBlackBoard()->GetBBInvalidatePlan()) {
		//AITRACE( AIShowGoals, ( m_pAI->m_hObject, "Honoring forced replanning request." ) );
		bUpdateGoals = true;
		bForceReplanning = true;
	}

	// Perform the actual goal updating/replanning if requested
	if (bUpdateGoals) {
		UpdateGoalRelevances(bForceReplanning);
	}

	// Clear the forced replanning flag. Clear this AFTER updating goal relevances.
	m_pAgent->GetBlackBoard()->SetBBInvalidatePlan(false);
}

//update the currentgoal
void GoalStorage::UpdateGoal()
{
	if (m_pCurrentGoal) {
		if (!m_pCurrentGoal->UpdateGoal()) {
			m_pAgent->GetBlackBoard()->SetBBSelectAction(true);
			m_pCurrentGoal->ClearPlan();
		}

		if (m_pAgent->GetBlackBoard()->GetBBTaskStatus() == kTaskStatus_Set) {
			m_pCurrentGoal->UpdateTaskStatus();
		}
	}
}
