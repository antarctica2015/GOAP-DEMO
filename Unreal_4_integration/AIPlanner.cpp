
#include "GoapTest.h"
#include "AIPlanner.h"
#include "GOAPController.h"
#include "GoalAbstract.h"
#include "ActionStorage.h"
#include "BlackBoard.h"
#include "Utils.h"

//Constructor & Deconstructor
Plan::Plan(AGOAPController* pAgent)
{
	m_pAgent = pAgent;
	m_iPlanStep = 0;
	m_fPlanActivationTime = 0.0f;

}

Plan::~Plan()
{
	DeactivatePlan();

	//delete the plan
	PlanStep* pPlanStep;

	for (auto itPlan = m_listPlanSteps.CreateIterator(); itPlan; ++itPlan) {
		pPlanStep = *itPlan;
		delete pPlanStep;
		pPlanStep = NULL;
	}

	m_listPlanSteps.Empty(); 
}

//activate a new plan
void Plan::ActivatePlan(AGOAPController* pAgent)
{
	if (!pAgent) {
		return;
	}

	m_pAgent = pAgent;

	m_fPlanActivationTime = GetCurrentTime();

	PlanStep*	pPlanStep = m_listPlanSteps[0];

	if (pPlanStep) {
		//
		ActionAbstract* pAction = m_pAgent->GetActionStorage()->GetAction(pPlanStep->eActionType);

		if (pAction) {

			//check if actions preconditions are met- bail out if not
			if (!pAction->ValidateContextPreconditions(m_pAgent, pPlanStep->wsWorldState, true)) {
				//log error to file
				//"Failed to activate plan due to first action's failed context preconditions" s_aszActionTypes[pAction->GetActionRecord()->eActionType], pActionRecord()->fActionCost

				m_pAgent->GetBlackBoard()->SetBBInvalidatePlan(true);
				return;
			}

			//log message: "Activating Action" s_aszActionTypes[pAction->GetActionRecord()->eActionType], pActionRecord()->fActionCost ;
			pAction->ActivateAction(m_pAgent, pPlanStep->wsWorldState);

			//if action was immediately complete then advance to another action
			if (pAction->IsActionComplete(m_pAgent)) {
				AdvancePlan();
			}
		}
	}
}


//deactivate a current plan
void Plan::DeactivatePlan()
{
	//deactivate the current action
	ActionAbstract* pAction = GetCurrentPlanStepAction();

	if (pAction) {
		pAction->DeactivateAction(m_pAgent);
	}
}

//returns true if current plan step is complete
bool Plan::IsPlanStepComplete()
{
	if (m_iPlanStep >= m_listPlanSteps.Num()) {
		return false;
	}

	PlanStep* pPlanStep = m_listPlanSteps[m_iPlanStep];

	if (pPlanStep) {
		ActionAbstract* pAction = m_pAgent->GetActionStorage()->GetAction(pPlanStep->eActionType);

		if (pAction) {
			return pAction->IsActionComplete(m_pAgent);
		}
	}

	return false;
}

//returns true if the current plan step is interruptible
bool Plan::IsPlanStepInterruptible()
{
	if (m_iPlanStep >= m_listPlanSteps.Num()) {
		return true;
	}

	PlanStep* pPlanStep = m_listPlanSteps[m_iPlanStep];

	if (pPlanStep) {
		ActionAbstract* pAction = m_pAgent->GetActionStorage()->GetAction(pPlanStep->eActionType);

		if (pAction) {
			return pAction->IsActionInterruptible(m_pAgent);
		}
	}

	return true;
}

//Advance plan to the next (plan) step and activate action
bool Plan::AdvancePlan()
{
	ActionAbstract* pAction;
	PlanStep* pPlanStep;

	// Continue advancing the plan until we find one that can activate and is not immediately complete.
	while (true) {
		// Apply Effects to context.
		// By default, Actions do not have any effect on the real world.
		// They only affect the planners representation of the world.
		// This gives more control of where WorldState variables are set or allows them to never be set.
		// (e.g. AtTargetPos is never really true in the real world, because targets more continuously.)

		pPlanStep = m_listPlanSteps[m_iPlanStep];

		if (pPlanStep) {
			pAction = m_pAgent->GetActionStorage()->GetAction(pPlanStep->eActionType);

			if (pAction) {
				pAction->ApplyContextEffect(m_pAgent, m_pAgent->GetWorldState(), &(pPlanStep->wsWorldState));
				pAction->DeactivateAction(m_pAgent);
			}
		}


		// Advance the step. Bail if no more steps.
		++m_iPlanStep;

		if (m_iPlanStep >= m_listPlanSteps.Num()) {
			return false;
		}

		pPlanStep = m_listPlanSteps[m_iPlanStep];

		if (pPlanStep) {

			pAction = m_pAgent->GetActionStorage()->GetAction(pPlanStep->eActionType);

			if (pAction) {
				// Action's preconditions are not met. Bail.
				if (!pAction->ValidateContextPreconditions(m_pAgent, pPlanStep->wsWorldState, false)) {
					return false;
				}

				// Bail if action is not immediately complete.

				//need a logging/screen print system
				//AITRACE( AIShowActions, ( m_pAI->m_hObject, "Activating Action: %s (%.2f)", s_aszActionTypes[pAction->GetActionRecord()->eActionType], pAction->GetActionRecord()->fActionCost ) );

				pAction->ActivateAction(m_pAgent, pPlanStep->wsWorldState);

				if (!pAction->IsActionComplete(m_pAgent)) {
					return true;
				}

				// Deactivate action if it was immediately complete.
				pAction->DeactivateAction(m_pAgent);
			}
		}
	}

	// Uh oh: Something is wrong.
	return false;
}

//return true if the plan is valid
bool Plan::IsPlanValid()
{
	if (m_iPlanStep >= m_listPlanSteps.Num()) {
		return false;
	}

	//
	PlanStep* pPlanStep = m_listPlanSteps[m_iPlanStep];

	if (pPlanStep) {
		ActionAbstract* pAction = m_pAgent->GetActionStorage()->GetAction(pPlanStep->eActionType);

		if (pAction) {
			//
			return pAction->ValidateAction(m_pAgent);
		}
	}

	return false;
}


//return the current plan steps action
ActionAbstract* Plan::GetCurrentPlanStepAction()
{
	if (m_iPlanStep >= m_listPlanSteps.Num()) {
		return NULL;
	}

	PlanStep* pPlanStep = m_listPlanSteps[m_iPlanStep];

	if (pPlanStep) {
		return m_pAgent->GetActionStorage()->GetAction(pPlanStep->eActionType);
	}

	return NULL;
}



//GOAP AI PLANNER

//ctor and dtor
AIPlanner::AIPlanner(AGOAPController* pAgent)
{
	m_pAgent = pAgent;

	//
	InitAIPlanner();
}

AIPlanner::~AIPlanner()
{

}

//initalise the planner (subsystems)
void AIPlanner::InitAIPlanner()
{
	//
	m_AStarStoragePlanner.InitAStarStoragePlanner(&m_AStarMachine);

	//
	m_AStarMachine.InitAStarMachine(m_pAgent, &m_AStarStoragePlanner, &m_AStarGoalPlanner, &m_AStarMapPlanner);

	//
	m_AStarMapPlanner.InitAStarMapPlanner(m_pAgent);

	//
	m_AStarMapPlanner.BuildEffectActionsTable();
}

//returns true if successful in building a plan that will satisfy the supplied Goal
bool AIPlanner::BuildPlan(GoalAbstract* pGoal)
{
	// Initialise the planner.
	m_AStarGoalPlanner.InitAStarGoalPlanner(m_pAgent, &m_AStarMapPlanner, pGoal);

	// Set the start of the search to -1, indicating that
	// the search starts from the Goal rather than from an Action.
	//(regressive search is more effecient than a brute force forward search)
	m_AStarMachine.SetAStarSource((ENUM_ASTAR_NODE_ID)-1);

	// Run the AStar machine to search for a valid plan to satisfy the Goal.
	m_AStarMachine.RunAStar();

	// If after the search the current node is NULL, then no valid plan was found.
	AStarNodePlanner* pNode = (AStarNodePlanner*)(m_AStarMachine.GetAStarNodeCurrent());

	if (!pNode) {
		//Log Message to File/Screen: "No plan found for" + pBrain->GetOwner()->m_szName;
		return false;
	}

	// Create a new plan.
	Plan* pPlan = new Plan(m_pAgent);

	//Log Message to File/Screen: "No plan found for" + pBrain->GetOwner()->m_szName;

	// Iterate over nodes in the planner's search path, and add them to the plan.
	ENUM_ACTION_TYPE eActionType;
	PlanStep* pPlanStep;

	// Set to last s
	PLAN_STEP_LIST::TIterator itPlan = pPlan->m_listPlanSteps.CreateIterator();
	//pPlan->m_listPlanSteps.Top()
	//AIPLAN_STEP_LIST::iterator itPlan = pPlan->m_lstAIPlanSteps.end();
	//how to get itierator/index of last position???

	while (pNode) {
		// If the Action is Invalid, this is the final node.

		//convert between map nodes and ai actions
		eActionType = m_AStarMapPlanner.ConvertID_AStarNode2Action(pNode->eAStarNodeID);

		if (eActionType == kAct_InvalidType) {
			break;
		}

		// Create a new plan step.
		pPlanStep = new PlanStep();

		// Set the Action for this plan step.
		//log message : "Current Action for" + pBrain->GetOwner()->m_szName;

		pPlanStep->eActionType = eActionType;

		// Advance the plan to the next node.
		pNode = (AStarNodePlanner*)(pNode->pAStarParent);

		// Copy the world state from the node. This is the world state that should be achieved by taking this step of the plan.
		pPlanStep->wsWorldState.CopyWorldState(pNode->wsWorldStateGoal);

		// Add the new step to the plan.
		pPlan->m_listPlanSteps.Emplace(pPlanStep);
	}

	// Set the new plan for the Goal.
	pGoal->SetPlan(pPlan);

	// Successfully built a plan.
	return true;
}

//Merge properties from the goal world state into the current world state,if not already present.
//Evaluate true world state of properties that were not orginally in the current world state.
void AIPlanner::MergeWorldStates(AGOAPController* pAgent, WorldState& wsCurrentState, WorldState& wsGoalState)
{

	STRUCT_WORLDSTATE_PROP* pProp;

	WSPROP_FLAGS* pFlagsCurrent = wsCurrentState.GetWSPropSetFlags();
	WSPROP_FLAGS* pFlagsGoal = wsGoalState.GetWSPropSetFlags();

	// NOTE: The current and goal world states only contain properties relevant to the Goal
	// that the planner is trying to satisfy, and properties added by preconditions of Actions.
	// The AI Agent's view of the true world state may contain many more properties.

	// Iterate over properties in goal world state.
	for (uint32 iProp = 0; iProp < kWSKey_Count; ++iProp)
	{
		// Continue if property already exists in current world state.
		if ((!pFlagsGoal->test(iProp)) || (pFlagsCurrent->test(iProp))) {
			continue;
		}

		pProp = wsGoalState.GetWSProp(iProp);

		if (!pProp) {
			continue;
		}

		// Add property to current world state.
		STRUCT_WORLDSTATE_PROP prop;
		prop.eWSKey = pProp->eWSKey;

		prop.eWSType = kWSType_Unset;

		// Get the true current value of the world state property, as determined by the AI's actual world state.
		EvaluateWorldStateProp(pAgent, prop);

		wsCurrentState.SetWSProp(&prop);
	}
}

//evaluate the true value of a world state property
void AIPlanner::EvaluateWorldStateProp(AGOAPController* pAgent, STRUCT_WORLDSTATE_PROP& prop)
{
	WorldState* pWorldState = pAgent->GetWorldState();

	if (!pWorldState) {
		return;
	}

	//pWSProp is NULL if prop.eWSKey not in agents worldstate
	STRUCT_WORLDSTATE_PROP* pWSProp = pWorldState->GetWSProp(prop.eWSKey);

	if (!pWSProp) {
		//AIASSERT( 0, pAI->m_hObject, "CAIPlanner::EvaluateWorldStateProp: Unhandled World State." );
		return;
	}

	prop = *pWSProp;
}