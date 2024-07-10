/******************************************************************************
 ** File Name:      keypad.h                                                  *
 ** Author:         Richard Yang                                              *
 ** DATE:           21/11/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 21/11/2001     Richard.Yang     Create.                                   *
 ******************************************************************************/

#ifndef _KEYPAD_H
#define _KEYPAD_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "tb_dal.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef struct KPDSVR_SIG_tag
{
	//xSignalHeaderRec signal_head;
	SIGNAL_VARS 
    int              key;
} KPDSVR_SIG_T;


typedef uint32 (*GPIO_EXPAND_KEY_CALLBACK_PFUNC)(void);

typedef struct
{
	BOOLEAN       is_key_expand;
	uint8         keyin_num;
	uint32        key_code[8];
	GPIO_EXPAND_KEY_CALLBACK_PFUNC		gpio_callback;
}GPIO_EXPAND_KEY_GPIO_INFO_T;

typedef KPDSVR_SIG_T    yPDef_KEY_DOWN;
typedef KPDSVR_SIG_T    yPDef_KPD_UP;
typedef KPDSVR_SIG_T    yPDef_KPD_SOFT_POWER_ON;
typedef KPDSVR_SIG_T    yPDef_KPD_SOFT_POWER_OFF;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Main thread of KPDSVR task. It manages the keypad signal
//                  from driver, and send it to the tasks which registered to 
//                  it before. It also processes PowerOn/PowerOff operation.
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC void thread_kpdsvr_entry(uint32 argc, void *argv);

/*****************************************************************************/
//  Description:    This function is used to Register expand key by gpio
//  Author:        hanjun.liu
//  Note:           
/*****************************************************************************/
PUBLIC  BOOLEAN KPD_Reg_Expand_Key(
                                   uint8              keyin_num,   //keyin num 
					GPIO_EXPAND_KEY_CALLBACK_PFUNC	gpio_callback_fun
					   	);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /* End keypad.h*/
