
#include "GoapTest.h"
#include "BlackBoard.h" 

// Constructor/Deconstructor
BlackBoard::BlackBoard()
{
//	m_bTargetVisible = false;
	m_eTaskStatus = kTaskStatus_Unset;

	m_bSelectAction = true;
	m_bInvalidatePlan = false;

//	m_bInvalidatePath = false;

	//m_bWeaponLoaded = false;

}

BlackBoard::~BlackBoard()
{
}
