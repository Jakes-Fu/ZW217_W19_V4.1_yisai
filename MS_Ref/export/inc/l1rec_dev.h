/******************************************************************************
 ** File Name:      l1rec_dev.h                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           10/04/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    We abstract layer1(DSP) recorder as an record device and 
 **                     match it with audio service device manager.     * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/04/2006     Benjamin.Wang     Create.                                  *
 ******************************************************************************/

#ifndef _L1REC_DEV_H
#define _L1REC_DEV_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Register l1rec_dev to audio service.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV L1REC_RegAudioDevice(void);

/*****************************************************************************/
//  Description:    Config layer1(DSP) record parameters.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
PUBLIC void L1REC_ConfigRecordDevice(
    AUD_REC_FORMAT_E eRecordFormat,     // record format
    BOOLEAN bIsfromMic,     // Input from Mic?
    BOOLEAN bIsfromNet      // Input from Air?
    );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _L1REC_DEV_H

// End of l1rec_dev.h

