
// Includes 
#include "astarstoragelinkedlist.h"
#include "utils.h"


   
//Constructor / Destructor
AStarStorageLinkedList::AStarStorageLinkedList()
{
	m_pOpenListHead = NULL;
	m_pClosedListHead = NULL;
}

AStarStorageLinkedList::~AStarStorageLinkedList()
{
	ResetAStarStorage();
}


//Create an AStarNode with a specified ID. 
AStarNodeAbstract* AStarStorageLinkedList::CreateAStarNode( ENUM_ASTAR_NODE_ID eAStarNode )
{
	return NULL;
}


//Destroy an AStarNode. 
void AStarStorageLinkedList::DestroyAStarNode( AStarNodeAbstract* pAStarNode )
{
	//
}

           
//Delete nodes from Open and Closed lists.
void AStarStorageLinkedList::ResetAStarStorage()
{
	// Delete nodes from both lists.
	ClearList( m_pOpenListHead );
	ClearList( m_pClosedListHead );
}
     
//Delete nodes from list.
void AStarStorageLinkedList::ClearList( AStarNodeAbstract*& pListHead )
{
	// List is empty.
	if( !pListHead )
	{
		return;
	}

	AStarNodeAbstract* pNode = pListHead;
	AStarNodeAbstract* pNodeDelete;
	pListHead = NULL;

	// Iterate over all nodes in list, and delete them.
	while( pNode )
	{
		pNodeDelete = pNode;
		pNode = pNode->pListNext;
		DestroyAStarNode( pNodeDelete );
	}
}

         
//Add nodes to Open list.
void AStarStorageLinkedList::AddToOpenList( AStarNodeAbstract* pAStarNode, AStarMapAbstract* pAStarMap )
{
	// Bail is node is already in Open list.
	unsigned long dwFlags = pAStarMap->GetAStarFlags( pAStarNode->eAStarNodeID );
	
	if( dwFlags & AStarMapAbstract::kASTAR_Open )
	{
		return;
	}

	// Add node to Open list, and set AStar flags.
	AddToList( m_pOpenListHead, pAStarNode );
	pAStarMap->SetAStarFlags( pAStarNode->eAStarNodeID, AStarMapAbstract::kASTAR_Open, AStarMapAbstract::kASTAR_OpenOrClosed );
}

        
//Add nodes to Closed list.
void AStarStorageLinkedList::AddToClosedList( AStarNodeAbstract* pAStarNode, AStarMapAbstract* pAStarMap )
{
	// The AStar algorithm will never try to insert an already closed node in the Closed list, 
	// so no need to check for it.

	// Add node to Closed list, and set AStar flags.
	AddToList( m_pClosedListHead, pAStarNode );
	pAStarMap->SetAStarFlags( pAStarNode->eAStarNodeID, AStarMapAbstract::kASTAR_Closed, AStarMapAbstract::kASTAR_OpenOrClosed );
}


//Add nodes to list.
void AStarStorageLinkedList::AddToList( AStarNodeAbstract*& pListHead, AStarNodeAbstract* pAStarNode )
{
	// Insert a node at the head of the list.
	if( pListHead )
	{
		pListHead->pListPrev = pAStarNode;
		pAStarNode->pListNext = pListHead;
	}

	pListHead = pAStarNode;
}

       
//Remove node from Open list.
void AStarStorageLinkedList::RemoveFromOpenList( AStarNodeAbstract* pAStarNode )
{
	// No need to clear AStar flags, because when node is inserted into the Closed list,
	// flags be be cleared and reset.

	RemoveFromList( m_pOpenListHead, pAStarNode );
}

          
//Remove node from Closed list.
void AStarStorageLinkedList::RemoveFromClosedList( AStarNodeAbstract* pAStarNode )
{
	// No need to clear AStar flags, because when node is inserted into the Open list,
	// flags be be cleared and reset.

	RemoveFromList( m_pClosedListHead, pAStarNode );
}

    
//Remove node from list.
void AStarStorageLinkedList::RemoveFromList( AStarNodeAbstract*& pListHead, AStarNodeAbstract* pAStarNode )
{
	// Remove a node from the middle or end of the list.
	if( pAStarNode->pListPrev )
	{
		pAStarNode->pListPrev->pListNext = pAStarNode->pListNext;
	}

	// Remove a node from the head of the list.
	else {
		pListHead = pAStarNode->pListNext;
	}

	// Complete 2-way link.
	if( pAStarNode->pListNext )
	{
		pAStarNode->pListNext->pListPrev = pAStarNode->pListPrev;
	}

	// NULLify links.
	pAStarNode->pListNext = NULL;
	pAStarNode->pListPrev = NULL;
}

        
//Remove and return cheapest node from Open list.
AStarNodeAbstract* AStarStorageLinkedList::RemoveCheapestOpenNode()
{
	// NOTE: Finding the cheapest node could be optimized by 
	//       maintaining a short, sorted list of the cheapest
	//       nodes. See "How to Achieve Lightning-Fast A*" in 
	//       AI Game Programming Wisdom, p. 133.
	//       Specifically "Be a Cheapskate" on p. 140.

	AStarNodeAbstract* pNodeCheapest = m_pOpenListHead;

	// Iterate over all nodes, and keep track of the cheapest.
	AStarNodeAbstract* pNode;

	for( pNode = m_pOpenListHead; pNode; pNode = pNode->pListNext )
	{
		if( pNode->fFitness < pNodeCheapest->fFitness )
		{
			pNodeCheapest = pNode;
		}
	}

	// Remove the cheapest node from the Open list.
	if( pNodeCheapest )
	{
		RemoveFromOpenList( pNodeCheapest );
	}

	// Return the cheapest node.
	return pNodeCheapest;
}

          
//Return node if found in Open list.
AStarNodeAbstract* AStarStorageLinkedList::FindInOpenList( ENUM_ASTAR_NODE_ID eAStarNode )
{
	return FindInList( m_pOpenListHead, eAStarNode );
}

     
//Return node if found in Closed list.
AStarNodeAbstract* AStarStorageLinkedList::FindInClosedList( ENUM_ASTAR_NODE_ID eAStarNode )
{
	return FindInList( m_pClosedListHead, eAStarNode );
}

         
//Return node if found in list.
AStarNodeAbstract* AStarStorageLinkedList::FindInList( AStarNodeAbstract* pListHead, ENUM_ASTAR_NODE_ID eAStarNode )
{
	// The Fear SDK notes that List searches could be optimized by using a hash table of lists rather than one flat list.

	// Iterate over all nodes in list.
	AStarNodeAbstract* pNode = pListHead;
	while( pNode )
	{
		// Return node if a match is found.
		if( pNode->eAStarNodeID == eAStarNode )
		{
			return pNode;
		}

		pNode = pNode->pListNext;
	}

	// No match was found.
	return NULL;
}
