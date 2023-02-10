
#include "aibrain.h"
#include "astarnavgraph.h"


//#include "navgraphedgeabstract.h"

class AStarMapNavGraph;
//----------------------------------------------------------------------------

AStarGoalNavGraph::AStarGoalNavGraph()
{
	m_eAStarNodeDest = kASTARNODE_Invalid;
	m_pAStarMapNavGraph = NULL;
}

AStarGoalNavGraph::~AStarGoalNavGraph()
{
}

          
//Get a best-guess distance from a node to the goal. (manhatten distance)
float AStarGoalNavGraph::GetHeuristicDistance( AStarNodeAbstract* pAStarNode )
{
	// Use Manhatten distance (sum of the differences in x,y,z) to get a best guess of
	// the distance from a node to the goal.

	// The start point is the node position //center of the poly.
	Ogre::Vector3 vManhattenDist;

	//CAINavMeshPoly* pNMPoly = m_pAStarMapNavMesh->GetNMPoly( pAStarNode->eAStarNodeID );
	NavGraphNode* pNavGraphNode = m_pAStarMapNavGraph->GetNavGraphNode( pAStarNode->eAStarNodeID );

	//NavGraphNode == NavGraphPoly? (for my purposes?)
	
	if( pNavGraphNode )
	{
		vManhattenDist = pNavGraphNode->GetPosition(); //GetPosition() //->GetNMPolyCenter();
	}

	//pNMPoly = m_pAStarMapNavMesh->GetNMPoly( m_eAStarNodeDest );
	pNavGraphNode = m_pAStarMapNavGraph->GetNavGraphNode( m_eAStarNodeDest );
	
	if( pNavGraphNode )
	{
		vManhattenDist -= pNavGraphNode->GetPosition(); //->GetNMPolyCenter();
	}

	// Get the absolute value of each component.

	if( vManhattenDist.x < 0.0f )
	{
		vManhattenDist.x *= -1.0f;
	}
	if( vManhattenDist.y < 0.0f )
	{
		vManhattenDist.y *= -1.0f;
	}
	if( vManhattenDist.z < 0.0f )
	{
		vManhattenDist.z *= -1.0f;
	}

	// Return the estimate.

	return ( vManhattenDist.x + vManhattenDist.y + vManhattenDist.z );
}



//Return the actual cost from one node to another.
float AStarGoalNavGraph::GetActualCost( AIBrain* pBrain, AStarNodeAbstract* pAStarNodeA, AStarNodeAbstract* pAStarNodeB )
{
	AStarNodeNavGraph* pNodeA = (AStarNodeNavGraph*)pAStarNodeA;
	AStarNodeNavGraph* pNodeB = (AStarNodeNavGraph*)pAStarNodeB;

	// Add the cost multiplier if the poly being entered has a nav mesh link. 
	//BKJ: find the edge that connects node a and node b
	//then get its cost?

	float flMultiplierWeight = 1.0f;

	////CAINavMeshPoly* pNMPolyParent = m_pAStarMapNavMesh->GetNMPoly( pNodeB->eAStarNodeID );
	//NavGraphNode* pNavGraphNodeParent = m_pAStarMapNavMesh->GetNavGraphNode( pNodeB->eAStarNodeID );
	//
	//if (pBrain && pNavGraphNodeParent)
	//{
	//	if( pNavGraphNodeParent-> )                //->GetNMLinkID() != kNMLink_Invalid )
	//	{
	//		//AINavMeshLinkAbstract* pLink = g_pAINavMesh->GetNMLink( pNMPolyParent->GetNMLinkID() );
			//NavGraphEdge* pEdge = g_pNavGraph->GetEdge( pNMPolyParent->GetNMLinkID() );
	//		
	//		//if (pLink) //if(pEdge)
	//		//{
	//		//	flMultiplierWeight = pLink->GetNMLinkPathingWeight(pAI);
			//  flMultiplierWeight = pEdge->GetEdgeCost(); //GetEdgeCost(pBrain)
	//		//	//AIASSERT(flMultiplierWeight >= 1.0, NULL, "");
	//		//}
	//	}
	//}

	//AStarGoal cant find AStar Map?
	//ENUM_NAVGRAPH_NODE_ID eNodeAID	= AStarMapNavGraph::ConvertID_AStarNode2NavGraphNode(pNodeA->eAStarNodeID);
	//ENUM_NAVGRAPH_NODE_ID eNodeBID	= AStarMapNavGraph::ConvertID_AStarNode2NavGraphNode(pNodeB->eAStarNodeID);

	NavGraphEdge* pEdge = g_pNavGraph->GetEdge( (ENUM_NAVGRAPH_NODE_ID)pNodeA->eAStarNodeID , (ENUM_NAVGRAPH_NODE_ID)pNodeB->eAStarNodeID );
	//NavGraphEdge* pEdge = g_pNavGraph->GetEdge( eNodeAID, eNodeBID );

	if (pEdge)
	{
		flMultiplierWeight = pEdge->GetEdgeCost();
	}

	// Set the true distance from nodeA to node B.

	///return flMultiplierWeight * pNodeA->vTrueEntryPos.Dist( pNodeB->vPotentialEntryPos );
	//return flMultiplierWeight * pNodeA->vTrueEntryPos.distance( pNodeB->vPotentialEntryPos );
	return flMultiplierWeight * pNodeA->vPosition.distance( pNodeB->vPosition );
}


//Return true if node is null, or matches the destination.
bool AStarGoalNavGraph::IsAStarFinished( AStarNodeAbstract* pAStarNode )
{
	// AStar is finished, because there are no more nodes.

	if( !pAStarNode )
	{
		return true;
	}

	// AStar is finished because the goal has been reached.
	if( pAStarNode->eAStarNodeID == m_eAStarNodeDest )
	{
		return true;
	}

	// AStar is not finished.
	return false;
}

//----------------------------------------------------------------------------

//AStarStorageNavMesh       
//Create an AStarNode with a specified ID. 
AStarNodeAbstract* AStarStorageNavGraph::CreateAStarNode( ENUM_ASTAR_NODE_ID eAStarNode )
{
	// Create a node with a specified AStarNodeID.

	//CAIAStarNodeNavMesh* pNode = AI_FACTORY_NEW( CAIAStarNodeNavMesh );
	AStarNodeNavGraph* pNode = new AStarNodeNavGraph(); // more here in ctor etc?

	pNode->eAStarNodeID = eAStarNode;

	return pNode;
}

             
//Destroy an AStarNode. 
void AStarStorageNavGraph::DestroyAStarNode( AStarNodeAbstract* pAStarNode )
{
	//AI_FACTORY_DELETE( pAStarNode );

	if(pAStarNode)
	{
		delete pAStarNode;
		pAStarNode = NULL;
	}
}

//----------------------------------------------------------------------------

//AStarMapNavGraph     
//PURPOSE:	Constructor / Destructor.
AStarMapNavGraph::AStarMapNavGraph()
{
	m_pNavGraph = NULL;
	m_pAStarFlags = NULL;
	m_nAStarFlags = 0;
}

AStarMapNavGraph::~AStarMapNavGraph()
{
	//debug_deletea( m_pAStarFlags );
	if(m_pAStarFlags)
	{
		delete [] m_pAStarFlags;
		m_nAStarFlags = NULL;
	}
}

//----------------------------------------------------------------------------  

//AStarMapNavMesh
//Initialise map by setting pointer to NavGraph.          
void AStarMapNavGraph::InitAStarMapNavGraph( NavGraph* pNavGraph )
{
	// Store off a pointer to the NavGraph.
	m_pNavGraph = pNavGraph;

	// Create a flags array as big as the number of NavMesh polys.
	//AIASSERT( m_pAStarFlags == NULL, NULL, "CAIAStarMapNavMesh::InitAStarMapNavMesh : Failed to destroy m_pAStarFlags before re-initializing the CAIAStarMapNavMesh." );

	//m_nAStarFlags = m_pNavGraph->GetNumNMPolys();
	m_nAStarFlags = m_pNavGraph->GetNumNodes();  //GetNumNMEdges/Nodes?


	//m_pAStarFlags = debug_newa( unsigned char, m_nAStarFlags );
	//m_pAStarFlags = new( unsigned char, m_nAStarFlags ); //new [] m_nAStarFlags //p_var = new type [size];
	m_pAStarFlags = new unsigned char [m_nAStarFlags];


}
          
//Releases resources owned by the map, resetting it to its initial state.
void AStarMapNavGraph::TermAStarMapNavGraph( )
{
	//// Delete the Create a flags array as big as the number of NavMesh polys.
	//if( m_pAStarFlags )
	//{
	//	debug_deletea( m_pAStarFlags );
	//	m_pAStarFlags = NULL;
	//	m_nAStarFlags = 0;
	//}

	// Delete the Create a flags array as big as the number of NavMesh polys.
	if( m_pAStarFlags )
	{	
		delete [] m_pAStarFlags;
		m_pAStarFlags = NULL;
		m_nAStarFlags = 0;
	}

}

          
//Initialise the map for a search. 
void AStarMapNavGraph::InitAStarMapNavGraphSearch( unsigned int dwCharTypeMask, const Ogre::Vector3& vSource, const Ogre::Vector3& vDest )
{
	m_dwCharTypeMask = dwCharTypeMask;
	m_vPathSource	 = vSource;
	m_vPathDest		 = vDest;
	//m_nAStarFlags	 = m_pNavGraph->GetNumNMPolys(); //er GetNumNMNodes?/Edges?
	m_nAStarFlags	 = m_pNavGraph->GetNumNodes();
	
	//void * memset ( void * ptr, int value, size_t num );
	memset( m_pAStarFlags, kASTAR_Unchecked, m_nAStarFlags );

}
         
//Return number of neighbours for an AStarNode.
//This total will include edges that do not actually neighbour anyone.
int AStarMapNavGraph::GetNumAStarNeighbours( AIBrain* pBrain, AStarNodeAbstract* pAStarNode )
{
	//cast pAStarNode to a PathNode/NavGraphNode?
	//get the nodes adjacency info from the main navgraph?
	//store each nodes neighbours inside the node itself
	//NavGraph->GetNode(nodeid)

	////Find NavMeshPoly corresponding to node.
	//CAINavMeshPoly* pNMPoly = GetNMPoly( pAStarNode->eAStarNodeID );
	//
	//if( pNMPoly )
	//{
	//	// The Link determines the number of neighbors.
	//	if( pBrain && pNMPoly->GetNMLinkID() != kNMLink_Invalid )
	//	{
	//		AINavMeshLinkAbstract* pLink = g_pAINavMesh->GetNMLink( pNMPoly->GetNMLinkID() );
	//		if( pLink )
	//		{
	//			int cNeighbours;
	//			if( pLink->GetNumNMLinkNeighbours( &cNeighbours ) )
	//			{
	//				return cNeighbours;
	//			}
	//		}
	//	}

	//	// Return number of node neighbors.
	//	
	//	return pNMPoly->GetNumNMPolyEdges();
	//}


	if(pAStarNode)
	{

		NavGraphNode* pNavGraphNode = GetNavGraphNode(  pAStarNode->eAStarNodeID );

		if(pNavGraphNode)
		{
			int cNeighbours = pNavGraphNode->GetNumNodeNeighbours();
			return cNeighbours;
		}
	}

	// No corresponding NavMeshNode was found.
	return 0;
}

//Return the node ID of a specified neighbor. 
//If neighbor is an edge with no adjacent poly, return Invalid. //hmmmm?
ENUM_ASTAR_NODE_ID AStarMapNavGraph::GetAStarNeighbour( AIBrain* pBrain, AStarNodeAbstract* pAStarNode, int iNeighbour, AStarStorageAbstract* pAStarStorage )
{
	// Find NavMeshPoly corresponding to node.
	//CAINavMeshPoly* pNMPoly = GetNMPoly( pAStarNode->eAStarNodeID );
	//CAINavMeshPoly* pNMPolyNeighbor = NULL;

	//Find NavGraphNode corresponding to node.
	NavGraphNode* pNavGraphNode = GetNavGraphNode( pAStarNode->eAStarNodeID );
	NavGraphNode* pNavGraphNodeNeighbour = NULL;

	// No corresponding poly.
	//if( !pNMPoly )
	//{
	//	return kASTARNODE_Invalid;
	//}

	//No corresponding node.
	if( !pNavGraphNode )
	{
		return kASTARNODE_Invalid;
	}

	// The Link determines the neighboring poly.
	//BKJ: not sure about this bit tbh
	bool bSetNeighbour = false;
	
	//if( pAI && pNMPoly->GetNMLinkID() != kNMLink_Invalid )
	//{
	//	ENUM_NMLinkID eLink = pNMPoly->GetNMLinkID();
	//	AINavMeshLinkAbstract* pLink = g_pAINavMesh->GetNMLink( eLink );
	//	if( pLink )
	//	{
	//		// Check this Link, because paths are searched from dest to source.

	//		if( !pLink->IsNMLinkEnabledToAI( pAI, LINK_CHECK_TIMEOUT ) )
	//		{
	//			return kASTARNODE_Invalid;
	//		}

	//		CAINavMeshPoly* pNMPolyParent = NULL;
	//		if( pAStarNode->pAStarParent )
	//		{
	//			pNMPolyParent = GetNMPoly( pAStarNode->pAStarParent->eAStarNodeID );
	//		}
	//		bSetNeighbor = pLink->GetNMLinkNeighborAtEdge( this, iNeighbor, pNMPolyParent, pNMPolyNeighbor );
	//	}
	//}

	//GetNMLinkNeighborAtEdge - Returns true if we found a pointer to the NavMesh poly neighbor at a specified index.

	if(pBrain && pNavGraphNode->GetNavGraphNodeID() != kNGNodeID_Invalid )
	{
		//
		NavGraphNode* pNavGraphNodeParent = NULL;
		
		if(pAStarNode->pAStarParent)
		{
			pNavGraphNodeParent = GetNavGraphNode( pAStarNode->pAStarParent->eAStarNodeID );
		}

		//BKJ: TODO figure this lot out 
		//bSetNeighbour = 
	}

	// Find a neighboring poly at the specified edge index.
	// Find a neighboring node at the specified edge index.
	if( !bSetNeighbour )
	{
		//pNMPolyNeighbor = pNMPoly->GetNMPolyNeighborAtEdge( iNeighbor );
		pNavGraphNodeNeighbour = pNavGraphNode->GetNavGraphNodeNeighbour( (ENUM_NAVGRAPH_EDGE_ID)iNeighbour );  //get node neighbour from edge id  i think

	}

	// No neighbor poly exists on the edge with the specified index.
	if( !pNavGraphNodeNeighbour )
	{
		return kASTARNODE_Invalid;
	}

	// Neighbor does not have matching character type restrictions.
	//if( !( pNMPolyNeighbor->GetNMCharTypeMask() & m_dwCharTypeMask ) )
	//{
	//	return kASTARNODE_Invalid;
	//}

	//// The poly's NavMeshLink may prevent the poly from being passable.
	//ENUM_NMLinkID eNeighborLink = pNMPolyNeighbor->GetNMLinkID();
	//if( pAI && eNeighborLink != kNMLink_Invalid )
	//{
	//	AINavMeshLinkAbstract* pNeighborLink = g_pAINavMesh->GetNMLink( eNeighborLink );
	//	if( pNeighborLink )
	//	{
	//		if( !pNeighborLink->IsLinkPassable( pAI, pNMPoly->GetNMPolyID() ) )
	//		{
	//			return kASTARNODE_Invalid;
	//		}
	//	}
	//}

	//get edge and check if its passable?
	//

	// Poly is passable, so return its node ID.
	//return ConvertID_NMPoly2AStarNode( pNMPolyNeighbor->GetNMPolyID() );

	return ConvertID_NavGraphNode2AStarNode( pNavGraphNodeNeighbour->GetNavGraphNodeID() );
}
     
//Setup parent / child relationship. with a potential neighbour?
void AStarMapNavGraph::SetupPotentialNeighbour( AStarNodeAbstract* pAStarNodeParent, AStarNodeAbstract* pAStarNodeChild )
{
	AStarNodeNavGraph* pNodeParent = (AStarNodeNavGraph*)pAStarNodeParent;
	AStarNodeNavGraph* pNodeChild  = (AStarNodeNavGraph*)pAStarNodeChild;

	// Child has no parent.
	if( !pNodeParent )
	{
		//pNodeChild->vPotentialEntryPos = m_vPathSource; //hmm?
		//pNodeChild->vTrueEntryPos = m_vPathSource;      //hmm?
		pNodeChild->vPosition = m_vPathSource;
		return;
	}

	// Can't find parent.
	//CAINavMeshPoly* pNMPolyParent = GetNMPoly( pNodeParent->eAStarNodeID );
	NavGraphNode* pNavGraphNodeParent = GetNavGraphNode( pNodeParent->eAStarNodeID );

	if( !pNavGraphNodeParent )
	{
		//pNodeChild->vPotentialEntryPos = m_vPathSource;
		//pNodeChild->vTrueEntryPos = m_vPathSource;
		pNodeChild->vPosition = m_vPathSource;

		return;
	}

	// Parent is a NavMeshLink. Let the Link set the entry pos.
	//ENUM_NMPolyID eChild = ConvertID_AStarNode2NMPoly( pNodeChild->eAStarNodeID );
	//if( pNMPolyParent && ( pNMPolyParent->GetNMLinkID() != kNMLink_Invalid ) )
	//{
	//	ENUM_NMLinkID eLink = pNMPolyParent->GetNMLinkID();
	//	AINavMeshLinkAbstract* pLink = g_pAINavMesh->GetNMLink( eLink );
	//	if( pLink )
	//	{
	//		LTVector vEntry;
	//		if( pLink->GetNMLinkOffsetEntryPos( eChild, pNodeParent->vTrueEntryPos, &vEntry ) )
	//		{
	//			pNodeChild->vPotentialEntryPos = vEntry;
	//			return;
	//		}
	//	}
	//}

	ENUM_NAVGRAPH_NODE_ID eChild = ConvertID_AStarNode2NavGraphNode( pNodeChild->eAStarNodeID );

	//if(pNavGraphNodeParent && pNavGraphNodeParent->GetNavGraphNodeID() != kNGNodeID_Invalid )
	//{
//
//	}


	// Default behavior.
	//CAINavMeshEdge* pEdge = pNMPolyParent->GetNMPolyNeighborEdge( eChild );
	NavGraphEdge* pEdge = g_pNavGraph->GetEdge( pNavGraphNodeParent->GetNavGraphNodeID(),  eChild ); //get edge from opposite (m_pTo) node id
	//NavGraphEdge* pEdge = pNavGraphNodeParent->GetEdgeFromNavGraphNodeID( eChild ); //get edge from opposite (m_pTo) node id

	if( pEdge )
	{
		//pNodeChild->vPotentialEntryPos = pEdge->GetNMEdgeMidPt();
		//pNodeChild->vPosition = pEdge->GetEdgeMidPoint(); //hmms?

	}
}

   
//Finalise a parent / child relationship. (remove?)
void AStarMapNavGraph::FinaliseNeighbour( AStarNodeAbstract* pAStarNodeChild )
{
	AStarNodeNavGraph* pNodeChild = (AStarNodeNavGraph*)pAStarNodeChild;
	
	//pNodeChild->vTrueEntryPos = pNodeChild->vPotentialEntryPos;
	//pNodeChild->vTrueEntryPos = pNodeChild->vPotentialEntryPos;
}
           

//Get the midpoint of a specified NavGraph Edge.
bool AStarMapNavGraph::GetNavGraphEdgeMidPt( ENUM_ASTAR_NODE_ID eAStarNodeA, ENUM_ASTAR_NODE_ID eAStarNodeB, Ogre::Vector3* pvMidPt )
{
	// Find NavMeshPoly corresponding to nodeA.
	//CAINavMeshPoly* pNMPolyA = GetNMPoly( eAStarNodeA );
	NavGraphNode* pNavGraphNodeA = GetNavGraphNode( eAStarNodeA );
	
	if( !pNavGraphNodeA )
	{
		return false;
	}

	// Find NavMeshEdge between nodeA and nodeB.
	//CAINavMeshEdge*	pEdge = pNMPolyA->GetNMPolyNeighborEdge( ConvertID_AStarNode2NMPoly( eAStarNodeB ) );
	NavGraphEdge*	pEdge = pNavGraphNodeA->GetEdgeFromNavGraphNodeID( ConvertID_AStarNode2NavGraphNode( eAStarNodeB ) );

	if( pEdge )
	{
		// Return the mid-point of the edge.
		//*pvMidPt = pEdge->GetNMEdgeMidPt();

		*pvMidPt = pEdge->GetEdgeMidPoint();

		return true;
	}

	// No edge exists between nodeA and nodeB.
	return false;
}

    
//Set AStar flags for a specified node.
void AStarMapNavGraph::SetAStarFlags( ENUM_ASTAR_NODE_ID eAStarNode, unsigned long dwFlags, unsigned long dwMask )
{
	// Set flags if nodeID is valid.
	if( ( eAStarNode != kASTARNODE_Invalid ) && ( eAStarNode < m_pNavGraph->GetNumNodes() )  )
	{
		m_pAStarFlags[eAStarNode] &= ~dwMask;
		m_pAStarFlags[eAStarNode] |= dwFlags;
	}
}
            
//Get AStar flags for a specified node.
unsigned long AStarMapNavGraph::GetAStarFlags( ENUM_ASTAR_NODE_ID eAStarNode )
{
	// Return flags if nodeID is valid.
	if( ( eAStarNode != kASTARNODE_Invalid ) && ( eAStarNode < m_pNavGraph->GetNumNodes() ) )
	{
		return m_pAStarFlags[eAStarNode];
	}

	return kASTAR_Unchecked;
}


       