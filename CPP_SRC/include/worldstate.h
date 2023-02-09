#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#pragma warning (disable:4267) //warning C4267: 'return' : conversion from 'size_t' to 'unsigned int', possible loss of data	e:\msc\code\goalplanner\include\worldstate.h

//------------------------------------------------------------------------
//
//  Name:   worldstate.h
//
//  Desc:   world state representation for agent & planner
//     
//  Author: Brett Keir Jones 2008
//
//------------------------------------------------------------------------

#include <bitset>
#include <vector>

//forward dec
class Agent;


//World state events
//enum ENUM_WORLDSTATE_EVENT
//{
//	kWSE_Invalid,
//
//};

// WorldState Property Keys.
// *must* be kept in sync with the g_szWorldStatePropKey string list
// subject to change!

enum ENUM_WORLDSTATE_PROP_KEY
{
	kWSKey_InvalidKey = -1,
	KWSKey_AreaCleared,
	kWSKey_AtNode,
	kWSKey_AtNodeType,
	kWSKey_AtTargetPos,
	kWSKey_Idling,
	kWSKey_InWeaponsRange,
	kWSKey_RoomCleared,
	kWSKey_TargetIsAimingAtMe,
	kWSKey_TargetIsLookingAtMe,
	kWSKey_TargetIsNeutralised,
	kWSKey_TargetIsCompliant,
	kWSKey_UsingObject,  //such as a door?
	kWSKey_WeaponLoaded,

	//Count needs to be last
	kWSKey_Count
};

// WorldState Property Keys.
// *must* be kept in sync with the ENUM_WORLDSTATE_PROP_KEY enum list

const char* const g_szWorldStatePropKey[] = 
{
	"KWSKey_AreaCleared",
	"kWSKey_AtNode",
	"kWSKey_AtNodeType",
	"kWSKey_AtTargetPos",
	"kWSKey_Idling",
	"kWSKey_InWeaponsRange",
	"kWSKey_RoomCleared",
	"kWSKey_TargetIsAimingAtMe",
	"kWSKey_TargetIsNeutralised",
	"kWSKey_TargetIsCompliant",
	"kWSKey_UsingObject",
	"kWSKey_WeaponLoaded"

};

//// world state property types - can remove?
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
		Agent*						pWSValue;
		int							iWSValue;
		bool						bWSValue;
		float						fWSValue;

	};


	//
	STRUCT_WORLDSTATE_PROP()
	{
		eWSKey   = kWSKey_InvalidKey;
		eWSType  = kWSType_InvalidType;
		eVariableWSKey = kWSKey_InvalidKey;
		iWSValue = (int)0; // cast zero to int
		bWSValue = false;
		fWSValue = 0.0f;
		pWSValue = NULL;
	}

	//Overload assignment '=' operator
	STRUCT_WORLDSTATE_PROP& operator=(const STRUCT_WORLDSTATE_PROP& rOtherProp )
	{
		//copy properties over
		eWSKey   = rOtherProp.eWSKey;
		eVariableWSKey  = rOtherProp.eVariableWSKey;
		eWSType	 = rOtherProp.eWSType;
		iWSValue = rOtherProp.iWSValue;
		bWSValue = rOtherProp.bWSValue;
		fWSValue = rOtherProp.fWSValue;
		pWSValue = rOtherProp.pWSValue;

		return *this;
	}

	////BKJ: need to recheck this.
	//STRUCT_WORLDSTATE_PROP* operator=(const STRUCT_WORLDSTATE_PROP* rOtherProp )
	//{
	//	//copy properties over
	//	eWSKey   = rOtherProp->eWSKey;
	//	eVariableWSKey  = rOtherProp->eVariableWSKey;
	//	eWSType	 = rOtherProp->eWSType;
	//	iWSValue = rOtherProp->iWSValue;
	//	bWSValue = rOtherProp->bWSValue;
	//	fWSValue = rOtherProp->fWSValue;
	//	pWSValue = rOtherProp->pWSValue;

	//	return this;
	//}

	////Overload equality '==' operator
	//bool operator==(const STRUCT_WORLDSTATE_PROP& rOtherProp )
	//{
	//	//
	//	if(eWSKey != rOtherProp.eWSKey)
	//	{
	//		return false;
	//	}

	//	if(eWSKey != rOtherProp.eWSType)
	//	{
	//		return false;
	//	}

	//	if(iWSValue != rOtherProp.iWSValue)
	//	{
	//		return false;
	//	}

	//	if(bWSValue != rOtherProp.bWSValue)
	//	{
	//		return false;
	//	}

	//	if(fWSValue != rOtherProp.fWSValue)
	//	{
	//		return false;
	//	}

	//	if(pWSValue != rOtherProp.pWSValue)
	//	{
	//		return false;
	//	}

	//	if(eVariableWSKey != rOtherProp.eVariableWSKey)
	//	{
	//		return false;
	//	}

	//	return true;
	//}

};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//typedefs to ease typing
typedef std::vector<STRUCT_WORLDSTATE_PROP>		WSPROP_LIST; // Never used?
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
	void	SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, STRUCT_WORLDSTATE_PROP* pProp );
	void	SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, ENUM_WORLDSTATE_PROP_TYPE eType, int iValue );
	//void	SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, ENUM_WORLDSTATE_PROP_TYPE eType, Agent* pValue );
	void	SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, ENUM_WORLDSTATE_PROP_TYPE eType, bool bValue );
	void	SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, ENUM_WORLDSTATE_PROP_TYPE eType, float fValue );

	//clear world state properties
	void	ClearWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey);
	//void	ClearWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey);

	//Get
	//STRUCT_WORLDSTATE_PROP*	GetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, Agent* pSubject);
	STRUCT_WORLDSTATE_PROP*	GetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey);
	STRUCT_WORLDSTATE_PROP*	GetWSProp(unsigned int iProp);

	//Evaluate a prop
	STRUCT_WORLDSTATE_PROP* DereferenceWSProp( ENUM_WORLDSTATE_PROP_KEY ePropKey );

	//Querys
	unsigned int	GetNumWSProps() const { return m_maskPropsSet.count(); }
	bool			HasWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey);
//	bool			HasWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey);

	//Planning
	void			CopyWorldState( WorldState& wsWorldState ); 
	unsigned int	GetNumWorldStateDifferences( WorldState& wsOtherWorldState );
	unsigned int	GetNumUnsatisfiedWorldStateProps( WorldState& wsOtherWorldState );

	//data access
	WSPROP_FLAGS*	GetWSPropSetFlags() { return &m_maskPropsSet; }

	//debugging
	//void PrintWorldStateToFile();
	//void PrintWorldStateToScreen();
	//void GetDebugInfo( std::string& sOut );


	const char* const GetWorldStatePropName(ENUM_WORLDSTATE_PROP_KEY ePropKey);

protected:

	//add a prop
	void			AddWSProp(STRUCT_WORLDSTATE_PROP& propToAdd);

	STRUCT_WORLDSTATE_PROP		m_aWSProps[kWSKey_Count];
	WSPROP_FLAGS				m_maskPropsSet;

};

#endif