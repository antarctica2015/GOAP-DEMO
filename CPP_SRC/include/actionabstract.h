#ifndef ACTIONABSTRACT_H
#define ACTIONABSTRACT_H

//------------------------------------------------------------------------
//
//  Name:   actionabstract.h
//
//  Desc:   action abstract class definition
//			base class with most of basic functionality
//
//			The AI uses sequences of Actions to satisfy Goals.
//			The GOAPPlanner finds the Action sequences.
//
//  Author: Brett Keir Jones 2008
//
//------------------------------------------------------------------------

#include "worldstate.h"

//more to add here
enum ENUM_ACTION_TYPE
{
	kAct_InvalidType = -1,
	//kAct_GotoNode,
	kAct_Idle,
	kAct_Reload,

	//count always last
	kAct_Count,
};

//Action Types as const strings
//why works now when didnt before?
static const char* s_aszActionTypes[] = 
{	
	//"kAct_GotoNode",
	"kAct_Idle",
	"kAct_Reload",
};

//forward declarations
class Agent;
class WorldState;

class ActionAbstract
{
public:
	
	virtual ENUM_ACTION_TYPE GetActionClassType() const { return (ENUM_ACTION_TYPE)-1 ; }

	public:

		ActionAbstract();
		virtual ~ActionAbstract();

		//Initialisation

		virtual void InitAction();

		//Planning actions

		virtual bool		CanSolvePlanGoalWS(Agent* pAgent, WorldState& wsCurrentState, WorldState& wsGoalState);
		void				SolvePlanWSVariable(Agent* pAgent, WorldState& wsCurrentState, WorldState& wsGoalState);
		virtual void		SetPlanWSPreconditions(Agent* pAgent, WorldState& wsGoalState);
		virtual void		ApplyWSEffect(Agent* pAgent, WorldState* pwsCurrentState, WorldState* pwsGoalState);

		ENUM_ACTION_TYPE	GetActionType() { return m_eActionType; }
		float				GetActionCost() { return m_fActionCost; }
		float				GetActionPrecedence() { return m_fActionPrecedence; }
		virtual float		GetActionProbability(Agent* pAgent) { return m_fActionProbability;}
		virtual void		FailActionProbability(Agent* pAgent) {}


		//Validation

		virtual bool		ValidateWSEffects(Agent* pAgent, WorldState& wsCurrentState, WorldState& wsGoalState);
		virtual bool		ValidateWSPreconditions(Agent* pAgent, WorldState& wsCurrentState, WorldState& wsGoalState, ENUM_WORLDSTATE_PROP_KEY* pFailedWSKey);
		virtual bool		ValidateContextPreconditions(Agent* pAgent, WorldState& wsGoalState, bool bIsPlanning) { return true ; }
		virtual bool		ValidateAction(Agent* pAgent);

		//Activation and completion

		virtual void		ActivateAction(Agent* pAgent, WorldState& wsGoalState);
		virtual void		DeactivateAction(Agent* pAgent) {}
		virtual bool		IsActionComplete(Agent* pAgent) { return false; }
		virtual void		ApplyContextEffect(Agent* pAgent, WorldState* wsCurrentState, WorldState* wsGoalState) {}
		virtual bool		IsActionInterruptible(Agent* pAgent) {return m_bActionIsInterruptible; }

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


#endif