

#include "agent.h"
#include "astarplanner.h"
#include "aiplanner.h"
#include "goalabstract.h"
#include "blackboard.h"
#include "utils.h"

extern Agent* g_pAgent;

// AStarNodePlanner

AStarNodePlanner::AStarNodePlanner()
{
	pAStarMachine = NULL;
	wsWorldStateCurrent.ResetWS();
	wsWorldStateGoal.ResetWS();
}

void AStarNodePlanner::DebugPrintExpand()
{
	AStarMapPlanner*	pMap = (AStarMapPlanner*)pAStarMachine->GetAStarMap();
	
	if( pMap )
	{
		ActionAbstract* pAction = pMap->GetAction( eAStarNodeID );
		
		if( pAction )
		{
			//AITRACE( AIShowPlanner, ( (HOBJECT)NULL, "Planner Expanding Action: %s", s_aszActionTypes[pAction->GetActionRecord()->eActionType] ) );
			std::cout << "Planner Expanding Action: " << s_aszActionTypes[pAction->GetActionType()] << std::endl;
		}
		else 
		{
			//AITRACE( AIShowPlanner, ( (HOBJECT)NULL, "Planner Expanding Goal" ) );
			std::cout << "Planner Expanding Goal. " <<  std::endl;
		}
	}
}

void AStarNodePlanner::DebugPrintNeighbour()
{
	AStarMapPlanner*	pMap = (AStarMapPlanner*)pAStarMachine->GetAStarMap();
	
	if( pMap )
	{
		ActionAbstract* pAction = pMap->GetAction( eAStarNodeID );
		
		if( pAction )
		{
			//AITRACE( AIShowPlanner, ( (HOBJECT)NULL, "   Neighbour Action: %s", s_aszActionTypes[pAction->GetActionRecord()->eActionType] ) );
			std::cout<< "Neighbour Action: " << s_aszActionTypes[pAction->GetActionType()] << std::endl;
		}
	}
}


// AStarGoalPlanner

//Constructor / Destructor.
AStarGoalPlanner::AStarGoalPlanner()
{
	m_eAStarNodeTarget = kASTARNODE_Invalid;
	m_pAStarMapPlanner = NULL;
	m_pGoal = NULL;
}

AStarGoalPlanner::~AStarGoalPlanner()
{
	//
}

          
//Initialise astar goal.
void AStarGoalPlanner::InitAStarGoalPlanner( Agent* pAgent, AStarMapPlanner* pAStarMapPlanner, GoalAbstract* pGoal )
{
	m_pAgent = pAgent;
	m_pAStarMapPlanner = pAStarMapPlanner; 
	m_pGoal = pGoal;
}


//Get a best-guess distance from a node to the goal.
float AStarGoalPlanner::GetHeuristicDistance( AStarNodeAbstract* pAStarNode )
{
	AStarNodePlanner* pNode = ( AStarNodePlanner* )pAStarNode;

	// The AStarMapPlanner converts between A Star Node IDs and Action enums.
	// The initial node in the search tree is the goal, and does not refer to 
	// any action. It has a node ID of -1 == kAct_InvalidType.

	// This node refers to an Action.
	ActionAbstract* pAction = m_pAStarMapPlanner->GetAction( pAStarNode->eAStarNodeID );

	if( pAction )
	{
		// Use the Action to solve unsatisfied world state properties.
		AStarNodePlanner* pNode = ( AStarNodePlanner* )pAStarNode;
		pAction->SolvePlanWSVariable( m_pAgent, pNode->wsWorldStateCurrent, pNode->wsWorldStateGoal );

		// Add additional preconditions from the Action to the goal world state.
		pAction->SetPlanWSPreconditions( m_pAgent, pNode->wsWorldStateGoal );

		// Determine the current world state values for any new world state properties.
		m_pAgent->GetGoalPlanner()->MergeWorldStates( m_pAgent, pNode->wsWorldStateCurrent, pNode->wsWorldStateGoal );
	}

	// This node refers to an Goal.
	else if( m_pGoal )
	{
		// Set the initial goal world state from the AIGoal.
		m_pGoal->SetWSSatisfaction( pNode->wsWorldStateGoal );

		// Determine the current world state values for world state properties.
		m_pAgent->GetGoalPlanner()->MergeWorldStates( m_pAgent, pNode->wsWorldStateCurrent, pNode->wsWorldStateGoal );
	}

	// Return the number of unsatisfied world state properties as an estimate of the 
	// number of actions it will take to satisfy the goal.
	// A property is unsatisfied if the value differs between the current and goal world states.
	return (float)( pNode->wsWorldStateCurrent.GetNumWorldStateDifferences( pNode->wsWorldStateGoal ) );
}

   
//Return the actual cost from one node to another.
float AStarGoalPlanner::GetActualCost(AStarNodeAbstract* pAStarNodeA, AStarNodeAbstract* pAStarNodeB )
{
	AStarNodePlanner* pNodeA = (AStarNodePlanner*)pAStarNodeA;
	AStarNodePlanner* pNodeB = (AStarNodePlanner*)pAStarNodeB;

	// Copy the current and goal world states from a parent node.
	pNodeB->wsWorldStateCurrent.CopyWorldState( pNodeA->wsWorldStateCurrent );
	pNodeB->wsWorldStateGoal.CopyWorldState( pNodeA->wsWorldStateGoal );

	// Return the cost of this node.
	ActionAbstract* pAction = m_pAStarMapPlanner->GetAction( pAStarNodeB->eAStarNodeID );
	
	if( pAction )
	{
		return pAction->GetActionCost();
	}

	return 1.0f;
}

  
//Return true if node is null, or a valid plan has been found.
bool AStarGoalPlanner::IsAStarFinished( AStarNodeAbstract* pAStarNode )
{
	// AStar is finished, because there are no more nodes.
	if( !pAStarNode )
	{
		return true;
	}

	// AStar is finished because a plan has been found that satisfies the AIGoal.
	AStarNodePlanner* pNode = (AStarNodePlanner*)pAStarNode;
	
	if( IsPlanValid( pNode ) )
	{
		return true;
	}

	// AStar is not finished.
	return false;
}


//Return true if the plan satisfies the Goal, from the current world state.
bool AStarGoalPlanner::IsPlanValid( AStarNodePlanner* pAStarNode )
{
	//AITRACE( AIShowPlanner, ( (HOBJECT)NULL, "Testing plan validity..." ) );
	std::cout << "Testing Plan Validity........" << std::endl;
	//printf();

	// Uncomment for debugging plans!
	unsigned int iStep = 1;
	AStarNodePlanner* pAStarNodeTemp = pAStarNode;
	ActionAbstract* pActionTemp = m_pAStarMapPlanner->GetAction( pAStarNodeTemp->eAStarNodeID );
	
	while( pActionTemp )
	{
		//AITRACE( AIShowPlanner, ( (HOBJECT)NULL, "  %d: %s", iStep, s_aszActionTypes[pActionTemp->GetActionRecord()->eActionType] ) );
		std::cout << "ActionType: "<< s_aszActionTypes[pActionTemp->GetActionType()] << " iStep: " << iStep << std::endl;

		++iStep;
		pAStarNodeTemp = (AStarNodePlanner*)( pAStarNodeTemp->pAStarParent );
		pActionTemp = pAStarNodeTemp ? m_pAStarMapPlanner->GetAction( pAStarNodeTemp->eAStarNodeID ) : NULL;
	}

	// Get the true current world state.
	WorldState wsWorldState;

	//g_pAIPlanner->MergeWorldStates( m_pAgent, wsWorldState, pAStarNode->wsWorldStateCurrent );
	//m_pAgent->GetGoalPlanner()->MergeWorldStates( m_pAgent, wsWorldState, pAStarNode->wsWorldStateCurrent );
	m_pAgent->GetGoalPlanner()->MergeWorldStates( m_pAgent, wsWorldState, pAStarNode->wsWorldStateCurrent );

	// Walk the sequence of Actions, modifying the world state on each iteration.
	ActionAbstract* pAction = m_pAStarMapPlanner->GetAction( pAStarNode->eAStarNodeID );
	
	if( !pAction )
	{
		//AITRACE( AIShowPlanner, ( (BaseObject*)m_pAgent->GetOwner(), "Plan invalid - No actions." ) );
		std::cout << "Plan invalid - No Actions" << std::endl;
		return false;
	}

	AStarNodePlanner* pNodeParent = NULL;
	
	while( pAction )
	{
		//AITRACE( AIShowPlanner, ( (HOBJECT)NULL, "PlanStep: %s", s_aszActionTypes[pAction->GetActionRecord()->eActionType] ) );
		std::cout << "PlanStep: " << s_aszActionTypes[pAction->GetActionType()] << std::endl;

		pNodeParent = (AStarNodePlanner*)( pAStarNode->pAStarParent );

		// Bail if the AIAction's effects are already met by the current world state.
		if( !pAction->ValidateWSEffects( m_pAgent, wsWorldState, pNodeParent->wsWorldStateGoal ) )
		{
			//AITRACE( AIShowPlanner, ( (BaseObject*)m_pAgent->GetOwner(), "Effect failed: %s", s_aszActionTypes[pAction->GetActionRecord()->eActionType] ) );
			std::cout << "Effect Failed: " << s_aszActionTypes[pAction->GetActionType()] << std::endl;
			return false;
		}

		// Bail if the AIAction's preconditions are not met by the current world state.
		ENUM_WORLDSTATE_PROP_KEY eFailedWSK = kWSKey_InvalidKey;
		
		if( !pAction->ValidateWSPreconditions( m_pAgent, wsWorldState, pNodeParent->wsWorldStateGoal, &eFailedWSK ) )
		
		{
			//AITRACE( AIShowPlanner, ( (BaseObject*)m_pAgent->GetOwner(), "Precondition failed: %s (%s)", s_aszActionTypes[pAction->GetActionRecord()->eActionType], GetAIWorldStatePropName( eFailedWSK ) ) );
			std::cout << "Precondition Failed: " << s_aszActionTypes[pAction->GetActionType()] << std::endl;
			return false;
		}

		//if( !pAction->ValidateContextPreconditions( m_pAgent, pNodeParent->wsWorldStateGoal, IS_PLANNING ) )
		if( !pAction->ValidateContextPreconditions( m_pAgent, pNodeParent->wsWorldStateGoal, true ) )
		{
			//AITRACE( AIShowPlanner, ( (BaseObject*)v->GetOwner(), "Real-time Precondition failed: %s", s_aszActionTypes[pAction->GetActionRecord()->eActionType] ) );
			std::cout << "Real-time Precondition failed: " << s_aszActionTypes[pAction->GetActionType()] << std::endl;
			return false;
		}

		// Bail if probability fails.
		if( ( pAction->GetActionProbability( m_pAgent ) < 1.0f ) &&
			( pAction->GetActionProbability( m_pAgent ) < GetRandom( 0.0f, 1.0f ) ) )
		{
			//AITRACE( AIShowPlanner, ( (BaseObject*)m_pAgent->GetOwner(), "Probability failed: %s", s_aszActionTypes[pAction->GetActionRecord()->eActionType] ) );
			std::cout << "Probability Failed: " << s_aszActionTypes[pAction->GetActionType()] << std::endl;
			pAction->FailActionProbability( m_pAgent );
			return false;
		}

		// Apply the Actions effects to the world state.
		pAction->ApplyWSEffect( m_pAgent, &wsWorldState, &( pNodeParent->wsWorldStateGoal ) );
		
		// Get the next Action in the sequence.
		pAStarNode = (AStarNodePlanner*)( pAStarNode->pAStarParent );
		
		if( pAStarNode )
		{
			pAction = m_pAStarMapPlanner->GetAction( pAStarNode->eAStarNodeID );
		}
	}

	// The final node is the Goal.
	// Return true if the goal is satisfied by the world state created by the sequence of actions.

	if( m_pGoal && pNodeParent )
	{
		if( pNodeParent->wsWorldStateGoal.GetNumUnsatisfiedWorldStateProps( wsWorldState ) > 0 )
		{
			//AITRACE( AIShowPlanner, ( (BaseObject*)m_pAgent->GetOwner(), "Satisfaction conditions not met." ) );
			std::cout << "Satisfaction conditions not met"<< std::endl;
			return false;
		}

		std::cout << "Plan is Valid" << std::endl;
		return true;
	}

	return false;
}


// AStarStoragePlanner
          
//Initialise the A Star Storage. 
void AStarStoragePlanner::InitAStarStoragePlanner( AStarMachine* pAStarMachine )
{
	m_pAStarMachine = pAStarMachine;
}

    
//Create an AStarNode with a specified ID. 
AStarNodeAbstract* AStarStoragePlanner::CreateAStarNode( ENUM_ASTAR_NODE_ID eAStarNode )
{
	// Create a node with a specified AStarNodeID.
	// The AIAStarMapPlanner converts between NodeIDs and Action enums.

	AStarNodePlanner* pNode = new AStarNodePlanner();

	pNode->eAStarNodeID = eAStarNode;
	pNode->pAStarMachine = m_pAStarMachine;

	return pNode;
}

       
//Destroy an AStarNode. 
void AStarStoragePlanner::DestroyAStarNode( AStarNodeAbstract* pAStarNode )
{
	if(pAStarNode)
	{
		delete pAStarNode;
		pAStarNode = NULL;
	}
}


// AStarMapPlanner

//Constructor/ Destructor
AStarMapPlanner::AStarMapPlanner()
{
	m_pAgent= NULL;
	m_bEffectTableBuilt = false;
}

AStarMapPlanner::~AStarMapPlanner()
{
	//
}

         
//Build the table that maps Actions to their effects.
void AStarMapPlanner::BuildEffectActionsTable()
{
	// Check if table already built.
	if( m_bEffectTableBuilt )
		return;

	// Build a table that maps effects to actions.
	int iKey;
	ActionAbstract* pAction;
	WorldState* pEffects;
	WSPROP_FLAGS* pFlags;

	// Iterate over all actions, storing them in the table indexed by their effects.
	int cActions =  m_pAgent->GetActionStorage()->GetNumActions();

	for( int iAction=0; iAction < cActions; ++iAction )
	{
		// Access an action.
		pAction =  m_pAgent->GetActionStorage()->GetAction( ( ENUM_ACTION_TYPE )iAction );
		
		if( !pAction )
		{
			continue;
		}

		// Get the list of effects for this action.
		pEffects = pAction->GetActionEffects();
		
		if( !pEffects )
		{
			continue;
		}

		// Get the bit flags representing the effects.
		pFlags = pEffects->GetWSPropSetFlags();
		
		if( !pFlags )
		{
			continue;
		}

		// Iterate over all possible WorldState property keys.
		for( iKey=0; iKey < kWSKey_Count; ++iKey )
		{
			// If the effect has something set for this key,
			// add the action's class enum to the table under this effect.
			if( pFlags->test( iKey ) )
			{
				m_listEffectActions[iKey].push_back( pAction->GetActionType() );
			}
		}
	}

	m_bEffectTableBuilt = true;
}

    
//Initialize the AStar map.
void AStarMapPlanner::InitAStarMapPlanner( Agent* pAgent )
{
	if( m_pAgent == pAgent )
	{
		return;
	}

	m_pAgent = pAgent;
}

     
//Compare function used to qsort actions.
int CompareActions(const void *arg1, const void *arg2 )
{
	if( !( arg1 && arg2 ) )
	{
		return 0;
	}

	// Actions are sorted by precedence.
	ENUM_ACTION_TYPE eAction1 = *( ENUM_ACTION_TYPE* )arg1;
	ENUM_ACTION_TYPE eAction2 = *( ENUM_ACTION_TYPE* )arg2;

	//pAgent comes from main?
	ActionAbstract* pAction1 =  g_pAgent->GetActionStorage()->GetAction( eAction1 );
	ActionAbstract* pAction2 =  g_pAgent->GetActionStorage()->GetAction( eAction2 );

	if( !( pAction1 && pAction2 ) )
	{
		return 0;
	}

	if( pAction1->GetActionPrecedence() < pAction2->GetActionPrecedence() )
	{
		return 1;
	}

	if( pAction1->GetActionPrecedence() > pAction2->GetActionPrecedence() )
	{
		return -1;
	}

	return 0;
}

    
//Return number of Neighbours for an AStarNode.
//Also build a temporary list of who the Neighbours are, to be used in GetAStarNeighbour().
int AStarMapPlanner::GetNumAStarNeighbours( Agent* pAgent, AStarNodeAbstract* pAStarNode )
{
	// Sanity check.
	if( !pAStarNode )
	{
		return 0;
	}

	// A Neighbour is an Action that has an effect that has the potential
	// to satisfy one of the unsatisfied properties of the goal.
	// Neighbours are based only on the property key enum, not on the associated
	// value.  For example, both DrawWeapon and HoslterWeapon are Neighbours
	// of an AStarNode that has an unsatisfied property of kWSK_WeaponArmed.

	m_cNeighbourActions = 0;
	ACTION_TYPE_LIST::iterator itAction;
	ENUM_ACTION_TYPE eAction;
	ActionAbstract* pAction;

	AStarNodePlanner* pNode = (AStarNodePlanner*)pAStarNode;
	STRUCT_WORLDSTATE_PROP* pPropCur;
	STRUCT_WORLDSTATE_PROP* pPropGoal;
	ENUM_WORLDSTATE_PROP_KEY eWSKey;

	// Get the flags representing properties set in the current and goal world states.

	WSPROP_FLAGS* pFlagsCur = pNode->wsWorldStateCurrent.GetWSPropSetFlags();
	WSPROP_FLAGS* pFlagsGoal = pNode->wsWorldStateGoal.GetWSPropSetFlags();

	// Iterate over all possible effect world state properties.
	for( unsigned int iEffect=0; iEffect < kWSKey_Count; ++iEffect )
	{
		// Ignore the effect if it does not appear in current and goal states.
		if( !( pFlagsCur->test( iEffect ) && pFlagsGoal->test( iEffect ) ) )
		{
			continue;
		}

		eWSKey = (ENUM_WORLDSTATE_PROP_KEY)iEffect;

		// Determine if the effect matches any of the world state properties
		// that are different in the current and goal world states.

		pPropCur = pNode->wsWorldStateCurrent.GetWSProp( eWSKey);
		pPropGoal = pNode->wsWorldStateGoal.GetWSProp( eWSKey);

		if( pPropCur && pPropGoal )
		{
			if( pPropCur->pWSValue != pPropGoal->pWSValue ) 
			{
				// Add actions for this effect to the list of Neighbours.
				for( itAction = m_listEffectActions[eWSKey].begin(); itAction != m_listEffectActions[eWSKey].end(); ++itAction )
				{
					eAction = *itAction;

					// Ignore actions if they are not in the AIs action set.
					if (!m_pAgent->GetActionStorage()->IsActionInActionList(eAction))
					{
						continue;
					}

					//pAction = g_pActionManager->GetAction( eAction );
					pAction = m_pAgent->GetActionStorage()->GetAction(eAction);

					// Ignore actions if their context preconditions are not met.
					if( !( pAction && pAction->ValidateContextPreconditions( m_pAgent, pNode->wsWorldStateGoal, true ) ) )
					{
						//AITRACE( AIShowPlanner, ( (HOBJECT)NULL, "  Action not Neighbour due to context preconditions: %s", s_aszActionTypes[pAction->GetActionRecord()->eActionType] ) );
						std::cout << "Action not Neighbour due to context preconditions: " << s_aszActionTypes[pAction->GetActionType()] << std::endl;
						continue;
					}

					m_aNeighbourActions[m_cNeighbourActions] = eAction;
					++m_cNeighbourActions;

					if( m_cNeighbourActions >= kAct_Count )
					{
						break;
					}
				}
			}
		}

		if( m_cNeighbourActions >= kAct_Count )
		{
			break;
		}
	}

	// Sort Actions by precedence.
	// This sorting will produce plans that have actions sorted by precedence,

	//hmm hmmm

	//qsort desc:
	if( m_cNeighbourActions > 1 )
	{
		//qsort( (void*)m_aNeighbourActions, (size_t)m_cNeighbourActions, sizeof( ENUM_ACTION_TYPE ), iResult );
		qsort( (void*)m_aNeighbourActions, (size_t)m_cNeighbourActions, sizeof( ENUM_ACTION_TYPE ),CompareActions );
	}

	return m_cNeighbourActions;
}


//Return the node ID of a specified Neighbour.
ENUM_ASTAR_NODE_ID AStarMapPlanner::GetAStarNeighbour( Agent* pAgent, AStarNodeAbstract* pAStarNode, int iNeighbour, AStarStorageAbstract* pAStarStorage )
{
	return ConvertID_AIAction2AStarNode( m_aNeighbourActions[iNeighbour] );
}
          
//Set AStar flags for a specified node.
void AStarMapPlanner::SetAStarFlags( ENUM_ASTAR_NODE_ID eAStarNode, unsigned long dwFlags, unsigned long dwMask )
{
	// AIAStarMapPlanner does not keep track of flags because a nodeID may 
	// be reused many times, and does not refer to the same node.
	// This is due to the fact that an Action may be used multiple times 
	// in the same plan for different reasons (e.g. GotoNode to different nodes).
	// So, reusing the same nodeID does not indicate a cycle, as it would 
	// in physical pathfinding, where a nodeID refers to a point in space.
}


//Get AStar flags for a specified node.
unsigned long AStarMapPlanner::GetAStarFlags( ENUM_ASTAR_NODE_ID eAStarNode )
{
	return kASTAR_Unchecked;
}

    
//Return a pointer to the AIAction referenced by the nodeID.
ActionAbstract* AStarMapPlanner::GetAction( ENUM_ASTAR_NODE_ID eAStarNode )
{
	return  m_pAgent->GetActionStorage()->GetAction( ConvertID_AStarNode2Action( eAStarNode ) ); 
}



