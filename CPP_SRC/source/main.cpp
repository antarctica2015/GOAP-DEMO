
//includes
#include "utils.h"
#include "agent.h"
#include "worldstate.h"
#include "goalabstract.h"
#include "actionabstract.h"
#include "goalstorage.h"
#include "actionstorage.h"
#include "aiplanner.h"


extern class Agent* g_pAgent = NULL; //hack until can write a replacement qsort function.


int main()
{
	int iNum;

	std::cout << "Demo program startup....." << std::endl; 

	std::cout<< "Creating An Agent" << std::endl;
	g_pAgent = new Agent();
	g_pAgent->Init();
	std::cout<< "Agent Created." << std::endl;
	std::cout<< "Demo program startup complete." << std::endl;

	//create a start world state for agent.
	//then attempt to find a plan for a goal.
	std::cout << "......................................................." << std::endl; 

	//Processing
	//std::cout<< "Selecting most relevant goal and creating and executing a Plan:- " << std::endl;
	//g_pAgent->GetGoalStorage()->SelectRelevantGoal(); // need to make a SetRelevantGoal();?
	//std::cout << "Current Goal: " << s_aszGoalTypes[g_pAgent->GetGoalStorage()->GetCurrentGoal()->GetGoalType()] << std::endl;

	//build a plan for input goal
	std::cout<< "create and execute a Plan to solve given goal:- " << std::endl;
	
	GoalAbstract* pGoal = g_pAgent->GetGoalStorage()->FindGoalByType(kGoal_Idle);
	g_pAgent->GetGoalPlanner()->BuildPlan(pGoal);

	std::cout << "..............................Plan Complete" << std::endl;

	std::cout << "......................................................." << std::endl; 

	//Shutdown
	std::cout<< "Demo Program Shutdown....." << std::endl;

	std::cout<< "Destroying An Agent" << std::endl;

	//delete pGoal;
	//pGoal = NULL;

	if(g_pAgent)
	{
		delete g_pAgent;
		g_pAgent = NULL;
	}
	std::cout<< "Agent Destroyed" << std::endl;


	std::cout<< "Demo Program Shutdown Complete." << std::endl;

	std::cout << "press a key to quit" << std::endl;
	std::cin >> iNum;

	return 0;
}


//