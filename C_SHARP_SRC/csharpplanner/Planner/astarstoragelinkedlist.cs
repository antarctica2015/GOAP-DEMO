
//------------------------------------------------------------------------
//
//  Name:   astarstoragelinkedlist.cs
//
//  Desc: AStar Storage class using a linked list.
//
//  Author: Brett Keir Jones 2015
//
//------------------------------------------------------------------------

using System;
using NSAStarMachine;

namespace NSAStarStorageLinkedList {

    public class AStarStorageLinkedList : AStarStorageAbstract
    {
        public AStarStorageLinkedList()
        {
            m_OpenListHead = null;
            m_ClosedListHead = null;
        }

        ~AStarStorageLinkedList()
        {
            ResetAStarStorage();
        }

        // AStarStorageAbstract required methods.

        //Create an AStarNode with a specified ID. //TODO: fix this
        public override AStarNodeAbstract CreateAStarNode(ENUM_ASTAR_NODE_ID eAStarNode) { return null; }

        public override void DestroyAStarNode(AStarNodeAbstract aStarNode) { }

        public override void ResetAStarStorage()
        {
            // Delete nodes from both lists.
            ClearList(ref m_OpenListHead);
            ClearList(ref m_ClosedListHead);
        }

        public override void AddToOpenList(AStarNodeAbstract aStarNode, AStarMapAbstract aStarMap)
        {
            // Bail is node is already in Open list.
	        ulong dwFlags = aStarMap.GetAStarFlags(aStarNode.eAStarNodeID);
            
            //TODO: remove
            Console.WriteLine("result of bitwise & = {0}",dwFlags & (ulong)AStarMapAbstract.ENUM_ASTAR_FLAGS.kASTAR_Open);
	
            //TODO: find a way to do bitwise AND on ulong
	       // if(dwFlags & (ulong)AStarMapAbstract.ENUM_ASTAR_FLAGS.kASTAR_Open)
	        //{
		     //   return;
	        //}

	        // Add node to Open list, and set AStar flags.
	        AddToList(ref m_OpenListHead, aStarNode);
	       // aStarMap.SetAStarFlags(aStarNode.eAStarNodeID, (ulong)AStarMapAbstract.ENUM_ASTAR_FLAGS.kASTAR_Open, (ulong)AStarMapAbstract.ENUM_ASTAR_FLAGS.kASTAR_OpenOrClosed );
        }


        public override void AddToClosedList(AStarNodeAbstract aStarNode, AStarMapAbstract aStarMap)
        {
            // The AStar algorithm will never try to insert an already closed node in the Closed list, 
	        // so no need to check for it.

	        // Add node to Closed list, and set AStar flags.
	        AddToList(ref m_ClosedListHead, aStarNode);
	        aStarMap.SetAStarFlags(aStarNode.eAStarNodeID, (ulong)AStarMapAbstract.ENUM_ASTAR_FLAGS.kASTAR_Closed, (ulong)AStarMapAbstract.ENUM_ASTAR_FLAGS.kASTAR_OpenOrClosed );
        }

        public override void RemoveFromOpenList(AStarNodeAbstract aStarNode)
        {
            // No need to clear AStar flags, because when node is inserted into the Closed list,
            // flags are cleared and reset.
            RemoveFromList(ref m_OpenListHead, aStarNode);
        }

        public override void RemoveFromClosedList(AStarNodeAbstract aStarNode)
        {
            // No need to clear AStar flags, because when node is inserted into the Open list,
            // flags are cleared and reset.
            RemoveFromList(ref m_ClosedListHead, aStarNode);
        }


        public override AStarNodeAbstract RemoveCheapestOpenNode()
        {
            // NOTE: Finding the cheapest node could be optimized by 
            //       maintaining a short, sorted list of the cheapest
            //       nodes. See "How to Achieve Lightning-Fast A*" in 
            //       AI Game Programming Wisdom, p. 133.
            //       Specifically "Be a Cheapskate" on p. 140.

            AStarNodeAbstract nodeCheapest = m_OpenListHead;

            // Iterate over all nodes, and keep track of the cheapest.
            AStarNodeAbstract node;

            for (node = m_OpenListHead; node != null; node = node.nextNode)
            {
                if (node.fFitness < nodeCheapest.fFitness)
                {
                    nodeCheapest = node;
                }
            }

            // Remove the cheapest node from the Open list.
            if (nodeCheapest != null)
            {
                RemoveFromOpenList(nodeCheapest);
            }

            // Return the cheapest node.
            return nodeCheapest;
        }

        public override AStarNodeAbstract FindInOpenList(ENUM_ASTAR_NODE_ID eAStarNode)
        {
            return FindInList(m_OpenListHead, eAStarNode);
        }

        public override  AStarNodeAbstract FindInClosedList(ENUM_ASTAR_NODE_ID eAStarNode)
        {
            return FindInList(m_ClosedListHead, eAStarNode);
        }

	    // List management.
        protected void ClearList(ref AStarNodeAbstract listHead)
        {
            // List is empty.
            if (listHead == null)
            {
                return;
            }

            AStarNodeAbstract node = listHead;
            AStarNodeAbstract nodeDelete;
            listHead = null;

            // Iterate over all nodes in list, and delete them.
            while(node != null)
            {
                nodeDelete = node;
                node = node.nextNode;
                DestroyAStarNode(nodeDelete);
            }
        }

	    protected void AddToList(ref AStarNodeAbstract listHead, AStarNodeAbstract aStarNode)
        {
            // Insert a node at the head of the list.
            if (listHead != null)
            {
                listHead.prevNode = aStarNode;
                aStarNode.nextNode = listHead;
            }

            listHead = aStarNode;
        }


        protected void RemoveFromList(ref AStarNodeAbstract listHead, AStarNodeAbstract aStarNode) 
        {
            // Remove a node from the middle or end of the list.
            if (aStarNode.prevNode != null)
            {
                aStarNode.prevNode.nextNode = aStarNode.nextNode;
            }
            else
            {   // Remove a node from the head of the list.
                listHead = aStarNode.nextNode;
            }

            // Complete 2-way link.
            if (aStarNode.nextNode != null)
            {
                aStarNode.nextNode.prevNode = aStarNode.prevNode;
            }

            // NULLify links.
            aStarNode.nextNode = null;
            aStarNode.prevNode = null;
        }

	    protected AStarNodeAbstract	FindInList(AStarNodeAbstract listHead, ENUM_ASTAR_NODE_ID eAStarNode)
        {
            // The Fear SDK notes that List searches could be optimized by using a hash table of lists rather than one flat list.
            // Iterate over all nodes in list.
            AStarNodeAbstract node = listHead;
            while (node != null)
            {
                // Return node if a match is found.
                if (node.eAStarNodeID == eAStarNode)
                {
                    return node;
                }

                node = node.nextNode;
            }

            // No match was found.
            return null;
        }

	    protected AStarNodeAbstract	m_OpenListHead;
	    protected AStarNodeAbstract	m_ClosedListHead;

    }
}