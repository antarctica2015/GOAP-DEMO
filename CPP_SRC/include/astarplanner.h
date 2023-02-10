#ifndef ASTARPLANNER_H
#define ASTARPLANNER_H

//------------------------------------------------------------------------
//
//  Name:   astarplanner.h
//
//  Desc:   AStar Node, Goal, Storage, and Map classes for finding
//          action plans from the planner. (from fear sdk)
//
//  Author: Brett Keir Jones 2008
//
//------------------------------------------------------------------------

#include "astarmachine.h"
#include "astarstoragelinkedlist.h"
#include "worldstate.h"
#include "actionabstract.h"
#include <iostream>


// Forward declarations.
class AStarMapPlanner;
class GoalAbstract;
class ActionAbstract;

//typedefs
typedef std::vector<ENUM_ACTION_TYPE> ACTION_TYPE_LIST; // move else where?


class AStarNodePlanner : public AStarNodeAbstract
{
public:

	 AStarNodePlanner();
	~AStarNodePlanner() {}

	// Debug.
	virtual void	DebugPrint() { std::cout << "ASTAR: " << eAStarNodeID << std::endl; }
	virtual void	DebugPrintExpand();
	virtual void	DebugPrintNeighbour();

public:

	WorldState		wsWorldStateCurrent;
	WorldState		wsWorldStateGoal;
	AStarMachine*	pAStarMachine;
};



class AStarGoalPlanner : public AStarGoalAbstract
{
public:
	 AStarGoalPlanner();
	~AStarGoalPlanner();

	// AStarGoalAbstract required functions.
	virtual void	SetTargetNode( ENUM_ASTAR_NODE_ID eAStarNode ) { m_eAStarNodeTarget = eAStarNode; }
	virtual float	GetHeuristicDistance( AStarNodeAbstract* pAStarNode );
	virtual float	GetActualCost(AStarNodeAbstract* pAStarNodeA, AStarNodeAbstract* pAStarNodeB );
	virtual bool	IsAStarFinished( AStarNodeAbstract* pAStarNode );

	// Initialisation.
	void			InitAStarGoalPlanner( Agent* pAgent, AStarMapPlanner* pAStarMapPlanner, GoalAbstract* pGoal );
	bool			IsPlanValid( AStarNodePlanner* pAStarNode );

protected:

	ENUM_ASTAR_NODE_ID		m_eAStarNodeTarget;
	AStarMapPlanner*		m_pAStarMapPlanner;
	GoalAbstract*			m_pGoal;
	Agent*					m_pAgent;
};



class AStarStoragePlanner : public AStarStorageLinkedList
{
public:

	void							InitAStarStoragePlanner( AStarMachine* pAStarMachine );

	// AStarStorageLinkedList overrides.
	virtual AStarNodeAbstract*		CreateAStarNode( ENUM_ASTAR_NODE_ID eAStarNode );
	virtual void					DestroyAStarNode( AStarNodeAbstract* pAStarNode );

protected:

	AStarMachine*	m_pAStarMachine;
};




//
class AStarMapPlanner : public AStarMapAbstract
{
public:
	 AStarMapPlanner();
	~AStarMapPlanner();

	// AStarMapAbstract required functions.
	virtual int					GetNumAStarNeighbours( Agent* pAgent, AStarNodeAbstract* pAStarNode );
	virtual ENUM_ASTAR_NODE_ID	GetAStarNeighbour( Agent* pAgent, AStarNodeAbstract* pAStarNode, int iNeighbour, AStarStorageAbstract* pAStarStorage );

	virtual void				SetAStarFlags( ENUM_ASTAR_NODE_ID eAStarNode, unsigned long dwFlags, unsigned long dwMask );
	virtual unsigned long		GetAStarFlags( ENUM_ASTAR_NODE_ID eAStarNode );

	// Initialisation.
	void	BuildEffectActionsTable();
	void	InitAStarMapPlanner( Agent* pAgent );
	
	// ID conversion.
	ENUM_ACTION_TYPE		ConvertID_AStarNode2Action( ENUM_ASTAR_NODE_ID eAStarNode ) { return ( ENUM_ACTION_TYPE )eAStarNode; }
	ENUM_ASTAR_NODE_ID		ConvertID_AIAction2AStarNode( ENUM_ACTION_TYPE eAction ) { return ( ENUM_ASTAR_NODE_ID )eAction; }

	// Action access.
	ActionAbstract*			GetAction( ENUM_ASTAR_NODE_ID eAStarNode );

protected:

	Agent*					m_pAgent;
	ACTION_TYPE_LIST		m_listEffectActions[kWSKey_Count];
	ENUM_ACTION_TYPE		m_aNeighbourActions[kAct_Count];
	int						m_cNeighbourActions;
	bool					m_bEffectTableBuilt;

};


#endif
