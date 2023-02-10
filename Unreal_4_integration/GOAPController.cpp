// Fill out your copyright notice in the Description page of Project Settings.

#include "GoapTest.h"
#include "GOAPController.h"
#include "AIPlanner.h"
#include "WorldState.h"
#include "Blackboard.h"
#include "ActionStorage.h"
#include "GoalStorage.h"

//
AGOAPController::AGOAPController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	m_pGoalPlanner = NULL;
	m_pWorldState = NULL;
	m_pPlan = NULL;
	m_pBlackBoard = NULL;
	m_pActionStorage = NULL;
	m_pGoalStorage = NULL;

	// Set all this running
	Init();

}

void AGOAPController::CleanUp()
{
	if (m_pWorldState)
	{
		delete m_pWorldState;
		m_pWorldState = NULL;
	}

	if (m_pBlackBoard)
	{
		delete m_pBlackBoard;
		m_pBlackBoard = NULL;
	}

	if (m_pGoalStorage)
	{
		delete m_pGoalStorage;
		m_pGoalStorage = NULL;
	}

	if (m_pPlan)
	{
		delete m_pPlan;
		m_pPlan = NULL;
	}

	if (m_pActionStorage)
	{
		delete m_pActionStorage;
		m_pActionStorage = NULL;
	}

	if (m_pGoalPlanner)
	{
		delete m_pGoalPlanner;
		m_pGoalPlanner = NULL;
	}

}

//Initialises the Agent
void AGOAPController::Init()
{
	m_pGoalPlanner = NULL;
	m_pWorldState = NULL;
	m_pPlan = NULL;
	m_pBlackBoard = NULL;
	m_pActionStorage = NULL;
	m_pGoalStorage = NULL;

	m_pActionStorage = new ActionStorage();

	m_pGoalStorage = new GoalStorage(this);

	m_pBlackBoard = new BlackBoard();

	m_pWorldState = new WorldState();
	m_pWorldState->ResetWS();

	m_pGoalPlanner = new AIPlanner(this);
}




