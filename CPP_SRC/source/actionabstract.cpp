
#include "actionabstract.h"
#include "agent.h"
#include "utils.h"

//Constructor & Deconstructor
ActionAbstract::ActionAbstract()
{
	m_eActionType = kAct_InvalidType;
	m_fActionCost = FLT_MAX;
	m_fActionPrecedence = 0.0f;
	m_bActionIsInterruptible = false;
	m_fActionProbability = 0.0f;

}

ActionAbstract::~ActionAbstract()
{
	m_wsWorldStatePreconditions.ResetWS();
	m_wsWorldStateEffects.ResetWS();
}


//initialise the action
void ActionAbstract::InitAction()
{

}

//set this actions preconditions in the plans goal world state.
void ActionAbstract::SetPlanWSPreconditions(Agent* pAgent, WorldState& wsGoalState)
{
	STRUCT_WORLDSTATE_PROP*	pPrecondition;
	STRUCT_WORLDSTATE_PROP*	pProp;

	//Iterate over all preconditions.
	for(unsigned int iPrecondition = 0; iPrecondition < kWSKey_Count; ++iPrecondition)
	{
		//get a pointer to the indexed precondition
		pPrecondition = m_wsWorldStatePreconditions.GetWSProp(iPrecondition);

		if(!pPrecondition)
		{
			continue;
		}

		// The precondition IS NOT a variable set its values in the goal state

		if( pPrecondition->eWSType != kWSType_Variable)
		{
			//needs a type cast from agent to baseobject?
			//or only when going from baseobject to agent?
			wsGoalState.SetWSProp( pPrecondition->eWSKey, kWSType_int, pPrecondition->iWSValue ) ;
		}


		//the precondition IS a variable
		//get a pointer to the world state property that the varible references
		//and set the value in the goal state with that referenced value.
		else
		{
			pProp = wsGoalState.GetWSProp( pPrecondition->eWSKey );

			if(pProp)
			{
				wsGoalState.SetWSProp( pPrecondition->eWSKey, kWSType_int, pProp->iWSValue  );
			}

		}
	}
}


//returns true is this actions preconditions are met in the plans current world state
bool ActionAbstract::ValidateWSPreconditions(Agent* pAgent, WorldState& wsCurrentState, WorldState& wsGoalState, ENUM_WORLDSTATE_PROP_KEY* pFailedWSKey)
{
	STRUCT_WORLDSTATE_PROP*	pPrecondition;
	STRUCT_WORLDSTATE_PROP*	pPropCurrent;
	STRUCT_WORLDSTATE_PROP*	pPropGoal;

	//iterate over all preconditions
	for(unsigned int iPrecondition = 0; iPrecondition < kWSKey_Count; ++iPrecondition )
	{
		//Get a pointer to the indexed precondition
		pPrecondition = m_wsWorldStatePreconditions.GetWSProp(iPrecondition);

		if(!pPrecondition)
		{
			continue;
		}

		//The precondition IS NOT a variable
		//determine if the current world state value matches the preconditions value

		if( pPrecondition->eWSType != kWSType_Variable )
		{
			pPropCurrent = wsCurrentState.GetWSProp( pPrecondition->eWSKey );

			if( pPropCurrent && 
				( ( pPropCurrent->iWSValue != pPrecondition->iWSValue ) ||
				  ( pPropCurrent->eWSType != pPrecondition->eWSType ) ) )
			{

				if(pFailedWSKey)
				{
					*pFailedWSKey = pPrecondition->eWSKey;
				}

				return false;
			}
		}

		//the precondition IS a variable
		//determine if the current world states value matches
		//the referenced value in the goal state
		else
		{
			pPropCurrent = wsCurrentState.GetWSProp( pPrecondition->eWSKey );
			pPropGoal    = wsGoalState.GetWSProp( pPrecondition->eVariableWSKey );

			if (pPropCurrent && pPropGoal)
			{
				if( ( pPropCurrent->iWSValue != pPropGoal->iWSValue ) ||
					( pPropCurrent->eWSType  != pPropGoal->eWSType))
				{
				
					if(pFailedWSKey)
					{
						*pFailedWSKey = pPrecondition->eWSKey;
					}

					return false;
				}
			}
		}
	}

	//precondtions are valid
	return true;
}


//returns true if this actions effects are not already met in the plans current world state
bool ActionAbstract::ValidateWSEffects(Agent* pAgent, WorldState& wsCurrentState, WorldState& wsGoalState)
{
	STRUCT_WORLDSTATE_PROP*	pPropEffect;
	STRUCT_WORLDSTATE_PROP*	pPropCurrent;
	STRUCT_WORLDSTATE_PROP*	pPropGoal;

	//iterate over all effects
	for(unsigned int iEffect = 0; iEffect < kWSKey_Count; ++iEffect )
	{
		//get a pointer to the indexed
		pPropEffect = m_wsWorldStateEffects.GetWSProp(iEffect);

		if(!pPropEffect)
		{
			continue;
		}

		//the effect IS NOT a variable.
		//determine if the current world states value matches the effects value

		if( pPropEffect->eWSType != kWSType_Variable )
		{
			pPropCurrent = wsCurrentState.GetWSProp( pPropEffect->eWSKey );

			//bit unweildy this - basically all need to be true
			if(  pPropCurrent && ( ( pPropCurrent->iWSValue == pPropEffect->iWSValue )
							  && ( pPropCurrent->eWSType == pPropEffect->eWSType )
							  && ( pPropCurrent->eWSType != kWSType_Unset )
							  && ( pPropCurrent->eWSType != kWSType_InvalidType ) ) )
			{
				continue;
			}

		}

		//the effect IS a variable
		//determine if the current world states value matches
		//the referenced value in the goal state

		else
		{
			pPropCurrent = wsCurrentState.GetWSProp( pPropEffect->eWSKey );
			pPropGoal    = wsGoalState.GetWSProp( pPropEffect->eVariableWSKey);


			if(pPropCurrent && pPropGoal)
			{
				//again nasty && block
				if( ( pPropGoal->iWSValue == pPropCurrent->iWSValue ) &&
					( pPropGoal->eWSType  == pPropCurrent->eWSType ) &&
					( pPropGoal->eWSType  != kWSType_Unset) &&
					( pPropGoal->eWSType  != kWSType_InvalidType) &&
					( pPropCurrent->eWSType != kWSType_Unset) &&
					( pPropCurrent->eWSType != kWSType_InvalidType) )
				{
					continue;
				}
			}
		}

		// an effect not met detected. this action is valid
		return true;
	}

	// all effects are met. this action is invalid
	return false;
}




// returns true if this action is valid to continue running
bool ActionAbstract::ValidateAction(Agent* pAgent)
{

	if(!pAgent)
	{
		return false;
	}

	//has something went gone wrong with the state?
	//if( ( pAgent->GetState() && ( pAgent->GetState()->GetStateStatus() == kStateStatus_Failed) ) )
	//{
	//	return false;
	//}

	//otherwise continue running
	return true;
}


//returns true if this action can potentially solve and of the unsatisfied properties of the plans goal state.
//This is used by the planner to find actions that are neighbours of the current world state
bool ActionAbstract::CanSolvePlanGoalWS(Agent* pAgent, WorldState& wsCurrentState, WorldState& wsGoalState)
{ 
	STRUCT_WORLDSTATE_PROP*	pPropEffect;
	STRUCT_WORLDSTATE_PROP*	pPropCurrent;
	STRUCT_WORLDSTATE_PROP*	pPropGoal;

	//iterate over all effects
	for(unsigned int iEffect = 0; iEffect < kWSKey_Count; ++iEffect )
	{
		//get a pointer to the indexed effect
		pPropEffect = m_wsWorldStateEffects.GetWSProp(iEffect);

		if(!pPropEffect)
		{
			continue;
		}

		//determine if the effect matches any of the world state properties
		//that are different in the current and goal states.

		pPropCurrent = wsCurrentState.GetWSProp( pPropEffect->eWSKey );
		pPropGoal    = wsGoalState.GetWSProp( pPropEffect->eWSKey );

		if (pPropCurrent && pPropGoal)
		{
			if( ( pPropCurrent->pWSValue != pPropGoal->pWSValue ) ||
				( pPropCurrent->eWSType != pPropGoal->eWSType) )
			{
				return true;
			}
		}
	}

	//this action cannot satisfy any of the
	//unmatched/unsatisfied worldstate properties
	return false;
}


void ActionAbstract::SolvePlanWSVariable(Agent* pAgent, WorldState& wsCurrentState, WorldState& wsGoalState)
{
	STRUCT_WORLDSTATE_PROP*	pPropEffect;
	STRUCT_WORLDSTATE_PROP*	pProp;

	//iterate over all effects
	for(unsigned int iEffect = 0; iEffect < kWSKey_Count; ++iEffect )
	{
		//get a pointer to the indexed effect
		pPropEffect = m_wsWorldStateEffects.GetWSProp(iEffect);

		if(!pPropEffect)
		{
			continue;
		}

		//the effect is not a variable
		//get the value of the propety effected
		if( pPropEffect->eWSType != kWSType_Variable)
		{
			pProp = wsGoalState.GetWSProp( pPropEffect->eWSKey );
		}

		// the effect is a variable
		// get the value of the property referenced by the variable effected
		else
		{
			pProp = wsGoalState.GetWSProp( pPropEffect->eVariableWSKey);
		}

		// set the current world states property to match the goal.
		if(pProp)
		{
			// note that we might set the prop to a defferent key, as specified by a variable
			if( pProp->eWSType != kWSType_Variable )
			{
				//"ActionAbstract::SolvePlamWSVariable:  Setting a world property to another variable"
				std::cout << "ActionAbstract: Setting a world property to another variable" << std::endl;
			}

			wsCurrentState.SetWSProp( pPropEffect->eWSKey, pProp);
		}
	}
}

//applys the effects of the action to the current world state
void ActionAbstract::ApplyWSEffect(Agent* pAgent, WorldState* pwsCurrentState, WorldState* pwsGoalState)
{
	if( !(pwsCurrentState && pwsGoalState) ) 
	{
		return;
	}

	STRUCT_WORLDSTATE_PROP*	pPropEffect;
	STRUCT_WORLDSTATE_PROP*	pProp;

	//iterate over all effects
	for(unsigned int iEffect = 0; iEffect < kWSKey_Count; ++iEffect )
	{
		//get a pointer to the indexed effect
		pPropEffect = m_wsWorldStateEffects.GetWSProp(iEffect);

		if(!pPropEffect)
		{
			continue;
		}

		//the effect is not a variable
		//set the effect on the current world state

		if( pPropEffect->eWSType != kWSType_Variable )
		{
			pwsCurrentState->SetWSProp( pPropEffect->eWSKey, pPropEffect->eWSType, pPropEffect->iWSValue);
		}

		//the effect is a variable
		// lookup the value of the effects variable, and set the effect on the current world state
		else
		{
			pProp = pwsGoalState->GetWSProp( pPropEffect->eVariableWSKey );

			if(pProp)
			{
				pwsCurrentState->SetWSProp( pPropEffect->eWSKey, pProp->eWSType, pProp->iWSValue  );
			}
		}
	}
}

//activate action
void ActionAbstract::ActivateAction(Agent* pAgent, WorldState& wsGoalState)
{

}