/******************************************************************************
 ** File Name:      fm_drvapi.h                                                  *
 ** Author:         Jackey Ling                                                   *
 ** DATE:           15/10/2012                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic input and output operations   *
 **                 on hardware, it can be treated as a hardware abstract     *
 **                 layer interface.                                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/12/2010     Xuepeng.Zhang    Create.                                   *
 ******************************************************************************/

#ifndef _FM_DRVAPI_H_
#define _FM_DRVAPI_H_

/*----------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **------------------------------------------------------------------------- */


/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
**                               Macro Define                                **
**---------------------------------------------------------------------------*/
#define SPRD_FREQ_DOWN_LIMIT 875
#define SPRD_FREQ_UP_LIMIT 1080


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function enable fm module
//  Author:         Jackey.Ling
//  Note:           
//
/*****************************************************************************/
PUBLIC void FM_SPRD_Enable(void);


/*****************************************************************************/
//  Description:    This function enable fm module
//  Author:         Jackey.Ling
//  Note:           
//
/*****************************************************************************/
PUBLIC void FM_SPRD_Disable(void);

//Add by luther.li 2012-12-01
/*****************************************************************************/
//  FUNCTION:     FM_SPRD_RecStereo
//  Description:  This function will return receive rssi strength
//  Note: is_up: 
/*****************************************************************************/
PUBLIC uint8 FM_SPRD_RecStereo(void);

/*****************************************************************************/
//  FUNCTION:     FM_SPRD_RecRSSI
//  Description:  This function will return receive rssi strength
//  Note: is_up: 
/*****************************************************************************/
PUBLIC uint16 FM_SPRD_RecRSSI(void);

/*****************************************************************************/
//  Description:    This function seek valid freq.
//  Author:         Jackey.Ling
//  Note:           
//
/*****************************************************************************/
PUBLIC uint32 FM_SPRD_Seek(uint32  freq, uint32 is_up);

/*****************************************************************************/
//  Description:    This function tune valid freq.
//  Author:         Jackey.Ling
//  Note:           
//
/*****************************************************************************/
PUBLIC uint16 FM_SPRD_Tune(uint32 freq, uint32 is_up);

/*****************************************************************************/
//  FUNCTION:     FM_SPRD_GetCurFreq
//  Description:  This function will return curent playing freq
//  Note: is_up: 
/*****************************************************************************/
PUBLIC uint16 FM_SPRD_GetCurFreq(void);
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  // _WDG_DRVAPI_H_


