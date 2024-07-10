/****************************************************************************************
** File Name:		layer1_midi.h															*	
** Author:			Jenny Xu															*
** DATE:			05/30/2003															*
** CopyRight		2002 SpreadTrum Communications CORP. All Rights Reserved.			*
** Description:		This header file is to handle all operations for Interface between  *
**					MCU	and DSP	on 2k Shared Dual-Port Memory.							*	    
**					More detailed Info. could be seen in Doc.MCU and DSP interface Spec *
*****************************************************************************************
**					Edit History														*	
** -------------------------------------------------------------------------------------*
**																						*
** DATE:			NAME				DESCIPTION										*
** 05/30/2003		Jenny Xu			Create.											*
*****************************************************************************************/

#ifndef _LAYER1_MIDI_H
#define	_LAYER1_MIDI_H

#include "sci_types.h"
/**-------------------------------------------------------------------------------------*
 **                         Dependencies												*
 **-------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------
	DESCRIPTION:initialize static variables for MIDI module
---------------------------------------------------------------------------------*/
extern void L1Midi_Init(void);

/*--------------------------------------------------------------------------------
	DESCRIPTION:	write midi data to shared memory
---------------------------------------------------------------------------------*/
extern void L1Midi_OutputData(uint16 *buf, uint32 count);

/*--------------------------------------------------------------------------------
	DESCRIPTION: send a command to DSP to stop MIDI planning
---------------------------------------------------------------------------------*/
extern void L1Midi_Stop(void);

/*--------------------------------------------------------------------------------
	DESCRIPTION: send a command to DSP to set MIDI volume.
---------------------------------------------------------------------------------*/
extern void L1Midi_SetVolume(uint16 volume);

/*--------------------------------------------------------------------------------
	DESCRIPTION: To check if midi is stoped
---------------------------------------------------------------------------------*/
extern BOOLEAN  L1Midi_GetMidIsStoped(void);

/*--------------------------------------------------------------------------------
	DESCRIPTION:	get buffer number for wave data  in  shared memory
---------------------------------------------------------------------------------*/
extern uint16 L1Wave_BufferNum(void);

#endif
