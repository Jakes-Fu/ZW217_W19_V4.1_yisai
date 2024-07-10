/******************************************************************************
 ** File Name:      mmi_app_vcard_trc.h                                         *
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
//trace_id:234
#ifndef _MMI_APP_VCARD_TRC_H_
#define _MMI_APP_VCARD_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIVC_COMMON_72_112_2_18_3_4_30_0 "mmivcard:MMIVC_Malloc, size=%d"
#define MMIVC_COMMON_97_112_2_18_3_4_30_1 "[MMIVCARD] MMIVC_Realloc ns == 0"
#define MMIVC_COMMON_150_112_2_18_3_4_30_2 "[MMIVCARD] vc_getcharset p_prop 0x%x"
#define MMIVC_COMMON_188_112_2_18_3_4_30_3 "[MMIVCARD] vc_getpropvalue p_prop 0x%x"
#define MMIVC_COMMON_240_112_2_18_3_4_30_4 "[MMIVCARD] vc_freedata PNULL == p_data"
#define MMIVC_COMMON_324_112_2_18_3_4_30_5 "[MMIVCARD] GetFirstValidLetterPos (PNULL == name) "
#define MMIVC_COMMON_353_112_2_18_3_4_30_6 "MMIVC_WriteFile_CallBack() error=%d, result=%d, param=%d"
#define MMIVC_COMMON_358_112_2_18_3_4_30_7 "MMIVC_WriteFile_CallBack (PNULL == callback_ptr)"
#define MMIVCALENDAR_199_112_2_18_3_4_31_8 "VcalSetDataToVcalInfo:p_datastr->wstr_len=%d"
#define MMIVCALENDAR_205_112_2_18_3_4_31_9 "VcalSetDataToVcalInfo:p_text==PNULL"
#define MMIVCALENDAR_214_112_2_18_3_4_31_10 "VcalSetDataToVcalInfo:proptype=%d"
#define MMIVCALENDAR_750_112_2_18_3_4_32_11 "MMIVCAL_VcalInfoToSchedule: No support the rrule!"
#define MMIVCALENDAR_840_112_2_18_3_4_32_12 "mmivcal:MMIVCAL_VcalendarDataToSch error_type=%d"
#define MMIVCALENDAR_886_112_2_18_3_4_32_13 "mmivcal:MMIVCAL_VcalBuildInit result=%d"
#define MMIVCALENDAR_906_112_2_18_3_4_32_14 "mmivcal:MMIVCAL_InitFileNamePtr result=%d"
#define MMIVCALENDAR_984_112_2_18_3_4_33_15 "mmivcal:MMIVCAL_BuildVcalendarByEvent start"
#define MMIVCALENDAR_1361_112_2_18_3_4_33_16 "mmivcal: vcal_lendth is=%d,vcal_info is %s "
#define MMIVCALENDAR_1426_112_2_18_3_4_33_17 "mmivcal:memory is not enough!"
#define MMIVCALENDAR_1433_112_2_18_3_4_34_18 "mmivcal:valid_index is error!"
#define MMIVCALENDAR_1458_112_2_18_3_4_34_19 "mmivcal:MMIVCAL_GetNextVcalItem start"
#define MMIVCALENDAR_1498_112_2_18_3_4_34_20 "mmivcal:memory is not enough!"
#define MMIVCALENDAR_1504_112_2_18_3_4_34_21 "mmivcal:not exist valid sch!"
#define MMIVCALENDAR_1531_112_2_18_3_4_34_22 "mmivcal:AddtVcalItem uuid=%d,ret=%d"
#define MMIVCALENDAR_1552_112_2_18_3_4_34_23 "mmivcal:ModifyVcalItem uuid is error"
#define MMIVCALENDAR_1562_112_2_18_3_4_34_24 "mmivcal:ModifyVcalItem uuid=%d,ret=%d"
#define MMIVCALENDAR_1579_112_2_18_3_4_34_25 "mmivcal:RemoveVcalItem uuid is error"
#define MMIVCALENDAR_1588_112_2_18_3_4_34_26 "mmivcal:RemoveVcalItem uuid=%d,ret=%d"
#define MMIVCALENDAR_1605_112_2_18_3_4_34_27 "mmivcal:VcalCreatfile()"
#define MMIVCALENDAR_1691_112_2_18_3_4_34_28 "mmivcal: MMIVCAL_ReadFile pp_object 0x%x"
#define MMIVCALENDAR_1697_112_2_18_3_4_34_29 "mmivcal:VcalCreatfile() file_handle=%d"
#define MMIVCALENDAR_1787_112_2_18_3_4_34_30 "mmivcal:MMIVCAL_SaveVcalendarFile g_vcard_handle_result is %d"
#define MMIVCALENDAR_1807_112_2_18_3_4_34_31 "mmivcal:MMIVCAL_SaveVcalendarFile: FFS_Write failed. result=%d"
#define MMIVCALENDAR_1983_112_2_18_3_4_35_32 "mmivcal:MMIVCAL_GetDataByFile() file_handle=%d"
#define MMIVCALENDAR_2035_112_2_18_3_4_35_33 "mmivcal: MMIVCAL_GetVcalendarFileName PNULL == entry_ptr"
#define MMIVCALENDAR_2112_112_2_18_3_4_35_34 "MMIVCAL_SaveVCalendarByData 0x%x"
#define MMIVCALENDAR_2117_112_2_18_3_4_35_35 "MMIVCAL_SaveVCalendarByData data_len %d !"
#define MMIVCALENDAR_2184_112_2_18_3_4_35_36 "MMIVCAL_SaveVCalendarByData result %d !"
#define MMIVCALENDAR_2231_112_2_18_3_4_35_37 "VcalWritedFile 0x%x"
#define MMIVCALENDAR_2259_112_2_18_3_4_35_38 "VcalWritedFile: ffs_error=%d"
#define MMIVCALENDAR_2342_112_2_18_3_4_35_39 "MMIVCAL_HandleWriteCNF result= %d"
#define MMIVCALENDAR_2361_112_2_18_3_4_35_40 "MMIVCAL_HandleWriteCNF result %d"
#define MMIVCALENDAR_2368_112_2_18_3_4_35_41 "MMIVCAL_HandleWriteCNF result_ptr->result=%d"
#define MMIVCARD_240_112_2_18_3_4_36_42 "MMIVC_GetFamilyName contact_ptr 0x%x"
#define MMIVCARD_288_112_2_18_3_4_36_43 "[MMIVCARD] VCARD_GetFamilyname contact_ptr 0x%x"
#define MMIVCARD_337_112_2_18_3_4_36_44 "[MMIVCARD] VCARD_GetRev date_ptr 0x%x time_ptr 0x%x"
#define MMIVCARD_458_112_2_18_3_4_37_45 "[MMIVCARD] MMIAPIVCARD_BuildVCardToBuffer parameter error !"
#define MMIVCARD_727_112_2_18_3_4_37_46 "[MMIVCARD] MMIAPIVCARD_BuildVCardToBuffer parameter error !"
#define MMIVCARD_1014_112_2_18_3_4_38_47 "[MMIVCARD] VCARD_GetVcardFileName PNULL == contact_ptr"
#define MMIVCARD_1092_112_2_18_3_4_38_48 "[MMIVCARD] VCARD_GetVcardFileName PNULL == entry_ptr"
#define MMIVCARD_1191_112_2_18_3_4_38_49 "[MMIVCARD] MMIAPIVCARD_InitFolder device_ptr 0x%x dir_ptr 0x%x device_len %d"
#define MMIVCARD_1225_112_2_18_3_4_38_50 "mmivcard:vc_creatfile()"
#define MMIVCARD_1281_112_2_18_3_4_38_51 "mmivcard:VCARD_SaveVcardFile g_vcard_handle_result is %d"
#define MMIVCARD_1301_112_2_18_3_4_38_52 "VCARD_SaveVcardFile: FFS_Write failed. result=%d"
#define MMIVCARD_1445_112_2_18_3_4_39_53 "mmivcard:VCARD_SaveVcardFile g_vcard_handle_result is %d"
#define MMIVCARD_1510_112_2_18_3_4_39_54 "[VCard] MMIAPIVCARD_HandleWriteCNF result %d"
#define MMIVCARD_1619_112_2_18_3_4_39_55 "[MMIVCARD] MMIAPIVCard_SaveVCardByData data_ptr 0x%x"
#define MMIVCARD_1624_112_2_18_3_4_39_56 "MMIAPIVCard_SaveVCardByData data_len %d !"
#define MMIVCARD_1694_112_2_18_3_4_39_57 "MMIAPIVCard_SaveVCardByData result %d !"
#define MMIVCARD_1719_112_2_18_3_4_39_58 "[MMIVCARD]VCARD_WritedFile filename 0x%x"
#define MMIVCARD_1748_112_2_18_3_4_39_59 "VCARD_WritedFile: ffs_error=%d"
#define MMIVCARD_1892_112_2_18_3_4_40_60 "[MMIVCARD] MMIAPIVCARD_ReadFile pp_object 0x%x"
#define MMIVCARD_1901_112_2_18_3_4_40_61 "mmivcard:MMIAPIVCARD_ReadFile() file_handle=%d"
#define MMIVCARD_2034_112_2_18_3_4_40_62 "[MMIVCARD] VCARD_read_next_item pp_object 0x%x"
#define MMIVCARD_2039_112_2_18_3_4_40_63 "mmivcard:VCARD_read_next_item() file_handle=%d"
#define MMIVCARD_2181_112_2_18_3_4_40_64 "[MMIVCARD] MMIAPIVCARD_ReadFile pp_object 0x%x"
#define MMIVCARD_2188_112_2_18_3_4_40_65 "mmivcard:MMIAPIVCARD_ReadFile() file_handle=%d"
#define MMIVCARD_2283_112_2_18_3_4_40_66 "[MMIVCARD] MMIAPIVCARD_ParseByData pp_object 0x%x"
#define MMIVCARD_2346_112_2_18_3_4_41_67 "[MMIVCARD] MMIAPIVCARD_GetDataByFile name_ptr 0x%x"
#define MMIVCARD_2352_112_2_18_3_4_41_68 "mmivcard:MMIAPIVCARD_GetDataByFile() file_handle=%d"
#define MMIVCARD_2403_112_2_18_3_4_41_69 "[MMIVCARD] vc_getproptype p_name 0x%x"
#define MMIVCARD_2432_112_2_18_3_4_41_70 "[MMIVCARD] vc_setdatatopb p_data 0x%x"
#define MMIVCARD_2512_112_2_18_3_4_41_71 "[MMIVCARD] vc_setdatatopb p_data 0x%x"
#define MMIVCARD_2599_112_2_18_3_4_41_72 "[MMIVCARD] vc_gettelephonenum p_Object 0x%x p_PropName 0x%x pp_Array 0x%x"
#define MMIVCARD_2718_112_2_18_3_4_41_73 "[MMIVCARD] vc_setteltopb p_telstr 0x%x "
#define MMIVCARD_2740_112_2_18_3_4_41_74 "[MMIVCARD] vc_cleanuptelnum p_org_num 0x%x "
#define MMIVCARD_2833_112_2_18_3_4_41_75 "[MMIVCARD] MMIAPIVCARD_AddObjectToPB pObject 0x%x "
#define MMIVCARD_2952_112_2_18_3_4_42_76 "[MMIVCARD] MMIAPIVCARD_BuildVCardToBuffer parameter error !"
#define MMIVCARD_3189_112_2_18_3_4_42_77 "[MMIVCARD] MMIAPIVCARD_AddObjectToPB pObject 0x%x "
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_VCARD_TRC)
TRACE_MSG(MMIVC_COMMON_72_112_2_18_3_4_30_0,"mmivcard:MMIVC_Malloc, size=%d")
TRACE_MSG(MMIVC_COMMON_97_112_2_18_3_4_30_1,"[MMIVCARD] MMIVC_Realloc ns == 0")
TRACE_MSG(MMIVC_COMMON_150_112_2_18_3_4_30_2,"[MMIVCARD] vc_getcharset p_prop 0x%x")
TRACE_MSG(MMIVC_COMMON_188_112_2_18_3_4_30_3,"[MMIVCARD] vc_getpropvalue p_prop 0x%x")
TRACE_MSG(MMIVC_COMMON_240_112_2_18_3_4_30_4,"[MMIVCARD] vc_freedata PNULL == p_data")
TRACE_MSG(MMIVC_COMMON_324_112_2_18_3_4_30_5,"[MMIVCARD] GetFirstValidLetterPos (PNULL == name) ")
TRACE_MSG(MMIVC_COMMON_353_112_2_18_3_4_30_6,"MMIVC_WriteFile_CallBack() error=%d, result=%d, param=%d")
TRACE_MSG(MMIVC_COMMON_358_112_2_18_3_4_30_7,"MMIVC_WriteFile_CallBack (PNULL == callback_ptr)")
TRACE_MSG(MMIVCALENDAR_199_112_2_18_3_4_31_8,"VcalSetDataToVcalInfo:p_datastr->wstr_len=%d")
TRACE_MSG(MMIVCALENDAR_205_112_2_18_3_4_31_9,"VcalSetDataToVcalInfo:p_text==PNULL")
TRACE_MSG(MMIVCALENDAR_214_112_2_18_3_4_31_10,"VcalSetDataToVcalInfo:proptype=%d")
TRACE_MSG(MMIVCALENDAR_750_112_2_18_3_4_32_11,"MMIVCAL_VcalInfoToSchedule: No support the rrule!")
TRACE_MSG(MMIVCALENDAR_840_112_2_18_3_4_32_12,"mmivcal:MMIVCAL_VcalendarDataToSch error_type=%d")
TRACE_MSG(MMIVCALENDAR_886_112_2_18_3_4_32_13,"mmivcal:MMIVCAL_VcalBuildInit result=%d")
TRACE_MSG(MMIVCALENDAR_906_112_2_18_3_4_32_14,"mmivcal:MMIVCAL_InitFileNamePtr result=%d")
TRACE_MSG(MMIVCALENDAR_984_112_2_18_3_4_33_15,"mmivcal:MMIVCAL_BuildVcalendarByEvent start")
TRACE_MSG(MMIVCALENDAR_1361_112_2_18_3_4_33_16,"mmivcal: vcal_lendth is=%d,vcal_info is %s ")
TRACE_MSG(MMIVCALENDAR_1426_112_2_18_3_4_33_17,"mmivcal:memory is not enough!")
TRACE_MSG(MMIVCALENDAR_1433_112_2_18_3_4_34_18,"mmivcal:valid_index is error!")
TRACE_MSG(MMIVCALENDAR_1458_112_2_18_3_4_34_19,"mmivcal:MMIVCAL_GetNextVcalItem start")
TRACE_MSG(MMIVCALENDAR_1498_112_2_18_3_4_34_20,"mmivcal:memory is not enough!")
TRACE_MSG(MMIVCALENDAR_1504_112_2_18_3_4_34_21,"mmivcal:not exist valid sch!")
TRACE_MSG(MMIVCALENDAR_1531_112_2_18_3_4_34_22,"mmivcal:AddtVcalItem uuid=%d,ret=%d")
TRACE_MSG(MMIVCALENDAR_1552_112_2_18_3_4_34_23,"mmivcal:ModifyVcalItem uuid is error")
TRACE_MSG(MMIVCALENDAR_1562_112_2_18_3_4_34_24,"mmivcal:ModifyVcalItem uuid=%d,ret=%d")
TRACE_MSG(MMIVCALENDAR_1579_112_2_18_3_4_34_25,"mmivcal:RemoveVcalItem uuid is error")
TRACE_MSG(MMIVCALENDAR_1588_112_2_18_3_4_34_26,"mmivcal:RemoveVcalItem uuid=%d,ret=%d")
TRACE_MSG(MMIVCALENDAR_1605_112_2_18_3_4_34_27,"mmivcal:VcalCreatfile()")
TRACE_MSG(MMIVCALENDAR_1691_112_2_18_3_4_34_28,"mmivcal: MMIVCAL_ReadFile pp_object 0x%x")
TRACE_MSG(MMIVCALENDAR_1697_112_2_18_3_4_34_29,"mmivcal:VcalCreatfile() file_handle=%d")
TRACE_MSG(MMIVCALENDAR_1787_112_2_18_3_4_34_30,"mmivcal:MMIVCAL_SaveVcalendarFile g_vcard_handle_result is %d")
TRACE_MSG(MMIVCALENDAR_1807_112_2_18_3_4_34_31,"mmivcal:MMIVCAL_SaveVcalendarFile: FFS_Write failed. result=%d")
TRACE_MSG(MMIVCALENDAR_1983_112_2_18_3_4_35_32,"mmivcal:MMIVCAL_GetDataByFile() file_handle=%d")
TRACE_MSG(MMIVCALENDAR_2035_112_2_18_3_4_35_33,"mmivcal: MMIVCAL_GetVcalendarFileName PNULL == entry_ptr")
TRACE_MSG(MMIVCALENDAR_2112_112_2_18_3_4_35_34,"MMIVCAL_SaveVCalendarByData 0x%x")
TRACE_MSG(MMIVCALENDAR_2117_112_2_18_3_4_35_35,"MMIVCAL_SaveVCalendarByData data_len %d !")
TRACE_MSG(MMIVCALENDAR_2184_112_2_18_3_4_35_36,"MMIVCAL_SaveVCalendarByData result %d !")
TRACE_MSG(MMIVCALENDAR_2231_112_2_18_3_4_35_37,"VcalWritedFile 0x%x")
TRACE_MSG(MMIVCALENDAR_2259_112_2_18_3_4_35_38,"VcalWritedFile: ffs_error=%d")
TRACE_MSG(MMIVCALENDAR_2342_112_2_18_3_4_35_39,"MMIVCAL_HandleWriteCNF result= %d")
TRACE_MSG(MMIVCALENDAR_2361_112_2_18_3_4_35_40,"MMIVCAL_HandleWriteCNF result %d")
TRACE_MSG(MMIVCALENDAR_2368_112_2_18_3_4_35_41,"MMIVCAL_HandleWriteCNF result_ptr->result=%d")
TRACE_MSG(MMIVCARD_240_112_2_18_3_4_36_42,"MMIVC_GetFamilyName contact_ptr 0x%x")
TRACE_MSG(MMIVCARD_288_112_2_18_3_4_36_43,"[MMIVCARD] VCARD_GetFamilyname contact_ptr 0x%x")
TRACE_MSG(MMIVCARD_337_112_2_18_3_4_36_44,"[MMIVCARD] VCARD_GetRev date_ptr 0x%x time_ptr 0x%x")
TRACE_MSG(MMIVCARD_458_112_2_18_3_4_37_45,"[MMIVCARD] MMIAPIVCARD_BuildVCardToBuffer parameter error !")
TRACE_MSG(MMIVCARD_727_112_2_18_3_4_37_46,"[MMIVCARD] MMIAPIVCARD_BuildVCardToBuffer parameter error !")
TRACE_MSG(MMIVCARD_1014_112_2_18_3_4_38_47,"[MMIVCARD] VCARD_GetVcardFileName PNULL == contact_ptr")
TRACE_MSG(MMIVCARD_1092_112_2_18_3_4_38_48,"[MMIVCARD] VCARD_GetVcardFileName PNULL == entry_ptr")
TRACE_MSG(MMIVCARD_1191_112_2_18_3_4_38_49,"[MMIVCARD] MMIAPIVCARD_InitFolder device_ptr 0x%x dir_ptr 0x%x device_len %d")
TRACE_MSG(MMIVCARD_1225_112_2_18_3_4_38_50,"mmivcard:vc_creatfile()")
TRACE_MSG(MMIVCARD_1281_112_2_18_3_4_38_51,"mmivcard:VCARD_SaveVcardFile g_vcard_handle_result is %d")
TRACE_MSG(MMIVCARD_1301_112_2_18_3_4_38_52,"VCARD_SaveVcardFile: FFS_Write failed. result=%d")
TRACE_MSG(MMIVCARD_1445_112_2_18_3_4_39_53,"mmivcard:VCARD_SaveVcardFile g_vcard_handle_result is %d")
TRACE_MSG(MMIVCARD_1510_112_2_18_3_4_39_54,"[VCard] MMIAPIVCARD_HandleWriteCNF result %d")
TRACE_MSG(MMIVCARD_1619_112_2_18_3_4_39_55,"[MMIVCARD] MMIAPIVCard_SaveVCardByData data_ptr 0x%x")
TRACE_MSG(MMIVCARD_1624_112_2_18_3_4_39_56,"MMIAPIVCard_SaveVCardByData data_len %d !")
TRACE_MSG(MMIVCARD_1694_112_2_18_3_4_39_57,"MMIAPIVCard_SaveVCardByData result %d !")
TRACE_MSG(MMIVCARD_1719_112_2_18_3_4_39_58,"[MMIVCARD]VCARD_WritedFile filename 0x%x")
TRACE_MSG(MMIVCARD_1748_112_2_18_3_4_39_59,"VCARD_WritedFile: ffs_error=%d")
TRACE_MSG(MMIVCARD_1892_112_2_18_3_4_40_60,"[MMIVCARD] MMIAPIVCARD_ReadFile pp_object 0x%x")
TRACE_MSG(MMIVCARD_1901_112_2_18_3_4_40_61,"mmivcard:MMIAPIVCARD_ReadFile() file_handle=%d")
TRACE_MSG(MMIVCARD_2034_112_2_18_3_4_40_62,"[MMIVCARD] VCARD_read_next_item pp_object 0x%x")
TRACE_MSG(MMIVCARD_2039_112_2_18_3_4_40_63,"mmivcard:VCARD_read_next_item() file_handle=%d")
TRACE_MSG(MMIVCARD_2181_112_2_18_3_4_40_64,"[MMIVCARD] MMIAPIVCARD_ReadFile pp_object 0x%x")
TRACE_MSG(MMIVCARD_2188_112_2_18_3_4_40_65,"mmivcard:MMIAPIVCARD_ReadFile() file_handle=%d")
TRACE_MSG(MMIVCARD_2283_112_2_18_3_4_40_66,"[MMIVCARD] MMIAPIVCARD_ParseByData pp_object 0x%x")
TRACE_MSG(MMIVCARD_2346_112_2_18_3_4_41_67,"[MMIVCARD] MMIAPIVCARD_GetDataByFile name_ptr 0x%x")
TRACE_MSG(MMIVCARD_2352_112_2_18_3_4_41_68,"mmivcard:MMIAPIVCARD_GetDataByFile() file_handle=%d")
TRACE_MSG(MMIVCARD_2403_112_2_18_3_4_41_69,"[MMIVCARD] vc_getproptype p_name 0x%x")
TRACE_MSG(MMIVCARD_2432_112_2_18_3_4_41_70,"[MMIVCARD] vc_setdatatopb p_data 0x%x")
TRACE_MSG(MMIVCARD_2512_112_2_18_3_4_41_71,"[MMIVCARD] vc_setdatatopb p_data 0x%x")
TRACE_MSG(MMIVCARD_2599_112_2_18_3_4_41_72,"[MMIVCARD] vc_gettelephonenum p_Object 0x%x p_PropName 0x%x pp_Array 0x%x")
TRACE_MSG(MMIVCARD_2718_112_2_18_3_4_41_73,"[MMIVCARD] vc_setteltopb p_telstr 0x%x ")
TRACE_MSG(MMIVCARD_2740_112_2_18_3_4_41_74,"[MMIVCARD] vc_cleanuptelnum p_org_num 0x%x ")
TRACE_MSG(MMIVCARD_2833_112_2_18_3_4_41_75,"[MMIVCARD] MMIAPIVCARD_AddObjectToPB pObject 0x%x ")
TRACE_MSG(MMIVCARD_2952_112_2_18_3_4_42_76,"[MMIVCARD] MMIAPIVCARD_BuildVCardToBuffer parameter error !")
TRACE_MSG(MMIVCARD_3189_112_2_18_3_4_42_77,"[MMIVCARD] MMIAPIVCARD_AddObjectToPB pObject 0x%x ")
END_TRACE_MAP(MMI_APP_VCARD_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_VCARD_TRC_H_

