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
#include "JSON_parser.h"

#include "sfr_carrier_manager.h"
#include "sfr_json_maker.h"
#include "sfr_dm_interface.h"

typedef struct _SFR_CTCC_CONTEXT
{
    int flag;
    char *package_buff;
} SFR_CTCC_CONTEXT;

#define CTCC_FLAG (0xc7ccf1a9)

#define CTCC_CHECK(_ctx)   (_ctx != NULL && ((SFR_CTCC_CONTEXT *)_ctx)->flag==CTCC_FLAG)

/*
{
    "REGVER":"3.0",
    "MEID":"A1860160160011",
    "MODEL":"HS-EG981",
    "SWVER":"E943.6.01.07.00",
    "SIM1CDMAIMSI":"460036180704398",
    "UETYPE":"1",
    "SIM1ICCID":"89860312805320133131",
    "SIM1LTEIMSI":"460111234567890",
    "SIM2CDMAIMSI":"",
    "SIM2ICCID":"",
    "SIM2LTEIMSI":"",
    "MACID":"24:e2:71:f4:d7:b0"
}
*/

#define REGVER_KEY "REGVER"
#define MEID_KEY "MEID"
#define MODEL_KEY "MODEL"
#define SWVER_KEY "SWVER"
#define SIM1CDMAIMSI_KEY "SIM1CDMAIMSI"
#define UETYPE_KEY "UETYPE"
#define SIM1ICCID_KEY "SIM1ICCID"
#define SIM1LTEIMSI_KEY "SIM1LTEIMSI"
#define SIM2CDMAIMSI_KEY "SIM2CDMAIMSI"
#define SIM2ICCID_KEY "SIM2ICCID"
#define SIM2LTEIMSI_KEY "SIM2LTEIMSI"
#define MACID_KEY "MACID"
#define OSVER_KEY "OSVER"
#define IMEI1_KEY "IMEI1"
#define IMEI2_KEY "IMEI2"
#define SIM1CELLID_KEY "SIM1CELLID"
#define SIM2CELLID_KEY "SIM2CELLID"
#define DATASIM_KEY "DATASIM"
#define ROM_KEY "ROM"
#define RAM_KEY "RAM"
#define SIM1VOLTESW_KEY "SIM1VoLTESW"
#define SIM2VOLTESW_KEY "SIM2VoLTESW"
#define ACCESSTYPE_KEY "ACCESSTYPE"
#define REGDATE_KEY "REGDATE"
#define MLPLVER_KEY "MLPLVER"
#define MSPLVER_KEY "MSPLVER"
#define MMEID_KEY "MMEID"

const char *getCTCCRegVersion(void)
{
    return "8.0";
}

static char s_date_buff[32];

const char *getCTCCRegDate(void)
{
    SCI_DATE_T _date = {31, 12, 2004, 6};
    SCI_TIME_T _time = {59, 59, 23};

    TM_GetSysDate(&_date);
    TM_GetSysTime(&_time);
    SCI_MEMSET(s_date_buff, 0, 32);
#ifndef WIN32
    snprintf(s_date_buff, 32, "%4d-%02d-%02d %02d:%02d:%02d",
        _date.year, _date.mon, _date.mday,
        _time.hour, _time.min, _time.sec);
#endif
    return s_date_buff;
}

const char *getMLPLVer(void)
{
    return NULL;
}

const char *getMSPLVer(void)
{
    return NULL;
}

const char *getMMEID(void)
{
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////

static void *ctcc_constructor(void)
{
    SFR_CTCC_CONTEXT *_ctx = NULL;
    uint8 i_sim = 0;
    BOOLEAN has_ctcc_sim = FALSE;
    BOOLEAN has_changed_iccid = FALSE;

    for(i_sim = 0; i_sim < MMI_DUAL_SYS_MAX; i_sim++)
    {
        if (MMIAPIPHONE_GetSimExistedStatus(i_sim)
			&& SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(i_sim)
			&& MMISFR_IsChinaTelecom(i_sim))
        {
            has_ctcc_sim = TRUE;
        }
        if(MMISFR_IsICCIDChanged(i_sim))
        {
            has_changed_iccid = TRUE;
        }
    }
    if (has_ctcc_sim && has_changed_iccid)
    {
            _ctx = SCI_ALLOCAZ(sizeof(SFR_CTCC_CONTEXT));
         if (_ctx != NULL)
         {
             _ctx->flag = CTCC_FLAG;
             _ctx->package_buff = NULL;
         }
            return (void *)_ctx;
    }

    return NULL;
}

static void ctcc_saver(void *ctx)
{
    uint8 i_sim = 0;

    if (!CTCC_CHECK(ctx)) return;

    for(i_sim = 0; i_sim < MMI_DUAL_SYS_MAX; i_sim++)
    {
        MMISFR_SaveICCID(i_sim);
    }
}

static int ctcc_get_loop(void)
{
    return (60 * 24 * 30);
}

static void ctcc_clean(void *ctx)
{
    if (CTCC_CHECK(ctx))
    {
        SFR_CTCC_CONTEXT *_ctx = (SFR_CTCC_CONTEXT *)ctx;
        if (_ctx->package_buff != NULL)
        {
            SCI_FREE(_ctx->package_buff);
        }
    }
}

static char *ctcc_get_url(void *ctx, int *len)
{
    if (CTCC_CHECK(ctx))
    {
        *len = -1;
        return "http://zzhc.vnet.cn/";
    }
}

static char *ctcc_get_accept(void *ctx, int *len)
{
    if (CTCC_CHECK(ctx))
    {
        *len = -1;
        return "text/html, application/json, */*";
    }
}

static char *ctcc_get_agent(void *ctx, int *len)
{
    if (CTCC_CHECK(ctx))
    {
        *len = -1;
        return MMIAPICOM_GetUserAgent();
        //return "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)";
    }
}

static char *ctcc_get_bodytype(void *ctx, int *len)
{
    if (CTCC_CHECK(ctx))
    {
        *len = -1;
        return "application/encrypted-json";
    }
}

static char *ctcc_get_package(void *ctx, int *len)
{
    if (CTCC_CHECK(ctx))
    {
        int _base64_max;
	 char *p_json;
	 int p_json_len;
	 int json_base64_len;
        SFR_CTCC_CONTEXT *_ctx = (SFR_CTCC_CONTEXT *)ctx;

        sfr_json_buff_init();

        sfr_add_json_char(JSON_START);
        // - RegVer
        if (sfr_add_json_item(REGVER_KEY, getCTCCRegVersion()))
            sfr_add_json_char(JSON_SPLIT);
        // - MEID
        if (sfr_add_json_item(MEID_KEY, sfr_dm_get_meid()))
            sfr_add_json_char(JSON_SPLIT);
        // - MODEL
        if (sfr_add_json_item(MODEL_KEY, sfr_dm_get_model()))
            sfr_add_json_char(JSON_SPLIT);
        // - SWVER
        if (sfr_add_json_item(SWVER_KEY, sfr_dm_get_sw_version()))
            sfr_add_json_char(JSON_SPLIT);
        // - SIM1CDMAIMSI
        if (sfr_add_json_item(SIM1CDMAIMSI_KEY, sfr_dm_get_sim1_cdma_imsi()))
            sfr_add_json_char(JSON_SPLIT);
        // - UETYPE
        if (sfr_add_json_item(UETYPE_KEY, sfr_dm_get_ue_type()))
            sfr_add_json_char(JSON_SPLIT);
        // - SIM1ICCID
        if (sfr_add_json_item(SIM1ICCID_KEY, sfr_dm_get_sim1_iccid()))
            sfr_add_json_char(JSON_SPLIT);
        // - SIM1LTEIMSI
        if (sfr_add_json_item(SIM1LTEIMSI_KEY, sfr_dm_get_sim1_lte_imsi()))
            sfr_add_json_char(JSON_SPLIT);
        // - SIM2CDMAIMSI
        if (sfr_add_json_item(SIM2CDMAIMSI_KEY, sfr_dm_get_sim2_cdma_imsi()))
            sfr_add_json_char(JSON_SPLIT);
        // - SIM12CCID
        if (sfr_add_json_item(SIM2ICCID_KEY, sfr_dm_get_sim2_iccid()))
            sfr_add_json_char(JSON_SPLIT);
        // - SIM2LTEIMSI
        if (sfr_add_json_item(SIM2LTEIMSI_KEY, sfr_dm_get_sim2_lte_imsi()))
            sfr_add_json_char(JSON_SPLIT);
        // - MACID
        if (sfr_add_json_item(MACID_KEY, sfr_dm_get_mac_id()))
            sfr_add_json_char(JSON_SPLIT);
        // - OSVER
        if (sfr_add_json_item(OSVER_KEY, sfr_dm_get_os_version()))
            sfr_add_json_char(JSON_SPLIT);
        // - IMEI1
        if (sfr_add_json_item(IMEI1_KEY, sfr_dm_get_imei1()))
            sfr_add_json_char(JSON_SPLIT);
        // - IMEI2
        if (sfr_add_json_item(IMEI2_KEY, sfr_dm_get_imei2()))
            sfr_add_json_char(JSON_SPLIT);
        // - SIM1CELLID
        if (sfr_add_json_item(SIM1CELLID_KEY, sfr_dm_get_sim1_cell_id()))
            sfr_add_json_char(JSON_SPLIT);
        // - SIM2CELLID
        if (sfr_add_json_item(SIM2CELLID_KEY, sfr_dm_get_sim2_cell_id()))
            sfr_add_json_char(JSON_SPLIT);
        // - DATASIM
        if (sfr_add_json_item(DATASIM_KEY, sfr_dm_get_data_sim()))
            sfr_add_json_char(JSON_SPLIT);
        // - ROM
        if (sfr_add_json_item(ROM_KEY, sfr_dm_get_rom_size()))
            sfr_add_json_char(JSON_SPLIT);
        // - RAM
        if (sfr_add_json_item(RAM_KEY, sfr_dm_get_ram_size()))
            sfr_add_json_char(JSON_SPLIT);
        // - SIM1VOLTESW
        if (sfr_add_json_item(SIM1VOLTESW_KEY, sfr_dm_get_sim1_volte_sw()))
            sfr_add_json_char(JSON_SPLIT);
        // - SIM2VOLTESW
        if (sfr_add_json_item(SIM2VOLTESW_KEY, sfr_dm_get_sim2_volte_sw()))
            sfr_add_json_char(JSON_SPLIT);
        // - ACCESSTYPE
        if (sfr_add_json_item(ACCESSTYPE_KEY, sfr_dm_get_accsee_type()))
            sfr_add_json_char(JSON_SPLIT);
        // - REGDATE
        if (sfr_add_json_item(REGDATE_KEY, getCTCCRegDate()))
            sfr_add_json_char(JSON_SPLIT);
        // - MLPLVER
        if (sfr_add_json_item(MLPLVER_KEY, getMLPLVer()))
            sfr_add_json_char(JSON_SPLIT);
        // - MSPLVER
        if(sfr_add_json_item(MSPLVER_KEY, getMSPLVer()))
            sfr_add_json_char(JSON_SPLIT);
        // - MMEID
        if(sfr_add_json_item(MMEID_KEY, getMMEID()))
            sfr_add_json_char(JSON_SPLIT);

        sfr_remove_last_split();
        sfr_add_json_char(JSON_END);

        p_json = sfr_get_json_string();
        p_json_len = strlen(p_json);

        SCI_TraceLow("[Self Reg] CTCC ::  json package:: (%d)", p_json_len);
        SFR_LongTrace(p_json, p_json_len);

        _base64_max = (p_json_len / 3 + 2) * 4;
        _ctx->package_buff = SCI_ALLOC_APPZ(_base64_max);
#ifndef WIN32
        if (_ctx->package_buff != NULL)
        {
            base64_encode(_ctx->package_buff, _base64_max, p_json, p_json_len);
        }
#endif
        sfr_json_buff_term();
        *len = json_base64_len;

        return _ctx->package_buff;
    }
}

typedef enum
{
    JSON_TAG_NONE = 0,
    JSON_TAG_RET_CODE,
    JSON_TAG_RET_DESC,
    JSON_TAG_MAX
} SFR_JSON_TAG_E;

typedef struct
{
    SFR_JSON_TAG_E cur_tag;
    int retCode;
} SFR_JSON_PARSR_CTX;

LOCAL const char* const s_json_tags_str[] =
{
    "",
    "resultCode",
    "resultDesc",
};

#define CTCC_RETURN_CODE_OK  (0)

LOCAL int _Json_Parser_CB(void* ctx, int type, const JSON_value* val)
{
    int res;

    SFR_JSON_PARSR_CTX *parseCtx = (SFR_JSON_PARSR_CTX *)ctx;
    switch(type)
    {
        case JSON_T_INTEGER:
            if (parseCtx->cur_tag == JSON_TAG_RET_CODE)
            {
                parseCtx->retCode = (*val).vu.integer_value;
                SCI_TRACE_LOW("[Self Reg] _Json_Parser_CB [int ret code : %d]", parseCtx->retCode);
            }
	     res = 1;
            break;
        case JSON_T_STRING:         // 10
            if (parseCtx->cur_tag == JSON_TAG_RET_CODE)
            {
                parseCtx->retCode = atoi((((*val).vu).str).value);
                SCI_TRACE_LOW("[Self Reg] _Json_Parser_CB [str ret code : %d]", parseCtx->retCode);
            }
	     res = 1;
            break;
        case JSON_T_KEY:            // 11
        {
            SFR_JSON_TAG_E j_tag = JSON_TAG_MAX;
            while(--j_tag)
            {
                if(strcmp((((*val).vu).str).value, s_json_tags_str[j_tag]) == 0)
                {
                    parseCtx->cur_tag = j_tag;
                    SCI_TRACE_LOW("[Self Reg] _Json_Parser_CB [%s]", s_json_tags_str[j_tag]);
                    break;
                }
            }
	     res = 1;
            break;
        }
        default:
            res = -1;
            break;
    }
    return res;
}

BOOLEAN ctcc_return_parser (void *ctx, char *string, int len)
{
    //return TRUE -- self register return ok
    //return FALSE -- self register return error
    int offset;
    JSON_config js_cfg = {0};
    JSON_parser jp_ptr = PNULL;
    SFR_JSON_PARSR_CTX _ctx;

    init_JSON_config(&js_cfg);

    js_cfg.callback = _Json_Parser_CB;
    js_cfg.allow_comments = TRUE;
    js_cfg.handle_floats_manually = TRUE;
    js_cfg.callback_ctx = &_ctx;

    jp_ptr = new_JSON_parser(&js_cfg);
    if(PNULL == jp_ptr)
    {
        SCI_TRACE_LOW("[Self Reg] ctcc_return_parser PNULL == jp_ptr");
        return FALSE;
    }

    for(offset = 0; offset < len; offset++)
    {
        int c = (*(string + offset)) & 0x000000ff;

        if(0 == JSON_parser_char(jp_ptr, c))
        {
            SCI_TRACE_LOW("[Self Reg] ctcc_return_parser parse error!");
            break;
        }
    }
    delete_JSON_parser(jp_ptr);

    return (CTCC_RETURN_CODE_OK ==_ctx.retCode ? TRUE : FALSE);
}

SFR_CARRIER sfr_carrier_ctcc =
{
    "CTCC",
    ctcc_get_loop,
    NULL,
    ctcc_constructor,
    ctcc_saver,
    ctcc_clean,
    ctcc_get_url,
    ctcc_get_accept,
    ctcc_get_agent,
    ctcc_get_bodytype,
    ctcc_get_package,
    ctcc_return_parser
};
