

#include "actionreload.h"
#include "agent.h"
#include "blackboard.h"
#include <iostream>

//Constructor/deconstructor
ActionReload::ActionReload()
{

}


//Handle initializing the Action, setting the 
//ActionRecord to use, as well as preconditions and effects.
void ActionReload::InitAction()
{
	std::cout << "Init Action: " << "kAct_Reload" << std::endl;
	super::InitAction();

	m_eActionType = kAct_Reload;
	m_fActionCost = 5.0f;
	m_fActionPrecedence = 1.0f;
	m_bActionIsInterruptible = false;
	m_fActionProbability = 1.0f;

	// Set preconditions.
	// Weapon must be armed.
	// Weapon must be unloaded.
	//m_wsWorldStatePreconditions.SetWSProp( kWSKey_WeaponArmed, NULL, kWSType_bool, true );

	// Set effects.
	// Weapon is loaded.
	m_wsWorldStateEffects.SetWSProp( kWSKey_WeaponLoaded, kWSType_bool, true );
}


//Return true if real-time preconditions are valid.
bool ActionReload::ValidateContextPreconditions( Agent* pAgent, WorldState& wsWorldStateGoal, bool bIsPlanning )
{
	if (!super::ValidateContextPreconditions(pAgent, wsWorldStateGoal, bIsPlanning))
	{
		return false;
	}

	// No such weapon type.
	/*if (!pWeapon)
	{
		return false;
	}*/

	//No ammo for this weapon type.
	//if ( !WeaponUtils::HasAmmo(pBrain, pWeaponRecord->eWeaponType, bIsPlanning) )
	//{
	//	return false;
	//}

	return true;
}


//Activate action.
void ActionReload::ActivateAction( Agent* pAgent, WorldState& wsWorldStateGoal )
{
	super::ActivateAction( pAgent, wsWorldStateGoal );

	// Reload our ranged weapon.
	//pAgent->SetCurrentWeapon( kWeaponType_Ranged ); //perhaps not

	//if not using ranged weapon then swap to it (SetCurrentWeapon)
	//then setreloadcurrent weapon // set currentweapon loaded.

	//should have some animation state controller to make
	//or timing system to make it take time to perform actions.
	//could read action time as cost? from action attributes.
	//
	pAgent->GetBlackBoard()->SetBBIsCurrentWeaponLoaded( true );
	//pAgent->GetBlackBoard()->SetBBFaceTarget( true );
}


//Determine if action has completed
bool ActionReload::IsActionComplete( Agent* pAgent )
{

	//could just check blackboard to see if weapon is loaded? (like e.long)
	if(pAgent->GetBlackBoard()->GetBBIsCurrentWeaponLoaded() )
	{	
		return true;
	}

	// Reloading is not complete.
	return false;
}
