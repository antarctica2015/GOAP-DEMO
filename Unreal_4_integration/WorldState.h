//------------------------------------------------------------------------
//
//  Name:   WorldState.h
//
//  Desc:   world state representation for agent & planner
//     
//  Author: Brett Keir Jones
//
//------------------------------------------------------------------------

#pragma once

#include "GoapTest.h"
#include <bitset>

//forward dec
class AGOAPController;

// WorldState Property Keys.
// *must* be kept in sync with the g_szWorldStatePropKey string list
enum ENUM_WORLDSTATE_PROP_KEY
{
	kWSKey_InvalidKey = -1,
	KWSKey_DummyWS,

	//Count needs to be last
	kWSKey_Count
};

// WorldState Property Keys.
// *must* be kept in sync with the ENUM_WORLDSTATE_PROP_KEY enum list
const char* const g_szWorldStatePropKey[] =
{
	"KWSKey_DummyWS",
};

enum ENUM_WORLDSTATE_PROP_TYPE
{
	kWSType_InvalidType = -1,
	kWSType_Unset,
	kWSType_Variable,
	kWSType_Object,
	kWSType_int,
	kWSType_bool,
	kWSType_float,
};

//World State Property 
struct STRUCT_WORLDSTATE_PROP
{
	//
	ENUM_WORLDSTATE_PROP_KEY	eWSKey;
	ENUM_WORLDSTATE_PROP_TYPE	eWSType;

	//remember unions cannot be compared!
	//should all br 4-byte data fields
	//altering values outside of WorldState::SetWSProp may be unsafe

	//declare (anonymous) union
	union {

		ENUM_WORLDSTATE_PROP_KEY	eVariableWSKey;
		AGOAPController*			pWSValue;
		int32						iWSValue;
		bool						bWSValue;
		float						fWSValue;

	};


	//
	STRUCT_WORLDSTATE_PROP()
	{
		eWSKey = kWSKey_InvalidKey;
		eWSType = kWSType_InvalidType;
		eVariableWSKey = kWSKey_InvalidKey;
		iWSValue = (int32)0; // cast zero to int
		bWSValue = false;
		fWSValue = 0.0f;
		pWSValue = NULL;
	}

	//Overload assignment '=' operator
	STRUCT_WORLDSTATE_PROP& operator=(const STRUCT_WORLDSTATE_PROP& rOtherProp)
	{
		//copy properties over
		eWSKey = rOtherProp.eWSKey;
		eVariableWSKey = rOtherProp.eVariableWSKey;
		eWSType = rOtherProp.eWSType;
		iWSValue = rOtherProp.iWSValue;
		bWSValue = rOtherProp.bWSValue;
		fWSValue = rOtherProp.fWSValue;
		pWSValue = rOtherProp.pWSValue;

		return *this;
	}

	//Other operator overloads should go here.

};

//////////////////////////////////////////////////////////////////////////////

//typedefs to ease typing
typedef std::bitset<kWSKey_Count>				WSPROP_FLAGS;

//WORLD STATE CLASS
class WorldState
{
public:

	WorldState();
	~WorldState();

	//
	void	ResetWS() { m_maskPropsSet.reset(); }

	//set properties
	void	SetWSProp(STRUCT_WORLDSTATE_PROP* pProp);
	void	SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, STRUCT_WORLDSTATE_PROP* pProp);
	void	SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, ENUM_WORLDSTATE_PROP_TYPE eType, int32 iValue);
	void	SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, ENUM_WORLDSTATE_PROP_TYPE eType, bool bValue);
	void	SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, ENUM_WORLDSTATE_PROP_TYPE eType, float fValue);

	//clear world state properties
	void	ClearWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey);

	//Get
	STRUCT_WORLDSTATE_PROP*	GetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey);
	STRUCT_WORLDSTATE_PROP*	GetWSProp(uint32 iProp);

	//Evaluate a prop
	STRUCT_WORLDSTATE_PROP* DereferenceWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey);

	//Querys
	uint32			GetNumWSProps() const { return m_maskPropsSet.count(); }
	bool			HasWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey);

	//Planning
	void			CopyWorldState(WorldState& wsWorldState);
	uint32			GetNumWorldStateDifferences(WorldState& wsOtherWorldState);
	uint32			GetNumUnsatisfiedWorldStateProps(WorldState& wsOtherWorldState);

	//data access
	WSPROP_FLAGS*	GetWSPropSetFlags() { return &m_maskPropsSet; }

	const char* const GetWorldStatePropName(ENUM_WORLDSTATE_PROP_KEY ePropKey);

protected:

	//add a prop
	void			AddWSProp(STRUCT_WORLDSTATE_PROP& propToAdd);

	STRUCT_WORLDSTATE_PROP		m_aWSProps[kWSKey_Count];
	WSPROP_FLAGS				m_maskPropsSet;

};