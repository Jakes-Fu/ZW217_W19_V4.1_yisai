/****************************************************************************
** File Name:      mmimms_edit.c                                           *
** Author:         aoke.hu                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        aoke.hu          create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmi_app_mms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mn_type.h"
#include "mmimms_edit.h"
#include "mmicc_export.h"
#include "mmimtv_export.h"
#include "gui_ucs2b_converter.h "
#include "mMmsApi.h"
#include "mmimms_export.h"
#include "mmiudisk_export.h"
#include "mmimms_file.h"
#include "mmimms_control_main.h"
#include "mmimms_setting.h"
#include "mmimms_text.h"
#include "sysMemFile.h"
#include "img_dec_interface.h"
#include "mmipub.h"
#include "mmifmm_export.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#include "dal_player.h"
#endif
#ifdef DRM_SUPPORT
#include "drm_api.h"
#include "mmi_drmfilemgr.h"
#include "mmidrm_export.h"
#endif
#include "mmi_modu_main.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIMMS_EDITHEAD_BUFFER_LEN      40
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/
typedef struct 
{
    uint16 mcc;
    uint16 mnc;
}MMS_NAME_MCC_MNC_T;

static const MMS_NAME_MCC_MNC_T s_nigeria_mcc_mnc_table[] = 
{
    {621,20},
    {621,30},
    {621,40},
    {621,50},
    {621,60},
}; //尼日尼亚

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : image type convert 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL uint32 ImageTypeConvert(GUIANIM_TYPE_E multim_img_type);

/*****************************************************************************/
//  Description : audio type convert 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL uint32 AudioTypeConvert(MMISRVAUD_RING_FMT_E multim_audio_type);

/*****************************************************************************/
//  Description : video type convert 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL uint32 VideoTypeConvert(MMICOM_VIDEO_TYPE_E multim_video_type);

/*****************************************************************************/
//  Description : audio type convert 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL MMISRVAUD_RING_FMT_E MMSTypeConvertToAudioType(int8 mms_audio_type);

/*****************************************************************************/
//  Description : 生成虚拟文件名（随随机的ASCII字串加后缀）
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL uint16 MMIMMS_GenRandomNameWithSuffix(
                                             MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr,
                                             MMIFILE_FILE_INFO_T* file_info_ptr, 
                                             uint8 *multim_fname,
                                             uint32 multim_len,
                                             MMIFMM_FILE_TYPE_E   file_type,
                                             MMI_STRING_T *vacard_str
                                             );

/*****************************************************************************/
//  Description : 删除彩信结构中对应类型的内容(目前仅用来删除文本内容) 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void DelSlideContent(MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr, 
                           uint16 del_type
                           );

/*****************************************************************************/
//  Description : 判断type格式类型是否是file_type类型文件所支持的的格式 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsAvailableType(uint16    type,
                              MMIFMM_FILE_TYPE_E file_type   
                              );

/*****************************************************************************/
//  Description : add filename to name list
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void AddFileNameToNameList(MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr,
                                 wchar      *file_name_ptr,
                                 uint16     file_name_len,
                                 uint8      *vfname_ptr,
                                 uint16     vfname_len
                                 );

/*****************************************************************************/
//  Description : remove filename from name list
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void RemoveFileNameFromNameList(MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr,
                                      uint8      *vfname_ptr,
                                      uint16     vfname_len
                                      );

/*****************************************************************************/
//  Description : ReducePicForMMS
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReducePicForMMS(uint8 *bmp_ptr, uint32 bmp_size, uint16 *file_name_ptr, uint32 w, uint32 h);
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : image type convert 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL uint32 ImageTypeConvert(GUIANIM_TYPE_E multim_img_type)
{
    uint32 type = 0;
    
    switch(multim_img_type)
    {
    case GUIANIM_TYPE_IMAGE_WBMP:
        type = MMS_TYPE_WBMP;
        break;
        
    case GUIANIM_TYPE_IMAGE_BMP:
        type = MMS_TYPE_BMP1;
        break;
        
    case GUIANIM_TYPE_IMAGE_JPG:
        type = MMS_TYPE_JPEG;
        break;
        
    case GUIANIM_TYPE_IMAGE_GIF:
        type = MMS_TYPE_GIF;
        break;
        
    case GUIANIM_TYPE_IMAGE_PNG:
        type = MMS_TYPE_PNG;
        break;
        
    default:
        break;
    }
    
    return type;
}

/*****************************************************************************/
//  Description : audio type convert 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL uint32 AudioTypeConvert(MMISRVAUD_RING_FMT_E multim_audio_type)
{
    uint32 type = 0;

    switch(multim_audio_type)
    {
    case MMISRVAUD_RING_FMT_MIDI:
    case MMISRVAUD_RING_FMT_SMAF:     //as midi
        type = MMS_TYPE_MIDI;
        break;

    case MMISRVAUD_RING_FMT_AMR:
        type = MMS_TYPE_AMR;
        break;
        
    case MMISRVAUD_RING_FMT_WAVE:
        type = MMS_TYPE_WAV;
        break;
           
    case MMISRVAUD_RING_FMT_MP3:
        type = MMS_TYPE_MP3;
        break;

    case MMISRVAUD_RING_FMT_WMA:
        type = MMS_TYPE_WMA;
        break;

    case MMISRVAUD_RING_FMT_AAC:
        type = MMS_TYPE_AAC;
        break;

    case MMISRVAUD_RING_FMT_M4A:
        type = MMS_TYPE_M4A;
        break;

    default:
        break;
    }
    
    return type;
}

/*****************************************************************************/
//  Description : video type convert 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL uint32 VideoTypeConvert(MMICOM_VIDEO_TYPE_E multim_video_type)
{
    uint32 type = 0;  

    switch(multim_video_type)
    {
    case MMICOM_VIDEO_TYPE_3GP:
        type = MMS_TYPE_3GP;
        break;

    case MMICOM_VIDEO_TYPE_MP4:
        type = MMS_TYPE_MP4;
        break;
#ifdef AVI_DEC_SUPPORT        
    case MMICOM_VIDEO_TYPE_AVI:
        type = MMS_TYPE_AVI;
        break;
#endif
           
//    case MPEG4_TYPE_SDP:
// 类型不支持，需要调整MMS库
//        break;

    default:
        break;
    }
    
    return type;
}
/*****************************************************************************/
//  Description : audio type convert 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL MMISRVAUD_RING_FMT_E MMSTypeConvertToAudioType(int8 mms_audio_type)
{
    MMISRVAUD_RING_FMT_E type = MMISRVAUD_RING_FMT_NONE;

    switch(mms_audio_type)
    {
    case MMS_TYPE_WAV:
        type = MMISRVAUD_RING_FMT_WAVE;
        break;

    case MMS_TYPE_AMR:
        type = MMISRVAUD_RING_FMT_AMR;
        break;

    case MMS_TYPE_MIDI:
    case MMS_TYPE_IMY:
        type = MMISRVAUD_RING_FMT_MIDI;
        break;

    case MMS_TYPE_MP3:
        type = MMISRVAUD_RING_FMT_MP3;
        break;

    case MMS_TYPE_WMA:
        type = MMISRVAUD_RING_FMT_WMA;
        break;

    case MMS_TYPE_AAC:
        type = MMISRVAUD_RING_FMT_AAC;
        break;

    case MMS_TYPE_M4A:
        type = MMISRVAUD_RING_FMT_M4A;
        break;

    default:
        break;
    }
    
    return type;
}


/*****************************************************************************/
//  Description : 检测是否可以进入彩信编辑模块
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_EnterEditCheck(void)
{
    MMIMMS_ERROR_E result_val = MMIMMS_NO_ERROR;

    if(!MMIAPISMS_IsMMSOrderReady())
    {
        result_val = MMIMMS_ERROR_NOT_INIT;
    }
    else if(MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        result_val = MMIMMS_ERROR_UDISK_USING;
    }
//#ifndef BROWSER_SUPPORT_NETFRONT
//    else if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
//    {
//        result_val = MMIMMS_ERROR_CC_IN_CALL;
//    }
//#endif
//     else if(MMIAPIMTV_IsMTVRunning()) //CMMB播放中
//     {
//         result_val = MMIMMS_ERROR_CMMB_RUNNING;
//     }
    

    return result_val;
}

/*****************************************************************************/
//  Description : 删除彩信结构中对应类型的内容(目前仅用来删除文本内容) 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void DelSlideContent(MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr,
                           uint16 del_type)
{
    P_MMS_CONTENT_T content = PNULL;
    P_MMS_SLIDE_T   slide = PNULL;

    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr ||PNULL == editdoc_ptr->editbody_ptr->slide_cur)
    {
        return;
    }
    slide = editdoc_ptr->editbody_ptr->slide_cur;
    content = slide->content_root;
    while(content)
    {
        if(content->type == del_type)
        {
            slide->content_cur = content;
            MMS_DelContOfSlide(slide,editdoc_ptr->editbody_ptr);
            return;
        }
        content = content->next;
    }
}

/*****************************************************************************/
//  Description : 判断type格式类型是否是file_type类型文件所支持的的格式 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsAvailableType(uint16    type,
                              MMIFMM_FILE_TYPE_E file_type   
                              )
{
    BOOLEAN return_val = FALSE;
    
    switch(file_type)
    {
    case MMIFMM_FILE_TYPE_PICTURE:
        if (type == MMS_TYPE_GIF ||
            type == MMS_TYPE_JPEG ||
            type == MMS_TYPE_WBMP ||
            type == MMS_TYPE_BMP1 ||
            type == MMS_TYPE_PNG )
        {
            return_val = TRUE;
        }
    	break;

    case MMIFMM_FILE_TYPE_MOVIE:
        if (type == MMS_TYPE_MP4 ||
            type == MMS_TYPE_3GP ||
            type == MMS_TYPE_AVI
            )
        {
            return_val = TRUE;
        }
    	break;

    default:
        break;
    }
    
    return return_val;
}


/*****************************************************************************/
//  Description : 在主题前添加字串信息 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_AddStringBeforeSubject(MMI_STRING_T *subject, 
                                          const wchar * add_subject, 
                                          const uint16 add_subject_len)
{
    uint16  max_subject_len                  = 0;
    uint16  temp_len                         = 0;
    wchar   wstr_ptr[MMIMMS_MAX_SUBJECT_LEN] = {0};
    BOOLEAN is_ucs2                          = FALSE;
    

    if(PNULL == subject || add_subject_len >= MMIMMS_MAX_SUBJECT_LEN)
    {
        return;
    }
    
    if(PNULL == subject->wstr_ptr)
    {
        subject->wstr_ptr = MMIMMS_ALLOC(MMIMMS_MAX_SUBJECT_LEN*sizeof(wchar));
        if(PNULL != subject->wstr_ptr)
        {
            subject->wstr_len = add_subject_len;
            MMI_WSTRNCPY(subject->wstr_ptr,MMIMMS_MAX_SUBJECT_LEN, add_subject, add_subject_len, add_subject_len);
        }
    }
    else
    {
        is_ucs2 = !MMIAPICOM_IsASCIIString(subject->wstr_ptr, subject->wstr_len);
        
        if (is_ucs2)
        {
#ifdef CMCC_UI_STYLE
            max_subject_len = MMIMMS_MAX_SUBJECT_LEN/3;
#else
            max_subject_len = MMIMMS_MAX_SUBJECT_LEN/2;
#endif
            if (max_subject_len > add_subject_len)
            {     
                MMI_WSTRNCPY(wstr_ptr, MMIMMS_MAX_SUBJECT_LEN, subject->wstr_ptr, subject->wstr_len, subject->wstr_len);
                SCI_MEMSET(subject->wstr_ptr, 0, (MMIMMS_MAX_SUBJECT_LEN*sizeof(wchar)));
                MMI_WSTRNCPY(subject->wstr_ptr,(max_subject_len-1), add_subject, add_subject_len, add_subject_len);
                temp_len = MIN(subject->wstr_len,(max_subject_len-add_subject_len));
                MMIAPICOM_Wstrncpy(subject->wstr_ptr+add_subject_len,wstr_ptr,temp_len);
                subject->wstr_len = add_subject_len + temp_len;
            }
        }
        else
        {
            MMI_WSTRNCPY(wstr_ptr, MMIMMS_MAX_SUBJECT_LEN, subject->wstr_ptr, subject->wstr_len, subject->wstr_len);
            SCI_MEMSET(subject->wstr_ptr, 0, (MMIMMS_MAX_SUBJECT_LEN*sizeof(wchar)));
            MMI_WSTRNCPY(subject->wstr_ptr,(MMIMMS_MAX_SUBJECT_LEN-1), add_subject, add_subject_len, add_subject_len);
            temp_len = MIN(subject->wstr_len,(MMIMMS_MAX_SUBJECT_LEN-add_subject_len));
            MMIAPICOM_Wstrncpy(subject->wstr_ptr+add_subject_len,wstr_ptr,temp_len);
            subject->wstr_len = add_subject_len + temp_len;
        }        
    }
}


/*****************************************************************************/
//  Description : 设置主题
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetSubject(
                              const MMI_STRING_T *subject_ptr, 
                              MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                              )
{
    uint16  len = 0;
    
    if(PNULL == editdoc_ptr)
    {
        return;
    }

    if(PNULL == editdoc_ptr->edit_subject.wstr_ptr)
    {
        editdoc_ptr->edit_subject.wstr_ptr = MMIMMS_ALLOC(MMIMMS_MAX_SUBJECT_LEN * sizeof(wchar));
    }
    if(PNULL != editdoc_ptr->edit_subject.wstr_ptr)
    {
        SCI_MEMSET(editdoc_ptr->edit_subject.wstr_ptr, 0, (MMIMMS_MAX_SUBJECT_LEN * sizeof(wchar)));
        editdoc_ptr->edit_subject.wstr_len = 0;
        if(subject_ptr && subject_ptr->wstr_ptr && subject_ptr->wstr_len > 0)
        {
            len = MIN(subject_ptr->wstr_len, MMIMMS_MAX_SUBJECT_LEN);
            MMI_WSTRNCPY(editdoc_ptr->edit_subject.wstr_ptr,MMIMMS_MAX_SUBJECT_LEN,
                subject_ptr->wstr_ptr, subject_ptr->wstr_len, len);
            editdoc_ptr->edit_subject.wstr_len = len;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]: mms_no_memory"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_507_112_2_18_2_37_33_4,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : 设置收件人
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSTo(
                            const MMI_STRING_T *string_to_ptr, 
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                            )
{
    
    if(PNULL == editdoc_ptr)
    {
        return;
    }
    if(string_to_ptr->wstr_len > 0)
    {
        if(editdoc_ptr->edit_to.wstr_ptr)
        {
            MMIMMS_FREE(editdoc_ptr->edit_to.wstr_ptr);
            editdoc_ptr->edit_to.wstr_ptr = PNULL;
            editdoc_ptr->edit_to.wstr_len = 0;
        }
        editdoc_ptr->edit_to.wstr_ptr = MMIMMS_ALLOC((string_to_ptr->wstr_len + 1) * sizeof(wchar));
        if(PNULL != editdoc_ptr->edit_to.wstr_ptr)
        {
            wchar semicolon = ';';
            uint32 i = 0;
            uint32 temp_pos = 0;
            MMI_STRING_T temp_string_to = {0};
            temp_string_to.wstr_ptr = MMIMMS_ALLOC((string_to_ptr->wstr_len + 2) * sizeof(wchar));
            
            if(PNULL == temp_string_to.wstr_ptr)
            {
                return;
            }
            SCI_MEMSET(temp_string_to.wstr_ptr,0,(string_to_ptr->wstr_len + 2) * sizeof(wchar));
            SCI_MEMCPY(temp_string_to.wstr_ptr,string_to_ptr->wstr_ptr,string_to_ptr->wstr_len * sizeof(wchar)) ;
            temp_string_to.wstr_len = string_to_ptr->wstr_len;
            if(semicolon != string_to_ptr->wstr_ptr[string_to_ptr->wstr_len - 1])
            {               

                temp_string_to.wstr_ptr[string_to_ptr->wstr_len] = semicolon;
                temp_string_to.wstr_len += 1;
            }

            SCI_MEMSET(editdoc_ptr->edit_to.wstr_ptr ,0,((string_to_ptr->wstr_len + 1) * sizeof(wchar)));
            editdoc_ptr->edit_to.wstr_len = 0;

            for (i = 0; i < temp_string_to.wstr_len && (0x00 != temp_string_to.wstr_ptr[i]); i++)
            {
                if (semicolon == temp_string_to.wstr_ptr[i])
                {
                    uint32 temp_len = 0;
                    wchar *temp_str = PNULL;
                    wchar *temp_str1 = PNULL;

                    temp_len = i - temp_pos + 1;//need add semicolon OR End character

                    if (1 == temp_len)
                    {
                        temp_pos = i + 1;

                        continue;
                    }

                    temp_str = SCI_ALLOCAZ((temp_len + 1) * sizeof(wchar));

                    if (PNULL == temp_str)
                    {
                        MMIMMS_FREE(temp_string_to.wstr_ptr);
                        return;
                    }

                    MMI_WSTRNCPY(temp_str, temp_len, temp_string_to.wstr_ptr + temp_pos, temp_len - 1, temp_len - 1);//no semicolon and End character

                    temp_str1 = MMIAPICOM_Wstrstr(editdoc_ptr->edit_to.wstr_ptr, temp_str);

                    if ((PNULL != temp_str1) && (semicolon == temp_str1[temp_len - 1] || (0x00 == temp_str1[temp_len - 1])))
                    {
                        //the same record
                    }
                    else
                    {
                        MMI_WSTRNCPY(editdoc_ptr->edit_to.wstr_ptr + editdoc_ptr->edit_to.wstr_len, temp_len,
                            temp_string_to.wstr_ptr + temp_pos, temp_len, temp_len);

                        editdoc_ptr->edit_to.wstr_len += temp_len;
                    }

                    temp_pos = i + 1;

                    SCI_FREE(temp_str);
                }
            }
            MMIMMS_FREE( temp_string_to.wstr_ptr);
        }
    }
    else if(PNULL != editdoc_ptr->edit_to.wstr_ptr)
    {
        SCI_MEMSET(editdoc_ptr->edit_to.wstr_ptr, 0, (editdoc_ptr->edit_to.wstr_len * sizeof(wchar)));
        editdoc_ptr->edit_to.wstr_len = 0;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SetMMSTo string_to NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_547_112_2_18_2_37_33_5,(uint8*)"");
    }

}
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef MMIMMS_SMS_CC_SUPPORT    
/*****************************************************************************/
//  Description : 设置抄送 人 
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSCC(
                            const MMI_STRING_T *string_cc_ptr, 
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                            )
{
    if(PNULL == editdoc_ptr || PNULL == string_cc_ptr)
    {
        return;
    }
    if(string_cc_ptr->wstr_len > 0)
    {
        if(PNULL != editdoc_ptr->edit_cc.wstr_ptr)
        {
            SCI_FREE(editdoc_ptr->edit_cc.wstr_ptr);
            editdoc_ptr->edit_cc.wstr_ptr = PNULL;
        }
        editdoc_ptr->edit_cc.wstr_ptr = MMIMMS_ALLOC((string_cc_ptr->wstr_len + 1) * sizeof(wchar));
        if(PNULL != editdoc_ptr->edit_cc.wstr_ptr)
        {
            SCI_MEMSET(editdoc_ptr->edit_cc.wstr_ptr ,0,(string_cc_ptr->wstr_len * sizeof(wchar)));
            MMI_WSTRNCPY(editdoc_ptr->edit_cc.wstr_ptr,string_cc_ptr->wstr_len,
                string_cc_ptr->wstr_ptr, string_cc_ptr->wstr_len, string_cc_ptr->wstr_len);
            editdoc_ptr->edit_cc.wstr_len = string_cc_ptr->wstr_len;
        }
    }
    else if(PNULL != editdoc_ptr->edit_cc.wstr_ptr)
    {
        SCI_MEMSET(editdoc_ptr->edit_cc.wstr_ptr, 0, (editdoc_ptr->edit_cc.wstr_len * sizeof(wchar)));
        editdoc_ptr->edit_cc.wstr_len = 0;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SetMMSCC string_to NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_588_112_2_18_2_37_33_6,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : 设置密送人
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSBCC(
                            const MMI_STRING_T *string_bcc_ptr, 
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                            )
{
    if(PNULL == editdoc_ptr || PNULL == string_bcc_ptr)
    {
        return;
    }
    if(string_bcc_ptr->wstr_len > 0)
    {
        if(editdoc_ptr->edit_bcc.wstr_ptr)
        {
            SCI_FREE(editdoc_ptr->edit_bcc.wstr_ptr);
            editdoc_ptr->edit_bcc.wstr_ptr = PNULL;
        }
        editdoc_ptr->edit_bcc.wstr_ptr = MMIMMS_ALLOC((string_bcc_ptr->wstr_len + 1) * sizeof(wchar));
        if(PNULL != editdoc_ptr->edit_bcc.wstr_ptr)
        {
            SCI_MEMSET(editdoc_ptr->edit_bcc.wstr_ptr ,0,(string_bcc_ptr->wstr_len * sizeof(wchar)));
            MMI_WSTRNCPY(editdoc_ptr->edit_bcc.wstr_ptr,string_bcc_ptr->wstr_len,
                string_bcc_ptr->wstr_ptr, string_bcc_ptr->wstr_len, string_bcc_ptr->wstr_len);
            editdoc_ptr->edit_bcc.wstr_len = string_bcc_ptr->wstr_len;
        }
    }
    else if(PNULL != editdoc_ptr->edit_bcc.wstr_ptr)
    {
        SCI_MEMSET(editdoc_ptr->edit_bcc.wstr_ptr, 0, (editdoc_ptr->edit_bcc.wstr_len * sizeof(wchar)));
        editdoc_ptr->edit_bcc.wstr_len = 0;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SetMMSBCC string_to NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_628_112_2_18_2_37_33_7,(uint8*)"");
    }
}
#endif
#endif

/*****************************************************************************/
//  Description : 设置收件人名称列表
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSToNameList(
                            MMIMMS_EDIT_NUMBER_LIST_T *numberlist_ptr, 
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                            )
{
    MMIMMS_EDIT_NUMBER_LIST_T *temp_numberlist_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *cursor_numberlist_ptr = PNULL;

    if(PNULL == numberlist_ptr || PNULL == editdoc_ptr)
    {
        return;
    }
    cursor_numberlist_ptr = editdoc_ptr->numberlist_header_ptr;
    while(PNULL != cursor_numberlist_ptr)
    {
        if(PNULL != cursor_numberlist_ptr->number_name_ptr)
        {
            SCI_FREE(cursor_numberlist_ptr->number_name_ptr);
            cursor_numberlist_ptr->number_name_ptr = PNULL;
        }
        if(PNULL != cursor_numberlist_ptr->number_ptr)
        {
            SCI_FREE(cursor_numberlist_ptr->number_ptr);
            cursor_numberlist_ptr->number_ptr = PNULL;
        }
        temp_numberlist_ptr = cursor_numberlist_ptr;
        cursor_numberlist_ptr = cursor_numberlist_ptr->next;
        SCI_FREE(temp_numberlist_ptr);
        temp_numberlist_ptr = PNULL;
    }
    editdoc_ptr->numberlist_header_ptr = numberlist_ptr;
}

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description : 设置收件人名称列表,包含CC,BCC
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSToNameListEx(
                            MMIMMS_EDIT_NUMBER_LIST_T *numberlist_ptr, 
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr,
                            MMIMMS_EDIT_ADD_CONTACT_TYPE contact_type
                            )
{
    MMIMMS_EDIT_NUMBER_LIST_T *temp_numberlist_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *cursor_numberlist_ptr = PNULL;

    if(PNULL == numberlist_ptr || PNULL == editdoc_ptr)
    {
        return;
    }
    if(MMIMMS_ADD_TO_CONTACT == contact_type)
    {
        cursor_numberlist_ptr = editdoc_ptr->numberlist_header_ptr;
    }
#ifdef MMIMMS_SMS_CC_SUPPORT    
    else if(MMIMMS_ADD_CC_CONTACT == contact_type)
    {
        cursor_numberlist_ptr = editdoc_ptr->numberlist_cc_header_ptr;
    }
    else if(MMIMMS_ADD_BCC_CONTACT == contact_type)
    {
        cursor_numberlist_ptr = editdoc_ptr->numberlist_bcc_header_ptr;
    }
#endif

    while(PNULL != cursor_numberlist_ptr)
    {
        if(PNULL != cursor_numberlist_ptr->number_name_ptr)
        {
            SCI_FREE(cursor_numberlist_ptr->number_name_ptr);
        }
        if(PNULL != cursor_numberlist_ptr->number_ptr)
        {
            SCI_FREE(cursor_numberlist_ptr->number_ptr);
        }
        temp_numberlist_ptr = cursor_numberlist_ptr;
        cursor_numberlist_ptr = cursor_numberlist_ptr->next;
        SCI_FREE(temp_numberlist_ptr);
    }

    if(MMIMMS_ADD_TO_CONTACT == contact_type)
    {
        editdoc_ptr->numberlist_header_ptr = numberlist_ptr;
    }
#ifdef MMIMMS_SMS_CC_SUPPORT    
    else if(MMIMMS_ADD_CC_CONTACT == contact_type)
    {
        editdoc_ptr->numberlist_cc_header_ptr = numberlist_ptr;
    }
    else if(MMIMMS_ADD_BCC_CONTACT == contact_type)
    {
        editdoc_ptr->numberlist_bcc_header_ptr = numberlist_ptr;
    }
#endif
    
}
#endif

/*****************************************************************************/
//  Description : 设置当前页面的播放时间,单位MS
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetCurSlideDuration(
                                       uint32 delay,
                                       MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                       )
{
    if(PNULL == editdoc_ptr)
    {
        return;
    }
    editdoc_ptr->editbody_ptr->slide_cur->times = delay;
}

/*****************************************************************************/
//  Description : add a new slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_AddSlide(
                                      MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr, 
                                      uint32  slide_direct
                                      )
{
    MMIMMS_ERROR_E result_val = MMIMMS_NO_ERROR;
    uint16 slide_num = 0;

    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr)
    {
        return MMIMMS_ERROR_FAIL;
    }

    slide_num = MMS_GetSlideCount(editdoc_ptr->editbody_ptr);
    if(slide_num >= MMIMMS_MAX_SLIDE)
    {
        result_val = MMIMMS_ERROR_MAX_SLIDE;
    }
    else if((uint32)(editdoc_ptr->editbody_ptr->totalsize) + MMIMMS_SINGLE_SLIDE_SIZE > MMIMMS_MMS_LIMIT_SIZE)
    {
        result_val = MMIMMS_ERROR_INVALID_SIZE;
    }
    else 
    {
        P_MMS_SLIDE_T slide_ret;
        if(slide_direct)
        {
             slide_ret = MMS_CreateNewSlide(editdoc_ptr->editbody_ptr); 
        }
        else
        {
             slide_ret = MMS_CreateNewSlideR(editdoc_ptr->editbody_ptr);
        }
        if(slide_ret)
        {
            MMS_SetSlideTextImageOrder(editdoc_ptr->editbody_ptr,editdoc_ptr->textimage_order);
            
        }
        else
        {
            result_val = MMIMMS_ERROR_NO_MEMORY;
        }
    }
    return result_val;
}

/*****************************************************************************/
//  Description : add text to slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_AddText(
                                     MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr,
                                     const MMI_STRING_T* str_info_ptr
                                     )
{
    MMIMMS_ERROR_E  return_val = MMIMMS_NO_ERROR;
    uint32          char_length = 0;
    uint8           text_fname[MMIMMS_FILE_NAME_LEN] = {0};
    FILE_HANDLE     file_handle = 0;

    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr)
    {
        return MMIMMS_ERROR_FAIL;
    }

    if (editdoc_ptr->editbody_ptr->slide_cur->text_flag)    //判断当前帧是否已有文本
    {
        DelSlideContent(editdoc_ptr, MMS_TYPE_TEXT);
        editdoc_ptr->editbody_ptr->slide_cur->text_flag = 0;
    }
    if (0 != str_info_ptr->wstr_len && PNULL != str_info_ptr->wstr_ptr)
    {
        MMIMMS_GenRandomName(text_fname,sizeof(text_fname));
        strcat((char *)text_fname,".txt");
        file_handle =sysVFopen((char*)text_fname,"w");
        if (file_handle)
        {
            wchar *mms_add_wstr_ptr = (wchar *)SCI_ALLOC_APP((str_info_ptr->wstr_len + 1) * sizeof(wchar));
            SCI_MEMSET(mms_add_wstr_ptr, 0, (str_info_ptr->wstr_len + 1) * sizeof(wchar));
            MMIAPICOM_Wstrncpy(mms_add_wstr_ptr, str_info_ptr->wstr_ptr, str_info_ptr->wstr_len);

            char_length = str_info_ptr->wstr_len * 3;   //MMS lib中进行了除2操作
            sysVFwrite((int8 *)(&char_length),1,sizeof(uint32),file_handle);
            sysVFwrite((int8 *)mms_add_wstr_ptr,str_info_ptr->wstr_len + 1,sizeof(wchar),file_handle);
            sysVFclose(file_handle);
            SCI_Free(mms_add_wstr_ptr);

            if (!MMS_AddConetnt((char *)text_fname,MMS_TYPE_TEXT,editdoc_ptr->editbody_ptr->slide_cur,editdoc_ptr->editbody_ptr))
            {
                return_val = MMIMMS_ERROR_NO_MEMORY;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"open the file error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_851_112_2_18_2_37_33_8,(uint8*)"");
            return_val = MMIMMS_ERROR_NO_MEMORY;
        }
    }
    return return_val;
}

/*****************************************************************************/
//  Description : del slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DelSlide(
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                            )
{
    uint8* vfname_ptr = PNULL;
    uint16 vfname_len = 0;

    if(PNULL == editdoc_ptr)
    {
        return ;
    }
    if(editdoc_ptr->editbody_ptr->slide_cur->audiosize > 0)
    {
        vfname_ptr = (uint8 *)editdoc_ptr->editbody_ptr->slide_cur->audiofile;
        vfname_len = SCI_STRLEN((char *)vfname_ptr);
        RemoveFileNameFromNameList(editdoc_ptr,vfname_ptr,vfname_len);
    }
    MMS_DelSlideOfEdoc(editdoc_ptr->editbody_ptr); 
}

/*****************************************************************************/
//  Description : del image in current slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DelImage(
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                            )
{
    uint16          type = 0;
    P_MMS_CONTENT_T    content = NULL;
    P_MMS_SLIDE_T      slide = PNULL; 

    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr)
    {
        return;
    }

    slide = editdoc_ptr->editbody_ptr->slide_cur; 
    if(slide->image_flag)
    {
        content = slide->content_root;
        while (content)
        {
            if (IsAvailableType((uint16)content->type, MMIFMM_FILE_TYPE_PICTURE))
            {
                type = (uint16)content->type;
                break;
            }
            content = content->next;
        }
        slide->content_cur = content;
		MMS_DelContOfSlide(slide,editdoc_ptr->editbody_ptr);
        slide->image_flag = 0;
    }
}

/*****************************************************************************/
//  Description : del music in slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DelMusic(
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                            )
{
    uint8 *vfname_ptr = PNULL;
    uint16 vfname_len = 0;
    uint32 delay = MMIMMS_SLIDE_PLAY_DEFAULT_TIME;

    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr)
    {
        return;
    }

    if(editdoc_ptr->editbody_ptr->slide_cur->audiosize > 0)
    {
        vfname_ptr = (uint8 *)editdoc_ptr->editbody_ptr->slide_cur->audiofile;
        vfname_len = SCI_STRLEN((char *)vfname_ptr);
        RemoveFileNameFromNameList(editdoc_ptr,vfname_ptr,vfname_len);
        MMIMMS_SetCurSlideDuration(delay,editdoc_ptr);
    }
    MMS_DeleteAudio(editdoc_ptr->editbody_ptr->slide_cur, editdoc_ptr->editbody_ptr);
}

/*****************************************************************************/
//  Description : del video in slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DelVideo(
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                            )
{
    uint16          type = 0;
    P_MMS_CONTENT_T    content = NULL;
    P_MMS_SLIDE_T      slide = PNULL;
    uint32 delay = MMIMMS_SLIDE_PLAY_DEFAULT_TIME;
    
    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr)
    {
        return;
    }

    slide = editdoc_ptr->editbody_ptr->slide_cur;
    if(slide->video_flag)
    {
        content = slide->content_root;
        while (content)
        {
            if (IsAvailableType((uint16)content->type, MMIFMM_FILE_TYPE_MOVIE))
            {
                type = (uint16)content->type;
                break;
            }
            content = content->next;
        }
        slide->content_cur = content;
		MMS_DelContOfSlide(slide,editdoc_ptr->editbody_ptr);
        slide->video_flag = 0;
        MMIMMS_SetCurSlideDuration(delay,editdoc_ptr);
    }
}

/*****************************************************************************/
//  Description : del text in slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DelText(
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                            )
{
    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr ||PNULL == editdoc_ptr->editbody_ptr->slide_cur)
    {
        return;
    }
    if (editdoc_ptr->editbody_ptr->slide_cur->text_flag)    //判断当前帧是否已有文本
    {
        DelSlideContent(editdoc_ptr, MMS_TYPE_TEXT);
        editdoc_ptr->editbody_ptr->slide_cur->text_flag = 0;
    }
}

/*****************************************************************************/
//  Description : del attachment
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_DelAttachment(
                                 uint8 * attachment_name_ptr,
                                 MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                 )
{
    if (PNULL == attachment_name_ptr || PNULL == editdoc_ptr)
    {
        return FALSE;
    }
    if (MMS_DeleteAttachment((char *)attachment_name_ptr, editdoc_ptr->editbody_ptr))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : go to head slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_HeadSlide(
                                       MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                       )
{
    MMIMMS_ERROR_E return_val = MMIMMS_NO_ERROR;

    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr || PNULL == editdoc_ptr->editbody_ptr->slide_root)
    {
        return_val = MMIMMS_ERROR_FAIL;
    }
    else if(editdoc_ptr->editbody_ptr->slide_cur == editdoc_ptr->editbody_ptr->slide_root)
    {
        return_val = MMIMMS_NO_OPERATION;
    }
	else
	{
		editdoc_ptr->editbody_ptr->slide_cur=editdoc_ptr->editbody_ptr->slide_root;
	}

    return return_val;
}

/*****************************************************************************/
//  Description : go to tail slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_PrevSlide(
                                       MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                       )
{
    MMIMMS_ERROR_E return_val = MMIMMS_NO_ERROR;

    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr || PNULL == editdoc_ptr->editbody_ptr->slide_cur)
	{
		return_val = MMIMMS_ERROR_FAIL;
	}
    else if(PNULL == editdoc_ptr->editbody_ptr->slide_cur->prev)
    {
        return_val = MMIMMS_NO_OPERATION;
    }
    else
	{
		editdoc_ptr->editbody_ptr->slide_cur=editdoc_ptr->editbody_ptr->slide_cur->prev;
	}

    return return_val;
}

/*****************************************************************************/
//  Description : del image in slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_NextSlide(
                                       MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                       )
{
    MMIMMS_ERROR_E return_val = MMIMMS_NO_ERROR;

    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr || PNULL == editdoc_ptr->editbody_ptr->slide_cur)
	{
		return_val = MMIMMS_ERROR_FAIL;
	}
    else if(PNULL == editdoc_ptr->editbody_ptr->slide_cur->next)
    {
        return_val = MMIMMS_NO_OPERATION;
    }
    else
	{
		editdoc_ptr->editbody_ptr->slide_cur=editdoc_ptr->editbody_ptr->slide_cur->next;
	}

    return return_val;
}

/*****************************************************************************/
//  Description : del image in slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_TailSlide(
                                       MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                       )
{
    MMIMMS_ERROR_E return_val = MMIMMS_NO_ERROR;
    P_MMS_SLIDE_T pTempSlide = PNULL;

    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr || PNULL == editdoc_ptr->editbody_ptr->slide_cur)
	{
		return_val = MMIMMS_ERROR_FAIL;
	}
    else if(PNULL == editdoc_ptr->editbody_ptr->slide_cur->next)
    {
        return_val = MMIMMS_NO_OPERATION;
    }
	else
	{
        pTempSlide = editdoc_ptr->editbody_ptr->slide_cur;
        while(pTempSlide->next)
        {
            pTempSlide = pTempSlide->next;
        }
        editdoc_ptr->editbody_ptr->slide_cur = pTempSlide;
	}

    return return_val;
}

/*****************************************************************************/
//  Description : get current slide index
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIMMS_GetSlideIndex(
                                   MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                   )
{
    if(PNULL == editdoc_ptr)
    {
        return 0;
    }
    return MMS_GetSlideIndex(editdoc_ptr->editbody_ptr);
}

/*****************************************************************************/
//  Description : get slide count
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIMMS_GetSlideCount(
                                   MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                   )
{
    if(PNULL == editdoc_ptr)
    {
        return 0;
    }
    return MMS_GetSlideCount(editdoc_ptr->editbody_ptr);
}

/*****************************************************************************/
//  Description : get image,music,video count
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIMMS_GetAllContentCount(
                                        MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                        )
{
    if(PNULL == editdoc_ptr)
    {
        return 0;
    }
    return MMS_GetAllContentCount(editdoc_ptr->editbody_ptr);
}

/*****************************************************************************/
//  Description : get image,music,video in slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetContentByName(
                                       uint8	*filename, 
                                       uint8 	**data,
                                       uint32	*data_len
                                       )
{
    if(sysVFGetConstFileData((int8 *)filename,(int8 **)data,(int32 *)data_len))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetCurrentSlideAudio(MMISRVAUD_RING_FMT_E          *type, 
                                           uint32                   *multim_buf_len,
                                           void                     **multim_buf_ptr,
                                           MMIMMS_EDIT_DOCUMENT_T   *editdoc_ptr
                                           )
{
    BOOLEAN         return_val = FALSE;
    P_MMS_EDIT_BODY_T editbody_ptr = PNULL;
    
    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr)
    {
        return FALSE;
    }
    editbody_ptr = editdoc_ptr->editbody_ptr;
    
    if(editbody_ptr->slide_cur->audio_type)
    {
        *multim_buf_ptr = PNULL;
        sysVFGetConstFileData(editbody_ptr->slide_cur->audiofile,(int8 **)multim_buf_ptr,(int32 *)multim_buf_len);
        if(*multim_buf_ptr && PNULL != type)
        {
            *type = MMSTypeConvertToAudioType(editbody_ptr->slide_cur->audio_type);
            return_val = TRUE;
        }

    }
    
    return return_val;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetCurrentSlideImage(uint32                   *type, 
                                           uint32                   *multim_buf_len,
                                           void                     **multim_buf_ptr,
                                           MMIMMS_EDIT_DOCUMENT_T   *editdoc_ptr
                                           )
{
    BOOLEAN         return_val = FALSE;
    P_MMS_CONTENT_T     content = NULL;
    P_MMS_EDIT_BODY_T editbody_ptr = PNULL;
    
    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr)
    {
        return FALSE;
    }
    editbody_ptr = editdoc_ptr->editbody_ptr;
    
    if(editbody_ptr->slide_cur->image_flag)
    {
        content = editbody_ptr->slide_cur->content_root;
        for(;content;content = content->next)
        {
            if(IsAvailableType((uint16)content->type, MMIFMM_FILE_TYPE_PICTURE))
            {
                break;
            }
        }
        if(content)
        {
            *multim_buf_ptr = PNULL;
            sysVFGetConstFileData(content->filename,(int8 **)multim_buf_ptr,(int32 *)multim_buf_len);
            if(*multim_buf_ptr)
            {
                *type = content->type;
                return_val = TRUE;
            }
        }

    }
    
    return return_val;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetCurrentSlideVideo(uint32                   *type, 
                                           uint32                   *multim_buf_len,
                                           void                     **multim_buf_ptr,
                                           MMIMMS_EDIT_DOCUMENT_T   *editdoc_ptr
                                           )
{
    BOOLEAN         return_val = FALSE;
//  uint32             len = 0,size=0;
    P_MMS_CONTENT_T     content = NULL;
    P_MMS_EDIT_BODY_T editbody_ptr = PNULL;

    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr)
    {
        return FALSE;
    }
    editbody_ptr = editdoc_ptr->editbody_ptr;
    
    if(editbody_ptr->slide_cur->video_flag)
    {
        content = editbody_ptr->slide_cur->content_root;
        for(;content;content = content->next)
        {
            if(IsAvailableType((uint16)content->type, MMIFMM_FILE_TYPE_MOVIE))
            {
                break;
            }
        }
        if(content)
        {
            *multim_buf_ptr = PNULL;
            sysVFGetConstFileData(content->filename,(int8 **)multim_buf_ptr,(int32 *)multim_buf_len);
            if(*multim_buf_ptr)
            {
                *type = content->type;
                return_val = TRUE;
            }
        }

    }
 
    return return_val;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetCurrentSlideAudioInfo(uint8 *fname,
                                              uint32 fname_max_len,
                                              MMIMMS_EDIT_DOCUMENT_T   *editdoc_ptr)
{
    P_MMS_EDIT_BODY_T editbody_ptr = PNULL;
    uint32          file_name_len=0;
    uint32          file_size =0;
    
    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr)
    {
        return 0;
    }
    editbody_ptr = editdoc_ptr->editbody_ptr;
    
    if(editbody_ptr->slide_cur->audio_type)
    {
        file_name_len = SCI_STRLEN(editbody_ptr->slide_cur->audiofile);
        file_name_len=MIN(fname_max_len,file_name_len);
        SCI_MEMCPY(fname,editbody_ptr->slide_cur->audiofile,file_name_len);
        file_size=editbody_ptr->slide_cur->audiosize;
    }
    
    return file_size;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetCurrentSlideImageInfo(uint8 *fname,
                                              uint32 fname_max_len,
                                              MMIMMS_EDIT_DOCUMENT_T   *editdoc_ptr)
{
    P_MMS_CONTENT_T     content = NULL;
    P_MMS_EDIT_BODY_T editbody_ptr = PNULL;
    uint32          file_size=0;
    uint32          file_name_len=0;
    
    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr)
    {
        return 0;
    }
    editbody_ptr = editdoc_ptr->editbody_ptr;
    
    if(editbody_ptr->slide_cur->image_flag)
    {
        content = editbody_ptr->slide_cur->content_root;
        for(;content;content = content->next)
        {
            if(IsAvailableType((uint16)content->type, MMIFMM_FILE_TYPE_PICTURE))
            {
                break;
            }
        }
        if(content)
        {
            file_name_len = SCI_STRLEN(content->filename);
            file_name_len=MIN(fname_max_len,file_name_len);
            SCI_MEMCPY(fname,content->filename,file_name_len);
            file_size=content->contentsize;
        }

    }
    
    return file_size;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetCurrentSlideVideoInfo(uint8 *fname,
                                              uint32 fname_max_len,
                                              MMIMMS_EDIT_DOCUMENT_T   *editdoc_ptr)
{
    P_MMS_CONTENT_T     content = NULL;
    P_MMS_EDIT_BODY_T editbody_ptr = PNULL;
    uint32          file_size=0;
    uint32          file_name_len=0;
    
    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr)
    {
        return 0;
    }
    editbody_ptr = editdoc_ptr->editbody_ptr;
    
    if(editbody_ptr->slide_cur->video_flag)
    {
        content = editbody_ptr->slide_cur->content_root;
        for(;content;content = content->next)
        {
            if(IsAvailableType((uint16)content->type, MMIFMM_FILE_TYPE_MOVIE))
            {
                break;
            }
        }
        if(content)
        {
            file_name_len = SCI_STRLEN(content->filename);
            file_name_len=MIN(fname_max_len,file_name_len);
            SCI_MEMCPY(fname,content->filename,file_name_len);
            file_size=content->contentsize;
        }
    }
 
    return file_size;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetCurrentSlideText(
                                           MMIMMS_EDIT_DOCUMENT_T   *editdoc_ptr,
                                           MMI_STRING_T             *string_ptr
                                           )
{
    BOOLEAN         return_val = FALSE;
    uint32          str_len = 0;
    uint8*          str_ptr = PNULL;
    P_MMS_CONTENT_T    content = PNULL;
    P_MMS_EDIT_BODY_T editbody_ptr = PNULL;

    if(PNULL == editdoc_ptr || PNULL == editdoc_ptr->editbody_ptr || PNULL == string_ptr)
    {
        return FALSE;
    }
    
    editbody_ptr = editdoc_ptr->editbody_ptr;
    if(editbody_ptr->slide_cur->text_flag)
    {
        content = editbody_ptr->slide_cur->content_root;
        for(;content;content = content->next)
        {
            if(content->type == MMS_TYPE_TEXT)
            {
                break;
            }
        }
        if(content)
        {
            if(sysVFGetConstFileData(content->filename,(int8 **)&str_ptr,(int32 *)&str_len))
            {
                string_ptr->wstr_ptr = (wchar*)(str_ptr+sizeof(uint32));
                //string_ptr->wstr_len = (uint16)(*(uint32 *)str_ptr)/sizeof(wchar);
                string_ptr->wstr_len = MMIAPICOM_Wstrlen(string_ptr->wstr_ptr);
                return_val = TRUE;
            }
            
        }
    }

    return return_val;
}

/*****************************************************************************/
//  Description : 根据用户设置，获取MMS head的值
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetDefaultHeadValue(
                                       MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                       )
{
    if(PNULL == editdoc_ptr)
    {
        return;
    }
    editdoc_ptr->message_class = MMS_MESSAGE_CLASS_TYPE_PERSONAL;

    if (MMIMMS_Setting_GetAnonymous())//匿名发送
    {
        editdoc_ptr->sender_visibility = MMS_SENDER_VISIBILITY_HIDE;
    }
    else
    {
        editdoc_ptr->sender_visibility = MMS_SENDER_VISIBILITY_SHOW;
    }

    switch(MMIMMS_Setting_GetPriority())     //优先级
    {
    case MMIMMS_SETTING_PRIORITY_LOW:
        editdoc_ptr->pritority = MMS_PRIORITY_VALUE_LOW;
    	break;
    case MMIMMS_SETTING_PRIORITY_NORMAL:
        editdoc_ptr->pritority = MMS_PRIORITY_VALUE_NORMAL;
    	break;
    case MMIMMS_SETTING_PRIORITY_HIGH:
        editdoc_ptr->pritority = MMS_PRIORITY_VALUE_HIGH;
    	break;
    default:
        editdoc_ptr->pritority = MMS_PRIORITY_VALUE_NORMAL;
        break;
    }

    if (MMIMMS_Setting_GetDeliveryReport())     //送达报告
    {
        editdoc_ptr->delivery_report = MMS_DELIVERY_REPORT_YES;
	}
	else
	{
        editdoc_ptr->delivery_report = MMS_DELIVERY_REPORT_NO;
	}


    if (MMIMMS_Setting_GetReadReport())     //阅读报告
    {
        editdoc_ptr->read_report = MMS_READ_REPORT_YES;
	}
	else
	{
        editdoc_ptr->read_report = MMS_READ_REPORT_NO;
	}
    
    switch(MMIMMS_Setting_GetValidPeriod())     //有效期
    {
    case MMIMMS_SETTING_VALID_PERIOD_1_HOUR:
        editdoc_ptr->expiry = MMIMMS_1_HOUR_SECONDS;
    	break;
    case MMIMMS_SETTING_VALID_PERIOD_12_HOUR:
        editdoc_ptr->expiry = MMIMMS_12_HOUR_SECONDS;
    	break;
    case MMIMMS_SETTING_VALID_PERIOD_1_DAY:
        editdoc_ptr->expiry = MMIMMS_1_DAY_SECONDS;
    	break;
    case MMIMMS_SETTING_VALID_PERIOD_1_WEEK:
        editdoc_ptr->expiry = MMIMMS_1_WEEK_SECONDS;
    	break;
    case MMIMMS_SETTING_VALID_PERIOD_LONGEST:
        editdoc_ptr->expiry = 0;
    	break;
    default:
        break;
    }
}


/*****************************************************************************/
//  Description : edit mms read report  可以放到UI层
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_EDIT_DOCUMENT_T * EditMMSReadReport(
                                                  MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                                  )
{
    MMIMMS_EDIT_DOCUMENT_T * reportdoc_ptr = PNULL;
    MMI_STRING_T            report_string = {0};
    uint32                 ldate = 0;
    wchar                  read_time[MMIMMS_EDITHEAD_BUFFER_LEN] = {0};

    MMI_TM_T             tm = {0};
    MMI_STRING_T           to_str = {0};

    MMI_STRING_T           message_id_str = {0};

    MMI_STRING_T           read_time_str = {0};
    char str[MMIMMS_EDITHEAD_BUFFER_LEN] = {0};
    MMISMS_ORDER_INFO_T * item_ptr = PNULL;
    const wchar     r_wstr[] = {'\r', 0 };
    uint16          r_wstr_len = MMIAPICOM_Wstrlen(r_wstr);
    uint16          str_len = 0;

    if(PNULL == editdoc_ptr)
    {
        return PNULL;
    }
    item_ptr = MMIMMS_GetListItemByID(editdoc_ptr->record_id);
    if(PNULL != item_ptr)
    {
    	str_len = SCI_STRLEN((char *)item_ptr->sender);
    }
    reportdoc_ptr = MMIMMS_CreateEditDocument();
    if(PNULL == item_ptr || 0 == str_len || PNULL == reportdoc_ptr)
    {
        SCI_TRACE_LOW("[MMIMMS]:EditMMSReadReport item_ptr=%d, str_len=%d, reportdoc_ptr=%d", item_ptr, str_len, reportdoc_ptr);
        MMIMMS_DestroyEditDocument(reportdoc_ptr);
        return PNULL;
    }
    MMS_CreateNewSlide(reportdoc_ptr->editbody_ptr);

    reportdoc_ptr->edit_to.wstr_len = str_len;
    reportdoc_ptr->edit_to.wstr_ptr = MMIMMS_ALLOC(str_len * sizeof(wchar));
    if (PNULL == reportdoc_ptr->edit_to.wstr_ptr)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:EditMMSReadReport edit_to mms_no_memory"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_1611_112_2_18_2_37_35_9,(uint8*)"");
        MMIMMS_DestroyEditDocument(reportdoc_ptr);
        return PNULL;
    }
    SCI_MEMSET(reportdoc_ptr->edit_to.wstr_ptr,0,(str_len * sizeof(wchar)));
    MMI_STRNTOWSTR(reportdoc_ptr->edit_to.wstr_ptr,str_len,(uint8 *)item_ptr->sender,str_len,str_len);
    ldate = MMIAPICOM_GetCurTime();
    tm = MMIAPICOM_Second2Tm(ldate);
    _snprintf((char*)str, (MMIMMS_EDITHEAD_BUFFER_LEN-1),
        "%04d%s%02d%s%02d%s%02d:%02d:%02d",
        tm.tm_year, "-", tm.tm_mon, "-", tm.tm_mday, "  ", tm.tm_hour, tm.tm_min, tm.tm_sec );

    MMIAPICOM_StrToWstr((uint8*)str,read_time);

    MMI_GetLabelTextByLang(TXT_COMMON_TO,&to_str);

    MMI_GetLabelTextByLang(TXT_MMS_DETAIL_MESSAGEID,&message_id_str);

    MMI_GetLabelTextByLang(TXT_MMS_READREPORT_TIME,&read_time_str);

    report_string.wstr_len += to_str.wstr_len + r_wstr_len + reportdoc_ptr->edit_to.wstr_len + r_wstr_len;

    report_string.wstr_len += message_id_str.wstr_len + r_wstr_len + editdoc_ptr->msg_id.wstr_len + r_wstr_len;

    report_string.wstr_len += read_time_str.wstr_len + r_wstr_len;
    report_string.wstr_len += MMIAPICOM_Wstrlen(read_time);

    report_string.wstr_ptr = (wchar*)MMIMMS_ALLOC((report_string.wstr_len+1) * sizeof(wchar));
    if (PNULL == report_string.wstr_ptr)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:EditMMSReadReport warning malloc report_string.wstr_len=%d failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_1641_112_2_18_2_37_35_10,(uint8*)"d",report_string.wstr_len);
        MMIMMS_DestroyEditDocument(reportdoc_ptr);
        return PNULL;
    }
    SCI_MEMSET(report_string.wstr_ptr, 0, ((report_string.wstr_len+1) * sizeof(wchar)));

    MMIAPICOM_Wstrncpy(report_string.wstr_ptr,to_str.wstr_ptr,to_str.wstr_len);
    MMIAPICOM_Wstrncat(report_string.wstr_ptr,r_wstr,r_wstr_len);
    MMIAPICOM_Wstrncat(report_string.wstr_ptr,reportdoc_ptr->edit_to.wstr_ptr,reportdoc_ptr->edit_to.wstr_len);
    MMIAPICOM_Wstrncat(report_string.wstr_ptr,r_wstr,r_wstr_len);
    MMIAPICOM_Wstrncat(report_string.wstr_ptr,message_id_str.wstr_ptr,message_id_str.wstr_len);
    MMIAPICOM_Wstrncat(report_string.wstr_ptr,r_wstr,r_wstr_len);
    MMIAPICOM_Wstrncat(report_string.wstr_ptr,editdoc_ptr->msg_id.wstr_ptr,editdoc_ptr->msg_id.wstr_len);
    MMIAPICOM_Wstrncat(report_string.wstr_ptr,r_wstr,r_wstr_len);
    MMIAPICOM_Wstrncat(report_string.wstr_ptr,read_time_str.wstr_ptr,read_time_str.wstr_len);
    MMIAPICOM_Wstrncat(report_string.wstr_ptr,r_wstr,r_wstr_len);
    MMIAPICOM_Wstrncat(report_string.wstr_ptr,read_time,MMIAPICOM_Wstrlen(read_time));
  
    MMIMMS_AddText(reportdoc_ptr,&report_string);
    MMIMMS_FREE(report_string.wstr_ptr);
    return reportdoc_ptr;
}

/*****************************************************************************/
//  Description : create mms edit doc
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_EDIT_DOCUMENT_T *MMIMMS_CreateEditDocument(void)
{
    MMIMMS_EDIT_DOCUMENT_T *editdoc_ptr = PNULL;
    
    editdoc_ptr = MMIMMS_ALLOC(sizeof(MMIMMS_EDIT_DOCUMENT_T));
    if(editdoc_ptr)
    {
        SCI_MEMSET(editdoc_ptr,0,sizeof(MMIMMS_EDIT_DOCUMENT_T));
        editdoc_ptr->editbody_ptr = MMS_CreateNewEditDoc();
        if(!editdoc_ptr->editbody_ptr)
        {
            MMIMMS_FREE(editdoc_ptr);
            editdoc_ptr = PNULL;
        }
    }
    
    return editdoc_ptr;
}

/*****************************************************************************/
//  Description : destroy mms edit doc
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DestroyEditDocument(MMIMMS_EDIT_DOCUMENT_T *editdoc_ptr)
{
    MMIMMS_EDIT_NAME_LIST_T* cursor_ptr = PNULL;
    MMIMMS_EDIT_NAME_LIST_T* free_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *cursor_numberlist_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *free_numberlist_ptr = PNULL;

    if(editdoc_ptr)
    {
        if(editdoc_ptr->edit_to.wstr_ptr)
        {
            MMIMMS_FREE(editdoc_ptr->edit_to.wstr_ptr);
        }
        if(editdoc_ptr->edit_cc.wstr_ptr)
        {
            MMIMMS_FREE(editdoc_ptr->edit_cc.wstr_ptr);
        }
        if(editdoc_ptr->edit_bcc.wstr_ptr)
        {
            MMIMMS_FREE(editdoc_ptr->edit_bcc.wstr_ptr);
        }
        if(editdoc_ptr->edit_subject.wstr_ptr)
        {
            MMIMMS_FREE(editdoc_ptr->edit_subject.wstr_ptr);
        }
        if(editdoc_ptr->msg_id.wstr_ptr)
        {
            MMIMMS_FREE(editdoc_ptr->msg_id.wstr_ptr);
        }
        if(editdoc_ptr->editbody_ptr)
        {
            MMS_DeleteEditDoc(editdoc_ptr->editbody_ptr);
        }
        if(editdoc_ptr->namelist_header_ptr)
        {
            cursor_ptr = editdoc_ptr->namelist_header_ptr;
            while(PNULL != cursor_ptr)
            {
                SCI_FREE(cursor_ptr->file_name_ptr);
                SCI_FREE(cursor_ptr->vfname_ptr);
                free_ptr = cursor_ptr;
                cursor_ptr = cursor_ptr->next;
                SCI_FREE(free_ptr);
            }
        }
        if(editdoc_ptr->numberlist_header_ptr)
        {
            cursor_numberlist_ptr = editdoc_ptr->numberlist_header_ptr;
            while(PNULL != cursor_numberlist_ptr)
            {
                SCI_FREE(cursor_numberlist_ptr->number_name_ptr);
                SCI_FREE(cursor_numberlist_ptr->number_ptr);
                free_numberlist_ptr = cursor_numberlist_ptr;
                cursor_numberlist_ptr = cursor_numberlist_ptr->next;
                SCI_FREE(free_numberlist_ptr);
            }
        }
        #ifdef MMIMMS_SMS_IN_1_SUPPORT
        #ifdef MMIMMS_SMS_CC_SUPPORT
        if(editdoc_ptr->numberlist_cc_header_ptr)
        {
            cursor_numberlist_ptr = editdoc_ptr->numberlist_cc_header_ptr;
            while(PNULL != cursor_numberlist_ptr)
            {
                SCI_FREE(cursor_numberlist_ptr->number_name_ptr);
                SCI_FREE(cursor_numberlist_ptr->number_ptr);
                free_numberlist_ptr = cursor_numberlist_ptr;
                cursor_numberlist_ptr = cursor_numberlist_ptr->next;
                SCI_FREE(free_numberlist_ptr);
            }
        }
        
        if(editdoc_ptr->numberlist_bcc_header_ptr)
        {
            cursor_numberlist_ptr = editdoc_ptr->numberlist_bcc_header_ptr;
            while(PNULL != cursor_numberlist_ptr)
            {
                SCI_FREE(cursor_numberlist_ptr->number_name_ptr);
                SCI_FREE(cursor_numberlist_ptr->number_ptr);
                free_numberlist_ptr = cursor_numberlist_ptr;
                cursor_numberlist_ptr = cursor_numberlist_ptr->next;
                SCI_FREE(free_numberlist_ptr);
            }
        }
        #endif
        #endif
        MMIMMS_FREE(editdoc_ptr);
    }
}


/*****************************************************************************/
//  Description : 生成虚拟文件名（随随机的ASCII字串加后缀）
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL uint16 MMIMMS_GenRandomNameWithSuffix(
                                             MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr,
                                             MMIFILE_FILE_INFO_T* file_info_ptr, 
                                             uint8 *multim_fname,
                                             uint32 multim_len,
                                             MMIFMM_FILE_TYPE_E   file_type,
                                             MMI_STRING_T *vacard_str
                                             )
{
    uint16 file_suffix_wstr[MMIMMS_SUFFIX_LEN + 1]        = {0};
    wchar  temp_file_suffix_wstr[MMIMMS_SUFFIX_LEN + 1]   = {0};
    wchar  don_wstr[]                                     = {'.', 0};
    uint8  file_suffix[MMIMMS_SUFFIX_LEN*3 + 1]           = {0};
    uint16 suffix_len                                     = MMIMMS_SUFFIX_LEN;
    uint32 suffix_change_len                              = 0;
    uint32 temp_name_len                                  = 0;
    wchar *file_name_str                                  = PNULL;
    uint16 file_name_len                                  = 0;

    P_MMS_SLIDE_T p_mms_slide_temp                       = PNULL;//解决彩信文件文件名重复问题
    P_MMS_CONTENT_T p_mms_content_temp                   = PNULL;
    MMS_ACCESSORY_PARAM_T *accessory_ptr                 = PNULL;
    BOOLEAN is_same_file_name                            = FALSE;
    uint8 *temp_name_ptr                                 = PNULL;
    uint8  random_file_name[MMIMMS_SUFFIX_LEN + 1]       = {0};
    uint16 file_name_len_random                          = 0;
    uint32 random_file_name_len = 0;

    if(MMIMMS_SUFFIX_LEN >= multim_len || PNULL ==multim_fname)
    {
        return 0;
    }
    if (
#if defined MMI_VCARD_SUPPORT        
        MMIFMM_FILE_TYPE_VCARD == file_type && 
#endif
        PNULL != vacard_str)
    {
        file_name_str = vacard_str->wstr_ptr;
        file_name_len = vacard_str->wstr_len;
    }
    else if(PNULL != file_info_ptr)
    {
        file_name_str = file_info_ptr->file_name;
        file_name_len = file_info_ptr->file_name_len;
    }
    else
    {
        return 0;
    }
    temp_name_ptr = (uint8 *)MMIMMS_ALLOC(MMIMMS_FILE_NAME_LEN + 1);
    if (PNULL == temp_name_ptr)
    {
        return 0;
    }
    SCI_MEMSET(temp_name_ptr, 0, (MMIMMS_FILE_NAME_LEN + 1));

    if ((MAX_MULTIFILE_NAME_LENGTH - 1) < file_name_len)
    {
        MMIAPIFMM_SplitFileName(file_name_str, file_name_len, PNULL, PNULL, temp_file_suffix_wstr, &suffix_len);
        suffix_change_len = MIN(suffix_len, MMIMMS_SUFFIX_LEN);
        MMI_WSTRNCPY(file_suffix_wstr, MMIMMS_SUFFIX_LEN, don_wstr, MMIAPICOM_Wstrlen(don_wstr), MMIAPICOM_Wstrlen(don_wstr));
        MMI_WSTRNCPY(file_suffix_wstr + MMIAPICOM_Wstrlen(don_wstr), MMIMMS_SUFFIX_LEN, temp_file_suffix_wstr, suffix_change_len, suffix_change_len);
        suffix_change_len = suffix_change_len + MMIAPICOM_Wstrlen(don_wstr);
        temp_name_len = GUI_WstrToUTF8(temp_name_ptr, MMIMMS_FILE_NAME_LEN, file_name_str, (MAX_MULTIFILE_NAME_LENGTH - suffix_change_len));
        suffix_change_len = GUI_WstrToUTF8(file_suffix, MMIMMS_SUFFIX_LEN*3, file_suffix_wstr, suffix_change_len);
        SCI_MEMCPY(temp_name_ptr + temp_name_len, file_suffix, suffix_change_len);
    }
    else
    {
        GUI_WstrToUTF8(temp_name_ptr, MMIMMS_FILE_NAME_LEN, file_name_str, file_name_len);
    } 

    if (PNULL != edit_doc_ptr && PNULL != edit_doc_ptr->editbody_ptr && PNULL != edit_doc_ptr->editbody_ptr->slide_root)
    {//检查是否有重复文件名的彩信文件
        is_same_file_name = FALSE;
        p_mms_slide_temp = edit_doc_ptr->editbody_ptr->slide_root;
        while (PNULL != p_mms_slide_temp)
        {
            p_mms_content_temp = p_mms_slide_temp->content_root;
            while (PNULL != p_mms_content_temp)
            {
                if (0 == strcmp((char *)temp_name_ptr, (char *)p_mms_content_temp->filename))
                {
                    is_same_file_name = TRUE;
                    break;
                }
                p_mms_content_temp =p_mms_content_temp->next;
            }
            if (!is_same_file_name)
            {
                if (0 == strcmp((char *)temp_name_ptr, (char *)p_mms_slide_temp->audiofile))
                {
                    is_same_file_name = TRUE;
                    break;
                }
            }
            if (!is_same_file_name)
            {
                accessory_ptr = edit_doc_ptr->editbody_ptr->accessory_ptr;
                while (PNULL != accessory_ptr)
                {
                    if (0 == strcmp((char *)temp_name_ptr, (char *)accessory_ptr->filename))
                    {
                        is_same_file_name = TRUE;
                        break;
                    }
                    accessory_ptr = accessory_ptr->next;
                }
            }
            if (is_same_file_name)
            {
                break;
            }
            else
            {
                p_mms_slide_temp = p_mms_slide_temp->next;
            }            
        }
    }
    if (is_same_file_name)
    {
        MMIMMS_GenRandomName(random_file_name, MMIMMS_SUFFIX_LEN);
        MMIAPIFMM_SplitFileName(file_name_str, file_name_len, PNULL, PNULL, temp_file_suffix_wstr, &suffix_len);
        suffix_change_len = MIN(suffix_len, MMIMMS_SUFFIX_LEN);
        MMI_WSTRNCPY(file_suffix_wstr, MMIMMS_SUFFIX_LEN, don_wstr, MMIAPICOM_Wstrlen(don_wstr), MMIAPICOM_Wstrlen(don_wstr));
        MMI_WSTRNCPY(file_suffix_wstr + MMIAPICOM_Wstrlen(don_wstr), MMIMMS_SUFFIX_LEN, temp_file_suffix_wstr, suffix_change_len, suffix_change_len);
        suffix_change_len = suffix_change_len + MMIAPICOM_Wstrlen(don_wstr);
        suffix_change_len = GUI_WstrToUTF8(file_suffix, MMIMMS_SUFFIX_LEN*3, file_suffix_wstr, suffix_change_len);        
        file_name_len_random = MAX_MULTIFILE_NAME_LENGTH - suffix_change_len - SCI_STRLEN((char *)random_file_name);
        file_name_len_random = MIN(file_name_len, file_name_len_random); 
        file_name_len_random = file_name_len_random - suffix_len - 1;
        temp_name_len = GUI_WstrToUTF8(multim_fname, MMIMMS_FILE_NAME_LEN, file_name_str, file_name_len_random);
        random_file_name_len = SCI_STRLEN((char *)random_file_name);
        SCI_MEMCPY(multim_fname + temp_name_len, random_file_name, random_file_name_len);
        SCI_MEMCPY(multim_fname + temp_name_len + random_file_name_len, file_suffix, suffix_change_len);
    }
    else
    {  
        uint16 index = 0;
        uint16 index_deal = 0;
        uint8  temp_ch = 0;

        while (*(temp_name_ptr + index))
        {
            temp_ch = *(temp_name_ptr + index);
            if (0x20 == temp_ch)
            {
                index_deal = index;
                while (*(temp_name_ptr + index_deal))
                {
                    *(temp_name_ptr + index_deal) = *(temp_name_ptr + index_deal + 1);
                    index_deal++;
                }
            }
            else 
            {
                index++;
            }
        }
        random_file_name_len = SCI_STRLEN((char *)temp_name_ptr);
        SCI_MEMCPY(multim_fname, temp_name_ptr, random_file_name_len);
    }

    MMIMMS_FREE(temp_name_ptr);
    temp_name_ptr = PNULL;

    return (SCI_STRLEN((char *)multim_fname));
}

/*****************************************************************************/
//  Description : add file to mms
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_AddFile(
                              MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr,
                              MMIFILE_FILE_INFO_T   *file_info_ptr,
                              MMIFMM_FILE_TYPE_E   file_type,
                              BOOLEAN is_replace_multifile,
                              uint32 replace_multifile_size
                              )
{
    BOOLEAN                     result = FALSE;
    BOOLEAN                     file_valid = FALSE;
    GUIANIM_TYPE_E              pic_type = GUIANIM_TYPE_NONE;
    MMISRVAUD_RING_FMT_E             music_type = MMISRVAUD_RING_FMT_MIDI;
    MMICOM_VIDEO_TYPE_E             movie_type = MMICOM_VIDEO_TYPE_MAX;
    wchar full_path_name [MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32  send_file_size = 0;
    uint8* send_data_ptr = PNULL;
//  uint32 data_buf_offset = 0;
    uint32  type = 0;
    int8 multim_fname[MMIMMS_FILE_NAME_LEN + 1] = {0};
    FILE_HANDLE file_handle = 0;
    uint32 duration = 0;
    MMIFILE_HANDLE  read_file_handle = SFS_INVALID_HANDLE;
    uint32 real_read_size            = 0;    
#ifdef DRM_SUPPORT
    uint16 suffix_wstr[MMIMMS_SUFFIX_LEN + 1]        = {0};
    uint16 suffix_len                                = MMIMMS_SUFFIX_LEN;        
    BOOLEAN is_drm_file = FALSE;
    uint32 drm_mime_type = 0;
    uint32 drm_file_type = 0;
#endif

    if(PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == file_info_ptr)
    {
        return FALSE;
    }
    
    if(MMIAPIFMM_CombineFullPath(file_info_ptr->device_name, file_info_ptr->device_name_len,
            file_info_ptr->dir_name, file_info_ptr->dir_name_len,
            file_info_ptr->file_name, file_info_ptr->file_name_len,
            full_path_name, &full_path_len))
    {
        uint32 insert_total_size = 0;
        uint16 reduce_width = 0;
        uint16 reduce_height = 0;

#ifdef DRM_SUPPORT 
		MMIAPIFMM_SplitFileName(file_info_ptr->file_name, file_info_ptr->file_name_len, PNULL, PNULL, suffix_wstr, &suffix_len);
		drm_file_type = MMIAPIFMM_GetFileType(suffix_wstr, suffix_len);    
		if (MMIFMM_DCF_FILE == drm_file_type)
		{
			drm_mime_type = MMIAPIDRM_GetDRMFileMimeType(SFS_INVALID_HANDLE, full_path_name);
			is_drm_file = TRUE;
		}
#endif
		GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &reduce_width, &reduce_height);
        reduce_width = MIN(MMIMMS_PIC_REDUCED_WIDTH_MAX, reduce_width);
        reduce_height = MIN(MMIMMS_PIC_REDUCED_HEIGHT_MAX, reduce_height);
        
        send_file_size = reduce_width*reduce_height*2;

        if (is_replace_multifile && 0 != replace_multifile_size)
        {
            insert_total_size = (uint32)edit_doc_ptr->editbody_ptr->totalsize - replace_multifile_size + file_info_ptr->file_size;
        }
        else
        {
            insert_total_size = (uint32)edit_doc_ptr->editbody_ptr->totalsize + file_info_ptr->file_size;
        }

        if (MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT < insert_total_size
            && file_info_ptr->file_size > send_file_size
            && insert_total_size - file_info_ptr->file_size + send_file_size <= MMIMMS_MAX_CONTENT_SIZE)
        {
            send_data_ptr = MMIMMS_ALLOC(send_file_size);

            if(ReducePicForMMS(send_data_ptr, send_file_size, full_path_name, reduce_width, reduce_height))
            {
                file_info_ptr->file_size = send_file_size;
                file_valid = TRUE; 
            }
            else
            {
                MMIMMS_FREE(send_data_ptr);
                send_data_ptr = PNULL;
            }
        }
        else
        {
            send_file_size = file_info_ptr->file_size;
            send_data_ptr = MMIMMS_ALLOC(file_info_ptr->file_size);
            if (PNULL !=send_data_ptr)
            {
                read_file_handle = MMIAPIFMM_CreateFile(full_path_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);
                if (SFS_INVALID_HANDLE == read_file_handle)
                {
                    MMIMMS_FREE(send_data_ptr);
                    send_data_ptr = PNULL;
                    return FALSE;
                }
#ifdef DRM_SUPPORT
				MMIAPIDRM_SDSetEncrypt( read_file_handle, TRUE);
				if(is_drm_file)
				{
					file_type = MMIDRM_GetMediaFileType(read_file_handle);
				}
#endif
                //if (PNULL !=send_data_ptr)
                {
                    MMIAPIFMM_ReadFile(read_file_handle, send_data_ptr, file_info_ptr->file_size, &real_read_size, PNULL);
                    //if(MMIAPIFMM_ReadFilesDataSyn(full_path_name, full_path_len, send_data_ptr, file_info_ptr->file_size))
                    if(0 < real_read_size)
                    {
                        file_valid = TRUE;
                    }
                    else
                    {
                        MMIMMS_FREE(send_data_ptr);
                        send_data_ptr = PNULL;
                    }
                    
                }
#if defined MMI_VCARD_SUPPORT        
                if (MMIFMM_FILE_TYPE_VCARD == file_type 
#ifdef MMI_VCALENDAR_SUPPORT
                    || MMIFMM_FILE_TYPE_VCALENDAR== file_type
#endif
                    )
                {
                    MMIAPIFMM_CloseFile(read_file_handle);
                    MMIAPIFMM_DeleteFileSyn(full_path_name, full_path_len);                    
                }
                else
#endif
                {
                    MMIAPIFMM_CloseFile(read_file_handle);
                }
            }
        }
    }
    
    if(file_valid)
    {
        switch(file_type)
        {
        case MMIFMM_FILE_TYPE_PICTURE:
#ifdef DRM_SUPPORT
            if (is_drm_file)
            {                
                switch(drm_mime_type) 
                {
                case MIME_TYPE_IMAGE_JPEG:
                    pic_type = GUIANIM_TYPE_IMAGE_JPG;
                    break;
                case MIME_TYPE_IMAGE_GIF:
                case MIME_TYPE_IMAGE_TIFF:
                    pic_type = GUIANIM_TYPE_IMAGE_GIF;
                    break;
                case MIME_TYPE_IMAGE_PNG:
                    pic_type = GUIANIM_TYPE_IMAGE_PNG;
                    break;
                case MIME_TYPE_IMAGE_BMP:
                    pic_type = GUIANIM_TYPE_IMAGE_BMP;
                    break;
                case MIME_TYPE_IMAGE_VND_WAP_WBMP:
                    pic_type = GUIANIM_TYPE_IMAGE_WBMP;
                    break;
                default:
                    break;
                }
            }
            else
#endif
            {
                pic_type = MMIAPICOM_GetImgaeType(file_info_ptr->file_name, file_info_ptr->file_name_len);
            }
            type = ImageTypeConvert(pic_type);
            MMIMMS_DelImage(edit_doc_ptr);
            break;
            
        case MMIFMM_FILE_TYPE_MUSIC:
#ifdef DRM_SUPPORT
            if (is_drm_file)
            {                
                switch(drm_mime_type) 
                {
                case MIME_TYPE_AUDIO_MIDI:
                case MIME_TYPE_AUDIO_X_MIDI:
                case MIME_TYPE_AUDIO_SP_MIDI:
                case MIME_TYPE_AUDIO_MID:
                    music_type = MMISRVAUD_RING_FMT_MIDI;
                	break;
               case MIME_TYPE_AUDIO_X_WAV:
                    music_type = MMISRVAUD_RING_FMT_WAVE;
                	break;
              case MIME_TYPE_AUDIO_AMR:
                    music_type = MMISRVAUD_RING_FMT_AMR;
                	break;
                default:
                    break;
                }
            }
            else
#endif
            {
                music_type = MMIAPICOM_GetMusicType( file_info_ptr->file_name, file_info_ptr->file_name_len);
            }
            type = AudioTypeConvert(music_type);
            MMIMMS_DelMusic(edit_doc_ptr);
            break;
            
        case MMIFMM_FILE_TYPE_MOVIE:
#ifdef DRM_SUPPORT
            if (is_drm_file)
            {                
                switch(drm_mime_type) 
                {
                case MIME_TYPE_VIDEO_3GPP:
                    movie_type = MMICOM_VIDEO_TYPE_3GP;
                    break;
               case MIME_TYPE_VIDEO_MPEG:
                    movie_type = MMICOM_VIDEO_TYPE_MP4;
                    break;
#ifdef AVI_DEC_SUPPORT 
               case MIME_TYPE_VIDEO_AVI:
                    movie_type = MMICOM_VIDEO_TYPE_AVI;
                    break;
#endif
                default:
                    break;
                }
            }
            else
#endif
            {
                movie_type = MMIAPICOM_GetMovieType( file_info_ptr->file_name, file_info_ptr->file_name_len);
            }
            type = VideoTypeConvert(movie_type);
            MMIMMS_DelVideo(edit_doc_ptr);
            break;
            
        default:
            break;
        }
        {
            int16  i = 0;
            BOOLEAN is_ascii = TRUE;
            BOOLEAN name_temp_len = 0;
            uint8 *name_ptr = PNULL;

            MMIMMS_GenRandomNameWithSuffix(edit_doc_ptr, file_info_ptr,(uint8 *)multim_fname,(MMIMMS_FILE_NAME_LEN-1),file_type, PNULL);
            name_temp_len = strlen((char*)multim_fname);
            name_ptr = multim_fname;

            for (i = 0; i < name_temp_len; i++)
            {
                if (0x80 < *(name_ptr + i))
                {
                    is_ascii = FALSE;
                    break;
                }
            }

            if (!is_ascii && name_temp_len >= 1)
            {
                for (i = name_temp_len - 1; i >= 0; i--)
                {
                    if (*((char*)multim_fname + i) == '.')
                    {
                        char ext[(MMIMMS_SUFFIX_LEN + 1) + 1] = {0};//including .

                        SCI_DATE_T              cur_date = {0};
                        SCI_TIME_T              cur_time = {0};

                        SCI_MEMSET(ext, 0, sizeof(ext));
                        _snprintf(ext, MMIMMS_SUFFIX_LEN, "%s", multim_fname + i);

                        TM_GetSysDate(&cur_date);
                        TM_GetSysTime(&cur_time);

                        SCI_MEMSET(multim_fname, 0, sizeof(multim_fname));
                        _snprintf((char*)multim_fname, (MMIMMS_FILE_NAME_LEN - (MMIMMS_SUFFIX_LEN + 1)),"%04d%02d%02d%s%02d%02d%02d", cur_date.year, cur_date.mon, cur_date.mday, "_", cur_time.hour, cur_time.min, cur_time.sec);
                        _snprintf((char*)(multim_fname + strlen(multim_fname)), (MMIMMS_SUFFIX_LEN + 1),"%s", (char*)ext);
                        break;
                    }
                }
            }
        }
//        MMI_WSTRNTOSTR(multim_fname, (MMIMMS_FILE_NAME_LEN-1), file_info_ptr->file_name, file_info_ptr->file_name_len, file_info_ptr->file_name_len);
        //获取输入的文件名/唯一文件名
        file_handle = sysVFopen((int8*)multim_fname,"w");
        
        if (!file_handle )
        {
            //SCI_TRACE_LOW:"open file error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_2250_112_2_18_2_37_36_11,(uint8*)"");
            MMIMMS_FREE(send_data_ptr);
            return FALSE;
        }
        
        sysVFwrite((int8 *)send_data_ptr, 1, file_info_ptr->file_size,(FILE_HANDLE) file_handle);
        sysVFclose((FILE_HANDLE)file_handle);
        switch(file_type)
        {
        case MMIFMM_FILE_TYPE_PICTURE:
            if(MMS_AddConetnt((char *)multim_fname,type,edit_doc_ptr->editbody_ptr->slide_cur,edit_doc_ptr->editbody_ptr))
            {
                //SCI_TRACE_LOW:"MMIMMS_AddFile add picture"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_2262_112_2_18_2_37_36_12,(uint8*)"");
                result = TRUE;
            }
            break;
            
        case MMIFMM_FILE_TYPE_MUSIC:
            if (0 == MMS_AddNewAudio((uint8 *)multim_fname,type,edit_doc_ptr->editbody_ptr->slide_cur,edit_doc_ptr->editbody_ptr))
            {
                //SCI_TRACE_LOW:"MMIMMS_AddFile add music"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_2270_112_2_18_2_37_36_13,(uint8*)"");
                AddFileNameToNameList(edit_doc_ptr,
                    file_info_ptr->file_name,
                    file_info_ptr->file_name_len,
                    (uint8 *)multim_fname,
                    SCI_STRLEN((char *)multim_fname));

                //计算duration时间
                duration = MMIMMS_GetCurrentSlideMusicTime(edit_doc_ptr);//获取时间
                if(duration > edit_doc_ptr->editbody_ptr->slide_cur->times)
                {
                    MMIMMS_SetCurSlideDuration(duration,edit_doc_ptr);
                }

                result = TRUE;
            }
            break;
            
        case MMIFMM_FILE_TYPE_MOVIE:
            if(MMS_AddConetnt((char *)multim_fname,type,edit_doc_ptr->editbody_ptr->slide_cur,edit_doc_ptr->editbody_ptr))
            {
                //SCI_TRACE_LOW:"MMIMMS_AddFile add movie"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_2291_112_2_18_2_37_36_14,(uint8*)"");

                //计算duration时间
                duration = MMIMMS_GetCurrentSlideVideoTime(edit_doc_ptr);//获取时间
                if(duration > edit_doc_ptr->editbody_ptr->slide_cur->times)
                {
                    duration += MMIMMS_SLIDE_PLAY_DEFAULT_TIME;
                    MMIMMS_SetCurSlideDuration(duration,edit_doc_ptr);
                }

                result = TRUE;
            }
            break;

        case MMIFMM_FILE_TYPE_NORMAL:
#if defined MMI_VCARD_SUPPORT
        case MMIFMM_FILE_TYPE_VCARD:
#endif
#ifdef MMI_VCALENDAR_SUPPORT
        case MMIFMM_FILE_TYPE_VCALENDAR:
#endif
#ifdef JAVA_SUPPORT
        case MMIFMM_FILE_TYPE_JAVA:
#endif
#ifdef EBOOK_SUPPORT
        case MMIFMM_FILE_TYPE_EBOOK:
#endif
            if(MMS_AddAttachment((char *)multim_fname, edit_doc_ptr->editbody_ptr))
            {
                //SCI_TRACE_LOW:"MMS_AddAttachment add normal file attachment"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_2320_112_2_18_2_37_36_15,(uint8*)"");
                result = TRUE;
            }
            break;
            
        default:
            break;
        }
    }
    if(send_data_ptr)
    {
        MMIMMS_FREE(send_data_ptr);
    }
    return result;
}

/*****************************************************************************/
//  Description : add attachment to mms
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_AddAndReplaceAttachment(
                                              MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr,
                                              MMIFILE_FILE_INFO_T   *file_info_ptr,
                                              uint8 *replaced_name_ptr,
                                              uint8   *file_data_ptr,
                                              uint32 file_data_len,
                                              MMIFMM_FILE_TYPE_E   file_type,
                                              MMI_STRING_T *vacard_str
                                              )
{
    BOOLEAN  result                                    = FALSE;
    uint32  send_file_size                             = 0;
    uint8* send_data_ptr                               = PNULL;
    uint8 multim_fname[MMIMMS_FILE_NAME_LEN +1]        = {0};
    FILE_HANDLE file_handle                            = 0;
    wchar full_path_name [MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 full_path_len                               = MMIFILE_FULL_PATH_MAX_LEN;
    BOOLEAN  file_valid                                = FALSE;
        
    if(PNULL == edit_doc_ptr || 
        PNULL == edit_doc_ptr->editbody_ptr || 
        (PNULL == file_info_ptr 
#if defined MMI_VCARD_SUPPORT
        && MMIFMM_FILE_TYPE_VCARD != file_type
#endif
        )
        )
    {
        return FALSE;
    }
    if (MMIFMM_FILE_TYPE_NORMAL == file_type)
    {
        if(
#if defined MMI_VCARD_SUPPORT
            PNULL != file_info_ptr && 
#endif        
            MMIAPIFMM_CombineFullPath(file_info_ptr->device_name, file_info_ptr->device_name_len,
            file_info_ptr->dir_name, file_info_ptr->dir_name_len,
            file_info_ptr->file_name, file_info_ptr->file_name_len,
            full_path_name, &full_path_len))
            
        {  
            send_file_size = file_info_ptr->file_size;
            send_data_ptr = MMIMMS_ALLOC(file_info_ptr->file_size);
            if (PNULL !=send_data_ptr)
            {
                if(MMIAPIFMM_ReadFilesDataSyn(full_path_name, full_path_len, send_data_ptr, file_info_ptr->file_size))
                {
                    file_valid = TRUE;
                }
                else
                {
                    MMIMMS_FREE(send_data_ptr);
                    send_data_ptr = PNULL;
                }
                
            }
        }
    }
#if defined MMI_VCARD_SUPPORT
    else if(MMIFMM_FILE_TYPE_VCARD == file_type)
    {
        send_data_ptr = MMIMMS_ALLOC(file_data_len);
        if (PNULL == send_data_ptr)
        {
            return FALSE;
        }
        file_valid = TRUE;
        send_file_size = file_data_len;
        SCI_MEMSET(send_data_ptr, 0 ,file_data_len);
        SCI_MEMCPY(send_data_ptr, file_data_ptr, file_data_len);
    }
#endif

    if (!file_valid)
    {
        return FALSE;
    }
#if defined MMI_VCARD_SUPPORT    
    if (
#if defined MMI_VCARD_SUPPORT
        MMIFMM_FILE_TYPE_VCARD == file_type && 
#endif
        PNULL != vacard_str && 
        strlen(".vcf") == vacard_str->wstr_len
        )//如果名称为空，只有后缀名
    {
        MMIMMS_GenRandomName(multim_fname, (MMIMMS_FILE_NAME_LEN-1));
        strcat((char *)multim_fname,".vcf");
    }
    else
#endif        
    {//for cr243821 当有外国服务器不识别utf8编码的时候做容错
        MN_PLMN_T select_plmn = {0, 0, 2};
        uint32 total_num = 0;
        uint32 sim_card_num = 0;
        uint16 sim_sys  = MN_DUAL_SYS_1;
        int16 i = 0;
        BOOLEAN is_nigeria_net = FALSE;
        BOOLEAN name_temp_len = 0;
        uint8 *name_ptr = PNULL;
        
        total_num = sizeof(s_nigeria_mcc_mnc_table) / sizeof(MMS_NAME_MCC_MNC_T);
        sim_card_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
        if (0 < sim_card_num)
        {
            MMIAPIPHONE_GetCurPLMN(&select_plmn, (MN_DUAL_SYS_E)sim_sys);
        }
        for (i = 0; i < total_num; i++)
        {
            if (select_plmn.mcc == s_nigeria_mcc_mnc_table[i].mcc && select_plmn.mnc == s_nigeria_mcc_mnc_table[i].mnc)
            {
                is_nigeria_net = TRUE;
                break;
            }
        }
        MMIMMS_GenRandomNameWithSuffix(edit_doc_ptr, file_info_ptr,(uint8 *)multim_fname,(MMIMMS_FILE_NAME_LEN-1),file_type, vacard_str);
        name_temp_len = strlen((char*)multim_fname);
        if (is_nigeria_net)
        {//如果是尼日尼亚，怎把uft8里面打不是asc码换成下画线
            name_ptr = (uint8 *)multim_fname;
            for (i = 0; i < name_temp_len; i++)
            {
                if (0x80 < *(name_ptr + i))
                {
                    *(name_ptr + i) = '_';
                }
            }
        }        

        {
            BOOLEAN is_ascii = TRUE;
            name_ptr = multim_fname;
            
            for (i = 0; i < name_temp_len; i++)
            {
                if (0x80 < *(name_ptr + i))
                {
                    is_ascii = FALSE;
                    break;
                }
            }
            
            if (!is_ascii && name_temp_len >= 1)
            {
                for (i = name_temp_len - 1; i >= 0; i--)
                {
                    if (*((char*)multim_fname + i) == '.')
                    {
                        char ext[(MMIMMS_SUFFIX_LEN + 1) + 1] = {0};//including .
                        
                        SCI_DATE_T              cur_date = {0};
                        SCI_TIME_T              cur_time = {0};
                        
                        SCI_MEMSET(ext, 0, sizeof(ext));
                        _snprintf(ext, MMIMMS_SUFFIX_LEN, "%s", multim_fname + i);
                        
                        TM_GetSysDate(&cur_date);
                        TM_GetSysTime(&cur_time);
                        
                        SCI_MEMSET(multim_fname, 0, sizeof(multim_fname));
                        _snprintf((char*)multim_fname, (MMIMMS_FILE_NAME_LEN - (MMIMMS_SUFFIX_LEN + 1)),"%04d%02d%02d%s%02d%02d%02d", cur_date.year, cur_date.mon, cur_date.mday, "_", cur_time.hour, cur_time.min, cur_time.sec);
                        _snprintf((char*)(multim_fname + strlen(multim_fname)), (MMIMMS_SUFFIX_LEN + 1),"%s", (char*)ext);
                        break;
                    }
                }
            }
        }
    }
    
    //获取输入的文件名/唯一文件名
    file_handle = sysVFopen((int8*)multim_fname,"w");    
    if (!file_handle )
    {
        //SCI_TRACE_LOW:"open file error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_2461_112_2_18_2_37_37_16,(uint8*)"");
        MMIMMS_FREE(send_data_ptr);
        return FALSE;
    }    
    sysVFwrite((int8 *)send_data_ptr, 1, send_file_size,(FILE_HANDLE) file_handle);
    sysVFclose((FILE_HANDLE)file_handle);

    switch(file_type)
    {
#if defined MMI_VCARD_SUPPORT
    case MMIFMM_FILE_TYPE_VCARD:
#endif
    case MMIFMM_FILE_TYPE_NORMAL:
        if (PNULL == replaced_name_ptr)
        {
            if(MMS_AddAttachment((char *)multim_fname, edit_doc_ptr->editbody_ptr))
            {
                //SCI_TRACE_LOW:"MMS_AddAttachment add attachment"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_2478_112_2_18_2_37_37_17,(uint8*)"");
                result = TRUE;
            }
        }
        else
        {
            if(MMS_ReplaceAttachment((char *)replaced_name_ptr, (char *)multim_fname, edit_doc_ptr->editbody_ptr))
            {
                //SCI_TRACE_LOW:"MMIMMS_ReplaceAttachment replace attachment"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_2486_112_2_18_2_37_37_18,(uint8*)"");
                result = TRUE;
            }
        }
        
        break;
    default:
        break;
    }
    
    MMIMMS_FREE(send_data_ptr);
    
    return result;
}

/*****************************************************************************/
//  Description :get attachment number of the edit body
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetAttachmentNumber(
                             P_MMS_EDIT_BODY_T  pDocument,          //[IN]the edit doc
                             uint32             *attachment_num_ptr //[OUT]the total number of attachment
                             )
{
    return MMS_GetAttachmentNumber(pDocument, attachment_num_ptr);
}

/*****************************************************************************/
//  Description :Get appointed attachment file size
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetAttachmentSize(
                                        char               *pFilename,     //[IN]the filename of content
                                        P_MMS_EDIT_BODY_T  pDocument,      //[IN]the edit doc
                                        uint32             *file_size_ptr  //[OUT]appointed attachment file size
                                        )
{
    return MMS_GetAttachmentSize(pFilename, pDocument, file_size_ptr);
}

/*****************************************************************************/
//  Description : 设置文字图片的排版方式，对单条MMS所有帧有效
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetTextImageOrder(
                                      MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr,
                                      MMS_TEXT_IMAGE_ORDER_E  order_type  
                                      )
{
    P_MMS_SLIDE_T slide = PNULL;
    if(edit_doc_ptr)
    {
        edit_doc_ptr->textimage_order = order_type;
        slide = edit_doc_ptr->editbody_ptr->slide_root;
        for(; PNULL != slide; slide = slide->next)
        {
            slide->textimage_order = edit_doc_ptr->textimage_order;
        }
    }
}

/*****************************************************************************/
//  Description : 获取文字图片的排版方式，对单条MMS所有帧有效
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMS_TEXT_IMAGE_ORDER_E MMIMMS_GetTextImageOrder(
                                                       MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr  
                                                       )
{
    MMS_TEXT_IMAGE_ORDER_E order_type = MMS_ORDER_TEXT_FIRST;
    if(edit_doc_ptr)
    {
        order_type = edit_doc_ptr->editbody_ptr->slide_cur->textimage_order;
    }

    return order_type;
}

/*****************************************************************************/
//  Description : 获取文字图片的排版方式，对单条MMS所有帧有效
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsWithAttachment(
                                       MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr 
                                       )
{
    BOOLEAN return_val = FALSE;
    if(PNULL != edit_doc_ptr && PNULL != edit_doc_ptr->editbody_ptr->accessory_ptr)
    {
        return_val = TRUE; 
    }
    return return_val;
}

/*****************************************************************************/
//  Description : add filename to name list
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void AddFileNameToNameList(MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr,
                                 wchar      *file_name_ptr,
                                 uint16     file_name_len,
                                 uint8      *vfname_ptr,
                                 uint16     vfname_len
                                 )
{
    MMIMMS_EDIT_NAME_LIST_T *cursor_ptr = PNULL;

    if(PNULL != edit_doc_ptr && PNULL != file_name_ptr && PNULL != vfname_ptr
        && 0 != file_name_len && 0 != vfname_len)
    {
        if(PNULL == edit_doc_ptr->namelist_header_ptr)
        {
            edit_doc_ptr->namelist_header_ptr = MMIMMS_ALLOC(sizeof(MMIMMS_EDIT_NAME_LIST_T));
            SCI_MEMSET(edit_doc_ptr->namelist_header_ptr,0,sizeof(MMIMMS_EDIT_NAME_LIST_T));
            edit_doc_ptr->namelist_header_ptr->file_name_ptr = MMIMMS_ALLOC(file_name_len*sizeof(wchar));
            MMIAPICOM_Wstrncpy(edit_doc_ptr->namelist_header_ptr->file_name_ptr,file_name_ptr,file_name_len);
            edit_doc_ptr->namelist_header_ptr->file_name_len = file_name_len;
            edit_doc_ptr->namelist_header_ptr->vfname_ptr = MMIMMS_ALLOC(vfname_len);
            SCI_MEMCPY(edit_doc_ptr->namelist_header_ptr->vfname_ptr,vfname_ptr,vfname_len);
            edit_doc_ptr->namelist_header_ptr->vfname_len = vfname_len;
        }
        else
        {
            cursor_ptr = edit_doc_ptr->namelist_header_ptr;
            while(PNULL != cursor_ptr->next)
            {
                cursor_ptr = cursor_ptr->next;
            }
            cursor_ptr->next = MMIMMS_ALLOC(sizeof(MMIMMS_EDIT_NAME_LIST_T));
            SCI_MEMSET(cursor_ptr->next,0,sizeof(MMIMMS_EDIT_NAME_LIST_T));
            cursor_ptr->next->file_name_ptr = MMIMMS_ALLOC(file_name_len*sizeof(wchar));
            MMIAPICOM_Wstrncpy(cursor_ptr->next->file_name_ptr,file_name_ptr,file_name_len);
            cursor_ptr->next->file_name_len = file_name_len;
            cursor_ptr->next->vfname_ptr = MMIMMS_ALLOC(vfname_len);
            SCI_MEMCPY(cursor_ptr->next->vfname_ptr,vfname_ptr,vfname_len);
            cursor_ptr->next->vfname_len = vfname_len;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]:AddFileNameToNameList fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_2626_112_2_18_2_37_37_19,(uint8*)"");
    }
}


/*****************************************************************************/
//  Description : remove filename from name list
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void RemoveFileNameFromNameList(MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr,
                                      uint8      *vfname_ptr,
                                      uint16     vfname_len
                                      )
{
    MMIMMS_EDIT_NAME_LIST_T *cursor_ptr = PNULL;
    MMIMMS_EDIT_NAME_LIST_T *free_ptr = PNULL;

    if(PNULL != edit_doc_ptr && PNULL != vfname_ptr && 0 != vfname_len)
    {
        cursor_ptr = edit_doc_ptr->namelist_header_ptr;
        if(PNULL != cursor_ptr)
        {
            if(0 == strncmp((char *)cursor_ptr->vfname_ptr,(char *)vfname_ptr,vfname_len))
            {
                edit_doc_ptr->namelist_header_ptr = cursor_ptr->next;
                SCI_FREE(cursor_ptr->file_name_ptr);
                SCI_FREE(cursor_ptr->vfname_ptr);
                SCI_FREE(cursor_ptr);
            }
            else
            {
                while(PNULL !=cursor_ptr->next)
                { 
                    if(0 == strncmp((char *)cursor_ptr->next->vfname_ptr,(char *)vfname_ptr,vfname_len))
                    {
                        SCI_FREE(cursor_ptr->next->file_name_ptr);
                        SCI_FREE(cursor_ptr->next->vfname_ptr);
                        free_ptr = cursor_ptr->next;
                        cursor_ptr->next = cursor_ptr->next->next;
                        SCI_FREE(free_ptr);
                        break;
                    }
                    cursor_ptr = cursor_ptr->next;
                }
            }
        } 
    }
}

/*****************************************************************************/
//  Description : 根据虚拟文件名获取对应的实际文件名
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIMMS_GetFileNameFromVfname(
                                           MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr,
                                           wchar      *file_name_ptr,          //[OUT]
                                           uint16     max_file_name_len,       //[OUT]
                                           uint8      *vfname_ptr,             //[IN]
                                           uint16     vfname_len               //[IN]
                                           )
{
    uint16  file_name_len = 0;
    MMIMMS_EDIT_NAME_LIST_T *cursor_ptr = PNULL;

    if(PNULL !=edit_doc_ptr &&  PNULL != file_name_ptr && PNULL != vfname_ptr && 0 != max_file_name_len && 0 != vfname_len)
    {
        cursor_ptr = edit_doc_ptr->namelist_header_ptr;
        while(PNULL !=cursor_ptr)
        { 
            if(0 == strncmp((char *)cursor_ptr->vfname_ptr,(char *)vfname_ptr,vfname_len))
            {
                file_name_len = MIN(max_file_name_len,cursor_ptr->file_name_len);
                MMIAPICOM_Wstrncpy(file_name_ptr,cursor_ptr->file_name_ptr,file_name_len);
                break;
            }
            cursor_ptr = cursor_ptr->next;
        }
        if(0 == file_name_len)
        {
            file_name_len = MIN(max_file_name_len,vfname_len);
            MMI_STRNTOWSTR(file_name_ptr, max_file_name_len, (uint8 *)vfname_ptr, vfname_len, file_name_len);
        }
    }

    return file_name_len;
}

/*****************************************************************************/
//  Description : ReducePicForMMS
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReducePicForMMS(uint8 *bmp_ptr, uint32 bmp_size, uint16 *file_name_ptr, uint32 w, uint32 h)
{
    IMG_DEC_FRAME_IN_PARAM_T    in_param = {0};
    IMG_DEC_FRAME_OUT_PARAM_T   out_param = {0};
    IMG_DEC_SRC_T               img_dec_src = {0};
    uint32                      handle = 0;

    if (PNULL == bmp_ptr || PNULL == file_name_ptr)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:ReducePicForMMS PNULL == bmp_ptr || PNULL == file_name_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_2726_112_2_18_2_37_37_20,(uint8*)"");
        return FALSE;
    }
       
    //img_dec_src.src_file_size = file_len;
    img_dec_src.file_name_ptr = file_name_ptr;

    if (IMG_DEC_RET_SUCCESS == IMG_DEC_Create(&img_dec_src,&handle))
    {
        in_param.callback = PNULL;
        in_param.handle = handle;
        in_param.data_format = IMG_DEC_JPEG;
        
        in_param.target_ptr = bmp_ptr;
        in_param.target_buf_size = bmp_size;
        in_param.target_width = w;
        in_param.target_height = h;
        in_param.write_data = PNULL;
        in_param.is_dec_thumbnail = FALSE;
    }
    else
    {
        return FALSE;
    }
         
    if (IMG_DEC_RET_SUCCESS != IMG_DEC_GetFrame(&in_param,&out_param))
    {
        return FALSE;
    }
    
    if (IMG_DEC_RET_SUCCESS != IMG_DEC_Destroy(handle))
    {
        return FALSE;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description :get current slide video time
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetCurrentSlideVideoTime(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr)
{
    uint32  video_time=0;
#ifdef VIDEO_PLAYER_SUPPORT
    uint32 video_type     = MMS_TYPE_MAX;
    uint8 play_video_type = MMICOM_VIDEO_TYPE_MAX;
    uint32 video_buf_len  = 0;
    uint8 *video_buf_ptr  = PNULL;
    DPLAYER_MEDIA_INFO_T media_info = {0};    
    
    
    if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
    {
        return video_time;
    }

    if (MMIMMS_GetCurrentSlideVideo(&video_type, &video_buf_len, &video_buf_ptr, edit_doc_ptr))
    {
        //SCI_TRACE_LOW:"MMIMMS_GetCurrentSlideVideoTime video_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_2786_112_2_18_2_37_37_21,(uint8*)"d",video_type);
        play_video_type = (uint8)MMICOM_VIDEO_TYPE_3GP;

        MMIAPIVP_GetMediaInfoFromBuf(video_buf_ptr,video_buf_len,(MMICOM_VIDEO_TYPE_E)play_video_type,&media_info);
        video_time = media_info.media_length;         
    }
#endif
    return video_time;
}


/*****************************************************************************/
//  Description :get current slide music time
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetCurrentSlideMusicTime(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr)
{
    uint32 music_time                = 0;
    MMISRVAUD_CONTENT_INFO_T mp3_file_info = {0};
    MMISRVAUD_RING_FMT_E   fmt = 0;
    uint32  music_buf_len            = 0;
    uint8 *music_buf_ptr             = PNULL;

    if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
    {
        return music_time;
    }
    if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize 
        && MMIMMS_GetCurrentSlideAudio(&fmt, &music_buf_len, &music_buf_ptr, edit_doc_ptr))
    {
        if (MMISRVAUD_GetBuffContentInfo(music_buf_ptr, music_buf_len, fmt, &mp3_file_info))
        {
            music_time = mp3_file_info.total_time;
            //SCI_TRACE_LOW:"GetCurrentSlideMusicTime  music_time= %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDIT_2819_112_2_18_2_37_37_22,(uint8*)"d", music_time);
        }
    }
    music_time = music_time* 1000;
    return music_time;
}

#define MMIMMS_SUFFIX_MAX_LEN   6

/*****************************************************************************/
//  Description : get file type
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIMMS_GetFileType(
                                             const wchar  *full_path_ptr, //in
                                             uint16       full_path_len   //in
                                             )
{
    const wchar         txt_str[] = {'t', 'x', 't', 0};//"bmp";
    MMI_STRING_T        suffix_str = {0};
    MMIFMM_FILE_TYPE_E      file_type = MMIFMM_FILE_TYPE_NORMAL;

    if (PNULL == full_path_ptr)
    {
        SCI_TRACE_LOW("MMIMMS_GetFileType full_path_ptr NULL");
        return MMIFMM_FILE_TYPE_NORMAL;
    }
    if (0 < full_path_len)
    {
        suffix_str.wstr_ptr = SCI_ALLOC_APP( ( MMIMMS_SUFFIX_MAX_LEN + 1 ) * sizeof(wchar) );

        SCI_MEMSET(suffix_str.wstr_ptr, 0, (( MMIMMS_SUFFIX_MAX_LEN + 1 ) * sizeof(wchar)) );

        MMIAPICOM_GetNameAndSuffix( full_path_ptr, full_path_len, PNULL, &suffix_str );

        MMIAPICOM_Wstrlower( suffix_str.wstr_ptr );

        if (0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, txt_str))
        {
            file_type = MMIFMM_FILE_TYPE_EBOOK;
        }
        // can be added to do here

        //free memory
        SCI_FREE(suffix_str.wstr_ptr);
    }

    return (file_type);
}

