/*************************************************************************
 ** File Name:      mmidm_nv.c                                           *
 ** Author:         jian.ma                                              *
 ** Date:           2009/07/02                                           *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved   *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2009/07/02     jian.ma         Create.                               *
*************************************************************************/

#define MMIDM_NV_C_
//#include "std_header.h"
#if (defined(DM_SUPPORT) || defined(DM_ONLY_SELFREG_SUPPORT))
#include "sci_types.h"
#include "mmi_modu_main.h"
#include "mmidm_app.h"
#include "mmidm_internal.h"

/*the length of dm nv*/
const uint16 dm_nv_len[] =
{
    sizeof(MN_IMSI_T),              //MMINV_DM_IMSI_INFO    
    sizeof(char)*MMIDM_APN_MAX_LEN,  //MMINV_DM_APN
    sizeof(char)*MMIDM_GATEWAY_IP_MAX_LEN, //MMINV_DM_GATEWAY_IP
    sizeof(char)*MMIDM_PORT_MAX_LEN,   //MMINV_DM_PORT
    sizeof(char)*MMIDM_SRNUM_MAX_LEN,  //MMINV_DM_SRNUM
    sizeof(char)*MMIDM_SRPORT_MAX_LEN,  //MMINV_DM_SRPORT
    sizeof(char)*DM_MAX_ID_STR,  //MMINV_DM_MON_SET
    sizeof(char)*DM_MAX_ID_STR,  //MMINV_DM_MAN_SET
    sizeof(char)*DM_MAX_ID_STR,  //MMINV_DM_IMEI_SET
    sizeof(BOOLEAN),             //MMINV_DM_HANDSET_LOCK
    sizeof(BOOLEAN),             //MMINV_DM_DOWNLOAD_FINISH
    sizeof(MMIDM_TEST_SERVER_E), //MMINV_DM_SERVER_TYPE
    sizeof(BOOLEAN),             //MMINV_DM_REG_SWITCH
    sizeof(FOTA_UPDATA_STATE_E),  //MMINV_DM_FOTA_STATE
    sizeof(int)*DM_SCOMO_APPID_MAX_NUMBER,  //MMINV_DM_APPID_TAB
    sizeof(MMIDM_RESUME_TYPE),    //MMINV_DM_RESUME_TYPE
    sizeof(E_VDM_NIA_UIMode_t),
    sizeof(MMIDM_BROKEN_TYPE),   //MMINV_DM_BROKEN_TYPE
    sizeof(MMIDM_PL_ALERT_TYPE),//MMIDM_PL_ALERT_TYPE //mary
    sizeof(MN_DUAL_SYS_E)//MMINV_DM_MAIN_SIM    
};

/*****************************************************************************/
// 	Description : register dm module nv len and max item
//	Global resource dependence : none
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDM_RegDmNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_DM, dm_nv_len, sizeof(dm_nv_len) / sizeof(uint16));
}

#endif


