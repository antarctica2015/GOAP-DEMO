#ifndef AGENT_H
#define AGENT_H

//------------------------------------------------------------------------
//
//  Name:   agent.h
//
//  Desc:   
//
//  Author: Brett Keir Jones 2008
//
//------------------------------------------------------------------------

//includes.

//forward declarations.

class AIPlanner;
class WorldState;
class Plan;
class BlackBoard;
class ActionStorage;
class GoalStorage;



//Agent class
class Agent
{
	
public:

	
	Agent();
	~Agent();

	//Methods

	void Init();

	//Accessors

	AIPlanner*  GetGoalPlanner()  const { return m_pGoalPlanner; }
	
	WorldState* GetWorldState()   const { return m_pWorldState; }
	void		SetWorldState(WorldState* pWorldState) { m_pWorldState = pWorldState; }
	
	Plan* GetPlan()  const { return m_pPlan; }
	void SetPlan(Plan* pPlan) { m_pPlan = pPlan; }


	//ActionStorage* GetActionStorage()  const { return m_pActionStorage; }
	ActionStorage*  GetActionStorage()  { return m_pActionStorage; }

	GoalStorage* GetGoalStorage()   { return m_pGoalStorage; }

	BlackBoard*	GetBlackBoard()   { return m_pBlackBoard; } 

//private:
	
	AIPlanner*		m_pGoalPlanner;
	WorldState*		m_pWorldState;
	Plan*			m_pPlan;
	BlackBoard*     m_pBlackBoard;

	//ActionSet
	ActionStorage* m_pActionStorage; 
	
	//GoalSet
	GoalStorage*	m_pGoalStorage;
};

#endif