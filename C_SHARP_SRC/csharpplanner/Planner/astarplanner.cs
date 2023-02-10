
//------------------------------------------------------------------------
//
//  Name:   astarplanner.cs
//
//  Desc:   AStar Node, Goal, Storage, and Map classes for finding
//          action plans from the planner. (from fear sdk)
//
//  Author: Brett Keir Jones 2015
//
//------------------------------------------------------------------------

using System;
using System.Collections;
using System.Collections.Generic;

using NSAStarMachine;
using NSWorldState;
using NSAgent;
using NSGoalAbstract;
using NSActionAbstract;
using NSAStarStorageLinkedList;
using NSUtils;

//

namespace NSAStarPlanner
{

   public class AStarNodePlanner : AStarNodeAbstract
   {

    public AStarNodePlanner()
    {
        aStarMachine = null;

        wsWorldStateCurrent = new WorldState();
        wsWorldStateGoal = new WorldState();

	    wsWorldStateCurrent.ResetWS();
	    wsWorldStateGoal.ResetWS();

    }

	// Debug.
	//public virtual void	DebugPrint() { Console.WriteLine("ASTAR: " + eAStarNodeID); }
    //public virtual void	DebugPrintExpand();
	//public virtual void	DebugPrintNeighbour();

    public WorldState   wsWorldStateCurrent;
	public WorldState   wsWorldStateGoal;
	public AStarMachine	aStarMachine;
   }

   //
   public class AStarGoalPlanner : AStarGoalAbstract
   {
        public AStarGoalPlanner() 
        {
            m_eAStarNodeTarget = ENUM_ASTAR_NODE_ID.kASTARNODE_Invalid;
	        m_AStarMapPlanner = null;
        	m_Goal = null;
        }

       	protected ENUM_ASTAR_NODE_ID    m_eAStarNodeTarget;
	    protected AStarMapPlanner		m_AStarMapPlanner;
	    protected GoalAbstract			m_Goal;
	    protected Agent 				m_Agent;

	    // AStarGoalAbstract required functions.
	    public override void SetTargetNode( ENUM_ASTAR_NODE_ID eAStarNode ) { m_eAStarNodeTarget = eAStarNode; }

	    // Initialisation.
	    public void InitAStarGoalPlanner( Agent agent, ref AStarMapPlanner aStarMapPlanner, GoalAbstract goal )
        {
            m_Agent = agent;
	        m_AStarMapPlanner = aStarMapPlanner; 
	        m_Goal = goal;
        }

        public override float GetHeuristicDistance( AStarNodeAbstract aStarNode ) 
       {
           AStarNodePlanner node = (AStarNodePlanner)aStarNode;

	        // The AStarMapPlanner converts between A Star Node IDs and Action enums.
	        // The initial node in the search tree is the goal, and does not refer to 
	        // any action. It has a node ID of -1 == kAct_InvalidType.

	        // This node refers to an Action.
	        ActionAbstract action = m_AStarMapPlanner.GetAction(aStarNode.eAStarNodeID);

	        if(action != null)
	        {
		        // Use the Action to solve unsatisfied world state properties.
		        AStarNodePlanner nodeTemp = (AStarNodePlanner)aStarNode;
		        action.SolvePlanWSVariable(m_Agent, ref nodeTemp.wsWorldStateCurrent, ref nodeTemp.wsWorldStateGoal);

		        // Add additional preconditions from the Action to the goal world state.
		        action.SetPlanWSPreconditions(m_Agent, ref nodeTemp.wsWorldStateGoal);

		        // Determine the current world state values for any new world state properties.
		        m_Agent.GetGoalPlanner().MergeWorldStates(m_Agent, ref nodeTemp.wsWorldStateCurrent, ref nodeTemp.wsWorldStateGoal);
	        }

	        // This node refers to an Goal.
	        else if(m_Goal != null)
	        {
		        // Set the initial goal world state from the AIGoal.
		        m_Goal.SetWSSatisfaction(ref node.wsWorldStateGoal);

		        // Determine the current world state values for world state properties.
		        m_Agent.GetGoalPlanner().MergeWorldStates(m_Agent, ref node.wsWorldStateCurrent, ref node.wsWorldStateGoal);
	        }

	        // Return the number of unsatisfied world state properties as an estimate of the 
	        // number of actions it will take to satisfy the goal.
	        // A property is unsatisfied if the value differs between the current and goal world states.
	        return (float)(node.wsWorldStateCurrent.GetNumWorldStateDifferences(ref node.wsWorldStateGoal));
       }

        public override float GetActualCost(AStarNodeAbstract aStarNodeA, AStarNodeAbstract aStarNodeB )
        {
            AStarNodePlanner nodeA = (AStarNodePlanner)aStarNodeA;
	        AStarNodePlanner nodeB = (AStarNodePlanner)aStarNodeB;

	        // Copy the current and goal world states from a parent node.
	        nodeB.wsWorldStateCurrent.CopyWorldState(ref nodeA.wsWorldStateCurrent);
	        nodeB.wsWorldStateGoal.CopyWorldState(ref nodeA.wsWorldStateGoal);

	        // Return the cost of this node.
	        ActionAbstract action = m_AStarMapPlanner.GetAction( aStarNodeB.eAStarNodeID );
	
	        if(action != null)
	        {
		        return action.GetActionCost();
	        }

	        return 1.0f;
        }

       //
       public override bool IsAStarFinished(AStarNodeAbstract aStarNode)
       {
           // AStar is finished, because there are no more nodes.
	       if(aStarNode == null)
           {
		       return true;
	       }

	       // AStar is finished because a plan has been found that satisfies the AIGoal.
	       AStarNodePlanner node = (AStarNodePlanner)aStarNode;
	
	       if(IsPlanValid(node))
	       {
		       return true;
	       }

	       // AStar is not finished.
	       return false;
       }

       //
       public bool IsPlanValid(AStarNodePlanner aStarNode)
       {
	        Console.WriteLine("Testing Plan Validity........");

	        // Uncomment for debugging plans!
	        uint iStep = 1;
	        AStarNodePlanner aStarNodeTemp = aStarNode;
	        ActionAbstract actionTemp = m_AStarMapPlanner.GetAction(aStarNodeTemp.eAStarNodeID);
	
	        while(actionTemp != null)
	        {
                // need to sort strings out.
               // Console.WriteLine("ActionType : {0} iStep: {1}", ActionAbstract.s_aszActionTypes[(int)actionTemp.GetActionType()] ,iStep);

		        ++iStep;
		        aStarNodeTemp = (AStarNodePlanner)( aStarNodeTemp.aStarParent );
		        actionTemp = (aStarNodeTemp != null) ? m_AStarMapPlanner.GetAction(aStarNodeTemp.eAStarNodeID) : null;
	        }

	        // Get the true current world state.
            // TODO: is this correct?
	        WorldState wsWorldState = new WorldState();

	        //g_pAIPlanner->MergeWorldStates( m_pAgent, wsWorldState, pAStarNode->wsWorldStateCurrent );
	        //m_pAgent->GetGoalPlanner()->MergeWorldStates( m_pAgent, wsWorldState, pAStarNode->wsWorldStateCurrent );

           // should be ' out wsWorldState' ?
	        m_Agent.GetGoalPlanner().MergeWorldStates(m_Agent, ref wsWorldState, ref aStarNode.wsWorldStateCurrent);

	        // Walk the sequence of Actions, modifying the world state on each iteration.
	        ActionAbstract action = m_AStarMapPlanner.GetAction(aStarNode.eAStarNodeID);
	
	        if(action == null)
	        {
                Console.WriteLine("Plan invalid - No Actions");
		        return false;
	        }

	        AStarNodePlanner nodeParent = null;
	
	        while(action != null)
	        {
                //Console.WriteLine("PlanStep: {0} ", ActionAbstract.s_aszActionTypes[(int)action.GetActionType()]);

		        nodeParent = (AStarNodePlanner)( aStarNode.aStarParent );

		        // Bail if the AIAction's effects are already met by the current world state.
		        if( !action.ValidateWSEffects( m_Agent, ref wsWorldState, ref nodeParent.wsWorldStateGoal ) )
		        {
                    //Console.WriteLine("Effect Failed: {0}", ActionAbstract.s_aszActionTypes[(int)action.GetActionType()]);
			        return false;
		        }

		        // Bail if the AIAction's preconditions are not met by the current world state.
		        ENUM_WORLDSTATE_PROP_KEY eFailedWSK = ENUM_WORLDSTATE_PROP_KEY.kWSKey_InvalidKey;
		
		        if( !action.ValidateWSPreconditions( m_Agent, ref wsWorldState, ref nodeParent.wsWorldStateGoal, ref eFailedWSK ) )
		        {
                    //Console.WriteLine("Precondition Failed: {0}", ActionAbstract.s_aszActionTypes[(int)action.GetActionType()]);
			        return false;
		        }

		        //if( !pAction->ValidateContextPreconditions( m_pAgent, pNodeParent->wsWorldStateGoal, IS_PLANNING ) )
		        if( !action.ValidateContextPreconditions( m_Agent, ref nodeParent.wsWorldStateGoal, true ) )
		        {
                    //Console.WriteLine("Real-time Precondition failed: {0}", ActionAbstract.s_aszActionTypes[(int)action.GetActionType()]);
			        return false;
		        }

		        // Bail if probability fails.
		        if( ( action.GetActionProbability( m_Agent ) < 1.0f ) &&
			        ( action.GetActionProbability( m_Agent ) < Utils.GetRandom(0.0f, 1.0f))) // TODO: replace get random.
		        {
                    //Console.WriteLine("Probability Failed: {0}", ActionAbstract.s_aszActionTypes[(int)action.GetActionType()]);
			        action.FailActionProbability( m_Agent );
			        return false;
		        }

		        // Apply the Actions effects to the world state.
		        action.ApplyWSEffect( m_Agent, ref wsWorldState, ref nodeParent.wsWorldStateGoal);
		
		        // Get the next Action in the sequence.
		        aStarNode = (AStarNodePlanner)(aStarNode.aStarParent);
		
		        if(aStarNode != null)
		        {
			        action = m_AStarMapPlanner.GetAction(aStarNode.eAStarNodeID);
		        }
	        }

	        // The final node is the Goal.
	        // Return true if the goal is satisfied by the world state created by the sequence of actions.

	        if(m_Goal != null && nodeParent != null)
	        {
		        if( nodeParent.wsWorldStateGoal.GetNumUnsatisfiedWorldStateProps(ref wsWorldState) > 0 )
		        {
                    Console.WriteLine("Satisfaction conditions not met");
			        return false;
		        }

                Console.WriteLine("Plan is Valid");
		        return true;
	        }

	        return false;
       }


   }

    //
    public class AStarStoragePlanner : AStarStorageLinkedList
    {   
        protected AStarMachine	m_AStarMachine;
    
        //Initialise the A Star Storage. 
	    public void	InitAStarStoragePlanner( ref AStarMachine aStarMachine )
        {
            m_AStarMachine = aStarMachine;
        }

	    // AStarStorageLinkedList overrides.
	    public override AStarNodeAbstract CreateAStarNode( ENUM_ASTAR_NODE_ID eAStarNode )
        {
            // Create a node with a specified AStarNodeID.
	        // The AIAStarMapPlanner converts between NodeIDs and Action enums.
	        AStarNodePlanner node = new AStarNodePlanner();

	        node.eAStarNodeID = eAStarNode;
	        node.aStarMachine = m_AStarMachine;

	        return node;
        }


	    public override void DestroyAStarNode( AStarNodeAbstract aStarNode ) 
        {
            if(aStarNode != null)
	        {
		        aStarNode = null;
	        }
        }

      
    }


    //
    public class AStarMapPlanner : AStarMapAbstract
    {
    
        public AStarMapPlanner()
        {
            m_Agent = null;
	        m_bEffectTableBuilt = false;

            m_listEffectActions = new List<ENUM_ACTION_TYPE>[(int)ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count];

            for (int i = 0; i < m_listEffectActions.Length; i++)
            {
                m_listEffectActions[i] = new List<ENUM_ACTION_TYPE>();
            }

            m_aNeighbourActions = new ENUM_ACTION_TYPE[(int)ENUM_ACTION_TYPE.kAct_Count];

            for (int i = 0; i < m_aNeighbourActions.Length; i++)
            {
                m_aNeighbourActions[i] = (ENUM_ACTION_TYPE)(-1);
            }

        }

        // Initialisation.
	    public void	BuildEffectActionsTable() 
        {
            // Check if table already built.
	        if( m_bEffectTableBuilt )
		        return;

	        // Build a table that maps effects to actions.
	        int iKey;
	        ActionAbstract action;
	        WorldState wsEffects;
	        BitArray wsFlags;

	        // Iterate over all actions, storing them in the table indexed by their effects.
	        int cActions =  m_Agent.GetActionStorage().GetNumActions();

	        for( int iAction=0; iAction < cActions; ++iAction )
	        {
		        // Access an action.
		        action =  m_Agent.GetActionStorage().GetAction((ENUM_ACTION_TYPE)iAction);
		
		        if(action == null )
		        {
			        continue;
		        }

		        // Get the list of effects for this action.
		        wsEffects = action.GetActionEffects();
		
		        if( wsEffects == null )
		        {
			        continue;
		        }

		        // Get the bit flags representing the effects.
		        wsFlags = wsEffects.GetWSPropSetFlags();
		
		        if(wsFlags == null)
		        {
			        continue;
		        }

		        // Iterate over all possible WorldState property keys.
		        for( iKey=0; iKey < (int)ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count; ++iKey )
		        {
			        // If the effect has something set for this key,
			        // add the action's class enum to the table under this effect.
			        if(wsFlags.Get(iKey) == true)
			        {
				        m_listEffectActions[iKey].Add(action.GetActionType());
			        }
		        }
	        }

	        m_bEffectTableBuilt = true;

        }

	    public void	InitAStarMapPlanner( Agent agent )
        {
            if( m_Agent == agent )
	        {
		        return;
	        }

	        m_Agent = agent; //TODO: check this i ok
        }

	    // AStarMapAbstract required functions.
        public override int GetNumAStarNeighbours(Agent agent, AStarNodeAbstract aStarNode)
        {
            // Sanity check.
	        if(aStarNode == null )
	        {
		        return 0;
	        }

	        // A Neighbour is an Action that has an effect that has the potential
	        // to satisfy one of the unsatisfied properties of the goal.
	        // Neighbours are based only on the property key enum, not on the associated
	        // value.  For example, both DrawWeapon and HoslterWeapon are Neighbours
	        // of an AStarNode that has an unsatisfied property of kWSK_WeaponArmed.

	        m_cNeighbourActions = 0;
	        ActionAbstract action;

	        AStarNodePlanner node = (AStarNodePlanner)aStarNode;
	        WorldStateProp propCur;
	        WorldStateProp propGoal;
	        ENUM_WORLDSTATE_PROP_KEY eWSKey;

	        // Get the flags representing properties set in the current and goal world states.
	        BitArray flagsCur = node.wsWorldStateCurrent.GetWSPropSetFlags();
	        BitArray flagsGoal = node.wsWorldStateGoal.GetWSPropSetFlags();

            //ACTION_TYPE_LIST::iterator itAction; //TODO

	        // Iterate over all possible effect world state properties.
	        for( int iEffect=0; iEffect < (int)ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count; ++iEffect )
	        {
		        // Ignore the effect if it does not appear in current and goal states.
		        if( !( flagsCur.Get(iEffect) && flagsGoal.Get(iEffect) ) )
		        {
			        continue;
		        }

		        eWSKey = (ENUM_WORLDSTATE_PROP_KEY)iEffect;

		        // Determine if the effect matches any of the world state properties
		        // that are different in the current and goal world states.

		        propCur = node.wsWorldStateCurrent.GetWSProp(eWSKey);
		        propGoal = node.wsWorldStateGoal.GetWSProp(eWSKey);

		        if( propCur != null && propGoal != null)
		        {
                    //TODO: is fails here as both sides are null - infininte loop if comment out.
			      // if( propCur.pWSValue != propGoal.pWSValue )
                   if (propCur != propGoal)
			        {
				        // Add actions for this effect to the list of Neighbours.
                        foreach(ENUM_ACTION_TYPE eAction in m_listEffectActions[(int)eWSKey])
                        {
					        // Ignore actions if they are not in the AIs action set.
					        if (!m_Agent.GetActionStorage().IsActionInActionList(eAction))
					        {
						        continue;
					        }

					        action = m_Agent.GetActionStorage().GetAction(eAction);

					        // Ignore actions if their context preconditions are not met.
					        if( !( (action != null) && action.ValidateContextPreconditions( m_Agent, ref node.wsWorldStateGoal, true ) ) )
					        {
						     // Console.WriteLine("Action not Neighbour due to context preconditions: {0}" , ActionAbstract.s_aszActionTypes[(int)action.GetActionType()]);
                              continue;
					        }

					        m_aNeighbourActions[m_cNeighbourActions] = eAction;
					        ++m_cNeighbourActions;

					        if( m_cNeighbourActions >= (int)ENUM_ACTION_TYPE.kAct_Count )
					        {
						        break;
					        }
				        }
			        }
		        }

		        if( m_cNeighbourActions >= (int)ENUM_ACTION_TYPE.kAct_Count )
		        {
			        break;
		        }
	        }

            //TODO: Important!
	        // Sort Actions by precedence.
	        // This sorting will produce plans that have actions sorted by precedence,
	        //qsort desc:
	        if( m_cNeighbourActions > 1 )
	        {
                //temp sorting
                Array.Sort(m_aNeighbourActions);

               // qsort( (void*)m_aNeighbourActions, (size_t)m_cNeighbourActions, sizeof( ENUM_ACTION_TYPE ), CompareActions );
               // Utils.Quicksort(m_aNeighbourActions, 0, m_cNeighbourActions - 1);

                //TO DO: replacement qsort and such like.
    	     }

	        return m_cNeighbourActions;


        }

	    public override ENUM_ASTAR_NODE_ID	GetAStarNeighbour( Agent agent, AStarNodeAbstract aStarNode, int iNeighbour, AStarStorageAbstract aStarStorage )
        {
            return ConvertID_AIAction2AStarNode( m_aNeighbourActions[iNeighbour] );
        }

        public override void SetAStarFlags(ENUM_ASTAR_NODE_ID eAStarNode, ulong dwFlags, ulong dwMask) 
        {
            // AIAStarMapPlanner does not keep track of flags because a nodeID may 
        	// be reused many times, and does not refer to the same node.
	        // This is due to the fact that an Action may be used multiple times 
	        // in the same plan for different reasons (e.g. GotoNode to different nodes).
	        // So, reusing the same nodeID does not indicate a cycle, as it would 
	        // in physical pathfinding, where a nodeID refers to a point in space.
        }

        // See above
        public override ulong GetAStarFlags(ENUM_ASTAR_NODE_ID eAStarNode)
        {
            return (ulong)ENUM_ASTAR_FLAGS.kASTAR_Unchecked;
        }
	
	    // ID conversion.
	    public ENUM_ACTION_TYPE		ConvertID_AStarNode2Action( ENUM_ASTAR_NODE_ID eAStarNode ) { return (ENUM_ACTION_TYPE)eAStarNode; }
	    public ENUM_ASTAR_NODE_ID	ConvertID_AIAction2AStarNode( ENUM_ACTION_TYPE eAction ) { return (ENUM_ASTAR_NODE_ID)eAction; }

	    // Action access.
	    public ActionAbstract   GetAction( ENUM_ASTAR_NODE_ID eAStarNode ) 
        {
            return  m_Agent.GetActionStorage().GetAction(ConvertID_AStarNode2Action(eAStarNode)); 
        }

        //need a qsort method...
        //int CompareActions(const void *arg1, const void *arg2 );

	    protected Agent				        m_Agent;
	    protected List<ENUM_ACTION_TYPE>[]  m_listEffectActions; //ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count
	    protected ENUM_ACTION_TYPE[]	    m_aNeighbourActions; //(int)ENUM_ACTION_TYPE.kAct_Count
	    protected int					    m_cNeighbourActions;
	    protected bool					    m_bEffectTableBuilt;

    }

}