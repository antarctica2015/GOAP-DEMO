#ifndef ASTARMACHINE_H
#define ASTARMACHINE_H

//------------------------------------------------------------------------
//
//  Name:   astarmachine.h
//
//  Desc:   A* machine declaration & abstract classes for components
//			based on Higgins(AI Wisdom 1) article.
//
//  Author: Brett Keir Jones 2008
//
//------------------------------------------------------------------------

//#include "utils.h" 
//

//forward declarations
class Agent;
class AStarMachine;
class AStarMapAbstract;
class AStarStorageAbstract;

//
enum ENUM_ASTAR_NODE_ID
{
	kASTARNODE_Invalid = -1,
};


//a star abstract node
class AStarNodeAbstract
{
public:
	AStarNodeAbstract();
	virtual ~AStarNodeAbstract();

	//Debug Print Methods
	virtual void	DebugPrint() = 0;
	virtual void	DebugPrintExpand() {}
	virtual void	DebugPrintNeighbour() {}


public:

	ENUM_ASTAR_NODE_ID		eAStarNodeID;

	float					fGoal;
	float					fHeuristic;
	float					fFitness;

	//linked-list pointers 
	AStarNodeAbstract*		pListPrev; 
	AStarNodeAbstract*		pListNext; 
	AStarNodeAbstract*		pAStarParent;
};


//a star abstract goal
class AStarGoalAbstract
{
public:

	virtual void	SetTargetNode( ENUM_ASTAR_NODE_ID eAStarNode) = 0;
	//virtual void	SetTargetNode( AStarNodeAbstract* pAStarNode) = 0;

	virtual float	GetHeuristicDistance( AStarNodeAbstract* pAStarNode ) = 0;
	virtual float	GetActualCost(AStarNodeAbstract* pAStarNodeA, AStarNodeAbstract* pAStarNodeB) = 0;
	virtual bool	IsAStarFinished( AStarNodeAbstract* pAStarNode ) = 0;
	virtual bool	IsAStarNodePassable( ENUM_ASTAR_NODE_ID eAStarNode ) {return true;}
	//virtual bool	IsAStarNodePassable( AStarNodeAbstract* pAStarNode ) {return true;}

};


//a star storage class
class AStarStorageAbstract
{
public:
	virtual AStarNodeAbstract*		CreateAStarNode( ENUM_ASTAR_NODE_ID eAStarNode ) = 0;
	virtual void					DestroyAStarNode( AStarNodeAbstract* pAStarNode ) = 0;

	virtual void	ResetAStarStorage() = 0;
	virtual void	AddToOpenList( AStarNodeAbstract* pAStarNode, AStarMapAbstract* pAStarMap ) = 0;
	virtual void	AddToClosedList( AStarNodeAbstract* pAStarNode, AStarMapAbstract* pAStarMap ) = 0;
	virtual void	RemoveFromOpenList( AStarNodeAbstract* pAStarNode ) = 0;
	virtual void	RemoveFromClosedList( AStarNodeAbstract* pAStarNode ) = 0;

	virtual AStarNodeAbstract*	FindInOpenList( ENUM_ASTAR_NODE_ID eAStarNode ) = 0;
	virtual AStarNodeAbstract*	FindInClosedList( ENUM_ASTAR_NODE_ID eAStarNode ) = 0;

	virtual AStarNodeAbstract*	RemoveCheapestOpenNode() = 0;
};



//The Astar Map determines how to correlate A* NodeIDs with locations on a map of any kind.
class AStarMapAbstract
{
public:
	enum ENUM_ASTAR_FLAGS
	{
		kASTAR_Unchecked	= 0x00,
		kASTAR_Open			= 0x01,
		kASTAR_Closed		= 0x02,
		kASTAR_OpenOrClosed	= kASTAR_Open | kASTAR_Closed,
		kASTAR_NotPassable	= 0x04,
	};

public:

	virtual int					GetNumAStarNeighbours( Agent* pAgent, AStarNodeAbstract* pAStarNode ) = 0;
	virtual ENUM_ASTAR_NODE_ID	GetAStarNeighbour( Agent* pAgent, AStarNodeAbstract* pAStarNode, int iNeighbour, AStarStorageAbstract* pAStarStorage ) = 0;

	virtual void				SetAStarFlags( ENUM_ASTAR_NODE_ID eAStarNode, unsigned long dwFlags, unsigned long dwMask ) = 0;
	virtual unsigned long		GetAStarFlags( ENUM_ASTAR_NODE_ID eAStarNode ) = 0;

	virtual void				SetupPotentialNeighbour( AStarNodeAbstract* pAStarNodeParent, AStarNodeAbstract* pAStarNodeChild ) {}
	virtual void				FinaliseNeighbour( AStarNodeAbstract* pAStarNodeChild ) {}
};



//the (generic)a star machine implementation
//perhaps modify later to use a different algothim at its core perhaps D*?
//could also adopt a templatised style.
class AStarMachine
{
public:

	AStarMachine();
	~AStarMachine();

	//initalise the a star machine
	void InitAStarMachine( Agent* pAgent, AStarStorageAbstract* pAStarStorage, AStarGoalAbstract* pAStarGoal, AStarMapAbstract* pAStarMap);

	//set up for A*
	void SetAStarSource(ENUM_ASTAR_NODE_ID eAStarNodeSource);
	void SetAStarTarget(ENUM_ASTAR_NODE_ID eAStarNodeTarget);

	//run A Star algorithm
	void RunAStar();

	//data access
	AStarNodeAbstract*		GetAStarNodeCurrent() { return m_pAStarNodeCurrent; }
	AStarStorageAbstract*	GetAStarStorage() { return m_pAStarStorage; }
	AStarMapAbstract*		GetAStarMap()	{ return m_pAStarMap; }

protected:

	Agent*					m_pAgent;

	AStarStorageAbstract*	m_pAStarStorage;
	AStarGoalAbstract*		m_pAStarGoal;
	AStarMapAbstract*		m_pAStarMap;

	AStarNodeAbstract*		m_pAStarNodeCurrent;

	ENUM_ASTAR_NODE_ID		m_eAStarNodeSource;
	ENUM_ASTAR_NODE_ID		m_eAStarNodeTarget;

};

#endif
