/******************************************************************************
 ** File Name:      ms_ref_ubot_trc.h                                         *
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
//trace_id:72
#ifndef _MS_REF_UBOT_TRC_H_
#define _MS_REF_UBOT_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define UBOT_CONFIG_233_112_2_18_1_54_43_0 "udisk capatcity overflow, pls reduce udisk capacity below cache size if you want to use pc connect func!!!"
#define UBOT_DRV_1265_112_2_18_1_54_46_1 "@@@@@ubot test start"
#define UBOT_DRV_1269_112_2_18_1_54_46_2 "@@@@@test case%d"
#define UBOT_DRV_1296_112_2_18_1_54_46_3 "@@@@@case%d,read, %d"
#define UBOT_DRV_1306_112_2_18_1_54_46_4 "@@@@@case%d,write, %d"
#define UBOT_DRV_1319_112_2_18_1_54_46_5 "@@@@@test loop, %d"
#define UBOT_DRV_1337_112_2_18_1_54_46_6 "@@@@@test case%d"
#define UBOT_DRV_1363_112_2_18_1_54_46_7 "@@@@@case%d,read, %d"
#define UBOT_DRV_1373_112_2_18_1_54_46_8 "@@@@@case%d,write, %d"
#define UBOT_DRV_1386_112_2_18_1_54_46_9 "@@@@@test loop, %d"
#define UBOT_DRV_1404_112_2_18_1_54_46_10 "@@@@@test case%d"
#define UBOT_DRV_1429_112_2_18_1_54_46_11 "@@@@@case%d,read, %d"
#define UBOT_DRV_1439_112_2_18_1_54_47_12 "@@@@@case%d,write, %d"
#define UBOT_DRV_1453_112_2_18_1_54_47_13 "@@@@@test loop, %d"
#define UBOT_DRV_1469_112_2_18_1_54_47_14 "@@@@@test completed!!"
#define UBOT_MAIN_224_112_2_18_1_54_47_15 "\nUSB_ERR_INVALID_CLASSCMD\n"
#define UBOT_MAIN_367_112_2_18_1_54_47_16 "UBOT get maxlun error"
#define UBOT_STORAGE_111_112_2_18_1_54_49_17 "\nUBOT_ProcReady() enter\n"
#define UBOT_STORAGE_114_112_2_18_1_54_49_18 "\nUBOT_ProcReady() startreceivedata\n"
#define UBOT_STORAGE_120_112_2_18_1_54_49_19 "\nUBOT_ProcReady() exit\n"
#define UBOT_STORAGE_125_112_2_18_1_54_49_20 "\nUBOT_ProcReady() UBOT_STATE_CBW\n"
#define UBOT_STORAGE_138_112_2_18_1_54_49_21 "\nUBOT_ProcCBW() enter\n"
#define UBOT_STORAGE_147_112_2_18_1_54_49_22 "\nUBOT_ProcCBW() err\n"
#define UBOT_STORAGE_160_112_2_18_1_54_49_23 "\nUBOT_ProcCBW() cmd err\n"
#define UBOT_STORAGE_166_112_2_18_1_54_49_24 "\nUBOT_ProcCBW() read fail\n"
#define UBOT_STORAGE_172_112_2_18_1_54_49_25 "\nUBOT_ProcCBW() data in\n"
#define UBOT_STORAGE_176_112_2_18_1_54_49_26 "\nUBOT_ProcCBW() data out\n"
#define UBOT_STORAGE_227_112_2_18_1_54_49_27 "UBOT_ProcDataIn() CSW 1"
#define UBOT_STORAGE_236_112_2_18_1_54_49_28 "UBOT_ProcDataIn() CSW 2"
#define UBOT_STORAGE_290_112_2_18_1_54_49_29 "UBOT_ProcCSW()"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_UBOT_TRC)
TRACE_MSG(UBOT_CONFIG_233_112_2_18_1_54_43_0,"udisk capatcity overflow, pls reduce udisk capacity below cache size if you want to use pc connect func!!!")
TRACE_MSG(UBOT_DRV_1265_112_2_18_1_54_46_1,"@@@@@ubot test start")
TRACE_MSG(UBOT_DRV_1269_112_2_18_1_54_46_2,"@@@@@test case%d")
TRACE_MSG(UBOT_DRV_1296_112_2_18_1_54_46_3,"@@@@@case%d,read, %d")
TRACE_MSG(UBOT_DRV_1306_112_2_18_1_54_46_4,"@@@@@case%d,write, %d")
TRACE_MSG(UBOT_DRV_1319_112_2_18_1_54_46_5,"@@@@@test loop, %d")
TRACE_MSG(UBOT_DRV_1337_112_2_18_1_54_46_6,"@@@@@test case%d")
TRACE_MSG(UBOT_DRV_1363_112_2_18_1_54_46_7,"@@@@@case%d,read, %d")
TRACE_MSG(UBOT_DRV_1373_112_2_18_1_54_46_8,"@@@@@case%d,write, %d")
TRACE_MSG(UBOT_DRV_1386_112_2_18_1_54_46_9,"@@@@@test loop, %d")
TRACE_MSG(UBOT_DRV_1404_112_2_18_1_54_46_10,"@@@@@test case%d")
TRACE_MSG(UBOT_DRV_1429_112_2_18_1_54_46_11,"@@@@@case%d,read, %d")
TRACE_MSG(UBOT_DRV_1439_112_2_18_1_54_47_12,"@@@@@case%d,write, %d")
TRACE_MSG(UBOT_DRV_1453_112_2_18_1_54_47_13,"@@@@@test loop, %d")
TRACE_MSG(UBOT_DRV_1469_112_2_18_1_54_47_14,"@@@@@test completed!!")
TRACE_MSG(UBOT_MAIN_224_112_2_18_1_54_47_15,"\nUSB_ERR_INVALID_CLASSCMD\n")
TRACE_MSG(UBOT_MAIN_367_112_2_18_1_54_47_16,"UBOT get maxlun error")
TRACE_MSG(UBOT_STORAGE_111_112_2_18_1_54_49_17,"\nUBOT_ProcReady() enter\n")
TRACE_MSG(UBOT_STORAGE_114_112_2_18_1_54_49_18,"\nUBOT_ProcReady() startreceivedata\n")
TRACE_MSG(UBOT_STORAGE_120_112_2_18_1_54_49_19,"\nUBOT_ProcReady() exit\n")
TRACE_MSG(UBOT_STORAGE_125_112_2_18_1_54_49_20,"\nUBOT_ProcReady() UBOT_STATE_CBW\n")
TRACE_MSG(UBOT_STORAGE_138_112_2_18_1_54_49_21,"\nUBOT_ProcCBW() enter\n")
TRACE_MSG(UBOT_STORAGE_147_112_2_18_1_54_49_22,"\nUBOT_ProcCBW() err\n")
TRACE_MSG(UBOT_STORAGE_160_112_2_18_1_54_49_23,"\nUBOT_ProcCBW() cmd err\n")
TRACE_MSG(UBOT_STORAGE_166_112_2_18_1_54_49_24,"\nUBOT_ProcCBW() read fail\n")
TRACE_MSG(UBOT_STORAGE_172_112_2_18_1_54_49_25,"\nUBOT_ProcCBW() data in\n")
TRACE_MSG(UBOT_STORAGE_176_112_2_18_1_54_49_26,"\nUBOT_ProcCBW() data out\n")
TRACE_MSG(UBOT_STORAGE_227_112_2_18_1_54_49_27,"UBOT_ProcDataIn() CSW 1")
TRACE_MSG(UBOT_STORAGE_236_112_2_18_1_54_49_28,"UBOT_ProcDataIn() CSW 2")
TRACE_MSG(UBOT_STORAGE_290_112_2_18_1_54_49_29,"UBOT_ProcCSW()")
END_TRACE_MAP(MS_REF_UBOT_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_UBOT_TRC_H_

