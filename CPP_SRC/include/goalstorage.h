#ifndef GOALSTORAGE_H
#define GOALSTORAGE_H

//------------------------------------------------------------------------
//
//  Name:   goalstorage.h
//
//  Desc:   goal storage class to create/hold an agents goals
//
//  Author: Brett Keir Jones 2008
//
//------------------------------------------------------------------------

//includes
#include "goalabstract.h"

//include all goals
#include "goalidle.h"

//forward dec
class  GoalAbstract;
class  Agent;

//
typedef std::vector<GoalAbstract*> GOAL_LIST;

enum ENUM_GOALSET_ID
{
	kGSetID_Invalid = -1,
	kGSetID_SWAT,
	kGSetID_Suspect,
};

//
class GoalStorage
{
public:

		GoalStorage(Agent* pAgent);
		~GoalStorage();


		void				Init();
		void				Terminate(bool bDestroyAll);

		//void SetGoalSet(ENUM_GOALSET_ID eGoalSet, bool bClearGoals);

		//void				SetGoalSet(const char* szGoalSet, const char* szName, bool bClearGoals);
		//void				CreateGoalSet(ENUM_GOALSET_ID eID);

		ENUM_GOALSET_ID		GetGoalSetID() const { return m_eGoalSet; }
		double				GetGoalSetTime() const { return m_fGoalSetTime; }

		GoalAbstract*		CreateNewGoal(ENUM_GOAL_TYPE eGoalType);

		GoalAbstract*	    AddGoal(ENUM_GOAL_TYPE eGoalType);
		//GoalAbstract*	    AddGoal(ENUM_GOAL_TYPE eGoalType, double fTime);
		void				RemoveGoal(ENUM_GOAL_TYPE eGoalType);

		GoalAbstract*	    FindGoalByType(ENUM_GOAL_TYPE eGoalType);

		void				SelectRelevantGoal();
		void				UpdateGoal();


		// Current goal.
		bool IsCurrentGoal( const GoalAbstract* const pGoal ) const { return (bool)(pGoal == m_pCurrentGoal); }
		
		void SetCurrentGoal(GoalAbstract* pGoal) { m_pCurrentGoal = pGoal;}

		// Debugging.
		GoalAbstract*		GetCurrentGoal() const { return m_pCurrentGoal; }

		GOAL_LIST::const_iterator BeginGoals() const { return m_plistGoals.begin(); }
		GOAL_LIST::const_iterator EndGoals() const { return m_plistGoals.end(); }


protected:

		//void	SetGoalSet(ENUM_GOALSET_ID m_eGoalSet, const char* szName, bool bClearGoals);

		void				UpdateGoalRelevances( bool bReplan );
		GoalAbstract*	    FindMostRelevantGoal( bool bRecalculate );


protected:

		Agent*				m_pAgent;

		GOAL_LIST			m_plistGoals;
		GoalAbstract*	    m_pCurrentGoal;

		ENUM_GOALSET_ID		m_eGoalSet;
		double				m_fGoalSetTime;

};



#endif