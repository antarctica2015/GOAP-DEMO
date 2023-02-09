#ifndef AIPLANNER_H
#define AIPLANNER_H

//------------------------------------------------------------------------
//
//  Name:  aiplanner.h
//
//  Desc:  Individual Goal Planner implementation.
//
//  Author: Brett Keir Jones 2008
//
//------------------------------------------------------------------------

#include "actionabstract.h"
#include "astarmachine.h"
#include "astarplanner.h"
#include "worldstate.h"

//Forward Declarations

class Agent;
class AIPlanner;
class GoalAbstract;


// a plan consits of a number of steps; 
// each step contains an Action and a representation of the world state it expects to achieve

class PlanStep
{
public:

	//ctor and dtor
	PlanStep()
	{
		eActionType = kAct_InvalidType;
		wsWorldState.ResetWS();
	
	}

	~PlanStep() {;}

	//Some Debug print func here?

	//

	ENUM_ACTION_TYPE	eActionType;
	WorldState			wsWorldState;

};

typedef std::vector<PlanStep*> PLAN_STEP_LIST; 


class Plan
{
	//declares all member functions of AIPlanner as friends,
	//a friend function has the right to access all (inc private) members,
	//while can enhance performance, personally i dislike using them,
	//but im sticking as close to fear as possible until its running,
	//so will just have to put up with it for now.
	friend class AIPlanner;

	public:

		Plan(Agent* pAgent);
		~Plan();

		void	ActivatePlan(Agent* pAgent);
		void	DeactivatePlan();

		bool	IsPlanStepComplete();
		bool	IsPlanStepInterruptible();

		bool	AdvancePlan();
		bool	IsPlanValid();

		double	GetPlanActivationTime() const { return m_fPlanActivationTime; }

		ActionAbstract*	GetCurrentPlanStepAction();

protected:

	Agent*			m_pAgent;
	PLAN_STEP_LIST	m_listPlanSteps;
	unsigned int	m_iPlanStep;
	double			m_fPlanActivationTime;


};


//(GOAP) AIPlanner implementation. 

class AIPlanner
{
public:

		AIPlanner(Agent* pAgent);
		~AIPlanner();

	  void	InitAIPlanner();

	  bool	BuildPlan(Agent* pAgent, GoalAbstract* pGoal);
	   bool	BuildPlan(GoalAbstract* pGoal);

	  void	MergeWorldStates(Agent* pAgent, WorldState& wsCurrentState, WorldState& wsGoalState);

protected:
		
	  void  EvaluateWorldStateProp(Agent* pAgent, STRUCT_WORLDSTATE_PROP& prop);
		

protected:

	Agent*				m_pAgent;

	AStarMachine		m_AStarMachine;
	AStarMapPlanner		m_AStarMapPlanner;
	AStarStoragePlanner m_AStarStoragePlanner;
	AStarGoalPlanner	m_AStarGoalPlanner;

};

#endif