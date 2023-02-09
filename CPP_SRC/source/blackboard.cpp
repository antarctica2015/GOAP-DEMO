




#include "blackboard.h" 

// Constructor/Deconstructor
BlackBoard::BlackBoard()
{

	//m_bFaceTarget = false;
	m_bTargetVisible = false;
	m_eTaskStatus = kTaskStatus_Unset;

	m_bSelectAction = true;
	m_bInvalidatePlan = false;

	//m_eActionSet = kActionSet_Invalid;
	//m_eTargetSelectSet = kTargetSelectSet_Invalid;

	m_bInvalidatePath = false;

	m_bWeaponLoaded = false;
	//m_bReloadCurrentWeapon = false;
	

}

BlackBoard::~BlackBoard()
{
}
