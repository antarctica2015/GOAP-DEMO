
#include "GoapTest.h"
#include "WorldState.h"
#include "GOAPController.h"
#include "Utils.h"

//ctor & dtor
WorldState::WorldState()
{
	m_maskPropsSet.reset();
	//test

}

WorldState::~WorldState()
{
	//perform cleanup
}

//add a given prop to the world state
void WorldState::AddWSProp(STRUCT_WORLDSTATE_PROP& propToAdd)
{
	m_aWSProps[propToAdd.eWSKey] = propToAdd;
	m_maskPropsSet.set(propToAdd.eWSKey, true);
}

//set a world state prop to match values of given world state property
void WorldState::SetWSProp(STRUCT_WORLDSTATE_PROP* pProp)
{
	if (pProp) {
		m_aWSProps[pProp->eWSKey] = *pProp;
		m_maskPropsSet.set(pProp->eWSKey, true);
	}

}

//set a world state prop to match values of given world state key and property
void WorldState::SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, STRUCT_WORLDSTATE_PROP* pProp)
{
	//this handles setting a prop to a different key than its own.
	//required to set variable values from other keys.
	if (pProp) {
		m_aWSProps[ePropKey] = *pProp;
		m_aWSProps[ePropKey].eWSKey = ePropKey;
		m_maskPropsSet.set(ePropKey, true);
	}

}

//set world state property - override for bools
void WorldState::SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, ENUM_WORLDSTATE_PROP_TYPE eType, bool bValue)
{
	m_aWSProps[ePropKey].eWSKey = ePropKey;
	m_aWSProps[ePropKey].eWSType = eType;

	//insure all bits (32) are cleared then set bool value

	m_aWSProps[ePropKey].iWSValue = 0;
	m_aWSProps[ePropKey].bWSValue = bValue;

	m_maskPropsSet.set(ePropKey, true);
}

//set world state property - override for ints
void WorldState::SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, ENUM_WORLDSTATE_PROP_TYPE eType, int32 iValue)
{
	m_aWSProps[ePropKey].eWSKey = ePropKey;
	m_aWSProps[ePropKey].eWSType = eType;
	m_aWSProps[ePropKey].iWSValue = iValue;

	m_maskPropsSet.set(ePropKey, true);
}

//set world state property - override for floats
void WorldState::SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, ENUM_WORLDSTATE_PROP_TYPE eType, float fValue)
{
	m_aWSProps[ePropKey].eWSKey = ePropKey;
	m_aWSProps[ePropKey].eWSType = eType;
	m_aWSProps[ePropKey].fWSValue = fValue;

	m_maskPropsSet.set(ePropKey, true);
}


//set world state property
//override for Agent*
//the union stores 'raw' objects which may be deleted
//this overload sets up a notifier (?) to all the prop to release object if its deleted
//void WorldState::SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey , ENUM_WORLDSTATE_PROP_TYPE eType, Agent* pValue )
//{
//	m_aWSProps[ePropKey].eWSKey   = ePropKey;
//  m_aWSProps[ePropKey].eWSType = eType;
//	m_aWSProps[ePropKey].pWSValue = pValue;
//
//	m_maskPropsSet.set(ePropKey, true);
//
//}

// clear a world state property
void WorldState::ClearWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey)
{
	m_maskPropsSet.set(ePropKey, false);

}

//// clear a world state property
//void WorldState::ClearWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey)
//{
//	m_maskPropsSet.set(ePropKey, false);
//
//}

// returns true if property exists in world state
bool WorldState::HasWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey)
{
	return m_maskPropsSet.test(ePropKey);

}

// returns true if property exists in world state
//bool WorldState::HasWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey)
//{
//	return m_maskPropsSet.test(ePropKey);
//}

//// returns a pointer to a specified world state property
//STRUCT_WORLDSTATE_PROP*	WorldState::GetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey)
//{
//	if( m_maskPropsSet.test(ePropKey) )
//	{
//		return &(m_aWSProps[ePropKey]) ;
//	}
//	
//	return NULL;
//}

// returns a pointer to a specified world state property
STRUCT_WORLDSTATE_PROP*	WorldState::GetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey)
{
	if (m_maskPropsSet.test(ePropKey)) {
		return &(m_aWSProps[ePropKey]);
	}

	return NULL;
}

// returns a pointer to a specified (by index) world state property
STRUCT_WORLDSTATE_PROP*	WorldState::GetWSProp(uint32 iProp)
{
	if (iProp >= kWSKey_Count) {
		return NULL;
	}

	if (m_maskPropsSet.test(iProp)) {
		return &(m_aWSProps[iProp]);
	}

	return NULL;
}

//evaluates a world state prop
//be careful:can be recursive! (if two props are used to set each others values)
STRUCT_WORLDSTATE_PROP* WorldState::DereferenceWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey)
{
	STRUCT_WORLDSTATE_PROP*	pEvaluatedProp = GetWSProp(ePropKey);

	int32 iCounter = 0;

	while ((pEvaluatedProp && kWSType_Variable) == pEvaluatedProp->eWSType) {
		if (iCounter >= kWSKey_Count) {
			//log Error to file
			pEvaluatedProp = NULL;
			break;
		}

		++iCounter;
		pEvaluatedProp = GetWSProp(pEvaluatedProp->eVariableWSKey);
	}

	return pEvaluatedProp;
}

//copies a world state - 1 prop at a time
void WorldState::CopyWorldState(WorldState& wsWorldState)
{
	m_maskPropsSet.reset();

	for (int32 iProp = 0; iProp < kWSKey_Count; ++iProp) {
		if (wsWorldState.m_maskPropsSet.test(iProp)) {
			m_maskPropsSet.set(iProp, true);
			m_aWSProps[iProp] = wsWorldState.m_aWSProps[iProp];
		}
	}

}

//returns the number of differences between two worldstates (current and given?)
uint32 WorldState::GetNumWorldStateDifferences(WorldState &wsOtherWorldState)
{
	uint32 iDifferences = 0;

	bool bIsCurrentSet;
	bool bIsOtherSet;

	for (int32 iProp = 0; iProp < kWSKey_Count; ++iProp) {
		bIsCurrentSet = m_maskPropsSet.test(iProp);
		bIsOtherSet = wsOtherWorldState.m_maskPropsSet.test(iProp);

		if (bIsCurrentSet && bIsOtherSet) {
			if ((m_aWSProps[iProp].pWSValue != wsOtherWorldState.m_aWSProps[iProp].pWSValue) || (m_aWSProps[iProp].eWSType != wsOtherWorldState.m_aWSProps[iProp].eWSType)) {
				++iDifferences;
			}
		} else if (bIsCurrentSet || bIsOtherSet) {
			++iDifferences;
		}

	}

	//return total number of differences
	return iDifferences;
}

// returns number of unsatisfied props in this world state, relative to another world state
uint32 WorldState::GetNumUnsatisfiedWorldStateProps(WorldState& wsOtherWorldState)
{
	uint32 iUnsatisfied = 0;

	for (int32 iProp = 0; iProp < kWSKey_Count; ++iProp) {
		if (!m_maskPropsSet.test(iProp)) {
			continue;
		}

		if (!wsOtherWorldState.m_maskPropsSet.test(iProp)) {
			++iUnsatisfied;
		}

		if ((m_aWSProps[iProp].pWSValue != wsOtherWorldState.m_aWSProps[iProp].pWSValue) || (m_aWSProps[iProp].eWSType != wsOtherWorldState.m_aWSProps[iProp].eWSType)) {
			++iUnsatisfied;
		}
	}

	//return total number of Unsatisfied props
	return iUnsatisfied;

}

//this function converts a ENUM_WORLDSTATE_PROP_KEY to a string name
//used for debugging and the planner
const char* const WorldState::GetWorldStatePropName(ENUM_WORLDSTATE_PROP_KEY ePropKey)
{
	if (ePropKey < kWSKey_InvalidKey || ePropKey >= kWSKey_Count) {
		return "ERROR:ePropKey_OutOfRange";
	}

	if (ePropKey == kWSKey_InvalidKey) {
		return "ERROR:kWSKey_InvalidKey";
	}

	return g_szWorldStatePropKey[ePropKey];
}

////Returns by parameter a description of the worldstate
//void WorldState::GetDebugInfo( std::string& OutInfoString )
//{
//	// Print out all worldstate keys which are not empty.
//	for ( int i = 0; i < kWSKey_Count; ++i )
//	{
//		STRUCT_WORLDSTATE_PROP* pProp = GetWSProp( i );
//		
//		if ( pProp && pProp->eWSType != kWSType_Unset )
//		{
//			OutInfoString += g_szWorldStatePropKey[i];
//			OutInfoString += ": ";
//
//			switch( pProp->eWSType )
//			{
//				case kWSType_Variable:
//					OutInfoString += "Variable";
//					break;
//
//				/*case kWSType_Object:
//					{
//						const char* const pszNodeName = GetObjectName( pProp->pWSValue );
//						OutInfoString += pszNodeName ? pszNodeName : "None";
//					}
//					break;*/
//
//				case kWSType_int:
//					{
//						char buf[10];
//						//LTSNPrintF(buf, LTARRAYSIZE(buf), "%d", pProp->iWSValue);
//						//snprintf(buf, LTARRAYSIZE(buf), "%d", pProp->iWSValue);
//						OutInfoString += buf;
//					}
//					break;
//
//				case kWSType_bool:
//					pProp->bWSValue ? OutInfoString += "1" : OutInfoString += "0";
//					break;
//
//				case kWSType_float:
//					pProp->fWSValue ? OutInfoString += "1" : OutInfoString += "0";
//					break;
//
//				default:
//					//AIASSERT( 0, 0, "CAIWorldState::GetDebugInfoString: Unrecognized world state prop type." );
//					std::cout << "WorldState:getdebuginfo: Unrecognised world state prop type." << std::endl;
//					break;
//			}
//
//			OutInfoString += "\n";
//		}
//	}
//}
