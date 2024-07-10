/****************************************************************************
** File Name:      mmisms_filecontrol.c                                     *
** Author:                                                                  *
** Date:           08/15/2012                                               *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe sms file control.          *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
**   DATE              NAME             DESCRIPTION                        *
** 08/15/2012        jixin.xu          Create for sms reconstruction
** 
****************************************************************************/
#include "mmi_app_sms_trc.h"
#if defined(MMI_SMS_EXPORT_SUPPORT)
/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_common.h"
#include "mmisms_id.h"
#include "mmisms_filecontrol.h"
#include "mmifmm_export.h"
#include "mmisms_text.h"
#include "mmisms_internal.h"
#include "mmidisplay_data.h"
#include "mmisms_image.h"
//#include "mmismsapp_wintab.h"
#include "mmipub.h"
#include "mmisms_order.h"
#include "mmisd_export.h"
#include "gui_ucs2b_converter.h"
#include "mmisms_read.h"
//#include "mmipb_text.h"
#include "mmimms_internal.h"
#include "mmiudisk_export.h"
#include "mmisms_filewin.h"
#include "mmismsapp_interface.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define MMISMS_FILE_MIN_SIZE 50
#define MMISMS_FILE_TIME_LEN 40
#define MMISMS_LEFT_BRACKET  0x28  //"("
#define MMISMS_RIGHT_BRACKET 0x29  //")"
#define MMISMS_DEFAULT_EXPORT_DIR (g_mmisms_default_export_dir_str)
#define MMISMS_DEFAULT_EXPORT_DIR_LEN  3//in the unit of uint8
#define MMISMS_DIR  ""
#define MMISMS_DIR_LEN  0

//END
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
const wchar g_mmisms_default_export_dir_str[] = { 'S', 'M', 'S', 0 };

/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                          EXTERN DECLARE                                *
**--------------------------------------------------------------------------*/
extern MMISMS_GLOBAL_T g_mmisms_global;   // SMS的全局变量

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

LOCAL uint32 s_mmisms_export_total_num = 0;//
LOCAL MMIFILE_HANDLE s_export_file_handle = 0;//
LOCAL wchar *s_full_name_ptr = PNULL;//
LOCAL  BOOLEAN s_mmisms_export_securitybox = FALSE;
LOCAL uint32 s_mmisms_export_current_num = 0;//实时获取当前删除的短信条目
LOCAL MMISMS_BOX_TYPE_E s_export_type = MMISMS_BOX_NONE;//导出信息所处信箱
LOCAL  MMISMS_BOX_TYPE_E s_mmisms_cur_boxtype = MMISMS_BOX_NONE;
LOCAL wchar s_sms_default_cr_lf[] = {0x0d, 0x0a, 0};

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : write the SMS box title
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WriteExportFileBoxType(BOOLEAN is_box_head);

/*****************************************************************************/
//  Description : set export file content
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetExportFileContent(MMI_STRING_T  *string_ptr);

/*****************************************************************************/
//  Description : Get transmitter display string
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFileTransmitterDispString(
                                           MN_SMS_ALPHABET_TYPE_E    alphabet,            //IN:
                                           MN_CALLED_NUMBER_T        *orginal_addr_ptr,    //IN:
                                           MMI_STRING_T            *string_ptr            //OUT:
                                           );

/**---------------------------------------------------------------------------*
**                         Fuction DEFINITION
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : write the SMS box title
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WriteExportFileBoxType(BOOLEAN is_box_head)
{
    MMI_STRING_T file_box_type = {0};
    MMI_TEXT_ID_T text_id = TXT_NULL;
    wchar *file_box_ptr = PNULL;
    uint8 cr_len = MMIAPICOM_Wstrlen(s_sms_default_cr_lf);    
    uint8 len = 0;
    uint8 *  file_box_text = PNULL;
    MMIFILE_HANDLE file_handle = PNULL;
    uint32 written = 0;
    BOOLEAN ret = FALSE;
    // MMI_IMAGE_ID_T  image_id = IMAGE_PUBWIN_SUCCESS;
    MMIFILE_ERROR_E     error = SFS_NO_ERROR;
    MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_NONE;
    uint32 type_write_len = 0;
    uint8 edcode_type[] = {0xEF, 0xBB, 0xBF, 0};

    if(is_box_head)
    {
       box_type = MMISMS_GetExportBoxType();
    }
    else
    {
       box_type = MMISMS_GetExportCurBoxType();
    }

    //SCI_TRACE_LOW:"WriteExportFileBoxType box_type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2645_112_2_18_2_52_44_77,(uint8*)"d",box_type);

    //未获得导出信箱位置
    if(MMISMS_BOX_NONE == box_type)
    {
       return FALSE;
    }
    //设置短信文件
    else
    {
       switch(box_type)
       {
       case MMISMS_BOX_MT:
           text_id = TXT_SMS_EXPORT_INBOX;
           break;
       
       case MMISMS_BOX_SENDFAIL:
           text_id = TXT_SMS_EXPORT_OUTBOX;
           break;
       
       case MMISMS_BOX_NOSEND:
           text_id = TXT_SMS_EXPORT_DRAFTBOX;
           break;    
       
       case MMISMS_BOX_SENDSUCC:
           text_id = TXT_SMS_EXPORT_SENTBOX;
           break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
       case MMISMS_BOX_SECURITY:
           text_id = TXT_SMS_EXPORT_SECURITYBOX;
           break;
#endif

       case MMISMS_BOX_ALL:
           text_id = TXT_SMS_EXPORT_ALLBOX;
           break;

#ifdef CHATMODE_ONLY_SUPPORT
        case MMISMS_BOX_CHAT:
            text_id = TXT_SMS_EXPORT_CHATBOX;
            break;
#endif

        default:
           break;
       }
       MMI_GetLabelTextByLang(text_id, &file_box_type);
   
       file_box_ptr = SCI_ALLOCA(sizeof(wchar) * (cr_len + file_box_type.wstr_len + cr_len + 1));
   
       if(PNULL == file_box_ptr)
       {
           return FALSE;
       }
       SCI_MEMSET(file_box_ptr,0x00,(sizeof(wchar) * (cr_len + file_box_type.wstr_len + cr_len + 1)));
   
       MMI_WSTRNCPY(
           file_box_ptr,
           cr_len,
           s_sms_default_cr_lf,
           cr_len,
           cr_len
           ); 
   
       len = file_box_type.wstr_len;
   
       MMI_WSTRNCPY(
           &file_box_ptr[cr_len],
           len,
           file_box_type.wstr_ptr,
           len,
           len
           ); 
   
       len = len + cr_len;
   
       MMI_WSTRNCPY(
           &file_box_ptr[len],
           len,
           s_sms_default_cr_lf,
           cr_len,
           cr_len
           ); 

       len = len + cr_len;
   
       file_box_text = (uint8 *)SCI_ALLOCA(len*3+1);

       if(PNULL == file_box_text)
       {
           SCI_FREE(file_box_ptr);
           file_box_ptr = PNULL;        
           return FALSE;
       }
       SCI_MEMSET(file_box_text,0,(len*3+1));

       GUI_WstrToUTF8( file_box_text, len*3+1, file_box_ptr, len );
   
       file_handle = MMISMS_GetExportFileHandle();
       if (PNULL != file_handle)
       {
           error = MMIAPIFMM_WriteFile(file_handle, edcode_type,
               strlen((char *)edcode_type), &type_write_len, PNULL);
            error = MMIAPIFMM_WriteFile(file_handle, file_box_text,
               strlen((char *)file_box_text), &written, PNULL);
            //SCI_TRACE_LOW:"WriteExportFileBoxType error is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2750_112_2_18_2_52_44_78,(uint8*)"d",error);
            if (SFS_NO_ERROR == error)
            {
               ret = TRUE;
            }
            else if(SFS_ERROR_NO_SPACE == error)
            {
                MMIAPIFMM_CloseFile(file_handle);
                MMIPUB_OpenAlertWarningWin( TXT_NO_SPACE);
            }            
            else
            {
                //写文件失败
                MMIAPIFMM_CloseFile(file_handle);
                MMIAPIFMM_DeleteFile(s_full_name_ptr, PNULL);


                //显示结果
                MMIPUB_OpenAlertWarningWin( TXT_ERROR);
           }
       }
       else
       {
			//打开文件失败
            MMIPUB_OpenAlertWarningWin( TXT_ERROR);
       }
   
       SCI_FREE(file_box_ptr);
       file_box_ptr = PNULL;        
   
       SCI_FREE(file_box_text);
       file_box_text = PNULL;
    }
    
    return ret;
}

/*****************************************************************************/
//  Description : set export file content
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetExportFileContent(MMI_STRING_T  *string_ptr)
{
    MMISMS_READ_MSG_T    *read_msg_ptr = &(g_mmisms_global.read_msg);
    MN_SMS_ALPHABET_TYPE_E  alphabet = MN_SMS_DEFAULT_ALPHABET;
    uint16 msg_len = 0;
    uint8 *msg_data_ptr = PNULL;
    // BOOLEAN find_result = FALSE;
    MMI_STRING_T num_name = {0};
    uint16 copy_offset = 0;
    uint16 convert_msg_len = 0;
    uint8 time_len = 0;
    wchar time[MMISMS_FILE_TIME_LEN] = {0};
    uint8 cr_len = MMIAPICOM_Wstrlen(s_sms_default_cr_lf);
    uint16 year = 0;
    uint8 year_len = 0;
    uint8 year_temp[6] = {0};

    if(PNULL == string_ptr)
    {
        return FALSE;
    }
        
    alphabet = read_msg_ptr->read_content.alphabet;
    msg_len = read_msg_ptr->read_content.length;
    msg_data_ptr = read_msg_ptr->read_content.data;
    
    num_name.wstr_ptr = SCI_ALLOCA((MMISMS_PBNAME_MAX_LEN + MMISMS_PBNAME_MAX_LEN + 2 + 2 + 1) *sizeof(wchar));
    if(PNULL == num_name.wstr_ptr)
    {
        return FALSE;
    }
    SCI_MEMSET((uint8 *)num_name.wstr_ptr, 0, ((MMISMS_PBNAME_MAX_LEN + MMISMS_PBNAME_MAX_LEN + 2 + 2 + 1) *sizeof(wchar)));
    
    //号码显示
    //PB中未找到匹配项,只显示号码
    //PB中找到匹配项，显示 姓名(号码)
    
    GetFileTransmitterDispString(
        alphabet,
        &(read_msg_ptr->address),
        &num_name
        );
    
    //设置年份
    if(MMISMS_READ_MTSMS == g_mmisms_global.read_msg.read_type)
    {
        year = read_msg_ptr->time_stamp.year + MMISMS_MT_OFFSERT_YEAR;
    }
    else
    {
        year = read_msg_ptr->time_stamp.year + MMISMS_OFFSERT_YEAR;;
    }
 
    year_len = sprintf(
        (char *)year_temp, 
        "%.4d/", 
        (uint16)year
        );
    time_len = year_len;
    
    MMI_STRNTOWSTR(
        time,
        time_len,
        (uint8 *)year_temp,
        time_len,
        time_len
        );
    
    //设置月，日，时间    
    time_len += MMISMS_GetTimeDispString(&(read_msg_ptr->time_stamp), &time[time_len]);
    
    // the length of SMS content will be too large to allocate memory, so we just calculate the length.
    if (alphabet != MN_SMS_UCS2_ALPHABET)
    {
        convert_msg_len = msg_len;
    }
    else
    {
        convert_msg_len = msg_len/sizeof(wchar);
    }
    
    // in first line, display address
    // in second line, display time
    // and then display the SMS content.
    
    // in order to allocate memory for string, calculate the string length.
    
    if (0 < time_len)
    {
        string_ptr->wstr_len = cr_len + num_name.wstr_len + cr_len + time_len + cr_len + convert_msg_len + cr_len;
    }
    else
    {
        string_ptr->wstr_len = cr_len + num_name.wstr_len + cr_len + convert_msg_len + cr_len;
    }
    
    // allocate memory
    string_ptr->wstr_ptr = SCI_ALLOCA((string_ptr->wstr_len+1)*(sizeof(wchar)));
    if(PNULL == string_ptr->wstr_ptr)
    {
        SCI_FREE(num_name.wstr_ptr);
        num_name.wstr_ptr = PNULL;
        return FALSE;
    }
    SCI_MEMSET((uint8 *)string_ptr->wstr_ptr, 0x00, ((string_ptr->wstr_len+1)*sizeof(wchar)));
    
    copy_offset = 0;
    
    // CR
    MMI_WSTRNCPY(
        &(string_ptr->wstr_ptr[copy_offset]),
        cr_len,
        s_sms_default_cr_lf,
        cr_len,
        cr_len
        );
    copy_offset += cr_len;
    
    // address
    MMI_WSTRNCPY(
        &(string_ptr->wstr_ptr[copy_offset]),
        num_name.wstr_len,
        num_name.wstr_ptr,
        num_name.wstr_len,
        num_name.wstr_len
        );
    
    copy_offset += num_name.wstr_len;
    // CR
    MMI_WSTRNCPY(
        &(string_ptr->wstr_ptr[copy_offset]),
        cr_len,
        s_sms_default_cr_lf,
        cr_len,
        cr_len
        );
    
    copy_offset += cr_len;
    
    if (0 < time_len)
    {
        // time
        MMI_WSTRNCPY(
            &(string_ptr->wstr_ptr[copy_offset]),
            time_len,
            time,
            time_len,
            time_len
            );
        
        copy_offset += time_len;
        // CR
        MMI_WSTRNCPY(
            &(string_ptr->wstr_ptr[copy_offset]),
            cr_len,
            s_sms_default_cr_lf,
            cr_len,
            cr_len
            );
        copy_offset += cr_len;
    }
    
    // message content
    if (convert_msg_len != msg_len)
    {
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
        MMI_MEMCPY( (uint8 *)&(string_ptr->wstr_ptr[copy_offset]),
            convert_msg_len*sizeof(wchar),
            msg_data_ptr,
            convert_msg_len*sizeof(wchar),
            convert_msg_len*sizeof(wchar)
            );
#else
        GUI_UCS2L2UCS2B((uint8 *)&(string_ptr->wstr_ptr[copy_offset]),
            convert_msg_len * sizeof(wchar),
            msg_data_ptr,
            convert_msg_len * sizeof(wchar));
#endif
    }
    else
    {
        if (MN_SMS_DEFAULT_ALPHABET == alphabet)
        {
            MMIAPICOM_Default2Wchar((uint8 *)msg_data_ptr,
                                    &(string_ptr->wstr_ptr[copy_offset]),
                                    convert_msg_len
                                   );
        } 
        else
        {
            MMI_STRNTOWSTR(
                &(string_ptr->wstr_ptr[copy_offset]),
                convert_msg_len,
                (uint8 *)msg_data_ptr,
                convert_msg_len,
                convert_msg_len
            );
        }
        
    }
    
    copy_offset += convert_msg_len; 
    
    // CR
    MMI_WSTRNCPY(
        &(string_ptr->wstr_ptr[copy_offset]),
        cr_len,
        s_sms_default_cr_lf,
        cr_len,
        cr_len
        );
    copy_offset += cr_len;    
    
    //SCI_TRACE_LOW:"MMISMS: SetExportFileContent length = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2501_112_2_18_2_52_44_73,(uint8*)"d", string_ptr->wstr_len);
    
    SCI_FREE(num_name.wstr_ptr);
    num_name.wstr_ptr = PNULL;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : Get transmitter display string
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFileTransmitterDispString(
                                           MN_SMS_ALPHABET_TYPE_E    alphabet,            //IN:
                                           MN_CALLED_NUMBER_T        *orginal_addr_ptr,    //IN:
                                           MMI_STRING_T            *string_ptr            //OUT:
                                           )
{
    uint8 tele_num[MMISMS_PBNAME_MAX_LEN + 2] = {0};
    uint16 num_len = 0;
    MMI_STRING_T num_name = {0};
    BOOLEAN find_result = FALSE;
    
     if (PNULL == orginal_addr_ptr || PNULL == string_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:GetFileTransmitterDispString orginal_addr_ptr string_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2526_112_2_18_2_52_44_74,(uint8*)"");
        return FALSE;
    }
    //存在号码   
    if (orginal_addr_ptr->num_len > 0)
    {
        num_name.wstr_len  = 0;
        num_name.wstr_ptr = SCI_ALLOCAZ((MMISMS_PBNAME_MAX_LEN + MMISMS_PBNAME_MAX_LEN + 2 + 2 + 1) *sizeof(wchar));
        if(PNULL == num_name.wstr_ptr)
        {
            //SCI_TRACE_LOW:"GetFileTransmitterDispString num_name.wstr_ptr alloc fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2536_112_2_18_2_52_44_75,(uint8*)"");
            return find_result;
        }
        //SCI_MEMSET(num_name.wstr_ptr,0x00,((MMISMS_PBNAME_MAX_LEN + MMISMS_PBNAME_MAX_LEN + 2 + 2 + 1) *sizeof(wchar)));
        
        if(PNULL == num_name.wstr_ptr)
        {
            return FALSE;
        }
        //indicate the phonebook info of the address is not saved.
        //SCI_TRACE_LOW:"MMISMS: GetFileTransmitterDispString number_type = %d, number_plan = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2547_112_2_18_2_52_44_76,(uint8*)"dd",orginal_addr_ptr->number_type, orginal_addr_ptr->number_plan);
        
        num_len = MMIAPICOM_GenNetDispNumber((MN_NUMBER_TYPE_E)(orginal_addr_ptr->number_type),
            (uint8)MIN((MMISMS_PBNUM_MAX_LEN >> 1),
            orginal_addr_ptr->num_len),
            orginal_addr_ptr->party_num,
            tele_num,
            (uint8)(MMISMS_PBNUM_MAX_LEN + 2)
            );
        
        find_result = MMISMS_GetNameByNumberFromPB(tele_num,&num_name, MMISMS_PBNAME_MAX_LEN);    
        
        //未找到PB匹配项，只显示号码        
        if ((!find_result) 
            || (0 == num_name.wstr_len))
        {
            MMI_STRNTOWSTR(num_name.wstr_ptr,
                MMISMS_PBNAME_MAX_LEN,
                (uint8 *)tele_num,
                MMISMS_PBNAME_MAX_LEN,
                strlen((char *)tele_num));
            
            num_name.wstr_len = strlen((char *)tele_num);
        }
        //找到PB匹配项，显示姓名(号码)
        else
        {
            //左括号
            num_name.wstr_ptr[num_name.wstr_len] = MMISMS_LEFT_BRACKET;
            num_name.wstr_len++;
            
            //号码            
            MMI_STRNTOWSTR( &num_name.wstr_ptr[num_name.wstr_len],
                MMISMS_PBNAME_MAX_LEN,
                (uint8 *)tele_num,
                MMISMS_PBNAME_MAX_LEN,
                strlen((char *)tele_num));    
            num_name.wstr_len += strlen((char *)tele_num);
            
            //右括号
            num_name.wstr_ptr[num_name.wstr_len] = MMISMS_RIGHT_BRACKET;
            num_name.wstr_len++;
        }
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_NO_NAME, &num_name);
    }
    
    string_ptr->wstr_len  = num_name.wstr_len;
    
    MMI_WSTRNCPY(
        string_ptr->wstr_ptr,
        (MMISMS_PBNAME_MAX_LEN + MMISMS_PBNAME_MAX_LEN + 2 + 2 + 1) *sizeof(wchar),
        num_name.wstr_ptr,
        (MMISMS_PBNAME_MAX_LEN + MMISMS_PBNAME_MAX_LEN + 2 + 2 + 1) *sizeof(wchar),
        string_ptr->wstr_len
        );
    
    if((PNULL != num_name.wstr_ptr) && (orginal_addr_ptr->num_len > 0))
    {
        SCI_FREE(num_name.wstr_ptr);
        num_name.wstr_ptr = PNULL;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : get device name
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC wchar *MMISMS_ExportFile_GetDeviceName(uint16 *device_name_len)
{
    MMIFILE_DEVICE_E file_type = MMI_DEVICE_SDCARD;
    
    *device_name_len = 0;
    
    file_type  = MMIAPIFMM_GetValidateDevice(file_type);

    if (MMI_DEVICE_NUM != file_type)
    {
        *device_name_len = MMIAPIFMM_GetDevicePathLen((file_type));
        return MMIAPIFMM_GetDevicePath((file_type));
    }
    else
    {
        return PNULL;
    }
}
#

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: rong.gu
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_ExportItem(MMISMS_ORDER_ID_T order_id,MMI_CTRL_ID_T ctrl_id,MMISMS_BOX_TYPE_E box_type,uint32* mark_num)
{
    uint32 export_num = 0;

    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return;
    }
    MMISMS_EnterSetExportPathWin();   
    MMISMS_SetExportBoxType(box_type);
    if (0 == *mark_num)
    {
        (*mark_num)++;
        MMISMS_SetCurOrderMarked(order_id,TRUE);
    }
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    if(MMISMS_BOX_SECURITY == box_type)
    {     
        MMISMS_SetExportSecurityBoxFlag(TRUE);    
    }
#endif
    export_num = MMISMS_GetExportMarkedMsgNum(box_type);
    MMISMS_SetExportTotalNum(export_num);
}

/*****************************************************************************/
//  Description : set export sms total num
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetExportTotalNum(uint32 total_num)
{
    //SCI_TRACE_LOW:"MMISMS_SetExportTotalNum: total_num is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2793_112_2_18_2_52_44_79,(uint8*)"d",total_num);
    s_mmisms_export_total_num = total_num;
}

/*****************************************************************************/
//  Description : get export sms total num
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetExportTotalNum(void)
{
    //SCI_TRACE_LOW:"MMISMS_GetExportTotalNum: s_mmisms_export_total_num is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2803_112_2_18_2_52_44_80,(uint8*)"d",s_mmisms_export_total_num);
    return s_mmisms_export_total_num;
}


LOCAL BOOLEAN ExportReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data)
{
    if(MMK_IsOpenWin(MMISMS_EXPORT_ANIM_WIN_ID))
    {
        //write successfully
        if(MMISMS_ExportMsgToFile())
        {
            MMK_PostMsg(MMISMS_EXPORT_ANIM_WIN_ID, MMISMS_MSG_EXPORT_NEXT, PNULL,0);
        }
        //write error
        else
        {
            MMK_CloseWin(MMISMS_EXPORT_ANIM_WIN_ID);
        }
    }
	
	return TRUE;

}


/*****************************************************************************/
//  Description : to export the SMS box called by window
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AppExportMsgBox(
                                                MMISMS_BOX_TYPE_E    type,    //IN:
                                                uint16* msg_type,  //out
                                                MMISMS_FOLDER_TYPE_E folder_type
                                                )
{
    MMISMS_OPER_ERR_E ret_val = MMISMS_EMPTY_BOX;
    // BOOLEAN is_right = FALSE;
    MMISMS_ORDER_ID_T first_pos = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    int32 i = 0;
    BOOLEAN is_need_security = MMISMS_GetExportSecurityBoxFlag();
    
    g_mmisms_global.operation.cur_order_index = 0;
    SCI_MEMSET(g_mmisms_global.operation.cur_order_id_arr, 0, 
        sizeof(g_mmisms_global.operation.cur_order_id_arr)); 
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_AppExportMsgBox type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_1875_112_2_18_2_52_42_63,(uint8*)"d", type);
    
    MMISMS_SetOperStatus(MMISMS_EXPORT_SMS);
    
    switch (type)        
    {
    case MMISMS_BOX_SENDSUCC://已发信箱
        first_pos = MMISMS_FindValidSecurityMoSMSForDelAll(type);
        break;
        
    case MMISMS_BOX_SENDFAIL://发件箱
        first_pos = MMISMS_FindValidSecurityMoSMSForDelAll(type);
        break;
        
    case MMISMS_BOX_NOSEND://草稿箱
        first_pos = MMISMS_FindValidSecurityMoSMSForDelAll(type);
        break;
        
    case MMISMS_BOX_MT://收件箱
        first_pos = MMISMS_FindValidSecurityMTSMSForDelAll();
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY://安全件箱
        first_pos = MMISMS_FindValidSecuritySMSForDelAll();
        break;        
#endif        
    case MMISMS_BOX_ALL:
        first_pos = MMISMS_FindValidSMSForExportAll(type,is_need_security);
        break;

#ifdef MMI_SMS_CHAT_SUPPORT    
    case MMISMS_BOX_CHAT:
        first_pos = MMISMS_FindValidChatSMSForDelAll();
        break;
#endif

    default:
        break;
    }
    
    // the index is valid
    if (PNULL != first_pos)
    {
        //设置当前order_id到全局变量数组
        if (first_pos->flag.is_concatenate_sms) //is long sms
        {
            cur_order_id = first_pos;
            while (cur_order_id != PNULL)
            {
                MMISMS_SetCurOrderId(i,cur_order_id); 
                cur_order_id  = cur_order_id->next_long_sms_ptr;
                i++;
            }
        }
        else //is normal sms
        {
            MMISMS_SetCurOrderId(0,first_pos); 
        } 
        
        first_pos->flag.is_marked = FALSE;// 找到后将该标记清除，避免反复找到同一条信息
        *msg_type = first_pos->flag.msg_type;
        //SCI_TRACE_LOW:"MMISMS_AppExportMsgBox: msg_type is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_1936_112_2_18_2_52_43_64,(uint8*)"d",first_pos->flag.msg_type);
        switch (first_pos->flag.msg_type)
        {
        case MMISMS_TYPE_SMS:
            //清空变量
            MMISMSAPP_ReadExportSms(first_pos,ExportReadSmsCallback,PNULL,PNULL);
#if 0            
            MMISMS_ReadyReadSms();
            MNSMS_ReadSmsEx(
                (MN_DUAL_SYS_E)first_pos->flag.dual_sys, 
                (MN_SMS_STORAGE_E)first_pos->flag.storage, 
                first_pos->record_id 
                );     
#endif                
            ret_val  = MMISMS_NO_ERR;
            break;
            
        default: 
            MMK_PostMsg(MMISMS_EXPORT_ANIM_WIN_ID, MMISMS_MSG_EXPORT_NEXT, PNULL,0);
            ret_val = MMISMS_NO_ERR;            
            break;            
        }        
    }
    else
    {
        //SCI_TRACE_LOW:"MMISMS: s_export_file_handle: The SmsBox is empty!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_1958_112_2_18_2_52_43_65,(uint8*)"");
        ret_val = MMISMS_FINISH_OPER;
    }
    
    return (ret_val);
}

/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetDefaultExportPath(
                                           wchar      *full_path_ptr,     //out
                                           uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                           )
{
    wchar* device_name_ptr = PNULL;
    uint16 device_name_len = 0;
    const wchar *storage_dir_ptr = PNULL;
    uint16 storage_dir_len = 0;
    BOOLEAN ret = TRUE;
    
    if (PNULL == full_path_ptr || PNULL == full_path_len_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetDefaultExportPath full_path_ptr full_path_len_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_1639_112_2_18_2_52_42_58,(uint8*)"");
        return FALSE;
    }    
    device_name_ptr = MMISMS_ExportFile_GetDeviceName(&device_name_len);
    
    if (PNULL == device_name_ptr || 0 == device_name_len)
    {
        return FALSE;
    }
    
    storage_dir_ptr = MMISMS_DEFAULT_EXPORT_DIR;
    storage_dir_len = MMISMS_DEFAULT_EXPORT_DIR_LEN;
    
    if (!MMIAPIFMM_CombineFullPath(device_name_ptr, device_name_len,
        (wchar*)MMISMS_DIR, MMISMS_DIR_LEN,
        storage_dir_ptr, storage_dir_len,
        full_path_ptr, full_path_len_ptr))
    {
        ret = FALSE;
    }
    
    return ret;
}


/*****************************************************************************/
//  Description : to create SMS file
//  Global resource dependence : g_mmisms_global
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_CreateExportMsgFile(wchar *s_dsp_path_str)
{
    uint32 access_mode = 0;
    uint16  full_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMI_STRING_T file_name_value_string = {0};
    uint8 time_len = 0;
    wchar file_name[MMISMS_FILE_TIME_LEN] = {0};
    SCI_TM_T file_time = {0};
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};
    uint8  str_str[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    MMI_STRING_T dsp_path_str = {0};
    MMI_STRING_T path_str = {0};
    MMIFILE_DEVICE_E dev = MMI_DEVICE_NUM;
    
    //获取路径    
    path_str.wstr_ptr = path;
    path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
    
    
    #ifndef MMI_GUI_STYLE_TYPICAL
    dsp_path_str.wstr_ptr = s_dsp_path_str;
    dsp_path_str.wstr_len = MMIAPICOM_Wstrlen(s_dsp_path_str);
    #else
    GUILABEL_GetText(MMISMS_SETTING_EXPORT_SAVEPOS_CTRL_ID, &dsp_path_str);
    #endif
    MMISMS_GetRealPathText(&dsp_path_str, path_str.wstr_ptr, &path_str.wstr_len);

    dev = MMIAPIFMM_GetDeviceTypeByPath(path_str.wstr_ptr, MMIFILE_DEVICE_LEN);
    if(MMI_DEVICE_NUM == dev)
    {
        return FALSE;
    }
    
    if(!MMIAPIFMM_IsDevEnoughSpace(MMISMS_FILE_MIN_SIZE, dev))
    {
        MMIPUB_OpenAlertWarningWin( TXT_NO_SPACE);
        return FALSE;
    }
    else
    {
        //用系统时间作文件名
        TM_GetSysDate(&sys_date);
        TM_GetSysTime(&sys_time);  
        file_time.tm_year=sys_date.year;
        file_time.tm_mday=sys_date.mday;   
        file_time.tm_mon=sys_date.mon;
        
        file_time.tm_hour=sys_time.hour;
        file_time.tm_min=sys_time.min;
        file_time.tm_sec = sys_time.sec;
        
        //生成文件名   
        sprintf((char*)str_str, "%04d%02d%02d_%02d%02d%02d",
            file_time.tm_year,file_time.tm_mon,\
            file_time.tm_mday, file_time.tm_hour,\
            file_time.tm_min, file_time.tm_sec);
        
        time_len = SCI_STRLEN((char *)str_str);
        time_len = MIN(time_len,MMISMS_FILE_TIME_LEN);
        
        MMI_STRNTOWSTR(
            file_name,
            time_len,
            (uint8 *)str_str,
            time_len,
            time_len
            );
        
        file_name_value_string.wstr_ptr = file_name;
        file_name_value_string.wstr_len = MMIAPICOM_Wstrlen(file_name);
        if(PNULL == s_full_name_ptr)
        {
            s_full_name_ptr = SCI_ALLOCA((MMIFILE_FULL_PATH_MAX_LEN + 1) * sizeof(wchar));
            if(PNULL == s_full_name_ptr)
            {
                MMIPUB_OpenAlertWarningWin( TXT_ERROR);
                return FALSE;
            }
        }
        SCI_MEMSET(s_full_name_ptr,0x00,(MMIFILE_FULL_PATH_MAX_LEN + 1) * sizeof(wchar));
        
        //合成文件全路径
        if (MMISMS_CombineExportFile(
            &path_str,
            &file_name_value_string,
            s_full_name_ptr,
            &full_name_len,
            TRUE
            ))
        {
            s_export_file_handle = 0;
            access_mode = SFS_MODE_READ | SFS_MODE_WRITE | SFS_MODE_OPEN_ALWAYS | SFS_MODE_APPEND;/*lint !e655*/
            s_export_file_handle = MMIAPIFMM_CreateFile(s_full_name_ptr, access_mode, NULL, NULL);
            //SCI_TRACE_LOW:"CreateExportMsgFile s_export_file_handle is 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2069_112_2_18_2_52_43_66,(uint8*)"d",s_export_file_handle);
            if(0 != s_export_file_handle )//|| 0 != param_to_create_export_main_win->s_export_file_handle
            {
                if(!WriteExportFileBoxType(TRUE))
                {
                    return FALSE;
                }
            }
            else
            {
                //创建文件失败
                MMIPUB_OpenAlertWarningWin( TXT_ERROR);
                return FALSE;            
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin( TXT_COM_FILE_NAME_TOO_LONG);
            return FALSE;
        } 
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : to export the SMS to file
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_ExportMsgToFile(void)
{
    MMIFILE_HANDLE file_handle = 0;
    BOOLEAN ret = FALSE;
    MMI_TEXT_ID_T text_id = TXT_ERROR;
    MMI_IMAGE_ID_T  image_id = IMAGE_PUBWIN_SUCCESS;
    uint32 written = 0;
    MMIFILE_ERROR_E     error = SFS_NO_ERROR;
    MMI_STRING_T content_str = {0};
    uint8 *  file_text = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMISMS_BOX_TYPE_E       cur_box_type = MMISMS_BOX_NONE;
    
    SCI_MEMSET(&content_str, 0, sizeof (content_str));
    
    //get file content
    if(SetExportFileContent(&content_str))
    {
        file_text = (uint8 *)SCI_ALLOCA(content_str.wstr_len*3+1);
        if(PNULL == file_text)
        {
            if(PNULL != content_str.wstr_ptr)
            {
                SCI_FREE(content_str.wstr_ptr);
                content_str.wstr_ptr = PNULL;        
            }
            return FALSE;
        }
        SCI_MEMSET(file_text,0,(content_str.wstr_len*3+1));
        GUI_WstrToUTF8( file_text, content_str.wstr_len*3+1, content_str.wstr_ptr, content_str.wstr_len );
        
        file_handle = MMISMS_GetExportFileHandle();
        if (0 != file_handle)
        {
            //if export all msg,every box type should be added
            if(MMISMS_BOX_ALL == MMISMS_GetExportBoxType())
            {
                cur_order_id = MMISMS_GetCurOperationOrderId();
                cur_box_type = MMISMS_GetSmsStateBoxtype(cur_order_id);
                if(s_mmisms_cur_boxtype !=  cur_box_type)
                {
                    s_mmisms_cur_boxtype = cur_box_type;
                    if(!WriteExportFileBoxType(FALSE))
                    {
												SCI_FREE(content_str.wstr_ptr);
												content_str.wstr_ptr = PNULL; 
												
												SCI_FREE(file_text);
												file_text = PNULL; 
                        return FALSE;
                    }
                }
            }
            
            error = MMIAPIFMM_WriteFile(file_handle, file_text,strlen((char *)file_text), &written, PNULL);
            //SCI_TRACE_LOW:"MMISMS_ExportMsgToFile: error is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2153_112_2_18_2_52_43_67,(uint8*)"d",error);
            if (SFS_NO_ERROR == error)
            {
                s_mmisms_export_current_num++;
                //SCI_TRACE_LOW:"MMISMS_ExportMsgToFile: s_mmisms_export_current_num is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2157_112_2_18_2_52_43_68,(uint8*)"d",s_mmisms_export_current_num);
                ret = TRUE;
            }
            else if(SFS_ERROR_NO_SPACE == error)
            {
                MMIAPIFMM_CloseFile(file_handle);
                MMIPUB_OpenAlertWarningWin( TXT_NO_SPACE);
            }
            else
            {
                //write file error
                text_id  = TXT_ERROR;
                image_id = IMAGE_PUBWIN_WARNING;
                MMIAPIFMM_CloseFile(file_handle);
                MMIAPIFMM_DeleteFile(s_full_name_ptr, PNULL);
                //show the result
                MMIPUB_OpenAlertWinByTextId(PNULL, text_id, TXT_NULL, image_id, PNULL,
                    PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
        }
        else
        {
            //open file error
            text_id  = TXT_ERROR;
            image_id = IMAGE_PUBWIN_WARNING;
            //show the result
            MMIPUB_OpenAlertWinByTextId(PNULL, text_id, TXT_NULL, image_id, PNULL,
                PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }   
    }

    SCI_FREE(content_str.wstr_ptr);
    content_str.wstr_ptr = PNULL;  
    
    SCI_FREE(file_text);
    file_text = PNULL;
    
    return ret;
}

/*****************************************************************************/
//  Description : set export sms current num
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetExportCurrentNum(uint32 current_num)
{
    //SCI_TRACE_LOW:"MMISMS_SetExportCurrentNum: current_num is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2813_112_2_18_2_52_44_81,(uint8*)"d",current_num);
    s_mmisms_export_current_num = current_num;
}

/*****************************************************************************/
//  Description : get export sms current num
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetExportCurrentNum(void)
{
    //SCI_TRACE_LOW:"MMISMS_GetExportCurrentNum: s_mmisms_export_current_num is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2823_112_2_18_2_52_45_82,(uint8*)"d",s_mmisms_export_current_num);
    return s_mmisms_export_current_num;
}
/*****************************************************************************/
//  Description : set export sms current box type
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetExportCurBoxType(MMISMS_BOX_TYPE_E box_type)
{
    //SCI_TRACE_LOW:"MMISMS_SetExportCurBoxType: box_type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2833_112_2_18_2_52_45_83,(uint8*)"d",box_type);
    s_mmisms_cur_boxtype = box_type;
}

/*****************************************************************************/
//  Description : get export sms current box type
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_BOX_TYPE_E MMISMS_GetExportCurBoxType(void)
{
    //SCI_TRACE_LOW:"MMISMS_GetExportCurBoxType: s_mmisms_cur_boxtype is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2843_112_2_18_2_52_45_84,(uint8*)"d",s_mmisms_cur_boxtype);
    return s_mmisms_cur_boxtype;
}

/*****************************************************************************/
//  Description : get export file handle
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMISMS_GetExportFileHandle(void)
{
    //SCI_TRACE_LOW:"MMISMS_GetExportFileHandle: s_export_file_handle is 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2291_112_2_18_2_52_43_72,(uint8*)"d",s_export_file_handle);
    return s_export_file_handle;
}

/*****************************************************************************/
//  Description : set export sms from which box
//                  TRUE:        do export all SMS operation  
//                  FALSE:    without do export all SMS operation
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetExportBoxType(MMISMS_BOX_TYPE_E export_box_type)
{
    //SCI_TRACE_LOW:"MMISMS_SetExportBoxType: export_box_type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_24950_112_2_18_2_55_12_837,(uint8*)"d",export_box_type);
    s_export_type = export_box_type;
}

/*****************************************************************************/
//  Description : get export sms from which box
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_BOX_TYPE_E MMISMS_GetExportBoxType(void)
{
    //SCI_TRACE_LOW:"MMISMS_GetExportBoxType: s_export_type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_24960_112_2_18_2_55_12_838,(uint8*)"d",s_export_type);
    return s_export_type;
}

/*****************************************************************************/
//  Description : set export security sms flag
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetExportSecurityBoxFlag(BOOLEAN is_export)
{
    //SCI_TRACE_LOW:"MMISMS_SetExportSecurityBoxFlag: is_export is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2866_112_2_18_2_52_45_85,(uint8*)"d",is_export);
    s_mmisms_export_securitybox = is_export;
}

/*****************************************************************************/
//  Description : get export security sms flag
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetExportSecurityBoxFlag(void)
{
    //SCI_TRACE_LOW:"MMISMS_GetExportSecurityBoxFlag: s_mmisms_export_securitybox is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2876_112_2_18_2_52_45_86,(uint8*)"d",s_mmisms_export_securitybox);
    return s_mmisms_export_securitybox;
}

/*****************************************************************************/
//  Description : free export file ptr
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_FreeExportFilePtr(void)
{
    if(PNULL != s_full_name_ptr)
    {
        SCI_FREE(s_full_name_ptr);
        s_full_name_ptr = PNULL;
        //SCI_TRACE_LOW:"MMISMS_FreeExportFilePtr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2890_112_2_18_2_52_45_87,(uint8*)"");
    }
}

#endif

