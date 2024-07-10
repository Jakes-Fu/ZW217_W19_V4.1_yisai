/******************************************************************************
 ** File Name:      mmi_app_qq_trc.h                                         *
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
//trace_id:216
#ifndef _MMI_APP_QQ_TRC_H_
#define _MMI_APP_QQ_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIQQ_FILE_110_112_2_18_2_49_6_0 "[MMIQQ]IFILEMGR_OpenFile()*****"
#define MMIQQ_FILE_172_112_2_18_2_49_6_1 "[MMIQQ]IFILE_Release()*****error = %d"
#define MMIQQ_FILE_198_112_2_18_2_49_6_2 "[MMIQQ]IFILE_Read()*****error = %d"
#define MMIQQ_FILE_224_112_2_18_2_49_6_3 "[MMIQQ]IFILE_Write()*****error = %d"
#define MMIQQ_FILE_281_112_2_18_2_49_6_4 "[MMIQQ]IFILEMGR_Remove()*****error = %d"
#define MMIQQ_FILE_305_112_2_18_2_49_6_5 "[MMIQQ]IFILE_Seek()*****error"
#define MMIQQ_FILE_333_112_2_18_2_49_6_6 "[MMIQQ]IFILE_Seek()*****error = %d"
#define MMIQQ_FILE_389_112_2_18_2_49_6_7 "[MMIQQ]IFILEMGR_MkDir()*****error = %d"
#define MMIQQ_FILE_457_112_2_18_2_49_6_8 "[MMIQQ]IFILEMGR_RmDir() result = %d"
#define MMIQQ_INPUT_237_112_2_18_2_49_7_9 "[MMIQQ]QInitEditWin:maxsize = %d"
#define MMIQQ_MAIN_151_112_2_18_2_49_7_10 "[MMIQQ]HandleQQMainWinMsg() msg_id == 0x%x"
#define MMIQQ_MAIN_186_112_2_18_2_49_8_11 "[MMIQQ]HandleQQMainWinMsg() MSG_FULL_PAINT--s_is_full_paint_for_open == %d"
#define MMIQQ_MAIN_435_112_2_18_2_49_8_12 "[QQ] EntryQQ is_use_wifi = %d, dual_sys =%d"
#define MMIQQ_MAIN_583_112_2_18_2_49_8_13 "[QQ]:QQIsHaveEnoughSpace free_low_word=%d"
#define MMIQQ_SOCKET_114_112_2_18_2_49_9_14 "[QQ]:QSOCKET_Init! **** "
#define MMIQQ_SOCKET_129_112_2_18_2_49_9_15 "[QQ]:QSOCKET_Free! GSockets.size = %d "
#define MMIQQ_SOCKET_146_112_2_18_2_49_9_16 "[QQ]:QSOCKET_Insert! **** "
#define MMIQQ_SOCKET_176_112_2_18_2_49_9_17 "[QQ]:QSocket_Remove! **** "
#define MMIQQ_SOCKET_277_112_2_18_2_49_9_18 "[QQ]:QSOCKET_ActiveGprs GSockets.gprs_status=%d"
#define MMIQQ_SOCKET_278_112_2_18_2_49_9_19 "[QQ] QSOCKET_ActiveGprs sim_select = %d"
#define MMIQQ_SOCKET_308_112_2_18_2_49_9_20 "[QQ]: QSOCKET_ActiveGprs() active pdp context apn = %s"
#define MMIQQ_SOCKET_321_112_2_18_2_49_9_21 "[QQ]: QSOCKET_ActiveGprs() use WIFI!!!"
#define MMIQQ_SOCKET_339_112_2_18_2_49_9_22 "[QQ]: QSOCKET_ActiveGprs() active pdp return_val = %d"
#define MMIQQ_SOCKET_353_112_2_18_2_49_9_23 "[QQ]:HandleQQPdpMsg msg_id=%d"
#define MMIQQ_SOCKET_354_112_2_18_2_49_9_24 "[QQ]:HandleQQPdpMsg result=%d"
#define MMIQQ_SOCKET_355_112_2_18_2_49_9_25 "[QQ]:HandleQQPdpMsg GSockets.gprs_status=%d"
#define MMIQQ_SOCKET_356_112_2_18_2_49_9_26 "[QQ]:HandleQQPdpMsg ps_interface=%d"
#define MMIQQ_SOCKET_386_112_2_18_2_49_9_27 "[QQ]:HandleQQPdpMsg Active GPRS OK! **** "
#define MMIQQ_SOCKET_401_112_2_18_2_49_9_28 "[QQ]:HandleQQPdpMsg  MMIPDP_DEACTIVE_IND"
#define MMIQQ_SOCKET_449_112_2_18_2_49_9_29 "[QQ]:QSOCKET_ConnectCB status = %d"
#define MMIQQ_SOCKET_482_112_2_18_2_49_9_30 "[QQ]:ActiveGprsCallBack msgid=%d"
#define MMIQQ_SOCKET_546_112_2_18_2_49_10_31 "[QQ]:QSOCKET_DeActiveGprs return_val=%d"
#define MMIQQ_SOCKET_649_112_2_18_2_49_10_32 "[QQ]:INETMGR_OpenSocket s_qq_net_id =%d"
#define MMIQQ_SOCKET_650_112_2_18_2_49_10_33 "[QQ]:INETMGR_OpenSocket GSockets.gprs_status =%d"
#define MMIQQ_SOCKET_668_112_2_18_2_49_10_34 "QSOCKET_Create() socketid=0x%08x"
#define MMIQQ_SOCKET_680_112_2_18_2_49_10_35 "setnbio error ***************%d"
#define MMIQQ_SOCKET_695_112_2_18_2_49_10_36 "[QQ]:ISOCKET_Connect "
#define MMIQQ_SOCKET_733_112_2_18_2_49_10_37 "QSOCKET_Read: len = *********************%d"
#define MMIQQ_SOCKET_737_112_2_18_2_49_10_38 "QSOCKET_Read: error = *********************%d"
#define MMIQQ_SOCKET_762_112_2_18_2_49_10_39 "QSOCKET_Write: len = **************%d"
#define MMIQQ_SOCKET_765_112_2_18_2_49_10_40 "QSOCKET_Write: error = **************%d"
#define MMIQQ_SOCKET_781_112_2_18_2_49_10_41 "[QQ]:ISOCKET_Release "
#define MMIQQ_SYSTEM_185_112_2_18_2_49_11_42 "[QQ]QPlayRecvMsg: Can't play msg ring"
#define MMIQQ_SYSTEM_255_112_2_18_2_49_11_43 "[QQ] QGoToUrl is_use_wifi = %d, dual_sys =%d"
#define MMIQQ_SYSTEM_714_112_2_18_2_49_12_44 "[QQ]QUPDATASTATUS =%d= %d"
#define MMIQQ_SYSTEM_747_112_2_18_2_49_12_45 "[QQ]MMIAPIQQ_GetIdleImageID = %d"
#define MMIQQ_SYSTEM_887_112_2_18_2_49_13_46 "[QQ] MMIQQ_SetSimSelect sim_select = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_QQ_TRC)
TRACE_MSG(MMIQQ_FILE_110_112_2_18_2_49_6_0,"[MMIQQ]IFILEMGR_OpenFile()*****")
TRACE_MSG(MMIQQ_FILE_172_112_2_18_2_49_6_1,"[MMIQQ]IFILE_Release()*****error = %d")
TRACE_MSG(MMIQQ_FILE_198_112_2_18_2_49_6_2,"[MMIQQ]IFILE_Read()*****error = %d")
TRACE_MSG(MMIQQ_FILE_224_112_2_18_2_49_6_3,"[MMIQQ]IFILE_Write()*****error = %d")
TRACE_MSG(MMIQQ_FILE_281_112_2_18_2_49_6_4,"[MMIQQ]IFILEMGR_Remove()*****error = %d")
TRACE_MSG(MMIQQ_FILE_305_112_2_18_2_49_6_5,"[MMIQQ]IFILE_Seek()*****error")
TRACE_MSG(MMIQQ_FILE_333_112_2_18_2_49_6_6,"[MMIQQ]IFILE_Seek()*****error = %d")
TRACE_MSG(MMIQQ_FILE_389_112_2_18_2_49_6_7,"[MMIQQ]IFILEMGR_MkDir()*****error = %d")
TRACE_MSG(MMIQQ_FILE_457_112_2_18_2_49_6_8,"[MMIQQ]IFILEMGR_RmDir() result = %d")
TRACE_MSG(MMIQQ_INPUT_237_112_2_18_2_49_7_9,"[MMIQQ]QInitEditWin:maxsize = %d")
TRACE_MSG(MMIQQ_MAIN_151_112_2_18_2_49_7_10,"[MMIQQ]HandleQQMainWinMsg() msg_id == 0x%x")
TRACE_MSG(MMIQQ_MAIN_186_112_2_18_2_49_8_11,"[MMIQQ]HandleQQMainWinMsg() MSG_FULL_PAINT--s_is_full_paint_for_open == %d")
TRACE_MSG(MMIQQ_MAIN_435_112_2_18_2_49_8_12,"[QQ] EntryQQ is_use_wifi = %d, dual_sys =%d")
TRACE_MSG(MMIQQ_MAIN_583_112_2_18_2_49_8_13,"[QQ]:QQIsHaveEnoughSpace free_low_word=%d")
TRACE_MSG(MMIQQ_SOCKET_114_112_2_18_2_49_9_14,"[QQ]:QSOCKET_Init! **** ")
TRACE_MSG(MMIQQ_SOCKET_129_112_2_18_2_49_9_15,"[QQ]:QSOCKET_Free! GSockets.size = %d ")
TRACE_MSG(MMIQQ_SOCKET_146_112_2_18_2_49_9_16,"[QQ]:QSOCKET_Insert! **** ")
TRACE_MSG(MMIQQ_SOCKET_176_112_2_18_2_49_9_17,"[QQ]:QSocket_Remove! **** ")
TRACE_MSG(MMIQQ_SOCKET_277_112_2_18_2_49_9_18,"[QQ]:QSOCKET_ActiveGprs GSockets.gprs_status=%d")
TRACE_MSG(MMIQQ_SOCKET_278_112_2_18_2_49_9_19,"[QQ] QSOCKET_ActiveGprs sim_select = %d")
TRACE_MSG(MMIQQ_SOCKET_308_112_2_18_2_49_9_20,"[QQ]: QSOCKET_ActiveGprs() active pdp context apn = %s")
TRACE_MSG(MMIQQ_SOCKET_321_112_2_18_2_49_9_21,"[QQ]: QSOCKET_ActiveGprs() use WIFI!!!")
TRACE_MSG(MMIQQ_SOCKET_339_112_2_18_2_49_9_22,"[QQ]: QSOCKET_ActiveGprs() active pdp return_val = %d")
TRACE_MSG(MMIQQ_SOCKET_353_112_2_18_2_49_9_23,"[QQ]:HandleQQPdpMsg msg_id=%d")
TRACE_MSG(MMIQQ_SOCKET_354_112_2_18_2_49_9_24,"[QQ]:HandleQQPdpMsg result=%d")
TRACE_MSG(MMIQQ_SOCKET_355_112_2_18_2_49_9_25,"[QQ]:HandleQQPdpMsg GSockets.gprs_status=%d")
TRACE_MSG(MMIQQ_SOCKET_356_112_2_18_2_49_9_26,"[QQ]:HandleQQPdpMsg ps_interface=%d")
TRACE_MSG(MMIQQ_SOCKET_386_112_2_18_2_49_9_27,"[QQ]:HandleQQPdpMsg Active GPRS OK! **** ")
TRACE_MSG(MMIQQ_SOCKET_401_112_2_18_2_49_9_28,"[QQ]:HandleQQPdpMsg  MMIPDP_DEACTIVE_IND")
TRACE_MSG(MMIQQ_SOCKET_449_112_2_18_2_49_9_29,"[QQ]:QSOCKET_ConnectCB status = %d")
TRACE_MSG(MMIQQ_SOCKET_482_112_2_18_2_49_9_30,"[QQ]:ActiveGprsCallBack msgid=%d")
TRACE_MSG(MMIQQ_SOCKET_546_112_2_18_2_49_10_31,"[QQ]:QSOCKET_DeActiveGprs return_val=%d")
TRACE_MSG(MMIQQ_SOCKET_649_112_2_18_2_49_10_32,"[QQ]:INETMGR_OpenSocket s_qq_net_id =%d")
TRACE_MSG(MMIQQ_SOCKET_650_112_2_18_2_49_10_33,"[QQ]:INETMGR_OpenSocket GSockets.gprs_status =%d")
TRACE_MSG(MMIQQ_SOCKET_668_112_2_18_2_49_10_34,"QSOCKET_Create() socketid=0x%08x")
TRACE_MSG(MMIQQ_SOCKET_680_112_2_18_2_49_10_35,"setnbio error ***************%d")
TRACE_MSG(MMIQQ_SOCKET_695_112_2_18_2_49_10_36,"[QQ]:ISOCKET_Connect ")
TRACE_MSG(MMIQQ_SOCKET_733_112_2_18_2_49_10_37,"QSOCKET_Read: len = *********************%d")
TRACE_MSG(MMIQQ_SOCKET_737_112_2_18_2_49_10_38,"QSOCKET_Read: error = *********************%d")
TRACE_MSG(MMIQQ_SOCKET_762_112_2_18_2_49_10_39,"QSOCKET_Write: len = **************%d")
TRACE_MSG(MMIQQ_SOCKET_765_112_2_18_2_49_10_40,"QSOCKET_Write: error = **************%d")
TRACE_MSG(MMIQQ_SOCKET_781_112_2_18_2_49_10_41,"[QQ]:ISOCKET_Release ")
TRACE_MSG(MMIQQ_SYSTEM_185_112_2_18_2_49_11_42,"[QQ]QPlayRecvMsg: Can't play msg ring")
TRACE_MSG(MMIQQ_SYSTEM_255_112_2_18_2_49_11_43,"[QQ] QGoToUrl is_use_wifi = %d, dual_sys =%d")
TRACE_MSG(MMIQQ_SYSTEM_714_112_2_18_2_49_12_44,"[QQ]QUPDATASTATUS =%d= %d")
TRACE_MSG(MMIQQ_SYSTEM_747_112_2_18_2_49_12_45,"[QQ]MMIAPIQQ_GetIdleImageID = %d")
TRACE_MSG(MMIQQ_SYSTEM_887_112_2_18_2_49_13_46,"[QQ] MMIQQ_SetSimSelect sim_select = %d")
END_TRACE_MAP(MMI_APP_QQ_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_QQ_TRC_H_

