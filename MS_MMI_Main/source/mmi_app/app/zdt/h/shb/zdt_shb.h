#ifndef __ZDT_SHB_H__
#define __ZDT_SHB_H__
/*
和盈互联智能穿戴通讯协议2.0兼容GSM网.pdf
*/
#include "zdt_shb_api.h"
#include "zdt_shb_http.h"
#include "zdt_shb_func.h"
#include "zdt_shb_db.h"
#include "zdt_shb_sms.h"

#define SHB_SOFT_VERSION "AngelCare.SHB061.YUN.L01"

#define SHB_AES_KEY_STR "ShouhubaoAES2017"
#define SHB_AES_IV_STR "ShouhubaoAESIVCS"
#define SHB_MD5_SECKEY_STR "ShouhubaoCS2017"

typedef struct _ZDT_SHB_DATA_t
{
    uint16 type;
    void* data_p;
} ZDT_SHB_DATA_T;

typedef struct
{
    SIGNAL_VARS
    ZDT_SHB_DATA_T * p_data;
} ZDT_SHB_DATA_SIG_T;

typedef struct SHB_APP_T {
// Common
    BOOLEAN         m_shb_net_is_init;
    BOOLEAN         m_shb_net_is_open;
    BOOLEAN         m_shb_net_is_reg;
    BOOLEAN         m_shb_net_is_land;
    
    uint8                       m_shb_reg_timer_id;
    uint8                       m_shb_land_timer_id;
    uint8                       m_shb_heart_timer_id;
    uint8                       m_shb_find_timer_id;
    uint8                       m_shb_cr_timer_id;
    uint8                       m_shb_pos_timer_id;
    uint8                       m_shb_gj_timer_id;
}SHB_APP_T;

PUBLIC BOOLEAN  SHB_PostEvtTo_APP(ZDT_APP_SIG_E sig_id, void* p_data, uint16 type);

extern MMI_RESULT_E SHB_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param);
extern int SHB_AppInit(void);
extern int SHB_AppFree(void);
extern int SHB_RestorFactory(void);
extern BOOLEAN SHB_LocReport_Judge(void);
extern void SHB_LocReport_Reset(void);

extern uint8 MMIZDT_SHB_NV_GetAlert(void);
extern BOOLEAN  MMIZDT_SHB_NV_SetAlert(uint8 need_alert);
extern uint8 MMIZDT_SHB_GetAlert(void);
extern SHB_APP_T  g_shb_app;
extern uint8  g_shb_imei_len;
extern BOOLEAN g_is_shb_power_off_ing;
extern BOOLEAN g_shb_is_stillness;
extern BOOLEAN g_shb_is_move;
extern BOOLEAN g_shb_is_jp_change;
extern BOOLEAN g_shb_is_lbs_change;
extern BOOLEAN g_is_user_update_cfg;
#endif
