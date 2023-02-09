//------------------------------------------------------------------------
//
//  Name:   BlackBoard.h
//
//  Desc:   Blackboard implementation:
//			The BlackBoard is used by various AI subsystems to share
//			their requests, intents, and results.
//
//  Author: Brett Keir Jones
//
//------------------------------------------------------------------------

#pragma once

//includes
#include "EnumeratedTypes.h"
#include "ActionStorage.h"

//forward declarations
class AGOAPController;

//intra-agent blackboard 
class BlackBoard
{
public:

	//ctor
	BlackBoard(AGOAPController* pOwner);
	BlackBoard();

	//dtor
	~BlackBoard();

	//agent info
	ENUM_WMTASK_STATUS	    GetBBTaskStatus() const { return m_eTaskStatus; }
	void					SetBBTaskStatus(ENUM_WMTASK_STATUS eStatus) { m_eTaskStatus = eStatus; }

	bool					GetBBSelectAction() const { return m_bSelectAction; }
	void					SetBBSelectAction(bool bSelectAction) { m_bSelectAction = bSelectAction; }

	//bool					GetBBTargetVisible() const { return m_bTargetVisible; }
	//void					SetBBTargetVisible(bool bVisible) { m_bTargetVisible = bVisible; }

	//plan info
	bool					GetBBInvalidatePlan() const { return m_bInvalidatePlan; }
	void					SetBBInvalidatePlan(bool bInvalidatePlan)  { m_bInvalidatePlan = bInvalidatePlan; }

private:

	//agent info
	ENUM_WMTASK_STATUS		m_eTaskStatus;

	bool					m_bSelectAction;

	//plan info
	bool					m_bInvalidatePlan;


};
