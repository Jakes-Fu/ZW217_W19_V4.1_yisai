/***************************************************************************************
** FileName    :  atest_key_step.h                                                    **
** Author      :  Mingxuan.Zhu                                                        **
** Date        :  01/06/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  The action of the press key message is defined in this file.        **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  01/06/2010     Mingxuan.Zhu         Created                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#ifndef _ATEST_KEY_STEP_H
#define _ATEST_KEY_STEP_H

/**-----------------------------------------------------------------------------------*
 **                                   Dependencies                                    *
 **-----------------------------------------------------------------------------------*/
#include "mmk_app.h"

/**-----------------------------------------------------------------------------------*
 **                                Function Prototypes                                *
 **-----------------------------------------------------------------------------------*/

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
PUBLIC BOOLEAN ATESTKEYSTEP_IsPressKeySerialMsg(MmiSignalS** signal_ptr);

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
PUBLIC void ATESTKEYSTEP_ProcessPressKey(uint32 pressKeyName);

/**************************************************************************************/
// Description      : trigger next press key message.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : BOOLEAN VALUE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
PUBLIC void ATESTKEYSTEP_TriggerPressKeySerialMsg(uint32 pressKeyState);

#endif//_ATEST_KEY_STEP_H