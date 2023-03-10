//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.18444
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------
using System;
using System.Collections.Generic;

using NSAgent;
using NSGoalAbstract;
using NSGoalIdle;
using NSBlackBoard;
using NSWorldState;
using NSUtils;
using NSEnums;

namespace NSGoalStorage
{
	public enum ENUM_GOALSET_ID
	{
		kGSetID_Invalid = -1,
		kGSetID_SWAT,
		kGSetID_Suspect,
	};

	public class GoalStorage
	{
        //	
		public Agent 		        m_Agent;
		public List<GoalAbstract>	m_listGoals;
		public GoalAbstract 	    m_CurrentGoal;
		
		ENUM_GOALSET_ID	m_GoalSet;
		double 			m_GoalSetTime;


		public GoalStorage(Agent agent)
		{
			m_Agent = agent;
            m_listGoals = new List<GoalAbstract>();

			Init();
		}

        ~GoalStorage()
		{
			Terminate(true);
		}


	    // Init the goal manager
		public void Init()
		{
			m_CurrentGoal 	= null;
			m_GoalSet		= ENUM_GOALSET_ID.kGSetID_Invalid;
			m_GoalSetTime	= 0.0f;

			//should be adding more goals here??
			AddGoal(ENUM_GOAL_TYPE.kGoal_Idle);

		}

		public void Terminate(bool bDestroyAll)
		{
            // Delete all goals.
	        // Use RemoveGoal so that all goal removals follow the same code path.
	        // When SetGoalSet is called, it may call Terminate() to clear the goals. 
	        // Calling RemoveGoal ensures that there are no dangling pointers.

            foreach (GoalAbstract goal in m_listGoals)
            {
                if(!bDestroyAll && goal.IsPermanentGoal())
		        {
			       continue;
		        }
		        else 
                {
                   // Console.WriteLine("Removing Goal: {0}", GoalAbstract.s_GoalTypes[(int)goal.GetGoalType()]);
			       // RemoveGoal(goal.GetGoalType());
		        }
	        }

            // go back though whats left.
	        foreach (GoalAbstract goal in m_listGoals)
            {
                //Console.WriteLine("Goal is permanent- NOT removing: {0}", GoalAbstract.s_GoalTypes[(int)goal.GetGoalType()]);
	        }
		
		}

		public ENUM_GOALSET_ID	GetGoalSetID() { return m_GoalSet; }
		public double			GetGoalSetTime() { return m_GoalSetTime; }

		public GoalAbstract	CreateNewGoal(ENUM_GOAL_TYPE eGoalType)
		{
			switch(eGoalType)
			{
			case ENUM_GOAL_TYPE.kGoal_Idle: 
			{ 
				return new GoalIdle(); 
			}
				
			default:
                    Console.WriteLine("CreateNewGoal: Unrecognized goal type.");       
                    break;            
			}
			
			return null;

		}

		public GoalAbstract	AddGoal(ENUM_GOAL_TYPE eGoalType)
		{
		    GoalAbstract goal = null;

            List<GoalAbstract>.Enumerator itGoal = m_listGoals.GetEnumerator();

            //
           // while (e.MoveNext())
	       // {
	        // int value = e.Current;

	        if( eGoalType != ENUM_GOAL_TYPE.kGoal_InvalidType && eGoalType < ENUM_GOAL_TYPE.kGoal_Count)
	        {

                while (itGoal.MoveNext())
                {
                    // If AI has the goal, return it.
			        if(itGoal.Current.GetGoalType() == eGoalType)
			        {
				        break;
			        }
                
                }

		        if( itGoal.MoveNext() == false ) // cant move next = we are at the end
		        {
			        if(eGoalType != ENUM_GOAL_TYPE.kGoal_InvalidType)
			        {
				        // Add goal to list of goals.
				        goal = null;
				
				        goal = CreateNewGoal(eGoalType); 
                       // Console.WriteLine("Created new goal: {0}" , GoalAbstract.s_GoalTypes[(int)eGoalType]);
			
				        if( goal == null )
				        {
                            Console.WriteLine("GoalStorage:AddGoal: Failed to create goal.");
					        return null;
				        }

				        // Initialize goal.
				        goal.InitGoal(m_Agent, eGoalType);

				        //add to vector
				        m_listGoals.Add(goal);
			        }
			        else
			        {
                       // Console.WriteLine("GoalStorage:AddGoal: Unable to create goal.", GoalAbstract.s_GoalTypes[(int)eGoalType] );
			        }
		        }
	        }

	        return goal;

		}

        // Removes a goal from the goal list
		public void RemoveGoal(ENUM_GOAL_TYPE eGoalType)
		{
		    if(eGoalType == ENUM_GOAL_TYPE.kGoal_InvalidType)
	        {
		        //error message here!
	        }

	        // Find goal of specified type.
            foreach (GoalAbstract goal in m_listGoals)
            {
		        if(goal.GetGoalType() == eGoalType)
		        {
			        if(m_CurrentGoal == goal)
			        {
				        m_CurrentGoal =  null;
			        }

			        // Delete the goal.
			        goal.TerminateGoal();

			        if(goal != null)
			        {
				       // goal = null; 
			        }
			
                    // i guess?
			        m_listGoals.Remove(goal);

			        return;
		        }
	        }

		}

        public GoalAbstract FindGoalByType(ENUM_GOAL_TYPE eGoalType)
        {
            foreach (GoalAbstract goal in m_listGoals)
            {
                if (goal.GetGoalType() == eGoalType)
                {
                    return goal;
                }
            }

            return null;
        }

		public void SelectRelevantGoal()
		{
            // Need to determine if the goals should be updated, and if so, if replanning is required.
	        bool bUpdateGoals = false;
	        bool bForceReplanning = false;

	        if (m_CurrentGoal != null)
	        {
		        // Test for an invalid plan.
		        if (!m_CurrentGoal.IsPlanValid())
		        {
                   // Console.WriteLine("Plan is invalid for Goal: {0} relevance: {1}", GoalAbstract.s_GoalTypes[(int)m_CurrentGoal.GetGoalType()], m_CurrentGoal.GetGoalRelevance());
			        bUpdateGoals = true;
			        bForceReplanning = true;
		        }

		        // Test for a satisfied plan.
		        else if (m_CurrentGoal.IsWSSatisfied(m_Agent.GetWorldState()))
		        {
                    //Console.WriteLine("Goal is satisfied: {0}", GoalAbstract.s_GoalTypes[(int)m_CurrentGoal.GetGoalType()]);
			        bUpdateGoals = true;
			        bForceReplanning = true;
		        }
	        }

	        // Test for a request to reselect actions.  
	        if (m_Agent.GetBlackBoard().GetBBSelectAction())
	        {
		        bUpdateGoals = true;
	        }

	        // Plan has been invalidated, so immediately force a replan.
	        if (m_Agent.GetBlackBoard().GetBBInvalidatePlan())
	        {
                Console.WriteLine("Honouring forced replan request: ");
		        bUpdateGoals = true;
		        bForceReplanning = true;
	        }

	        // Perform the actual goal updating/replanning if requested
	        if (bUpdateGoals)
	        {
		        UpdateGoalRelevances(bForceReplanning);
	        }

	        // Clear the forced replanning flag.  
	        // Clear this AFTER updating goal relevances.
	        m_Agent.GetBlackBoard().SetBBInvalidatePlan(false);
		}

		public void Update()
		{
		    if(m_CurrentGoal != null)
	        {
		        if(!m_CurrentGoal.UpdateGoal())
		        {
			        m_Agent.GetBlackBoard().SetBBSelectAction(true);
			        m_CurrentGoal.ClearPlan();
		        }

		        if( m_Agent.GetBlackBoard().GetBBTaskStatus() == ENUM_WMTASK_STATUS.kTaskStatus_Set )
		        {
			        m_CurrentGoal.UpdateTaskStatus();
		        }
	        }
		}

		public bool IsCurrentGoal(GoalAbstract goal) { return (bool)(goal == m_CurrentGoal); }

		public void SetCurrentGoal(GoalAbstract goal) { m_CurrentGoal = goal; }

		//Debugging
		public GoalAbstract	GetCurrentGoal() { return m_CurrentGoal; }

		protected void UpdateGoalRelevances(bool bReplan)
		{
		    bool bRequiresHigherInterruptPriority = false;
	        bool bGoalChanged = false;

	        GoalAbstract goalMax = FindMostRelevantGoal(true);

	        // Set the new current goal, which is the
	        // goal with the highest relevance that can
	        // formulate a valid plan of actions.
	        while(goalMax != null)
	        {
		        // If the pGoalMax has the same relevance as the current goal, prefer the 
		        // current to avoid abandoning a goal prematurely when there isn't anything
		        // better to do.

		        if ((m_CurrentGoal != null && goalMax != null) && (m_CurrentGoal != goalMax)
			          && (m_CurrentGoal.GetGoalRelevance() == goalMax.GetGoalRelevance()))
		        {
			        goalMax = m_CurrentGoal;
		        }

		        // Goal has not changed.
		        // No planning necessary.
		        if( ( goalMax == m_CurrentGoal ) && ( !bReplan ) 
			          && ( !m_CurrentGoal.ReplanRequired() ) )
		        {
			        break;
		        }

		        // Goal must interrupt to be valid (ie damage response which must be played).
		        if ( ( ( bRequiresHigherInterruptPriority && (goalMax != null) && (m_CurrentGoal != null) )
			        && ( goalMax != m_CurrentGoal ) )
			        && ( goalMax.GetInterruptPriority() <= m_CurrentGoal.GetInterruptPriority() ) )
		        {
			        goalMax.ClearGoalRelevance();
		        }
		        else if((goalMax.GetActivationChance() < 1.0f ) && ( goalMax.GetActivationChance() < Utils.GetRandom(0.0f, 1.0f) ) )
		        {  // Failed to meet random probability of activation.
			        goalMax.ClearGoalRelevance();
		        }
		        else if(goalMax.BuildPlan())
		        {
                     // Activate the goal if we formulated a valid plan.

			        if(m_CurrentGoal != null)
			        {
				        m_CurrentGoal.DeactivateGoal();
                        //Console.WriteLine("Deactivating Goal: {0}", GoalAbstract.s_GoalTypes[(int)m_CurrentGoal.GetGoalType()] );

			        }

			        m_CurrentGoal = goalMax;

                    //Console.WriteLine("Activating Goal {0} , Relevance: {1}", GoalAbstract.s_GoalTypes[(int)m_CurrentGoal.GetGoalType()], m_CurrentGoal.GetGoalRelevance());

			        m_CurrentGoal.ActivateGoal();
			        m_CurrentGoal.ActivatePlan();
			        bGoalChanged = true;
			        break;
		        }
		        else 
		        {    // Could not build a valid plan.
			        goalMax.HandleBuildPlanFailure();
		        }

		        goalMax = FindMostRelevantGoal(false);
	        }

	        // No max goal was found, so the AI has no current goal or state.
	        if(bReplan && (goalMax != null )&& !bRequiresHigherInterruptPriority)
	        {
		        m_CurrentGoal = null;
	        }


	        // Clear the SelectAction flag from the blackboard.
	        // If the Goal required a interrupt, and the Goal did not change,
	        // then do not clear the SelectAction flag.
	        if(bRequiresHigherInterruptPriority && !bGoalChanged )
	        {
		        return;
	        }
	        else
	        {
		        m_Agent.GetBlackBoard().SetBBSelectAction(false);
	        }
		}


		public GoalAbstract FindMostRelevantGoal( bool bRecalculate )
		{
			float fRelevance;
	        float fMaxRelevance = 0.0f;

	        double fNextRecalcTime;

	        GoalAbstract goalMax = null;
	
            foreach (GoalAbstract goal in m_listGoals)
	        {

		        // Recalculate the current relevance.
		        if(bRecalculate)
		        {
			        // Do not re-evaluate this goal if it is not time yet.
			        fNextRecalcTime = goal.GetNextRecalcTime();
			
			        if( (goal != m_CurrentGoal) &&
				        (fNextRecalcTime > 0.0f) &&
                        (fNextRecalcTime > (DateTime.UtcNow.Millisecond/1000)))  
			        {
				        goal.ClearGoalRelevance();
			        }

			        // Do not re-evaluate this goal if it is currently satisfied.
			        else if((!goal.GetReEvalOnSatisfied() ) && goal.IsWSSatisfied( m_Agent.GetWorldState()))
			        {
				        goal.ClearGoalRelevance();
			        }
			        // Goal is unsatisfied, or should always be re-evaluated.
			        else
			        {
				        goal.CalculateGoalRelevance();

				        if(fNextRecalcTime > 0.0f)
				        {
					        goal.SetNextRecalcTime();
				        }
			        }
		        }

		        fRelevance = goal.GetGoalRelevance();

		        // Select a new max.
		        if(fRelevance > fMaxRelevance)
		        {
			        fMaxRelevance = fRelevance;
			        goalMax = goal;
		        }
	        }

	        return goalMax;


		}

	}



}

