/*****************************************************************************
** File Name:      mmisns_srv.c                                              *
** Author:         renwei                                                    *
** Date:           11/2011                                                   *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2011        renwei           Create                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmi_app_sns_trc.h"
#ifdef SNS_SUPPORT
//#include "std_header.h"
#include <stdio.h>
#include "mmisns_srv.h"
#include "mmk_type.h"
#include "mmiwre.h"
//#include "mmiwre_preload.h"
#include "sci_types.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmisns_nv.h"
#include "mmisms_id.h"
#if 0
#ifdef BROWSER_SUPPORT_DORADO
#include "mmibrowser_id.h"
#endif
#ifdef PIC_VIEWER_SUPPORT
#include "mmipicview_id.h"
#endif
#endif
#include "mmiwidget_id.h"
#include "mmisns_sqlite.h"
#include "mmifmm_export.h"
#include "os_api.h"
#include "gui_ucs2b_converter.h"
#include "mmimms_id.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmiwre_adaptor.h"
#include "mmiwre_export.h"
#include "mmipb_task.h"
#include "mmk_app.h"
#include "mmiudisk_export.h"
#include "mmipb_app.h"
#include "mmipub.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
//#define MMI_SNS_ACC_NAME_LEN       32
//#define MMI_SNS_ACC_PASSWORD_LEN   32

//#define MMI_SNS_TEXT_LEN           4 * 140
//#define MMI_SNS_MAX                3
//#define MMI_TEXT_LEN                 140
#define MMI_SNS_PIC_PATCH            128
#define MMI_USERID_LEN               60
#define MMI_FILE_PATH_LEN            255
#define MMI_FILE_NAME_LEN            100
#define WRE_FILE_NUM                 10
#define STRSTR                       strstr
#define SINA_SNS_SPLIT_MARK          "FEDCBA"

#define WRE_SINA_ACCOUNT_PATH                "E:\\wreapp\\sinaweibo\\save\\account.save"
#define WRE_TIWTTER_ACCOUNT_PATH             "E:\\wreapp\\WRETwit\\save\\account.save"
#define WRE_FACEBOOK_ACCOUNT_PATH            "E:\\wreapp\\Facebook\\save\\account.save"


#define WRE_SINA_FILE_FULL_PATH              "E:\\wreapp\\sinaweibo\\save\\userinfo\\%s\\sns_homeline"
#define WRE_SINA_FRIENDS_FILE_FULL_PATH      "E:\\wreapp\\sinaweibo\\save\\userinfo\\%s\\sns_friends"
#define WRE_SINA_ACTIVEINFO_FILE_FULL_PATH      "E:\\wreapp\\sinaweibo\\save\\userinfo\\%s\\sns_release"
//efine WRE_SINA_ACTIVEINFO_FILE_FULL_PATH  "E:\\wreapp\\sinaweibo\\save\\userinfo\\%s\\sns_latest.status"

#define WRE_TIWTTER_FILE_FULL_PATH           "E:\\wreapp\\WRETwit\\save\\userinfo\\%s\\sns_homeline"
#define WRE_TIWTTER_FRIENDS_FILE_FULL_PATH   "E:\\wreapp\\WRETwit\\save\\userinfo\\%s\\sns_friends"
//efine WRE_TIWTTER_ACTIVEINFO_FILE_FULL_PATH  "E:\\wreapp\\WRETwit\\save\\userinfo\\%s\\sns_latest.status"
#define WRE_TIWTTER_ACTIVEINFO_FILE_FULL_PATH      "E:\\wreapp\\sinaweibo\\save\\userinfo\\%s\\sns_release"

#define WRE_FACEBOOK_FILE_FULL_PATH          "E:\\wreapp\\Facebook\\save\\userinfo\\%s\\sns_homeline"
#define WRE_FACEBOOK_FRIENDS_FILE_FULL_PATH  "E:\\wreapp\\Facebook\\save\\userinfo\\%s\\sns_friends"
//#define WRE_FACEBOOK_ACTIVEINFO_FILE_FULL_PATH  "E:\\wreapp\\Facebook\\save\\userinfo\\%s\\sns_latest.status"
#define WRE_FACEBOOK_ACTIVEINFO_FILE_FULL_PATH      "E:\\wreapp\\sinaweibo\\save\\userinfo\\%s\\sns_release"

#define LOWORD(x)  (uint16)(0x0000ffff&(x))
/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

typedef struct _SNSId
{
    int mmi_sns_id;
    int wre_sns_id;
}T_SNSId;
        
T_SNSId g_SNSId[MMI_SNS_MAX]={
    
{MMI_SNS_SINA, WRE_FIX_ID_SINA},
{MMI_SNS_FACEBOOK, WRE_FIX_ID_FACEBOOK},            
{MMI_SNS_TIWTTER, WRE_FIX_ID_TWITTER},
};

typedef struct T_WreAppFileInfo
{
    unsigned short file_seq;
    unsigned short file_item_num;
}WreAppFileInfo;
/*
分享模块的数据可以用此结构给出
@mmi_data 	可字定义该结构以扩展其他需要的数据 
@id					模块id,可以枚举出所有类型
@text				分享的文本内容
@file_path 	分享的图片路径
@file_buf   分享的图片的buf
@buf_len    buf长度
*/
typedef struct _SNS_SHARE_DATA
{
	//平台模块id
	int 		id;
	
	//文本  字符串  //长度可计算
	uint16 *text;		
	
	//图片路径		字符串 //长度可计算
	uint16 *file_path;
	
	//图片buf     首地址 //长度预先给出
	char   *file_buf;
	int     buf_len;  //有效数据的长度	
	
	
	//预留数据区域
	void *mmi_data; 			
}T_SNS_SHARE_DATA;

typedef struct T_SNS_WIDGET_GET_DATA
{
    int           curr_num;     //当前个数
    int           diection_flag;//UP, DOWN
    int           index;// 索引值 GET WIDGETDATA的时候用的
    T_WIDGET_DATA text_info[MMI_WIDGET_TEXT_NUM];

}T_SNS_WIDGET_GET_DATA;

typedef struct
{
    E_MMISNS_WIDGET_COMMOND_TYPE widget_commond; //命令方式有:新建/获取数据/进入TEXT详情界面
    
    union
    {
        int                    text_detail_index;//该TEXT内容在WRE APP中的INDEX
        T_SNS_WIDGET_GET_DATA  data; 
    }u;   
    
}T_SNS_WIDGET_DATA;


typedef struct 
{
    int16 *file_path;// 大头贴路径
    int16 *name;     // 昵称
    int16 *email;    // email
    int16 *birthday; // 生日
    int16 *sign_text;// 签名

}T_FRIENDS_INFO;

#define PB_SNS_USER_ID_LEN  20

typedef struct SNS_PB_UPDATE_USER_STATUS
{
    char id[PB_SNS_USER_ID_LEN];
}SNS_PB_UPDATE_USER_STATUS;

typedef enum
{
   GOTO_CARE_LIST = 0x21, //关注列表
   GOTO_USER_DETAIL_LIST, //具体某个用户的详细列表

}PB_COMMOND_TYPE; /*lint !e726*/

typedef struct
{
    PB_COMMOND_TYPE pb_commond;

    union
    {
        SNS_PB_UPDATE_USER_STATUS  user_data;
    }u;
    
}T_SNS_PB_DATA;

typedef struct sns_commond_data
{
    //char sns_flag[4]; //区分MOCOR启动
    E_MMISNS_COMMOND_TYPE commond_type;//区分数据类型

    union
    {
        T_SNS_SHARE_DATA        data;           //分享数据，这里有SMS/MMS, PIC VIEW,/CAMERA等应用
        T_SNS_WIDGET_DATA       widget_data;    //
        T_SNS_PB_DATA   pb_data;//pb 获取好友信息
    }u;

}T_SNS_COMMOND_DATA;

typedef struct _CMMISns
{
    IMMISnsVtbl          *pvt;
    int                   m_nRef;
    T_SNS_COMMOND_DATA    data;
    
}CMMISns;



/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

LOCAL int MMISNS_AddRef(IMMISns *pIMMISns);
LOCAL int MMISNS_Release(IMMISns *pIMMISns);
LOCAL int MMISNS_CheckAccount(IMMISns *pIMMISns, int snsid, uint32 nameaddr, uint32 passwordaddr);
LOCAL int MMISNS_ShareToSns(IMMISns *pIMMISns, int snsid, int appid, uint32 dataaddr);
LOCAL int MMISNS_GetRegisterInfo(IMMISns *pIMMISns, uint32 infoaddr);
LOCAL int MMISNS_Register(IMMISns *pIMMISns, int snsid, uint32 infoaddr);
LOCAL int MMISNS_WidgetNewEdit(IMMISns *pIMMISns, int snsid);

#ifdef SNS_PULLING_SUPPORT
LOCAL int MMISNS_WidgetRefresh(IMMISns *pIMMISns, int snsid);
#endif //SNS_PULLING_SUPPORT

LOCAL int MMISNS_WidgetCmdGetData(IMMISns *pIMMISns, int snsid,int index, int direction_flag, unsigned long int data_addr);
LOCAL int MMISNS_WidgetEnterTextDetail(IMMISns *pIMMISns, int snsid, int index);
//LOCAL int MMISNS_WidgetGetData(IMMISns *pIMMISns, int snsid, unsigned long int data_addr);
LOCAL int MMISNS_WidgetGetNum(IMMISns *pIMMISns, int snsid, unsigned long int data_addr);
LOCAL int MMISNS_CheckUserLogIn(IMMISns *pIMMISns, int snsid, uint32 infoaddr);
LOCAL int MMISNS_WidgetUserLogIn(IMMISns *pIMMISns, int snsid);
LOCAL int MMISNS_CreateSNSDB(IMMISns *pIMMISns);
LOCAL int MMISNS_GetDataByDb(IMMISns *pIMMISns, uint32 infoaddr);
LOCAL int MMISNS_DestroySNSDB(IMMISns *pIMMISns);
LOCAL int MMISNS_GetNumByDbType(IMMISns *pIMMISns, uint32 infoaddr, uint32 etype);
LOCAL int MMISNS_GetDataByDbName(IMMISns *pIMMISns, uint32 infoaddr);
LOCAL int MMISNS_GetTotalNumByDb(IMMISns *pIMMISns, uint32 infoaddr);
LOCAL int MMISNS_GetDataByDbType(IMMISns *pIMMISns, uint32 infoaddr, uint32 etype);
LOCAL int MMISNS_IsDbOpen(IMMISns *pIMMISns);
LOCAL int MMISNS_ResetDBUerPara(IMMISns *pIMMISns, uint32 infoaddr);
LOCAL int MMISNS_IsFixAppInstalled(IMMISns *pIMMISns, int snsid);
LOCAL int MMISNS_AppInstall(IMMISns *pIMMISns, int snsid);

LOCAL int MMISNS_PBGetAccountNumber(IMMISns *pIMMISns, int snsid, uint8 *pNumber);
LOCAL int MMISNS_PBGetActiveInfo(IMMISns *pIMMISns, int snsid, uint8 *friendid, uint16 *pname, uint8 *pnum, PbActiveData  **data_addr);

/////////
LOCAL MMI_RESULT_E MMISNS_GetWidgetData(int snsid, int index, int direction_flag, unsigned long int data_addr);
LOCAL MMI_RESULT_E MMISNS_GetAccountUserId(int snsid, uint8 *pUserId);
LOCAL MMI_RESULT_E MMISNS_GetLogInData(int snsid, BOOLEAN *pLogIn);
LOCAL int MMISNS_InitPBDataBySNSId(int snsid);
LOCAL int MMISNS_PBGotoActiveWin(IMMISns *pIMMISns, int snsid, unsigned long int userid);
//LOCAL void  MMISNS_SendEvtToAppToReleaseMem (T_WIDGET_EVT_RESULT evt_res);
/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
IMMISnsVtbl gvtMMISns ={MMISNS_AddRef,
    MMISNS_Release,
    MMISNS_CheckAccount,
    MMISNS_ShareToSns,
    MMISNS_Register,
    MMISNS_GetRegisterInfo,
    MMISNS_IsFixAppInstalled,
    MMISNS_AppInstall,
    MMISNS_WidgetNewEdit,
#ifdef SNS_PULLING_SUPPORT
    MMISNS_WidgetRefresh,
#endif //SNS_PULLING_SUPPORT    
    MMISNS_WidgetCmdGetData,
    MMISNS_WidgetEnterTextDetail,
    //MMISNS_WidgetGetData,
    MMISNS_WidgetGetNum,
    MMISNS_CheckUserLogIn,
    MMISNS_WidgetUserLogIn,
    MMISNS_PBGetAccountNumber,
    MMISNS_PBGetActiveInfo,
    MMISNS_CreateSNSDB,
    MMISNS_GetDataByDb,
    MMISNS_DestroySNSDB,
    MMISNS_GetNumByDbType,
    MMISNS_GetDataByDbName,
    MMISNS_GetTotalNumByDb,
    MMISNS_GetDataByDbType,
    MMISNS_IsDbOpen,
    MMISNS_ResetDBUerPara,
    MMISNS_PBGotoActiveWin
    
};

LOCAL IMMISns *g_pIMMISns = PNULL;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create sns object
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
PUBLIC void IMMISNS_CreateObject(IMMISns **ppMMISns)
{
    CMMISns *pMe = PNULL;
    
    if(NULL == ppMMISns)
    {
        return;
    }

    pMe = (CMMISns *) SCI_ALLOCA(sizeof(CMMISns));
    SCI_TRACE_LOW("[MMISNS]CreateObject ALLOC");
    if(PNULL == pMe)
    {
        return;
    }
    
    //SCI_TRACE_LOW:"renwei-IMMISNS_CreateObject"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_273_112_2_18_2_55_44_27,(uint8*)"");
    SCI_MEMSET(pMe, 0, sizeof(CMMISns));
    
    pMe->pvt    = &gvtMMISns;
    pMe->m_nRef = 1;

    *ppMMISns = (IMMISns *)pMe;
}

/*****************************************************************************/
//  Description : add sns object num
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_AddRef(IMMISns *pMMISns)
{
    CMMISns        *pMe    = PNULL;
    //SCI_TRACE_LOW:"renwei-MMISNS_AddRef"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_289_112_2_18_2_55_44_28,(uint8*)"");
    if(PNULL == pMMISns)
    {
        return -1;
    }

    pMe =(CMMISns *)pMMISns;

    return pMe->m_nRef++;
}

/*****************************************************************************/
//  Description : add sns object num
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_Release(IMMISns *pMMISns)
{
    
    CMMISns        *pMe    = PNULL;

    if(PNULL == pMMISns)
    {
        return -1;
    }
    
    pMe =(CMMISns *)pMMISns;
    //SCI_TRACE_LOW:"renwei-MMISNS_Release  pMe->m_nRef =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_315_112_2_18_2_55_44_29,(uint8*)"d", pMe->m_nRef);
    if(--pMe->m_nRef == 0)
    {
        if(MMI_SNS_COMMOND_SHARE_DATA == pMe->data.commond_type)
        {
            //SCI_TRACE_LOW:"renwei-MMISNS_Release  SHARE_DATA pMe->m_nRef"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_320_112_2_18_2_55_44_30,(uint8*)"");
            if(pMe->data.u.data.text)
            {
                SCI_FREE(pMe->data.u.data.text);
                pMe->data.u.data.text = PNULL;
            }
        
            if(pMe->data.u.data.file_path)
            {
                SCI_FREE(pMe->data.u.data.file_path);
                pMe->data.u.data.file_path = PNULL;
            }

            if(pMe->data.u.data.file_buf)
            {
                SCI_FREE(pMe->data.u.data.file_buf);
                pMe->data.u.data.file_buf = PNULL;
            }
        
            SCI_FREE(pMe);
            pMe = PNULL;
        }
        else if(MMI_SNS_COMMOND_PB_GET_DATA == pMe->data.commond_type)
        {
            //SCI_TRACE_LOW:"renwei-MMISNS_Release  PB_GET_DATA pMe->m_nRef"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_344_112_2_18_2_55_44_31,(uint8*)"");
        }
        else if(MMI_SNS_COMMOND_WIDGET == pMe->data.commond_type)
        {
            //SCI_TRACE_LOW:"renwei-MMISNS_Release  COMMOND_WIDGET pMe->m_nRef"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_348_112_2_18_2_55_44_32,(uint8*)"");
            SCI_FREE(pMe);
            pMe = PNULL;
        }
        else 
        {
            //SCI_TRACE_LOW:"renwei-MMISNS_Release  other COMMOND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_354_112_2_18_2_55_44_33,(uint8*)"");
            SCI_FREE(pMe);
            pMe = PNULL;
        }
        
        return 0;
    }
    
    return pMe->m_nRef;

}
/*****************************************************************************/
//  Description : check account 
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_CheckAccount(IMMISns *pIMMISns, int snsid, uint32 nameaddr, uint32 passwordaddr)
{
#if 0
    mocor_sns_client_data *pclient_data = PNULL;
    CMMISns               *pMe          = PNULL;
    
    if(PNULL == pIMMISns || snsid >= MAX_WRE_FIXED_APP_NUM || 0 == nameaddr || 0 == passwordaddr)
    {
        return -1;
    }

    pMe          = (CMMISns *)pIMMISns;
    pclient_data = (mocor_sns_client_data *)SCI_ALLOC(sizeof(mocor_sns_client_data));

    if(PNULL == pclient_data)
    {
        return -1;
    }

    SCI_MEMSET(pclient_data, 0, sizeof(mocor_sns_client_data));
    
    //pclient_data->commond_type = MMI_SNS_ONLY_ACCOUNT;
    //SCI_STRCPY(pclient_data->name_password.name , (uint8 *)nameaddr);
    //SCI_STRCPY(pclient_data->name_password.password , (uint8 *)passwordaddr);

    MMIWRE_StartAppEx(snsid, pclient_data);

    SCI_FREE(pclient_data);
#endif
    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description : whether app is installed
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_IsFixAppInstalled(IMMISns *pIMMISns, int snsid)
{
    if(snsid >= MMI_SNS_MAX)
    {
        return -1;
    }
    
    return WRE_IsFixAppInstalled(g_SNSId[snsid].wre_sns_id);
}
/*****************************************************************************/
//  Description : installed app  
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_AppInstall(IMMISns *pIMMISns, int snsid)
{
    if(snsid >= MMI_SNS_MAX)
    {
        return -1;
    }
    
    return MMIWRE_StartApp(g_SNSId[snsid].wre_sns_id);
}
/*****************************************************************************/
//  Description : use widget enter new sns window
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_WidgetNewEdit(IMMISns *pIMMISns, int snsid)
{
    CMMISns   *pMe   = PNULL;
    uint8   temp[16] = {0};
    
    if(PNULL == pIMMISns || snsid >= MMI_SNS_MAX)
    {
        return MMI_RESULT_FALSE;
    }
    
    pMe = (CMMISns *)pIMMISns;

    pMe->data.commond_type                 = MMI_SNS_COMMOND_WIDGET;
    pMe->data.u.widget_data.widget_commond = MMI_WIDGET_SNS_COMMOND_NEW;

    //SCI_TRACE_LOW:"renwei-MMISNS_WidgetNewEdit  commond_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_445_112_2_18_2_55_45_34,(uint8*)"d",pMe->data.commond_type);
    //SCI_TRACE_LOW:"renwei-MMISNS_WidgetNewEdit  widget_commond =%d, snsid=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_446_112_2_18_2_55_45_35,(uint8*)"dd",pMe->data.u.widget_data.widget_commond, g_SNSId[snsid].wre_sns_id);

    sprintf((char *)temp, "SNS%d", (int)&pMe->data);
    MMIWRE_StartAppEx(g_SNSId[snsid].wre_sns_id, temp);
    
    return MMI_RESULT_TRUE; 

}

#ifdef SNS_PULLING_SUPPORT
/*****************************************************************************/
//  Description : use widget to perform sns refresh
//  Global resource dependence : 
//  Author: david chen
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_WidgetRefresh(IMMISns *pIMMISns, int snsid)
{
    CMMISns   *pMe   = PNULL;
    uint8   temp[16] = {0};
    
    if(PNULL == pIMMISns || snsid >= MMI_SNS_MAX)
    {
        return MMI_RESULT_FALSE;
    }
    
    pMe = (CMMISns *)pIMMISns;

    pMe->data.commond_type = MMI_SNS_COMMOND_WIDGET;
    pMe->data.u.widget_data.widget_commond = MMI_WIDGET_SNS_COMMOND_GET_DATA;

    sprintf((char *)temp, "SNS%d", (int)&pMe->data);

    return MMIWRE_StartAppSilent(g_SNSId[snsid].wre_sns_id, temp);
}
#endif //SNS_PULLING_SUPPORT

/*****************************************************************************/
//  Description : widget get sns data
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_WidgetCmdGetData(IMMISns *pIMMISns, int snsid, int index, int direction_flag, unsigned long int data_addr)
{
    CMMISns     *pMe = PNULL;
    MMI_RESULT_E res = MMI_RESULT_FALSE;
    
    if(PNULL == pIMMISns || snsid >= MMI_SNS_MAX || 0 == data_addr)
    {
        return MMI_RESULT_FALSE;
    }

    pMe = (CMMISns *)pIMMISns;

    switch(snsid)
    {
        case MMI_SNS_FACEBOOK:
        case MMI_SNS_TIWTTER:
        case MMI_SNS_SINA:
        {
            res = MMISNS_GetWidgetData(snsid, index, direction_flag, data_addr);
            break;
        }
        default:
            break;

    }

    return MMI_RESULT_TRUE; 
    
}
#if 0 
/*****************************************************************************/
//  Description : widget get sns data
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_WidgetGetData(IMMISns *pIMMISns, int snsid, unsigned long int data_addr)
{

  
    CMMISns       *pMe   = PNULL;
    T_WIDGET_DATA *pData = PNULL;
    int            i     = 0;
     //SCI_TRACE_LOW:"renwei-MMISNS_WidgetGetData"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_500_112_2_18_2_55_45_36,(uint8*)"");
    if(PNULL == pIMMISns || snsid >= MMI_SNS_MAX)
    {
        return MMI_RESULT_FALSE;
    }

    pMe   = (CMMISns *)pIMMISns;
    pData = (T_WIDGET_DATA *)data_addr;

    if(PNULL == pData)
    {
        return MMI_RESULT_FALSE;
    }
    
    for(i =0; i < pMe->data.u.widget_data.u.data.curr_num; i++)
    {
        pData[i].index = pMe->data.u.widget_data.u.data.text_info[i].index;
        //SCI_TRACE_LOW:"renwei-MMISNS_WidgetGetData  index=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_517_112_2_18_2_55_45_37,(uint8*)"d", pData[i].index);
        MMIAPICOM_Wstrcpy(pData[i].name, pMe->data.u.widget_data.u.data.text_info[i].name);
        MMIAPICOM_Wstrcpy(pData[i].date, pMe->data.u.widget_data.u.data.text_info[i].date);
        MMIAPICOM_Wstrcpy(pData[i].text, pMe->data.u.widget_data.u.data.text_info[i].text);
    }

    return MMI_RESULT_TRUE; 
}
#endif
/*****************************************************************************/
//  Description : widget get sns data num
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_WidgetGetNum(IMMISns *pIMMISns, int snsid, unsigned long int data_addr)
{
   
    CMMISns   *pMe = PNULL;
    //SCI_TRACE_LOW:"renwei-MMISNS_WidgetGetNum"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_534_112_2_18_2_55_45_38,(uint8*)"");
    if(PNULL == pIMMISns || snsid >= MMI_SNS_MAX)
    {
        return MMI_RESULT_FALSE;
    }

    pMe = (CMMISns *)pIMMISns;
    //SCI_TRACE_LOW:"renwei-MMISNS_WidgetGetNum num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_541_112_2_18_2_55_45_39,(uint8*)"d",pMe->data.u.widget_data.u.data.curr_num);
    *((uint16*)data_addr) = pMe->data.u.widget_data.u.data.curr_num;
    
    return MMI_RESULT_TRUE; 
    
}

/*****************************************************************************/
//  Description : enter wer sns text detail window
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_WidgetEnterTextDetail(IMMISns *pIMMISns, int snsid, int index)
{
    CMMISns   *pMe = PNULL;
    uint8 temp[16] ={0};
    if(PNULL == pIMMISns || snsid >= MMI_SNS_MAX)
    {
        return MMI_RESULT_FALSE;
    }

    pMe = (CMMISns *)pIMMISns;

    pMe->data.commond_type                      = MMI_SNS_COMMOND_WIDGET;
    pMe->data.u.widget_data.widget_commond      = MMI_WIDGET_SNS_COMMOND_ENTER_DETAIL;
    pMe->data.u.widget_data.u.text_detail_index = index;

    //SCI_TRACE_LOW:"renwei-MMISNS_WidgetEnterTextDetail  index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_567_112_2_18_2_55_45_40,(uint8*)"d", index);
    //SCI_TRACE_LOW:"renwei-MMISNS_WidgetEnterTextDetail  wre_sns_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_568_112_2_18_2_55_45_41,(uint8*)"d",g_SNSId[snsid].wre_sns_id);

    sprintf((char *)temp, "SNS%d", (int)&pMe->data);
    MMIWRE_StartAppEx(g_SNSId[snsid].wre_sns_id, temp);

    return MMI_RESULT_TRUE; 

}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_ShareToSns(IMMISns *pIMMISns, int snsid, int appid, uint32 dataaddr)
{
    CMMISns               *pMe         = PNULL;
    //MMI_STRING_T          *pData       = PNULL;
    //T_MMS_SHARE_TO_SNS    *pMMSData    = PNULL; 
    uint8                    temp[16]    = {0};
    uint16                  string_len  = 0;
    LCD_ANGLE_E      angle_ptr = LCD_ANGLE_0; 
    MMI_HANDLE_T     win_handle;	
    MMIFILE_DEVICE_E dev    = MMI_DEVICE_SDCARD;	
    //SCI_TRACE_LOW:"renwei-MMISNS_ShareToSns"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_590_112_2_18_2_55_45_42,(uint8*)"");

    if(PNULL == pIMMISns || snsid >= MMI_SNS_MAX ||  0 == dataaddr)
    {
        return MMI_RESULT_FALSE;
    }
	
    if (!MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(dev), MMIAPIFMM_GetDevicePathLen(dev)))
    {
        MMIPUB_OpenAlertWarningWin(TXT_NO_SD_CARD_SHARE_FAILED);
        return MMI_RESULT_FALSE;
    }
		
    if(MMIAPIUDISK_UdiskIsRun())
        {    
         MMIPUB_OpenAlertFailWin(TXT_SHARE_FAIL_UDISK_USING);
         return MMI_RESULT_FALSE;
         }


	
    #if 0
    if(SNS_MODULE_MMS != appid)
    {
        pData    = (MMI_STRING_T *)dataaddr;
    }
    else
    {
        pMMSData = (T_MMS_SHARE_TO_SNS *)dataaddr;
    }
    #endif    
    g_pIMMISns             = pIMMISns;
    SCI_TRACE_LOW("renwei --MMISNS_ShareToSns g_pIMMISns =%d", g_pIMMISns);
    pMe                    = (CMMISns *)pIMMISns;
    pMe->data.u.data.id    = appid;
    pMe->data.commond_type = MMI_SNS_COMMOND_SHARE_DATA;
    
    switch(appid)
    {        
        case SNS_MODULE_CAMERA:
        case SNS_MODULE_IMAGE_BROWSER:
        {  
            MMI_STRING_T *pData = PNULL;
            
            pData                      = (MMI_STRING_T *)dataaddr;
            string_len                 = pData->wstr_len;
            pMe->data.u.data.file_path = (uint16 *)SCI_ALLOCA((string_len+1) * sizeof(wchar));
            SCI_TRACE_LOW("[MMISNS]MMISNS_ShareToSns CAMERA OR IMAGE_BROWSER ALLOC");
            if(PNULL == pMe->data.u.data.file_path)
            {
                return MMI_RESULT_FALSE; 
            }
            
            SCI_MEMSET(pMe->data.u.data.file_path, 0, (string_len+1) * sizeof(wchar));
            MMIAPICOM_Wstrncpy(pMe->data.u.data.file_path, pData->wstr_ptr, string_len);
            
            break;
        }
        case SNS_MODULE_BROWSER:
        case SNS_MODULE_SMS:
        {
            MMI_STRING_T *pData = PNULL;
            
            pData                 = (MMI_STRING_T *)dataaddr;
            
            if(SNS_MODULE_BROWSER == appid && MMIAPIBROWSER_IsRunning())
            {
                MMIAPIBROWSER_Close();
            }
            
            string_len            = pData->wstr_len;
            pMe->data.u.data.text = (uint16 *)SCI_ALLOCA((string_len+1) * sizeof(wchar));
            SCI_TRACE_LOW("[MMISNS]MMISNS_ShareToSns BROWSER OR SMS ALLOC");
            if(PNULL == pMe->data.u.data.text)
            {
                return MMI_RESULT_FALSE; 
            }
            
            SCI_MEMSET(pMe->data.u.data.text, 0, (string_len+1) * sizeof(wchar));
            
            MMIAPICOM_Wstrncpy(pMe->data.u.data.text, pData->wstr_ptr, string_len);
            
            break;
        }
        case SNS_MODULE_MMS:
        {
            T_MMS_SHARE_TO_SNS    *pMMSData    = PNULL; 

            pMMSData = (T_MMS_SHARE_TO_SNS *)dataaddr;
            
            if(MMI_SNS_DATA_TEXT == pMMSData->buf_type)
            {
                string_len            = (uint16)MMIAPICOM_Wstrlen((wchar *)pMMSData->mms_buff);
                
                pMe->data.u.data.text = (uint16 *)SCI_ALLOCA((string_len+1)*sizeof(wchar));
                SCI_TRACE_LOW("[MMISNS]MMISNS_ShareToSns TEXT OF MMS ALLOC");                

                if(PNULL == pMe->data.u.data.text)
                {
                    return MMI_RESULT_FALSE; 
                }
                
                SCI_MEMSET(pMe->data.u.data.text, 0, (string_len+1)*sizeof(wchar));
                
                MMIAPICOM_Wstrncpy(pMe->data.u.data.text, (wchar*)pMMSData->mms_buff, string_len);
            }
            else if(MMI_SNS_DATA_PIC == pMMSData->buf_type)
            {
                pMe->data.u.data.buf_len  = pMMSData->mms_buff_len;
                pMe->data.u.data.file_buf = (uint16 *)SCI_ALLOCA((pMMSData->mms_buff_len+1) *sizeof(uint8));
                SCI_TRACE_LOW("[MMISNS]MMISNS_ShareToSns PIC OF MMS ALLOC");
                if(PNULL == pMe->data.u.data.file_buf)
                {
                    return MMI_RESULT_FALSE; 
                }
                
                SCI_MEMSET(pMe->data.u.data.file_buf, 0, (pMMSData->mms_buff_len+1) *sizeof(uint8));
                SCI_MEMCPY(pMe->data.u.data.file_buf, pMMSData->mms_buff, pMMSData->mms_buff_len);
            }
            
            break;
        }
        default:
            break;

    }
    
    sprintf((char *)temp, "SNS%d", (int)&pMe->data);
    win_handle =  MMK_GetFocusWinHandle();
    MMK_SetWinSupportAngle( win_handle, WIN_SUPPORT_ANGLE_0 );
    MMK_SetWinAngle( win_handle, 0, FALSE );	
    MMIWRE_StartAppEx(g_SNSId[snsid].wre_sns_id, temp);
    MMK_GetWinAngle(win_handle,&angle_ptr);            
    MMK_SetWinSupportAngle(win_handle, WIN_SUPPORT_ANGLE_ALL);
    MMK_SetWinAngle(win_handle, angle_ptr, FALSE); 	
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : register sns client info
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_Register(IMMISns *pIMMISns, int snsid, uint32 infoaddr)
{
    CMMISns               *pMe           = PNULL;
    T_SNSClientInfoData   *pClientdata   = PNULL;
    T_SNSClientRegInfo     ClientRegInfo = {0};
    MN_RETURN_RESULT_E     nv_return     = MN_RETURN_SUCCESS;
    int                    curr_num      = 0;
    BOOLEAN                find_flag     = MMI_RESULT_FALSE;
    int                    index         = 0;
    //SCI_TRACE_LOW:"renwei-MMISNS_Register"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_705_112_2_18_2_55_45_43,(uint8*)"");
    if(PNULL == pIMMISns || 0 == infoaddr)
    {
        return MMI_RESULT_FALSE;
    }
    
    pMe          = (CMMISns *)pIMMISns;
    pClientdata  = (T_SNSClientInfoData *)infoaddr;

    nv_return    = MMI_ReadNVItem(MMINV_SNS_STRUCT_PART, &ClientRegInfo);

    if(MN_RETURN_SUCCESS != nv_return)
    {
        MMI_WriteNVItem(MMINV_SNS_STRUCT_PART, &ClientRegInfo);
        //return MMI_RESULT_FALSE;
    }

    curr_num     = ClientRegInfo.curr_num;

    while(curr_num)
    {
        if(snsid == ClientRegInfo.data[curr_num-1].eSNSId)
        {
            find_flag = MMI_RESULT_TRUE;
            break;
        }

        curr_num --;
    }

    if(MMI_RESULT_TRUE == find_flag)
    {
        index = curr_num;
    }
    else
    {
        if(ClientRegInfo.curr_num + 1 > MMI_SNS_MAX)
        {
           return MMI_RESULT_FALSE; 
        }

        index = ClientRegInfo.curr_num;
        ClientRegInfo.curr_num ++;
                
    }

    MMI_MEMCPY((void *)&ClientRegInfo.data[index], sizeof(T_SNSClientInfoData), (void *)pClientdata,sizeof(T_SNSClientInfoData),sizeof(T_SNSClientInfoData));
    
    MMI_WriteNVItem(MMINV_SNS_STRUCT_PART, &ClientRegInfo);

    return MMI_RESULT_TRUE; 
}


/*****************************************************************************/
//  Description : Get register sns client info
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_GetRegisterInfo(IMMISns *pIMMISns, uint32 infoaddr)
{
    CMMISns               *pMe            = PNULL;
    MN_RETURN_RESULT_E     nv_return      = MN_RETURN_SUCCESS;
    T_SNSClientRegInfo    *pClientRegInfo = {0};
    //SCI_TRACE_LOW:"renwei-MMISNS_GetRegisterInfo"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_768_112_2_18_2_55_45_44,(uint8*)"");
    if(PNULL == pIMMISns || 0 == infoaddr)
    {
        return MMI_RESULT_FALSE;
    }
    
    pMe            = (CMMISns *)pIMMISns;
    pClientRegInfo = (T_SNSClientRegInfo *)infoaddr;

    nv_return    = MMI_ReadNVItem(MMINV_SNS_STRUCT_PART, pClientRegInfo);

    if(MN_RETURN_SUCCESS != nv_return)
    {
        return MMI_RESULT_FALSE;
    }
    
    return MMI_RESULT_TRUE; 
}
/*****************************************************************************/
//  Description : sns Init PB Data
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
PUBLIC void MMISNS_InitPBData(void)
{
    int     i      = 0;
    BOOLEAN bLogIn = 0;
    
    while(i < MMI_SNS_MAX)
    {
        bLogIn = 0;
        
        switch(i)
        {
            case MMI_SNS_FACEBOOK:
            case MMI_SNS_TIWTTER:
            case MMI_SNS_SINA:
            {
                MMISNS_GetLogInData(i, &bLogIn);        

                if(bLogIn)
                {
                    MMISNS_InitPBDataBySNSId(i);
                }
                break;
            }
            default:
                break;
        }

        i++;
    }  
}
/*****************************************************************************/
//  Description : sns module init
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
PUBLIC void MMISNS_ModuleInit(void)
{
    MMI_RegSNSClientInfoNv();
}
/*****************************************************************************/
//  Description : read Sns Friends file
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISNS_GetSnsFriendsId(FILEARRAY  file_array, int fileindex, uint16 *pname, uint8 *pfriendsid)
{
    uint8           *buf_ptr                         = PNULL;
    uint8           *p1                              = PNULL;
    uint8           *p3                              = PNULL;
    uint16           wname[MMI_NAME_LEN+1]           = {0};
    FILEARRAY_DATA_T array_item                      = {0};
    MMI_RESULT_E     res                             = MMI_RESULT_FALSE;
    uint8  friendid[MMI_USERID_LEN] = {0};

    uint8          name[MMI_NAME_LEN+1] ={0};
    if(PNULL == file_array)
    {
        return res;
    }
    
    MMIAPIFILEARRAY_Read(file_array, (uint32)fileindex, &array_item );
    
    buf_ptr = SCI_ALLOCA(array_item.size);
    
    if (PNULL == buf_ptr)
    {
        return res;
    }
    
    SCI_MEMSET(buf_ptr, 0x00, array_item.size);

    if(MMIFILE_ReadFilesDataSyn((const uint16 *)array_item.filename, array_item.name_len, buf_ptr, array_item.size))
    {
        uint32    j         = 0;
        
        p1 = buf_ptr;
        p3 = p1;
        
        while(p3 < buf_ptr+ array_item.size)
        {
            j  = 0;
            p3 = p1;

            // get user id 
            while(*p3 != '\0' && j < array_item.size)
            {   
                friendid[j] = *p3;
                p3++;
                j++; 
            }
            
            p3 ++; //跳过\0
            
            // get name
            j  = 0;
            while(*p3 != '\0' && j < array_item.size)
            {
                name[j] = *p3;
                
                p3++;
                j++; 
            }

            GUI_UTF8ToWstr(wname, MMI_NAME_LEN, name, j);
            
            if( 0 == MMIAPICOM_Wstrcmp(pname, wname))
            {

               SCI_STRCPY((char *)pfriendsid, (char *)friendid);
               SCI_FREE(buf_ptr);
    
               return MMI_RESULT_TRUE; 
            }
            
            p3 ++; //跳过\0

            // get photo path
            j = 0;
            while(*p3 != '\0' && j < array_item.size)
            {                
                p3++;
                j++; 
            }

            p3 ++; //跳过\0
            
            // get email
            j = 0;
            if('\0' == *p3)
            {
                ;
            }
            else
            {
                while(*p3 != '\0' && j < array_item.size)
                {                    
                    p3++;
                    j++; 
                }
                
            }
            
            p3 ++; //跳过\0  

            // get birthday
            j = 0;
            if('\0' == *p3)
            {
                ;
            }
            else
            {
                while(*p3 != '\0' && j < array_item.size)
                {                    
                    p3++;
                    j++; 
                } 
            }
            
            p3 ++; //跳过\0

            // get brief
           
            j = 0;
            while(*p3 != '\0' && j < array_item.size)
            {                
                p3++;
                j++; 
            }
            
            p3 ++; //跳过\0    
            
            p3 += 3;//SCI_STRLEN(SINA_SNS_SPLIT_MARK);
            p1  = p3;
              
            SCI_MEMSET(name, 0x00, MMI_NAME_LEN);
        }
    }
    
    SCI_FREE(buf_ptr);
    
    return MMI_RESULT_FALSE; 
}
/*****************************************************************************/
//  Description : read SnsHomeLine file
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISNS_ParseFileGetActiveTextEx(uint16* filepath, uint16 *pname, uint8 *pNum, unsigned long int data_addr)
{
    uint8           *buf_ptr    = PNULL;
    uint8           *p1         = PNULL;
    uint8           *p2         = PNULL;
    uint8           *p3         = PNULL;
    MMI_RESULT_E     res        = MMI_RESULT_FALSE;
    uint8          name[MMI_NAME_LEN+1] ={0};
    uint8          date[MMI_DATE_LEN+1] ={0}; 
    uint8          *ptext = PNULL;
    PbActiveData  *pactivedata = PNULL;
    uint32         file_size =0;
    
    if(PNULL == filepath || 0 == data_addr)
    {
        return res;
    }

    pactivedata = (PbActiveData *)data_addr;
    
    MMIAPIFMM_GetFileInfo(filepath, MMIAPICOM_Wstrlen(filepath), &file_size, NULL, NULL);
    
    buf_ptr = SCI_ALLOCA(file_size);
    
    if (PNULL == buf_ptr)
    {
        return res;
    }
    
    SCI_MEMSET(buf_ptr, 0x00, file_size);

    if(MMIFILE_ReadFilesDataSyn((const uint16 *)filepath, MMIAPICOM_Wstrlen(filepath), buf_ptr, file_size))
    {
        uint32 i         = 0;
        uint32 j         = 0;
        
        ptext = SCI_ALLOCA(MMI_TEXT_LEN);

        if(PNULL == ptext)
        {
            SCI_FREE(buf_ptr);
            return MMI_RESULT_FALSE; 
        }
        
        SCI_MEMSET(ptext, 0x00, MMI_TEXT_LEN);

        p1 = buf_ptr;
        
        {
            j  = 0;
            p2 = p1;
            p3 = p1;

            // get text
            j = 0;
            while(*p3 != '\0' && j < file_size && j < MMI_TEXT_LEN)
            {
                ptext[j] = *p3;
                
                p3++;
                j++; 
            }

            GUI_UTF8ToWstr(pactivedata[0].text, j, ptext, j);

            if(*p3 != '\0')
            {
                while(*p3 != '\0' && j < file_size)
                {
                    p3++;
                    j++;
                }
            }

            p3 ++; //跳过\0

             // get date
            j = 0;
            while(*p3 != '\0' && j < file_size && j < MMI_DATE_LEN)
            {
                date[j] = *p3;
                
                p3++;
                j++; 
            }

            GUI_UTF8ToWstr(pactivedata[0].date, j, date, j);
            
            p3 ++; //跳过\0
            
            i++;
            
            p3 += 3;//SCI_STRLEN(SINA_SNS_SPLIT_MARK);
            p1  = p3;

            SCI_MEMSET(name, 0x00, MMI_NAME_LEN);
            SCI_MEMSET(date, 0x00, MMI_DATE_LEN);
            SCI_MEMSET(ptext, 0x00, MMI_TEXT_LEN);
        }

        SCI_FREE(ptext);
        *pNum = 1;
        
    }
   
    SCI_FREE(buf_ptr);

    return MMI_RESULT_TRUE; 
}
/*****************************************************************************/
//  Description : read SnsHomeLine file
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISNS_ParseFileGetActiveText(FILEARRAY  file_array, int fileindex, uint16 *pname, uint8 *pNum, unsigned long int data_addr)
{
    uint8           *buf_ptr    = PNULL;
    uint8           *p1         = PNULL;
    uint8           *p2         = PNULL;
    uint8           *p3         = PNULL;
    FILEARRAY_DATA_T array_item = {0};
    MMI_RESULT_E     res        = MMI_RESULT_FALSE;
    uint8          name[MMI_NAME_LEN+1] ={0};
    uint16         wname[MMI_NAME_LEN+1] ={0};
    uint8          date[MMI_DATE_LEN+1] ={0}; 
    uint8          *ptext = PNULL;
    PbActiveData  **pactivedata = PNULL;
    BOOLEAN        bfind =0;
    int            index = 0;
    
    if(PNULL == file_array || 0 == data_addr)
    {
        return res;
    }

    pactivedata = (PbActiveData **)data_addr;
    
    MMIAPIFILEARRAY_Read(file_array, (uint32)fileindex, &array_item );
    
    buf_ptr = SCI_ALLOCA(array_item.size);
    
    if (PNULL == buf_ptr)
    {
        return res;
    }
    
    SCI_MEMSET(buf_ptr, 0x00, array_item.size);

    if(MMIAPIFMM_ReadFilesDataSyn((const uint16 *)array_item.filename, array_item.name_len, buf_ptr, array_item.size))
    {
        uint32 i         = 0;
        uint32 j         = 0;
        
        ptext = SCI_ALLOCA(MMI_TEXT_LEN);

        if(PNULL == ptext)
        {
            SCI_FREE(buf_ptr);
            return MMI_RESULT_FALSE; 
        }
        
        SCI_MEMSET(ptext, 0x00, MMI_TEXT_LEN);

        p1 = buf_ptr;
        p3 = p1;
        while(p3 < buf_ptr+ array_item.size && *pNum < MMI_PB_ACTIVE_TEXT_NUM)
        {
            j  = 0;
            p2 = p1;
            p3 = p1;

            // get name
            while(*p3 != '\0' && j < array_item.size && j < MMI_NAME_LEN)
            {
                name[j] = *p3;
                
                p3++;
                j++; 
            }

            GUI_UTF8ToWstr(wname, j, name, j);

            if(*p3 != '\0')
            {
                while(*p3 != '\0' && j < array_item.size)
                {
                    p3++;
                    j++;
                }
            }
                        
            if( 0 == MMIAPICOM_Wstrcmp(pname, wname))
            {
                *pNum = *pNum +1;
                bfind = TRUE;
            }
            
            p3 ++; //跳过\0

            // get text
            j = 0;
            while(*p3 != '\0' && j < array_item.size && j < MMI_TEXT_LEN)
            {
                ptext[j] = *p3;
                
                p3++;
                j++; 
            }
            
            if(bfind)
            {
                index = *pNum -1;
                
                GUI_UTF8ToWstr(pactivedata[index]->text, j, ptext, j);
            }
            
            if(*p3 != '\0')
            {
                while(*p3 != '\0' && j < array_item.size)
                {
                    p3++;
                    j++;
                }
            }
            
            p3 ++; //跳过\0

             // get date
            j = 0;
            while(*p3 != '\0' && j < array_item.size)
            {
                date[j] = *p3;
                
                p3++;
                j++; 
            }

            if(bfind)
            {
                index = *pNum -1;

                GUI_UTF8ToWstr(pactivedata[index]->date, j, date, j);
            }
            
            p3 ++; //跳过\0
            
            i++;
            
            p3 += 3;//SCI_STRLEN(SINA_SNS_SPLIT_MARK);
            p1  = p3;

            bfind = FALSE;
            SCI_MEMSET(name, 0x00, MMI_NAME_LEN);
            SCI_MEMSET(date, 0x00, MMI_DATE_LEN);
            SCI_MEMSET(ptext, 0x00, MMI_TEXT_LEN);
        }

        SCI_FREE(ptext);
        
    }
    
    SCI_FREE(buf_ptr);

    return MMI_RESULT_TRUE; 
}
/*****************************************************************************/
//  Description : read SnsHomeLine file
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISNS_ReadSnsHomeLine(FILEARRAY  file_array, int fileindex, WreAppFileInfo *pfile_info, unsigned long int data_addr)
{
    uint8           *buf_ptr    = PNULL;
    //uint8           *pItemPos   = PNULL;
    //uint8           *pPos       = PNULL;
    uint8           *p1         = PNULL;
    uint8           *p2         = PNULL;
    uint8           *p3         = PNULL;
    T_WIDGET_DATA   *pData      = PNULL;
    FILEARRAY_DATA_T array_item = {0};
    MMI_RESULT_E     res        = MMI_RESULT_FALSE;
    uint32            file_item_num  = 0;
    uint8          name[MMI_NAME_LEN+1] ={0};
    uint8          date[MMI_DATE_LEN+1] ={0}; 
    uint8          *ptext = PNULL;
    
    
    if(PNULL == file_array || 0 == data_addr)
    {
        return res;
    }
    
    pData = (T_WIDGET_DATA *)data_addr;
    
    MMIAPIFILEARRAY_Read(file_array, (uint32)fileindex, &array_item );
    
    buf_ptr = SCI_ALLOCA(array_item.size);
    
    if (PNULL == buf_ptr)
    {
        return res;
    }
    
    SCI_MEMSET(buf_ptr, 0x00, array_item.size);

    if(MMIAPIFMM_ReadFilesDataSyn((const uint16 *)array_item.filename, array_item.name_len, buf_ptr, array_item.size))
    {
        uint32 i         = 0;
        uint32 j         = 0;
        uint32 num_index = 0;
        
        ptext = SCI_ALLOCA(MMI_TEXT_LEN);

        if(PNULL == ptext)
        {
            SCI_FREE(buf_ptr);
            return MMI_RESULT_FALSE; 
        }
        
        SCI_MEMSET(ptext, 0x00, MMI_TEXT_LEN);
      
        while(j < (uint32)fileindex && j < WRE_FILE_NUM)
        {
            num_index = (j+1) *pfile_info[j].file_item_num;
            j++;
        }
        
        p1 = buf_ptr;
        file_item_num = pfile_info[fileindex].file_item_num;
        
        while(i < file_item_num && i < WRE_FILE_NUM)
        {
            j  = 0;
            p2 = p1;
            p3 = p1;

            // get name
            while(*p3 != '\0' && j < array_item.size && j < MMI_NAME_LEN)
            {
                name[j] = *p3;
                
                p3++;
                j++; 
            }

            
            GUI_UTF8ToWstr(pData[i].name, j, name, j);

            if(*p3 != '\0')
            {
                while(*p3 != '\0' && j < array_item.size)
                {
                    p3++;
                    j++;
                }
            }
            
            p3 ++; //跳过\0

            // get text
            j = 0;
            while(*p3 != '\0' && j < array_item.size && j < MMI_TEXT_LEN)
            {
                ptext[j] = *p3;
                
                p3++;
                j++; 
            }

            GUI_UTF8ToWstr(pData[i].text, j, ptext, j);

            if(*p3 != '\0')
            {
                while(*p3 != '\0' && j < array_item.size)
                {
                    p3++;
                    j++;
                }
            }
            
            p3 ++; //跳过\0

             // get date
            j = 0;
            while(*p3 != '\0' && j < array_item.size)
            {
                date[j] = *p3;
                
                p3++;
                j++; 
            }

            GUI_UTF8ToWstr(pData[i].date, j, date, j);

            // get wre index
            pData[i].index = num_index +i;

            
            p3 ++; //跳过\0
            
            i++;
            
            p3 += 3;//SCI_STRLEN(SINA_SNS_SPLIT_MARK);
            p1  = p3;

            SCI_MEMSET(name, 0x00, MMI_NAME_LEN);
            SCI_MEMSET(date, 0x00, MMI_DATE_LEN);
            SCI_MEMSET(ptext, 0x00, MMI_TEXT_LEN);
        }

        SCI_FREE(ptext);
        
    }
    
    SCI_FREE(buf_ptr);

    return MMI_RESULT_TRUE; 
}

/*****************************************************************************/
//  Description : read widget data
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISNS_GetWidgetData(int snsid, int index, int direction_flag, unsigned long int data_addr)
{
    uint32       file_num                          = 0;
    FILEARRAY    file_array                        = {0};
    uint8        UserId[MMI_USERID_LEN]            = {0};
    uint8         full_path[MMI_FILE_PATH_LEN]      = {0}; 
    uint16        wstr_full_path[MMI_FILE_PATH_LEN] = {0}; 
    MMI_RESULT_E res                               = MMI_RESULT_FALSE;
    MMIFMM_FILTER_T filter                         = {{"*"}};
    uint32 i = 0;
    int file_index = 0;
    uint16  *file_name[WRE_FILE_NUM] = {0};
    uint8   ufile_name[MMI_FILE_NAME_LEN] = {0};
    WreAppFileInfo *pfile_info = PNULL;
    uint16 file_len = 0;
        
    FILEARRAY_DATA_T array_item = {0};
    
    if(PNULL == data_addr)
    {
        return res;
    }
    
    res = MMISNS_GetAccountUserId(snsid, (uint8 *)&UserId);  
    
    if(MMI_RESULT_FALSE == res || 0 == SCI_STRLEN((char *)UserId))    
    {
        return res;
    }

    if(MMI_SNS_SINA == snsid)
    {
        sprintf((char *)full_path, WRE_SINA_FILE_FULL_PATH, UserId);
    }
    else if(MMI_SNS_TIWTTER == snsid)
    {
        sprintf((char *)full_path, WRE_TIWTTER_FILE_FULL_PATH, UserId);
    }
    else
    {
        sprintf((char *)full_path, WRE_FACEBOOK_FILE_FULL_PATH, UserId);
    }
    
    
    file_array = MMIAPIFILEARRAY_Create();

    MMIAPICOM_StrToWstr(full_path, wstr_full_path);
    
    MMIAPIFMM_SearchFileInPath(wstr_full_path, 
                            MMIAPICOM_Wstrlen(wstr_full_path),
                            &filter,
                            FALSE,
                            FUNC_FIND_FILE,
                            file_array,
                            0,
                            0);

    
    
    file_num  = MMIAPIFILEARRAY_GetArraySize(file_array);

    if(0 == file_num)
    {
        MMIAPIFILEARRAY_Destroy(&file_array);
        return res;
    }

    for(i = 0 ; i < file_num; i++)
    {
        file_name[i] = (uint16 *)SCI_ALLOCA(sizeof(uint16)*MMI_FILE_NAME_LEN);

        if(PNULL == file_name[i])
        {
            uint32 j = 0;
            
            MMIAPIFILEARRAY_Destroy(&file_array);
            
            while(j <= i && j < file_num)
            {
                if(file_name[j])
                {
                    SCI_FREE(file_name[j]);
                }
                j++;
            }
            
            return res;
        }

        SCI_MEMSET((uint16*)file_name[i], 0, sizeof(uint16)*MMI_FILE_NAME_LEN);
    }
    
    file_len    = sizeof(uint16)*MMI_FILE_NAME_LEN;
    
    pfile_info  = (WreAppFileInfo *)SCI_ALLOCA(sizeof(WreAppFileInfo)*file_num);

    if(PNULL == pfile_info)    
    {
        for(i = 0 ; i < file_num; i++)
        {
            if(file_name[i])
            {
                SCI_FREE(file_name[i]);
            }
        }
        
        MMIAPIFILEARRAY_Destroy(&file_array);
        return MMI_RESULT_FALSE;
    }

    SCI_MEMSET((uint16*)pfile_info, 0, sizeof(WreAppFileInfo)*file_num);

    MMIAPIFILEARRAY_Sort(file_array, FILEARRAY_SORT_NAME_ASCEND);

    for(i = 0; i < file_num; i++)
    {
        MMIAPIFILEARRAY_Read(file_array, (uint32)i, &array_item );

        MMIAPIFMM_SplitFullPathExt((const uint16 *)array_item.filename,
                                  MMIAPICOM_Wstrlen(array_item.filename),
                                  PNULL,
                                  PNULL,
                                  file_name[i], 
                                  &file_len,
                                  PNULL,
                                  PNULL
                                  );

        MMIAPICOM_WstrToStr(file_name[i], ufile_name);
        pfile_info[i].file_item_num =  LOWORD(atoi(ufile_name));/*lint !e64*/
    }

    //寻找对应的文件INDEX

    if(0 == index && 0 == direction_flag)//翻到最后一页
    {
        res = MMISNS_ReadSnsHomeLine(file_array, file_num-1, pfile_info, data_addr);
    }
    else if(0 == index && 1 == direction_flag)//刚开始LOAD数据
    {
        res = MMISNS_ReadSnsHomeLine(file_array, 0, pfile_info, data_addr);
    }
    else
    {
        int item_num = 0;
        i = 0;

        item_num = (i+1)*pfile_info[i].file_item_num;
        
        while(item_num <= index && i < file_num-1)
        {
            i++;
            item_num += pfile_info[i].file_item_num;
        }

        if(direction_flag)//down
        {
            if(file_num-1 == i)
            {
                file_index = 0;
            }
            else
            {
                file_index = i+1;
            }
        }
        else //up
        {
            if(0 == i)
            {
                file_index = file_num-1;
            }
            else
            {
                file_index = i-1;
            }
        }
        
        
        res = MMISNS_ReadSnsHomeLine(file_array, file_index, pfile_info, data_addr);
    }
    
    for(i = 0 ; i < file_num; i++)
    {
        if(file_name[i])
        {
            SCI_FREE(file_name[i]);
        }
    }
    
    SCI_FREE(pfile_info);
    MMIAPIFILEARRAY_Destroy(&file_array);
    return res;

}

/*****************************************************************************/
//  Description :Read SNS Account File
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISNS_ReadSNSAccountFile(uint16* full_path_arr, char **ppbuf)
{
    MMI_RESULT_E  res         = MMI_RESULT_FALSE;
    SFS_HANDLE    sfs_handle  = NULL;
    int           size        = 0;
    uint32        read_num    = 0;
    MMIFILE_ERROR_E  read_file   = SFS_NO_ERROR;


    if(PNULL == ppbuf)
    {
        return res; 
    }
    
    if(FALSE == MMIAPIFMM_IsFileExist(full_path_arr, MMIAPICOM_Wstrlen(full_path_arr))) 
    {
        return res;
    }

    sfs_handle = MMIAPIFMM_CreateFile(full_path_arr, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, PNULL, PNULL);

    if(SFS_INVALID_HANDLE == sfs_handle)
    {
        return res;
    }

    size = MMIAPIFMM_GetFileSize(sfs_handle);
    *ppbuf = SCI_ALLOCA(size +1);

    if(PNULL == *ppbuf)
    {
        MMIAPIFMM_CloseFile(sfs_handle);
        return res;
    }
    
    SCI_MEMSET(*ppbuf, 0, size+1);

    do
    {
        read_file = MMIAPIFMM_ReadFile(sfs_handle, *ppbuf, size, &read_num, PNULL);

        if(SFS_NO_ERROR != read_file)
        {
           MMIAPIFMM_CloseFile(sfs_handle);
           SCI_FREE(*ppbuf);
           *ppbuf    = PNULL;
           return MMI_RESULT_FALSE;
        }
        
    }while(0);

    MMIAPIFMM_CloseFile(sfs_handle);
    return MMI_RESULT_TRUE;    
}
/*****************************************************************************/
//  Description :get wre app active info
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_PBGetActiveInfo(IMMISns *pIMMISns, int snsid, uint8 *friendid, uint16 *pname, uint8 *pnum, PbActiveData  **pactivetext)
{
    uint32       file_num                           = 0;
    FILEARRAY    file_array                         = {0};
    //uint8        UserId[MMI_USERID_LEN]             = {0};
    uint8         full_path[MMI_FILE_PATH_LEN]      = {0}; 
    uint16        wstr_full_path[MMI_FILE_PATH_LEN] = {0}; 
    MMI_RESULT_E res                               = MMI_RESULT_FALSE;
    MMIFMM_FILTER_T filter                         = {{"*"}};
    uint32 i = 0;    
    
    if(PNULL == pnum || PNULL == pactivetext || PNULL == pname || PNULL == friendid  || 0 == SCI_STRLEN(friendid))
    {
        return res; 
    }
#if 0
    res = MMISNS_GetAccountUserId(snsid, (uint8 *)&UserId);  
    
    if(MMI_RESULT_FALSE == res || 0 == SCI_STRLEN((char *)UserId))    
    {
        *pnum = 0;
        return MMI_RESULT_TRUE;
    }
#endif

    if(MMI_SNS_SINA == snsid)
    {
        sprintf((char *)full_path, WRE_SINA_ACTIVEINFO_FILE_FULL_PATH, friendid);
    }
    else if(MMI_SNS_TIWTTER == snsid)
    {
        sprintf((char *)full_path, WRE_TIWTTER_ACTIVEINFO_FILE_FULL_PATH, friendid);
    }
    else
    {
        sprintf((char *)full_path, WRE_FACEBOOK_ACTIVEINFO_FILE_FULL_PATH, friendid);
    }
    
    
    file_array = MMIAPIFILEARRAY_Create();

    MMIAPICOM_StrToWstr(full_path, wstr_full_path);
    
    MMIAPIFMM_SearchFileInPath(wstr_full_path, 
                            MMIAPICOM_Wstrlen(wstr_full_path),
                            &filter,
                            FALSE,
                            FUNC_FIND_FILE,
                            file_array,
                            0,
                            0);
    
    file_num  = MMIAPIFILEARRAY_GetArraySize(file_array);

    if(0 == file_num)
    {
        MMIAPIFILEARRAY_Destroy(&file_array);
        *pnum = 0;
        return MMI_RESULT_TRUE;
    }

    for(i = 0 ; pactivetext && i < MMI_PB_ACTIVE_TEXT_NUM; i++)
    {
        pactivetext[i] = (PbActiveData  *)SCI_ALLOCA(sizeof(PbActiveData));

        if(PNULL == pactivetext[i])
        {
            uint32 j = 0;
            
            MMIAPIFILEARRAY_Destroy(&file_array);
            
            while(j <= i && j < file_num)
            {
                if(pactivetext[i])
                {
                    SCI_FREE(pactivetext[i]);
                }
                j++;
            }

            *pnum = 0;
            MMIAPIFILEARRAY_Destroy(&file_array);
            return res;
        }

        SCI_MEMSET(pactivetext[i], 0, sizeof(PbActiveData));
    }
    
    MMIAPIFILEARRAY_Sort(file_array, FILEARRAY_SORT_NAME_ASCEND);

    for(i = 0; i < file_num; i++)
    {
        if(*pnum >= MMI_PB_ACTIVE_TEXT_NUM)
        {
            break;
        }
        
        res = MMISNS_ParseFileGetActiveText(file_array, i, pname, pnum, (unsigned long int)pactivetext);

        if(MMI_RESULT_FALSE == res)
        {
            break;
        }
    }
    #if 0
    if(0 == *pnum)
    {
        SCI_MEMSET(full_path, 0, MMI_FILE_PATH_LEN);
        SCI_MEMSET(wstr_full_path, 0, MMI_FILE_PATH_LEN);
        
        if(MMI_SNS_SINA == snsid)
        {
            sprintf((char *)full_path, WRE_SINA_ACTIVEINFO_FILE_FULL_PATH, friendid);
        }
        else if(MMI_SNS_TIWTTER == snsid)
        {
            sprintf((char *)full_path, WRE_TIWTTER_ACTIVEINFO_FILE_FULL_PATH, friendid);
        }
        else
        {
            sprintf((char *)full_path, WRE_FACEBOOK_ACTIVEINFO_FILE_FULL_PATH, friendid);
        }

        
        MMIAPICOM_StrToWstr(full_path, wstr_full_path);
        
        MMISNS_ParseFileGetActiveTextEx(wstr_full_path, pname, pnum, (unsigned long int)pactivetext[0]);
        
    }  
    #endif
    MMIAPIFILEARRAY_Destroy(&file_array);
    return MMI_RESULT_TRUE;

}
/*****************************************************************************/
//  Description :get account Number from wreapp login file 
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_PBGetAccountNumber(IMMISns *pIMMISns, int snsid, uint8 *pNumber)
{
    char         *pUserIdStr        = PNULL;
    char         *pTempUserId       = PNULL;
    char         *pbuf              = PNULL;
    MMI_RESULT_E  res               = MMI_RESULT_FALSE;
    const char   *account_start_str = "account";
    const uint16 full_path_arr[MMI_FILE_PATH_LEN]= {0};

    if(PNULL == pNumber)
    {
        return res; 
    }
    
    if(MMI_SNS_SINA == snsid)
    {
        MMIAPICOM_StrToWstr((uint8 *)WRE_SINA_ACCOUNT_PATH, (wchar *)full_path_arr);
    }
    else if(MMI_SNS_TIWTTER == snsid)
    {
        MMIAPICOM_StrToWstr((uint8 *)WRE_TIWTTER_ACCOUNT_PATH, (wchar *)full_path_arr);
    }
    else
    {
        MMIAPICOM_StrToWstr((uint8 *)WRE_FACEBOOK_ACCOUNT_PATH, (wchar *)full_path_arr);
    }
    
    
    res = MMISNS_ReadSNSAccountFile((uint16 *)full_path_arr, &pbuf);

    if(MMI_RESULT_FALSE == res)
    {
        if(pbuf)
        {
            SCI_FREE(pbuf);
            pbuf = PNULL;
        }
        return res;    
    }

    pUserIdStr = STRSTR(pbuf, account_start_str);

    if(pUserIdStr)
    {
        pUserIdStr += SCI_STRLEN(account_start_str);
        pUserIdStr +=4; // ":"

        pTempUserId = (char *)pNumber;
        
        while(*pUserIdStr != 0x22)
        {
           *pTempUserId = *pUserIdStr;
            pUserIdStr ++;
            pTempUserId++;
        }
        
    }

    SCI_FREE(pbuf);
    pbuf = PNULL;
    
    return MMI_RESULT_TRUE;    
}

/*****************************************************************************/
//  Description :get account user id from wreapp login file 
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISNS_GetAccountUserId(int snsid, uint8 *pUserId)
{
    char         *pUserIdStr     = PNULL;
    char         *pTempUserId    = PNULL;
    char         *pbuf           = PNULL;
    MMI_RESULT_E  res            = MMI_RESULT_FALSE;
    const char *userid_start_str = "user_id";
    const uint16 full_path_arr[MMI_FILE_PATH_LEN]= {0};

    if(PNULL == pUserId)
    {
        return res; 
    }
    
    if(MMI_SNS_SINA == snsid)
    {
        MMIAPICOM_StrToWstr((uint8 *)WRE_SINA_ACCOUNT_PATH, (wchar *)full_path_arr);
    }
    else if(MMI_SNS_TIWTTER == snsid)
    {
        MMIAPICOM_StrToWstr((uint8 *)WRE_TIWTTER_ACCOUNT_PATH, (wchar *)full_path_arr);
    }
    else
    {
        MMIAPICOM_StrToWstr((uint8 *)WRE_FACEBOOK_ACCOUNT_PATH, (wchar *)full_path_arr);
    }
    
    
    res = MMISNS_ReadSNSAccountFile((uint16 *)full_path_arr, (char **)&pbuf);

    if(MMI_RESULT_FALSE == res)
    {
        if(pbuf)
        {
            SCI_FREE(pbuf);
            pbuf = PNULL;
        }
        return res;    
    }

    pUserIdStr = STRSTR(pbuf, userid_start_str);

    if(pUserIdStr)
    {
        pUserIdStr += SCI_STRLEN(userid_start_str);
        pUserIdStr +=4; // ":"

        pTempUserId = (char *)pUserId;
        
        while(*pUserIdStr != 0x22)
        {
           *pTempUserId = *pUserIdStr;
            pUserIdStr ++;
            pTempUserId++;
        }
        
    }

    SCI_FREE(pbuf);
    pbuf = PNULL;
    res  =  MMI_RESULT_TRUE;
    
    return res;    
}

/*****************************************************************************/
//  Description : get login data
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISNS_GetLogInData(int snsid, BOOLEAN *pLogIn)
{
    uint8        *pUserIdStr      = PNULL;
    uint8        *pTokenStr       = PNULL;
    uint8        *pbuf            = PNULL;
    MMI_RESULT_E  res             = MMI_RESULT_FALSE;
    char        *userid_start_str = "user_id";
    char        *token_start_str  = "token_token";
    uint16 full_path_arr[MMI_FILE_PATH_LEN]= {0};
    
    if(PNULL == pLogIn)
    {
        //*pLogIn = 0;
        return MMI_RESULT_FALSE;
    }

    if(MMI_SNS_SINA == snsid)
    {
        MMIAPICOM_StrToWstr((uint8 *)WRE_SINA_ACCOUNT_PATH, (wchar *)full_path_arr);
    }
    else if(MMI_SNS_TIWTTER == snsid)
    {
        MMIAPICOM_StrToWstr((uint8 *)WRE_TIWTTER_ACCOUNT_PATH, (wchar *)full_path_arr);
    }
    else
    {
        MMIAPICOM_StrToWstr((uint8 *)WRE_FACEBOOK_ACCOUNT_PATH, (wchar *)full_path_arr);
    }

    res = MMISNS_ReadSNSAccountFile((uint16 *)full_path_arr, (char **)&pbuf);

    if(MMI_RESULT_FALSE == res)
    {
        if(pbuf)
        {
            SCI_FREE(pbuf);
            pbuf = PNULL;
            *pLogIn = 0;
        }
        return res;    
    }
    
    pUserIdStr = (uint8 *)STRSTR((char *)pbuf, (char *)userid_start_str);
    pTokenStr  = (uint8 *)STRSTR((char *)pbuf, (char *)token_start_str);


    if(PNULL == pUserIdStr || PNULL == pTokenStr)
    {
        *pLogIn = 0;
    }
    else
    {
        *pLogIn = 1;
    }

    SCI_FREE(pbuf);
    pbuf = PNULL;
    res  =  MMI_RESULT_TRUE;
    
    return res;    
}

/*****************************************************************************/
//  Description : check user whether log in
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_CheckUserLogIn(IMMISns *pIMMISns, int snsid, uint32 infoaddr)
{
    BOOLEAN              bLogIn         = MMI_RESULT_FALSE;
    MMI_RESULT_E         res            = MMI_RESULT_FALSE;
    
    if(PNULL == pIMMISns || 0 == infoaddr)
    {
        return MMI_RESULT_FALSE;
    }
    
    switch(snsid)
    {
        case MMI_SNS_FACEBOOK:
        case MMI_SNS_TIWTTER:
        case MMI_SNS_SINA:
        {
            res = MMISNS_GetLogInData(snsid, &bLogIn);
            break;
        }
        default:
            break;

    }


    *((uint8 *)infoaddr) = bLogIn;
    
    
    return res; 

}
/*****************************************************************************/
//  Description :  user log in now
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_WidgetUserLogIn(IMMISns *pIMMISns, int snsid)
{
    CMMISns             *pMe            = PNULL;
    uint8                temp[16]       = {0};
    
    if(PNULL == pIMMISns || snsid >= MMI_SNS_MAX)
    {
        return MMI_RESULT_FALSE;
    }
    

    pMe = (CMMISns *)pIMMISns;

    pMe->data.commond_type                  = MMI_SNS_COMMOND_WIDGET;
    pMe->data.u.widget_data.widget_commond  = MMI_WIDGET_SNS_COMMOND_LOGIN;
    //SCI_TRACE_LOW:"renwei-MMISNS_WidgetUserLogIn commond_type= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_2067_112_2_18_2_55_48_45,(uint8*)"d",pMe->data.commond_type);
    //SCI_TRACE_LOW:"renwei-MMISNS_WidgetUserLogIn widget_commond= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_2068_112_2_18_2_55_48_46,(uint8*)"d",pMe->data.u.widget_data.widget_commond);
    
    sprintf((char *)temp, "SNS%d", (int)&pMe->data);
        
    MMIWRE_StartAppEx(g_SNSId[snsid].wre_sns_id, temp);
    
    return MMI_RESULT_TRUE; 

}

/*****************************************************************************/
//  Description : save log info to nv
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_SaveWreAppLogInfo(int wreapp_id, int commond_result)
{
    T_SNSClientRegInfo     ClientRegInfo = {0};
    MN_RETURN_RESULT_E     nv_return     = MN_RETURN_SUCCESS;
    
    nv_return  = MMI_ReadNVItem(MMINV_SNS_STRUCT_PART, &ClientRegInfo);

    if(MN_RETURN_SUCCESS != nv_return)
    {
        MMI_WriteNVItem(MMINV_SNS_STRUCT_PART, &ClientRegInfo);
    }

    ClientRegInfo.data[wreapp_id].bLogIn = commond_result;

    //SCI_TRACE_LOW:"renwei-MMISNS_SaveWreAppLogInfo  commond_result =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_2096_112_2_18_2_55_48_47,(uint8*)"d", commond_result);
    MMI_WriteNVItem(MMINV_SNS_STRUCT_PART, &ClientRegInfo);

    return MMI_RESULT_TRUE; 
}

/*****************************************************************************/
//  Description : send evt to these app to release memory
//  Global resource dependence : 
//  Author: guodong.liu
//  Note: 
/*****************************************************************************/
/*LOCAL void  MMISNS_SendEvtToAppToReleaseMem (T_WIDGET_EVT_RESULT evt_res)
{
     if(MMK_IsOpenWin(MMIBROWSER_MAIN_POPMENU_WIN_ID))
   {
        MMK_SendMsg(MMIBROWSER_MAIN_POPMENU_WIN_ID, MSG_SNS_OPERA_RESULT, &evt_res);
   }
     else if(MMK_IsOpenWin(MMIBROWSER_BOOKMARK_POPMENU_WIN_ID))
  {
        MMK_SendMsg(MMIBROWSER_BOOKMARK_POPMENU_WIN_ID, MSG_SNS_OPERA_RESULT, &evt_res);
  }
    else if(MMK_IsOpenWin(MMIDC_SEND_WIN_ID))
    {
        MMK_SendMsg(MMIDC_SEND_WIN_ID, MSG_SNS_OPERA_RESULT, &evt_res);
    }      
    else if(MMK_IsOpenWin(MMIMMS_SAVE_FILE_MENU_WIN_ID))
    {
        MMK_SendMsg(MMIMMS_SAVE_FILE_MENU_WIN_ID, MSG_SNS_OPERA_RESULT, &evt_res);
    }          
    else if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_OPT_WIN_ID))
    {
        MMK_SendMsg(MMIPICVIEW_PREVIEW_OPT_WIN_ID, MSG_SNS_OPERA_RESULT, &evt_res);
    }    
    else if(MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID))
    {
        MMK_SendMsg(MMISMS_MSGBOX_MAIN_WIN_ID, MSG_SNS_OPERA_RESULT, &evt_res);  
    } 

}*/

/*****************************************************************************/
//  Description : forward event to mocor app
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
PUBLIC void MMISNS_SendEvtToApp(int appid, int wreapp_id, int commond_type, int commond_result)
{
    MMI_HANDLE_T        handle  = 0;
    T_WIDGET_EVT_RESULT evt_res ={0};
    
    if(appid >= SNS_MODULE_MAX)
    {
        return;
    }

    SCI_TRACE_LOW("renwei --MMISNS_SendEvtToApp appid =%d wreapp_id =%d commond_type =%d",appid, wreapp_id, commond_type); 
    SCI_TRACE_LOW("renwei --MMISNS_SendEvtToApp commond_result =%d", commond_result); 
    switch(appid)
    {
#if 0    
        case SNS_MODULE_IMAGE_BROWSER:
        {
#ifdef PIC_VIEWER_SUPPORT
            handle =  MMIPICVIEW_PREVIEW_OPT_WIN_ID;
#endif
            break;
        }
        case SNS_MODULE_MMS:
        {
            handle =  MMIMMS_SAVE_FILE_MENU_WIN_ID;
            break;
        }
        case SNS_MODULE_SMS:
        {
            handle =  MMISMS_MSGBOX_MAIN_WIN_ID;       
            break;
        }
        case SNS_MODULE_CAMERA:
        {
            //handle =  MMIDC_SEND_WIN_ID;
            break;
        }
        case SNS_MODULE_BROWSER:
        {
#ifdef BROWSER_SUPPORT_DORADO
                handle =  MMIBROWSER_MAIN_POPMENU_WIN_ID;
#endif
                break;
        }
#endif        
        case SNS_MODULE_SNS_WREAPP:
        case SNS_MODULE_SNS_WIDGET:
        {
            if(MMI_SNS_SINA == wreapp_id)
            {
                handle= WIDGET_SNS_SINA_WIN_ID;
            }
            else if(MMI_SNS_FACEBOOK == wreapp_id)
            {
                handle= WIDGET_SNS_FACEBOOK_WIN_ID;
            }
            else if(MMI_SNS_TIWTTER == wreapp_id)
            {
                handle= WIDGET_SNS_TIWTTER_WIN_ID;
            }
            else
            {
                return;
            }
            
            break;
        }
//	    case SNS_MODULE_SNS_PB:
//	    {
//	        return ;
//	    }
        default:
        {
            break;    
        }
        
    }

    if(SNS_MODULE_SNS_WIDGET == appid || SNS_MODULE_SNS_WREAPP == appid)
    {
            evt_res.wreapp_id      = wreapp_id;
            evt_res.commond_type   = commond_type;
            evt_res.commond_result = commond_result;
            SCI_TRACE_LOW("renwei --MMISNS_SendEvtToApp wreapp_id =%d commond_type =%d ,result =%d ", wreapp_id, commond_type,commond_result); 
            MMK_SendMsg(handle, MSG_SNS_OPERA_RESULT, &evt_res);

            if(WRE_COMMOND_LOGIN == commond_type)
            {
               SCI_TRACE_LOW("[MMISNS]MMISNS_SendEvtToApp MMIPB_SendSignalToPBTask start MMIPB_SIG_SNS_UPDATE");            
               MMIPB_SetPBStatus(MMIPB_STATUS_SORTING);
               MMIPB_SendSignalToPBTask(MMIPB_SIG_SNS_UPDATE, 0);  //yanyan.an modify for cr NEWMS00232599
            }
    }
    else if(SNS_MODULE_SNS_PB == appid)
    {   
        if(g_pIMMISns)
        {
            MMISNS_Release(g_pIMMISns);
            g_pIMMISns = PNULL;
        }
        
        evt_res.commond_type   = commond_type;
        evt_res.commond_result = commond_result;

        if(GOTO_CARE_LIST == commond_type)
        {
            SCI_TRACE_LOW("[MMISNS]MMISNS_SendEvtToApp commond_type == GOTO_CARE_LIST");
            MMISNS_InitPBDataBySNSId(wreapp_id);
            MMIPB_SetPBStatus(MMIPB_STATUS_SORTING);
            MMIPB_SendSignalToPBTask(MMIPB_SIG_SNS_UPDATE, 0);
        }
        else if(GOTO_USER_DETAIL_LIST == commond_type)
        {
            SCI_TRACE_LOW("[MMISNS]MMISNS_SendEvtToApp commond_type == GOTO_USER_DETAIL_LIST");
            MMIPB_SendSignalToPBTask(MMIPB_SIG_SNS_READ_ACTIVE, 0);
        }
        else
        {
            SCI_TRACE_LOW("renwei --MMISNS_SendEvtToApp SNS_PB  commond_type ERROR "); 
        }
    }    
    else
    {
  //      MMISNS_SendEvtToAppToReleaseMem ( evt_res);
        if(g_pIMMISns)
        {
            SCI_TRACE_LOW("renwei --MMISNS_SendEvtToApp g_pIMMISns =%d", g_pIMMISns);         
            MMISNS_Release(g_pIMMISns);
            g_pIMMISns = PNULL;
        }
    }
    
}

/*****************************************************************************/
//  Description : read Sns Friends file
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISNS_ReadSnsFriendsFile(FILEARRAY  file_array, int fileindex, int snsid)
{
    uint8           *buf_ptr                         = PNULL;
    uint8           *p1                              = PNULL;
    uint8           *p3                              = PNULL;
	uint8            type                            = 0;
    uint8            userid[MMI_USER_ID_LEN+1]          = {0};
    uint8            name[MMI_NAME_LEN+1]            = {0};
    uint8            photopath[MMI_PHOTO_PATH_LEN+1] = {0};
    uint8            email[MMI_EMAIL_LEN+1]          = {0}; 
    uint8            birthday[MMI_BIRTHDAY_LEN+1]    = {0}; 
    uint8            brief[MMI_TEXT_LEN+1]           = {0};
    FILEARRAY_DATA_T array_item                      = {0};
    MMI_RESULT_E     res                             = MMI_RESULT_FALSE;

    if(PNULL == file_array)
    {
        return res;
    }
    
    type = (uint8)snsid;
    MMIAPIFILEARRAY_Read(file_array, (uint32)fileindex, &array_item );
    
    buf_ptr = SCI_ALLOCA(array_item.size);
    
    if (PNULL == buf_ptr)
    {
        return res;
    }
    
    SCI_MEMSET(buf_ptr, 0x00, array_item.size);

    if(MMIAPIFMM_ReadFilesDataSyn((const uint16 *)array_item.filename, array_item.name_len, buf_ptr, array_item.size))
    {
        uint32                j         = 0;
        MMISNS_SQLITE_DATA_T  dbData    ={0};
        
        p1 = buf_ptr;
        p3 = p1;
        
        MMISNS_Sqlite_Create();
        MMISNS_Sqlite_Transaction(PNULL, TRUE);
        
        while(p3 < buf_ptr+ array_item.size)
        {
            j  = 0;
            p3 = p1;

            // get user id 
            while(*p3 != '\0' && j < array_item.size && j < MMI_USER_ID_LEN)
            {   
                userid[j] = *p3;
                p3++;
                j++; 
            }

            dbData.user_id = (char *)userid;

            if(*p3 != '\0')
            {
                while(*p3 != '\0' && j < array_item.size)
                {
                    p3++;
                    j++;
                }
            }
            
            p3 ++; //跳过\0
            
            // get name
            j  = 0;
            while(*p3 != '\0' && j < array_item.size && j < MMI_NAME_LEN)
            {
                name[j] = *p3;
                
                p3++;
                j++; 
            }

            dbData.name = (char *)name;

            if(*p3 != '\0')
            {
                while(*p3 != '\0' && j < array_item.size)
                {
                    p3++;
                    j++;
                }
            }
            
            p3 ++; //跳过\0

            // get photo path
            j = 0;
            while(*p3 != '\0' && j < array_item.size && j < MMI_PHOTO_PATH_LEN)
            {
                photopath[j] = *p3;
                
                p3++;
                j++; 
            }
            
            dbData.file_path = (char *)photopath;
            
            if(*p3 != '\0')
            {
                while(*p3 != '\0' && j < array_item.size)
                {
                    p3++;
                    j++;
                }
            }
            
            p3 ++; //跳过\0
            
            // get email
            j = 0;
            if('\0' == *p3)
            {
                dbData.email = "\0";
            }
            else
            {
                while(*p3 != '\0' && j < array_item.size && j < MMI_EMAIL_LEN)
                {
                    email[j] = *p3;
                    
                    p3++;
                    j++; 
                }
                
                dbData.email = (char *)email;
            }

            if(*p3 != '\0')
            {
                while(*p3 != '\0' && j < array_item.size)
                {
                    p3++;
                    j++;
                }
            }
            
            p3 ++; //跳过\0  

            // get birthday
            j = 0;
            if('\0' == *p3)
            {
                dbData.birth = "\0";
            }
            else
            {
                while(*p3 != '\0' && j < array_item.size && j < MMI_BIRTHDAY_LEN)
                {
                    birthday[j] = *p3;
                    
                    p3++;
                    j++; 
                }
                
                dbData.birth = (char *)birthday;
            }

            if(*p3 != '\0')
            {
                while(*p3 != '\0' && j < array_item.size)
                {
                    p3++;
                    j++;
                }
            }
            
            p3 ++; //跳过\0

            // get brief
           
            j = 0;
            while(*p3 != '\0' && j < array_item.size && j < MMI_TEXT_LEN)
            {
                brief[j] = *p3;
                
                p3++;
                j++; 
            }

            dbData.sign = (char *)brief;

            if(*p3 != '\0')
            {
                while(*p3 != '\0' && j < array_item.size)
                {
                    p3++;
                    j++;
                }
            }

            p3 ++; //跳过\0  
            
            // get type
            dbData.type = (char *)&type;

            
            p3 += 3;//SCI_STRLEN(SINA_SNS_SPLIT_MARK);
            p1  = p3;

            MMISNS_Sqlite_AddItem(PNULL, &dbData);

            SCI_MEMSET(userid, 0x00, MMI_USER_ID_LEN);    
            SCI_MEMSET(name, 0x00, MMI_NAME_LEN);
            SCI_MEMSET(photopath, 0x00, MMI_PHOTO_PATH_LEN);
            SCI_MEMSET(email, 0x00, MMI_EMAIL_LEN);
            SCI_MEMSET(birthday, 0x00, MMI_BIRTHDAY_LEN);
            SCI_MEMSET(brief, 0x00, MMI_TEXT_LEN);
        }
        
        MMISNS_Sqlite_Transaction(PNULL, FALSE);
        MMISNS_Sqlite_Destroy();
    }
    
    SCI_FREE(buf_ptr);

    return MMI_RESULT_TRUE; 
}

/*****************************************************************************/
//  Description : Init PB sns friends
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_InitPBDataBySNSId(int snsid)
{
    uint32            i                                 = 0;
    uint32            file_num                          = 0;
    FILEARRAY         file_array                        = {0};
    uint8             UserId[MMI_USERID_LEN]            = {0};
    uint8             *full_path                        = PNULL; 
    uint16            *wstr_full_path                   = PNULL; 
    MMI_RESULT_E      res                               = MMI_RESULT_FALSE;
    MMIFMM_FILTER_T   filter                            = {{"*"}};
   

    if(snsid >= MMI_SNS_MAX)
    {
        return res;
    }

    res = MMISNS_GetAccountUserId(snsid, (uint8 *)&UserId);  
    
    if(MMI_RESULT_FALSE == res || 0 == SCI_STRLEN((char *)UserId))    
    {
        return res;
    }

    full_path = SCI_ALLOCA((MMI_FILE_PATH_LEN + 1) * sizeof(uint8));
    SCI_MEMSET(full_path, 0x00, (MMI_FILE_PATH_LEN + 1) * sizeof(uint8));    
    wstr_full_path = SCI_ALLOCA((MMI_FILE_PATH_LEN + 1) * sizeof(uint16));
    SCI_MEMSET(wstr_full_path, 0x00, (MMI_FILE_PATH_LEN + 1) * sizeof(uint16));    

    if(MMI_SNS_SINA == snsid)
    {
        sprintf((char *)full_path, WRE_SINA_FRIENDS_FILE_FULL_PATH, UserId);
    }
    else if(MMI_SNS_TIWTTER == snsid)
    {
        sprintf((char *)full_path, WRE_TIWTTER_FRIENDS_FILE_FULL_PATH, UserId);
    }
    else
    {
        sprintf((char *)full_path, WRE_FACEBOOK_FRIENDS_FILE_FULL_PATH, UserId);
    }
    
    
    file_array = MMIAPIFILEARRAY_Create();

    MMIAPICOM_StrToWstr(full_path, wstr_full_path);
    
    MMIAPIFMM_SearchFileInPath(wstr_full_path, 
                            MMIAPICOM_Wstrlen(wstr_full_path),
                            &filter,
                            FALSE,
                            FUNC_FIND_FILE,
                            file_array,
                            0,
                            0);

    file_num  = MMIAPIFILEARRAY_GetArraySize(file_array);

    if(0 != file_num)
    {
        for(i = 0 ; i < file_num; i++)
        {
            MMISNS_ReadSnsFriendsFile(file_array, i, snsid);
        }
    }
       
    MMIAPIFILEARRAY_Destroy(&file_array);
    SCI_FREE(full_path); 
    SCI_FREE(wstr_full_path);
    return MMI_RESULT_TRUE;

}


/*****************************************************************************/
//  Description : pb get sns friends
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
PUBLIC int MMISNS_UpDatePBFriends(int snsid)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
#if 0    
    if(snsid >= MMI_SNS_MAX)
    {
        return MMI_RESULT_FALSE;
    }

    switch(snsid)
    {
        case MMI_SNS_FACEBOOK:
        case MMI_SNS_TIWTTER:
        case MMI_SNS_SINA:
        {
            res = MMISNS_InitPBDataBySNSId(snsid);
            break;
        }
        default:
        {
            break;
        }   

    }

#else
    uint8    temp[16] = {0};
    CMMISns  *pMe      = PNULL;

    if(snsid >= MMI_SNS_MAX)
    {
        return MMI_RESULT_FALSE;
    }

    
    pMe = (CMMISns *)SCI_ALLOCA(sizeof(CMMISns));
    SCI_TRACE_LOW("[MMISNS]UpDatePBFriends ALLOC");
 
    if(!pMe)
    {
        return MMI_RESULT_FALSE; 
    }

    SCI_MEMSET(pMe, 0, sizeof(CMMISns));

    g_pIMMISns  = pMe;
    
    pMe->pvt    = &gvtMMISns;
    pMe->m_nRef = 1;

    
    pMe->data.commond_type          = MMI_SNS_COMMOND_PB_GET_DATA;
    pMe->data.u.pb_data.pb_commond  = GOTO_CARE_LIST;

    sprintf((char *)temp, "SNS%d", (int)&pMe->data);
    SCI_TRACE_LOW("g_SNSId[%d].wre_sns_id = %d, temp = %d", snsid, g_SNSId[snsid].wre_sns_id, temp);
    MMIWRE_StartAppEx(g_SNSId[snsid].wre_sns_id, temp);

    //SCI_FREE(pMe);
#endif
    return res;
}


/*****************************************************************************/
//  Description : Open Sqlite Database
//  Global resource dependence : 
//  Author: CBK
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_CreateSNSDB(IMMISns *pIMMISns)
{
    if(PNULL == pIMMISns)
    {
        return MMI_RESULT_FALSE;
    }
    
    return MMISNS_Sqlite_Create();
}

/*****************************************************************************/
//  Description : Search SNS contact from sns database
//  Global resource dependence : 
//  Author: CBK
//  Note: from start ID to End ID which in infoaddr
/*****************************************************************************/
LOCAL int MMISNS_GetDataByDb(IMMISns *pIMMISns, uint32 infoaddr)
{
    CMMISns               *pMe         = PNULL;    
    MMISNS_SQLITE_RESULT *dbDataHandle = PNULL;
    
    if(PNULL == pIMMISns || 0 == infoaddr)
    {
        return MMI_RESULT_FALSE;
    }

    pMe            = (CMMISns *)pIMMISns;
    dbDataHandle = (MMISNS_SQLITE_RESULT*)infoaddr;

    return MMISMS_Sqlite_GetItemByID(dbDataHandle);
}

/*****************************************************************************/
//  Description : DestroySNSData base
//  Global resource dependence : 
//  Author: CBK
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_DestroySNSDB(IMMISns *pIMMISns)
{
    if(PNULL == pIMMISns)
    {
        return MMI_RESULT_FALSE;
    }
    
    MMISNS_Sqlite_Destroy();
    
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : DestroySNSData base
//  Global resource dependence : 
//  Author: CBK
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_GetNumByDbType(IMMISns *pIMMISns, uint32 infoaddr, uint32 etype)
{
    if(PNULL == pIMMISns)
    {
        return MMI_RESULT_FALSE;
    }

    return MMISMS_Sqlite_GetNumByType((void *)infoaddr, (void *)etype);

}

/*****************************************************************************/
//  Description : Search SNS contact from sns database by name
//  Global resource dependence : 
//  Author: CBK
//  Note: from start ID to End ID which in infoaddr
/*****************************************************************************/
LOCAL int MMISNS_GetDataByDbName(IMMISns *pIMMISns, uint32 infoaddr)
{
    CMMISns               *pMe            = PNULL;    
    MMISNS_SQLITE_RESULT *dbDataHandle = PNULL;
    
    if(PNULL == pIMMISns || 0 == infoaddr)
    {
        return MMI_RESULT_FALSE;
    }

    pMe            = (CMMISns *)pIMMISns;
    dbDataHandle = (MMISNS_SQLITE_RESULT*)infoaddr;

    //SCI_TRACE_LOW:"MMISNS_GetDataByDbName   !!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SRV_2555_112_2_18_2_55_49_49,(uint8*)"");

    return MMISMS_Sqlite_GetItemByName(dbDataHandle);
 
}

/*****************************************************************************/
//  Description : get SNS contact total num from sns database 
//  Global resource dependence : 
//  Author: CBK
//  Note: from start ID to End ID which in infoaddr
/*****************************************************************************/
LOCAL int MMISNS_GetTotalNumByDb(IMMISns *pIMMISns, uint32 infoaddr)
{
    if(PNULL == pIMMISns)
    {
        return MMI_RESULT_FALSE;
    }

    return MMISNS_Sqlite_GetTotalNum((void *)infoaddr);

}    

/*****************************************************************************/
//  Description : get SNS contact data from sns database by type
//  Global resource dependence : 
//  Author: CBK
//  Note: from start ID to End ID which in infoaddr
/*****************************************************************************/
LOCAL int MMISNS_GetDataByDbType(IMMISns *pIMMISns, uint32 infoaddr, uint32 etype)
{
    if(PNULL == pIMMISns)
    {
        return MMI_RESULT_FALSE;
    }

    return MMISMS_Sqlite_GetItemByType((void *)infoaddr, (void *)etype);
}    

/*****************************************************************************/
//  Description : MMISNS_IsDbOpen
//  Global resource dependence : 
//  Author: CBK
//  Note: from start ID to End ID which in infoaddr
/*****************************************************************************/
LOCAL int MMISNS_IsDbOpen(IMMISns *pIMMISns)
{
    if(PNULL == pIMMISns)
    {
        return MMI_RESULT_FALSE;
    }

    return MMISNS_Sqlite_isDBOpen();
 
} 
/*****************************************************************************/
//  Description : reset all user parameter which related to base
//  Global resource dependence : 
//  Author: CBK
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_ResetDBUerPara(IMMISns *pIMMISns, uint32 infoaddr)
{
    if(PNULL == pIMMISns || PNULL == infoaddr)
    {
        return MMI_RESULT_FALSE;
    }
    
    return MMISNS_Sqlite_ResetUserPara((void **)infoaddr);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: wei.ren
//  Note: 
/*****************************************************************************/
LOCAL int MMISNS_PBGotoActiveWin(IMMISns *pIMMISns, int snsid, unsigned long int userid)
{
    CMMISns   *pMe            = PNULL;
    uint8      temp[16]       = {0};
    char      *puserid        = PNULL;
    SCI_TRACE_LOW("[MMISNS]MMISNS_PBGotoActiveWin snsid = %d, userid = %d", snsid, userid);
    if(PNULL == pIMMISns || snsid >= MMI_SNS_MAX || PNULL == userid)
    {
        return MMI_RESULT_FALSE;
    }
    
    puserid = (char  *)userid;
    
    if(SCI_STRLEN(puserid) > PB_SNS_USER_ID_LEN)
    {
        return MMI_RESULT_FALSE;
    }
    
    g_pIMMISns             = pIMMISns;
    pMe = (CMMISns *)pIMMISns;

    pMe->data.commond_type          = MMI_SNS_COMMOND_PB_GET_DATA;
    pMe->data.u.pb_data.pb_commond  = GOTO_USER_DETAIL_LIST; 

    SCI_STRCPY(pMe->data.u.pb_data.u.user_data.id, puserid);

    sprintf((char *)temp, "SNS%d", (int)&pMe->data);
    SCI_TRACE_LOW("[MMISNS] MMIWRE_StartAppEx start entry");
    MMIWRE_StartAppEx(g_SNSId[snsid].wre_sns_id, temp);
    
    return MMI_RESULT_TRUE; 

}

#ifdef SNS_PULLING_SUPPORT
/*****************************************************************************/
//  Description : set sns widget command data
//  Global resource dependence : 
//  Author: david chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMISNS_SetWidgetCommandData(IMMISns *pMMISns, E_MMISNS_WIDGET_COMMOND_TYPE type)
{
    CMMISns *pMe = (CMMISns *)pMMISns;    

    pMe->data.commond_type                  = MMI_SNS_COMMOND_WIDGET;
    pMe->data.u.widget_data.widget_commond  = type;
}

/*****************************************************************************/
//  Description : get sns widget command data
//  Global resource dependence : 
//  Author: david chen
//  Note: 
/*****************************************************************************/
PUBLIC int MMISNS_GetWidgetCommandData(IMMISns *pMMISns)
{
    CMMISns *pMe = (CMMISns *)pMMISns;    

    return (int)&(pMe->data);
}

/*****************************************************************************/
//  Description : save sns widget setting data to NV
//  Global resource dependence : 
//  Author: david chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISNS_SaveWidgetTimerSetting(int snsid, uint16 timer_type)
{
    T_SNSClientRegInfo     ClientRegInfo = {0};
    MN_RETURN_RESULT_E     nv_return     = MN_RETURN_SUCCESS;

    if(snsid >= MMI_SNS_MAX)
    {
        return FALSE;
    }    
    
    nv_return  = MMI_ReadNVItem(MMINV_SNS_STRUCT_PART, &ClientRegInfo);

    if(MN_RETURN_SUCCESS != nv_return)
    {
        MMI_WriteNVItem(MMINV_SNS_STRUCT_PART, &ClientRegInfo);

        return FALSE;
    }

    ClientRegInfo.data[snsid].eSNSTimerType = timer_type;

    MMI_WriteNVItem(MMINV_SNS_STRUCT_PART, &ClientRegInfo);

    return TRUE;
}

/*****************************************************************************/
//  Description : load sns widget setting data from NV
//  Global resource dependence : 
//  Author: david chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISNS_LoadWidgetTimerSetting(int snsid, uint16 *timer_type_ptr)
{
    T_SNSClientRegInfo     ClientRegInfo = {0};
    MN_RETURN_RESULT_E     nv_return     = MN_RETURN_SUCCESS;

    if((snsid >= MMI_SNS_MAX) || (PNULL == timer_type_ptr))
    {
        return FALSE;
    }    
    
    nv_return  = MMI_ReadNVItem(MMINV_SNS_STRUCT_PART, &ClientRegInfo);

    if(MN_RETURN_SUCCESS != nv_return)
    {
        MMI_WriteNVItem(MMINV_SNS_STRUCT_PART, &ClientRegInfo);
        return FALSE;
    }

    *timer_type_ptr = ClientRegInfo.data[snsid].eSNSTimerType;

    return TRUE;
}
#endif //SNS_PULLING_SUPPORT

#endif

