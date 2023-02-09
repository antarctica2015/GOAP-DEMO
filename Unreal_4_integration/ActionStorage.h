
//------------------------------------------------------------------------
//
//  Name:   ActionStorage.h
//
//  Desc:   action storage class to create/hold an agents actions
//
//  Author: Brett Keir Jones 
//
//------------------------------------------------------------------------

#pragma once

#include "ActionAbstract.h"

//include all action header files
#include "ActionTest.h" 

//forward declarations
class ActionStorage;
class ActionAbstract;

//typedefs
typedef TArray<ActionAbstract*> ACTION_LIST;

//
class ActionStorage
{
public:

	ActionStorage();
	~ActionStorage();


	void	Init();
	void	Term();

	//Querys
	ActionAbstract*		GetAction(ENUM_ACTION_TYPE eActionType);
	int32				GetNumActions() const { return m_pActionList.Num(); }
	bool				IsActionInActionList(ENUM_ACTION_TYPE eActionType);

protected:

	ActionAbstract*	CreateNewAction(ENUM_ACTION_TYPE eActionClass); //

	//ACTION_LIST::TConstIterator BeginActions() const { return m_pActionList.GetData().CreateConstIterator() ; }
	//ACTION_LIST::TConstIterator EndAction() const { return m_pActionList.Top().CreateConstIterator() ; }

protected:

	ACTION_LIST m_pActionList; //
};
