/*****************************************************************************
** File Name:      ocr_export.c                                                      *
** Author:         jian.ma                                                   *
** Date:           06/2007                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to define the OCR apis for other moudle  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2007       jian.ma             Create                                  *
******************************************************************************/



/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_ocr_trc.h"
#ifdef MMI_OCR_ENABLE
#include "Mmk_app.h"
#include "mmiocr_export.h"
#include "mmiocr_internal.h"
#include "mmiocr_id.h"
#include "mmi_appmsg.h"
#include "mmiocr_lib.h"
#include "mmipb_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmk_timer.h"

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
extern MMI_APPLICATION_T	    g_ocr_app;
extern uint32		s_ocr_process_timer_id = 0;
extern uint32		s_ocr_timeout_timer_id = 0;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : open preview window 
//	Global resource dependence : none
//  Author: jian.ma
//	Note: main process is done in ocr priveiw window.
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOCR_OpenPreviewWin(void)
{
	if(MMITHEME_IsTVOpen())
	{
		return FALSE;  
	}

	if(MMIDC_AllocJpegBuf())
	{
		if(!MMIOCR_OpenPreviewWin())
		{
			MMIDC_FreeJpegBuf();
			return FALSE;
		}
	}	
	return TRUE;
}

/*****************************************************************************/
// 	Description : close ocr preview win
//	Global resource dependence : 
//  Author: jian.ma
//	Note:	
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOCR_ClosePreviewWinOpen(void)
{
    if (MMK_IsOpenWin(MMIOCR_PREVIEW_WIN_ID))
    {
        MMK_CloseWin(MMIOCR_PREVIEW_WIN_ID);
	}
	return TRUE;
}

/*****************************************************************************/
// 	Description : handle the timer msg of OCR window
//	Global resource dependence : none
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOCR_HandleProcessTimer (uint8 timer_id,  uint32 param )
{
	BOOLEAN    result = FALSE;
	uint32	   pubwin_time = 3000;
	//SCI_TRACE_LOW:"[MMIOCR]: MMIAPIOCR_HandleProcessTimer %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_EXPORT_81_112_2_18_2_42_7_13,(uint8*)"d",timer_id);
    if (s_ocr_process_timer_id == timer_id)
    {
        result = TRUE;
		MMIOCR_StopRecogTimer();
		MMIOCR_SetCurCaptureOSDMode(OSD_OCR_WIN_CAPTURE_RECOGING);
		MMIOCR_UpdateCurCaptureOSD();
		MMIOCR_StartRecogTimer();
    }
	
	if (s_ocr_timeout_timer_id == timer_id)
	{
		result = TRUE;
		MMIOCR_StopRecogTimer();
		MMIOCR_StopRecogTimeOutTimer();
		MMIPUB_OpenAlertWinByTextId(&pubwin_time,TXT_OCR_TIME_OUT,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
		if(MMK_IsOpenWin(MMIOCR_CAPTURE_WIN_ID))
		{
			MMK_PostMsg(MMIOCR_CAPTURE_WIN_ID,MSG_OCR_RECOG_FAIL,PNULL,PNULL);
			MMIAPICOM_TriggerMMITask();
		}
	}
    return (result);
}



/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: Annie.an
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIOCR_Init(void)
{   

	g_ocr_app.ProcessMsg = MMIOCR_HandleOCRMsg;
//	g_ocr_app.component_type = CT_APPLICATION;
}


#else               /*------------dummy------------*/
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "mmk_type.h"

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : open preview window 
//	Global resource dependence : none
//  Author: jian.ma
//	Note: main process is done in ocr priveiw window.
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOCR_OpenPreviewWin(void)
{
    return TRUE;
}

/*****************************************************************************/
// 	Description : close ocr preview win
//	Global resource dependence : 
//  Author: jian.ma
//	Note:	
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOCR_ClosePreviewWinOpen(void)
{
    return TRUE;
}

/*****************************************************************************/
// 	Description : handle the timer msg of OCR window
//	Global resource dependence : none
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOCR_HandleProcessTimer (uint8 timer_id,  uint32 param )
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: Annie.an
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIOCR_Init(void)
{
}

#endif

