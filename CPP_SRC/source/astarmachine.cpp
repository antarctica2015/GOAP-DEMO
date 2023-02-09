

#include "astarmachine.h"
#include "agent.h"
#include "utils.h"

// Generic A* Machine Implementation

//From FEAR SDK:

//THE A* MACHINE:
//The A* machine is a generic machine that runs the A* search algorithm. The
//A* machine is composed 3 components: A*Storage, A*Goal, and A*Map. This 
//architecture allows for the creation of multiple A* machines for different
//purposes.  
//
//For example, an A* machine may find shortest paths on a NavGraph 
//if provided with an A* Map wrapped around a NavGraph, and an A*Goal
//that can estimate and measure distances between locations on a NavGraph.
//
//A*Storage:
//The A*Storage contains the Open and Closed lists. The implementaion of the 
//lists is up to the derived A*Storage class. Lists may be linked lists, static
//arrays or something else.
//
//A*Goal:
//The A*Goal determines when the algorithm has completed (successfully or 
//unsuccessfully), and the estimated and actual distances between A*Nodes.
//
//A*Map:
//The A*Map determines how to correlate A* NodeIDs with locations on 
//a map of any kind. The map holds flags with the status of each A* node.
//
//This architecture is based on the A* machine described in "Generic A* 
//Pathfinding", AI Game Programming Wisdom, p.114 (by Dan Higgins)


// AStarNodeAbstract
AStarNodeAbstract::AStarNodeAbstract()
{
	eAStarNodeID = kASTARNODE_Invalid; 

	fGoal = 0.0f;
	fHeuristic = 0.0f;
	fFitness = FLT_MAX;

	pListPrev = NULL;
	pListNext = NULL;
	pAStarParent = NULL;
}

AStarNodeAbstract::~AStarNodeAbstract()
{
	//
}




// AStarMachine
//Create/destroy the A* machine.
AStarMachine::AStarMachine()
{
	m_pAgent = NULL;

	m_pAStarStorage = NULL;
	m_pAStarGoal = NULL;
	m_pAStarMap = NULL;

	m_pAStarNodeCurrent = NULL;

	m_eAStarNodeSource = kASTARNODE_Invalid;
	m_eAStarNodeTarget = kASTARNODE_Invalid;
}

AStarMachine::~AStarMachine()
{
	//m_pAgent = NULL;
}


//Initialize the A* machine by setting the storage, goal, and map components.
void AStarMachine::InitAStarMachine(Agent* pAgent,AStarStorageAbstract* pAStarStorage, AStarGoalAbstract* pAStarGoal, AStarMapAbstract* pAStarMap )
{
	m_pAgent = pAgent;
	m_pAStarStorage = pAStarStorage;
	m_pAStarGoal = pAStarGoal;
	m_pAStarMap = pAStarMap;
}


//Set the A* machine's source node. This resets the machine to start a new search.
void AStarMachine::SetAStarSource( ENUM_ASTAR_NODE_ID eAStarNodeSource )
{
	// Reset the storage for a new search.
	m_pAStarStorage->ResetAStarStorage();
	m_eAStarNodeSource = eAStarNodeSource;
}


//Set the A* machine's target goal node.
void AStarMachine::SetAStarTarget( ENUM_ASTAR_NODE_ID eAStarNodeTarget )
{
	// Set the A* goal's destination (target) node.
	m_pAStarGoal->SetTargetNode( eAStarNodeTarget );
	m_eAStarNodeTarget = eAStarNodeTarget;
}

//From FEAR SDK:
//----------------------------------------------------------------------------
//              
//	ROUTINE:	AStarMachine::RunAStar
//              
//	PURPOSE:	Run the A* algorithm.
//
//  The A* Algorithm (from Artificial Intelligence: A New Synthesis, p. 144 ):
// 
//    g(n) = cost of best path from n0 to n.
//    h(n) = best guess (heuristic) of cost of path from n to a goal.
//    f(n) = g(n) + h(n) = cost of lowest-cost path to a goal - contrained
//           to go through node n.
//
//  1. Create a search graph G, consisting solely of the start node, n0. 
//     Put n0 on a list called OPEN.
//  2. Create a list called CLOSED that is initially empty.
//  3. If OPEN is empty, exit with failure.
//  4. Select the first node on OPEN, remove it from OPEN, and put it on CLOSED. 
//     Call this node n.
//  5. If n is a goal node, exit successfully with the solution obtained by tracing 
//     a path along the pointers from n to n0 in G. (The pointers define a search tree 
//     and are established in step 7).
//  6. Expand node n, generating the set M, of its successors that are not already 
//     ancestors of n in G. Install these members of M as successors of n in G.
//  7. Establish a pointer to n from each of those members of M that were not already 
//     in G (ie not already on either OPEN or CLOSED). Add these members of M to OPEN. 
//     For each member m of M that was already on OPEN or CLOSED, redirect its pointer 
//     to n if the best path to m found so far is through n. For each member of M already 
//     on CLOSED, redirect the pointers of each of its decendants in G so that they point 
//     backward along the best paths found so far to these descendants.
//  8. Reorder the list OPEN in order of increasing f values. (Ties among minimal f values 
//     are resolved in favour of the deepest node in the search tree).
//  9. Go to step 3.
//
//  NOTE: For step 7, we re-insert CLOSED nodes back into the OPEN list 
//        rather than re-directing the pointers of its decendants.
//        Less book keeping.
//             
//----------------------------------------------------------------------------

void AStarMachine::RunAStar()
{
	std::cout<< "Running A Star." << std::endl;

	//Sanity check.
	if(!m_pAgent)
	{
		return;
		//error message
	}

	int cNeighbours, iNeighbour;
	AStarNodeAbstract* pAStarNodeNeighbour;
	ENUM_ASTAR_NODE_ID eAStarNeighbourNode;
	unsigned long dwFlags = 0;
	float g, h, f; //

	// Step 1:

	// Add source node to the Open list.
	// The source will be the only node in the Open list so far.
	m_pAStarNodeCurrent = m_pAStarStorage->CreateAStarNode( m_eAStarNodeSource );
	
	if( !m_pAStarNodeCurrent )
	{
		return;
	}

	m_pAStarStorage->AddToOpenList( m_pAStarNodeCurrent, m_pAStarMap );

	// Setup first node.
	m_pAStarMap->SetupPotentialNeighbour( NULL, m_pAStarNodeCurrent );

	// Set f, g, and h for source node.
	h = m_pAStarGoal->GetHeuristicDistance( m_pAStarNodeCurrent );
	m_pAStarNodeCurrent->fGoal = 0.0f;
	m_pAStarNodeCurrent->fHeuristic = h;
	m_pAStarNodeCurrent->fFitness = h;

	std::cout<< "F: "<< m_pAStarNodeCurrent->fGoal << " G: " << m_pAStarNodeCurrent->fHeuristic << " H: " << m_pAStarNodeCurrent->fFitness << std::endl;

	// Step 2 is taken care of by the A* storage's ResetAStarStorage().

	// Infinite loop, broken when A* goal determines that the algorithm has terminated.
	while( true )
	{
		std::cout<< "In A Star Loop." << std::endl;
		
		// Steps 3 and 4:

		// Remove the cheapest node found in the Open list, and explore the node's Neighbours.
		m_pAStarNodeCurrent = m_pAStarStorage->RemoveCheapestOpenNode();

		// Add the current node to the Closed list.
		if( m_pAStarNodeCurrent )
		{
			m_pAStarNodeCurrent->DebugPrintExpand(); //no logging yet
			m_pAStarStorage->AddToClosedList( m_pAStarNodeCurrent, m_pAStarMap );
		}

		// Step 5:

		// The goal determines if A* is finished:
		// A* terminates when the goal has been reached, or there are no more nodes to explore.

		if( m_pAStarGoal->IsAStarFinished( m_pAStarNodeCurrent ) )
		{
			break;
		}

		// Step 6:

		// Iterate over neighbours of the current node.
		cNeighbours = m_pAStarMap->GetNumAStarNeighbours( m_pAgent, m_pAStarNodeCurrent );
		
		for( iNeighbour = 0; iNeighbour < cNeighbours; ++iNeighbour )
		{
			// Get the A* nodeID of the neighbouring node.
			eAStarNeighbourNode = m_pAStarMap->GetAStarNeighbour( m_pAgent, m_pAStarNodeCurrent, iNeighbour, m_pAStarStorage );
			
			// No neighbour exists with this index.
			if( eAStarNeighbourNode == kASTARNODE_Invalid )
			{
				continue;
			}

			// Step 7:

			// Get the A* flags for the neighbouring node.
			dwFlags = m_pAStarMap->GetAStarFlags( eAStarNeighbourNode );

			// The node is already open, so get it from the Open list.
			if( dwFlags & AStarMapAbstract::kASTAR_Open )
			{
				pAStarNodeNeighbour = m_pAStarStorage->FindInOpenList( eAStarNeighbourNode );
			}

			// The node is already closed, so get it from the Closed list.
			else if( dwFlags & AStarMapAbstract::kASTAR_Closed )
			{
				pAStarNodeNeighbour = m_pAStarStorage->FindInClosedList( eAStarNeighbourNode );
			}

			// The node is not on any of the lists, so create a new node with the 
			// neighbour's A* nodeID if the goal determines that it is passable.
			else if( m_pAStarGoal->IsAStarNodePassable( eAStarNeighbourNode ) )
			{
				pAStarNodeNeighbour = m_pAStarStorage->CreateAStarNode( eAStarNeighbourNode );
				pAStarNodeNeighbour->DebugPrintNeighbour();// not yet
			}

			// Neighbour is not passable.
			else 
			{
				continue;
			}

			// Setup potential neighbour node.
			m_pAStarMap->SetupPotentialNeighbour( m_pAStarNodeCurrent, pAStarNodeNeighbour );

			// If our current best path is through this neighbour,
			// we do not need to re-assess the neighbour.
			if( m_pAStarNodeCurrent->pAStarParent == pAStarNodeNeighbour )
			{
				continue;
			}

			// Calculate new f, g, and h values for the neighbour node.

			g = m_pAStarNodeCurrent->fGoal;
			g += m_pAStarGoal->GetActualCost(m_pAStarNodeCurrent, pAStarNodeNeighbour );
			h = m_pAStarGoal->GetHeuristicDistance( pAStarNodeNeighbour );
			f = g + h; //important!
		
			std::cout<< "New F: "<< f << " G: " << g << " H: " << h << std::endl;

			// Bail from processing this neighbour if it is more expensive to get
			// to this neighbour node from the current node than from its previous 
			// parent.

			
			// To prevent infinite loops caused by floating point error, 
			// f must be more than a floating point epsilon smaller than
			// the stored fitness.  This is because f will be higher precision
			// as it is stored in a registered, where as the stored fitness value
			// was stored as a float.
			// Just to be extra safe, we are using a value larger than FLT_EPSILON. 

			const float fPathingEpsilon = 1e-6f;  
			
			if( f >= pAStarNodeNeighbour->fFitness*(1. - fPathingEpsilon) )
			{
				continue;
			}

			// Assign new A* values to the Neighbour node.
			pAStarNodeNeighbour->fGoal = g;
			pAStarNodeNeighbour->fHeuristic = h;
			pAStarNodeNeighbour->fFitness = g + h;

			// If the node was previously closed, remove it from the Closed list,
			// because it will be re-opened to re-explore its neighbours.
			if( dwFlags & AStarMapAbstract::kASTAR_Closed )
			{
				m_pAStarStorage->RemoveFromClosedList( pAStarNodeNeighbour );
			}

			// Add neighbour node to the Open list, and set the neighbour's parent as the current node.
			m_pAStarStorage->AddToOpenList( pAStarNodeNeighbour, m_pAStarMap );
			pAStarNodeNeighbour->pAStarParent = m_pAStarNodeCurrent;
			
			// Finalise neighbour node setup.
			m_pAStarMap->FinaliseNeighbour( pAStarNodeNeighbour );
		}

		// Step 8 is taken care of the A* storage.
		// The implemenation A* storage may choose to sort nodes
		// on insertion, or do a search for the cheapest node.
	}
}

//----------------------------------------------------------------------------