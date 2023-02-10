

#include "actionstorage.h"
#include "utils.h"


//Constructor & Deconstructor
ActionStorage::ActionStorage()
{
	
	m_pActionList.clear();

	Init();
	
}

ActionStorage::~ActionStorage()
{
	Term();
	m_pActionList.clear();
}

//Initalise
void ActionStorage::Init()
{
	ActionAbstract*			pAction;

	//create and initialise all the actions
	for(unsigned int iAction =0 ; iAction < kAct_Count; ++iAction)
	{
		//
		pAction = CreateNewAction( (ENUM_ACTION_TYPE)iAction ); //cast to (ActionAbstract*) ??
		//std::cout << "Created Action: " << s_aszActionTypes[ pAction->GetActionType() ] << std::endl;
		std::cout << "Created Action: " << s_aszActionTypes[ (ENUM_ACTION_TYPE)iAction ] << std::endl;

			if(pAction)
			{	
				pAction->InitAction();
				m_pActionList.push_back(pAction);	
			}
		
	}
}



//terminate the action manager
void ActionStorage::Term()
{
	ActionAbstract* pAction;
	ACTION_LIST::iterator itAction = m_pActionList.begin();
	
	for( itAction ;itAction != m_pActionList.end(); ++itAction )
	{
		pAction = (*itAction);

		if(pAction)
		{
			std::cout << "Destroying Action: " << s_aszActionTypes[pAction->GetActionType()] << std::endl;
			delete pAction;
			pAction = NULL;
		}

	}
}


//create an action
ActionAbstract* ActionStorage::CreateNewAction(ENUM_ACTION_TYPE eActionClass)
{
	switch(eActionClass)
	{
		case kAct_Idle: { return new ActionIdle(); }     //0
		case kAct_Reload: { return new ActionReload(); } //1
		//case kAct_Goto: { return new (ActionAbstract*)( ActionGoto() ) ; }     //2

		default: std::cout << "ActionStorage::CreateAction: Unrecognised Action Type!" << std::endl;
			
			//break; ////log error to file: "ActionStorage::CreateAction: Unrecognised Action Type!"
	}

	return NULL;
}


//get a pointer to an action
ActionAbstract* ActionStorage::GetAction(ENUM_ACTION_TYPE eActionType)
{
	if( (eActionType > kAct_InvalidType) && ( eActionType < kAct_Count ) )
	{
		return m_pActionList[eActionType];
	}

	return NULL;
}

//returns true if the action is in the specified set, else returns false.
bool ActionStorage::IsActionInActionList(ENUM_ACTION_TYPE eActionType)
{
	//check if action enum is invalid.
	if( (eActionType <= kAct_InvalidType) || ( eActionType >= kAct_Count ) )
	{
		return false;
	}

	for(int iAction = 0 ; iAction < kAct_Count ; ++iAction)
	{
		if( m_pActionList[iAction]->GetActionType() == eActionType )
		{
			return true;
		}
	}

	std::cout << "Action not in list!: " << eActionType << std::endl;
	return false;
}

