
//------------------------------------------------------------------------
//
//  Name:  AIPlanner.h
//
//  Desc:  Individual Goal Planner implementation.
//
//  Author: Brett Keir Jones
//
//------------------------------------------------------------------------

#pragma once

#include "ActionAbstract.h"
#include "AStarMachine.h"
#include "AStarPlanner.h"
#include "WorldState.h"

//Forward Declarations
class AGOAPController;
class AIPlanner;
class GoalAbstract;


// a plan consits of a number of steps, each step contains an Action and a representation of the world state it expects to achieve
class PlanStep
{
public:

	PlanStep()
	{
		eActionType = kAct_InvalidType;
		wsWorldState.ResetWS();

	}

	~PlanStep() { ; }

	ENUM_ACTION_TYPE	eActionType;
	WorldState			wsWorldState;

};

typedef TArray<PlanStep*> PLAN_STEP_LIST;

class Plan
{
	//declares all member functions of AIPlanner as friends,
	//a friend function has the right to access all (inc private) members,
	//while can enhance performance, personally i dislike using them,
	//but im sticking as close to fear as possible until its running,
	//so will just have to put up with it for now.
	friend class AIPlanner;

public:

	Plan(AGOAPController* pAgent);
	~Plan();

	void	ActivatePlan(AGOAPController* pAgent);
	void	DeactivatePlan();

	bool	IsPlanStepComplete();
	bool	IsPlanStepInterruptible();

	bool	AdvancePlan();
	bool	IsPlanValid();

	double	GetPlanActivationTime() const { return m_fPlanActivationTime; }

	ActionAbstract*	GetCurrentPlanStepAction();

protected:

	AGOAPController*	m_pAgent;
	PLAN_STEP_LIST		m_listPlanSteps;
	int32				m_iPlanStep;
	double				m_fPlanActivationTime;


};


//(GOAP) AIPlanner implementation. 
class AIPlanner
{
public:

	AIPlanner(AGOAPController* pAgent);
	~AIPlanner();

	void	InitAIPlanner();

	bool	BuildPlan(AGOAPController* pAgent, GoalAbstract* pGoal);
	bool	BuildPlan(GoalAbstract* pGoal);

	void	MergeWorldStates(AGOAPController* pAgent, WorldState& wsCurrentState, WorldState& wsGoalState);

protected:

	void  EvaluateWorldStateProp(AGOAPController* pAgent, STRUCT_WORLDSTATE_PROP& prop);


protected:

	AGOAPController*	m_pAgent;

	AStarMachine		m_AStarMachine;
	AStarMapPlanner		m_AStarMapPlanner;
	AStarStoragePlanner m_AStarStoragePlanner;
	AStarGoalPlanner	m_AStarGoalPlanner;

};

