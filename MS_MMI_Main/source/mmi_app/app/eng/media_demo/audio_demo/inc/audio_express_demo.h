/*****************************************************************************
** File Name:      audio_express_demo.h                                                  *
** Author:                                                                   *
** Date:           11/10/2010                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to audio express demo               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2010        hai.li           Create
******************************************************************************/

#ifndef _AUDIO_EXPRESS_DEMO_H_
#define _AUDIO_EXPRESS_DEMO_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    Reg express  funcation
//  Author:        
//  Note:
//****************************************************************************/  
PUBLIC void AUDIO_Express_Demo_RegExp(void);

/*****************************************************************************/
//  Description:   UnReg express funcation
//  Author:        
//  Note:
//****************************************************************************/  
PUBLIC void AUDIO_Express_Demo_UnRegExp(void);


/*****************************************************************************/
//  Description : API of  start demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void AUDIO_Express_Demo_Start(void);


/*****************************************************************************/
//  Description : API of stop demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void AUDIO_Express_Demo_Stop(void);


/*****************************************************************************/
//  Description:   set express par
//  Author:        
//  Note:
//****************************************************************************/  
PUBLIC void AUDIO_Express_Demo_SetPar(uint8 aud_para);
/*****************************************************************************/
//  Description:   play mp3 on SD card
//  Author:        
//  Note:
//****************************************************************************/  

PUBLIC BOOLEAN MP3_Express_Demo_Start(void);
/*****************************************************************************/
//  Description:   stop mp3 on SD card
//  Author:        
//  Note:
//****************************************************************************/  

PUBLIC void MP3_Express_Demo_Stop(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

