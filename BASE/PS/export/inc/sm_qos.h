/******************************************************************************
 ** File Name:      sm_qos.h                                                  *
 ** Author:         Tracy Zhang                                               *
 ** Date:           2008-09-25                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 2008-09-25     Tracy Zhang      Create.                                   *
 ******************************************************************************/
#ifndef _SM_QOS_H_
#define _SM_QOS_H_

#include "sci_types.h"
#include "comm_def.h"
#include "gsm_gprs.h"

/*****************************************************************************/
//  Description: The procedure decodes the R99 QoS from the r97/98 QoS 
//  received from the network or from the ms, and stores it in a structure.
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 3GPP TS 23.107 section 9.1.2.2 Table 6
//        3GPP TS 24.008 section 10.5.6.5 Table 10.5.156
/****************************************************************************/
extern void Qos_R97_to_R99(
                    TOTAL_QOS_T     *qos_ptr        //IN/OUT:
                    );

/*****************************************************************************/
//  Description: The procedure decodes the R97/98 QoS from the R99 QoS 
//  received from the network or from the ms, and stores it in a structure.
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 3GPP TS 23.107 section 9.1.2.3 Table 7
//        3GPP TS 24.008 section 10.5.6.5 Table 10.5.156
//        Is mapped when a R99 UE (except UMTS only UE) receives a request for 
//        a PDP Context Activation with R99 QoS attributes.
/****************************************************************************/
extern void Qos_R99_to_R97(
                    TOTAL_QOS_T     *qos_ptr        //IN/OUT:
                    );

/*****************************************************************************/
//  Description: The procedure set the QoS
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 3GPP TS 23.107 section 9.1.2.2 Table 6
//        3GPP TS 24.008 section 10.5.6.5 Table 10.5.156
/****************************************************************************/
extern BOOLEAN SM_SetQos(
                  BOOLEAN         is_set_qmin,    //IN: true, is set min qos
                  TOTAL_QOS_T     *qos_ptr        //IN/OUT:
                  );

/*****************************************************************************/
//  Description : compare the qos parameter
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: the function suppose the qos is valid
/*****************************************************************************/
extern int32 SM_CompareQosValid(                           //RETURN:
                         TOTAL_QOS_T    *qos1_ptr,  //IN:
                         TOTAL_QOS_T    *qos2_ptr   //IN:
                         );

/*****************************************************************************/
//  Description : Set the qos subscribed value
//  Global resource dependence : none
//  Author:hyman.wu
//  Note:
/*****************************************************************************/
extern void SM_SetSubscribeQosValue(
                             TOTAL_QOS_T* qos_ptr
                             );



/*****************************************************************************/
//  Description: Set the QoS in the valid range, just for R97/98
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
extern void SM_SetVaildQos_R97(
                        TOTAL_QOS_T *qos_ptr
                        );

/*****************************************************************************/
//  Description: Set the QoS in the valid range, just for R99
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
extern void SM_SetVaildQos_R99(
                        BOOLEAN     is_set_qmin,    //IN: true, is set min qos
                        TOTAL_QOS_T *qos_ptr
                        );

#endif
