/****************************************************************************************
** File Name:		layer1_voicecall.c			    									*			
** Author:			Jenny Xu															*
** DATE:			11/10/2003															*
** CopyRight		2001 SpreadTrum Communications CORP. All Rights Reserved.			*
** Description:		This C file is to handle all operations for HTW's voice call between*
**					 MCU and DSP on 2k Shared Dual-Port Memory.	        				*	    
*****************************************************************************************


*****************************************************************************************
**					Edit History														*	
** -------------------------------------------------------------------------------------*
**																						*
** DATE:			NAME				DESCIPTION										*
** 11/10/2003		Jenny Xu			Create.											*
*****************************************************************************************/

#ifndef _LAYER1_VOICECALL_H
#define	_LAYER1_VOICECALL_H

#include "sci_types.h"

/**-------------------------------------------------------------------------------------*
 **                         Dependencies												*
 **-------------------------------------------------------------------------------------*/
 
/*--------------------------------------------------------------------------------
	DESCRIPTION: start to transfer voice call, do some initialize, and then send 
				 a command to dsp .
---------------------------------------------------------------------------------*/
extern void SCI_StartTransferVoiceData(void);

/*--------------------------------------------------------------------------------
	DESCRIPTION: stop to transfer voice call, and send a command to dsp.
---------------------------------------------------------------------------------*/
extern void SCI_StopTransferVoiceData(void);

/*--------------------------------------------------------------------------------
	DESCRIPTION: start to transfer voice call, do some initialize
---------------------------------------------------------------------------------*/
extern uint16  SCI_TransferVoiceData(int16 *buffer,uint16 size);

/*--------------------------------------------------------------------------------
	DESCRIPTION: Enable or disable mute, then return last time's state.
---------------------------------------------------------------------------------*/
extern BOOLEAN SCI_EnableMute(BOOLEAN is_mute_on);


#endif
