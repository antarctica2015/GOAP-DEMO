
#include "GoapTest.h"
#include "ActionStorage.h"
#include "Utils.h"

//Constructor & Deconstructor
ActionStorage::ActionStorage()
{
	m_pActionList.Empty();

	Init();

}

ActionStorage::~ActionStorage()
{
	Term();
	m_pActionList.Empty();
}

//Initalise
void ActionStorage::Init()
{
	ActionAbstract*			pAction;

	//create and initialise all the actions
	for (uint32 iAction = 0; iAction < kAct_Count; ++iAction) {
		//
		pAction = CreateNewAction((ENUM_ACTION_TYPE)iAction);

		if (pAction) {
			pAction->InitAction();
			m_pActionList.Emplace(pAction);
		}

	}
}


//terminate the action manager
void ActionStorage::Term()
{
	ActionAbstract* pAction;

	for (auto itAction = m_pActionList.CreateConstIterator(); itAction ; ++itAction) {
		pAction = (*itAction);

		if (pAction) {
			delete pAction;
			pAction = NULL;
		}

	}
}


//create an action
ActionAbstract* ActionStorage::CreateNewAction(ENUM_ACTION_TYPE eActionClass)
{
	switch (eActionClass) 
	{
	case kAct_Test: { return new ActionTest(); }     //0

	default: 
		break; ////log error to file: "ActionStorage::CreateAction: Unrecognised Action Type!"
	}

	return NULL;
}


//get a pointer to an action
ActionAbstract* ActionStorage::GetAction(ENUM_ACTION_TYPE eActionType)
{
	if ((eActionType > kAct_InvalidType) && (eActionType < kAct_Count)) {
		return m_pActionList[eActionType];
	}

	return NULL;
}

//returns true if the action is in the specified set, else returns false.
bool ActionStorage::IsActionInActionList(ENUM_ACTION_TYPE eActionType)
{
	//check if action enum is invalid.
	if ((eActionType <= kAct_InvalidType) || (eActionType >= kAct_Count)) {
		return false;
	}

	for (int32 iAction = 0; iAction < kAct_Count; ++iAction) {
		if (m_pActionList[iAction]->GetActionType() == eActionType) {
			return true;
		}
	}

	return false;
}

