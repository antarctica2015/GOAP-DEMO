
//------------------------------------------------------------------------
//
//  Name:   ActionAbstract.h
//
//  Desc:   action abstract class definition
//			base class with most of basic functionality
//
//			The AI uses sequences of Actions to satisfy Goals.
//			The GOAPPlanner finds the Action sequences.
//
//  Author: Brett Keir Jones
//
//------------------------------------------------------------------------

#pragma once

#include "WorldState.h"

//list of actions
enum ENUM_ACTION_TYPE
{
	kAct_InvalidType = -1,
	kAct_Test,
	kAct_Idle,
	kAct_Follow,

	//count always last
	kAct_Count,
};

//Action Types as const strings
static const char* s_aszActionTypes[] =
{
	"kAct_Test",
	"kAct_Idle",
	"kAct_Follow",
};

//forward declarations
class AGOAPController;
class WorldState;

class ActionAbstract
{
public:

	virtual ENUM_ACTION_TYPE GetActionClassType() const { return (ENUM_ACTION_TYPE)-1; }

public:

	ActionAbstract();
	virtual ~ActionAbstract();

	//Initialisation
	virtual void InitAction();

	//Planning actions
	virtual bool		CanSolvePlanGoalWS(AGOAPController* pAgent, WorldState& wsCurrentState, WorldState& wsGoalState);
	void				SolvePlanWSVariable(AGOAPController* pAgent, WorldState& wsCurrentState, WorldState& wsGoalState);
	virtual void		SetPlanWSPreconditions(AGOAPController* pAgent, WorldState& wsGoalState);
	virtual void		ApplyWSEffect(AGOAPController* pAgent, WorldState* pwsCurrentState, WorldState* pwsGoalState);

	ENUM_ACTION_TYPE	GetActionType() { return m_eActionType; }
	float				GetActionCost() { return m_fActionCost; }
	float				GetActionPrecedence() { return m_fActionPrecedence; }
	virtual float		GetActionProbability(AGOAPController* pAgent) { return m_fActionProbability; }
	virtual void		FailActionProbability(AGOAPController* pAgent) {}


	//Validation
	virtual bool		ValidateWSEffects(AGOAPController* pAgent, WorldState& wsCurrentState, WorldState& wsGoalState);
	virtual bool		ValidateWSPreconditions(AGOAPController* pAgent, WorldState& wsCurrentState, WorldState& wsGoalState, ENUM_WORLDSTATE_PROP_KEY* pFailedWSKey);
	virtual bool		ValidateContextPreconditions(AGOAPController* pAgent, WorldState& wsGoalState, bool bIsPlanning) { return true; }
	virtual bool		ValidateAction(AGOAPController* pAgent);

	//Activation and completion
	virtual void		ActivateAction(AGOAPController* pAgent, WorldState& wsGoalState);
	virtual void		DeactivateAction(AGOAPController* pAgent) {}
	virtual bool		IsActionComplete(AGOAPController* pAgent) { return false; }
	virtual void		ApplyContextEffect(AGOAPController* pAgent, WorldState* wsCurrentState, WorldState* wsGoalState) {}
	virtual bool		IsActionInterruptible(AGOAPController* pAgent) { return m_bActionIsInterruptible; }

	//Data access
	WorldState*			GetActionEffects() { return &m_wsWorldStateEffects; }
	WorldState*			GetActionPrecondition() { return &m_wsWorldStatePreconditions; }

protected:

	WorldState				m_wsWorldStatePreconditions;
	WorldState				m_wsWorldStateEffects;

	ENUM_ACTION_TYPE		m_eActionType;
	float					m_fActionCost;
	float					m_fActionPrecedence;
	bool					m_bActionIsInterruptible;
	float					m_fActionProbability;



};