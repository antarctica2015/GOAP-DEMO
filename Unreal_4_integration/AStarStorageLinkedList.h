//------------------------------------------------------------------------
//
//  Name:   AStarStorageLinkedList.h
//
//  Desc: AStar Storage class using a linked list.
//
//  Author: Brett Keir Jones
//
//------------------------------------------------------------------------

#pragma once

#include "AStarMachine.h"

//Linked list storage class for A* nodes.
//could probably replace with STL list or vector.

class AStarStorageLinkedList : public AStarStorageAbstract
{
public:
	AStarStorageLinkedList();
	~AStarStorageLinkedList();

	// AStarStorageAbstract required functions.
	virtual AStarNodeAbstract*		CreateAStarNode(ENUM_ASTAR_NODE_ID eAStarNode);
	virtual void					DestroyAStarNode(AStarNodeAbstract* pAStarNode);

	virtual void	ResetAStarStorage();

	virtual void	AddToOpenList(AStarNodeAbstract* pAStarNode, AStarMapAbstract* pAStarMap);
	virtual void	AddToClosedList(AStarNodeAbstract* pAStarNode, AStarMapAbstract* pAStarMap);

	virtual void	RemoveFromOpenList(AStarNodeAbstract* pAStarNode);
	virtual void	RemoveFromClosedList(AStarNodeAbstract* pAStarNode);

	virtual AStarNodeAbstract*	RemoveCheapestOpenNode();

	virtual AStarNodeAbstract*	FindInOpenList(ENUM_ASTAR_NODE_ID eAStarNode);
	virtual AStarNodeAbstract*	FindInClosedList(ENUM_ASTAR_NODE_ID eAStarNode);

protected:

	// List management.
	void			ClearList(AStarNodeAbstract*& pListHead);
	void			AddToList(AStarNodeAbstract*& pListHead, AStarNodeAbstract* pAStarNode);
	void			RemoveFromList(AStarNodeAbstract*& pListHead, AStarNodeAbstract* pAStarNode);

	AStarNodeAbstract*	FindInList(AStarNodeAbstract* pListHead, ENUM_ASTAR_NODE_ID eAStarNode);

protected:

	AStarNodeAbstract*	m_pOpenListHead;
	AStarNodeAbstract*	m_pClosedListHead;
};
