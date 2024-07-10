#include "sci_types.h"
#include "mmk_app.h"
#include "mmi_module.h"
#include "mn_type.h"
#include "mn_api.h"
#include "http_api.h"
#include "mmi_signal_ext.h"
#include "sfr_carrier_manager.h"
#include "sfr_header.h"
#include "sfr_nv.h"

LOCAL uint16 mmisfr_nv_len[] =
{
    MMI_DUAL_SYS_MAX *sizeof(SFR_ICCID), //MMINV_SFR_SIM0_ICCID
    sizeof(int32),
#ifdef SFR_SUPPORT_CUCC
    sizeof(uint8),    // MMINV_SFR_CUCC_TYPE
    sizeof(BOOLEAN),    // MMINV_SFR_CUCC_COLLECT
    sizeof(uint8),    // MMINV_SFR_CUCC_CRICLE
    sizeof(uint8),    // MMINV_SFR_CUCC_SERVER
    sizeof(MMI_SFR_CUCC_HIS_ITEM) * CUCC_HIS_COUNT, //MMINV_SFR_CUCC_HISTORY
#endif
};

LOCAL SFR_ICCID s_sfr_iccid[MMI_DUAL_SYS_MAX] = {0,};

PUBLIC void MMISFR_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_SFR, mmisfr_nv_len, sizeof(mmisfr_nv_len) / sizeof(uint16));
}

PUBLIC char * MMISFR_NVGetICCID(uint8 sim)
{
    if (sim >= MMI_DUAL_SYS_MAX)
    {
        return NULL;
    }
    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_SFR_SIM_ICCID, &s_sfr_iccid))
    {
        SCI_MEMSET(&s_sfr_iccid, 0, MMI_DUAL_SYS_MAX *sizeof(SFR_ICCID));
        MMI_WriteNVItem(MMINV_SFR_SIM_ICCID, &s_sfr_iccid);
    }
    return s_sfr_iccid[sim].num;
}

PUBLIC void MMISFR_NVSetICCID(uint8 sim, char *iccid)
{
    if (sim >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_SFR_SIM_ICCID, &s_sfr_iccid))
    {
        SCI_MEMSET(&s_sfr_iccid, 0, MMI_DUAL_SYS_MAX *sizeof(SFR_ICCID));
    }

    SCI_MEMSET(&s_sfr_iccid[sim], 0, sizeof(SFR_ICCID));
    SCI_MEMCPY(&s_sfr_iccid[sim].num, iccid, SFR_ICCID_LENGTH);

    MMI_WriteNVItem(MMINV_SFR_SIM_ICCID, &s_sfr_iccid);
}

PUBLIC BOOLEAN MMISFR_IsICCIDChanged(MN_DUAL_SYS_E sim)
{
    char *p_nv_iccid;
    char nv_iccid[SFR_ICCID_LENGTH + 1] = {0};
    char sim_iccid[SFR_ICCID_LENGTH + 1] = {0};
    MNSIM_ICCID_T ccid;

    if (sim >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    p_nv_iccid = MMISFR_NVGetICCID(sim);
    if (p_nv_iccid == NULL)
    {
        return FALSE;
    }

    SCI_MEMCPY(nv_iccid, p_nv_iccid, SFR_ICCID_LENGTH);

    MNSIM_GetICCIDEx(sim, &ccid);
    MMIAPICOM_BcdToStr(0/*PACKED_LSB_FIRST*/, ccid.id_num, 20, sim_iccid);

    SCI_TraceLow("[Self Reg] Telecom sim ICCID sim (%d) : [%s]", sim, sim_iccid);
    SCI_TraceLow("[Self Reg] Telecom sim ICCID sim (%d) nv : [%s]", sim, nv_iccid);
    if (SCI_MEMCMP(sim_iccid, nv_iccid, SFR_ICCID_LENGTH) == 0)
    {
        SCI_TraceLow("[Self Reg] Telecom sim ICCID not changed.");
        return FALSE;
    }

    SCI_TraceLow("[Self Reg] Telecom sim ICCID changed sim (%d): [%s]", sim, sim_iccid);
    return TRUE;
}

PUBLIC BOOLEAN MMISFR_SaveICCID(MN_DUAL_SYS_E sim)
{
    char sim_iccid[SFR_ICCID_LENGTH + 1] = {0};
    MNSIM_ICCID_T ccid;

    if (sim >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }
    MNSIM_GetICCIDEx(sim, &ccid);
    MMIAPICOM_BcdToStr(0/*PACKED_LSB_FIRST*/, ccid.id_num, 20, sim_iccid);
    MMISFR_NVSetICCID(sim, sim_iccid);
    SCI_TraceLow("[Self Reg] Telecom sim ICCID Save [%s]", sim_iccid);
    if (MMISFR_IsICCIDChanged(sim))
    {
       SCI_TraceLow("[Self Reg] Telecom sim ICCID Save [OOOOROROROORORO]", sim_iccid);
    }
    return TRUE;
}

PUBLIC uint32 MMISFR_GetEventID(void)
{
    int32 evid;

    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_SFR_EVENT_ID, &evid))
    {
        evid = 0;
        MMI_WriteNVItem(MMINV_SFR_EVENT_ID, &evid);
    }
    return evid;
}

PUBLIC void MMISFR_SetEventID(uint32 evid)
{
    MMI_WriteNVItem(MMINV_SFR_EVENT_ID, &evid);
}

#ifdef SFR_SUPPORT_CUCC

PUBLIC uint8 MMISFR_GetCUCCType(void)
{
    uint8 type;

    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_SFR_CUCC_TYPE, &type))
    {
        type = MMISFR_CUCC_TYPE_FIRST;
        SCI_TRACE_LOW("[Self Reg CUCC] get type  <first>");
        MMI_WriteNVItem(MMINV_SFR_CUCC_TYPE, &type);
    }
    return type;
}

PUBLIC void MMISFR_SetCUCCType(uint8 type)
{
    MMI_WriteNVItem(MMINV_SFR_CUCC_TYPE, &type);
}

PUBLIC uint8  MMISFR_GetCollectInfoMode(void)
{
  BOOLEAN collect_info_mode;
  if(MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_SFR_CUCC_COLLECT,&collect_info_mode))
  {
     collect_info_mode = FALSE;
     MMI_WriteNVItem(MMINV_SFR_CUCC_COLLECT, &collect_info_mode);
  }

  return collect_info_mode;
}


PUBLIC void MMISFR_SetCollectInfoMode(BOOLEAN collect_info_mode)
{
    MMI_WriteNVItem(MMINV_SFR_CUCC_COLLECT, &collect_info_mode);
}

PUBLIC uint8 MMISFR_GetCUCCRegCircle(void)
{
    uint8 circle;

    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_SFR_CUCC_CRICLE, &circle))
    {
        circle = MMISFR_CUCC_CIRCLE_DAY;
        MMI_WriteNVItem(MMINV_SFR_CUCC_CRICLE, &circle);
    }
    return circle;
}

PUBLIC void MMISFR_SetCUCCRegCircle(uint8 circle)
{
    int timm = (24 * 60);
    MMI_WriteNVItem(MMINV_SFR_CUCC_CRICLE, &circle);

    switch(circle)
    {
        case MMISFR_CUCC_CIRCLE_5MIN : timm = (5); break;
	 case MMISFR_CUCC_CIRCLE_HOUR : timm = (60); break;
	 case MMISFR_CUCC_CIRCLE_DAY : timm = (24 * 60); break;
	 case MMISFR_CUCC_CIRCLE_WEEK: timm = (7 * 24 * 60); break;
	 case MMISFR_CUCC_CIRCLE_MONTH: timm = (30 * 24 * 60); break;
	 default: break;
    }
    MMISFR_ResetEvent(timm);
}

PUBLIC uint8 MMISFR_GetCUCCRegServer(void)
{
    uint8 serv;

    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_SFR_CUCC_SERVER, &serv))
    {
        serv = MMISFR_CUCC_SERVER_REG;
        MMI_WriteNVItem(MMINV_SFR_CUCC_SERVER, &serv);
    }
    return serv;
}

PUBLIC void MMISFR_SetCUCCRegServer(uint8 serv)
{
    MMI_WriteNVItem(MMINV_SFR_CUCC_SERVER, &serv);
}

#endif
