/****************************************************************************************
** File Name:		L1c_Command.h				    							        *
** Author:			Jenny Xu															*
** DATE:			06/21/2005															*
** CopyRight		2005 SpreadTrum Communications CORP. All Rights Reserved.			*
** Description:		This is the function of Physical Layer								*
**					It contains all functions to handle DSP command related procssing   * 
**					plus shared memory.   	                                            *
*****************************************************************************************

*****************************************************************************************
**					Edit History														*
** -------------------------------------------------------------------------------------*
**																						*
** DATE:			NAME				DESCIPTION										*
** 06/21/2005		Jenny Xu			Create.											*
**                                                                                      *
*****************************************************************************************/
#ifndef _COMMAND_H
#define _COMMAND_H

/**-------------------------------------------------------------------------------------*
 **                         Dependencies												*
 **-------------------------------------------------------------------------------------*/
#include "sci_types.h"

/**-------------------------------------------------------------------------------------*
 **                         Macro definition                                            *
 **-------------------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

//define callback function type
typedef void (*RECORD_CALLBACK)(uint16 voice_data_length);
typedef BOOLEAN (*PLAY_CALLBACK)(void);

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/* for Vedio Phone */                      
extern void  l1_regrecordcallbackfun(RECORD_CALLBACK record_callback);
extern void  l1_regplaycallbackfun(PLAY_CALLBACK play_callback);
extern void  l1_readvoicedata(uint16 *voice_data_ptr, uint16 voice_data_length);
extern void  l1_writevoicedata(uint16 *voice_data_ptr, uint16 voice_data_length);    
extern void  l1_rwvoicedata(uint16 voice_data_word_length);

/*----------------------------------------------------------------------
        Description:  This function is used to control voice band
------------------------------------------------------------------------*/
extern void Command_VoiceBandCtrl(
                          uint16 cmd_type,       //control type
                          uint16 para0,          //control para0
                          uint16 para1,          //control para1
                          uint16 para2           //control para2 
                      );

/*----------------------------------------------------------------------
        Description:  This function is used to control voice band
------------------------------------------------------------------------*/
extern void Command_PrintTestPoint(
                          uint16 address,          //control para0
                          uint16 value              //control para1
                      );
		
extern void l1_dspSleepForever(BOOLEAN is_sleep_forever);

/*----------------------------------------------------------------------
        Description:  This function is used to set testloop mode2 , enable or disable
------------------------------------------------------------------------*/
extern void Command_SetTestLoopMode2Cmd(
                          uint16  loop_mode       
                       );

extern void  l1_setdspresetentry(void);
/*----------------------------------------------------------------------
        Description:  This function is used to note dsp start gsm meas.
------------------------------------------------------------------------*/
extern void Command_StartGsmMeasCmd(
                          BOOLEAN  is_start_gsm_meas       
                       );

/*----------------------------------------------------------------------
        Description:  This function is used to note dsp rat change to GSM.
------------------------------------------------------------------------*/
extern void Command_RatChangeCmd(
                          BOOLEAN  change_to_gsm_mode
                       );
/*----------------------------------------------------------------------
        Description:  This function is used to note dsp ARM is Ready.
------------------------------------------------------------------------*/
extern void Command_ArmIsReadyCmd(
                          void
                       );		
#endif
