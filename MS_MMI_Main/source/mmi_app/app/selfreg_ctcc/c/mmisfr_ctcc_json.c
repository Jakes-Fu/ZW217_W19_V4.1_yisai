/*****************************************************************************
** File Name:      mmisfr_ctcc_json.c
** Author:
** Copyright (C) 2000-2100, UNISOC Technoloies Co.,Ltd.
** Description:
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                         NAME                        DESCRIPTION
** 2020/11/24             mark.zhang1            Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "os_api.h"
#include "mmiphone_export.h"
#include "mmisfr_ctcc_json.h"
#include "mmisfr_ctcc_main.h"
#include "version.h"
#include "mmiutil.h"
#include "JSON_parser.h"
#include "dal_time.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MAX_TRACE_SIZE  128 // MAX_STR_SIZE

#define DEF_BUFFER_SIZE 512 // bytes

#define PARSE_DATA_SUCCESS (0)

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
/* Converts a given string into a base64 encoded buffer */
extern PUBLIC int32 CFLBASE64_Encode(
    unsigned char
    *out_ptr,     // [OUT] pointer to a char to hold the converted string
    int32 out_len,              // [IN]  buffer length, out_len=in_len*4/3 + 4 ( suggest! )
    const unsigned char *in_ptr,// [IN]  String to convert
    int32 in_len                // [IN]  Length of the string to be converted
);

/**-------------------------------------------------------------------------*
**                         STATIC DEFINITION                       *
**--------------------------------------------------------------------------*/
LOCAL char s_ue_info[128];

LOCAL char *s_buff_ptr;
LOCAL int s_buff_size;

LOCAL const char *s_json_tags_str[] =
{
    "",
    "resultCode",
};

char s_dm_str[MMISFR_INDEX_MAX][MMISFR_STR_LEN + 2] ={
    "RTOS",//MMISFR_INDEX_SYS_VERSION
    "1.0.0.1",//MMISFR_INDEX_SOFTWARE_VER
    "TEST",//MMISFR_INDEX_SOFTWARE_NAME
    "11",//MMISFR_INDEX_UETYPE
    "",//MMISFR_INDEX_EID
};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN sfr_init_buff(void)
{
    s_buff_size = DEF_BUFFER_SIZE;
    s_buff_ptr = SCI_ALLOC_APPZ(s_buff_size);

    return (NULL == s_buff_ptr);
}

LOCAL void sfr_free_buff(void)
{
    SCI_Free(s_buff_ptr);
    s_buff_ptr = NULL;
}

LOCAL BOOLEAN sfr_resize_buff(void)
{
    char *json_buff_new;

    s_buff_size <<= 1;
    json_buff_new = SCI_ALLOC_APPZ(s_buff_size);
    if (NULL == json_buff_new)
    {
        SCI_TraceLow("[DMCTCC] sfr_resize_buff failed!");
        return FALSE;
    }

    SCI_MEMCPY(json_buff_new, s_buff_ptr, strlen(s_buff_ptr));
    SCI_FREE(s_buff_ptr);
    s_buff_ptr = json_buff_new;
    SCI_TraceLow("[DMCTCC] sfr_resize_buff [%d]", s_buff_size);

    return TRUE;
}

LOCAL BOOLEAN sfr_add_item(const char *key, const char *value)
{
    char new_data[128] = {0};
    uint16  json_buffer_len = strlen(s_buff_ptr);
    BOOLEAN resize_flag = TRUE;


    if (NULL == key || NULL == value)
    {
        return FALSE;
    }

    if (json_buffer_len > 0) //added data
    {
        sprintf(new_data, ",\"%s\":\"%s\"", key, value);
    }
    else
    {
        sprintf(new_data, "{\"%s\":\"%s\"", key, value);
    }

    if ((json_buffer_len + strlen(new_data)) >= s_buff_size)
    {
        resize_flag = sfr_resize_buff();
    }

    if (resize_flag)
    {
        strcat(s_buff_ptr, new_data);
    }

    return TRUE;
}

LOCAL void sfr_add_char(char ch)
{
    uint16 string_len = strlen(s_buff_ptr);
    BOOLEAN resize_flag = TRUE;

    if (string_len + 1  >= s_buff_size)
    {
        resize_flag = sfr_resize_buff();
    }

    if (resize_flag)
    {
        s_buff_ptr[string_len] = ch;
    }
}

//data info begin
LOCAL const char *sfr_get_json_string(void)
{
    return s_buff_ptr;
}

LOCAL const char *sfr_get_meid(void)
{
    return "";
}

LOCAL const char *sfr_get_model(void)
{
    return s_dm_str[MMISFR_INDEX_SOFTWARE_NAME];
}

LOCAL const char *sfr_get_sw_version(void)
{
#ifdef WIN32
	//return "1.0.0";
#else
    return s_dm_str[MMISFR_INDEX_SOFTWARE_VER];
#endif
    //return "1.0.0";
}

LOCAL const char *sfr_get_sim1_iccid(void)
{
    return sfr_get_iccid(MN_DUAL_SYS_1);
}

LOCAL const char *sfr_get_sim1_lte_imsi(void)
{
    MN_IMSI_T tmp_imsi = MNSIM_GetImsiEx(MN_DUAL_SYS_1);
    SCI_MEMSET(s_ue_info, 0, sizeof(s_ue_info));
    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)tmp_imsi.imsi_val,
                       MN_MAX_IMSI_ARR_LEN << 1, s_ue_info);

    return s_ue_info + 1; // +1 is skip the "9"
}

LOCAL const char *sfr_get_mac_id(void)
{
#ifdef MMI_WIFI_SUPPORT
    static uint8  addr_str[20] = {0};
    char wifi_addr[6] = {0};  //ETH_ALEN
#ifndef WIN32
    wifi_get_mac(wifi_addr);
#endif
    sprintf((char *)addr_str, "%02x:%02x:%02x:%02x:%02x:%02x", wifi_addr[0],
            wifi_addr[1], wifi_addr[2], wifi_addr[3], wifi_addr[4], wifi_addr[5]);
    SCI_TRACE_LOW("[DMCTCC] sfr_get_mac_id=%s", addr_str);

    return addr_str;
#else
    return "";
#endif
}

LOCAL const char *sfr_get_os_version(void)
{
    return s_dm_str[MMISFR_INDEX_SYS_VERSION];
}

LOCAL const char *sfr_get_imei1(void)
{
    MN_IMEISVN_T imeisvn = {0};

    if (MNNV_GetIMEISVNEx(0, &imeisvn))
    {
        char ori_imei[(MN_MAX_IMEI_LENGTH << 1) + 1] = {0};
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)imeisvn.imei,
                           MN_MAX_IMEI_LENGTH << 1, ori_imei);
        SCI_MEMSET(s_ue_info, 0, sizeof(s_ue_info));
        SCI_MEMCPY(s_ue_info, &ori_imei[1], strlen(ori_imei) - 1);//first is '*'
    }

    return s_ue_info;
}

LOCAL const char *sfr_get_sim1_cell_id(void)
{
    MN_PHONE_CURRENT_PLMN_INFO_T plmn_info = {0};

    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(MN_DUAL_SYS_1);
    SCI_TRACE_LOW("[DMCTCC] sfr_get_sim1_cell_id,  plmn_info.cell_id =%d",
                  plmn_info.cell_id);
    SCI_MEMSET(s_ue_info, 0, sizeof(s_ue_info));
    sprintf(s_ue_info, "%d", plmn_info.cell_id);

    return s_ue_info;
}

LOCAL const char *sfr_get_time(void)
{
    SCI_DATE_T _date = {0};
    SCI_TIME_T _time = {0};

    TM_GetSysDate(&_date);
    TM_GetSysTime(&_time);
    SCI_MEMSET(s_ue_info, 0, sizeof(s_ue_info));
    sprintf(s_ue_info, "%4d-%02d-%02d %02d:%02d:%02d",
            _date.year, _date.mon, _date.mday,
            _time.hour, _time.min, _time.sec);

    return s_ue_info;
}
//data info end

LOCAL void sfr_long_trace(const char *string, uint16 len)
{
    if (len < MAX_TRACE_SIZE)
    {
        SCI_TraceLow("[DMCTCC] %s ", string);
    }
    else
    {
        char trace_buff[MAX_TRACE_SIZE] = {0};
        uint8 step = MAX_TRACE_SIZE - 1;
        char *p_temp = string ;

        while (len >= step)
        {
            SCI_MEMCPY(trace_buff, p_temp, step);
            SCI_TraceLow("[DMCTCC] %s ", trace_buff);

            p_temp += step;
            len -= step;
        }
        if (len > 0)
        {
            SCI_MEMSET(trace_buff, 0, MAX_TRACE_SIZE);
            SCI_TraceLow("[DMCTCC] %s ", p_temp);
        }
    }
}

LOCAL int sfr_json_parser_callback(void *ctx, int type, const JSON_value *val)
{
    int parse_flag = 1; // continue
    SFR_JSON_PARSR_CTX *parseCtx = (SFR_JSON_PARSR_CTX *)ctx;

    switch (type)
    {
        case JSON_T_INTEGER:
            if (SFR_JSON_TAG_RET_CODE == parseCtx->cur_tag)
            {
                parseCtx->retCode = (*val).vu.integer_value;
                SCI_TRACE_LOW("[DMCTCC] JSON_T_INTEGER, parseCtx->retCode:%d",
                              parseCtx->retCode);
                parse_flag = 0; // END
            }
            break;

        case JSON_T_STRING:
            if (SFR_JSON_TAG_RET_CODE == parseCtx->cur_tag)
            {
                parseCtx->retCode = atoi((((*val).vu).str).value);
                SCI_TRACE_LOW("[DMCTCC] JSON_T_STRING,  parseCtx->retCode:%d",
                              parseCtx->retCode);
                parse_flag = 0; // END
            }
            break;

        case JSON_T_KEY:
        {
            SFR_JSON_TAG_E tag_index = SFR_JSON_TAG_MAX;
            while (--tag_index)
            {
                if (strcmp((((*val).vu).str).value, s_json_tags_str[tag_index]) == 0)
                {
                    parseCtx->cur_tag = tag_index;
                    SCI_TRACE_LOW("[DMCTCC] JSON_T_KEY,  str:%s", s_json_tags_str[tag_index]);
                    break;
                }
            }
            break;
        }

        default:
            break;
    }

    return parse_flag;
}

PUBLIC BOOLEAN sfr_parse_data(uint8 *data, uint32 data_len)
{
    int offset;
    JSON_config js_cfg = {0};
    JSON_parser jp_ptr = PNULL;
    SFR_JSON_PARSR_CTX parse_ctx = {0};

    init_JSON_config(&js_cfg);

    js_cfg.callback = sfr_json_parser_callback;
    js_cfg.allow_comments = TRUE;
    js_cfg.handle_floats_manually = TRUE;
    js_cfg.callback_ctx = &parse_ctx;
    jp_ptr = new_JSON_parser(&js_cfg);
    if (PNULL == jp_ptr)
    {
        SCI_TRACE_LOW("[DMCTCC] sfr_parse_data, PNULL == jp_ptr");
        return FALSE;
    }

    for (offset = 0; offset < data_len; offset++)
    {
        int c = (*(data + offset)) & 0x000000ff;
        if (0 == JSON_parser_char(jp_ptr, c))
        {
            SCI_TRACE_LOW("[DMCTCC] sfr_parse_data, parse END!");
            break;
        }
    }
    delete_JSON_parser(jp_ptr);

    return (PARSE_DATA_SUCCESS == parse_ctx.retCode);
}

PUBLIC char *sfr_get_post_data(uint32 *p_dst_data_len)
{
    uint16 base64_max_len;
    char *p_json;
    uint16 src_data_len;
    uint32 dst_data_len;
	unsigned char *be_encoded_buff = NULL;

    SCI_TraceLow("[DMCTCC] sfr_get_post_data, come in");
    if (sfr_init_buff())
    {
        return NULL;
    }

    // - RegVer
    sfr_add_item("REGVER", "12.0");

    // - MODEL
    sfr_add_item("MODEL", sfr_get_model());

    // - SWVER
    sfr_add_item("SWVER", sfr_get_sw_version());

    // - UETYPE
    sfr_add_item("UETYPE", s_dm_str[MMISFR_INDEX_UETYPE]);

    // - SIM1ICCID
    sfr_add_item("SIM1ICCID", sfr_get_sim1_iccid());

    // - SIM1LTEIMSI
    sfr_add_item("SIM1LTEIMSI", sfr_get_sim1_lte_imsi());

    // - MACID
    sfr_add_item("MACID", sfr_get_mac_id());

    // - OSVER
    sfr_add_item("OSVER", sfr_get_os_version());

    // - IMEI1
    sfr_add_item("IMEI1", sfr_get_imei1());

    // - SIM1CELLID
    sfr_add_item("SIM1CELLID", sfr_get_sim1_cell_id());

    // - MEID
    sfr_add_item("MEID", "");

    // - NETWORKTYPE
    sfr_add_item("NETWORKTYPE", "LTE");

    // - SIM1CDMAIMSI
    sfr_add_item("SIM1CDMAIMSI", "");

    // - REGDATE
    sfr_add_item("REGDATE", sfr_get_time());

    // - EID
    sfr_add_item("EID", s_dm_str[MMISFR_INDEX_EID]);

    // - END
    sfr_add_char('}');

    p_json = sfr_get_json_string();
    src_data_len = strlen(p_json);
    SCI_TraceLow("[DMCTCC] sfr_get_post_data, src_data_len (%d)", src_data_len);
    sfr_long_trace(p_json, src_data_len);

    base64_max_len = (src_data_len / 3 + 2) * 4;
    be_encoded_buff = (unsigned char *)SCI_ALLOC_APPZ(base64_max_len);
    if (be_encoded_buff != NULL)
    {
        CFLBASE64_Encode(be_encoded_buff, base64_max_len,  p_json, src_data_len);
        dst_data_len = strlen(be_encoded_buff);
        *p_dst_data_len = dst_data_len;
    }

    sfr_free_buff();

    return be_encoded_buff;
}

/*****************************************************************************/
//  Description :set SYS_VERSION,SOFTWARE_VER,SOFTWARE_NAME,UETYPEµÈÐÅÏ¢
//  Param :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN sfr_set_str(MMISFR_INDEX_E str_index, const char str[MMISFR_STR_LEN + 1])
{
    uint8 in_str_len = 0;
    if(str_index >= MMISFR_INDEX_MAX )
    {
        SCI_TraceLow("[DMCTCC] sfr_set_str not support : %d  max is %d", str_index, MMISFR_INDEX_MAX - 1);
        return FALSE;
    }
    in_str_len = strlen(str);
    SCI_TraceLow("[DMCTCC] sfr_set_str index: %d, str: %s , str len %d", str_index, str, in_str_len);

    SCI_MEMSET(s_dm_str[str_index], 0, MMISFR_STR_LEN + 2);
    SCI_MEMCPY( s_dm_str[str_index], str, in_str_len);
    return TRUE;
}

