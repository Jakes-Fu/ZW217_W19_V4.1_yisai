/******************************************************************************
 ** File Name:      ms_ref_mtp_trc.h                                         *
 ** DATE:           2012.03.18                                                *
 ** Copyright:      Copyright 2012 by Spreadtrum Communications,Inc.          *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************
 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **---------------------------------------------------------------------------*
 **  2012.03.18                         Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//trace_id:59
#ifndef _MS_REF_MTP_TRC_H_
#define _MS_REF_MTP_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MTP_DRIVER_345_112_2_18_1_49_14_0 "\nMTP_bind() alloc req fail\n"
#define MTP_DRIVER_378_112_2_18_1_49_14_1 "\nMTP_bind() error\n"
#define MTP_DRIVER_387_112_2_18_1_49_14_2 "\nusb_ep_alloc_request(fsg->bulk_in) error\n"
#define MTP_DRIVER_394_112_2_18_1_49_14_3 "\nusb_ep_alloc_request(fsg->bulk_out) error\n"
#define MTP_DRIVER_402_112_2_18_1_49_14_4 "\nusb_ep_alloc_request(fsg->bulk_out) error\n"
#define MTP_DRIVER_492_112_2_18_1_49_15_5 "\nUSB_ERR_INVALID_CLASSCMD\n"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_MTP_TRC)
TRACE_MSG(MTP_DRIVER_345_112_2_18_1_49_14_0,"\nMTP_bind() alloc req fail\n")
TRACE_MSG(MTP_DRIVER_378_112_2_18_1_49_14_1,"\nMTP_bind() error\n")
TRACE_MSG(MTP_DRIVER_387_112_2_18_1_49_14_2,"\nusb_ep_alloc_request(fsg->bulk_in) error\n")
TRACE_MSG(MTP_DRIVER_394_112_2_18_1_49_14_3,"\nusb_ep_alloc_request(fsg->bulk_out) error\n")
TRACE_MSG(MTP_DRIVER_402_112_2_18_1_49_14_4,"\nusb_ep_alloc_request(fsg->bulk_out) error\n")
TRACE_MSG(MTP_DRIVER_492_112_2_18_1_49_15_5,"\nUSB_ERR_INVALID_CLASSCMD\n")
END_TRACE_MAP(MS_REF_MTP_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_MTP_TRC_H_

