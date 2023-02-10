
//------------------------------------------------------------------------
//
//  Name:   GoalTest.h
//
//  Desc:  
//
//  Author: Brett Keir Jones
//
//------------------------------------------------------------------------

#include "GoalAbstract.h"

class GoalTest : public GoalAbstract
{
	typedef GoalAbstract super;

public:
	virtual ENUM_GOAL_TYPE GetGoalClassType() const { return kGoal_Test; }

	GoalTest();

	virtual void	InitGoal(AGOAPController* pAgent, ENUM_GOAL_TYPE eGoalType);

	virtual void	CalculateGoalRelevance();

	virtual void	SetWSSatisfaction(WorldState& WorldState);
	virtual bool	IsWSSatisfied(WorldState* pwsWorldState);

};
