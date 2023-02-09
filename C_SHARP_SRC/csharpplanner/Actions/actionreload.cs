
//------------------------------------------------------------------------
//
//  Name:   actionreload.h
//
//  Desc:  
//
//  Author: Brett Keir Jones 2015
//
//------------------------------------------------------------------------
using System;
using NSAgent;
using NSActionAbstract;
using NSWorldState;

namespace NSActionReload 
{
        public class ActionReload : ActionAbstract
        {
		    public override ENUM_ACTION_TYPE GetActionClassType() { return ENUM_ACTION_TYPE.kAct_Reload; }

            public ActionReload()
            { 
            
            }

		    //ActionAbstract members.
		    public override void InitAction()
            {
                Console.WriteLine("Init Action: kAct_Reload");
                
                //
	            base.InitAction();

	            m_eActionType = ENUM_ACTION_TYPE.kAct_Reload;
	            m_fActionCost = 5.0f;
	            m_fActionPrecedence = 1.0f;
	            m_bActionIsInterruptible = false;
	            m_fActionProbability = 1.0f;

	            // Set effects.
	            // Weapon is loaded.
	            m_wsWorldStateEffects.SetWSProp(ENUM_WORLDSTATE_PROP_KEY.kWSKey_WeaponLoaded, ENUM_WORLDSTATE_PROP_TYPE.kWSType_bool, true );
            }

            public override bool ValidateContextPreconditions(Agent agent, ref WorldState wsWorldStateGoal, bool bIsPlanning)
            { 
                if (base.ValidateContextPreconditions(agent, ref wsWorldStateGoal, bIsPlanning))
	            {
		            return false;
	            }

	            return true;
            }

		    public override void ActivateAction(Agent agent, ref WorldState wsWorldStateGoal)
            {
            	base.ActivateAction(agent, ref wsWorldStateGoal);

	            agent.GetBlackBoard().SetBBIsCurrentWeaponLoaded(true);
            }

            public override bool IsActionComplete(Agent agent)
            {
                //could just check blackboard to see if weapon is loaded? (like e.long)
                if (agent.GetBlackBoard().GetBBIsCurrentWeaponLoaded())
                {
                    return true;
                }

                // Reloading is not complete.
                return false;
            }
        }

}
