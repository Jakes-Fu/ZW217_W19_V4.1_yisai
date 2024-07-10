#ifndef __ZDT_YX_H__
#define __ZDT_YX_H__
/*
和盈互联智能穿戴通讯协议2.0兼容GSM网.pdf
*/
#include "zdt_yx_api.h"
#include "zdt_yx_net.h"
#include "zdt_yx_func.h"
#include "zdt_yx_captrue.h"
#include "zdt_yx_voice.h"
#include "zdt_yx_db.h"
#include "zdt_yx_sms.h"

#define YX_AES_KEY_STR "4ac3b4824198ee129f53a064479cdb49"
#define YX_AES_IV_STR "9540542e04f15414ab18d669a5456bed"
#define YX_MD5_SECKEY_STR "2e7357617f28d4d152cc5305eaf591d8"

#define YX_COMPANY_STR "BIHEE"
#define YX_PS_VERSION_STR "2.1"
#define YX_FW_VERSION_STR "W9.0.1"

typedef struct _ZDT_YX_DATA_t
{
    uint16 type;
    void* data_p;
} ZDT_YX_DATA_T;

typedef struct
{
    SIGNAL_VARS
    ZDT_YX_DATA_T * p_data;
} ZDT_YX_DATA_SIG_T;

typedef struct YX_APP_T {
    ZDT_TCP_INTERFACE_T * m_yx_tcp_reg_interface;
// Common
    BOOLEAN         m_yx_net_is_open;
    BOOLEAN         m_yx_net_is_init;
    BOOLEAN         m_yx_net_first_reg;
    BOOLEAN         m_yx_is_reg;
    BOOLEAN         m_yx_allow_rereg;
    BOOLEAN         m_yx_is_land;
    uint32           m_yx_lk_send_time;
    uint32           m_yx_reg_ip;
    uint8          m_yx_send_err_num;
    uint16         m_yx_reg_port;
    uint16         m_yx_rsp_err_num;
    BOOLEAN         m_yx_test_stop;
//End Common

    uint8                       m_yx_reg_timer_id;
    uint8                       m_yx_land_timer_id;
    uint8                       m_yx_heart_timer_id;
    uint8                       m_yx_find_timer_id;
    uint8                       m_yx_cr_timer_id;
    uint8                       m_yx_pos_timer_id;
    uint8                       m_yx_gj_timer_id;
#if defined(ZDT_GPS_SUPPORT) || defined(ZDT_WIFI_SUPPORT) 
    YX_LOCTION_LINK_NODE_T * m_yx_loction_link_head;
#endif
    YX_MDBC_LINK_NODE_T * m_yx_mdbc_link_head;

#if 1
    YX_CAP_SEND_NODE_T  *m_yx_cap_send_link_head;
    char		m_yx_pFileSendCapName[MAX_YX_CAP_FILENAME_SIZE+1]; 
    uint16		m_yx_pFileSendCapFullName[MAX_YX_CAP_FILENAME_SIZE+1];
    uint16		m_yx_pFileSendCapFullName_len;
    BOOLEAN           m_yx_cap_is_sending;
    int32               m_yx_cap_file_seek;
    uint8               m_yx_cap_pack_num;
    uint8                m_yx_cap_pack_idx;
    uint16               m_yx_cap_data_idx;
#endif
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
// Voc record
    YX_VOC_SEND_NODE_T  * m_yx_voc_send_link_head;
    YX_VOC_RCV_FILE_NODE_T  * m_yx_voc_rcvfile_link_head;
    uint16 m_yx_voc_send_times;
    BOOLEAN           m_yx_voc_is_sending;
    BOOLEAN           m_yx_voc_is_playing;
    int32               m_yx_voc_file_seek;
    uint8               m_yx_voc_pack_num;
    uint8                m_yx_voc_pack_idx;
    uint16               m_yx_voc_data_idx;
    MMISRV_HANDLE_T      m_yx_pRecordMedia;
    MMISRV_HANDLE_T      m_yx_pPlayMedia;
    char		m_yx_pFileSendVocName[MAX_YX_VOC_FILENAME_SIZE+1]; 
    char		m_yx_pFileRcvVocName[MAX_YX_VOC_FILENAME_SIZE+1]; 
    uint16		m_yx_pFileSendVocFullName[MAX_YX_VOC_FILENAME_SIZE+1];
    uint16		m_yx_pFileSendVocFullName_len;
    uint16		m_yx_pFileRcvVocFullName[MAX_YX_VOC_FILENAME_SIZE+1];
    uint16		m_yx_pFileRcvVocFullName_len;
    uint16       m_yx_cur_rcv_is_complete;
    YXRecorderStatus   m_yx_eRecorderStatus;
    YXFilePlayStatus    m_yx_eFilePlayrStatus;
    
    yx_db_vocrcv_index_type  *m_yx_db_vocrcv_list_head;/*链表头,未做初始化前为空*/
    uint32		    m_yx_db_vocrcv_list_length;

    yx_db_vocrcv_index_type 		m_yx_pDB_vocrcv_CurList;				/*当前结点*/
    yx_db_vocrcv_index_type 		m_yx_pDB_vocrcv_last_CurList;				/*最后一条结点*/

    uint8 m_yx_voc_play_time_id;
    uint8 m_yx_voc_record_time_id;
    uint8 m_yx_voc_record_stop_time_id;
    uint8 m_yx_voc_rcv_time_id;
// Voc record End
#endif
#ifdef ZDT_PLAT_YX_SUPPORT_FRIEND
    uint16          m_yx_friend_status;
    uint8           m_yx_friend_mark;
    uint8*         m_yx_friend_alert_str;
    uint16         m_yx_friend_alert_len;
#endif
}YX_APP_T;

#ifdef ZDT_PLAT_YX_SUPPORT_FRIEND
typedef enum  _YX_Friend_Disp_STAT
{
    YX_FRIEND_NULL,
    YX_FRIEND_ERR,
    YX_FRIEND_NO_CARD,
    YX_FRIEND_NO_SRV,
    YX_FRIEND_NO_LAND,
    YX_FRIEND_SEARCH,
    YX_FRIEND_SEARCH_ERR,
    YX_FRIEND_SEARCH_NULL,
    YX_FRIEND_SEARCH_RSP,
    YX_FRIEND_CONFIRM,
    YX_FRIEND_CONFIRM_ERR,
    YX_FRIEND_CONFIRM_OK,
    YX_FRIEND_SARCH_MAX
}YXFriendDispStatus;
extern int YX_API_Friend_Start(void);
#endif

PUBLIC BOOLEAN  YX_PostEvtTo_APP(ZDT_APP_SIG_E sig_id, void* p_data, uint16 type);

extern MMI_RESULT_E YX_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param);
extern int YX_AppInit(void);
extern int YX_AppFree(void);
extern int YX_RestorFactory(void);
extern BOOLEAN YX_LocReport_Judge(void);
extern void YX_LocReport_Reset(void);

extern YX_APP_T  g_yx_app;
extern uint8  g_yx_imei_len;
extern BOOLEAN g_is_yx_power_off_ing;
extern BOOLEAN g_yx_is_stillness;
extern BOOLEAN g_yx_is_move;
extern BOOLEAN g_yx_is_jp_change;
extern BOOLEAN g_yx_is_lbs_change;
#endif
