/***************************************************************************************
** FileName    :  atest_key_step.c                                                    **
** Author      :  Mingxuan.Zhu                                                        **
** Date        :  01/06/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  The implementaion of the press key message.                         **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  01/06/2010     Mingxuan.Zhu         Created                                       **
**  08/08/2010     Mingxuan.Zhu         Updated                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#include "mn_type.h"
#include "mmisms_export.h"
#include "atest_key_step.h"
#include "mmk_kbd.h"

/**************************************************************************************/
// Description      : check whether the received message is a press key message or not.
//--------------------------------------------------------------------------------------
// Input Parameter  : MmiSignalS** signal_ptr 
// Output Parameter : NONE
// Return Value     : BOOLEAN VALUE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN ATESTKEYSTEP_IsPressKeySerialMsg(MmiSignalS** signal_ptr)
{
    if (0xFDFD == (*signal_ptr)->SignalCode) 
    {
        // atest testing message
        return TRUE;        
    }
    return FALSE;
}

/**************************************************************************************/
// Description      : simulate the press key's action.
//--------------------------------------------------------------------------------------
// Input Parameter  : uint32 pressKeyName
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
PUBLIC void ATESTKEYSTEP_ProcessPressKey(uint32 pressKeyName)
{
    MmiSignalS *press_key_msg = PNULL;  // define press key variable
    
    press_key_msg->sig = pressKeyName;  // press key's value
    press_key_msg->SignalCode = KPD_DOWN;  // press key's action
    MMK_DispatchMSGKbd( &press_key_msg );
    press_key_msg->SignalCode = KPD_UP;    // press key's action
    MMK_DispatchMSGKbd( &press_key_msg );
    MMITHEME_UpdateRect();
    SCI_Sleep(200);
}

/**************************************************************************************/
// Description      : trigger next press key message.
//--------------------------------------------------------------------------------------
// Input Parameter  : uint32 press_key_state
// Output Parameter : NONE
// Return Value     : BOOLEAN VALUE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
PUBLIC void ATESTKEYSTEP_TriggerPressKeySerialMsg(uint32 pressKeyState)
{
    MmiSignalS   *sendSignal;
    
    MmiCreateSignal(0xFDFD, sizeof(MmiSignalS), (MmiSignalS**)&sendSignal);
    sendSignal->Sender = P_APP;
    MmiSendSignal(TASK_FL_ID, (MmiSignalS*)sendSignal);
}
