/******************************************************************************
 ** File Name:      BT_log.c                                                     *
 ** Author:         Fei Zhang                                              *
 ** DATE:           07/04/2011                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/04/2010     fei.zhang        Add BT USB Log function       *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
//#include "chip_plf_export.h"
#include "cmddef.h"
#include "mmieng_gpstest_nmea_log.h"
#include "ref_param.h"
#include "mem_prod.h"
#include "ref_outport.h"
#include "sfs.h"
//#include "priority_system.h"

#include "mmk_timer.h"
#include "dal_time.h"
#include "mmi_common.h"
#include "mmicom_trace.h"
#include "mmieng_gpstest_nmea_log.h"
//#if 1

/**---------------------------------------------------------------------------*
 **                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         Compiler Flag                                                               *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/*-----------------------------SIO DEBUG MACOR-------------------------------*/

#define LOG_DEFAULT_MAX_SIZE (50*1024*1024) /*50M*/

#define  MAX_LOG_NAME_LEN   60


/*-----------------------------END--------------------------------------------*/

/**---------------------------------------------------------------------------*
 **  Static Variables & functions
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **  external Variables
 **---------------------------------------------------------------------------*/
LOCAL uint32 LOG_saved_size[LOG_SAVE_TYPE_MAX] = {0}, LOG_stepup_size = LOG_DEFAULT_MAX_SIZE;

LOCAL SFS_HANDLE  sfs_nmea_log[LOG_SAVE_TYPE_MAX] = {NULL};
LOCAL uint8 Log_FileName[LOG_SAVE_TYPE_MAX][SFS_MAX_PATH+1] ={0};
LOCAL uint16 Log_FileFullName[LOG_SAVE_TYPE_MAX][SFS_MAX_PATH+1] ={0};
/*****************************************************************************/
// Description: Check BT log frame buffer is full or not
// Global resource dependence: s_sio_bt_frame_ctrl
// Author: fei.zhang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN _ConvertLogFileName(MMI_STRING_T LogName_Header, uint8 *Log_FileName)
{
     // 日期和时间
    wchar  date_time[MAX_LOG_NAME_LEN +1]    = {0};
    wchar  time_wstr[MAX_LOG_NAME_LEN +1]    = {0};
    uint8  time_str[MAX_LOG_NAME_LEN+1]      = {0};
    uint8  date_str[MAX_LOG_NAME_LEN+1]      = {0};
    wchar  log_filename[MAX_LOG_NAME_LEN+1]  = {0};

    SCI_DATE_T    sys_date = {0};
    SCI_TIME_T    sys_time = {0};
    SCI_TRACE_LOW("[GPS]: enter _ConvertLogFileName, LogName_Header:%ls\n", LogName_Header);

    if((NULL == Log_FileName)||(NULL ==LogName_Header.wstr_ptr)){
        SCI_TRACE_LOW("[GPS]: MMIENG_ConvertLogFileName, input parm is null\n");
        return FALSE;
    }

    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);   
    
      // 日期
    sprintf((char*) date_str, "%04d%02d%02d", sys_date.year, sys_date.mon, sys_date.mday); 
    MMIAPICOM_StrToWstr(date_str, date_time);
    
    // 时间
    sprintf((char*)time_str,"%02d%02d%02d",sys_time.hour,sys_time.min,sys_time.sec); 
    MMIAPICOM_StrToWstr(time_str, time_wstr);
    MMIAPICOM_Wstrcat(date_time, time_wstr);

    /* Combine file name something */
    MMIAPICOM_Wstrncat(log_filename, LogName_Header.wstr_ptr,LogName_Header.wstr_len);
    MMIAPICOM_Wstrcat(log_filename, L"_");
    MMIAPICOM_Wstrcat(log_filename, date_time);
    SCI_TRACE_LOW("[GPS]: enter _ConvertLogFileName, finish convert, file name:%ls\n",log_filename);

    MMIAPICOM_WstrToStr(log_filename,Log_FileName);
    SCI_TRACE_LOW("[GPS]: enter _ConvertLogFileName, finish convert, file name:%s\n",Log_FileName);

    return TRUE;

}


LOCAL uint16 _gb2ucs(					//return the number of UCS2 code that have been converted.
						uint16 *ucs2_ptr, 	//destination: string pointer to UCS2 string.
						const uint8 *gb_ptr,//source: string pointer to GB string
						uint16 len			//the length to convert from GB to UCS2.
						)
{
    uint16 tmp_gb, count = 0;

    SCI_ASSERT(PNULL != gb_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != ucs2_ptr); /*assert verified*/

    while((len > 0) && (len < 0xfff))
    {
        len--;
        count += 2;
        tmp_gb = (uint16)(*gb_ptr++);

        if (tmp_gb < 0x80)	//An ASCII
        {
            *ucs2_ptr++ = tmp_gb;
        }
    }

    return count;
}

LOCAL BOOLEAN _CreatFile(
                SFS_HANDLE*     fd_ptr,
                uint16*         filename,
                uint16*         device_name
                )
{
    SFS_ERROR_E device_result = SFS_ERROR_NO_SPACE;
    BOOLEAN return_result = FALSE;

    device_result = SFS_GetDeviceStatus(device_name);
    SCI_TRACE_LOW("[GPS]: enter _CreatLOGFile, file name :%ls\n", filename);

    if(SFS_ERROR_NONE == device_result)
    {
        *fd_ptr = SFS_CreateFile(filename, SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, NULL, NULL);/*lint !e655*/

        if(NULL != *fd_ptr)
        {
            SCI_TRACE_LOW("[GPS]: create file success!");
            return_result = TRUE;
        }
        else
        {
            SCI_TRACE_LOW("[GPS]: create file fail!");
            return_result = FALSE;
        }
    }
    else
    {
        SCI_TRACE_LOW("[GPS]: device status(%d) error!", device_result);
        return_result = FALSE;
    }

    return return_result;
}

LOCAL BOOLEAN _CreatLOGFile( uint8 *log_filename, LOG_SAVE_TYPE log_type)
{
    BOOLEAN        ret_create_file;
    char        log_file_name_gb[SFS_MAX_PATH+1] ={0};
    uint16      log_file_name[SFS_MAX_PATH+1] ={0};
    uint8   temp_logName[SFS_MAX_PATH+1] = {0};
    uint16      log_file_name_len = 0;
    uint16      tcard_name[2] = {'E', 0};    
    MMI_STRING_T file_name = {0};

    if(NULL == log_filename)
    {
    	SCI_TRACE_LOW("[GPS]: _CreatLOGFile,logfile name is null !!\n"); 

        return FALSE;
    }
    
    if (LOG_SAVE_TYPE_MAX <= log_type)
    {
        TRACE_APP_ENGINEERMODE("[GPS]:log_type invalid :%d",log_type);
        return FALSE;
    }
    
    SCI_TRACE_LOW("[GPS]: enter _CreatLOGFile, file name:%s\n",log_filename); 
    
    file_name.wstr_ptr = SCI_ALLOC(sizeof(wchar)*MAX_LOG_NAME_LEN + 1);
    SCI_MEMSET( file_name.wstr_ptr, 0, sizeof(file_name.wstr_ptr));
    MMIAPICOM_StrToWstr(log_filename, file_name.wstr_ptr);
    file_name.wstr_len = MMIAPICOM_Wstrlen(file_name.wstr_ptr);

    //SCI_TRACE_LOW("[GPS]: enter _CreatLOGFile, start convert file name, name: %s\n",file_name.wstr_ptr ); 
    _ConvertLogFileName(file_name,temp_logName);

    log_file_name_len = sprintf(log_file_name_gb,"E:\\%s.log",temp_logName);
    //_gb2ucs(log_file_name,  (uint8 *) log_file_name_gb, log_file_name_len);
    GUI_GB2UCS(log_file_name,  (uint8 *) log_file_name_gb, log_file_name_len);
   // SCI_TRACE_LOW("[GPS]: start to save LOG,logfile name is %s \n", log_file_name_gb);

    SCI_MEMCPY(&(Log_FileFullName[log_type]), log_file_name, (uint32)SCI_STRLEN(log_file_name));

    /* create new log file in sfs, original file */
    ret_create_file = _CreatFile(&(sfs_nmea_log[log_type]),log_file_name,tcard_name);

    if(FALSE == ret_create_file)
    {
        SCI_TRACE_LOW("[GPS]: create file %s fail, finish_time %d \n",log_file_name, SCI_GetTickCount());
        return FALSE;
    }

    SCI_TRACE_LOW("[GPS]: create file suc, finish_time %d \n", SCI_GetTickCount());
    return TRUE;
}
/* step:50M, LOGfile_num++, and create file  */
LOCAL BOOLEAN _WriteFile_Stepupfile(LOG_SAVE_TYPE log_type)
{
    BOOLEAN ret;
    if (LOG_SAVE_TYPE_MAX <= log_type)
    {
        TRACE_APP_ENGINEERMODE("[GPS]:log_type invalid :%d",log_type);
        return FALSE;
    }

    SFS_CloseFile( sfs_nmea_log[log_type]);

    //   LOGfile_num++;
    ret = _CreatLOGFile(&(Log_FileName[log_type]),log_type);

    if(ret)
    {
        // REFPARAM_SetBtCardLogNum(BTLOGfile_num);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

LOCAL BOOLEAN _WriteFile_DelFile(LOG_SAVE_TYPE log_type)
{
    SFS_ERROR_E     file_ret = SFS_ERROR_NONE;
    uint16      file_name_len = 0;

    if (LOG_SAVE_TYPE_MAX <= log_type)
    {
        TRACE_APP_ENGINEERMODE("[GPS]:log_type invalid :%d",log_type);
        return FALSE;
    }
    
    /* Close log file in sfs */
    SFS_CloseFile( sfs_nmea_log[log_type]);

    file_ret = SFS_DeleteFile( Log_FileFullName[log_type],PNULL );
    if( SFS_ERROR_NONE == file_ret )
    {
        SCI_TRACE_LOW("[GPS]: _WriteFile_DelFile, del(%s) ok",Log_FileFullName[log_type]);
        return TRUE;
    }

    //SCI_TRACE_LOW("[GPS]: _WriteFile_DelFile, del file fail,file name:%s",BTLOGfile_num);
    return FALSE;
}


PUBLIC uint16 MMIENG_WriteLOGFile(
                uint8*              write_buff_ptr,
                uint32              write_length,
                LOG_SAVE_TYPE       log_type
                )
{
    SFS_ERROR_E         result = SFS_ERROR_NO_SPACE;
    uint32              wrtlen;
    BOOLEAN          return_result = NMEA_LOG_FAIL;

    if (LOG_SAVE_TYPE_MAX <= log_type)
    {
        TRACE_APP_ENGINEERMODE("[GPS]:log_type invalid :%d",log_type);
        return FALSE;
    }

    result = SFS_WriteFile(sfs_nmea_log[log_type], (void*)write_buff_ptr, write_length, &wrtlen, PNULL);

    switch( result )
    {
        case SFS_ERROR_NONE:/*0:success*/
        {
            SCI_TRACE_LOW("[GPS]:   SFS_FlushFile");
            SFS_FlushFile(sfs_nmea_log[log_type], NULL);

            LOG_saved_size[log_type] += write_length;

            if( LOG_saved_size[log_type] >= LOG_stepup_size )
            {
                SCI_TRACE_LOW("[GPS]: stepupfile, LOG_saved_size(%d),LOG_stepup_size(%d)",LOG_saved_size[log_type],LOG_stepup_size);/* cr208045 */
                /* step of save size is 50M */
                LOG_saved_size[log_type] = 0;
                return_result  = _WriteFile_Stepupfile(log_type);
                if( FALSE == return_result )
                {
                    /* turn off saving */
                    SFS_CloseFile( sfs_nmea_log[log_type]);
                    return_result = NMEA_LOG_FAIL;
                }
                else
                {
                    return_result = NMEA_LOG_SUC;
                }
            }
            else
            {
                return_result = NMEA_LOG_SUC;
            }
            break ;
        }

        case SFS_ERROR_NO_SPACE:/*11:sd card is full,del 50M,*/
        {
            SCI_TRACE_LOW("[GPS]: MMIENG_WriteLOGFile,ret = SFS_ERROR_NO_SPACE");
            return_result  = _WriteFile_DelFile(log_type);
            if( FALSE == return_result )
            {
                /* just turn off saving, tbd more better */
                SFS_CloseFile( sfs_nmea_log[log_type]);
            }
            return_result = NMEA_LOG_NOSPACE;
            break ;
        }

        case SFS_ERROR_DEVICE:/*1: device error*/
        case SFS_ERROR_INVALID_PARAM:/*3: param you give is invalid*/
        {
            /*In UDisk mode, SFS will be unmounted, and return 3 */
            SCI_TRACE_LOW("[GPS]: MMIENG_WriteLOGFile, result = %d",result);
            SFS_CloseFile( sfs_nmea_log[log_type]);
            return_result = NMEA_LOG_FAIL;
            break;
        }

        default:
        {
            SCI_TRACE_LOW("[GPS]: MMIENG_WriteLOGFile, result = %d",result);
            SFS_CloseFile( sfs_nmea_log[log_type]);
            return_result = NMEA_LOG_FAIL;
            break ;
        }
    }

    return return_result;
}
/*****************************************************************************/
// Description: enable log save to T card
// Global resource dependence: BTLOGfile_num, sfs_nmea_log
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_LogOpen (uint8 *newLog_fileName,LOG_SAVE_TYPE log_type)
{
    BOOLEAN ret = FALSE;

    if(NULL == newLog_fileName)
    {
        SCI_TRACE_LOW("[GPS]: LogOpen,logfile name is null !!\n");
        return FALSE;
    }
    
    if (LOG_SAVE_TYPE_MAX <= log_type)
    {
        TRACE_APP_ENGINEERMODE("[GPS]:log_type invalid :%d",log_type);
        return FALSE;
    }

    SCI_TRACE_LOW("[GPS]: enter LogOpen, file name:%s\n", newLog_fileName);

    if(sfs_nmea_log[log_type])
    {
        SFS_CloseFile( sfs_nmea_log[log_type]);
        sfs_nmea_log[log_type] = NULL;
    }

    //LOGfile_num = REFPARAM_GetBtCardLogNum() + 1;
    ret = _CreatLOGFile(newLog_fileName,log_type);
    if(!ret)
    {
        sfs_nmea_log[log_type] = NULL;
        SCI_TRACE_LOW ("[GPS]: Open Log file fail!");
        return FALSE;
    }

    SCI_MEMCPY(&(Log_FileName[log_type]), newLog_fileName, (uint32) SCI_STRLEN(newLog_fileName));
    SCI_TRACE_LOW ("[GPS]: Open Log file success!");

   // REFPARAM_SetBtCardLogNum(BTLOGfile_num);
   // REFPARAM_SetBtCardLogFlag(TRUE);

    return TRUE;
}

/*****************************************************************************/
// Description: disable BT log save to T card
// Global resource dependence: sfs_nmea_log
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void MMIENG_LogClose (LOG_SAVE_TYPE log_type)
{
    if (LOG_SAVE_TYPE_MAX <= log_type)
    {
        TRACE_APP_ENGINEERMODE("[GPS]:log_type invalid :%d",log_type);
        return ;
    }

    SFS_CloseFile( sfs_nmea_log[log_type]);
    sfs_nmea_log[log_type] = NULL;
    SCI_TRACE_LOW ("[GPS]: Close Log file success!");

   // REFPARAM_SetBtCardLogFlag(FALSE);

   // return TRUE;
}

