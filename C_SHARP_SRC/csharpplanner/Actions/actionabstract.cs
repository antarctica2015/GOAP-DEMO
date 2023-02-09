
//------------------------------------------------------------------------
//
//  Name:   actionbstract.cs
//
//  Desc:   action abstract class definition
//			base class with most of basic functionality
//
//			The AI uses sequences of Actions to satisfy Goals.
//			The GOAPPlanner finds the Action sequences.
//
//  Author: Brett Keir Jones 2015
//
//------------------------------------------------------------------------

using System;

using NSAgent;
using NSWorldState;

namespace NSActionAbstract
{
    //more to add here
    public enum ENUM_ACTION_TYPE
    {
	    kAct_InvalidType = -1,
	    kAct_Idle,
	    kAct_Reload,

	    //count always last
	    kAct_Count,
    };

    public abstract class ActionAbstract
    {
        //Action Types as const strings
        public string[] s_aszActionTypes = 
        {	
	        "kAct_Idle",
	        "kAct_Reload",
        };
       
	    public virtual ENUM_ACTION_TYPE GetActionClassType() { return (ENUM_ACTION_TYPE)(-1) ; }

		    public ActionAbstract()
            {
                m_wsWorldStatePreconditions = new WorldState();
                m_wsWorldStateEffects = new WorldState();

                m_eActionType = ENUM_ACTION_TYPE.kAct_InvalidType;
	            m_fActionCost = float.MaxValue;
	            m_fActionPrecedence = 0.0f;
	            m_bActionIsInterruptible = false;
	            m_fActionProbability = 0.0f;
            }

            ~ActionAbstract()
            {
                m_wsWorldStatePreconditions.ResetWS();
	            m_wsWorldStateEffects.ResetWS();
            }

		    //Initialisation
            public virtual void InitAction() { }

		    //Planning actions

            // Returns true if this action can potentially solve and of the unsatisfied properties of the plans goal state.
            // This is used by the planner to find actions that are neighbours of the current world state
		    public virtual bool	CanSolvePlanGoalWS(Agent agent, ref WorldState wsCurrentState, ref WorldState wsGoalState)
            {
                WorldStateProp propEffect;
                WorldStateProp propCurrent;
                WorldStateProp propGoal;

	            //iterate over all effects
	            for(uint iEffect = 0; iEffect < (int)ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count; ++iEffect)
	            {
		            //get a pointer to the indexed effect
		            propEffect = m_wsWorldStateEffects.GetWSProp(iEffect);

		            if(propEffect == null)
		            {
			            continue;
		            }

		            //determine if the effect matches any of the world state properties
		            //that are different in the current and goal states.

		            propCurrent = wsCurrentState.GetWSProp(propEffect.eWSKey);
		            propGoal    = wsGoalState.GetWSProp(propEffect.eWSKey);

		            if (propCurrent != null && propGoal != null)
		            {
			            if( ( propCurrent.pWSValue != propGoal.pWSValue ) ||
				            ( propCurrent.eWSType != propGoal.eWSType) )
			            {
				            return true;
			            }
		            }
	            }

	            //this action cannot satisfy any of the
	            //unmatched/unsatisfied worldstate properties
	            return false;

            }

            //
		    public void	SolvePlanWSVariable(Agent agent, ref WorldState wsCurrentState, ref WorldState wsGoalState)
            {
                WorldStateProp propEffect;
                WorldStateProp prop;

	            //iterate over all effects
	            for(uint iEffect = 0; iEffect < (int)ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count; ++iEffect )
	            {
		            //get a pointer to the indexed effect
		            propEffect = m_wsWorldStateEffects.GetWSProp(iEffect);

		            if(propEffect == null)
		            {
			            continue;
		            }

		            //the effect is not a variable, get the value of the propety effected
		            if(propEffect.eWSType != ENUM_WORLDSTATE_PROP_TYPE.kWSType_Variable)
		            {
			            prop = wsGoalState.GetWSProp(propEffect.eWSKey);
		            }
		            else
		            {
                        // the effect is a variable, get the value of the property referenced by the variable effected
			            prop = wsGoalState.GetWSProp(propEffect.eVariableWSKey);
		            }

		            // set the current world states property to match the goal.
		            if(prop != null)
		            {
			            // note that we might set the prop to a defferent key, as specified by a variable
			            if(prop.eWSType != ENUM_WORLDSTATE_PROP_TYPE.kWSType_Variable)
			            {
                            Console.WriteLine("ActionAbstract: Setting a world property to another variable");
			            }

			            wsCurrentState.SetWSProp(propEffect.eWSKey, prop);
		            }
	            }
            
            }
		   
            //
            public virtual void	SetPlanWSPreconditions(Agent agent, ref WorldState wsGoalState)
            {
                WorldStateProp precondition;
                WorldStateProp prop;

	            //Iterate over all preconditions.
	            for(uint iPrecondition = 0; iPrecondition < (int)ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count; ++iPrecondition)
	            {
		            //get a pointer to the indexed precondition
		            precondition = m_wsWorldStatePreconditions.GetWSProp(iPrecondition);

		            if(precondition == null)
		            {
			            continue;
		            }

		            // The precondition IS NOT a variable, set its values in the goal state
		            if( precondition.eWSType != ENUM_WORLDSTATE_PROP_TYPE.kWSType_Variable)
		            {
			            wsGoalState.SetWSProp(precondition.eWSKey, ENUM_WORLDSTATE_PROP_TYPE.kWSType_int, precondition.iWSValue) ;
		            }
		            else
		            {
                        //the precondition IS a variable,
		                //get a pointer to the world state property that the varible references
		                //and set the value in the goal state with that referenced value.
			            prop = wsGoalState.GetWSProp(precondition.eWSKey);

			            if(prop != null)
			            {
				            wsGoalState.SetWSProp(precondition.eWSKey, ENUM_WORLDSTATE_PROP_TYPE.kWSType_int, prop.iWSValue );
			            }

		            }
	            }
            }

            // Applys the effects of the action to the current world state
		    public virtual void	ApplyWSEffect(Agent agent, ref WorldState pwsCurrentState, ref WorldState pwsGoalState)
            {
                if( !(pwsCurrentState != null && pwsGoalState != null) ) 
	            {
		            return;
	            }

                WorldStateProp propEffect;
                WorldStateProp prop;

	            //iterate over all effects
	            for(uint iEffect = 0; iEffect < (int)ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count; ++iEffect )
	            {
		            //get a pointer to the indexed effect
		            propEffect = m_wsWorldStateEffects.GetWSProp(iEffect);

		            if(propEffect == null)
		            {
			            continue;
		            }

		            //the effect is not a variable,
		            //set the effect on the current world state
		            if(propEffect.eWSType != ENUM_WORLDSTATE_PROP_TYPE.kWSType_Variable )
		            {
			            pwsCurrentState.SetWSProp(propEffect.eWSKey, propEffect.eWSType, propEffect.iWSValue);
		            }
		            else
		            {
                        // The effect is a variable,
		                // lookup the value of the effects variable, and set the effect on the current world state
			            prop = pwsGoalState.GetWSProp(propEffect.eVariableWSKey);

			            if(prop != null)
			            {
				            pwsCurrentState.SetWSProp(propEffect.eWSKey, prop.eWSType, prop.iWSValue );
			            }
		            }
	            }

            }

		    public ENUM_ACTION_TYPE	GetActionType() { return m_eActionType; }
		    public float			GetActionCost() { return m_fActionCost; }
		    public float			GetActionPrecedence() { return m_fActionPrecedence; }
		    public virtual float	GetActionProbability(Agent agent) { return m_fActionProbability; }
		    public virtual void	    FailActionProbability(Agent agent) {}

		    //Validation
		    public virtual bool	ValidateWSEffects(Agent agent, ref WorldState wsCurrentState, ref WorldState wsGoalState)
            {
                WorldStateProp propEffect;
                WorldStateProp propCurrent;
                WorldStateProp propGoal;

	            //iterate over all effects
	            for(uint iEffect = 0; iEffect < (int)ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count; ++iEffect )
	            {
		            //get a pointer to the indexed
		            propEffect = m_wsWorldStateEffects.GetWSProp(iEffect);

		            if(propEffect == null)
		            {
			            continue;
		            }

		            //the effect IS NOT a variable.
		            //determine if the current world states value matches the effects value
		            if( propEffect.eWSType != ENUM_WORLDSTATE_PROP_TYPE.kWSType_Variable )
		            {
			            propCurrent = wsCurrentState.GetWSProp(propEffect.eWSKey);

			            //bit unweildy this - basically all need to be true
			            if(  (propCurrent != null ) && ( ( propCurrent.iWSValue == propEffect.iWSValue )
							              && ( propCurrent.eWSType == propEffect.eWSType )
							              && ( propCurrent.eWSType != ENUM_WORLDSTATE_PROP_TYPE.kWSType_Unset )
							              && ( propCurrent.eWSType != ENUM_WORLDSTATE_PROP_TYPE.kWSType_InvalidType ) ) )
			            {
				            continue;
			            }

		            }
		            else
		            {
                        //the effect IS a variable
		                //determine if the current world states value matches
		                //the referenced value in the goal state
			            propCurrent = wsCurrentState.GetWSProp(propEffect.eWSKey);
			            propGoal    = wsGoalState.GetWSProp(propEffect.eVariableWSKey);


			            if(propCurrent != null && propGoal != null)
			            {
				            //again nasty && block
				            if( ( propGoal.iWSValue == propCurrent.iWSValue ) &&
					            ( propGoal.eWSType  == propCurrent.eWSType ) &&
					            ( propGoal.eWSType  != ENUM_WORLDSTATE_PROP_TYPE.kWSType_Unset) &&
					            ( propGoal.eWSType  != ENUM_WORLDSTATE_PROP_TYPE.kWSType_InvalidType) &&
					            ( propCurrent.eWSType != ENUM_WORLDSTATE_PROP_TYPE.kWSType_Unset) &&
					            ( propCurrent.eWSType != ENUM_WORLDSTATE_PROP_TYPE.kWSType_InvalidType) )
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
		   
            public virtual bool	ValidateWSPreconditions(Agent agent, ref WorldState wsCurrentState, ref WorldState wsGoalState, ref ENUM_WORLDSTATE_PROP_KEY failedWSKey)
            {
                WorldStateProp precondition;
                WorldStateProp propCurrent;
                WorldStateProp propGoal;

	            //iterate over all preconditions
	            for(uint iPrecondition = 0; iPrecondition < (int)ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count; ++iPrecondition )
	            {
		            //Get a pointer to the indexed precondition
		            precondition = m_wsWorldStatePreconditions.GetWSProp(iPrecondition);

		            if(precondition == null)
		            {
			            continue;
		            }

		            //The precondition IS NOT a variable
		            //determine if the current world state value matches the preconditions value
		            if( precondition.eWSType != ENUM_WORLDSTATE_PROP_TYPE.kWSType_Variable )
		            {
			            propCurrent = wsCurrentState.GetWSProp(precondition.eWSKey);

			            if( (propCurrent != null) && 
				            ( ( propCurrent.iWSValue != precondition.iWSValue ) ||
				              ( propCurrent.eWSType != precondition.eWSType ) ) )
			            {

				           // if(failedWSKey != null)
				           // {
					            failedWSKey = precondition.eWSKey;
				          //  }

				            return false;
			            }
		            }

		            //the precondition IS a variable
		            //determine if the current world states value matches
		            //the referenced value in the goal state
		            else
		            {
			            propCurrent = wsCurrentState.GetWSProp( precondition.eWSKey );
			            propGoal    = wsGoalState.GetWSProp( precondition.eVariableWSKey );

			            if (propCurrent != null && propGoal != null)
			            {
				            if( ( propCurrent.iWSValue != propGoal.iWSValue ) ||
					            ( propCurrent.eWSType  != propGoal.eWSType))
				            {
				
					           // if(failedWSKey != null)
					            //{
						            failedWSKey = precondition.eWSKey;
					           // }

					            return false;
				            }
			            }
		            }
	            }

	            //precondtions are valid
	            return true;
            }
		   

        public virtual bool	ValidateContextPreconditions(Agent agent, ref WorldState wsGoalState, bool bIsPlanning) { return true ; }
		    
        // returns true if this action is valid to continue running
        public virtual bool	ValidateAction(Agent agent)
        {
            if(agent == null)
	        {
		            return false;
	        }

	        //has something went gone wrong with the state?
	        //if( ( pAgent->GetState() && ( pAgentGetState()->GetStateStatus() == kStateStatus_Failed) ) )
	        //{
	        //	return false;
	        //}

	        //otherwise continue running
	        return true;
        }

		   //Activation and completion
           public virtual void ActivateAction(Agent agent, ref WorldState wsGoalState) {}
		   public virtual void		DeactivateAction(Agent agent) {}
		   public virtual bool		IsActionComplete(Agent agent) { return false; }
		   public virtual void		ApplyContextEffect(Agent agent, WorldState wsCurrentState, ref WorldState wsGoalState) {}
		   public virtual bool		IsActionInterruptible(Agent agent) {return m_bActionIsInterruptible; }

		   //Data access
		   public  WorldState		GetActionEffects() { return m_wsWorldStateEffects; }
		   public  WorldState	    GetActionPrecondition() { return m_wsWorldStatePreconditions; }	

		   protected WorldState		m_wsWorldStatePreconditions;
		   protected WorldState		m_wsWorldStateEffects;

		   protected ENUM_ACTION_TYPE   m_eActionType;
		   protected float				m_fActionCost;
		   protected float				m_fActionPrecedence;
		   protected bool				m_bActionIsInterruptible;
		   protected float				m_fActionProbability;

    }
}