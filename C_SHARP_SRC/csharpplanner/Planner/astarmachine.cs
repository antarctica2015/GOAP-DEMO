
//------------------------------------------------------------------------
//
//  Name:   astarmachine.cs
//
//  Desc:   A* machine declaration & abstract classes for components
//			based on Higgins(AI Wisdom 1) article.
//
//  Author: Brett Keir Jones 2015
//
//------------------------------------------------------------------------

using System;
using NSAgent;

namespace NSAStarMachine
{
    public enum ENUM_ASTAR_NODE_ID
    {
        kASTARNODE_Invalid = -1,
    };

    public abstract class AStarNodeAbstract 
    {
        public ENUM_ASTAR_NODE_ID eAStarNodeID;

        public float fGoal;
        public float fHeuristic;
        public float fFitness;

        public AStarNodeAbstract prevNode;
        public AStarNodeAbstract nextNode;
        public AStarNodeAbstract aStarParent;

        //
        protected AStarNodeAbstract()
        {
            eAStarNodeID = ENUM_ASTAR_NODE_ID.kASTARNODE_Invalid;

            fGoal = 0.0f;
            fHeuristic = 0.0f;
            fFitness = float.MaxValue;

            prevNode = null;
            nextNode = null;
            aStarParent = null;
        }

    }


    public abstract class AStarGoalAbstract
    {
       public abstract void     SetTargetNode(ENUM_ASTAR_NODE_ID eAStarNode);
	   public abstract float	GetHeuristicDistance(AStarNodeAbstract aStarNode);
	   public abstract float	GetActualCost(AStarNodeAbstract aStarNodeA, AStarNodeAbstract aStarNodeB);
	   public abstract bool 	IsAStarFinished(AStarNodeAbstract aStarNode);

	   public virtual bool	IsAStarNodePassable(ENUM_ASTAR_NODE_ID eAStarNode) { return true; }
	   public virtual bool	IsAStarNodePassable(AStarNodeAbstract aStarNode) { return true; }
    
    }

    //a star storage class
    public abstract class AStarStorageAbstract
    {
        public abstract AStarNodeAbstract   CreateAStarNode(ENUM_ASTAR_NODE_ID eAStarNode);
	    public abstract  void				DestroyAStarNode(AStarNodeAbstract aStarNode);

	    public abstract  void	ResetAStarStorage();
	    public abstract  void	AddToOpenList(AStarNodeAbstract aStarNode, AStarMapAbstract aStarMap);
	    public abstract  void	AddToClosedList(AStarNodeAbstract aStarNode, AStarMapAbstract aStarMap);
	    public abstract  void	RemoveFromOpenList(AStarNodeAbstract aStarNode);
	    public abstract  void	RemoveFromClosedList(AStarNodeAbstract aStarNode);

	    public abstract  AStarNodeAbstract	FindInOpenList(ENUM_ASTAR_NODE_ID eAStarNode);
	    public abstract  AStarNodeAbstract	FindInClosedList(ENUM_ASTAR_NODE_ID eAStarNode);

	    public abstract  AStarNodeAbstract	RemoveCheapestOpenNode();
    };



    // The Astar Map determines how to correlate A* NodeIDs with locations on a map of any kind.
    public abstract class AStarMapAbstract
    {
        public enum ENUM_ASTAR_FLAGS
	    {
		        kASTAR_Unchecked	= 0x00,
		        kASTAR_Open			= 0x01,
		        kASTAR_Closed		= 0x02,
		        kASTAR_OpenOrClosed	= kASTAR_Open | kASTAR_Closed,
		        kASTAR_NotPassable	= 0x04,
	    };

	    public abstract int					GetNumAStarNeighbours(Agent agent, AStarNodeAbstract aStarNode);
	    public abstract ENUM_ASTAR_NODE_ID	GetAStarNeighbour(Agent agent, AStarNodeAbstract aStarNode, int iNeighbour, AStarStorageAbstract aStarStorage);

	    public abstract void				SetAStarFlags(ENUM_ASTAR_NODE_ID eAStarNode, ulong dwFlags, ulong dwMask);
	    public abstract ulong		        GetAStarFlags(ENUM_ASTAR_NODE_ID eAStarNode);

        public virtual void                 SetupPotentialNeighbour(AStarNodeAbstract aStarNodeParent, AStarNodeAbstract aStarNodeChild) { }
        public virtual void                 FinaliseNeighbour(AStarNodeAbstract aStarNodeChild) { }
    };


    public class AStarMachine
    {
	    protected Agent		            m_Agent;

        protected AStarStorageAbstract  m_AStarStorage;
        protected AStarGoalAbstract     m_AStarGoal;
        protected AStarMapAbstract      m_AStarMap;

        protected AStarNodeAbstract     m_AStarNodeCurrent;

        protected ENUM_ASTAR_NODE_ID    m_eAStarNodeSource;
        protected ENUM_ASTAR_NODE_ID    m_eAStarNodeTarget;


        // Methods etc
        public AStarMachine() 
        {
            m_Agent = null;

            m_AStarStorage = null;
            m_AStarGoal = null;
            m_AStarMap = null;

            m_AStarNodeCurrent = null;

            m_eAStarNodeSource = ENUM_ASTAR_NODE_ID.kASTARNODE_Invalid;
            m_eAStarNodeTarget = ENUM_ASTAR_NODE_ID.kASTARNODE_Invalid;
        
        }

        //data access
        public AStarNodeAbstract GetAStarNodeCurrent() { return m_AStarNodeCurrent; }
        public AStarStorageAbstract GetAStarStorage() { return m_AStarStorage; }
        public AStarMapAbstract GetAStarMap() { return m_AStarMap; }

        // void InitAStarMachine( Agent* pAgent, AStarStorageAbstract* pAStarStorage, AStarGoalAbstract* pAStarGoal, AStarMapAbstract* pAStarMap);
        // Initialize the A* machine by setting the storage, goal, and map components.
        // public void InitAStarMachine(Agent agent, ref AStarStorageAbstract aStarStorage, ref AStarGoalAbstract aStarGoal, ref AStarMapAbstract aStarMap) 
        public void InitAStarMachine(Agent agent, AStarStorageAbstract aStarStorage,  AStarGoalAbstract aStarGoal, AStarMapAbstract aStarMap) 
     
        {
            m_Agent = agent;
            m_AStarStorage = aStarStorage;
            m_AStarGoal = aStarGoal;
            m_AStarMap = aStarMap;
        }

        // Set the A* machine's source node. This resets the machine to start a new search.
        public void SetAStarSource(ENUM_ASTAR_NODE_ID eAStarNodeSource) 
        {
            // Reset the storage for a new search.
            m_AStarStorage.ResetAStarStorage();
            m_eAStarNodeSource = eAStarNodeSource;
        }

        // Set the A* machine's target goal node.
        public void SetAStarTarget(ENUM_ASTAR_NODE_ID eAStarNodeTarget)
        {
            // Set the A* goal's destination (target) node.
            m_AStarGoal.SetTargetNode(eAStarNodeTarget);
            m_eAStarNodeTarget = eAStarNodeTarget;
        }

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
        public void RunAStar()
        {
            Console.WriteLine("Running A Star");

	        //Sanity check.
	        if(m_Agent == null)
	        {
		        return;
	        }

	        int cNeighbours, iNeighbour;
	        AStarNodeAbstract aStarNodeNeighbour;
	        ENUM_ASTAR_NODE_ID eAStarNeighbourNode;
	        ulong dwFlags = 0;
	        float g, h, f; 

	        // Step 1:

	        // Add source node to the Open list.
	        // The source will be the only node in the Open list so far.
	        m_AStarNodeCurrent = m_AStarStorage.CreateAStarNode(m_eAStarNodeSource);
	
	        if(m_AStarNodeCurrent == null)
	        {
		        return;
	        }

	        m_AStarStorage.AddToOpenList(m_AStarNodeCurrent, m_AStarMap);

	        // Setup first node.
	        m_AStarMap.SetupPotentialNeighbour(null, m_AStarNodeCurrent);

	        // Set f, g, and h for source node.
	        h = m_AStarGoal.GetHeuristicDistance(m_AStarNodeCurrent);
	        m_AStarNodeCurrent.fGoal = 0.0f;
	        m_AStarNodeCurrent.fHeuristic = h;
	        m_AStarNodeCurrent.fFitness = h;

	        Console.WriteLine("F:" + m_AStarNodeCurrent.fGoal + " G:" + m_AStarNodeCurrent.fHeuristic + " H:" + m_AStarNodeCurrent.fFitness );

	        // Step 2 is taken care of by the A* storage's ResetAStarStorage().

	        // Infinite loop, broken when A* goal determines that the algorithm has terminated.
	        while( true )
	        {
		        Console.WriteLine("In main A Star loop");
		
		        // Steps 3 and 4:

		        // Remove the cheapest node found in the Open list, and explore the node's Neighbours.
		        m_AStarNodeCurrent = m_AStarStorage.RemoveCheapestOpenNode();

		        // Add the current node to the Closed list.
		        if(m_AStarNodeCurrent != null)
		        {
			       // m_AStarNodeCurrent.DebugPrintExpand(); //not implemented yet.
			        m_AStarStorage.AddToClosedList(m_AStarNodeCurrent, m_AStarMap);
		        }

		        // Step 5:

		        // The goal determines if A* is finished:
		        // A* terminates when the goal has been reached, or there are no more nodes to explore.

		        if(m_AStarGoal.IsAStarFinished(m_AStarNodeCurrent))
		        {
			        break;
		        }

		        // Step 6:

		        // Iterate over neighbours of the current node.
		        cNeighbours = m_AStarMap.GetNumAStarNeighbours(m_Agent, m_AStarNodeCurrent);
		
		        for(iNeighbour = 0; iNeighbour < cNeighbours; ++iNeighbour)
		        {
			        // Get the A* nodeID of the neighbouring node.
			        eAStarNeighbourNode = m_AStarMap.GetAStarNeighbour(m_Agent, m_AStarNodeCurrent, iNeighbour, m_AStarStorage);
			
			        // No neighbour exists with this index.
			        if(eAStarNeighbourNode == ENUM_ASTAR_NODE_ID.kASTARNODE_Invalid)
			        {
				        continue;
			        }

			        // Step 7:

			        // Get the A* flags for the neighbouring node.
			        dwFlags = m_AStarMap.GetAStarFlags(eAStarNeighbourNode);

			        // The node is already open, so get it from the Open list.
                    // TODO: might need to change how the flags are used here??
			        if ((dwFlags & (ulong)AStarMapAbstract.ENUM_ASTAR_FLAGS.kASTAR_Open) == (ulong)AStarMapAbstract.ENUM_ASTAR_FLAGS.kASTAR_Open)
			        {
				        aStarNodeNeighbour = m_AStarStorage.FindInOpenList(eAStarNeighbourNode);
			        }

			        // The node is already closed, so get it from the Closed list.
                    // TODO: seems wrong
                    else if ((dwFlags & (ulong)AStarMapAbstract.ENUM_ASTAR_FLAGS.kASTAR_Closed) == (ulong)AStarMapAbstract.ENUM_ASTAR_FLAGS.kASTAR_Closed)
			        {
				        aStarNodeNeighbour = m_AStarStorage.FindInClosedList(eAStarNeighbourNode);
			        }

			        // The node is not on any of the lists, so create a new node with the 
			        // neighbour's A* nodeID if the goal determines that it is passable.
			        else if(m_AStarGoal.IsAStarNodePassable(eAStarNeighbourNode))
			        {
				        aStarNodeNeighbour = m_AStarStorage.CreateAStarNode(eAStarNeighbourNode);
				        //aStarNodeNeighbour.DebugPrintNeighbour();// not yet
			        }

			        // Neighbour is not passable.
			        else 
			        {
				        continue;
			        }

			        // Setup potential neighbour node.
			        m_AStarMap.SetupPotentialNeighbour(m_AStarNodeCurrent, aStarNodeNeighbour);

			        // If our current best path is through this neighbour,
			        // we do not need to re-assess the neighbour.
			        if(m_AStarNodeCurrent.aStarParent == aStarNodeNeighbour)
			        {
				        continue;
			        }

			        // Calculate new f, g, and h values for the neighbour node.

			        g = m_AStarNodeCurrent.fGoal;
			        g += m_AStarGoal.GetActualCost(m_AStarNodeCurrent, aStarNodeNeighbour);
			        h = m_AStarGoal.GetHeuristicDistance(aStarNodeNeighbour);
			        
                    //
                    f = g + h; //important!
                    //

                    Console.WriteLine("New F:{0} G:{1} H:{2}", f,g,h);

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
			
			        if( f >= aStarNodeNeighbour.fFitness*(1.0 - fPathingEpsilon) )
			        {
				        continue;
			        }

			        // Assign new A* values to the Neighbour node.
			        aStarNodeNeighbour.fGoal = g;
			        aStarNodeNeighbour.fHeuristic = h;
			        aStarNodeNeighbour.fFitness = g + h;

			        // If the node was previously closed, remove it from the Closed list,
			        // because it will be re-opened to re-explore its neighbours.
                    // TODO: not sure this is correct at all here
			        if((dwFlags & (ulong)AStarMapAbstract.ENUM_ASTAR_FLAGS.kASTAR_Closed) == (ulong)AStarMapAbstract.ENUM_ASTAR_FLAGS.kASTAR_Closed)
			        {
				        m_AStarStorage.RemoveFromClosedList(aStarNodeNeighbour);
			        }

			        // Add neighbour node to the Open list, and set the neighbour's parent as the current node.
			        m_AStarStorage.AddToOpenList(aStarNodeNeighbour, m_AStarMap);
			        aStarNodeNeighbour.aStarParent = m_AStarNodeCurrent;
			
			        // Finalise neighbour node setup.
			        m_AStarMap.FinaliseNeighbour( aStarNodeNeighbour );
		        }

		        // Step 8 is taken care of the A* storage.
		        // The implemenation A* storage may choose to sort nodes
		        // on insertion, or do a search for the cheapest node.
	        }

        }

    };

}