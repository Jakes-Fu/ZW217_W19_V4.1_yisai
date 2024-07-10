/******************************************************************************
 ** File Name:      mmi_app_sns_trc.h                                         *
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
//trace_id:223
#ifndef _MMI_APP_SNS_TRC_H_
#define _MMI_APP_SNS_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMISNS_SQLITE_110_112_2_18_2_55_42_0 "MMIVIRTUALARRAY_Transaction, handle = 0!"
#define MMISNS_SQLITE_144_112_2_18_2_55_42_1 "MMISMS_Sqlite_GetItemByID  search_handle->uiNum !!!"
#define MMISNS_SQLITE_150_112_2_18_2_55_42_2 "MMISMS_Sqlite_GetItemByID  search_handle->uiNum = 0 !!!"
#define MMISNS_SQLITE_160_112_2_18_2_55_42_3 "PackQuerySqlSentence: %s."
#define MMISNS_SQLITE_175_112_2_18_2_55_42_4 "MMISMS_Sqlite_GetItemByID  s_handle.sql_result error or result empty!!!"
#define MMISNS_SQLITE_212_112_2_18_2_55_43_5 "MMIVIRTUALARRAY_GetItemById, data_ptr = 0!"
#define MMISNS_SQLITE_221_112_2_18_2_55_43_6 "PackQuerySqlSentence: %s."
#define MMISNS_SQLITE_241_112_2_18_2_55_43_7 "MMIVIRTUALARRAY_GetItemById, error:%s."
#define MMISNS_SQLITE_265_112_2_18_2_55_43_8 "MMISNS_Sqlite_AddItem, data_ptr = 0!"
#define MMISNS_SQLITE_273_112_2_18_2_55_43_9 "MMISNS_Sqlite_AddItem, num > MAX !"
#define MMISNS_SQLITE_286_112_2_18_2_55_43_10 "PackAddSqlSentence: %s."
#define MMISNS_SQLITE_291_112_2_18_2_55_43_11 "MMISNS_Sqlite_AddItem, error:%d."
#define MMISNS_SQLITE_347_112_2_18_2_55_43_12 "CreateTable, create talbe fail.%d,%d"
#define MMISNS_SQLITE_358_112_2_18_2_55_43_13 "CreateTable, create index fail..%d,%d"
#define MMISNS_SQLITE_367_112_2_18_2_55_43_14 "MMISNS_Sqlite_Create, error!"
#define MMISNS_SQLITE_383_112_2_18_2_55_43_15 "MMISNS_Sqlite_Destroy, handle or search_handle = 0!"
#define MMISNS_SQLITE_448_112_2_18_2_55_43_16 "MMISMS_Sqlite_GetNumByType, handle = 0!"
#define MMISNS_SQLITE_455_112_2_18_2_55_43_17 "MMISMS_Sqlite_GetNumByType PackQuerySqlSentence: %s."
#define MMISNS_SQLITE_474_112_2_18_2_55_43_18 "MMISMS_Sqlite_GetNumByType, error:%d."
#define MMISNS_SQLITE_502_112_2_18_2_55_43_19 "MMISMS_Sqlite_GetItemByName   !!!"
#define MMISNS_SQLITE_512_112_2_18_2_55_43_20 "PackQuerySqlSentence: %s."
#define MMISNS_SQLITE_530_112_2_18_2_55_43_21 "MMISMS_Sqlite_GetItemByName  s_handle.sql_result error or result empty %d!!!"
#define MMISNS_SQLITE_568_112_2_18_2_55_43_22 "MMISNS_Sqlite_GetTotalNum, handle = 0!"
#define MMISNS_SQLITE_610_112_2_18_2_55_43_23 "MMISMS_Sqlite_GetItemByType, handle = 0!"
#define MMISNS_SQLITE_620_112_2_18_2_55_43_24 "MMISMS_Sqlite_GetItemByType PackQuerySqlSentence: %s."
#define MMISNS_SQLITE_638_112_2_18_2_55_43_25 "MMISMS_Sqlite_GetItemByType, error:%d."
#define MMISNS_SQLITE_691_112_2_18_2_55_43_26 "MMISNS_Sqlite_CleanAll, DB is open , cannot clean all!!!"
#define MMISNS_SRV_273_112_2_18_2_55_44_27 "renwei-IMMISNS_CreateObject"
#define MMISNS_SRV_289_112_2_18_2_55_44_28 "renwei-MMISNS_AddRef"
#define MMISNS_SRV_315_112_2_18_2_55_44_29 "renwei-MMISNS_Release  pMe->m_nRef =%d"
#define MMISNS_SRV_320_112_2_18_2_55_44_30 "renwei-MMISNS_Release  SHARE_DATA pMe->m_nRef"
#define MMISNS_SRV_344_112_2_18_2_55_44_31 "renwei-MMISNS_Release  PB_GET_DATA pMe->m_nRef"
#define MMISNS_SRV_348_112_2_18_2_55_44_32 "renwei-MMISNS_Release  COMMOND_WIDGET pMe->m_nRef"
#define MMISNS_SRV_354_112_2_18_2_55_44_33 "renwei-MMISNS_Release  other COMMOND"
#define MMISNS_SRV_445_112_2_18_2_55_45_34 "renwei-MMISNS_WidgetNewEdit  commond_type=%d"
#define MMISNS_SRV_446_112_2_18_2_55_45_35 "renwei-MMISNS_WidgetNewEdit  widget_commond =%d, snsid=%d"
#define MMISNS_SRV_500_112_2_18_2_55_45_36 "renwei-MMISNS_WidgetGetData"
#define MMISNS_SRV_517_112_2_18_2_55_45_37 "renwei-MMISNS_WidgetGetData  index=%d"
#define MMISNS_SRV_534_112_2_18_2_55_45_38 "renwei-MMISNS_WidgetGetNum"
#define MMISNS_SRV_541_112_2_18_2_55_45_39 "renwei-MMISNS_WidgetGetNum num = %d"
#define MMISNS_SRV_567_112_2_18_2_55_45_40 "renwei-MMISNS_WidgetEnterTextDetail  index=%d"
#define MMISNS_SRV_568_112_2_18_2_55_45_41 "renwei-MMISNS_WidgetEnterTextDetail  wre_sns_id=%d"
#define MMISNS_SRV_590_112_2_18_2_55_45_42 "renwei-MMISNS_ShareToSns"
#define MMISNS_SRV_705_112_2_18_2_55_45_43 "renwei-MMISNS_Register"
#define MMISNS_SRV_768_112_2_18_2_55_45_44 "renwei-MMISNS_GetRegisterInfo"
#define MMISNS_SRV_2067_112_2_18_2_55_48_45 "renwei-MMISNS_WidgetUserLogIn commond_type= %d"
#define MMISNS_SRV_2068_112_2_18_2_55_48_46 "renwei-MMISNS_WidgetUserLogIn widget_commond= %d"
#define MMISNS_SRV_2096_112_2_18_2_55_48_47 "renwei-MMISNS_SaveWreAppLogInfo  commond_result =%d"
#define MMISNS_SRV_2172_112_2_18_2_55_48_48 "renwei-MMISNS_SendEvtToApp  wreapp_id=%d ,wreapp_id=%d ,wreapp_id=%d"
#define MMISNS_SRV_2555_112_2_18_2_55_49_49 "MMISNS_GetDataByDbName   !!!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_SNS_TRC)
TRACE_MSG(MMISNS_SQLITE_110_112_2_18_2_55_42_0,"MMIVIRTUALARRAY_Transaction, handle = 0!")
TRACE_MSG(MMISNS_SQLITE_144_112_2_18_2_55_42_1,"MMISMS_Sqlite_GetItemByID  search_handle->uiNum !!!")
TRACE_MSG(MMISNS_SQLITE_150_112_2_18_2_55_42_2,"MMISMS_Sqlite_GetItemByID  search_handle->uiNum = 0 !!!")
TRACE_MSG(MMISNS_SQLITE_160_112_2_18_2_55_42_3,"PackQuerySqlSentence: %s.")
TRACE_MSG(MMISNS_SQLITE_175_112_2_18_2_55_42_4,"MMISMS_Sqlite_GetItemByID  s_handle.sql_result error or result empty!!!")
TRACE_MSG(MMISNS_SQLITE_212_112_2_18_2_55_43_5,"MMIVIRTUALARRAY_GetItemById, data_ptr = 0!")
TRACE_MSG(MMISNS_SQLITE_221_112_2_18_2_55_43_6,"PackQuerySqlSentence: %s.")
TRACE_MSG(MMISNS_SQLITE_241_112_2_18_2_55_43_7,"MMIVIRTUALARRAY_GetItemById, error:%s.")
TRACE_MSG(MMISNS_SQLITE_265_112_2_18_2_55_43_8,"MMISNS_Sqlite_AddItem, data_ptr = 0!")
TRACE_MSG(MMISNS_SQLITE_273_112_2_18_2_55_43_9,"MMISNS_Sqlite_AddItem, num > MAX !")
TRACE_MSG(MMISNS_SQLITE_286_112_2_18_2_55_43_10,"PackAddSqlSentence: %s.")
TRACE_MSG(MMISNS_SQLITE_291_112_2_18_2_55_43_11,"MMISNS_Sqlite_AddItem, error:%d.")
TRACE_MSG(MMISNS_SQLITE_347_112_2_18_2_55_43_12,"CreateTable, create talbe fail.%d,%d")
TRACE_MSG(MMISNS_SQLITE_358_112_2_18_2_55_43_13,"CreateTable, create index fail..%d,%d")
TRACE_MSG(MMISNS_SQLITE_367_112_2_18_2_55_43_14,"MMISNS_Sqlite_Create, error!")
TRACE_MSG(MMISNS_SQLITE_383_112_2_18_2_55_43_15,"MMISNS_Sqlite_Destroy, handle or search_handle = 0!")
TRACE_MSG(MMISNS_SQLITE_448_112_2_18_2_55_43_16,"MMISMS_Sqlite_GetNumByType, handle = 0!")
TRACE_MSG(MMISNS_SQLITE_455_112_2_18_2_55_43_17,"MMISMS_Sqlite_GetNumByType PackQuerySqlSentence: %s.")
TRACE_MSG(MMISNS_SQLITE_474_112_2_18_2_55_43_18,"MMISMS_Sqlite_GetNumByType, error:%d.")
TRACE_MSG(MMISNS_SQLITE_502_112_2_18_2_55_43_19,"MMISMS_Sqlite_GetItemByName   !!!")
TRACE_MSG(MMISNS_SQLITE_512_112_2_18_2_55_43_20,"PackQuerySqlSentence: %s.")
TRACE_MSG(MMISNS_SQLITE_530_112_2_18_2_55_43_21,"MMISMS_Sqlite_GetItemByName  s_handle.sql_result error or result empty %d!!!")
TRACE_MSG(MMISNS_SQLITE_568_112_2_18_2_55_43_22,"MMISNS_Sqlite_GetTotalNum, handle = 0!")
TRACE_MSG(MMISNS_SQLITE_610_112_2_18_2_55_43_23,"MMISMS_Sqlite_GetItemByType, handle = 0!")
TRACE_MSG(MMISNS_SQLITE_620_112_2_18_2_55_43_24,"MMISMS_Sqlite_GetItemByType PackQuerySqlSentence: %s.")
TRACE_MSG(MMISNS_SQLITE_638_112_2_18_2_55_43_25,"MMISMS_Sqlite_GetItemByType, error:%d.")
TRACE_MSG(MMISNS_SQLITE_691_112_2_18_2_55_43_26,"MMISNS_Sqlite_CleanAll, DB is open , cannot clean all!!!")
TRACE_MSG(MMISNS_SRV_273_112_2_18_2_55_44_27,"renwei-IMMISNS_CreateObject")
TRACE_MSG(MMISNS_SRV_289_112_2_18_2_55_44_28,"renwei-MMISNS_AddRef")
TRACE_MSG(MMISNS_SRV_315_112_2_18_2_55_44_29,"renwei-MMISNS_Release  pMe->m_nRef =%d")
TRACE_MSG(MMISNS_SRV_320_112_2_18_2_55_44_30,"renwei-MMISNS_Release  SHARE_DATA pMe->m_nRef")
TRACE_MSG(MMISNS_SRV_344_112_2_18_2_55_44_31,"renwei-MMISNS_Release  PB_GET_DATA pMe->m_nRef")
TRACE_MSG(MMISNS_SRV_348_112_2_18_2_55_44_32,"renwei-MMISNS_Release  COMMOND_WIDGET pMe->m_nRef")
TRACE_MSG(MMISNS_SRV_354_112_2_18_2_55_44_33,"renwei-MMISNS_Release  other COMMOND")
TRACE_MSG(MMISNS_SRV_445_112_2_18_2_55_45_34,"renwei-MMISNS_WidgetNewEdit  commond_type=%d")
TRACE_MSG(MMISNS_SRV_446_112_2_18_2_55_45_35,"renwei-MMISNS_WidgetNewEdit  widget_commond =%d, snsid=%d")
TRACE_MSG(MMISNS_SRV_500_112_2_18_2_55_45_36,"renwei-MMISNS_WidgetGetData")
TRACE_MSG(MMISNS_SRV_517_112_2_18_2_55_45_37,"renwei-MMISNS_WidgetGetData  index=%d")
TRACE_MSG(MMISNS_SRV_534_112_2_18_2_55_45_38,"renwei-MMISNS_WidgetGetNum")
TRACE_MSG(MMISNS_SRV_541_112_2_18_2_55_45_39,"renwei-MMISNS_WidgetGetNum num = %d")
TRACE_MSG(MMISNS_SRV_567_112_2_18_2_55_45_40,"renwei-MMISNS_WidgetEnterTextDetail  index=%d")
TRACE_MSG(MMISNS_SRV_568_112_2_18_2_55_45_41,"renwei-MMISNS_WidgetEnterTextDetail  wre_sns_id=%d")
TRACE_MSG(MMISNS_SRV_590_112_2_18_2_55_45_42,"renwei-MMISNS_ShareToSns")
TRACE_MSG(MMISNS_SRV_705_112_2_18_2_55_45_43,"renwei-MMISNS_Register")
TRACE_MSG(MMISNS_SRV_768_112_2_18_2_55_45_44,"renwei-MMISNS_GetRegisterInfo")
TRACE_MSG(MMISNS_SRV_2067_112_2_18_2_55_48_45,"renwei-MMISNS_WidgetUserLogIn commond_type= %d")
TRACE_MSG(MMISNS_SRV_2068_112_2_18_2_55_48_46,"renwei-MMISNS_WidgetUserLogIn widget_commond= %d")
TRACE_MSG(MMISNS_SRV_2096_112_2_18_2_55_48_47,"renwei-MMISNS_SaveWreAppLogInfo  commond_result =%d")
TRACE_MSG(MMISNS_SRV_2172_112_2_18_2_55_48_48,"renwei-MMISNS_SendEvtToApp  wreapp_id=%d ,wreapp_id=%d ,wreapp_id=%d")
TRACE_MSG(MMISNS_SRV_2555_112_2_18_2_55_49_49,"MMISNS_GetDataByDbName   !!!")
END_TRACE_MAP(MMI_APP_SNS_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_SNS_TRC_H_

