/*****************************************************************************
** File Name:      mmisfr_ctcc_main.c
** Author:
** Copyright (C) 2000-2100, UNISOC Technoloies Co.,Ltd.
** Description:    Entry of DM APP Module
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                       NAME                          DESCRIPTION
** 2020/11/16           mark.zhang1             Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "os_api.h"
#include "sig_code.h"
#include "in_message.h"
#include "mmiutil.h"
#include "mmisfr_ctcc_main.h"
#include "mmisfr_ctcc_pdp.h"

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
MMI_APPLICATION_T g_sfr_ctcc_app = {0};

/**-------------------------------------------------------------------------*
**                         STATIC DEFINITION                       *
**--------------------------------------------------------------------------*/
/* ctcc sim ready? */
LOCAL BOOLEAN s_fire_flag = FALSE;

LOCAL uint16 s_sfr_nv_len[] =
{
    MMI_DUAL_SYS_MAX * sizeof(SFR_ICCID), //MMINV_SFR_SIM_ICCID
};

LOCAL SFR_ICCID s_sfr_iccid[MMI_DUAL_SYS_MAX] = {0};
LOCAL char s_sim_iccid[SFR_ICCID_LENGTH + 1] = {0};
LOCAL char s_nv_iccid[SFR_ICCID_LENGTH + 1] = {0};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN is_sim_ready(void) //ctcc card
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;//single card

    SCI_TRACE_LOW("[DMCTCC] is_sim_ready, s_fire_flag=%d",
                  s_fire_flag);
    return (s_fire_flag  && MMIAPIPHONE_GetSimExistedStatus(dual_sys)
            && MMIPHONE_IsSimOk(dual_sys));
}

LOCAL char *get_saved_iccid(MN_DUAL_SYS_E sim_index)
{
    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_SFR_SIM_ICCID, s_sfr_iccid))
    {
        SCI_MEMSET(s_sfr_iccid, 0, MMI_DUAL_SYS_MAX * sizeof(SFR_ICCID));
        MMI_WriteNVItem(MMINV_SFR_SIM_ICCID, s_sfr_iccid);
    }
    SCI_MEMCPY(s_nv_iccid, s_sfr_iccid[sim_index].num, SFR_ICCID_LENGTH);

    return s_nv_iccid;
}

LOCAL BOOLEAN is_iccid_changed(MN_DUAL_SYS_E sim_index)
{
    char *sim_data;
    char *nv_data;

    if (sim_index >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }
    sim_data = sfr_get_iccid(sim_index);
    nv_data = get_saved_iccid(sim_index);
    SCI_TraceLow("[DMCTCC] is_iccid_changed,  sim_data=%s ,  nv_data=%s",
                 sim_data, nv_data);
    if (SCI_MEMCMP(sim_data, nv_data, SFR_ICCID_LENGTH) == 0)
    {
        SCI_TraceLow("[DMCTCC] ICCID  not changed.");
        return FALSE;
    }

    return TRUE;
}

LOCAL BOOLEAN is_ctcc_sim(DPARAM param)
{
    PHONE_SERVICE_STATUS_T service_status = {0};
    APP_MN_SIM_READY_IND_T  sim_ready_ind   = *((APP_MN_SIM_READY_IND_T *)param);

    //MMIAPIPHONE_GetCurServiceStatus(0, &service_status);
    //uint16 plmn_mcc = service_status.plmn.mcc ;
    //uint16 plmn_mnc = service_status.plmn.mnc ;
    uint16 plmn_mcc = sim_ready_ind.hplmn.mcc;
    uint16 plmn_mnc = sim_ready_ind.hplmn.mnc;
    SCI_TRACE_LOW("[DMCTCC] is_ctcc_sim, mcc =%d, mnc = %d", plmn_mcc, plmn_mnc);

    if ((460 == plmn_mcc) && ((03 == plmn_mnc) || (05 == plmn_mnc)
                              || (11 == plmn_mnc)))
    {
        return TRUE;
    }

    return FALSE;
}

LOCAL MMI_RESULT_E sfr_main_func(PWND app_ptr, uint16 msg_id,
                                 DPARAM param)
{
    MN_DUAL_SYS_E sim_index   = MN_DUAL_SYS_1;

    if (HTTP_SIGNAL_BEG <= msg_id && msg_id <= HTTP_SIGNAL_END)
    {
        return sfr_handle_http_msg(app_ptr, msg_id, param);
    }
    SCI_TRACE_LOW("[DMCTCC]sfr_main_func, msg_id=%d",
                  (msg_id - APP_MN_SIM_READY_IND));
    switch (msg_id)
    {
        case APP_MN_SIM_READY_IND:
            if (is_ctcc_sim(param) && is_iccid_changed(sim_index))
            {
                s_fire_flag = TRUE;
                MMISFR_CTCC_HandleNetworkStatus();
            }
            break;

        case APP_MN_SIM_NOT_READY_IND:
            s_fire_flag = FALSE;
            break;

        default:
            break;
    }
    return MMI_RESULT_FALSE;
}

PUBLIC char *sfr_get_iccid(MN_DUAL_SYS_E sim_index)
{
    MNSIM_ICCID_T icc_id = {0};

    if (0 == strlen(s_sim_iccid))
    {
        MNSIM_GetICCIDEx(MN_DUAL_SYS_1, &icc_id);//single card
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, icc_id.id_num, SFR_ICCID_LENGTH,
                           s_sim_iccid);
    }
    SCI_TraceLow("[DMCTCC] sfr_get_iccid,  s_sim_iccid=%s",
                 s_sim_iccid);

    return s_sim_iccid;
}

PUBLIC void sfr_save_iccid(MN_DUAL_SYS_E sim_index)
{
    if (MN_RETURN_FAILURE  == MMI_ReadNVItem(MMINV_SFR_SIM_ICCID, s_sfr_iccid)
            || SCI_MEMCMP(s_sfr_iccid[sim_index].num, s_sim_iccid, SFR_ICCID_LENGTH) != 0)
    {
        //SCI_MEMSET(s_sfr_iccid[sim_index].num, 0, sizeof(SFR_ICCID));
        SCI_MEMCPY(s_sfr_iccid[sim_index].num, s_sim_iccid, SFR_ICCID_LENGTH);
        MMI_WriteNVItem(MMINV_SFR_SIM_ICCID, s_sfr_iccid);
        SCI_TRACE_LOW("[DMCTCC] sfr_save_iccid END!");
    }
    else
    {
        SCI_TRACE_LOW("[DMCTCC] sfr_save_iccid, Do nothing!");
    }
}

PUBLIC void MMISFR_CTCC_HandleNetworkStatus(void)
{
    SCI_TRACE_LOW("[DMCTCC] MMISFR_CTCC_HandleNetworkStatus start");
    if (is_sim_ready())
    {
        sfr_trigger_pdp();
    }
}

PUBLIC void MMISFR_CTCC_InitModule(void)
{
    MMI_RegModuleNv(MMI_MODULE_SFR_CTCC, s_sfr_nv_len,
                    sizeof(s_sfr_nv_len) / sizeof(uint16));
    g_sfr_ctcc_app.ProcessMsg = sfr_main_func;
}

