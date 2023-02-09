
//------------------------------------------------------------------------
//
//  Name:   actionidle.cs
//
//  Desc:   Idle action class implementation
//
//  Author: Brett Keir Jones 2015
//
//------------------------------------------------------------------------

using System;
using NSAgent;
using NSActionAbstract;
using NSWorldState;

namespace NSActionIdle
{
    public class ActionIdle : ActionAbstract
    {
		//my version:
		public override ENUM_ACTION_TYPE GetActionClassType() { return ENUM_ACTION_TYPE.kAct_Idle; }

		//ActionAbstract members.
		public override void InitAction()
        {
            Console.WriteLine("Init Action KAct_Idle");
            
            //TODO: check how to access parent class methods in c#
	        base.InitAction();

	        m_eActionType = ENUM_ACTION_TYPE.kAct_Idle;
	        m_fActionCost = 2.0f;
	        m_fActionPrecedence = 1.0f;
	        m_bActionIsInterruptible = true;
	        m_fActionProbability = 1.0f;

	        //dont want to idle if out weapon is not loaded (want it to reload then idle)
	        //m_wsWorldStatePreconditions.SetWSProp(key_name, prop_key_type, key_value )
	        m_wsWorldStatePreconditions.SetWSProp(ENUM_WORLDSTATE_PROP_KEY.kWSKey_WeaponLoaded, ENUM_WORLDSTATE_PROP_TYPE.kWSType_bool, true);

	        //set effects
	        //Effect = ai agent is idling.
	        m_wsWorldStateEffects.SetWSProp(ENUM_WORLDSTATE_PROP_KEY.kWSKey_Idling, ENUM_WORLDSTATE_PROP_TYPE.kWSType_bool, true);
        
        }


        public override void ActivateAction(Agent agent, ref WorldState wsGoalState)
        { 
         base.ActivateAction(agent, ref wsGoalState);

	        //check swat4 actions for ideas!
	        //we only use the idle aim around behavior if the officer isn't moving and clearing
        }

    }
}