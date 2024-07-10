/*****************************************************************************
** File Name:      mmiocr_wintab.c                                              *
** Author:         jian.ma                                                   *
** Date:           06/2007													 *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the OCR internal functions  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2007        jian.ma           Create                                   *
******************************************************************************/



/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_ocr_trc.h"
#include "std_header.h"

#ifdef MMI_OCR_ENABLE
#include "window_parse.h"
#include "mmiocr_internal.h"
#include "mmiocr_id.h"
#include "mmidc_export.h"
#include "mmiocr_text.h"
#include "Mmi_text.h"
#include "Mmk_app.h"
#include "mmi_common.h"
#include "mmi_nv.h"

#include "mmidc_id.h"
#include "guilabel.h"
#include "mmidc_capture.h"
#include "mmi_appmsg.h"
#include "mmiocr_lib.h"
#include "mmi_default.h"
#include "mmk_timer.h"
#include "guiref_scale.h"
//#include "mmieng.h"
#include "mmieng_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define SHOOT_RECT_WIDTH					80
#define SHOOT_RECT_HEIGHT					100

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern void MMIAPISET_StopAllRing(BOOLEAN is_include_mp3);
extern GUI_LCD_DEV_INFO MMIDC_GetReviewBlockInfo(void);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL uint8	s_3seconds_timer_id = 0;    //used for close OSD in 3 seconds if user doesn't do any operations
LOCAL OCR_CARD_INFO s_recog_result_info[OCR_MAX_RECOG_REGION_NUM];
LOCAL BOOLEAN s_capture_is_complete = FALSE;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
//*****************************************************************************/
// 	Description : alert DC_Open, DC_SetPreviewParam, DC_StartPreview wrong
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void AlertOCRStartPreviewWrong(void);

/*****************************************************************************/
// 	Description : handle the message of preview window
//	Global resource dependence : none
//  Author: Annie.an
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIOCR_HandlePreviewWinMsg(
									  MMI_WIN_ID_T win_id, 
                                      MMI_MESSAGE_ID_E msg_id, 
                                      DPARAM param
									  );

/*****************************************************************************/
// 	Description : startCapture
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN StartCapture(void);

/*****************************************************************************/
// 	Description : Handle capture window msg which include layout msg,recognize
//                msg,and will send the recognize result to pb for display and
//                edit.
//	Global resource dependence : 
//  Author: Annie.an 
//	Note:  This is the main process of ocr app. All refresh is done when full paint
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIOCR_HandleCaptureWinMsg(	 
											   MMI_WIN_ID_T win_id, 
											   MMI_MESSAGE_ID_E	msg_id, 
											   DPARAM		param
											   );


/*****************************************************************************/
// 	Description : Update current preview osd 
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void MMIOCR_UpdateCurPreviewOSD(void);

/*****************************************************************************/
// 	Description : handle the cancel message of capture window
//	Global resource dependence : none
//  Author: jian.ma 
//	Note:
/*****************************************************************************/
LOCAL void CaptureHandleAppCANCELMsgProcess(MMI_MESSAGE_ID_E msg_id);


/*****************************************************************************/
// 	Description : handle the OK message of capture window
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void CaptureHandleAppOKMsgProcess(MMI_MESSAGE_ID_E msg_id);


/*****************************************************************************/
// 	Description : get preview param
//	Global resource dependence : 
//  Author: gang.tong
//	Note:	for MMI upper level
/*****************************************************************************/
LOCAL BOOLEAN OCRAdap_GetPreviewParam(DCAMERA_PREVIEW_PARAM_T * preview_param_ptr);

/*****************************************************************************/
// 	Description : Set all preview param
//	Global resource dependence : 
//  Author: jian.ma
//	Note:	
/*****************************************************************************/
LOCAL BOOLEAN MMIOCR_SetAllPreviewParam(void);

/*****************************************************************************/
// 	Description : draw focus win
//	Global resource dependence : 
//  Author: jian.ma
//	Note:	
/*****************************************************************************/
LOCAL void DrawFocusWin(uint32 color);

/*****************************************************************************/
// 	Description : set auto focus param
//	Global resource dependence : 
//  Author: jian.ma
//	Note:	
/*****************************************************************************/
LOCAL void SetAutoFocusParam(void);


/*****************************************************************************/
// 	Description : Aoto focus call back 
//	Global resource dependence : none
//  Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL uint32 AutoFocusCallback(uint32 result);

//the window for preview
WINDOW_TABLE(MMIOCR_PREVIEWWIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)MMIOCR_HandlePreviewWinMsg),    
	WIN_ID(MMIOCR_PREVIEW_WIN_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	END_WIN
};

WINDOW_TABLE(MMIOCR_SHOOTWIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)MMIOCR_HandleCaptureWinMsg), 
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIOCR_SHOOTING_LABEL_CTRL_ID),// shooting
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIOCR_RECOG_LABEL_CTRL_ID),// recoging
	WIN_ID(MMIOCR_CAPTURE_WIN_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	END_WIN
};

/*****************************************************************************/
// 	Description : open preview window 
//	Global resource dependence : none
//  Author: jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIOCR_OpenPreviewWin(void)
{
	BOOLEAN ret = FALSE;

	ret = MMK_CreateWin((uint32*)MMIOCR_PREVIEWWIN_TAB, PNULL);
	//SCI_TRACE_LOW:"[MMIOCR]: MMIOCR_OpenPreviewWin, ret = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_WINTAB_180_112_2_18_2_42_9_22,(uint8*)"d", ret);

	return ret;
}
/*****************************************************************************/
// 	Description : open the window of camera menu
//	Global resource dependence : none
//  Author:jian.ma 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIOCR_OpenCaptureWin(void)
{
	BOOLEAN ret = FALSE;
		
	MMIDC_OpenDC(DC_FOR_CAPTURE);
	MMIOCR_StartPreview(DC_FOR_CAPTURE);
	ret = MMK_CreateWin((uint32 *)MMIOCR_SHOOTWIN_TAB, PNULL);
	return ret;
}


/*****************************************************************************/
// 	Description : Aoto focus call back 
//	Global resource dependence : none
//  Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL uint32 AutoFocusCallback(uint32 result)
{
	//SCI_TRACE_LOW:"\" AutoFocusCallback : result %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_WINTAB_205_112_2_18_2_42_9_23,(uint8*)"d", result);
	if(DCAMERA_AUTO_FOCUS_STATUS_SUCCESS == result)
	{
		MMK_PostMsg(MMIOCR_PREVIEW_WIN_ID, MSG_OCR_AF_SUCCESS, (DPARAM)PNULL, 0);
	}
	else if(DCAMERA_AUTO_FOCUS_STATUS_FAIL == result)
	{
		MMK_PostMsg(MMIOCR_PREVIEW_WIN_ID, MSG_OCR_AF_FAIL, (DPARAM)PNULL, 0);
	}
	MMIAPICOM_TriggerMMITask();	
	return 1;
}

/*****************************************************************************/
// 	Description : handle the message of preview window
//	Global resource dependence : none
//  Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIOCR_HandlePreviewWinMsg(
									  MMI_WIN_ID_T win_id, 
                                      MMI_MESSAGE_ID_E msg_id, 
                                      DPARAM param
									  )
{
	MMI_RESULT_E recode	= MMI_RESULT_TRUE;		
	
	//SCI_TRACE_LOW:"[MMIOCR]: MMIOCR_HandlePreviewWinMsg, msg_id = 0x%.2x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_WINTAB_230_112_2_18_2_42_9_24,(uint8*)"d", msg_id);
 	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		MMIDC_EnableScaleDown(FALSE);
	    MMIDC_ClearMainBlock(MMI_BLACK_COLOR);
	    
		MMIDC_OpenOSDBLock();
		MMIDC_ConfigReviewBLock();
		MMIDC_EnableReviewBlock(DC_FOR_PREVIEW, TRUE);
		MMIDC_EnableOsdBlock(DC_FOR_PREVIEW, TRUE);		
				
		// dc open
		if (!MMIDC_OpenDC(DC_FOR_PREVIEW))
		{			
			AlertOCRStartPreviewWrong();
			MMK_CloseWin(win_id);
			break;
		}			
		
		//set preview all parameters
		if (!MMIOCR_SetAllPreviewParam())
		{		
			AlertOCRStartPreviewWrong();
			MMK_CloseWin(win_id);
			break;
		}

		//start preview
		if (!MMIOCR_StartPreview(DC_FOR_PREVIEW))
		{
			AlertOCRStartPreviewWrong();
			MMK_CloseWin(win_id);
			break;
		}
	
		MMIDC_SetDCCurPreviewOSDMode(OSD_DC_WIN_PREVIEW_DEFAULT);
		MMIOCR_UpdateCurPreviewOSD();
		//do not allow enter sleep
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        break;

	case MSG_LOSE_FOCUS:			
		MMIDC_EnableOsdBlock(DC_FOR_PREVIEW, FALSE);
		MMIDC_StopPreview(DC_FOR_PREVIEW);
		MMIDC_CloseDC(DC_FOR_PREVIEW);
		if(OSD_DC_WIN_PREVIEW_SHOOT != MMIDC_GetDCCurPreviewOSDMode())
		{
			MMIDC_UnconfigReviewBLock();
			MMIDC_EnableScaleDown(TRUE);
		}
		break;

	case MSG_GET_FOCUS:						
		MMIDC_ClearMainBlock(MMI_BLACK_COLOR);
		MMIDC_EnableScaleDown(FALSE);
		MMIDC_OpenOSDBLock();
		MMIDC_ConfigReviewBLock();
		MMIDC_EnableOsdBlock(DC_FOR_PREVIEW, TRUE);
		MMIDC_EnableReviewBlock(DC_FOR_PREVIEW, TRUE);
		//MMIDEFAULT_EnableKeyRing(FALSE);
        //MMIDEFAULT_EnableTpRing(FALSE);
		//forbid enter sleep mode
		MMIDEFAULT_AllowTurnOffBackLight(FALSE);
		// open dc
		if (!MMIDC_OpenDC(DC_FOR_PREVIEW))
		{
			AlertOCRStartPreviewWrong();
			MMK_CloseWin(win_id);
			break;
		}		
		if (!MMIOCR_SetAllPreviewParam())
		{
			AlertOCRStartPreviewWrong();
			MMK_CloseWin(win_id);
			break;
		}
		if (!MMIOCR_StartPreview(DC_FOR_PREVIEW))
		{
			AlertOCRStartPreviewWrong();
			MMK_CloseWin(win_id);
			break;
		}	
		// set current preview osd 		
		MMIDC_SetDCCurPreviewOSDMode(OSD_DC_WIN_PREVIEW_DEFAULT);
		MMIOCR_UpdateCurPreviewOSD();
		break;
		
	case MSG_FULL_PAINT:
		break;
		
	case MSG_TIMER:	
		if (0 != s_3seconds_timer_id && s_3seconds_timer_id == *((uint8 *) param))
		{
			GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
			GUI_RECT_T rect = {MMI_MAINSCREEN_WIDTH / 2 - 20, MMI_MAINSCREEN_HEIGHT / 2 - 55,
				MMI_MAINSCREEN_WIDTH / 2 + 20, MMI_MAINSCREEN_HEIGHT / 2 + 55};
			MMK_StopTimer(s_3seconds_timer_id);
            s_3seconds_timer_id = 0;
			MMIOCR_UpdateCurPreviewOSD();
			MMITHEME_StoreUpdateRect(&dev_info, rect);
		}
		break;
	case MSG_APP_CAMERA:
    	{
    	    SetAutoFocusParam();
    	    
    		DrawFocusWin(MMI_RED_COLOR);
    	}
		break;
	case MSG_APP_WEB:
	case MSG_APP_OK:
		if(0 != s_3seconds_timer_id)
		{
			MMK_StopTimer(s_3seconds_timer_id);
            s_3seconds_timer_id = 0;
		}
		MMIDC_SetDCCurPreviewOSDMode(OSD_DC_WIN_PREVIEW_SHOOT);
		StartCapture();	
		break;		
    case MSG_APP_CANCEL: 		
		MMK_CloseWin(win_id);		
        break;
        
 	case MSG_OCR_AF_FAIL:
		{
			GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
			GUI_RECT_T rect = {MMI_MAINSCREEN_WIDTH / 2 - 20, MMI_MAINSCREEN_HEIGHT / 2 - 55,
				MMI_MAINSCREEN_WIDTH / 2 + 20, MMI_MAINSCREEN_HEIGHT / 2 + 55};
			MMIOCR_UpdateCurPreviewOSD();
			MMITHEME_StoreUpdateRect(&dev_info, rect);
		}
		break;
		
	case MSG_OCR_AF_SUCCESS:
		{
			DrawFocusWin(MMI_GREEN_COLOR);
            if(0 != s_3seconds_timer_id) //added by ryan.xu
            {
                MMK_StopTimer(s_3seconds_timer_id);
                s_3seconds_timer_id = 0;
            }
			s_3seconds_timer_id = MMK_CreateTimer(MMI_2SECONDS, TRUE);
		}
		break;	
    case MSG_CLOSE_WINDOW:
	
    	//如果用户按红键退出，需要关闭timer
		if(0 != s_3seconds_timer_id) 
		{
			MMK_StopTimer(s_3seconds_timer_id);
			s_3seconds_timer_id = 0;
		}
		if(!MMIDC_StopPreview(DC_FOR_ALL))
        {
    		//SCI_TRACE_LOW:"[MMIOCR]::HandlePreviewWinMsg: MSG_CLOSE_WINDOW, stop dc error!"
    		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_WINTAB_385_112_2_18_2_42_10_25,(uint8*)"");
        }

        if(!MMIDC_CloseDC(DC_FOR_ALL))
        {
    		//SCI_TRACE_LOW:"[MMIOCR]: HandlePreviewWinMsg: MSG_CLOSE_WINDOW, close dc error! "
    		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_WINTAB_390_112_2_18_2_42_10_26,(uint8*)"");
        }
		//allow enter sleep
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		//MMIDEFAULT_EnableKeyRing(TRUE);
        //MMIDEFAULT_EnableTpRing(TRUE);

        MMIDC_SetDCCurPreviewOSDMode(OSD_DC_WIN_PREVIEW_NONE);		
		MMIDC_FreeJpegBuf();
		MMIDC_CloseOSDBLock();
		MMIDC_UnconfigReviewBLock();
		MMIDC_EnableScaleDown(TRUE);
		
        break;

	default:
		recode = MMI_RESULT_FALSE;
		break;
	}

	return recode;

}


/*****************************************************************************/
// 	Description : Handle capture window msg which include layout msg,recognize
//                msg,and will send the recognize result to pb for display and
//                edit.
//	Global resource dependence : 
//  Author: jian.ma 
//	Note:  This is the main process of ocr app. All refresh is done when full paint
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIOCR_HandleCaptureWinMsg(	 
											   MMI_WIN_ID_T win_id, 
											   MMI_MESSAGE_ID_E	msg_id, 
											   DPARAM		param
											   )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
		
	//SCI_TRACE_LOW:"[MMIOCR]: HandleCaptureWinMsg, msg_id = 0x%.2x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_WINTAB_429_112_2_18_2_42_10_27,(uint8*)"d", msg_id);
	switch(msg_id)
    {
	case MSG_OPEN_WINDOW:	
        {
        	GUI_RECT_T lab_rect = {0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};
			GUILABEL_SetRect(MMIOCR_SHOOTING_LABEL_CTRL_ID, &lab_rect, FALSE);
        }
        {
        	GUI_RECT_T lab_rect = {0, MMI_MAINSCREEN_HEIGHT/4, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};
			GUILABEL_SetRect(MMIOCR_RECOG_LABEL_CTRL_ID, &lab_rect, FALSE);
        }	
		MMK_SetCtrlBlockInfo(win_id, MMIOCR_SHOOTING_LABEL_CTRL_ID, MMIDC_GetOSDBlockInfo());
		MMK_SetCtrlBlockInfo(win_id, MMIOCR_RECOG_LABEL_CTRL_ID, MMIDC_GetOSDBlockInfo());

		GUILABEL_SetFont(MMIOCR_SHOOTING_LABEL_CTRL_ID, MMI_DEFAULT_SMALL_FONT, MMI_GREEN_COLOR);
		GUILABEL_SetFont(MMIOCR_RECOG_LABEL_CTRL_ID, MMI_DEFAULT_SMALL_FONT, MMI_GREEN_COLOR);
		//display waiting on LCD
		MMIDC_OpenOSDBLock();
		MMIDC_ConfigReviewBLock();
		MMIDC_EnableOsdBlock(DC_FOR_CAPTURE, TRUE);
		
		MMIOCR_SetCurCaptureOSDMode(OSD_DC_WIN_CAPTURE_SNAPSHOTING);
		MMIOCR_UpdateCurCaptureOSD();
		
		MMK_PostMsg(win_id, MSG_DC_START_SHOOT_OPERATION, PNULL, 0);
		break;
	case MSG_DC_START_SHOOT_OPERATION:
		if (!MMIOCR_Capture(&g_dc_setting_info))
		{
			MMIDC_OpenPromptAlertWin(MMIPUB_SOFTKEY_ONE, TXT_BROWSER_DATA_ERROR, IMAGE_PUBWIN_FAIL, MMI_2SECONDS, PNULL);
			MMK_CloseWin(win_id);
		}	
		break;
	case MSG_DC_REVIEW_SUCCESS_MSG:
		MMIDC_EnableReviewBlock(DC_FOR_CAPTURE, TRUE);
		MMIOCR_SetCurCaptureOSDMode(OSD_DC_WIN_CAPTURE_DONE);
		MMIOCR_UpdateCurCaptureOSD();		
		break;
	case MSG_DC_REVIEW_FAILE_MSG:	
		break;
	case MSG_DC_CAPTURE_SUCCESS_MSG:	/** capture success **/
	    s_capture_is_complete = TRUE;		
		MMIOCR_SetCurCaptureOSDMode(OSD_DC_WIN_CAPTURE_DONE);	
		// do review 
		MMIOCR_ReviewCapturedPhotos(win_id, MULTI_SHOT_DISABLE, TRUE, TRUE);				
		break;

	case MSG_DC_CAPTURE_FAILE_MSG:		/** capture faile **/
	    s_capture_is_complete = TRUE;
		MMIDC_OpenPromptAlertWin(MMIPUB_SOFTKEY_ONE, TXT_BROWSER_DATA_ERROR, IMAGE_PUBWIN_FAIL, MMI_2SECONDS, PNULL);
		MMK_CloseWin(win_id);	
		break;

    case MSG_FULL_PAINT: 		
#ifdef WIN32
			MMIDC_ClearMainBlock(MMI_BLACK_COLOR);
#endif
		break;
    case MSG_GET_FOCUS:	
        break;	
    case MSG_LOSE_FOCUS:    
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_OK:
		// save and close the win	
		CaptureHandleAppOKMsgProcess(msg_id);
        break;

    case MSG_APP_CANCEL:
		// delete the file and close this win	
		CaptureHandleAppCANCELMsgProcess(msg_id);
        break;
        
	 case MSG_OCR_RECOG_OVER:  // receive the ocr recognizing finished msg.	
		MMIOCR_StopAllTimer();
		MMIOCR_SetCurCaptureOSDMode(OSD_OCR_WIN_CAPTURE_RECOG_DONE);
		MMIOCR_UpdateCurCaptureOSD();
		SCI_Sleep(1000);
		SCI_MEMSET(s_recog_result_info,0,(OCR_MAX_RECOG_REGION_NUM)*sizeof(OCR_CARD_INFO));	
		MMK_CloseWin(win_id);
#ifdef WIN32
		MMIOCR_DisplayResultInPB(PNULL);
#else
		SCI_MEMCPY(s_recog_result_info,OCR_GetRecogResult(),(OCR_MAX_RECOG_REGION_NUM)*sizeof(OCR_CARD_INFO));	
		MMIOCR_DisplayResultInPB(s_recog_result_info); // send the recognize result to pb.
#endif	
		
		break;
	case MSG_OCR_RECOG_FAIL:	
		MMK_CloseWin(win_id);
		break;
	case MSG_KEYDOWN_RED:
	    break;
    case MSG_CLOSE_WINDOW:	
		MMIOCR_StopAllTimer();
   	 	OCR_AutoRecogStop();			
        MMIDC_EnableReviewBlock(DC_FOR_CAPTURE, FALSE);
		MMIDC_EnableOsdBlock(DC_FOR_CAPTURE, FALSE);
		MMIDC_UnconfigReviewBLock();
		MMIDC_CloseOSDBLock();
		MMIDC_CloseDC(DC_FOR_CAPTURE);
        break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;
}


//*****************************************************************************/
// 	Description : alert DC_Open, DC_SetPreviewParam, DC_StartPreview wrong
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void AlertOCRStartPreviewWrong(void)
{
    MMIPUB_OpenAlertFailWin(TXT_DC_OPEN_FAIL);
}

//*****************************************************************************/
// 	Description : alert DC_Open, DC_SetPreviewParam, DC_StartPreview wrong
//	Global resource dependence : none
//  Author: jian.ma 
//	Note:
/*****************************************************************************/
PUBLIC void MMIOCR_AlertOCRRecogWrong(void)
{	
	MMIPUB_OpenAlertFailWin(TXT_OCR_RECOG_FAIL);   

}

/*****************************************************************************/
// 	Description : Update current preview osd 
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void MMIOCR_UpdateCurPreviewOSD(void)
{
	GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
	GUI_RECT_T lcd_rect = MMITHEME_GetFullScreenRect();
	
	/** clear osd block **/
	if(GUIBLOCK_IsEnable(&dev_info))
	{
		GUIBLOCK_ClearByColor(MMI_OSD_TRANSPARENT_COLOR,&dev_info);
		
		switch(MMIDC_GetDCCurPreviewOSDMode())
		{
		case OSD_DC_WIN_PREVIEW_NONE:		
			break;
		case OSD_DC_WIN_PREVIEW_DEFAULT:					// "Capture","取消"窗口
			/** set soft key **/		
			GUIWIN_SetSoftkeyStyle(
							MMIOCR_PREVIEW_WIN_ID,
							MMI_GREEN_COLOR,
							GUI_SOFTKEY_BG_NULL,
							MMI_OSD_TRANSPARENT_COLOR,
							NULL);
			GUIWIN_SetSoftkeyTextId(
							MMIOCR_PREVIEW_WIN_ID,
							TXT_COMMON_CAPTURE,
							TXT_NULL,
							STXT_RETURN,
							TRUE
							);
			/** close OSD setting menu **/
			MMITHEME_StoreUpdateRect(&dev_info, lcd_rect);	
			break;
		case OSD_DC_WIN_PREVIEW_SHOOT:
			/** set soft key **/		
			GUIWIN_SetSoftkeyStyle(
							MMIOCR_PREVIEW_WIN_ID,
							MMI_GREEN_COLOR,
							GUI_SOFTKEY_BG_NULL,
							MMI_OSD_TRANSPARENT_COLOR,
							NULL);
			GUIWIN_SetSoftkeyTextId(
							MMIOCR_PREVIEW_WIN_ID,
							TXT_OCR_RECOG,
							TXT_NULL,
							STXT_RETURN,
							TRUE
							);
			/** close OSD setting menu **/
			MMITHEME_StoreUpdateRect(&dev_info, lcd_rect);	
			break;
		case OSD_DC_WIN_PREVIEW_SETTING:				// 显示OSD setting选择的窗口								
			break;
		case OSD_DC_WIN_PREVIEW_DELAY:					// 延时拍照提示
			break;
		default:
			//SCI_ASSERT(0);
			break;
		}	
	}
}


/*****************************************************************************/
// 	Description : Update current capture osd 
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMIOCR_UpdateCurCaptureOSD(void)
{
	GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
	MMI_STRING_T str = {0};
	uint8 progress[5] = {0};
	static uint32 progress_num = 0;
	static int16  progress_step = 0;

	if(GUIBLOCK_IsEnable(&dev_info))
	{		
		switch(MMIOCR_GetCurCaptureOSDMode())
		{
		case OSD_OCR_WIN_CAPTURE_RECOG_START:
			/** clear osd block **/
			GUIBLOCK_ClearByColor(MMI_OSD_TRANSPARENT_COLOR,&dev_info);	
			GUILABEL_SetTextById(MMIOCR_SHOOTING_LABEL_CTRL_ID, TXT_WAITING, TRUE);
			GUILABEL_SetTextById(MMIOCR_RECOG_LABEL_CTRL_ID, TXT_NULL, TRUE);
			GUILCD_Invalidate(GUI_MAIN_LCD_ID, GUIREF_GetUpdateBlockSet(dev_info.block_id));
			progress_num = 0;
			break;
		case OSD_OCR_WIN_CAPTURE_RECOGING:
			{
				progress_num = (progress_num+1>=100)?99:(progress_num+3);
				sprintf((char*)progress, "%3d %%",progress_num);
				str.length = SCI_STRLEN((char*)progress);
				str.str_ptr = progress;
				GUILABEL_SetText(MMIOCR_RECOG_LABEL_CTRL_ID, &str, FALSE);
				GUIBLOCK_ClearByColor(MMI_OSD_TRANSPARENT_COLOR,&dev_info);

				progress_step = (progress_step==3)?0:(++progress_step);
				switch(progress_step)
				{
					case 0:
						GUILABEL_SetTextById(MMIOCR_SHOOTING_LABEL_CTRL_ID, TXT_WAITING, TRUE);
						break;
					case 1:
						GUILABEL_SetTextById(MMIOCR_SHOOTING_LABEL_CTRL_ID, TXT_WAITING, TRUE);
						break;
					case 2:
						GUILABEL_SetTextById(MMIOCR_SHOOTING_LABEL_CTRL_ID, TXT_WAITING, TRUE);
						break;
					case 3:
						GUILABEL_SetTextById(MMIOCR_SHOOTING_LABEL_CTRL_ID, TXT_WAITING, TRUE);
						break;
					default:
						break;
				}
				GUILCD_Invalidate(GUI_MAIN_LCD_ID, GUIREF_GetUpdateBlockSet(dev_info.block_id));
			}
			break;
		case OSD_OCR_WIN_CAPTURE_SNAPSHOTING:			// 显示拍摄中的OSD窗口	
			/** clear osd block **/
			GUIBLOCK_ClearByColor(MMI_OSD_TRANSPARENT_COLOR,&dev_info);	

			GUILABEL_SetTextById(MMIOCR_SHOOTING_LABEL_CTRL_ID, (MMI_TEXT_ID_T)TXT_OCR_SHOTTING, TRUE);
			GUILCD_Invalidate(GUI_MAIN_LCD_ID, GUIREF_GetUpdateBlockSet(dev_info.block_id));
				
			break;
		case OSD_OCR_WIN_CAPTURE_DONE:
			/** clear osd block **/
			GUIBLOCK_ClearByColor(MMI_OSD_TRANSPARENT_COLOR,&dev_info);	
			/** set label **/
			GUILABEL_SetTextById(MMIOCR_SHOOTING_LABEL_CTRL_ID, TXT_NULL, TRUE);
			GUILCD_Invalidate(GUI_MAIN_LCD_ID, GUIREF_GetUpdateBlockSet(dev_info.block_id));
			/** set soft key **/
			GUIWIN_SetSoftkeyStyle(
							MMIOCR_CAPTURE_WIN_ID,
							MMI_GREEN_COLOR,
							GUI_SOFTKEY_BG_NULL,
							MMI_OSD_TRANSPARENT_COLOR,
							NULL);
			GUIWIN_SetSoftkeyTextId(
							MMIOCR_PREVIEW_WIN_ID,
							TXT_OCR_RECOG,
							TXT_NULL,
							STXT_RETURN,
							TRUE
							);
			break;
		case OSD_OCR_WIN_CAPTURE_RECOG_DONE:
			/** clear osd block **/
			SCI_MEMCPY(progress,"100 %",5);
			str.length = 5;
			str.str_ptr = progress;
			GUILABEL_SetText(MMIOCR_RECOG_LABEL_CTRL_ID, &str, FALSE);
			GUIBLOCK_ClearByColor(MMI_OSD_TRANSPARENT_COLOR,&dev_info);	
			GUILABEL_SetTextById(MMIOCR_SHOOTING_LABEL_CTRL_ID, STXT_FINISH, TRUE);
			GUILCD_Invalidate(GUI_MAIN_LCD_ID, GUIREF_GetUpdateBlockSet(dev_info.block_id));
			break;
		default:
			//SCI_ASSERT(0);
			break;
		}
	}
}

/*****************************************************************************/
// 	Description : play shutter voice
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void TryPlayShutterVoice(void)
{
//#ifdef MMI_DC_TOTAL_OSD_ENABLE
	if(MMIDC_IsShutterVoiceEnable())
	{	
		MMIAPISET_PlayRing(0,FALSE, 0, 1, MMISET_RING_TYPE_DC, PNULL);
	}

//#endif
}

/*****************************************************************************/
// 	Description : startCapture
//	Global resource dependence : none
//  Author:jian.ma 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN StartCapture(void)
{		
    s_capture_is_complete = FALSE;
	// shutter voice 		
	TryPlayShutterVoice();				
	
	MMITHEME_SetUpdateDelayCount(1);
			        
	MMIOCR_OpenCaptureWin();		

	return TRUE;
}

/*****************************************************************************/
// 	Description : handle the cancel message of capture window
//	Global resource dependence : none
//  Author: jian.ma 
//	Note:
/*****************************************************************************/
LOCAL void CaptureHandleAppCANCELMsgProcess(MMI_MESSAGE_ID_E msg_id)
{
	//SCI_PASSERT(MSG_APP_CANCEL == msg_id,(("msg_id = %d"),msg_id));	/*assert verified*/
    
	switch(MMIOCR_GetCurCaptureOSDMode())
	{
	case OSD_OCR_WIN_CAPTURE_NONE:
	case OSD_OCR_WIN_CAPTURE_RECOG_START:
	case OSD_OCR_WIN_CAPTURE_RECOG_DONE:
	case OSD_OCR_WIN_CAPTURE_RECOGING:
		MMK_CloseWin(MMIOCR_CAPTURE_WIN_ID);
		break;
	case OSD_OCR_WIN_CAPTURE_SNAPSHOTING:			// 显示拍摄中的OSD窗口	
		break;
	case OSD_OCR_WIN_CAPTURE_DONE:
		MMIDC_EnableReviewBlock(DC_FOR_ALL, FALSE);
		MMK_CloseWin(MMIOCR_CAPTURE_WIN_ID);
		break;
	default:
		//SCI_ASSERT(0);
		break;
	}
}

/*****************************************************************************/
// 	Description : handle the OK message of capture window
//	Global resource dependence : none
//  Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL void CaptureHandleAppOKMsgProcess(MMI_MESSAGE_ID_E msg_id)
{
	//SCI_ASSERT(MSG_APP_OK == msg_id);/*assert verified*/
	switch(MMIOCR_GetCurCaptureOSDMode())
	{
	case OSD_OCR_WIN_CAPTURE_RECOG_START:
		break;
	case OSD_OCR_WIN_CAPTURE_SNAPSHOTING:			// 显示拍摄中的OSD窗口	
		break;
	case OSD_OCR_WIN_CAPTURE_DONE:	
		MMIOCR_SetCurCaptureOSDMode(OSD_OCR_WIN_CAPTURE_RECOG_START);
		MMIOCR_UpdateCurCaptureOSD();
		MMIOCR_StartRecogTimer();
		MMIOCR_StartRecogTimeOutTimer();
#ifdef WIN32
		MMK_PostMsg(MMIOCR_CAPTURE_WIN_ID,MSG_OCR_RECOG_OVER,PNULL,PNULL);
		return;
#endif
		if(OCR_NONE_ERROR!=OCR_AutoRecog()) //call ocr lib for recognize.
		{
		   //SCI_TRACE_LOW:"[MMIOCR]:mmiocr_wintab.c:OCR_AutoRecog fail"
		   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_WINTAB_812_112_2_18_2_42_11_28,(uint8*)"");
		}
		break;
	default:
		
		break;
	}
}


/*****************************************************************************/
// 	Description : use DC Driver interface to start preview
//	Global resource dependence : 
//  Author: gang.tong
//	Note:	这个函数用于Stop了Preview以后，再次Start
/*****************************************************************************/
PUBLIC BOOLEAN MMIOCR_StartPreview(DC_OWNER_E owner)
{
	DCAMERA_RETURN_VALUE_E	ret = DCAMERA_OP_SUCCESS;
	DCAMERA_PREVIEW_PARAM_T preview_param = {0};
	uint32  preview_flag = MMIAPIDC_GetPreviewflagData();
	
	if(MMIDC_IsDCOpened())
	{	
		if(0 == preview_flag)
		{
			OCRAdap_GetPreviewParam(&preview_param);
			
			ret = DCAMERA_StartPreview(&preview_param);
			
			if(DCAMERA_OP_SUCCESS == ret)
			{
				/* set flag */
				preview_flag = preview_flag | owner;
			}
		}	
		else
		{
			/* set flag */
			preview_flag = preview_flag | owner;		
		}
		MMIAPIDC_SetPreviewflagData(preview_flag);
		//SCI_TRACE_LOW:"[MMIOCR]: Start Preview retrun: 0x%.2x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_WINTAB_852_112_2_18_2_42_11_29,(uint8*)"d", ret);
	}
	return (DCAMERA_OP_SUCCESS == ret);
}

/*****************************************************************************/
// 	Description : get preview param
//	Global resource dependence : 
//  Author: gang.tong
//	Note:	for MMI upper level
/*****************************************************************************/
LOCAL BOOLEAN OCRAdap_GetPreviewParam(DCAMERA_PREVIEW_PARAM_T * preview_param_ptr)
{
	GUI_LCD_DEV_INFO dev_info = MMIDC_GetReviewBlockInfo();
    
    GUI_RECT_T screen_rect = {0};
    
	//SCI_ASSERT(PNULL != preview_param_ptr);/*assert verified*/

	if(MMITHEME_IsTVOpen()||PNULL == preview_param_ptr)
	{
		return FALSE;
	}
	
	screen_rect.top = MMI_OCR_PREVIEW_START_Y;
	screen_rect.left = 0;
	screen_rect.right = MMI_OCR_PREVIEW_WIDTH - 1;
	screen_rect.bottom = MMI_OCR_PREVIEW_START_Y + MMI_OCR_PREVIEW_HEIGHT - 1;
	GUIBLOCK_SetRect(screen_rect,&dev_info);
	
	preview_param_ptr->lcd_block_id	= GUI_BLOCK_1;
	preview_param_ptr->disp_rect.x	= MMI_OCR_PREVIEW_START_X;
	preview_param_ptr->disp_rect.y	= MMI_OCR_PREVIEW_START_Y;
	preview_param_ptr->disp_rect.w	= MMI_OCR_PREVIEW_WIDTH;
	preview_param_ptr->disp_rect.h	= MMI_OCR_PREVIEW_HEIGHT;
	
	preview_param_ptr->lcd_id		= GUI_MAIN_LCD_ID;

	preview_param_ptr->target_rect.x = 0;
	preview_param_ptr->target_rect.y = 0;	
	/** set photo size **/

	preview_param_ptr->target_rect.w = OCR_PHOTO_SIZE_1600X1200_W;
	preview_param_ptr->target_rect.h = OCR_PHOTO_SIZE_1600X1200_H;
	
	/** set other param **/
	preview_param_ptr->rotation_mode = DCAMERA_ROTATION_0;
	preview_param_ptr->distortion_mode =DCAMERA_DIST_MODE_NORMAL;// DCAMERA_DIST_MODE_TRIMMING;//
	
	return TRUE;
}

/*****************************************************************************/
// 	Description : Set all preview param
//	Global resource dependence : 
//  Author: jian.ma
//	Note:	
/*****************************************************************************/
LOCAL BOOLEAN MMIOCR_SetAllPreviewParam(void)
{
	BOOLEAN ret = TRUE;
	ret = ret && (DCAMERA_OP_SUCCESS == DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_BRIGHTNESS, BRIGHTNESS_LEVEL_6));
	ret = ret && (DCAMERA_OP_SUCCESS == DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_CONTRAST, CONTRAST_LEVEL_6));
	ret = ret && (DCAMERA_OP_SUCCESS == DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_ZOOM, ZOOM_LEVEL_0));
	ret = ret && (DCAMERA_OP_SUCCESS == DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_EFFECT, PHOTO_EFFECT_NORMAL));
	ret = ret && (DCAMERA_OP_SUCCESS == DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_PREVIEW_MODE, ENVIROMENT_INDOOR));
	return ret;
}


/*****************************************************************************/
// 	Description : draw focus win
//	Global resource dependence : 
//  Author: jian.ma
//	Note:	
/*****************************************************************************/
LOCAL void DrawFocusWin(uint32 color)
{
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
    
    GUI_RECT_T rect = { (MMI_MAINSCREEN_WIDTH - SHOOT_RECT_WIDTH) / 2,
                        (MMI_MAINSCREEN_HEIGHT - SHOOT_RECT_HEIGHT) / 2, (MMI_MAINSCREEN_WIDTH + SHOOT_RECT_WIDTH) / 2,
                        (MMI_MAINSCREEN_HEIGHT + SHOOT_RECT_HEIGHT) / 2};
    uint32 width_1_3 = SHOOT_RECT_WIDTH / 3;
    uint32 height_1_3 = SHOOT_RECT_HEIGHT / 3;
    
	LCD_DrawLine(&dev_info, (uint16)rect.left, (uint16)rect.top, (uint16)(rect.left + width_1_3), (uint16)rect.top, color);
	LCD_DrawLine(&dev_info, (uint16)rect.right - (uint16)width_1_3, (uint16)rect.top, (uint16)rect.right, (uint16)rect.top, color);
	
	LCD_DrawLine(&dev_info, (uint16)rect.right, (uint16)rect.top, rect.right, rect.top + height_1_3, color);
	LCD_DrawLine(&dev_info, (uint16)rect.right, (uint16)rect.bottom - height_1_3, rect.right, rect.bottom, color);
	
	LCD_DrawLine(&dev_info, (uint16)rect.left, (uint16)rect.bottom, rect.left + width_1_3, rect.bottom, color);
	LCD_DrawLine(&dev_info, (uint16)rect.right - width_1_3, (uint16)rect.bottom, rect.right, rect.bottom, color);
	
	LCD_DrawLine(&dev_info, (uint16)rect.left, rect.top, rect.left, rect.top + height_1_3, color);
	LCD_DrawLine(&dev_info, (uint16)rect.left, rect.bottom - height_1_3, rect.left, rect.bottom, color);
	
	GUILCD_Invalidate(GUI_MAIN_LCD_ID, GUIREF_GetUpdateBlockSet(dev_info.block_id));
	SCI_Sleep(100);
	
	LCD_DrawRect(&dev_info, rect, MMI_OSD_TRANSPARENT_COLOR);
	GUILCD_Invalidate(GUI_MAIN_LCD_ID, GUIREF_GetUpdateBlockSet(dev_info.block_id));
	SCI_Sleep(100);
	
	LCD_DrawLine(&dev_info, (uint16)rect.left, (uint16)rect.top, (uint16)rect.left + width_1_3, rect.top, color);
	LCD_DrawLine(&dev_info, (uint16)rect.right - width_1_3, (uint16)rect.top, rect.right, rect.top, color);
	
	LCD_DrawLine(&dev_info, (uint16)rect.right, rect.top, rect.right, rect.top + height_1_3, color);
	LCD_DrawLine(&dev_info, (uint16)rect.right, (uint16)rect.bottom - height_1_3, rect.right, rect.bottom, color);
	
	LCD_DrawLine(&dev_info, (uint16)rect.left, (uint16)rect.bottom, rect.left + width_1_3, rect.bottom, color);
	LCD_DrawLine(&dev_info, (uint16)rect.right - width_1_3, rect.bottom, rect.right, rect.bottom, color);
	
	LCD_DrawLine(&dev_info, (uint16)rect.left, (uint16)rect.top, rect.left, rect.top + height_1_3, color);
	LCD_DrawLine(&dev_info, (uint16)rect.left, (uint16)rect.bottom - height_1_3, rect.left, rect.bottom, color);
	MMITHEME_StoreUpdateRect(&dev_info, rect);
}

/*****************************************************************************/
// 	Description : set auto focus param
//	Global resource dependence : 
//  Author: jian.ma
//	Note:	
/*****************************************************************************/
LOCAL void SetAutoFocusParam(void)
{
	GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();

	GUI_RECT_T rect = {MMI_MAINSCREEN_WIDTH / 2 - 20, MMI_MAINSCREEN_HEIGHT / 2 - 55,
		                MMI_MAINSCREEN_WIDTH / 2 + 20, MMI_MAINSCREEN_HEIGHT / 2 + 55};
	DCAMERA_AF_INFO_T af_param = {0};
	
	MMIOCR_UpdateCurPreviewOSD();
	MMITHEME_StoreUpdateRect(&dev_info, rect);
	af_param.enable_af = 1;
	af_param.af_callback_func = (DCAMERA_AF_CALLBACK_FUNC)AutoFocusCallback;
	af_param.af_rect.x = 144;
	af_param.af_rect.y = 224;
	af_param.af_rect.w = 240;
	af_param.af_rect.h = 320;
	af_param.af_single = 1;
	DCAMERA_SetAutoFocus(&af_param);   
}

#endif
