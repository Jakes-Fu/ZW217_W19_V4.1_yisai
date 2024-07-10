/******************************************************************************
**
**
**
**  小区广播模块
**
**
**
**
**
**
*******************************************************************************/
#if defined(MMI_SMSCB_SUPPORT)

#ifndef  _MMISMSCB_CONTROL_H_    
#define  _MMISMSCB_CONTROL_H_   
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mn_type.h"
#include "dal_time.h"
#include "mmisms_export.h"
//#include "mmismscb_wintab.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif


#define MMI_MAX_CB_LANGUAGE_NUM (MN_CB_LANGUAGE_ID_MAX_NUM + 1)               //最大语言数
#ifdef MMI_ETWS_SUPPORT
#define MMI_CB_CHANNEL_NAME_LEN 30               //最大信道列表数
#else
#define MMI_CB_CHANNEL_NAME_LEN 10               //最大信道列表数
#endif
#define MMI_MAX_CB_CHANNEL_NUM  MN_CB_MSG_ID_MAX_NUM               //最大信道数

#ifdef MMI_ETWS_SUPPORT
#define MMI_MAX_CB_MSG_NUM      10     //最大广播信息存储数
#else
#define MMI_MAX_CB_MSG_NUM      (10 * MMI_DUAL_SYS_MAX)     //最大广播信息存储数
#endif

#define MMI_CB_ALERT_NUM 1             //CB Alert的信道个数
#ifdef MMI_ETWS_SUPPORT
#define MMI_CB_ETWS_TITLE_NAME_MAX_LEN  30
#endif

#define MMI_LTE_MACRO_MSG_LEN                 10972
#define MMI_GSMBIT_FORMAT_DATA_LEN_BIT        88
#define MMI_GSMBIT_FORMAT_SUPPORT_MAX_LEN     82
#define MMI_GSMBIT_FORMAT_PAGE_NUMBER_BIT     5
#define MMI_GSMBIT_FORMAT_ONEPAGE_LEN         83
#define MMI_GSMBIT_FORMAT_PARAMETER_LEN       6

#ifdef MMI_ETWS_SUPPORT

#define TXT_CMAS_PWS_911_TITLE {0x8B66, 0x8A0A, 0x901A, 0x77E5, 0}
#define TXT_CMAS_PWS_919_TITLE {'A', 'l', 'e', 'r', 't', ' ', 'M', 'e', 's', 's', 'a', 'g', 'e', 0}
#define TXT_CMAS_PWS_4370_TITLE {0x570B, 0x5BB6, 0x7D1A, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4371_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4372_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4373_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4374_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4375_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4376_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4377_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4378_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4379_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4380_TITLE {0x6BCF, 0x6708, 0x6E2C, 0x8A66, 0x7528, 0x8A0A, 0x606F, 0}
#define TXT_CMAS_PWS_4383_TITLE {'P', 'r', 'e', 's', 'i', 'd', 'e', 'n', 't', 'i', 'a', 'l', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4384_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4385_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4386_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4387_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4388_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4389_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4390_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4391_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4392_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4393_TITLE {'R', 'e', 'q', 'u', 'i', 'r', 'e', 'd', ' ', 'M', 'o', 'n', 't', 'h', 'l', 'y', ' ', 'T', 'e', 's', 't', 0}

#endif

#ifdef MMI_ETWS_SUPPORT
typedef struct
{
    uint16 channel_id;
    uint16 primary_channel;
    uint16 secondary_channel;
    wchar title_id[MMI_CB_ETWS_TITLE_NAME_MAX_LEN];
    wchar popup_string[MMI_CB_ETWS_TITLE_NAME_MAX_LEN];
    MMI_TEXT_ID_T string_id;
    uint16 is_default_enabled;
    uint16 can_be_disabled;
    uint16 can_be_visible;
    uint16 alert_level;
    uint16 alert_tone_type;
    uint16 vibrate_type;
    uint16 is_continued;
    uint16 alert_period;
} CMAS_CHANNEL_INFO_T;

typedef struct
{
    uint16 mcc;
    CMAS_CHANNEL_INFO_T *channel_info;
} CMAS_CONTROL_LIST_T;

typedef enum
{
    MMI_CMAS_CHILE_ID = 0x00,
    MMI_CMAS_UAE_ID,
    MMI_CMAS_TAIWAN_PWS_ID,
    MMI_CMAS_EU_ALERT_NL_ID,
    MMI_CMAS_EU_ALERT_LT_ID,
    MMI_CMAS_MAX_ID
} CMAS_CB_SPEC_T;

extern CMAS_CHANNEL_INFO_T s_cmas_chile_channel_info[];
extern CMAS_CHANNEL_INFO_T s_cmas_uae_channel_info[];
extern CMAS_CHANNEL_INFO_T s_cmas_pws_channel_info[];
extern CMAS_CHANNEL_INFO_T s_cmas_ue_alert_nl_channel_info[];
#endif

//------------------------------------------------------------------------------------------
//
// 小区广播语言选择列表结构
//
//------------------------------------------------------------------------------------------
typedef struct{

    BOOLEAN selected_arr[MMI_MAX_CB_LANGUAGE_NUM];                       //语言选择列表数组

}MMISMS_CB_LANGUAGE_T;


//------------------------------------------------------------------------------------------
//
// 小区广播信道名称结构
//
//------------------------------------------------------------------------------------------
typedef struct{

        wchar    wstr[MMI_CB_CHANNEL_NAME_LEN + 1];                           //信道名称字符串数组
        uint16   wstr_len;                                                //信道名称长度
}MMISMS_CB_CHANNEL_NAME_T;

//------------------------------------------------------------------------------------------
//
// 小区广播信道选项结构
//
//------------------------------------------------------------------------------------------
typedef struct{

        MN_CB_MSG_ID_T           msg_id;                                 //信道索引号
        MMISMS_CB_CHANNEL_NAME_T name;                                   //信道名称数组
        BOOLEAN is_enabled;                                              //是否开启       

}MMISMS_CB_CHANNEL_ITEM_T;

//------------------------------------------------------------------------------------------
//
// 小区广播信道列表结构
//
//------------------------------------------------------------------------------------------
typedef struct{

        uint16 num;                                                      //信道总数
        MMISMS_CB_CHANNEL_ITEM_T channel_arr[MMI_MAX_CB_CHANNEL_NUM];    //信道选项数组

}MMISMS_CB_CHANNEL_T;

//------------------------------------------------------------------------------------------
//
// 小区广播消息选项结构
//
//------------------------------------------------------------------------------------------
typedef struct mmisms_cbmsg_tag
{
    wchar             context[MN_CB_MACRO_MSG_LEN + 1];                   //小区广播消息内容
    uint16            len;                                            //小区广播消息内容长度
    SCI_TIME_T        time;                                           //接受时间
    SCI_DATE_T        date;                                           //接受日期
    MN_CB_MSG_ID_T    msg_id;                                         //信道索引号
    BOOLEAN           is_read;                                        //是否已被阅读
    MN_DUAL_SYS_E     dual_sys;
    struct mmisms_cbmsg_tag * msg_next_ptr;
}MMISMS_CB_MESSAGE_ITEM_T;

#ifdef MMI_ETWS_SUPPORT
#if 0
typedef struct mmisms_cbsmsg_tag
{
    wchar             context[MN_CB_MACRO_MSG_LEN + 1];                   //小区广播消息内容
    uint16            len;                                            //小区广播消息内容长度
    SCI_TIME_T        time;                                           //接受时间
    SCI_DATE_T        date;                                           //接受日期
    MN_CB_MSG_ID_T    msg_id;                                         //信道索引号
    BOOLEAN           is_read;                                        //是否已被阅读
    MN_DUAL_SYS_E     dual_sys;
}MMISMS_CB_STORAGE_MSG_ITEM_T;
#endif

typedef struct
{
    wchar             context[MN_CB_MACRO_MSG_LEN + 1];                   //小区广播消息内容
    uint16            len;                                            //小区广播消息内容长度
    SCI_TIME_T        time;                                           //接受时间
    SCI_DATE_T        date;                                           //接受日期
    MN_CB_MSG_ID_T    msg_id;                                         //信道索引号
    BOOLEAN           is_read;                                        //是否已被阅读
    MN_DUAL_SYS_E     dual_sys;
}MMISMS_CB_MESSAGE_ARRAY_T;

#endif

//------------------------------------------------------------------------------------------
//
// 小区广播消息列表结构
//
//------------------------------------------------------------------------------------------
typedef struct{
        uint16 num;                                                      //信息列表总数
        MMISMS_CB_MESSAGE_ITEM_T *msg_ptr;                                 //信息列表数组
}MMISMS_CB_MESSAGE_T;

//------------------------------------------------------------------------------------------
//
// 小区广播重要CB Channel 结构体
//
//------------------------------------------------------------------------------------------
typedef struct{
    uint16 type;
    MN_CB_MSG_ID_T cb_alert_id;
#ifdef MMI_ETWS_SUPPORT
    BOOLEAN is_enabled;
    MMISMS_CB_CHANNEL_NAME_T name;
#endif
}MN_CB_ALERT_CONTENT_T;

#ifdef MMI_ETWS_SUPPORT
typedef struct{
    uint16 type;
    MN_CB_MSG_ID_T cb_alert_id;
}MN_CB_ETWS_CONTENT_T;
#endif

typedef struct{
    uint16 emegence_num;
    uint16 sim_cbmi_num;
    uint16 soft_conf_num;
#ifdef MMI_ETWS_SUPPORT
//    uint16 etws_num;
#endif
    MN_CB_ALERT_CONTENT_T cb_alert_content[MMI_MAX_CB_CHANNEL_NUM];
#ifdef MMI_ETWS_SUPPORT
//    MN_CB_ETWS_CONTENT_T cb_etws_content[MMI_MAX_CB_CHANNEL_NUM];
#endif
    MN_CB_MSG_ID_T soft_conf_id[5];
}MN_CB_ALERT_MSG_T;

typedef enum
{
    MMI_CB_EMERGENCE_ID = 0x00,
#ifdef MMI_ETWS_SUPPORT
    MMI_CB_ETWS_ID,
#endif
    MMI_CB_SIM_CBMI_ID,
    MMI_CB_USER_CONF_ID,
}MMI_CB_ALERT_ID_T;

#ifdef MMI_ETWS_SUPPORT
#define MMISMS_CB_CONFIG_T MMINV_SMSCB_ENABLE_T
#endif

/******************************************************************************************************************/
//
//
//                        小区广播模块对外接口函数声明
//
//
/******************************************************************************************************************/

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  函数名                 ：MMIAPISMSCB_GetCBMsgType
//  功能描述           :
//  全局变量使用:    s_is_alert_type_msg
//  返回值                 :    TRUE    : Alert Message
//                                          FALSE  : Normal Message
//  作者                       :    Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC BOOLEAN MMIAPISMSCB_GetCBMsgType(void);

 /*****************************************************************************/
//  Description : get g_cb_alert_id[index]
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC MN_CB_MSG_ID_T MMIAPISMSCB_GetCBAlertID(uint8 index);

/*****************************************************************************/
//     Description :
//    Global resource dependence :
//  Author:Candice.Chen
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMSCB_GetETWSNum(void);

/*****************************************************************************/
//     Description :
//    Global resource dependence :
//  Author:Candice.Chen
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetETWSNum(uint16 num);
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名                 ：MMIAPISMSCB_InitCBAlertInfo
//  功能描述           :
//  全局变量使用:    s_cb_alert_msg
//  返回值                 :    void
//  作者                       :    Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMIAPISMSCB_InitCBAlertInfo(MN_DUAL_SYS_E dual_sys);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名                 ：MMIAPISMSCB_GetCBAllAlertInfo
//  功能描述           :
//  全局变量使用:    s_cb_all_alert_info
//  返回值                 :
//  作者                       :    Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MN_CB_ALERT_MSG_T  * MMIAPISMSCB_GetCBAllAlertInfo(MN_DUAL_SYS_E dual_sys);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         AddNewMsgNodeToCBMsgList
//  功能描述:        
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            CBK
//
//----------------------------------------------------------------------------------------------------------------

PUBLIC void AddNewMsgNodeToCBMsgList(MMISMS_CB_MESSAGE_ITEM_T* cur_msg_ptr);

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_Get_CB_Message_Num
//  功能描述:
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:           Candice
//
//----------------------------------------------------------------------------------------------------------------
 PUBLIC void MMISMS_Set_CB_Message_Num( uint16 num );

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_Get_CB_Message_Ptr
//  功能描述:
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            jixin.xu
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_MESSAGE_T *MMISMS_Get_CB_Message_Parent_Ptr( void );
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_Get_CB_Message_Num
//  功能描述:        
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            jixin.xu
//
//----------------------------------------------------------------------------------------------------------------
 PUBLIC uint16 MMISMS_Get_CB_Message_Num( void );

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_Get_CB_Message_Ptr
//  功能描述:        
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            jixin.xu
//
//----------------------------------------------------------------------------------------------------------------
 PUBLIC MMISMS_CB_MESSAGE_ITEM_T *MMISMS_Get_CB_Message_Ptr( void );

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_DeleteMsgFromCBMsgList
//  功能描述:        从小区广播收件箱列表中删除一条记录
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            liqing.peng
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMS_DeleteMsgFromCBMsgList(
                                  uint16      index
                                  );

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         IsCBMsgListEmpty
//  功能描述:        判断广播收件箱是否为空
//  全局变量使用:    s_cb_msg
//  返回值:          TRUE 为空，FALSE 不为空
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC BOOLEAN MMISMS_IsCBMsgListEmpty(void);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         UpdateCBMsgList
//  功能描述:        刷新小区广播收件箱列表
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMS_UpdateCBMsgList(
                           MMI_CTRL_ID_T    ctrl_id        //IN:
                           );

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_Init
//  功能描述:        初始化小区广播接口函数(在手机初始化时调用, MMIMAIN.c 中调用)
//  全局变量使用:    无
//  返回值:          MN_RETURN_SUCCESS 初始化成功，MN_RETURN_FAILURE 初始化失败
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
MN_RETURN_RESULT_E MMISMSCB_Init(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : open SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
void MMISMSCB_OpenMainMenu(void);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_SwitchSmsCBStatus
//  功能描述:        切换小区广播启动状态
//  全局变量使用:    无
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
void MMISMSCB_SwitchSmsCBStatus(
                                BOOLEAN cb_enabled      //要切换的状态 (输入参数)
                                );

/*****************************************************************************/
//  Description : open SMSCB setting window      
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
void MMISMSCB_OpenSettingWin(void);

/*****************************************************************************/
//  Description : set setting card
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMSCB_SetSelectCard(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : get setting card
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E  MMISMSCB_GetSelectCard( void );

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         InitCB_ETWSChannel
//  功能描述:
//  全局变量使用:    cell_broadcast_channel
//  返回值:
//  作者:
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_CHANNEL_T InitCB_ETWSChannel(void);

/*****************************************************************************/
//  Description : Get Current Channel table according the MCC
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC CMAS_CHANNEL_INFO_T* GetCMASChannelTable( MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Current Channel info according the channel ID
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC CMAS_CHANNEL_INFO_T* GetCMASChannelInfo( MN_DUAL_SYS_E dual_sys, uint16 msg_id);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif
#endif //#if defined(MMI_SMSCB_SUPPORT)

