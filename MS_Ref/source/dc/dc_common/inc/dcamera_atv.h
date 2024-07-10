/*******************************************************************************
 ** FileName:      dcamera_atv.h                                                *
 ** Author:        Eddy.wei                                                     *
 ** Date:          2012/01/18                                                 *
 ** CopyRight:     2012, Spreatrum, Incoporated, All right reserved            *
 ** Description:                                                               *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME                   DESCRIPTION                          *
 ** 2012/01/18    Eddy.wei                  Created                             *
 *******************************************************************************/

#ifndef DCAMERA_ATV_H
#define DCAMERA_ATV_H

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
#define ATV_SCAN_CHN_TIMER 500
#define ATV_GET_RSSI_TIMER 3000

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

/******************************************************************************/
// Description: dc analog tv Rssi handler
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS : success 
//          other: fail
// Note:
/******************************************************************************/
PUBLIC uint32 DCAMERA_AtvRssiHandler(void);
/******************************************************************************/
// Description: dc analog tv handle
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      atv_param: atv cmd
// output parameter:
//                      None
// return:
//          SCI_SUCCESS : success 
//          other: fail
// Note:
/******************************************************************************/
PUBLIC uint32 DCAMERA_AtvHandle(uint32 atv_param);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /*#ifndef DC_MISC_H*/
