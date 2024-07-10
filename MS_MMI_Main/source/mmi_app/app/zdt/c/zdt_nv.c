/*************************************************************************
 ** File Name:      mmicc_nv.c                                          *
 ** Author:         bruce.chi                                           *
 ** Date:           2006/09/25                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/25     bruce.chi        Create.                              *
*************************************************************************/


#define _MMIZDT_NV_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "sci_types.h"
#include "zdt_nv.h"
#include "zdt_net.h"
#include "mmi_modu_main.h"

LOCAL ZDT_ICCID s_zdt_iccid = {0};


#ifdef ZDT_NFC_SUPPORT
LOCAL ZDT_NFC_INFO s_zdt_nfc_info = {0};
#endif

/*the length of set nv*/
const uint16 zdt_nv_len[] =
{
    sizeof(uint8),
    sizeof(uint8),
#ifdef ZDT_NET_SUPPORT
    sizeof(uint8),
    sizeof(ZDT_ICCID),
    sizeof(ZDT_RFID),
#endif
#ifdef ZDT_WIFI_SUPPORT
    sizeof(uint8),
#endif
#ifdef ZDT_PLAT_SHB_SUPPORT
    sizeof(uint8),
#endif

#ifdef ZDT_LED_SUPPORT
    sizeof(uint8),
    sizeof(uint8),
    sizeof(uint8),
    sizeof(uint8),
    sizeof(uint8),
#endif
    sizeof(uint8),
    sizeof(uint8),//yangyu add for MMI_ZDT_NV_REJECT_CALL_ONOFF
    sizeof(uint8),//yangyu add ZDT_FIRST_BIND
    sizeof(STEP_NV_T),
    sizeof(uint8),
#ifdef ZDT_NFC_SUPPORT
    sizeof(ZDT_NFC_INFO),
#endif
    sizeof(uint8),//MMI_ZDT_NV_NEED_WHITECALL
    sizeof(uint8),//MMI_ZDT_NV_SETUP_WIZARD 开机向导
    sizeof(uint8),
};

/*****************************************************************************/
// 	Description : register set module nv len and max item
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIZDT_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_ZDT, zdt_nv_len, ARR_SIZE(zdt_nv_len));
}

LOCAL uint8  s_zdt_first_pwon  = TRUE;

PUBLIC uint8 MMIZDT_NV_GetFirstPWON(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 first_pwon = TRUE;
    return_value = MMI_ReadNVItem(MMI_ZDT_NV_FIRST_PWON,&first_pwon);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //自动识别
        first_pwon = TRUE;
        MMI_WriteNVItem(MMI_ZDT_NV_FIRST_PWON,&first_pwon);
    }
    s_zdt_first_pwon = first_pwon;
    return first_pwon;
}

PUBLIC BOOLEAN  MMIZDT_NV_SetFirstPWON(uint8 first_pwon) 
{
    uint8 cur_val = first_pwon;
    if(s_zdt_first_pwon != first_pwon)
    {
        MMI_WriteNVItem(MMI_ZDT_NV_FIRST_PWON,&cur_val);
        s_zdt_first_pwon = first_pwon;
        return TRUE;
    }
    return FALSE;
}

PUBLIC uint8 MMIZDT_GetFirstPWON(void)
{
    return s_zdt_first_pwon;
}

#if 1
LOCAL uint8  s_zdt_need_reset  = TRUE;
PUBLIC uint8 MMIZDT_NV_GetNeedReset(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 need_reset = TRUE;
    return_value = MMI_ReadNVItem(MMI_ZDT_NV_NEED_RESET,&need_reset);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //自动识别
        need_reset = TRUE;
        MMI_WriteNVItem(MMI_ZDT_NV_NEED_RESET,&need_reset);
    }
    s_zdt_need_reset = need_reset;
    return need_reset;
}

PUBLIC BOOLEAN  MMIZDT_NV_SetNeedReset(uint8 need_reset) 
{
    uint8 cur_val = need_reset;
    if(s_zdt_need_reset != need_reset)
    {
        MMI_WriteNVItem(MMI_ZDT_NV_NEED_RESET,&cur_val);
        s_zdt_need_reset = need_reset;
        return TRUE;
    }
    return FALSE;
}

PUBLIC uint8 MMIZDT_GetNeedReset(void)
{
    return s_zdt_need_reset;
}
#endif

#ifdef ZDT_NET_SUPPORT
PUBLIC char * MMIZDT_NVGetICCID(void)
{
    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMI_ZDT_NV_ICCID, &s_zdt_iccid))
    {
        SCI_MEMSET(&s_zdt_iccid, 0, sizeof(ZDT_ICCID));
        MMI_WriteNVItem(MMI_ZDT_NV_ICCID, &s_zdt_iccid);
    }
   return s_zdt_iccid.num;
}

PUBLIC void MMIZDT_NVSetICCID(char *iccid)
{
    SCI_MEMSET(&s_zdt_iccid, 0, sizeof(ZDT_ICCID));
    SCI_MEMCPY(&s_zdt_iccid.num, iccid, ZDT_ICCID_LEN);

    MMI_WriteNVItem(MMI_ZDT_NV_ICCID, &s_zdt_iccid);
}

PUBLIC BOOLEAN MMIZDT_IsICCIDChanged(char * sim_iccid)
{
    char *p_nv_iccid;
    char nv_iccid[ZDT_ICCID_LEN + 1] = {0};
    if(sim_iccid == NULL)
    {
        return FALSE;
    }
    p_nv_iccid = MMIZDT_NVGetICCID();

    if (p_nv_iccid[0]== NULL)
    {
    	MMIZDT_NVSetICCID(sim_iccid);
        return FALSE;
    }

    SCI_MEMCPY(nv_iccid, p_nv_iccid, ZDT_ICCID_LEN);

    if (SCI_MEMCMP(sim_iccid, nv_iccid, ZDT_ICCID_LEN) == 0)
    {
        SCI_TraceLow("MMIZDT_IsICCIDChanged not changed.");
        return FALSE;
    }

    SCI_TraceLow("MMIZDT_IsICCIDChanged changed [%s]", sim_iccid);
    return TRUE;
}
#endif

#ifdef ZDT_NET_SUPPORT
PUBLIC char * MMIZDT_NVGetRFID(void)
{
    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMI_ZDT_NV_RFID, &g_zdt_phone_rfid))
    {
        SCI_MEMSET(&g_zdt_phone_rfid, 0, sizeof(ZDT_RFID));
        MMI_WriteNVItem(MMI_ZDT_NV_RFID, &g_zdt_phone_rfid);
    }
   return g_zdt_phone_rfid.num;
}

PUBLIC void MMIZDT_NVSetRFID(char *rfid)
{
    SCI_MEMSET(&g_zdt_phone_rfid, 0, sizeof(ZDT_RFID));
    SCI_MEMCPY(&g_zdt_phone_rfid.num, rfid, ZDT_RFID_LEN);

    MMI_WriteNVItem(MMI_ZDT_NV_RFID, &g_zdt_phone_rfid);
}

PUBLIC void MMIZDT_NVClearRFID(void)
{
    SCI_MEMSET(&g_zdt_phone_rfid, 0, sizeof(ZDT_RFID));
    MMI_WriteNVItem(MMI_ZDT_NV_RFID, &g_zdt_phone_rfid);
}

PUBLIC BOOLEAN MMIZDT_IsRFIDChanged(char * phone_rfid)
{
    char *p_nv_rfid;
    char nv_rfid[ZDT_RFID_LEN + 1] = {0};
    if(phone_rfid == NULL)
    {
        return FALSE;
    }
    p_nv_rfid = g_zdt_phone_rfid.num;

    if (p_nv_rfid[0]== NULL)
    {
    	 MMIZDT_NVSetRFID(phone_rfid);
        return FALSE;
    }

    SCI_MEMCPY(nv_rfid, p_nv_rfid, ZDT_RFID_LEN);

    if (SCI_MEMCMP(phone_rfid, nv_rfid, ZDT_RFID_LEN) == 0)
    {
        SCI_TraceLow("MMIZDT_IsRFIDChanged not changed.");
        return FALSE;
    }

    SCI_TraceLow("MMIZDT_IsRFIDChanged changed [%s]", phone_rfid);
    return TRUE;
}
#endif

PUBLIC void ZDT_NV_SetIsAutoPower(BOOLEAN isOn)
{
    BOOLEAN status = isOn;
    MMINV_WRITE(MMI_ZDT_AUTO_POWER, &status);
}

PUBLIC BOOLEAN ZDT_NV_GetIsAutoPower()
{
    BOOLEAN status = FALSE;
    MN_RETURN_RESULT_E  return_value    =   MN_RETURN_FAILURE;

    MMINV_READ(MMI_ZDT_AUTO_POWER, &status, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMI_ZDT_AUTO_POWER, &status);
    }
    SCI_TraceLow("ZDT_NV_GetIsAutoPower status = %d " ,status );
    return status;
}

PUBLIC void  MMIZDT_NV_SetRejectUnknownCall(uint8 isOn) 
{
     BOOLEAN status = isOn;
    MMINV_WRITE(MMI_ZDT_NV_REJECT_CALL_ONOFF, &status);
}

#ifdef ZDT_NFC_SUPPORT
PUBLIC BOOLEAN MMIZDT_NVGetNFC_INFO(uint8 * info_buf)
{
    BOOLEAN res = TRUE;
    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMI_ZDT_NV_NFC_INFO, &s_zdt_nfc_info))
    {
        SCI_MEMSET(&s_zdt_nfc_info, 0, sizeof(ZDT_NFC_INFO));
        MMI_WriteNVItem(MMI_ZDT_NV_NFC_INFO, &s_zdt_nfc_info);
        res = FALSE;
    }
    if(s_zdt_nfc_info.info[0] != 0)
    {
        SCI_MEMCPY(info_buf,s_zdt_nfc_info.info,ZDT_NFC_INFO_LEN);
    }
   return res;
}

PUBLIC void MMIZDT_NVSetNFC_INFO(uint8 *info_buf)
{
    SCI_MEMSET(&s_zdt_nfc_info, 0, sizeof(ZDT_NFC_INFO));
    SCI_MEMCPY(&s_zdt_nfc_info.info, info_buf, ZDT_NFC_INFO_LEN);
    MMI_WriteNVItem(MMI_ZDT_NV_NFC_INFO, &s_zdt_nfc_info);
}
#endif

#if 1
LOCAL uint8  s_zdt_need_white_call  = TRUE;
PUBLIC uint8 MMIZDT_NV_GetNeedWhiteCall(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 need_white_call = TRUE;
    return_value = MMI_ReadNVItem(MMI_ZDT_NV_NEED_WHITECALL,&need_white_call);
    if (MN_RETURN_SUCCESS != return_value)
    {
        need_white_call = FALSE;
        MMI_WriteNVItem(MMI_ZDT_NV_NEED_WHITECALL,&need_white_call);
    }
    s_zdt_need_white_call = need_white_call;
    return need_white_call;
}

PUBLIC BOOLEAN  MMIZDT_NV_SetNeedWhiteCall(uint8 need_white_call) 
{
    uint8 cur_val = need_white_call;
    if(s_zdt_need_white_call != need_white_call)
    {
        MMI_WriteNVItem(MMI_ZDT_NV_NEED_WHITECALL,&cur_val);
        s_zdt_need_white_call = need_white_call;
        return TRUE;
    }
    return FALSE;
}

PUBLIC uint8 MMIZDT_GetNeedWhiteCall(void)
{
    return s_zdt_need_white_call;
}
#endif

#ifdef SETUP_WIZARD_SUPPORT

PUBLIC uint8 MMIZDT_Get_Setup_Wizard_Status(void)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    uint8 status = TRUE;
    return_value = MMI_ReadNVItem(MMI_ZDT_NV_SETUP_WIZARD,&status);
    if (MN_RETURN_SUCCESS != return_value)
    {
        status = FALSE;
        MMI_WriteNVItem(MMI_ZDT_NV_SETUP_WIZARD,&status);
    }
    return status;
}

PUBLIC void MMIZDT_NV_Set_Setup_Wizard(uint8 status)
{
    uint8 set_status = status;
    MMI_WriteNVItem(MMI_ZDT_NV_SETUP_WIZARD,&set_status);
}

#endif

