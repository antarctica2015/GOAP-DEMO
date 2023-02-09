//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.18444
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using System;
using System.Collections;

using NSAgent;
using NSUtils;

namespace NSWorldState
{
        // WorldState Property Keys.
        // *must* be kept in sync with the g_szWorldStatePropKey string list
        // subject to change!
        public enum ENUM_WORLDSTATE_PROP_KEY
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
        }

        // world state property types - can remove?
        public enum ENUM_WORLDSTATE_PROP_TYPE
        {
            kWSType_InvalidType = -1,
            kWSType_Unset,
            kWSType_Variable,
            kWSType_Object,
            kWSType_int,
            kWSType_bool,
            kWSType_float,
        };


    //TODO: replace with a class?
    //World State Property 
   public class WorldStateProp
    {
	    //
	    public ENUM_WORLDSTATE_PROP_KEY	 eWSKey;
        public ENUM_WORLDSTATE_PROP_TYPE eWSType;

        public ENUM_WORLDSTATE_PROP_KEY eVariableWSKey;
        public Agent pWSValue;
        public int iWSValue;
        public bool bWSValue;
        public float fWSValue;

        public WorldStateProp()
        {
            eWSKey = ENUM_WORLDSTATE_PROP_KEY.kWSKey_InvalidKey;
            eWSType = ENUM_WORLDSTATE_PROP_TYPE.kWSType_InvalidType;

            eVariableWSKey = ENUM_WORLDSTATE_PROP_KEY.kWSKey_InvalidKey;
            pWSValue = null;
            iWSValue = (int)0;
            bWSValue = false;
            fWSValue = 0.0f;
        }

	    //Overload assignment '=' operator
	    //public static STRUCT_WORLDSTATE_PROP operator = (const ref STRUCT_WORLDSTATE_PROP rOtherProp)
        public WorldStateProp opAssign(ref WorldStateProp rOtherProp)
	    {
		    //copy properties over
		    eWSKey   = rOtherProp.eWSKey;
		    eVariableWSKey = rOtherProp.eVariableWSKey;
		    eWSType	 = rOtherProp.eWSType;
		    iWSValue = rOtherProp.iWSValue;
		    bWSValue = rOtherProp.bWSValue;
		    fWSValue = rOtherProp.fWSValue;
		    pWSValue = rOtherProp.pWSValue;

		    return this;
	    }
    }

    //WORLD STATE CLASS
	public class WorldState
	{
        //TODO: cant just have a string not inside a class
        public string[] g_szWorldStatePropKey = 
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

	   public WorldState() 
       {
           m_maskPropsSet = new BitArray((int)ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count);
           m_maskPropsSet.SetAll(false);

           m_aWSProps = new WorldStateProp[(int)ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count];

           for (int i = 0; i < m_aWSProps.Length; ++i)
           {
               m_aWSProps[i] = new WorldStateProp();
           }

       }

	   //
	   public void ResetWS() { m_maskPropsSet.SetAll(false); }

	   //set properties
       public void SetWSProp(ref WorldStateProp prop)
       {
           if(prop != null)
	       {
	            m_aWSProps[(int)(prop.eWSKey)] = prop;
	            m_maskPropsSet.Set((int)(prop.eWSKey), true);
	       }
       }

       // This handles setting a prop to a different key than its own.
	   // Required to set variable values from other keys.
       public void SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, WorldStateProp prop)
       {
	        if(prop != null)
	        {
		        m_aWSProps[(int)ePropKey] = prop;
		        m_aWSProps[(int)ePropKey].eWSKey = ePropKey;
		        m_maskPropsSet.Set((int)ePropKey, true);
	        }
       }

       // Set world state property - override for bools
	   public void SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, ENUM_WORLDSTATE_PROP_TYPE eType, int iValue )
       {
         	m_aWSProps[(int)ePropKey].eWSKey  = ePropKey;
	        m_aWSProps[(int)ePropKey].eWSType = eType;
	        m_aWSProps[(int)ePropKey].iWSValue = iValue;
	
	        m_maskPropsSet.Set((int)ePropKey, true);
       }

        //
	   public void	SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, ENUM_WORLDSTATE_PROP_TYPE eType, bool bValue )
       {
           m_aWSProps[(int)ePropKey].eWSKey  = ePropKey;
	       m_aWSProps[(int)ePropKey].eWSType = eType;

	       //insure all bits (32) are cleared then set bool value
	       m_aWSProps[(int)ePropKey].iWSValue = 0;
	       m_aWSProps[(int)ePropKey].bWSValue = bValue;

	       m_maskPropsSet.Set((int)ePropKey, true);
       }

        //
	   public void	SetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey, ENUM_WORLDSTATE_PROP_TYPE eType, float fValue )
       {
           	m_aWSProps[(int)ePropKey].eWSKey  = ePropKey;
	        m_aWSProps[(int)ePropKey].eWSType = eType;
	        m_aWSProps[(int)ePropKey].fWSValue = fValue;
	
	        m_maskPropsSet.Set((int)ePropKey, true);
       }


	   //clear world state properties
	   public void	ClearWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey)
       {
           m_maskPropsSet.Set((int)ePropKey, false);
       }

	   //Get
       public WorldStateProp GetWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey)
       {
            if( m_maskPropsSet.Get((int)ePropKey) == true )
	        {
		        return m_aWSProps[(int)ePropKey] ;
	        }

	        return null;
       }


       public WorldStateProp GetWSProp(uint iProp)
       {
            if( iProp >= (int)ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count)
	        {
		        return null;
	        }

	        if( m_maskPropsSet.Get((int)iProp) == true )
	        {
		        return m_aWSProps[iProp];
	        }

            return null;
       }

	   //Evaluate a prop
       public WorldStateProp DereferenceWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey)
       {
           WorldStateProp evaluatedProp = GetWSProp(ePropKey);

	        int iCounter = 0;

           // was while( (pEvaluatedProp && kWSType_Variable ) == pEvaluatedProp->eWSType )

	        while( (evaluatedProp != null) && ((int)ENUM_WORLDSTATE_PROP_TYPE.kWSType_Variable == (int)evaluatedProp.eWSType ))
	        {
		        if(iCounter >= (int)ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count)
		        {
                    Console.WriteLine("WorldState::DereferenceWSProp: Recursive properties detected!");
			        evaluatedProp = null;
			        break;
		        }

		        ++iCounter;
		        evaluatedProp = GetWSProp(evaluatedProp.eVariableWSKey);
	        }

	        return evaluatedProp;
       }

	   //Querys
	   public uint GetNumWSProps() { return (uint)m_maskPropsSet.Count; }

	   public bool  HasWSProp(ENUM_WORLDSTATE_PROP_KEY ePropKey)
       {
           	return m_maskPropsSet.Get((int)ePropKey);
       }


	   //Planning
	   public void CopyWorldState( ref WorldState wsWorldState )
       {
            //memberwise copy?
            m_maskPropsSet.SetAll(false);

	        for(int iProp = 0; iProp < (int)ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count; ++iProp)
	        {
		        if( wsWorldState.m_maskPropsSet.Get(iProp) )
		        {
			        m_maskPropsSet.Set(iProp, true);
			        m_aWSProps[iProp] = wsWorldState.m_aWSProps[iProp];
		        }
	        }
       } 

       // Returns the number of differences between two worldstates (current and given?)
	   public uint GetNumWorldStateDifferences(ref WorldState wsOtherWorldState)
       {
                uint iDifferences = 0;

	            bool bIsCurrentSet;
	            bool bIsOtherSet;

	            for(int iProp =0; iProp < (int)ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count; ++iProp)
	            {
		            bIsCurrentSet = m_maskPropsSet.Get(iProp);
		            bIsOtherSet   = wsOtherWorldState.m_maskPropsSet.Get(iProp);

		            if(bIsCurrentSet && bIsOtherSet)
		            {
			            if( ( m_aWSProps[iProp].pWSValue != wsOtherWorldState.m_aWSProps[iProp].pWSValue ) ||
				            ( m_aWSProps[iProp].eWSType != wsOtherWorldState.m_aWSProps[iProp].eWSType ) )
			            {
				            ++iDifferences;
			            }
		            }
		            else if( bIsCurrentSet || bIsOtherSet )
		            {
			            ++iDifferences;
		            }

	            }

	            //return total number of differences
	            return iDifferences;
       }

      // Returns number of unsatisfied props in this world state, relative to another world state
	  public uint GetNumUnsatisfiedWorldStateProps(ref WorldState wsOtherWorldState)
      {
            uint iUnsatisfied = 0;

	        for(int iProp =0; iProp < (int)ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count; ++iProp)
	        {
		        if( !m_maskPropsSet.Get(iProp) )
		        {
			        continue;
		        }

		        if( !wsOtherWorldState.m_maskPropsSet.Get(iProp) )
		        {
			        ++iUnsatisfied;
		        }

		        if( ( m_aWSProps[iProp].pWSValue != wsOtherWorldState.m_aWSProps[iProp].pWSValue )||
			        ( m_aWSProps[iProp].eWSType != wsOtherWorldState.m_aWSProps[iProp].eWSType ) )
		        {
				        ++iUnsatisfied;
		        }
	        }

	        //return total number of Unsatisfied props
	        return iUnsatisfied;
      }

	   //data access
	   public BitArray GetWSPropSetFlags() { return m_maskPropsSet; }

	   public string GetWorldStatePropName(ENUM_WORLDSTATE_PROP_KEY ePropKey)
       {
            if( ePropKey < ENUM_WORLDSTATE_PROP_KEY.kWSKey_InvalidKey || ePropKey >= ENUM_WORLDSTATE_PROP_KEY.kWSKey_Count )
	        {
		        return "ERROR:ePropKey_OutOfRange";
	        }

	        if( ePropKey == ENUM_WORLDSTATE_PROP_KEY.kWSKey_InvalidKey)
	        {
		        return "ERROR:kWSKey_InvalidKey";
	        }

	        return g_szWorldStatePropKey[(int)ePropKey];
       }
	   
       //add a prop
       protected void AddWSProp(ref WorldStateProp propToAdd)
       {
           	m_aWSProps[(int)propToAdd.eWSKey] = propToAdd;
	        m_maskPropsSet.Set((int)propToAdd.eWSKey, true);
       }

       protected WorldStateProp[] m_aWSProps;        
	   protected BitArray		  m_maskPropsSet;

	}
}

