/******************************************************************************
 ** File Name:      layer1_ps_if.h                                            *
 ** Author:         Fancier.Fan                                               *
 ** DATE:           08/02/2006                                                *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/02/2006     Fancier.Fan      Create.                                   *
 ******************************************************************************/

#ifndef _LAYER1_PS_IF_H
#define _LAYER1_PS_IF_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifdef _RTOS

#include "sci_types.h"
#include "l1macros.h"
#include "gsm_gprs.h"

#else
#include  "..\..\..\..\RTOS\export\inc\sci_types.h"
#include "l1macros.h"
//#include "gsm_gprs.h"

#endif
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

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*-------------------------------------------------
	FUNCTION NAME:	SystemInfo_CheckIfGetIntFromDsp
	DESCRIPTION: Gprs alloc block
	NOTES:	@Jenny.xu
--------------------------------------------------*/
extern void	GprsSysInfo_FreeBlock(
	BLOCK_STRUCT_T** p_block
	);

/*-------------------------------------------------
	FUNCTION NAME:	SystemInfo_CheckIfGetIntFromDsp
	DESCRIPTION: check this static variable
	NOTES:	@Jenny.xu
--------------------------------------------------*/
void SystemInfo_CheckIfGetIntFromDsp(
	void
	);

/*-----------------------------------------------------------
	FUNCTION NAME:	Layer1_Mac_GetAbsoluteFn
	DESCRIPTION:	Layer1 and Mac should call this function 
					to transfer reduced fn to absolute fn
	NOTES:			
-----------------------------------------------------------*/
Fn_T Layer1_Mac_GetAbsoluteFn(
		REDUCED_FRAME_NUMBER_T reduced_fn
		);

/*----------------------------------------------------------------------------
	FUNCATION NAME:	L1Main_CheckIfBCCHCollide
	Description:    modifiy for avoiding the colliding of two continue BCCH In Dual Sim Code
	                0, not collide
	                1, collide
	Notes: 	        @
-----------------------------------------------------------------------------*/
extern BOOLEAN L1Main_CheckIfBCCHCollide(ARFCN_T arfcn_old, MS_BAND_E old_band_ind, ARFCN_T arfcn_new, MS_BAND_E new_band_ind);

/*-----------------------------------------------------------
	Function Name:	Layer1_GetNcellDeltaFn
	Description:	Layer1 provide api function for rr to get delta fn.
-----------------------------------------------------------*/
extern Fn_T Layer1_GetNcellDeltaFn(ARFCN_T arfcn, MS_BAND_E band_ind);

/*-------------------------------------------------------------------------
    FUNCTION NAME: GprsSysInfo_GetDownlinkTbfSlotMask
    DESCRIPTION:   get the exe downlink link slot mask if it present 
    NOTE:          used by mac 
    AUTHOR:        suzhi.hao
-------------------------------------------------------------------------*/
extern uint8 GprsSysInfo_GetDownlinkTbfSlotMask(void);

/*-------------------------------------------------------------------------
    FUNCTION NAME: GprsSysInfo_FindExeTBFParams
    DESCRIPTION: find exe tbf params 
    NOTE: 
    AUTHOR: tom.fu
-------------------------------------------------------------------------*/
extern uint8 GprsSysInfo_GetDownlinkSlotMask(void);

/*-------------------------------------------------------------------------
    FUNCTION NAME: GprsSysInfo_GetUplinkTbfSlotMask
    DESCRIPTION:   get the exe uplink link slot mask if it present 
    NOTE:          used by mac 
    AUTHOR:        suzhi.hao
-------------------------------------------------------------------------*/
extern uint8 GprsSysInfo_GetUplinkTbfSlotMask(void);

#ifdef MULTI_SIM_SYS_QUAD
/*-------------------------------------------------------------------------
    FUNCTION NAME: L1Main_SetCardNum
    DESCRIPTION: 1.used by ps to set card num; 2.set event for layer1 to download dsp code
    NOTE: according to cr225969
-------------------------------------------------------------------------*/
extern void L1Main_SetCardNumAndSetSimEvent(uint8 card_num);

/*-------------------------------------------------------------------------
    FUNCTION NAME: L1Main_CheckIfQualSimReady
    DESCRIPTION:
    NOTE: according to cr225969
-------------------------------------------------------------------------*/
extern BOOLEAN L1Main_CheckIfQualSimReady(void);

/*-------------------------------------------------------------------------
    FUNCTION NAME: L1Main_GetSimEventForDspDownLoad
    DESCRIPTION: 1.used by ps to set card num; 2.set event for layer1 to download dsp code
    NOTE: according to cr225969
-------------------------------------------------------------------------*/
extern void L1Main_GetSimEventForDspDownLoad(void);

/*-------------------------------------------------------------------------
    FUNCTION NAME: L1Main_CheckIfCaliMode
    DESCRIPTION: 
    NOTE: according to cr225969
-------------------------------------------------------------------------*/
extern BOOLEAN L1Main_CheckIfCaliMode(void);

extern void L1Main_GetDspReadyToAppEvent(void);
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }  
#endif

#endif  // _LAYER1_PS_IF_H
