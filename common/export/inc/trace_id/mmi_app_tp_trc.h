/******************************************************************************
 ** File Name:      mmi_app_tp_trc.h                                         *
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
//trace_id:229
#ifndef _MMI_APP_TP_TRC_H_
#define _MMI_APP_TP_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define COORDINATE_WINTAB_261_112_2_18_3_3_30_0 "MMI:COORDINATE_HandleMsg(0x%x,0x%x)"
#define COORDINATE_WINTAB_304_112_2_18_3_3_30_1 "COORDINATE.C param = %d"
#define COORDINATE_WINTAB_309_112_2_18_3_3_30_2 "COORDINATE.C mid : x = %d, y = %d"
#define COORDINATE_WINTAB_314_112_2_18_3_3_30_3 "COORDINATE.C param = %d"
#define COORDINATE_WINTAB_319_112_2_18_3_3_30_4 "COORDINATE.C right top: x = %d, y = %d"
#define COORDINATE_WINTAB_324_112_2_18_3_3_30_5 "COORDINATE.C param = %d"
#define COORDINATE_WINTAB_329_112_2_18_3_3_30_6 "COORDINATE.C left bottom: x = %d, y = %d"
#define COORDINATE_WINTAB_479_112_2_18_3_3_31_7 "MMI:COORDINATE_OpenWin(), alloc failed"
#define COORDINATE_WINTAB_661_112_2_18_3_3_31_8 "PointVerify (x,y,adc_x,adc_y)=%d,%d"
#define COORDINATE_WINTAB_691_112_2_18_3_3_31_9 "COORDINATE.C PointVerify: x=%d, adc_x=%d, min=%d, max=%d"
#define COORDINATE_WINTAB_692_112_2_18_3_3_31_10 "COORDINATE.C PointVerify: y=%d, adc_y=%d, min=%d, max=%d"
#define COORDINATE_WINTAB_884_112_2_18_3_3_31_11 "[VINE]--Not match the scale of x and y(1)\r\n"
#define COORDINATE_WINTAB_893_112_2_18_3_3_32_12 "[VINE]--Not match the scale of x and y(2)\r\n"
#define COORDINATE_WINTAB_905_112_2_18_3_3_32_13 "COORDINATE.C IsVerifySuccess: right top corner."
#define COORDINATE_WINTAB_910_112_2_18_3_3_32_14 "COORDINATE.C IsVerifySuccess: left bottom corner."
#define COORDINATE_WINTAB_915_112_2_18_3_3_32_15 "COORDINATE.C IsVerifySuccess: center."
#define COORDINATE_WINTAB_1272_112_2_18_3_3_33_16 "tp_cali_fun is null\r\n"
#define COORDINATE_WINTAB_1289_112_2_18_3_3_33_17 "COORDINATE.C successed."
#define COORDINATE_WINTAB_1302_112_2_18_3_3_33_18 "COORDINATE.C update failed."
#define COORDINATE_WINTAB_1343_112_2_18_3_3_33_19 "COORDINATE.C s_right_top_adc_x = %d"
#define COORDINATE_WINTAB_1344_112_2_18_3_3_33_20 "COORDINATE.C s_right_top_adc_y = %d"
#define COORDINATE_WINTAB_1345_112_2_18_3_3_33_21 "COORDINATE.C s_left_bottom_adc_x = %d"
#define COORDINATE_WINTAB_1346_112_2_18_3_3_33_22 "COORDINATE.C s_left_bottom_adc_y = %d"
#define COORDINATE_WINTAB_1347_112_2_18_3_3_33_23 "COORDINATE.C s_mid_adc_x = %d"
#define COORDINATE_WINTAB_1348_112_2_18_3_3_33_24 "COORDINATE.C s_mid_adc_y = %d"
#define COORDINATE_WINTAB_1349_112_2_18_3_3_33_25 "COORDINATE.C tp_info_ptr->tp_width = %d"
#define COORDINATE_WINTAB_1350_112_2_18_3_3_33_26 "COORDINATE.C tp_info_ptr->tp_lcd_height = %d"
#define COORDINATE_WINTAB_1409_112_2_18_3_3_33_27 "COORDINATE.C IsVerifySuccess: right top corner."
#define COORDINATE_WINTAB_1413_112_2_18_3_3_33_28 "COORDINATE.C IsVerifySuccess: left bottom corner."
#define COORDINATE_WINTAB_1417_112_2_18_3_3_33_29 "COORDINATE.C IsVerifySuccess: center."
#define COORDINATE_WINTAB_1420_112_2_18_3_3_33_30 "COORDINATE.C IsVerifySuccess: set_success=%d."
#define COORDINATE_WINTAB_1422_112_2_18_3_3_33_31 "COORDINATE.C IsVerifySuccess: right top corner."
#define COORDINATE_WINTAB_1426_112_2_18_3_3_33_32 "COORDINATE.C IsVerifySuccess: left bottom corner."
#define COORDINATE_WINTAB_1484_112_2_18_3_3_33_33 "COORDINATE.C new cal: x_min_top=%d"
#define COORDINATE_WINTAB_1485_112_2_18_3_3_33_34 "COORDINATE.C new cal: x_max_top=%d"
#define COORDINATE_WINTAB_1486_112_2_18_3_3_33_35 "COORDINATE.C new cal: x_top_factor=%d"
#define COORDINATE_WINTAB_1488_112_2_18_3_3_33_36 "COORDINATE.C new cal: x_min_bottom=%d"
#define COORDINATE_WINTAB_1489_112_2_18_3_3_33_37 "COORDINATE.C new cal: x_max_bottom=%d"
#define COORDINATE_WINTAB_1490_112_2_18_3_3_33_38 "COORDINATE.C new cal: x_bottom_factor=%d"
#define COORDINATE_WINTAB_1492_112_2_18_3_3_33_39 "COORDINATE.C new cal: y_min_left=%d"
#define COORDINATE_WINTAB_1493_112_2_18_3_3_33_40 "COORDINATE.C new cal: y_max_left=%d"
#define COORDINATE_WINTAB_1494_112_2_18_3_3_33_41 "COORDINATE.C new cal: y_left_factor=%d"
#define COORDINATE_WINTAB_1496_112_2_18_3_3_33_42 "COORDINATE.C new cal: y_min_right=%d"
#define COORDINATE_WINTAB_1497_112_2_18_3_3_33_43 "COORDINATE.C new cal: y_max_right=%d"
#define COORDINATE_WINTAB_1498_112_2_18_3_3_33_44 "COORDINATE.C new cal: y_right_factor=%d"
#define COORDINATE_WINTAB_1500_112_2_18_3_3_33_45 "COORDINATE.C new cal: x_min_up=%d"
#define COORDINATE_WINTAB_1501_112_2_18_3_3_33_46 "COORDINATE.C new cal: y_min_up=%d"
#define COORDINATE_WINTAB_1505_112_2_18_3_3_33_47 "COORDINATE.C failed."
#define COORDINATE_WINTAB_1510_112_2_18_3_3_33_48 "COORDINATE.C successed."
#define COORDINATE_WINTAB_1516_112_2_18_3_3_33_49 "COORDINATE.C update failed."
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_TP_TRC)
TRACE_MSG(COORDINATE_WINTAB_261_112_2_18_3_3_30_0,"MMI:COORDINATE_HandleMsg(0x%x,0x%x)")
TRACE_MSG(COORDINATE_WINTAB_304_112_2_18_3_3_30_1,"COORDINATE.C param = %d")
TRACE_MSG(COORDINATE_WINTAB_309_112_2_18_3_3_30_2,"COORDINATE.C mid : x = %d, y = %d")
TRACE_MSG(COORDINATE_WINTAB_314_112_2_18_3_3_30_3,"COORDINATE.C param = %d")
TRACE_MSG(COORDINATE_WINTAB_319_112_2_18_3_3_30_4,"COORDINATE.C right top: x = %d, y = %d")
TRACE_MSG(COORDINATE_WINTAB_324_112_2_18_3_3_30_5,"COORDINATE.C param = %d")
TRACE_MSG(COORDINATE_WINTAB_329_112_2_18_3_3_30_6,"COORDINATE.C left bottom: x = %d, y = %d")
TRACE_MSG(COORDINATE_WINTAB_479_112_2_18_3_3_31_7,"MMI:COORDINATE_OpenWin(), alloc failed")
TRACE_MSG(COORDINATE_WINTAB_661_112_2_18_3_3_31_8,"PointVerify (x,y,adc_x,adc_y)=%d,%d")
TRACE_MSG(COORDINATE_WINTAB_691_112_2_18_3_3_31_9,"COORDINATE.C PointVerify: x=%d, adc_x=%d, min=%d, max=%d")
TRACE_MSG(COORDINATE_WINTAB_692_112_2_18_3_3_31_10,"COORDINATE.C PointVerify: y=%d, adc_y=%d, min=%d, max=%d")
TRACE_MSG(COORDINATE_WINTAB_884_112_2_18_3_3_31_11,"[VINE]--Not match the scale of x and y(1)\r\n")
TRACE_MSG(COORDINATE_WINTAB_893_112_2_18_3_3_32_12,"[VINE]--Not match the scale of x and y(2)\r\n")
TRACE_MSG(COORDINATE_WINTAB_905_112_2_18_3_3_32_13,"COORDINATE.C IsVerifySuccess: right top corner.")
TRACE_MSG(COORDINATE_WINTAB_910_112_2_18_3_3_32_14,"COORDINATE.C IsVerifySuccess: left bottom corner.")
TRACE_MSG(COORDINATE_WINTAB_915_112_2_18_3_3_32_15,"COORDINATE.C IsVerifySuccess: center.")
TRACE_MSG(COORDINATE_WINTAB_1272_112_2_18_3_3_33_16,"tp_cali_fun is null\r\n")
TRACE_MSG(COORDINATE_WINTAB_1289_112_2_18_3_3_33_17,"COORDINATE.C successed.")
TRACE_MSG(COORDINATE_WINTAB_1302_112_2_18_3_3_33_18,"COORDINATE.C update failed.")
TRACE_MSG(COORDINATE_WINTAB_1343_112_2_18_3_3_33_19,"COORDINATE.C s_right_top_adc_x = %d")
TRACE_MSG(COORDINATE_WINTAB_1344_112_2_18_3_3_33_20,"COORDINATE.C s_right_top_adc_y = %d")
TRACE_MSG(COORDINATE_WINTAB_1345_112_2_18_3_3_33_21,"COORDINATE.C s_left_bottom_adc_x = %d")
TRACE_MSG(COORDINATE_WINTAB_1346_112_2_18_3_3_33_22,"COORDINATE.C s_left_bottom_adc_y = %d")
TRACE_MSG(COORDINATE_WINTAB_1347_112_2_18_3_3_33_23,"COORDINATE.C s_mid_adc_x = %d")
TRACE_MSG(COORDINATE_WINTAB_1348_112_2_18_3_3_33_24,"COORDINATE.C s_mid_adc_y = %d")
TRACE_MSG(COORDINATE_WINTAB_1349_112_2_18_3_3_33_25,"COORDINATE.C tp_info_ptr->tp_width = %d")
TRACE_MSG(COORDINATE_WINTAB_1350_112_2_18_3_3_33_26,"COORDINATE.C tp_info_ptr->tp_lcd_height = %d")
TRACE_MSG(COORDINATE_WINTAB_1409_112_2_18_3_3_33_27,"COORDINATE.C IsVerifySuccess: right top corner.")
TRACE_MSG(COORDINATE_WINTAB_1413_112_2_18_3_3_33_28,"COORDINATE.C IsVerifySuccess: left bottom corner.")
TRACE_MSG(COORDINATE_WINTAB_1417_112_2_18_3_3_33_29,"COORDINATE.C IsVerifySuccess: center.")
TRACE_MSG(COORDINATE_WINTAB_1420_112_2_18_3_3_33_30,"COORDINATE.C IsVerifySuccess: set_success=%d.")
TRACE_MSG(COORDINATE_WINTAB_1422_112_2_18_3_3_33_31,"COORDINATE.C IsVerifySuccess: right top corner.")
TRACE_MSG(COORDINATE_WINTAB_1426_112_2_18_3_3_33_32,"COORDINATE.C IsVerifySuccess: left bottom corner.")
TRACE_MSG(COORDINATE_WINTAB_1484_112_2_18_3_3_33_33,"COORDINATE.C new cal: x_min_top=%d")
TRACE_MSG(COORDINATE_WINTAB_1485_112_2_18_3_3_33_34,"COORDINATE.C new cal: x_max_top=%d")
TRACE_MSG(COORDINATE_WINTAB_1486_112_2_18_3_3_33_35,"COORDINATE.C new cal: x_top_factor=%d")
TRACE_MSG(COORDINATE_WINTAB_1488_112_2_18_3_3_33_36,"COORDINATE.C new cal: x_min_bottom=%d")
TRACE_MSG(COORDINATE_WINTAB_1489_112_2_18_3_3_33_37,"COORDINATE.C new cal: x_max_bottom=%d")
TRACE_MSG(COORDINATE_WINTAB_1490_112_2_18_3_3_33_38,"COORDINATE.C new cal: x_bottom_factor=%d")
TRACE_MSG(COORDINATE_WINTAB_1492_112_2_18_3_3_33_39,"COORDINATE.C new cal: y_min_left=%d")
TRACE_MSG(COORDINATE_WINTAB_1493_112_2_18_3_3_33_40,"COORDINATE.C new cal: y_max_left=%d")
TRACE_MSG(COORDINATE_WINTAB_1494_112_2_18_3_3_33_41,"COORDINATE.C new cal: y_left_factor=%d")
TRACE_MSG(COORDINATE_WINTAB_1496_112_2_18_3_3_33_42,"COORDINATE.C new cal: y_min_right=%d")
TRACE_MSG(COORDINATE_WINTAB_1497_112_2_18_3_3_33_43,"COORDINATE.C new cal: y_max_right=%d")
TRACE_MSG(COORDINATE_WINTAB_1498_112_2_18_3_3_33_44,"COORDINATE.C new cal: y_right_factor=%d")
TRACE_MSG(COORDINATE_WINTAB_1500_112_2_18_3_3_33_45,"COORDINATE.C new cal: x_min_up=%d")
TRACE_MSG(COORDINATE_WINTAB_1501_112_2_18_3_3_33_46,"COORDINATE.C new cal: y_min_up=%d")
TRACE_MSG(COORDINATE_WINTAB_1505_112_2_18_3_3_33_47,"COORDINATE.C failed.")
TRACE_MSG(COORDINATE_WINTAB_1510_112_2_18_3_3_33_48,"COORDINATE.C successed.")
TRACE_MSG(COORDINATE_WINTAB_1516_112_2_18_3_3_33_49,"COORDINATE.C update failed.")
END_TRACE_MAP(MMI_APP_TP_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_TP_TRC_H_

