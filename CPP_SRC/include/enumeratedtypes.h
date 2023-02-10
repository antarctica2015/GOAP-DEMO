#ifndef ENUMTYPES_H
#define ENUMTYPES_H

//------------------------------------------------------------------------
//
//  Name:   enumeratedtypes.h
//
//  Desc:   
//
//  Author: Brett Keir Jones 2008
//
//------------------------------------------------------------------------
//

enum ENUM_TASK_TYPE // task / orders (check notes and add more?)
{
	kTask_InvalidType = 0,
	kTask_Advance,
	kTask_ClearRoom,  //search room?
	kTask_CoverDirection,  // cover direction
	kTask_DeployItem,
	kTask_Follow,
	kTask_FallIn, // rejoin/form formation
	kTask_MoveTo,
	kTask_Restrain
};

enum ENUM_WMTASK_STATUS
{
	kTaskStatus_Unset = 0,
	kTaskStatus_Set,
	kTaskStatus_Done,
};

//enum ENUM_KNOWLEDGE_TYPE
//{
//	kKnowledge_InvalidType = 0,
//	kKnowledge_AvoidNode,
//	kKnowledge_BlockedPath,
//	kKnowledge_DoorBlocked,
//	kKnowledge_EnemyKnowsPosition,
//};


//enum ENUM_ACTION_STATUS //used for?
//{
//	kAction_Failed = 0,
//	kAction_Set,
//	kAction_Complete
//};

#endif