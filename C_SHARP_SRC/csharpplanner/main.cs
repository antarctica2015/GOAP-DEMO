using System;

//will need to add some namespaces
using NSAgent;
using NSGoalAbstract;

namespace NSCSharpPlanner
{
	class MainClass
	{
       
		public static void Main (string[] args)
		{
			Console.WriteLine ("Demo startup");

			Console.WriteLine ("Create an Agent");
			Agent myAgent = new Agent();

            myAgent.Init();

			Console.WriteLine ("Create a Goal");
			// some code

            GoalAbstract goal = myAgent.GetGoalStorage().FindGoalByType(ENUM_GOAL_TYPE.kGoal_Idle);
        
			Console.WriteLine ("Search for Plan");
			//
            myAgent.GetGoalPlanner().BuildPlan(goal);

			Console.WriteLine ("Display Results");
			//

			Console.WriteLine ("Tidy up");
			myAgent = null; //might not be feasible in c#?

			Console.WriteLine ("Demo shutdown");
            
            Console.ReadLine();
		}
	}
}
