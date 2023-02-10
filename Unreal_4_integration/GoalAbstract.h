//------------------------------------------------------------------------
//
//  Name:   GoalAbstract.h
//
//  Desc:  abstract goal class definiton
//
//  Author: Brett Keir Jones
//
//------------------------------------------------------------------------

#pragma once

#include "WorldState.h"

//move to enum header file?
enum ENUM_AI_CONTEXT
{
	kContext_Invalid = -1,
	kContext_Test,
};


enum ENUM_GOAL_TYPE
{
	kGoal_InvalidType = -1,
	kGoal_Test,
	kGoal_Follow,

	// count must always be last
	kGoal_Count,
};

//const strings for goal types.
static const char* s_aszGoalTypes[] =
{
	"kGoal_Test",
	"kGoal_Follow",
};


//forward declerations
class AGOAPController;
class Plan;
class ActionAbstract;

class GoalAbstract
{
public:

	GoalAbstract();
	virtual ~GoalAbstract();

	//for debugging
	static const char * const GetGoalTypeName(ENUM_GOAL_TYPE eGoalType);
	static	ENUM_GOAL_TYPE	GetGoalType(const char* const pszGoalTypeName);

	virtual void	InitGoal(AGOAPController* pAgent, ENUM_GOAL_TYPE eGoalType);
	virtual void	TerminateGoal();

	virtual void	CalculateGoalRelevance() = 0; //pure virtual func
	float			GetGoalRelevance() const { return m_fGoalRelevance; }
	void			ClearGoalRelevance() { m_fGoalRelevance = 0.0f; }
	virtual void	HandleBuildPlanFailure()	{ ClearGoalRelevance(); }
	bool			GetReEvalOnSatisfied() const { return m_bReEvalOnSatisfied; }

	virtual float	GetActivationChance() const { return m_fActivateChance; }
	virtual bool	GetCanReactivateDuringTransitions() const { return m_bCanReactivateDuringTransitions; }
	virtual float	GetInterruptPriority() const { return m_fInterruptPriority; }
	float			GetFrequency() const { return m_fFrequency; }
	float           GetRecalcRate() const { return m_fRecalcRate; }

	virtual double	GetNextRecalcTime() { return m_fNextRecalcTime; }
	virtual void	SetNextRecalcTime();

	virtual void	ActivateGoal();
	virtual void	DeactivateGoal();
	virtual bool	UpdateGoal();

	virtual void	SetWSSatisfaction(WorldState& wsWorldState) {}
	virtual bool	IsWSSatisfied(WorldState* pwsWorldState) { return false; }

	virtual void	UpdateTaskStatus() {}

	virtual bool	ReplanRequired() { return false; }
	bool			BuildPlan();
	void			SetPlan(Plan* pPlan);
	void			ActivatePlan();
	virtual bool	IsPlanValid();
	void			ClearPlan();
	bool			IsPlanInterruptible();
	ActionAbstract*	GetCurrentAction();

	//returns the ENUM_AI_CONTEXT associated with this goal
	//this is guaranteed to be a valid contect or kContext_Invalid
	//it is only valid to call this function on active goals
	ENUM_AI_CONTEXT	GetContext() const;

	bool			IsPermanentGoal() { return false; }
	bool			IsAwarenessValid();

	ENUM_GOAL_TYPE	GetGoalType() const { return m_eGoalType; }


protected:

	AGOAPController*	m_pAgent; //backpointer to ai
	ENUM_GOAL_TYPE		m_eGoalType;
	float				m_fGoalRelevance;
	double				m_fNextRecalcTime;
	double				m_fActivationTime;


	ENUM_AI_CONTEXT		m_eAIContext;
	float				m_fRecalcRate;
	float				m_fActivateChance;
	float				m_fInterruptPriority;
	float				m_fFrequency;

	bool				m_bCanReactivateDuringTransitions; //dunno what this means
	bool				m_bReEvalOnSatisfied;

private:

	//This function is private as it should not be used by derived classes
	//to get thecontext were use GetGontext() which does some additional validation/reporting
	virtual ENUM_AI_CONTEXT	OnGetContext() const;

};