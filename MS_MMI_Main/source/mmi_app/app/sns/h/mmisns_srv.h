/***************************************************************************
** File Name:      mmisns_srv.h                                             *
** Author:                                                                 *
** Date:           4/01/2009                                               *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 4/2009         renwei     Create                                  *
**																		   *
****************************************************************************/
#ifdef SNS_SUPPORT
#ifndef _MMISNS_SRV_H_
#define _MMISNS_SRV_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef _cplusplus
	extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define MMI_PB_ACTIVE_TEXT_NUM    20
#define MMI_NAME_LEN              20 *2
#define MMI_USER_ID_LEN           50
#define MMI_DATE_LEN              20 *2
#define MMI_TEXT_LEN              300
#define MMI_WIDGET_TEXT_NUM       10
#define MMI_PHOTO_PATH_LEN        255
#define MMI_EMAIL_LEN             320
#define MMI_BIRTHDAY_LEN          MMI_DATE_LEN
/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

#ifdef SNS_PULLING_SUPPORT
typedef enum
{
    MMI_SNS_TIMER_PRESET_OFF = 0,
    MMI_SNS_TIMER_PRESET_5MIN, 
    MMI_SNS_TIMER_PRESET_15MIN, 
    MMI_SNS_TIMER_PRESET_30MIN,
    MMI_SNS_TIMER_PRESET_60MIN,
    MMI_SNS_TIMER_PRESET_MAX
}E_MMISNS_TIMER_PRESET_TYPE;
#endif //SNS_PULLING_SUPPORT

typedef enum
{
    MMI_SNS_DATA_TEXT,
    MMI_SNS_DATA_PIC,    
    MMI_SNS_DATA_TEXT_PIC,
    MMI_SNS_DATA_MAX
}E_MMISNS_DATA_TYPE;

typedef enum
{
    MMI_SNS_SINA,
    MMI_SNS_FACEBOOK,
    MMI_SNS_TIWTTER,
    MMI_SNS_MAX,
}E_MMISNS_TO_CLIENT_ID;
typedef struct
{
    int wreapp_id;
    int commond_type;
    int commond_result;

}T_WIDGET_EVT_RESULT;

typedef struct T_PbActiveData
{
    uint16  date[MMI_DATE_LEN+1];
    uint16  text[MMI_TEXT_LEN+1];
}PbActiveData;

typedef struct T_WIDGET_DATA
{
    int            index;                  //Ë÷ÒýÖµ
    wchar          name[MMI_NAME_LEN+1];   //ÐÕÃû
    wchar          date[MMI_DATE_LEN+1];   //Ê±¼ä
    wchar          text[MMI_TEXT_LEN+1];//ÎÄ±¾ÄÚÈÝ
}T_WIDGET_DATA;

typedef struct _SNSClientInfoData
{
    E_MMISNS_TO_CLIENT_ID       eSNSId;
    unsigned char               bSNSMenu;
    unsigned char               bSNSWidget;
    unsigned char               bLogIn;
    E_MMISNS_DATA_TYPE          eSNSSupport;
#ifdef SNS_PULLING_SUPPORT
    E_MMISNS_TIMER_PRESET_TYPE  eSNSTimerType; //Preset timer setting
#endif //SNS_PULLING_SUPPORT
}T_SNSClientInfoData;

typedef struct _SNSClientRegInfo
{
    int                 curr_num;
    T_SNSClientInfoData data[MMI_SNS_MAX];    
    
}T_SNSClientRegInfo;


typedef enum SNS_MODULE_ID
{
    SNS_MODULE_NOE = 0,
    SNS_MODULE_IMAGE_BROWSER,   //Í¼Æ¬ä¯ÀÀÆ÷
    SNS_MODULE_SMS,             //¶ÌÐÅ
    SNS_MODULE_MMS,             //²ÊÐÅ
    SNS_MODULE_CAMERA,          //ÅÄÕÕ
    SNS_MODULE_BROWSER,         //ä¯ÀÀÆ÷
    SNS_MODULE_SNS_WIDGET,      //SNS WIDGET
    SNS_MODULE_SNS_PB,          //PB ºÃÓÑ 
    SNS_MODULE_SNS_WREAPP,      //µÇÂ¼×´Ì¬, Êý¾Ý×´Ì¬
    SNS_MODULE_MAX,
}SNS_MODULE_ID;

typedef struct
{
    E_MMISNS_DATA_TYPE buf_type;
    uint32 mms_buff_len;
    uint8 *mms_buff;
}T_MMS_SHARE_TO_SNS;


typedef enum
{
    MMI_WIDGET_SNS_COMMOND_NEW = 0x11,
    MMI_WIDGET_SNS_COMMOND_GET_DATA,    
    MMI_WIDGET_SNS_COMMOND_ENTER_DETAIL,
    MMI_WIDGET_SNS_COMMOND_LOGIN,
    MMI_WIDGET_SNS_COMMOND_MAX

}E_MMISNS_WIDGET_COMMOND_TYPE;

typedef enum
{
    MMI_SNS_COMMOND_CHECK_ACCOUNT = 0xF1,
    MMI_SNS_COMMOND_PB_GET_DATA,
    MMI_SNS_COMMOND_WIDGET,
    MMI_SNS_COMMOND_SHARE_DATA,

    MMI_SNS_COMMOND_MAX,
}E_MMISNS_COMMOND_TYPE;


typedef enum
{
    WRE_COMMOND_LOGIN = 0xe1,
    WRE_COMMOND_WIDGET_DATA_MODIFY,
    WRE_COMMOND_PB_DATA_MODIFY,
    WRE_COMMOND_WIDGET_MAX
}E_WRE_COMMOND_TYPE;


typedef struct _IMMISns IMMISns;

typedef struct _IMMISnsVtbl  IMMISnsVtbl;

struct _IMMISns
{
    struct IMMISnsVtbl *pvt;
};



struct _IMMISnsVtbl
{
    int(*AddRef)(IMMISns *pIMMISns);
    int(*Release)(IMMISns *pIMMISns);
    int(*CheckAccount)(IMMISns *pIMMISns, int snsid, unsigned long int nameaddr, unsigned long int passwordaddr);
    int(*ShareToSns)(IMMISns *pIMMISns, int snsid, int appid, unsigned long int data);
    int(*Register)(IMMISns *pIMMISns, int snsid, unsigned long int infoaddr);
    int(*GetRegisterInfo)(IMMISns *pIMMISns, unsigned long int infoaddr);
    int(*IsFixAppInstalled)(IMMISns *pIMMISns, int snsid);
    int(*AppInstall)(IMMISns *pIMMISns, int snsid);
    int(*WidgetNewEdit)(IMMISns *pIMMISns, int snsid);
#ifdef SNS_PULLING_SUPPORT
    int(*WidgetRefresh)(IMMISns *pIMMISns, int snsid);
#endif //SNS_PULLING_SUPPORT    
    int(*WidgetCmdGetData)(IMMISns *pIMMISns, int snsid, int index, int direction,unsigned long int data_addr);
    int(*WidgetEnterTextDetail)(IMMISns *pIMMISns, int snsid, int index);
    //int(*WidgetGetData)(IMMISns *pIMMISns, int snsid, unsigned long int data_addr);
    int(*WidgetGetNum) (IMMISns *pIMMISns, int snsid, unsigned long int data_addr);
    int(*CheckUserLogIn) (IMMISns *pIMMISns, int snsid, unsigned long int data_addr);
    int(*WidgetUserLogIn)(IMMISns *pIMMISns, int snsid);
    int(*PBGetAccountNumber)(IMMISns *pIMMISns, int snsid, unsigned char *pNumber);
    int(*PBGetActiveInfo)(IMMISns *pIMMISns, int snsid, unsigned char *friendid, unsigned short *pname, unsigned char *pnum, PbActiveData  **data_addr);
    int(*CreateSNSDB)(IMMISns *pIMMISns); //Create/Open Database
    int(*GetDataByDb)(IMMISns *pIMMISns, unsigned long int infoaddr);     // Get all data from DB including sina, twitter, facebook
    int(*DestroySNSDB)(IMMISns *pIMMISns);  //destroy Database handle
    int(*GetNumByDbType)(IMMISns *pIMMISns, unsigned long int infoaddr, unsigned long int etype);  // get number from database by type(sina, twitter or facebook)
    int(*GetDataByDbName)(IMMISns *pIMMISns, unsigned long int infoaddr);  //get data from DB by name
    int(*GetTotalNumByDb)(IMMISns *pIMMISns, unsigned long int infoaddr);   // get total number in DB
    int(*GetDataByDbType)(IMMISns *pIMMISns, unsigned long int infoaddr, unsigned long int etype);  // get data from DB by  type(sina, twitter or facebook)
    int(*IsDbOpen)(IMMISns *pIMMISns);    // if DB open
    int(*ResetDBUerPara)(IMMISns *pIMMISns, unsigned long int infoaddr);  // clear all data parameter from UI side
    int(*PBGotoActiveWin)(IMMISns *pIMMISns, int snsid, unsigned long int userid);

};

#define IMMISNS_AddRef(p)   (*(IMMISnsVtbl **)p)->AddRef(p)
#define IMMISNS_Release(p)   (*(IMMISnsVtbl **)p)->Release(p)
#define IMMISNS_CheckAccount(p,snsid,nameaddr, passwordaddr) (*(IMMISnsVtbl **)p)->CheckAccount((p),(snsid),(nameaddr),(passwordaddr))
#define IMMISNS_ShareToSns(p,snsid, appid, p_data)   (*(IMMISnsVtbl **)p)->ShareToSns((p),(snsid),(appid),(p_data))
#define IMMISNS_Register(p,snsid, infoaddr)   (*(IMMISnsVtbl **)p)->Register((p),(snsid),(infoaddr))
#define IMMISNS_GetRegisterInfo(p, infoaddr)   (*(IMMISnsVtbl **)p)->GetRegisterInfo((p),(infoaddr))
#define IMMISNS_IsFixAppInstalled(p, snsid)   (*(IMMISnsVtbl **)p)->IsFixAppInstalled((p),(snsid))
#define IMMISNS_AppInstall(p, snsid)   (*(IMMISnsVtbl **)p)->AppInstall((p),(snsid))
#define IMMISNS_WidgetNewEdit(p, snsid)   (*(IMMISnsVtbl **)p)->WidgetNewEdit((p),(snsid))
#ifdef SNS_PULLING_SUPPORT
#define IMMISNS_WidgetRefresh(p, snsid)   (*(IMMISnsVtbl **)p)->WidgetRefresh((p),(snsid))
#endif //SNS_PULLING_SUPPORT
#define IMMISNS_WidgetCmdGetData(p, snsid, index, direction, data_addr)   (*(IMMISnsVtbl **)p)->WidgetCmdGetData((p),(snsid),(index),(direction),(data_addr))
#define IMMISNS_WidgetEnterTextDetail(p, snsid, index)   (*(IMMISnsVtbl **)p)->WidgetEnterTextDetail((p),(snsid),(index))
//#define IMMISNS_WidgetGetData(p, snsid, data_addr) (*(IMMISnsVtbl **)p)->WidgetGetData((p),(snsid),(data_addr))
#define IMMISNS_WidgetGetNum(p, snsid, data_addr)   (*(IMMISnsVtbl **)p)->WidgetGetNum((p),(snsid),(data_addr))
#define IMMISNS_CheckUserLogIn(p, snsid, data_addr)   (*(IMMISnsVtbl **)p)->CheckUserLogIn((p),(snsid),(data_addr))
#define IMMISNS_WidgetUserLogIn(p, snsid)   (*(IMMISnsVtbl **)p)->WidgetUserLogIn((p),(snsid))

#define IMMISNS_PBGetAccountNumber(p, snsid, pNumber)   (*(IMMISnsVtbl **)p)->PBGetAccountNumber((p),(snsid),(pNumber))
#define IMMISNS_PBGetActiveInfo(p, snsid, friendid, pname, pnum, data_addr)   (*(IMMISnsVtbl **)p)->PBGetActiveInfo((p),(snsid), (friendid), (pname),(pnum), (data_addr))

#define IMMISNS_CreateSNSDB(p) (*(IMMISnsVtbl **)p)->CreateSNSDB(p)
#define IMMISNS_GetDataByDb(p, infoaddr)   (*(IMMISnsVtbl **)p)->GetDataByDb((p),(infoaddr))
#define IMMISNS_DestroySNSDB(p)   (*(IMMISnsVtbl **)p)->DestroySNSDB(p)
#define IMMISNS_GetNumByDbType(p, infoaddr, etype)  (*(IMMISnsVtbl **)p)->GetNumByDbType((p),(infoaddr), (etype))
#define IMMISNS_GetDataByDbName(p, infoaddr)   (*(IMMISnsVtbl **)p)->GetDataByDbName((p),(infoaddr))
#define IMMISNS_GetTotalNumByDb(p, infoaddr)  (*(IMMISnsVtbl **)p)->GetTotalNumByDb((p),(infoaddr))
#define IMMISNS_GetDataByDbType(p, infoaddr, etype)  (*(IMMISnsVtbl **)p)->GetDataByDbType((p),(infoaddr), (etype))
#define IMMISNS_IsDbOpen(p)  (*(IMMISnsVtbl **)p)->IsDbOpen(p)
#define IMMISNS_ResetDBUerPara(p, infoaddr)  (*(IMMISnsVtbl **)p)->ResetDBUerPara((p),(infoaddr))

#define IMMISNS_PBGotoActiveWin(p, snsid, userid)  (*(IMMISnsVtbl **)p)->PBGotoActiveWin((p),(snsid),(userid))
void IMMISNS_CreateObject(IMMISns **ppMMISns);
void MMISNS_ModuleInit(void);
void MMISNS_SendEvtToApp(int appid, int wreapp_id, int commond_type, int commond_result);
PUBLIC void MMISNS_InitPBData(void);
PUBLIC int MMISNS_UpDatePBFriends(int snsid);

#ifdef SNS_PULLING_SUPPORT
PUBLIC void MMISNS_SetWidgetCommandData(IMMISns *pMMISns, E_MMISNS_WIDGET_COMMOND_TYPE type);
PUBLIC int MMISNS_GetWidgetCommandData(IMMISns *pMMISns);
PUBLIC BOOLEAN MMISNS_SaveWidgetTimerSetting(int sns_id, uint16 timer_type);
PUBLIC BOOLEAN MMISNS_LoadWidgetTimerSetting(int sns_id, uint16 *timer_type_ptr);
#endif //SNS_PULLING_SUPPORT

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/



/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif//_MMISNS_SRV_H_
#endif
