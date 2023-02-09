#ifndef BLACKBOARD_H
#define BLACKBOARD_H

//------------------------------------------------------------------------
//
//  Name:   blackboard.h
//
//  Desc:   Blackboard implementation:
//			The BlackBoard is used by various AI subsystems to share
//			their requests, intents, and results.
//
//  Author: Brett Keir Jones 2008
//
//------------------------------------------------------------------------

//includes
#include "enumeratedtypes.h"
#include "actionstorage.h"


//forward declarations
class Agent;


//intra-agent blackboard 
class BlackBoard
{
public:
	
	//ctor
	BlackBoard(Agent* pOwner);
	BlackBoard();
	
	//dtor
	~BlackBoard();


	//agent info

	//ENUM_ACTIONSET			GetBBActionSet() const { return m_eActionSet; }
	//void					SetBBActionSet(ENUM_ACTIONSET eActionSet) { m_eActionSet = eActionSet; }

	ENUM_WMTASK_STATUS	    GetBBTaskStatus() const { return m_eTaskStatus; }
	void					SetBBTaskStatus( ENUM_WMTASK_STATUS eStatus ) { m_eTaskStatus = eStatus; }

	bool					GetBBSelectAction() const { return m_bSelectAction; }
	void					SetBBSelectAction( bool bSelectAction ) { m_bSelectAction = bSelectAction; }
	
	//goal info


	//navigation info

	//targetting info
	//bool					GetBBFaceTarget() const { return m_bFaceTarget; }
	//void					SetBBFaceTarget( bool bFace ) { m_bFaceTarget = bFace; }

	bool					GetBBTargetVisible() const { return m_bTargetVisible; }
	void					SetBBTargetVisible( bool bVisible ) { m_bTargetVisible = bVisible; }

	//ENUM_TARGET_SELECT_SET	GetBBTargetSelectSet() const { return m_eTargetSelectSet; }
	//void					SetBBTargetSelectSet(ENUM_TARGET_SELECT_SET eTargetSelectSet) { m_eTargetSelectSet = eTargetSelectSet; }

	bool					GetBBInvalidatePath() const { return m_bInvalidatePath; }
	void					SetBBInvalidatePath(bool bRecacl) { m_bInvalidatePath = bRecacl; }

	//weapons info

	//ENUM_WEAPON_TYPE		GetBBCurrentWeapon(){ return m_eCurrentWeapon; };
	//void					SetBBCurrentWeapon(ENUM_WEAPON_TYPE eWeapon){ m_eCurrentWeapon = eWeapon; };

	bool					GetBBIsCurrentWeaponLoaded(){ return m_bWeaponLoaded; };
	void					SetBBIsCurrentWeaponLoaded(bool bWeaponLoaded){ m_bWeaponLoaded = bWeaponLoaded; }; 

	//bool					GetBBReloadCurrentWeapon(){ return m_bReloadCurrentWeapon; };
	//void					SetBBReloadCurrentWeapon(bool bReload){ m_bReloadCurrentWeapon = bReload; };

	
	//plan info
	bool					GetBBInvalidatePlan() const { return m_bInvalidatePlan ;}
	void					SetBBInvalidatePlan(bool bInvalidatePlan)  { m_bInvalidatePlan = bInvalidatePlan ;}

private:

	//agent info
	
	//ENUM_ACTIONSET			m_eActionSet;
	//ENUM_TARGET_SELECT_SET	m_eTargetSelectSet;
	ENUM_WMTASK_STATUS		m_eTaskStatus;
	
	bool					m_bSelectAction;
	
	//goal info
	//navigation info
	
	//targetting info

	//bool					m_bFaceTarget;
	bool					m_bTargetVisible;
	bool					m_bInvalidatePath;

	//weapons info
	bool					m_bWeaponLoaded;
	//bool					m_bReloadCurrentWeapon;
	
	//plan info
	bool					m_bInvalidatePlan;


};

#endif
