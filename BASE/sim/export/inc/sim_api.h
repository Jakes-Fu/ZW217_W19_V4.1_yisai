/******************************************************************************
 ** File Name:      sim_api.h                                                 *
 ** Author:         Yongxia.zhang                                             *
 ** Date:           05/21/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This header file including the api the SIM module defined *
 **					and called by the upper layer       					  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 05/21/2002     Yongxia.zhang      create                                  *
 ******************************************************************************/

#ifndef _SIM_API_H_
#define _SIM_API_H_

//#include "sim_type.h"

#ifdef __cplusplus
    extern "C"
    {
#endif
//----------------------------function declaration-----------------------------------------
//----------------------------function called by ps_main.c---------------------------------
/**********************************************************************
//    Description:
//      The entry function of SIM layer.the major function of SIM layer.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMCARD_Task(
                  uint32 agrc,  //the argument num
                  void* argv    //the argument array
                  );

//----------------------------function called by stk module--------------------------------
/**********************************************************************
//    Description:
//      The function to collect the SIM Application Toolkit facilities
//    that are supported by the ME and make up the TEMINAL PROFILE data
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMAT_GetMeProfile(
                                uint8* profile,     //out:the Teminal profile data
                                uint16* profile_len  //in/out:when in,the length of the buf that to save the teminal profile
                                                    //out,the really terminal profile data length
                                );

const char * SCI_GetSimVersionInfo(void);

/**********************************************************************
//    Description:  get sim info. --working volt and atr
//    Global resource dependence :
//    Author:shuyan.hu
//    Note: This is only for DIAG tools!
***********************************************************************/
void SIMHAL_GetAtrBuf(uint8 *buff, uint8 card_id);

/**********************************************************************
//    Description:  get SIM ATR raw data
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_GetAtrRawData(uint8 card_id, uint16 *len, uint8 **buff);

/**********************************************************************
//    Description:
//      the function that send the SIM initialization requset to SIM task
//    Global resource dependence :
//    Author:Jacky.ying
//    Note:
***********************************************************************/
void SIMSIGNAL_SIMDphoneInitReq (uint32 cur_sim_task, BOOLEAN is_entire_service);


#ifdef __cplusplus
    }
#endif
#endif  //_SIM_API_H_
