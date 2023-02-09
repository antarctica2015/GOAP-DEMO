
//------------------------------------------------------------------------
//
//  Name:   GoalFollow.h
//
//  Desc:  
//
//  Author: Brett Keir Jones
//
//------------------------------------------------------------------------

#include "GoalAbstract.h"

class GoalFollow : public GoalAbstract
{
	typedef GoalAbstract super;

public:
	virtual ENUM_GOAL_TYPE GetGoalClassType() const { return kGoal_Follow; }

	GoalFollow();

	virtual void	InitGoal(AGOAPController* pAgent, ENUM_GOAL_TYPE eGoalType);

	virtual void	CalculateGoalRelevance();

	virtual void	SetWSSatisfaction(WorldState& WorldState);
	virtual bool	IsWSSatisfied(WorldState* pwsWorldState);

};