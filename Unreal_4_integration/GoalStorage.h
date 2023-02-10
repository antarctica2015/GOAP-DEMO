//------------------------------------------------------------------------
//
//  Name:   GoalStorage.h
//
//  Desc:   goal storage class to create/hold an agents goals
//
//  Author: Brett Keir Jones
//
//------------------------------------------------------------------------

//includes
#include "GoalAbstract.h"

//include all goals
#include "GoalTest.h"

//forward dec
class  GoalAbstract;
class  AGOAPController;

//
typedef TArray<GoalAbstract*> GOAL_LIST;

enum ENUM_GOALSET_ID
{
	kGSetID_Invalid = -1,
	kGSetID_One,
	kGSetID_Two,
};

//
class GoalStorage
{
public:

	GoalStorage(AGOAPController* pAgent);
	~GoalStorage();

	void				Init();
	void				Terminate(bool bDestroyAll);

	ENUM_GOALSET_ID		GetGoalSetID() const { return m_eGoalSet; }
	double				GetGoalSetTime() const { return m_fGoalSetTime; }

	GoalAbstract*		CreateNewGoal(ENUM_GOAL_TYPE eGoalType);

	GoalAbstract*	    AddGoal(ENUM_GOAL_TYPE eGoalType);
	void				RemoveGoal(ENUM_GOAL_TYPE eGoalType);

	GoalAbstract*	    FindGoalByType(ENUM_GOAL_TYPE eGoalType);

	void				SelectRelevantGoal();
	void				UpdateGoal();

	// Current goal.
	bool IsCurrentGoal(const GoalAbstract* const pGoal) const { return (bool)(pGoal == m_pCurrentGoal); }

	void SetCurrentGoal(GoalAbstract* pGoal) { m_pCurrentGoal = pGoal; }

	// Debugging.
	GoalAbstract*		GetCurrentGoal() const { return m_pCurrentGoal; }

	//GOAL_LIST::TConstIterator BeginGoals() const { return m_plistGoals.GetData.CreateConstIterator() ; }
	//GOAL_LIST::TConstIterator EndGoals() const { return m_plistGoals.Top.CreateConstIterator(); }


protected:

	void				UpdateGoalRelevances(bool bReplan);
	GoalAbstract*	    FindMostRelevantGoal(bool bRecalculate);


protected:

	AGOAPController*   m_pAgent;

	GOAL_LIST			m_plistGoals;
	GoalAbstract*	    m_pCurrentGoal;

	ENUM_GOALSET_ID		m_eGoalSet;
	double				m_fGoalSetTime;

};
