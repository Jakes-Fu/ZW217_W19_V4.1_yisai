/*****************************************************************************
** File Name:      ocr.c                                                      *
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
#include "std_header.h"
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

MMI_APPLICATION_T	    g_ocr_app;

#ifdef MMI_OCR_ENABLE

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIOCR_RECOG_MAX_TIME_OUT_NUM	30000
#define MMIOCR_RECOG_PROCESS_TIMER_NUM	500
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL MMI_WIN_ID_T s_review_win_id = 0;
LOCAL const GUI_SIZE_T s_photo_rect = {0, 0, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT};
LOCAL OCR_DATA_INFO_T   		s_ocr_data_info = {0};	
LOCAL OSD_OCR_WIN_CAPTURE_MODE_E s_cur_capture_osd_mode = OSD_OCR_WIN_CAPTURE_NONE;
LOCAL uint32		s_ocr_process_timer_id = 0;
LOCAL uint32		s_ocr_timeout_timer_id = 0;
LOCAL uint32		s_recog_num = 0;//recog card info size num
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : capture call back
//	Global resource dependence : 
//  Author: jian.ma 
//	Note:
/*****************************************************************************/
LOCAL uint32 OCRAdap_CaptureCallBack(uint32 ret, uint16* data_buffer, uint32 data_len);

/*****************************************************************************/
// 	Description : do snap shoot capture
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN OCRSnapShot(MMIDC_SETTING_INFO_T const *setting_ptr);

/*****************************************************************************/
// 	Description : review photo
//	Global resource dependence : 
//  Author:gang.tong
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN OCRReviewPhotoFromBuf(uint8 const * photo_ptr, uint32 photo_size, GUI_SIZE_T const * disp_rect_ptr);

/*****************************************************************************/
// 	Description : review one photo of multi photos buffer
//	Global resource dependence : 
//  Author:gang.tong
//	Note: id start from 0
/*****************************************************************************/
LOCAL BOOLEAN MMIOCR_ReviewSpecialCapturedPhotoBuf(uint32 owner_win_id);

/*****************************************************************************/
// 	Description : callback function of review 
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL uint32 OCRReviewPhotoCallBack(uint32 back_param);

/*****************************************************************************/
// 	Description : recode the win id of review photo window
//	Global resource dependence : 
//  Author:gang.tong
//	Note: id from 0
/*****************************************************************************/
LOCAL void SetReviewWinID(MMI_WIN_ID_T owner_win_id);

/*****************************************************************************/
// 	Description : get the win id of review photo window
//	Global resource dependence : 
//  Author:gang.tong
//	Note: id from 0
/*****************************************************************************/
LOCAL MMI_WIN_ID_T GetReviewWinID(void);

/*****************************************************************************/
// 	Description : Handle the signal including auto focus ,layout ,recognize . 
//	Global resource dependence : 
//  Author:Annie.an 
//	Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIOCR_HandleOCRMsg (
										 PWND    app_ptr, 
										 uint16  msg_id, 
										 DPARAM  param
										 )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
	//SCI_TRACE_LOW:"[MMIOCR]:MMIOCR_HandleOCRMsg: msg_id %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_108_112_2_18_2_42_5_0,(uint8*)"d",msg_id);
	switch (msg_id)
	{
	case  DC_AF_SINGLE_OVER:
	
		break;
		
	case  OCR_LAYOUT:
		break;
		
	case OCR_RECOG:
        if(MMK_IsOpenWin(MMIOCR_CAPTURE_WIN_ID)&&MMK_IsFocusWin(MMIOCR_CAPTURE_WIN_ID))
        {	
            MMK_PostMsg(MMIOCR_CAPTURE_WIN_ID,MSG_OCR_RECOG_OVER,PNULL,PNULL);
            
        }
        else
        {
            MMK_PostMsg(MMIOCR_CAPTURE_WIN_ID,MSG_OCR_RECOG_FAIL,PNULL,PNULL);
           
        }
        break; 
		
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
	
	return (result);
}
/*****************************************************************************/
// 	Description : do capture
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIOCR_Capture(MMIDC_SETTING_INFO_T const *setting_ptr)
{
	//SCI_ASSERT(PNULL != setting_ptr);/*assert verified*/
#ifdef WIN32
	MMK_PostMsg(MMIOCR_CAPTURE_WIN_ID, MSG_DC_CAPTURE_SUCCESS_MSG, (DPARAM)PNULL, 0);
	return TRUE;
#else	
#endif
	/** not burst shot **/
	return OCRSnapShot(setting_ptr);
}


/*****************************************************************************/
// 	Description : do snap shoot capture
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN OCRSnapShot(MMIDC_SETTING_INFO_T const *setting_ptr)
{
	DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
	DCAMERA_SNAPSHOT_PARAM_T snapshot_param = {0};
	DCAMERA_SNAPSHOT_RETURN_PARAM_T snapshot_return = {0};

	//SCI_ASSERT(PNULL != setting_ptr);/*assert verified*/
	
	MMIDC_OpenDC(DC_FOR_CAPTURE);
	MMIOCR_StartPreview(DC_FOR_CAPTURE);
	
	snapshot_param.data_type		    = DCAMERA_DATA_TYPE_YUV;
	snapshot_param.buf_len			    = MMIDC_MAX_PHOTO_SIZE;
	snapshot_param.data_buf			    = MMIDC_GetImageBuffer();

	snapshot_param.file_name			= PNULL;
	snapshot_param.snapshot_callback	= (DCAMERA_CALL_BACK)OCRAdap_CaptureCallBack;

	snapshot_param.enc_quality_level	= DC_PHOTO_QUALITY_8;

	
	snapshot_param.flash_enable			= 0;
	snapshot_param.personal_snapshot.select_personal = 0;	
	
	ret = DCAMERA_DoSnapshot(&snapshot_param, &snapshot_return);	
	//SCI_TRACE_LOW:"[MMIOCR]: SnapShot return: 0x%.2x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_184_112_2_18_2_42_6_1,(uint8*)"d", ret);
	if((DCAMERA_OP_SUCCESS == ret))
	{

	}
	else
	{
		MMIDC_StopPreview(DC_FOR_CAPTURE);
		//MMIDC_SetPhotoLen(0, 0);
	}

	if(FRAME_DISABLE != MMIDC_GetPhotoFrameStatus())
	{
		//MMIDC_UpdatePhotoSize();
	}
	return (DCAMERA_OP_SUCCESS == ret);
}

/*****************************************************************************/
// 	Description : capture call back
//	Global resource dependence : 
//  Author: jian.ma 
//	Note:
/*****************************************************************************/
LOCAL uint32 OCRAdap_CaptureCallBack(uint32 ret, uint16* data_buffer, uint32 data_len)
{

	//SCI_TRACE_LOW:"[MMIOCR]: DCAdap_CaptureCallBack, data_len =%d ret = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_209_112_2_18_2_42_6_2,(uint8*)"dd", data_len,ret);

	if (DCAMERA_OP_SUCCESS == ret)
	{
		s_ocr_data_info.data_buffer = (uint8*)data_buffer;
		s_ocr_data_info.data_len = data_len;
		MMK_PostMsg(MMIOCR_CAPTURE_WIN_ID, MSG_DC_CAPTURE_SUCCESS_MSG, (DPARAM)PNULL, 0);
	}
	else
	{
		MMK_PostMsg(MMIOCR_CAPTURE_WIN_ID, MSG_DC_CAPTURE_FAILE_MSG, (DPARAM)PNULL, 0);
	}

	/** increase the number of captured photo **/	
	MMIDC_StopPreview(DC_FOR_CAPTURE);
	
	return 0;
}


/*****************************************************************************/
// 	Description : review the captured photos
//	Global resource dependence : 
//  Author:gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIOCR_ReviewCapturedPhotos(uint32 owner_win_id, MULTI_SHOT_E multi_shot, BOOLEAN is_first_review, BOOLEAN is_full_rect)
{		
	if(!MMIOCR_ReviewSpecialCapturedPhotoBuf(owner_win_id))
	{
		//SCI_TRACE_LOW:"[MMIOCR]: MMIOCR_ReviewCapturedPhotos, review failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_237_112_2_18_2_42_6_3,(uint8*)"");
		return FALSE;
	}
	return TRUE;
}



/*****************************************************************************/
// 	Description : review one photo of multi photos buffer
//	Global resource dependence : 
//  Author:gang.tong
//	Note: id start from 0
/*****************************************************************************/
LOCAL BOOLEAN MMIOCR_ReviewSpecialCapturedPhotoBuf(uint32 owner_win_id)
{				
	/** set the win id of review photo window **/
	SetReviewWinID(owner_win_id);
#ifdef WIN32
	MMK_PostMsg(GetReviewWinID(), MSG_DC_REVIEW_SUCCESS_MSG, (DPARAM)PNULL, 0);
	return TRUE;
#endif
	
	return OCRReviewPhotoFromBuf(s_ocr_data_info.data_buffer,s_ocr_data_info.data_len, &s_photo_rect);		
}

/*****************************************************************************/
// 	Description : review photo
//	Global resource dependence : 
//  Author:gang.tong
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN OCRReviewPhotoFromBuf(uint8 const * photo_ptr, uint32 photo_size, GUI_SIZE_T const * disp_rect_ptr)
{
	DCAMERA_IMAGE_REVIEW_PARAM_T	review_param = {0};
	DCAMERA_RETURN_VALUE_E	ret = DCAMERA_OP_SUCCESS;
		
	if(PNULL == photo_ptr)
	{
		return FALSE;
	}
	//SCI_ASSERT(PNULL != disp_rect_ptr);/*assert verified*/
	
	review_param.lcd_id = GUI_MAIN_LCD_ID;
	review_param.image_type = DCAMERA_DATA_TYPE_YUV;
	review_param.lcd_block_id = GUI_BLOCK_1;
	review_param.disp_rect.x = MMI_OCR_PREVIEW_START_X;
	review_param.disp_rect.y = MMI_OCR_PREVIEW_START_Y;
	review_param.disp_rect.w = MMI_OCR_PREVIEW_WIDTH;
	review_param.disp_rect.h = MMI_OCR_PREVIEW_HEIGHT;
	review_param.image_buffer = (uint8*)photo_ptr;
	review_param.buffer_len = photo_size;
	review_param.file_name = PNULL;

	MMIOCR_GetImageRect(&review_param.image_disp_rect);
	
	review_param.image_src_rect.x = 0;
	review_param.image_src_rect.y = 0;
	review_param.image_src_rect.w = review_param.image_disp_rect.w;
	review_param.image_src_rect.h = review_param.image_disp_rect.h;

	//SCI_TRACE_LOW:"[MMIOCR]: review_param image_src_rect w=%d,h=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_294_112_2_18_2_42_6_4,(uint8*)"dd", review_param.image_src_rect.w,review_param.image_src_rect.h);
	
	review_param.review_callback = (DCAMERA_CALL_BACK)OCRReviewPhotoCallBack;
	review_param.rotation_mode = 0;

	review_param.distortion_mode = DCAMERA_DIST_MODE_NORMAL;//DCAMERA_DIST_MODE_NORMAL;//DCAMERA_DIST_MODE_TRIMMING;

	if(MMIDC_OpenDC(DC_FOR_REVIEW))
	{
#ifndef WIN32
		ret = DCAMERA_ReviewImage(&review_param);
#endif
		//SCI_TRACE_LOW:"[MMIOCR]: ReviewPhotoFromBuf return 0x%.2x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_306_112_2_18_2_42_6_5,(uint8*)"d", ret);
	}
	else
	{
		
		return FALSE;
	}	
	
	return (DCAMERA_OP_SUCCESS == ret);
}


/*****************************************************************************/
// 	Description : callback function of review 
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL uint32 OCRReviewPhotoCallBack(DCAMERA_RETURN_VALUE_E ret_value, void* param_ptr, uint32 param_size)
{
	//SCI_TRACE_LOW:"[MMIOCR]: ReviewPhotoCallBack, ret_value = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_324_112_2_18_2_42_6_6,(uint8*)"d", ret_value);
	switch(ret_value)
	{
	case 0:		// success
		MMK_PostMsg(GetReviewWinID(), MSG_DC_REVIEW_SUCCESS_MSG, (DPARAM)PNULL, 0);
		break;
	case 1:		// faile
		MMK_PostMsg(GetReviewWinID(), MSG_DC_REVIEW_FAILE_MSG, (DPARAM)PNULL, 0);
		break;
	default:
	    //SCI_PASSERT(0,("ret_value = %d",ret_value));/*assert verified*/
		break;
	}
	MMIDC_CloseDC(DC_FOR_REVIEW);
	return 0;
}


/*****************************************************************************/
// 	Description : get preview param
//	Global resource dependence : 
//  Author: gang.tong
//	Note:	for MMI upper level
/*****************************************************************************/
PUBLIC BOOLEAN MMIOCR_GetImageRect(DCAMERA_RECT_T *image_rect)
{
	//SCI_ASSERT(PNULL != image_rect);/*assert verified*/

    if(PNULL != image_rect)
    {
        image_rect->x = 0;
        image_rect->y = 0;
        image_rect->w = OCR_PHOTO_SIZE_1600X1200_W;
        image_rect->h = OCR_PHOTO_SIZE_1600X1200_H;
    }
	
	return TRUE;
}


/*****************************************************************************/
 // 	Description : According the result t.
 //	Global resource dependence : none
 //  Author: Annie.an
 //	Note:
 /*****************************************************************************/
 PUBLIC void MMIOCR_DisplayResultInPB(OCR_CARD_INFO *result)
 { 

	 int32               i=0;
	 int32 				 j= 0;
	 uint32              length=0;
	 uint8				 buffer[MMIPB_BCD_NUMBER_MAX_LEN*2]= {0};
	 MMIOCR_INFO_ENTRY_T  ocr_info = { 0 };
	 uint32 			 recog_num = MMIOCR_GetRecogCardInfoSizeNum();
	 BOOLEAN 			 flag[5]={0};//phone,email只保存第一条信息，中文姓名优先于英文姓名

     for( i=0;i<recog_num;i++)
	 { 
	 	//SCI_TRACE_LOW:"[MMIOCR]:result[%d].wClass[0] = 0x%x"
	 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_380_112_2_18_2_42_6_7,(uint8*)"dd",i,result[i].wClass[0]);
	 
		 switch(result[i].wClass[0])
		 {	
			 case CARD_AREA_NAME_ENG:			 	 
			 	 if (flag[0])
				 {
				 	//SCI_TRACE_LOW:"[MMIOCR]:enter ENG name processs flag[0]= %d"
				 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_387_112_2_18_2_42_6_8,(uint8*)"d",flag[0]);
				 	break;
				 }					
				 length=MMIAPICOM_Wstrlen((wchar*)result[i].String);
				 SCI_MEMSET(buffer,0x00,sizeof(buffer));

				 MMIAPICOM_Wchar2char((char*)buffer,(wchar*)result[i].String);			
				 
				 ocr_info.entry_name.is_ucs2 = FALSE;
				 ocr_info.entry_name.alpha_len =(length > MMIPB_NAME_MAX_LEN) ? MMIPB_NAME_MAX_LEN :length;
				 SCI_MEMSET(ocr_info.entry_name.alpha, 0x00, MMIPB_NAME_MAX_LEN*sizeof(uint8));
				 SCI_MEMCPY(ocr_info.entry_name.alpha,buffer,ocr_info.entry_name.alpha_len);	
				 break;
			 case CARD_AREA_NAME_CHN:	
				
				 length=MMIAPICOM_Wstrlen((wchar*)result[i].String);
				 ocr_info.entry_name.is_ucs2=TRUE;
				 ocr_info.entry_name.alpha_len=(length*2 > MMIPB_NAME_MAX_LEN) ? MMIPB_NAME_MAX_LEN : length*2;			
				 SCI_MEMSET(ocr_info.entry_name.alpha, 0x00, MMIPB_NAME_MAX_LEN*sizeof(uint8));
				 SCI_MEMCPY(ocr_info.entry_name.alpha,(uint8*)(result[i].String),ocr_info.entry_name.alpha_len);
				 flag[0] = TRUE;		
				 break;
				 
			 case CARD_AREA_EMAIL:					
				 if (flag[1])
				 {
				 	//SCI_TRACE_LOW:"[MMIOCR]:enter email process flag[1]= %d"
				 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_413_112_2_18_2_42_6_9,(uint8*)"d",flag[1]);
				 	break;
				 }
				 ocr_info.mail.is_ucs2=FALSE;
				 length=MMIAPICOM_Wstrlen((wchar*)result[i].String);

				 SCI_MEMSET(buffer,0x00,sizeof(buffer));
				
				 MMIAPICOM_Wchar2char((char*)buffer,(wchar*)result[i].String);
				 ocr_info.mail.mail_len=(length > MMIPB_MAX_MAIL_LEN) ? MMIPB_MAX_MAIL_LEN : length;
				 SCI_MEMSET(ocr_info.mail.mail, 0x00, MMIPB_MAX_MAIL_LEN*sizeof(uint8));
				 SCI_MEMCPY(ocr_info.mail.mail,(uint8*)buffer,ocr_info.mail.mail_len);	
				 flag[1] = TRUE;		
				 break;	
			 case CARD_AREA_MOBILE:
				
				 if (flag[2])
				 {
				 	//SCI_TRACE_LOW:"[MMIOCR]:enter mobile number process flag[2]= %d"
				 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_431_112_2_18_2_42_6_10,(uint8*)"d",flag[2]);
				 	break;
				 }
				 length=MMIAPICOM_Wstrlen((wchar*)result[i].String);
				 length = (length>MMIPB_BCD_NUMBER_MAX_LEN)?MMIPB_BCD_NUMBER_MAX_LEN:length;
				
				 SCI_MEMSET(buffer,0x00,sizeof(buffer));
				 SCI_MEMSET(ocr_info.personal_number,0x00,sizeof(ocr_info.personal_number));
				 
				 
				 GUI_UCS2GB(buffer,(const uint8*)result[i].String,(uint16)(length*2));
				 SCI_MEMSET(ocr_info.personal_number,0x00,sizeof(ocr_info.personal_number));
				 MMIAPICOM_GetPhoneNumberFormString(buffer,length,ocr_info.personal_number);
				 
				 
				 flag[2] = TRUE;
				 break;
			 case CARD_AREA_PHONE:
				 length=MMIAPICOM_Wstrlen((wchar*)result[i].String);
				 if (flag[3])
				 {
				 	//SCI_TRACE_LOW:"[MMIOCR]:enter company number process flag[3]= %d"
				 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_452_112_2_18_2_42_6_11,(uint8*)"d",flag[3]);
				 	break;
				 }
				 
				 length = (length>MMIPB_BCD_NUMBER_MAX_LEN)?MMIPB_BCD_NUMBER_MAX_LEN:length;	 
				 SCI_MEMSET(buffer,0x00,sizeof(buffer));
				 SCI_MEMSET(ocr_info.company_number,0x00,sizeof(ocr_info.company_number));
				 GUI_UCS2GB(buffer,(const uint8*)result[i].String,(uint16)(length*2));
				 
				 SCI_MEMSET(ocr_info.company_number,0x00,sizeof(ocr_info.company_number));
				 MMIAPICOM_GetPhoneNumberFormString(buffer,length,ocr_info.company_number);
				 
				 
				 flag[3] = TRUE;
				 break;	
		     case CARD_AREA_ZIPCODE:
		     	 if (flag[4])
				 {
				 	//SCI_TRACE_LOW:"[MMIOCR]:enter  zipcode  process flag[4]= %d"
				 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_470_112_2_18_2_42_6_12,(uint8*)"d",flag[4]);
				 	break;
				 }
				 length=MMIAPICOM_Wstrlen((wchar*)result[i].String);
				
				 SCI_MEMSET(buffer,0x00,sizeof(buffer));
				 SCI_MEMSET(&ocr_info.custom_text,0x00,sizeof(MMIPB_CUSTOM_TEXT_T));
				 ocr_info.custom_text.text_len = (length>MMIPB_MAX_CUSTOM_TEXT_LEN/2)?MMIPB_MAX_CUSTOM_TEXT_LEN/2:length;
				 ocr_info.custom_text.is_valid = TRUE;
				 ocr_info.custom_text.is_ucs2 = FALSE;

				 MMIAPICOM_Wchar2char((char*)buffer,(wchar*)result[i].String);
				 SCI_MEMCPY(ocr_info.custom_text.text_buf,buffer,ocr_info.custom_text.text_len);
				 flag[4] = TRUE;
				 break;
			 default:
				 break;
		 }
	 }
#ifdef WIN32 //for testing
{
	SCI_MEMCPY(ocr_info.entry_name.alpha,"haha",4);
	ocr_info.entry_name.alpha_len = 4;
	ocr_info.entry_name.is_ucs2 = FALSE;
	SCI_MEMCPY(ocr_info.personal_number,"860013912345678",15);
	SCI_MEMCPY(ocr_info.company_number,"(86) 06280028906280",19);
	MMIAPICOM_GetPhoneNumberFormString(ocr_info.company_number,19,buffer);

	SCI_MEMSET(ocr_info.company_number,0x00,sizeof(ocr_info.company_number));
	SCI_MEMCPY(ocr_info.company_number,buffer,strlen(buffer));
	SCI_MEMCPY(ocr_info.custom_text .text_buf,"201203",6);
	ocr_info.custom_text.is_ucs2 = FALSE;
	ocr_info.custom_text.is_valid = TRUE;
	ocr_info.custom_text.text_len = 6;

	SCI_MEMCPY(ocr_info.mail.mail,"test@spreadtrum.com",19);	
	ocr_info.mail.is_ucs2 = FALSE;
	ocr_info.mail.mail_len = 19;

	ocr_info.title.is_ucs2 = FALSE;
	ocr_info.title.title_len = 5;
	SCI_MEMCPY(ocr_info.title.title_text,"laoda",5);
		
	ocr_info.org.is_ucs2 = FALSE;
	ocr_info.org.org_len = 10;
	SCI_MEMCPY(ocr_info.org.org_text,"spreadtrum",ocr_info.org.org_len);
		
	ocr_info.work_adr.is_ucs2 = FALSE;
	ocr_info.work_adr.adr_len = 10;
	SCI_MEMCPY(ocr_info.work_adr.adr_text ,"zhangjiang",ocr_info.work_adr.adr_len);
}
#endif                      
	MMIAPIPB_AddContactToPhonebook(&ocr_info);
	
 }


 /*****************************************************************************/
// 	Description : get buf data
//	Global resource dependence : 
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC uint8* MMIOCR_GetBufData(void)
{
	return s_ocr_data_info.data_buffer;
}

/*****************************************************************************/
// 	Description : Set current capture osd mode
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMIOCR_SetCurCaptureOSDMode(OSD_OCR_WIN_CAPTURE_MODE_E cur_mode)
{
	//SCI_PASSERT(cur_mode < OSD_OCR_WIN_CAPTURE_MAX,("cur_mode = %d",cur_mode));/*assert verified*/
    if(cur_mode < OSD_OCR_WIN_CAPTURE_MAX)
    {
        s_cur_capture_osd_mode = cur_mode;
    }
}

/*****************************************************************************/
// 	Description : Get current capture osd mode
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC OSD_OCR_WIN_CAPTURE_MODE_E MMIOCR_GetCurCaptureOSDMode(void)
{

	return s_cur_capture_osd_mode;
}

/*****************************************************************************/
// 	Description : recode the win id of review photo window
//	Global resource dependence : 
//  Author:gang.tong
//	Note: id from 0
/*****************************************************************************/
LOCAL void SetReviewWinID(MMI_WIN_ID_T owner_win_id)
{
	s_review_win_id = owner_win_id;
}

/*****************************************************************************/
// 	Description : get the win id of review photo window
//	Global resource dependence : 
//  Author:gang.tong
//	Note: id from 0
/*****************************************************************************/
LOCAL MMI_WIN_ID_T GetReviewWinID(void)
{
	return s_review_win_id;
}

/*****************************************************************************/
// 	Description : start recog timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIOCR_StartRecogTimer(void)
{
	if (0 ==s_ocr_process_timer_id)
	{
		s_ocr_process_timer_id = MMK_CreateTimerCallback( MMIOCR_RECOG_PROCESS_TIMER_NUM, MMIAPIOCR_HandleProcessTimer, NULL, FALSE);
	}
}


/*****************************************************************************/
// 	Description : stop recog timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIOCR_StopRecogTimer(void)
{
	if( 0!=s_ocr_process_timer_id)
	{
		MMK_StopTimer((uint8)s_ocr_process_timer_id);
		s_ocr_process_timer_id=0;
	}
}

/*****************************************************************************/
// 	Description : start recog timerout timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIOCR_StartRecogTimeOutTimer(void)
{
	if (0 ==s_ocr_timeout_timer_id)
	{
		s_ocr_timeout_timer_id = MMK_CreateTimerCallback( MMIOCR_RECOG_MAX_TIME_OUT_NUM, MMIAPIOCR_HandleProcessTimer, NULL, FALSE);
	}
}

/*****************************************************************************/
// 	Description : stop recog time out timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIOCR_StopRecogTimeOutTimer(void)
{
	if( 0!=s_ocr_timeout_timer_id)
	{
		MMK_StopTimer((uint8)s_ocr_timeout_timer_id);
		s_ocr_timeout_timer_id=0;
	}
}

/*****************************************************************************/
// 	Description : stop all timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIOCR_StopAllTimer(void)
{
	MMIOCR_StopRecogTimer();
	MMIOCR_StopRecogTimeOutTimer();
}

/*****************************************************************************/
// 	Description : Set card info size num
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIOCR_SetRecogCardInfoSizeNum(uint32 recog_num)
{
	s_recog_num = recog_num;
}


/*****************************************************************************/
// 	Description : Get card info size num
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIOCR_GetRecogCardInfoSizeNum(void)
{
	
	return s_recog_num;
}

#endif

