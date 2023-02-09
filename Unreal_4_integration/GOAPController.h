//------------------------------------------------------------------------
//
//  Name:   GOAPController.h
//
//  Desc:   agent brain class
//
//  Author: Brett Keir Jones 
//
//------------------------------------------------------------------------

#pragma once

#include "AIController.h"
#include "GOAPController.generated.h"

//forward declarations.
class AIPlanner;
class WorldState;
class Plan;
class BlackBoard;
class ActionStorage;
class GoalStorage;

/**
 * 
 */
UCLASS()
class GOAPTEST_API AGOAPController : public AAIController
{
	GENERATED_BODY()

public:

	AGOAPController(const FObjectInitializer& ObjectInitializer);

	void Init();
	void CleanUp();

	//Accessors
	AIPlanner*  GetGoalPlanner()  const { return m_pGoalPlanner; }

	WorldState* GetWorldState()   const { return m_pWorldState; }
	void		SetWorldState(WorldState* pWorldState) { m_pWorldState = pWorldState; }

	Plan* GetPlan()  const { return m_pPlan; }
	void SetPlan(Plan* pPlan) { m_pPlan = pPlan; }

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
