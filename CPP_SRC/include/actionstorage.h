#ifndef ACTIONSTORAGE_H
#define ACTIONSTORAGE_H

//------------------------------------------------------------------------
//
//  Name:   actionstorage.h
//
//  Desc:   action storage class to create/hold an agents actions
//
//  Author: Brett Keir Jones 2008
//
//------------------------------------------------------------------------

#include "actionabstract.h"

//include all action header files?
#include "actionidle.h" 
#include "actionreload.h" 
//#include "actiongoto.h"

//forward declarations

class ActionStorage;
class ActionAbstract;

//typedefs
typedef std::vector<ActionAbstract*> ACTION_LIST;

//only have one actionset.
//enum ENUM_ACTIONSET
//{
//kActionSet_Invalid = -1,
//kActionSet_BaseChar,
//};

//
class ActionStorage
{
public:

	ActionStorage();
	~ActionStorage();


	void	Init();
	void	Term();

	//Querys
	ActionAbstract*		GetAction( ENUM_ACTION_TYPE eActionType );
	int					GetNumActions() const { return m_pActionList.size() ; }
	bool				IsActionInActionList( ENUM_ACTION_TYPE eActionType );

	protected:

		ActionAbstract*	CreateNewAction(ENUM_ACTION_TYPE eActionClass); //

		ACTION_LIST::const_iterator BeginActions() const { return m_pActionList.begin(); }
		ACTION_LIST::const_iterator EndAction() const { return m_pActionList.end(); }

	protected:

		ACTION_LIST m_pActionList; //


};

#endif