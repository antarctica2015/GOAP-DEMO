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

using NSAgent;
using NSEnums;

namespace NSBlackBoard
{
    // Functions as an agents internal memory / scratchpad.
	public class BlackBoard
	{
	    public BlackBoard()
        {
	        m_bTargetVisible = false;
	        m_eTaskStatus = ENUM_WMTASK_STATUS.kTaskStatus_Unset;
	        m_bSelectAction = true;
	        m_bInvalidatePlan = false;
	        m_bInvalidatePath = false;
	        m_bWeaponLoaded = false;
        }
	
	   // Agent info
	   public ENUM_WMTASK_STATUS	GetBBTaskStatus() { return m_eTaskStatus; }
	   public void					SetBBTaskStatus( ENUM_WMTASK_STATUS eStatus ) { m_eTaskStatus = eStatus; }

	   public bool					GetBBSelectAction() { return m_bSelectAction; }
	   public void					SetBBSelectAction( bool bSelectAction ) { m_bSelectAction = bSelectAction; }
	
	   // Goal info

	   // Navigation info

	   // Targetting info
	   public bool					GetBBTargetVisible() { return m_bTargetVisible; }
	   public void					SetBBTargetVisible( bool bVisible ) { m_bTargetVisible = bVisible; }

	   public bool					GetBBInvalidatePath()  { return m_bInvalidatePath; }
	   public void					SetBBInvalidatePath(bool bRecacl) { m_bInvalidatePath = bRecacl; }

	   // Weapons info
	   public bool					GetBBIsCurrentWeaponLoaded(){ return m_bWeaponLoaded; }
	   public void					SetBBIsCurrentWeaponLoaded(bool bWeaponLoaded){ m_bWeaponLoaded = bWeaponLoaded; }

	   // Plan info
	   public  bool					GetBBInvalidatePlan() { return m_bInvalidatePlan ;}
	   public void					SetBBInvalidatePlan(bool bInvalidatePlan)  { m_bInvalidatePlan = bInvalidatePlan ;}

	   // Agent info

	   private ENUM_WMTASK_STATUS	m_eTaskStatus;
	   private bool					m_bSelectAction;

	   private bool					m_bTargetVisible;
	   private bool					m_bInvalidatePath;

	   // Weapons info
	   private bool					m_bWeaponLoaded;

	   // Plan info
	   private bool				    m_bInvalidatePlan;
	}
}

