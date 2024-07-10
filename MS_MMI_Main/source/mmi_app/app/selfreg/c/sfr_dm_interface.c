#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "mmiphone_export.h"

#include "mmi_filemgr.h"
#include "version.h"
#include "mmipdp_export.h"
#include "in_message.h"
#include "mmieng_internal.h"
#include "mmisms_app.h"
#include "mmi_module.h"
#include "dal_time.h"
#include "mmk_authen.h"
#include "mmisms_set.h"
#include "mmi_nv.h"
#include "mmipb_export.h"
#include "mn_type.h"

static char s_sfr_com_buff[128];

const char *sfr_dm_get_imei1(void);
const char *sfr_dm_get_imei2(void);

const char *sfr_dm_get_meid(void)
{
#if 0
    if (FALSE == MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)
		&& TRUE == MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
        return sfr_dm_get_imei2();
    }
    return sfr_dm_get_imei1();
#endif
}

const char *sfr_dm_get_manuf(void)
{
    return "HMD";
}
const char *sfr_dm_get_model(void)
{
#if 0
#ifndef WIN32
    char *temp_ptr = NULL;

    temp_ptr = PRODUCT_TYPE_Read();

    if (temp_ptr != NULL)
    {
        char *p = NULL;
	 for(p = temp_ptr; p != NULL && *p != '\0'; p++)
	 {
	     if(*p == '-')
	         *p = ' ';
	 }
        strcpy(s_sfr_com_buff, sfr_dm_get_manuf());
	 strcat(s_sfr_com_buff, "-");
        strcat(s_sfr_com_buff, temp_ptr);

        return s_sfr_com_buff;
    }
    return "";
#else
    return "HMD-TA 1289";
#endif
#endif
    return "HMD-TA 1289";
}

const char *sfr_dm_get_sw_version(void)
{
#ifndef WIN32
    char *temp_ptr = NULL;

    temp_ptr = strchr(VERSION_GetInfo(PROJECT_VERSION), ':') + 1;

    if (temp_ptr != NULL)
    {
        strncpy(s_sfr_com_buff, temp_ptr, 32);
        return s_sfr_com_buff;
    }
#else
    return "1.0.0.0";
#endif
}

const char *sfr_dm_get_sim1_cdma_imsi(void)
{
    return "";//"460036180704398";
}

const char *sfr_dm_get_ue_type(void)
{
    return "1";
}

const char *sfr_dm_get_sim1_iccid(void)
{
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
    {
        char sim_iccid[2*MNSIM_ICCID_ID_NUM_LEN + 1];
        MNSIM_ICCID_T ccid;

        MNSIM_GetICCIDEx(MN_DUAL_SYS_1, &ccid);
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, ccid.id_num, 20, s_sfr_com_buff);

        return s_sfr_com_buff;
    }
    return "";
}

const char *sfr_dm_get_sim1_lte_imsi(void)
{
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
    {
        MN_IMSI_T tmp_imsi = MNSIM_GetImsiEx(MN_DUAL_SYS_1);

        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)tmp_imsi.imsi_val, MN_MAX_IMSI_ARR_LEN<<1, s_sfr_com_buff);

        return s_sfr_com_buff + 1; // +1 is skip the "9"
    }
    return "";
}

const char *sfr_dm_get_sim2_cdma_imsi(void)
{
    return "";
}

const char *sfr_dm_get_sim2_iccid(void)
{
#if 0
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
        char sim_iccid[2*MNSIM_ICCID_ID_NUM_LEN + 1] = {0};
        MNSIM_ICCID_T ccid = {0};

        MNSIM_GetICCIDEx(MN_DUAL_SYS_2, &ccid);
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, ccid.id_num, 20, s_sfr_com_buff);

        return s_sfr_com_buff;
    }
    return "";
#endif
}

const char *sfr_dm_get_sim2_lte_imsi(void)
{
#if 0
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
        MN_IMSI_T tmp_imsi = MNSIM_GetImsiEx(MN_DUAL_SYS_2);

        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)tmp_imsi.imsi_val, MN_MAX_IMSI_ARR_LEN<<1, s_sfr_com_buff);

        return s_sfr_com_buff + 1; // +1 is skip the "9"
    }
    return "";
#endif
}

const char *sfr_dm_get_mac_id(void)
{
#if 0
    char buf[8];

    //return "24:e2:71:f4:d7:b0";
    if (TCPIPNETIF_GetMac(buf, 8, TCPIPNETIF_GetNetifWithNetid(g_sfr_linker.net_id)))
    {
        sprintf(s_sfr_com_buff, "%02x:%02x:%02x:%02x:%02x:%02x",
                buf[0], buf[1], buf[2], buf[3], buf[4],buf[5]);

        return s_sfr_com_buff;
    }
#endif
    return "";
}

const char *sfr_dm_get_sim1_mnc(void)
{
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
    {
        MN_PLMN_T plmn_info = {0};
        MMIAPIPHONE_GetSimHPLMN(&plmn_info, MN_DUAL_SYS_1);
        sprintf(s_sfr_com_buff, "%.3d%.2d", plmn_info.mcc, plmn_info.mnc);
        return s_sfr_com_buff;
    }
    return "";
}

const char *sfr_dm_get_sim2_mnc(void)
{
#if 0
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
        MN_PLMN_T plmn_info = {0};
        MMIAPIPHONE_GetSimHPLMN(&plmn_info, MN_DUAL_SYS_2);
        sprintf(s_sfr_com_buff, "%.3d%.2d", plmn_info.mcc, plmn_info.mnc);
        return s_sfr_com_buff;
    }
    return "";
#endif
}

const char *sfr_dm_get_sim1_msisdn(void)
{
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
    {
        PHONEBOOK_ENTRY_T sim_contact = {0};

        sim_contact.entry_id = 1;

        PHONEBOOK_ReadPhonebookEntryEx(MN_DUAL_SYS_1, PHONEBOOK_MSISDN_STORAGE, &sim_contact);

        if(sim_contact.number_len > 0)
        {
            MMIAPICOM_GenDispNumber(MMIPB_GetNumberTypeFromUint8(sim_contact.ton),
                sim_contact.number_len, sim_contact.number, s_sfr_com_buff, 127);
            SCI_TraceLow("[Self Reg] PHONEBOOK_ReadPhonebookEntryEx sim 1 number = %s", s_sfr_com_buff);

            return s_sfr_com_buff;
        }
    }
    return "";
}

const char *sfr_dm_get_sim2_msisdn(void)
{
#if 0
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
        PHONEBOOK_ENTRY_T sim_contact = {0};

        sim_contact.entry_id = 1;

        PHONEBOOK_ReadPhonebookEntryEx(MN_DUAL_SYS_2, PHONEBOOK_MSISDN_STORAGE, &sim_contact);

        if(sim_contact.number_len > 0)
        {
            MMIAPICOM_GenDispNumber(MMIPB_GetNumberTypeFromUint8(sim_contact.ton),
                sim_contact.number_len, sim_contact.number, s_sfr_com_buff, 127);

            SCI_TraceLow("[Self Reg] PHONEBOOK_ReadPhonebookEntryEx sim 2 number = %s", s_sfr_com_buff);
            return s_sfr_com_buff;
        }
    }
    return "";
#endif
}
const char *sfr_dm_get_os_name(void)
{
    return "MOCOR";
}

const char *sfr_dm_get_os_version(void)
{
#ifndef WIN32
    char *temp_ptr = NULL;

    temp_ptr = strchr(VERSION_GetInfo(PLATFORM_VERSION), ':') + 1;

    if (temp_ptr != NULL)
    {
        strncpy(s_sfr_com_buff, temp_ptr, 32);
        return s_sfr_com_buff;
    }
#endif
    return "MOCOR_20A";
}

const char *sfr_dm_get_imei1(void)
{
    int i;
    char imei_str[18]={0};
    MN_IMEI_T tmp_imei = {0};

    if (MNNV_GetIMEIEx(MN_DUAL_SYS_1, tmp_imei))
    {
        for (i = 0; i < 8; i++)
        {
            imei_str[2 * i] = (tmp_imei[i]) & 0x0F;
            imei_str[2 * i + 1] = tmp_imei[i] >> 4;
        }
    }
    sprintf(s_sfr_com_buff,
            "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
            imei_str[1], imei_str[2], imei_str[3], imei_str[4],
            imei_str[5], imei_str[6], imei_str[7], imei_str[8],
            imei_str[9], imei_str[10], imei_str[11], imei_str[12],
            imei_str[13], imei_str[14], imei_str[15]);

    return s_sfr_com_buff;
}

const char *sfr_dm_get_imei2(void)
{
#if 0
#if defined MULTI_SIM_SYS_SINGLE
    return "";
#else
    int i;
    char imei_str[18] = {0};
    MN_IMEI_T tmp_imei = {0};

    if (MNNV_GetIMEIEx(MN_DUAL_SYS_2, tmp_imei))
    {
        for (i = 0; i < 8; i++)
        {
            imei_str[2 * i] = (tmp_imei[i]) & 0x0F;
            imei_str[2 * i + 1] = tmp_imei[i] >> 4;
        }
    }
    sprintf(s_sfr_com_buff,
            "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
            imei_str[1], imei_str[2], imei_str[3], imei_str[4],
            imei_str[5], imei_str[6], imei_str[7], imei_str[8],
            imei_str[9], imei_str[10], imei_str[11], imei_str[12],
            imei_str[13], imei_str[14], imei_str[15]);

    return s_sfr_com_buff;
#endif
#endif
}

const char *sfr_dm_get_sim1_cell_id(void)
{
    //return "123456789";
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
    {
        MSG_CAMP_STATUS_T plmn_info = {0};
        //MNPHONE_GetPlmnInfoEx(MN_DUAL_SYS_1, &plmn_info);
        MMIAPIPHONE_GetCurPLMN(&plmn_info,MN_DUAL_SYS_1);
        if (plmn_info.cell_id != 0)
        {
            sprintf(s_sfr_com_buff, "%d", plmn_info.cell_id);
            return s_sfr_com_buff;
        }
    }
    return "";
}

const char *sfr_dm_get_sim2_cell_id(void)
{
#if 0
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
        MSG_CAMP_STATUS_T plmn_info = {0};
        MMIAPIPHONE_GetCurPLMN(MN_DUAL_SYS_2, &plmn_info);
        if (plmn_info.cell_id != 0)
        {
            sprintf(s_sfr_com_buff, "%d", plmn_info.cell_id);
            return s_sfr_com_buff;
        }
    }
    return "";
#endif
}

const char *sfr_dm_get_sim1_lac(void)
{
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
    {
        MSG_CAMP_STATUS_T plmn_info = {0};
        // TODO:接口使用错误
        //MMIAPIPHONE_GetCurPLMN(MN_DUAL_SYS_1, &plmn_info);
        sprintf(s_sfr_com_buff, "%d", plmn_info.lac);
        return s_sfr_com_buff;
    }
    return "";
}

const char *sfr_dm_get_sim2_lac(void)
{
#if 0
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
        MSG_CAMP_STATUS_T plmn_info = {0};
        MMIAPIPHONE_GetCurPLMN(MN_DUAL_SYS_2, &plmn_info);
        sprintf(s_sfr_com_buff, "%d", plmn_info.lac);
        return s_sfr_com_buff;
    }
    return "";
#endif
}

const char *sfr_dm_get_data_sim(void)
{
#if 0
    MN_DUAL_SYS_E _dusim = MN_DUAL_SYS_1;

    MMIAPIPHONE_GetDataServiceSIM(&_dusim);

    return (_dusim == MN_DUAL_SYS_2 ? "2" : "1");
#endif
}

const char *sfr_dm_get_rom_size(void)
{
    return "128M";
}

const char *sfr_dm_get_ram_size(void)
{
    return "48M";
}

const char *sfr_dm_get_free_size(void)
{
    return "10M";
}

static BOOLEAN get_volte_sw(MN_DUAL_SYS_E dual_sys)
{
    uint8 volte_info = MMIAPIPHONE_GetVolteInfo();

    if(volte_info & (1 << dual_sys))
        return TRUE;

    return FALSE;
}

const char *sfr_dm_get_sim1_volte_sw(void)
{
    if (!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
    {
       if (!get_volte_sw(MN_DUAL_SYS_1))
       {
#ifdef SFR_SUPPORT_CUCC
           return "0";  // 0:off
 #else
           return "2";  // 2:off
 #endif
       }
       return "1";  // 1: on
    }
    return ""; // no sim
}

const char *sfr_dm_get_sim2_volte_sw(void)
{
#if 0
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
       if (!get_volte_sw(MN_DUAL_SYS_2))
       {
#ifdef SFR_SUPPORT_CUCC
           return "0";  // 0:off
 #else
           return "2";  // 2:off
 #endif
       }
       return "1";  // 1: on
    }
    return ""; //  no sim
#endif
}

const char *sfr_dm_get_accsee_type(void)
{
#ifdef SFR_SUPPORT_CUCC
    return "Mobile";
#else
    return "1"; //1:GPRS 2:Wifi
#endif
}

const char *sfr_dm_get_spec_version(void)
{
    return "V3.0";
}
const char *sfr_dm_get_time_total_mseconds(void)
{
    uint32 sec = 315504000; // secs form 1970-1-1 8:00:00 to 1980-1-1 00:00:00

    // sec for 1980-01-01
    sec += MMIAPICOM_GetCurTime();

    sprintf(s_sfr_com_buff, "%d000", sec);

    return s_sfr_com_buff;
}

const char *sfr_dm_get_time(void)
{
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};

    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);

    sprintf(s_sfr_com_buff, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", sys_date.year,sys_date.mon,sys_date.mday,sys_time.hour,sys_time.min, sys_time.sec);

    return s_sfr_com_buff;
}

