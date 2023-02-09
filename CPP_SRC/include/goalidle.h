#ifndef GOALIDLE_H
#define GOALIDLE_H

//------------------------------------------------------------------------
//
//  Name:   goalidle.h
//
//  Desc:  
//
//  Author: Brett Keir Jones 2008
//
//------------------------------------------------------------------------

//includes
#include "goalabstract.h"


class GoalIdle : public GoalAbstract
{
	typedef GoalAbstract super;

	public:
		virtual ENUM_GOAL_TYPE GetGoalClassType() const { return kGoal_Idle; }


		GoalIdle();

		virtual void	InitGoal(Agent* pAgent, ENUM_GOAL_TYPE eGoalType);

		virtual void	CalculateGoalRelevance();
		
		virtual void	SetWSSatisfaction( WorldState& WorldState );
		virtual bool	IsWSSatisfied( WorldState* pwsWorldState );

};

#endif