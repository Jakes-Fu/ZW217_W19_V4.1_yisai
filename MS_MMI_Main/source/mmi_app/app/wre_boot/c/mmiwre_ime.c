/////////////////////////////////////////////////////
/******************************************************************************
 ** File Name:      adaptor_image.c                                          *
 ** Author:         RiancyZhang                                              *
 ** DATE:           30/12/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file wraps image-layer for mocor os				       *
 **																              *
 ******************************************************************************/
#include "window_parse.h"
#include "guires.h"
#include "guiwin.h"
#include "mmi_id.h"
#include "mmitheme_clipbrd.h"
#include "mmiclipbrd_export.h"
#include "mmk_app.h"
#include "guiim_base.h"
#include "guiedit.h"
#include "mmidisplay_data.h"
#include "mmiwre_ime.h"
//#include "mmiset.h" //heng.xiao 20120209 add
#include "mmiset_export.h"
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

#define MAKELONG_IME(a, b) ((long)(((uint16)(a)) | ((uint32)((uint32)(b))) << 16))

typedef struct
{
    GUI_RECT_T      rt;
    wchar *         wstr_ptr;
    uint16          wstr_len;
    uint16          max_str_len;
    GUIIM_TYPE_T    allow_inputmethod;
    GUIIM_TYPE_T    cur_inputmethod;
    GUIEDIT_TYPE_E  edit_type;    
    uint16          wCursorPos; //光标位置
    void            (*EditProc)(const void*  hWnd,
                                uint32 wParam,  //编辑后的字符串
                                uint32 lParam   //编辑后的字符串长度（高16位）和光标位置（低16位）
                               );
    void *          hWnd;      //Edit自身窗口句柄（与EditProc的第一个参数对应）
} WRE_TEXTEDITBOX_INIT_DATA_T;

// add by liang.chen, 2011-09-02
typedef void (*EditProcFuncPtr)(const void *hWnd, uint32 wParam, uint32 lParam);
// add end, 2011-09-02

LOCAL WRE_TEXTEDITBOX_INIT_DATA_T *pInit_data=NULL;
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
LOCAL MMI_RESULT_E HandleWRETextEditWinMsg(MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM param);
LOCAL WINDOW_TABLE(WRE_TEXT_EDIT_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO(WIN_ONE_LEVEL),
	//WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_ALL),
	WIN_FUNC((uint32)HandleWRETextEditWinMsg),    
#ifdef MMI_PDA_SUPPORT
	WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
	WIN_TITLE(TXT_NULL),
	WIN_ID(WRE_TEXT_EDIT_WIN_ID),
	WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
	END_WIN
};
/*=============================================================
 函数名称 : TriggerMocorIme
 功能 : 触发mocor平台输入法
 参数:	void* hIMEWnd[IN] --- 输入法窗口(或ID)
		unsigned int msg[IN] --- 触发消息
		unsigned int wParam[IN] --- 消息参数
		long lParam[IN]
 
 返回 : !0 : 成功
		0 : 失败
   
=============================================================*/
long MMIWRE_TriggerMocorIme(void* hIMEWnd,uint32 msg,uint32 wParam,uint32 lParam)
{
    IMECALLPROCPARAM  *pImeParam=(IMECALLPROCPARAM*)lParam;
    
    if( NULL == pImeParam)
    {
        //failure
        return 0;
    }
    pInit_data = (WRE_TEXTEDITBOX_INIT_DATA_T*)SCI_ALLOCA(sizeof(WRE_TEXTEDITBOX_INIT_DATA_T));
    if(NULL == pInit_data)
    {
        return 0;
    }
    memset(pInit_data,0x00,sizeof(WRE_TEXTEDITBOX_INIT_DATA_T));
    //输入法设置
    if(pImeParam->dwImeType == _IN_ONLYNUMBER || pImeParam->dwImeType == _IN_ONLYPHONENUMBER)
    {
        pInit_data->allow_inputmethod = GUIIM_TYPE_DIGITAL;
        pInit_data->cur_inputmethod = GUIIM_TYPE_DIGITAL;
        if (pImeParam->dwImeType == _IN_ONLYNUMBER)
        {
            pInit_data->edit_type = GUIEDIT_TYPE_DIGITAL;
        }
        else 
        {
            pInit_data->edit_type = GUIEDIT_TYPE_PHONENUM;
        }
    }
    
    else
    {
        //当前键盘
        if(pImeParam->wImeCurKb == 0xFFFF)
        {
            pInit_data->cur_inputmethod = GUIIM_TYPE_SMART;
        }
        else
        {
            pInit_data->cur_inputmethod = pImeParam->wImeCurKb;
        }
        
        if(pImeParam->dwImeSupportKBs ==  (int)0xFFFFFFFF)
        {
            pInit_data->allow_inputmethod = GUIIM_TYPE_SET_ALL;
        }
        //add protect by heng.xiao 20111019
        else if(pImeParam->dwImeSupportKBs ==  0)
        {
            free(pInit_data);
            pInit_data = NULL;
            return 0;
        }
        else
        {
            pInit_data->allow_inputmethod = pImeParam->dwImeSupportKBs;
        }
        
    }
    
    //掩码设置
    if(pImeParam->bIsMask)
    {
        pInit_data->edit_type = GUIEDIT_TYPE_PASSWORD;
    }
    else
    {
        if (pInit_data->edit_type == 0)
        {//no value,so default --- 20110926 by heng.xiao
            pInit_data->edit_type = GUIEDIT_TYPE_TEXT;
        }
        //pInit_data->edit_type = (pImeParam->dwImeType == _IN_ONLYPHONENUMBER)?GUIEDIT_TYPE_PHONENUM:GUIEDIT_TYPE_TEXT;
    }
    
    
    //其它设置
    pInit_data->rt.left = 0;
    pInit_data->rt.top  = MMI_CLIENT_RECT_TOP;
    pInit_data->rt.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    pInit_data->rt.bottom = MMI_CLIENT_RECT_BOTTOM;
    pInit_data->hWnd = pImeParam->hWnd;
    pInit_data->EditProc = (EditProcFuncPtr)pImeParam->EditProc;
    pInit_data->max_str_len = pImeParam->wLimitLen;
    pInit_data->wCursorPos = pImeParam->wCursorPos;
    if(pImeParam->wInitDataLen > 0)
    {
        pInit_data->wstr_len = pImeParam->wInitDataLen;
        pInit_data->wstr_ptr = (wchar *)pImeParam->pszInitData;
    }
    
    //MMK_CreateWin((uint32*)WRE_TEXT_EDIT_WIN_TAB, pInit_data);
    MMK_CreateWinByApplet(SPRD_WRE_APPLET_ID, (uint32*)WRE_TEXT_EDIT_WIN_TAB, NULL);
    return 1;
}
/*=============================================================
 函数名称 : HandleWRETextEditWinMsg
 功能 : 带EDIT输入法的窗口处理过程
 参数:	MMI_WIN_ID_T win_id[IN] --- 消息ID
		MMI_MESSAGE_ID_E msg_id[IN] --- 消息类型
		DPARAM param[IN] --- 消息参数
		
 
 返回 : MMI_RESULT_TRUE : 成功
		MMI_RESULT_FALSE : 失败
   
=============================================================*/
LOCAL MMI_RESULT_E HandleWRETextEditWinMsg(
											MMI_WIN_ID_T win_id, 
											MMI_MESSAGE_ID_E msg_id, 
											DPARAM param
											)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T editbox_str_info = {0};
    MMI_CONTROL_CREATE_T create = {0};
    //WRE_TEXTEDITBOX_INIT_DATA_T *init_data_ptr = (WRE_TEXTEDITBOX_INIT_DATA_T*)MMK_GetWinAddDataPtr(win_id);
    GUIEDIT_INIT_DATA_T		edit_init_data = {0};
    uint16 pos = 0;
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:        
        if (pInit_data == PNULL)
		{
            return MMI_RESULT_FALSE;
        }
		edit_init_data.type = pInit_data->edit_type;
        edit_init_data.both_rect=MMITHEME_GetWinClientBothRect(win_id);
		edit_init_data.str_max_len = pInit_data->max_str_len;		
		
        create.ctrl_id = WRE_CTRL_ID_IM; //Control id
        create.guid = SPRD_GUI_EDITBOX_ID;
        create.parent_win_handle = win_id;
        create.init_data_ptr = &edit_init_data; 
		//Create Control dynamicly
        MMK_CreateControl(&create); 
		//密码输入界面增加输入法切换显示
		if (GUIEDIT_TYPE_PASSWORD == edit_init_data.type)
		{
			BOOLEAN is_disp_im_icon = TRUE;
			BOOLEAN is_disp_num_info = TRUE;
			GUIEDIT_SetStyle(WRE_CTRL_ID_IM, GUIEDIT_STYLE_MULTI);//多行
            GUIEDIT_SetAlign(WRE_CTRL_ID_IM, ALIGN_LEFT);//靠左上起
			GUIEDIT_SetDispImIcon(WRE_CTRL_ID_IM,&is_disp_im_icon,&is_disp_num_info);
		}
        GUIEDIT_SetIm(WRE_CTRL_ID_IM,pInit_data->allow_inputmethod,pInit_data->cur_inputmethod);
		//初始化字符串
		if(pInit_data->wstr_len > 0)
		{
			GUIEDIT_SetString(WRE_CTRL_ID_IM,pInit_data->wstr_ptr,(uint16)pInit_data->wstr_len);
		}
		//设置光标位置
		GUIEDIT_SetCursorCurPos(WRE_CTRL_ID_IM,pInit_data->wCursorPos);
        MMK_SetAtvCtrl(win_id, WRE_CTRL_ID_IM);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, COMMON_TXT_NULL, STXT_RETURN, FALSE); //Softbutton
        break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;           
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527    
#endif		
    case MSG_CTL_MIDSK:
	case MSG_APP_OK:
	case MSG_APP_WEB:
	case MSG_CTL_OK:      
        GUIEDIT_GetString(WRE_CTRL_ID_IM, &editbox_str_info);
        pos = GUIEDIT_GetCursorCurPos(WRE_CTRL_ID_IM);
		
		if(pInit_data->EditProc)
		{
			//数据通知
			(pInit_data->EditProc)(pInit_data->hWnd,(unsigned int)editbox_str_info.wstr_ptr,MAKELONG_IME(pos,editbox_str_info.wstr_len));
		}
		
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
		if(pInit_data->EditProc)
		{
			//数据通知
			(pInit_data->EditProc)(pInit_data->hWnd,0,0);
		}
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_RED: 
        MMK_CloseWin(win_id);
        //MMK_CloseApplet(SPRD_WRE_APPLET_ID);
        break;
		
    case MSG_CLOSE_WINDOW:
		if(pInit_data) SCI_Free(pInit_data);
		pInit_data=NULL;
        break;
#ifdef MMI_PDA_SUPPORT  //heng.xiao 20120209 add for bug --- iphone style,can't dispay sub info
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
#if defined MMI_ISTYLE_SUPPORT 		
        if (MMIAPISET_GetIdleStyle() == MMISET_IDLE_ISTYLE)
        {
            MMI_STRING_T edit_str = {0};
            uint8 remain_num[25] = {0};
            wchar str_ptr[50] = {0};
            int len;
        
            GUIEDIT_GetString(WRE_CTRL_ID_IM, &edit_str);
            len = sprintf((char*)remain_num,"%d/%d",edit_str.wstr_len,pInit_data->max_str_len);            
            //convert wchar
            MMI_STRNTOWSTR(str_ptr,49,(const uint8 *)remain_num,len,len);
            GUITITLE_SetText(win_id,MMITHEME_GetTitleCtrlId(),str_ptr,49,TRUE);
        }
#endif		
        recode = MMI_RESULT_FALSE;
        break;
#endif        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}


/*=============================================================
 函数名称 : ClipBoardCopyForMocor
 功能 : 拷贝数据到剪切板
 参数:
		unsigned short wDataLength[IN] --- 数据长度
		char *pBuffer[IN] --- 数据
 
 返回 : 如果== wDataLength : 成功
		0 : 失败
   
=============================================================*/
unsigned short ClipBoardCopyForMocor(void *pBuffer,unsigned short wDataLength)
{
	CLIPBRD_RESULT_E ret;
	uint32 unMin=wDataLength;
	uint32 unMaxLeng = 0;
	
#ifdef CLIPBRD_SLIDER_SUPPORT
	unMaxLeng =MMITHEME_GetClipbrdMaxSize();
#endif

	if (wDataLength > unMaxLeng)
	{
		unMin = unMaxLeng;
	}

	ret = MMIAPICLIPBRD_SetData(CLIPBRD_FORMAT_TEXT,(void*)pBuffer,unMin);
	if(CLIPBRD_RESULT_NOERROR != ret)
	{
		return 0;
	}

	return (unsigned short)unMin;
}


/*=============================================================
 函数名称 : ClipBoardPasteForMocor
 Author:  heng.xiao
 功能 : 从剪切板拷贝数据
 参数:  TBYTE*pBuffer[IN/OUT] --- 数据缓冲区
		WORD wBufferLength[IN] --- 缓冲区大小
 
  返回 : 如果== wDataLength : 成功
		0 : 失败
   
=============================================================*/
unsigned short ClipBoardPasteForMocor(void *pBuffer, unsigned short wBufferLength)
{
	CLIPBRD_RESULT_E ret;
	uint32 wDataLen=0;
    char* p_temp = NULL;
	
	ret = MMIAPICLIPBRD_GetData(CLIPBRD_FORMAT_TEXT,(void**)&p_temp,&wDataLen);
	if(CLIPBRD_RESULT_NOERROR != ret)
	{
		return 0;
	}
    if (wDataLen > wBufferLength)
    {
        wDataLen = wBufferLength;
    }
    memcpy(pBuffer,p_temp,wDataLen);		
	return (unsigned short)wDataLen;
}


/*=============================================================
 函数名称 : ClipBoardClearForMocor
 功能 : 清除剪切板
 参数:  void
 
 返回 : void
   
=============================================================*/
void ClipBoardClearForMocor(void)
{
	MMIAPICLIPBRD_Clear(CLIPBRD_FORMAT_TEXT);
}


/*=============================================================
 函数名称 : clipboardfuncproc
 功能 : 从剪切板拷贝数据
 参数:  TBYTE*pBuffer[IN/OUT]  --- 数据缓冲区
		WORD wBufferLength[IN] --- 缓冲区大小
        int opt[IN]            --- get,set,clear         
 
  返回 :    
=============================================================*/
unsigned short clipboardfuncproc(void *pBuffer,unsigned short wDataLength,int opt)
{
    if (opt == CLIP_BORRD_CLEAR)
    {
        ClipBoardClearForMocor();
        return 1;
    }
    else if (opt == CLIP_BORRD_GET)
    {
        return ClipBoardPasteForMocor(pBuffer,wDataLength);
    }
    else if (opt == CLIP_BORRD_SET)
    {
        return ClipBoardCopyForMocor(pBuffer,wDataLength);
    }
    return 0;
}



/*Edit by script, ignore 2 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
