/*
* mv_fs_interface.c
* this file give the file system interface for mobile video
* date: 2010-5-6
* author: 
*/
#define _MV_FS_API_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_mobile_video_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "sfs.h"
#include "sci_api.h"
#include "mv_fs_api.h"
#include "mv_debug.h"
#include "mv.h"
#include "dal_time.h"
#include "mmifmm_export.h"
#include "mmk_app.h"
#include "mmivp_internal.h"
#include "mmisd_export.h"
#include "mv_mm_api.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

//#define DEBUG_FS 
#define MINUTE  60
#define HOUR   (60*MINUTE)
#define DAY    (24*HOUR)
#define YEAR   (365*DAY)


//#define MOBILE_VIDEO_USER_AGENT L"Lenovo-TD86t"//240X400
#define MOBILE_VIDEO_USER_AGENT L"Lenovo-TD19t" //240X320
//#define MOBILE_VIDEO_USER_AGENT L"GiONEE_TD200"//320X240
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
static int month[12] = {
    0,
        DAY*(31),
        DAY*(31+29),
        DAY*(31+29+31),
        DAY*(31+29+31+30),
        DAY*(31+29+31+30+31),
        DAY*(31+29+31+30+31+30),
        DAY*(31+29+31+30+31+30+31),
        DAY*(31+29+31+30+31+30+31+31),
        DAY*(31+29+31+30+31+30+31+31+30),
        DAY*(31+29+31+30+31+30+31+31+30+31),
        DAY*(31+29+31+30+31+30+31+31+30+31+30)
};


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

                                                 
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : SFS_GetAttr_MV
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E SFS_GetAttr_MV(const uint16 *name, uint32* second);


//#ifdef      DEBUG_FS
//LOCAL void MV_PRINT_STR (const char* function_name, const uint16* name);
// #endif
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : SFS_GetAttr_MV
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E SFS_GetAttr_MV(const uint16 *name, uint32* second)
{
    SFS_DATE_T modify_date = {0};
    SFS_TIME_T modify_time = {0};

    if (PNULL  == name || PNULL == second)
    {
        return SFS_ERROR_IO_PENDING;
    }
    
    if( MMIAPIFMM_GetFileInfo((wchar *)name,
        (uint16)MMIAPICOM_Wstrlen((wchar *)name),
        PNULL,
        &modify_date,
        &modify_time))
    {
        long  res = 0;
        int32  year = 0;
        
        year = modify_date.year - 1970; 
        
        res = YEAR * year + DAY * ((year + 1)/4);
        
        res += month[ modify_date.mon];
        
        if(modify_date.mon > 1 && ((year+2)%4))
        {
            res -=DAY;
        }
        
        res += DAY*(modify_date.mday - 1);
        
        res += HOUR*(modify_time.hour);
        
        res += MINUTE*(modify_time.min);
        
        res += modify_time.sec;
        
        *second = res;
        
    }
#ifdef DEBUG_FS
    //MV_TRACE_LOW:"[MV] SFS_GetAttr_MV *second = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_125_112_2_18_2_39_11_28,(uint8*)"d",*second);
#endif
    return SFS_ERROR_NONE;    
}

/*****************************************************************************/
//  Description : this function is used to open a file 
//  Global resource dependence : 
//  Author:
//  Note: wstrFilename: the file name 
//        dwMode: the open operation mode
//        return: the handle of opened file
/*****************************************************************************/
_HANDLE CM_File_Open(_WCHAR* wstrFilename, _UINT32 dwMode)
{
    _UINT32 handle = 0;
    _UINT32 newMode = 0;
    uint16 new_path[MMIFILE_FULL_PATH_MAX_LEN] = {0};
	
#ifdef DEBUG_FS
		//MV_TRACE_LOW:"[MV] [CM_File_Open] dwMode[%d]"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_143_112_2_18_2_39_11_29,(uint8*)"d",dwMode);
#endif
    MV_path_transfer(wstrFilename, new_path);
    switch(dwMode)
    {  
    case modeRead:

        newMode |= SFS_MODE_OPEN_EXISTING;
        newMode |= SFS_MODE_READ;
        break;
        
    case modeWrite:

        newMode |= SFS_MODE_OPEN_EXISTING;
        newMode |= SFS_MODE_WRITE;
        break;

    case modeReadWrite:

        newMode |= SFS_MODE_OPEN_EXISTING;
        newMode |= SFS_MODE_SHARE_READ;
        newMode |= SFS_MODE_SHARE_WRITE;
        break;

    case modeCreate:

        newMode |= SFS_MODE_CREATE_NEW; 
        newMode |= SFS_MODE_SHARE_READ;
        newMode |= SFS_MODE_SHARE_WRITE;
        break;

    default:
        break;
    }

    handle = MMIAPIFMM_CreateFile((wchar *)new_path,newMode,NULL,NULL);

    if(NULL == handle)
    {
#ifdef  DEBUG_FS
        //MV_TRACE_LOW:"[MV][CM_File_Open] open file fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_183_112_2_18_2_39_11_30,(uint8*)"");
#endif
    }
    else
    {
#ifdef  DEBUG_FS
        //MV_TRACE_LOW:"[MV][CM_File_Open] open file ok handle=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_189_112_2_18_2_39_11_31,(uint8*)"d",handle);
#endif
    }
    return (_HANDLE) handle;

}

/*****************************************************************************/
//  Description : this function is used to close a file
//  Global resource dependence : 
//  Author:
//  Note: hFile: the file handle which will be closed
//        return : NULL
/*****************************************************************************/
_VOID CM_File_Close(_HANDLE hFile)
{
    MMIFILE_HANDLE file_handle = 0;    
    MMIFILE_ERROR_E err = SFS_ERROR_NONE;
    
    file_handle = (SFS_HANDLE) hFile;
    
    err = MMIAPIFMM_CloseFile(file_handle);

#ifdef DEBUG_FS
    //MV_TRACE_LOW:"[MV][CM_File_Close] close file error code = %x "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_211_112_2_18_2_39_11_32,(uint8*)"d",err);
#endif
    
}

/*****************************************************************************/
//  Description : this function is used to read data from file to buffer 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
/*
*this function is used to read data from file to buffer 
*hFile: the file handle
*pBuffer: the data buffer
*nCount: the data number will be read from file 
*return 
*   the number of data : successful
*   0  : fail
*/
_INT CM_File_Read(_HANDLE hFile, _VOID* pBuffer, _INT nCount)
{
    MMIFILE_HANDLE handle = NULL;
    void *buffer = PNULL;
    uint32 bytes_to_read = 0;
    uint32 bytes_read = 0;
    MMIFILE_ERROR_E  err = SFS_ERROR_NONE;
    
    handle = (MMIFILE_HANDLE) hFile;    
    buffer = pBuffer;    
    bytes_to_read = nCount;
    
    err = MMIAPIFMM_ReadFile(handle,buffer,bytes_to_read,&bytes_read,PNULL);

#ifdef  DEBUG_FS
    //MV_TRACE_LOW:"[MV] [CM_File_Read] read file error = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_244_112_2_18_2_39_11_33,(uint8*)"d",err);
#endif
    
    if(err == SFS_ERROR_NONE)
    {        
        return  bytes_read;        
    }
    else
    {        
        return  0;        
    }
}

/*
*this function is used to write data to file 
*hFile: the file handle
*pBuffer: the data buffer  
*nCount: the data number which will write them to file
*/
_INT CM_File_Write(_HANDLE hFile,_VOID* pBuffer,_INT nCount)
{
    MMIFILE_ERROR_E err         = SFS_ERROR_NONE;
    MMIFILE_HANDLE  handle      = 0;
    uint32          bytes_to_write = 0;
    uint32          bytes_written = 0;
    void            *buffer = PNULL;
#ifdef      DEBUG_FS
    //MV_TRACE_LOW:"[MV] [CM_File_Write] begin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_270_112_2_18_2_39_11_34,(uint8*)"");
#endif
    handle = (MMIFILE_HANDLE)hFile;    
    buffer = pBuffer;    
    bytes_to_write = nCount;
    
    err = MMIAPIFMM_WriteFile(handle,buffer,bytes_to_write,&bytes_written,PNULL);   
        
#ifdef      DEBUG_FS
    //MV_TRACE_LOW:"[MV] [CM_File_Write] file write error = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_279_112_2_18_2_39_11_35,(uint8*)"d",err);
#endif

#ifdef      DEBUG_FS
    //MV_TRACE_LOW:"[MV] [CM_File_Write] end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_283_112_2_18_2_39_11_36,(uint8*)"");
#endif
    if(err == SFS_ERROR_NONE)
    {
        return bytes_written;        
    }
    else
    {        
        return 0;        
    }
    
}

/*
*this function is used to flush buffer to file
*hFile: the file handle
*
*
*/
_VOID CM_File_Flush(_HANDLE hFile)
{
    MMIFILE_ERROR_E err = SFS_ERROR_NONE;
    MMIFILE_HANDLE  handle = NULL;
    
    handle =(MMIFILE_HANDLE) hFile;
    
    err = SFS_FlushFile( handle, PNULL);
    
    if(err != SFS_ERROR_NONE)
    {
#ifdef DEBUG_FS     
        //MV_TRACE_LOW:"[MV] [CM_File_Flush] file flush error code = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_313_112_2_18_2_39_12_37,(uint8*)"d",err);
#endif  
    }
    else
    {
#ifdef DEBUG_FS     
        //MV_TRACE_LOW:"[MV]--WD:[CM_File_Flush] file flush error code = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_319_112_2_18_2_39_12_38,(uint8*)"d",err);
#endif      
    }
    
}

/*
*this function is used to get file length
*hFile : the file handle
*
*
*/

_INT CM_File_GetLength(_HANDLE hFile)
{
    MMIFILE_HANDLE  sfs_handle  = NULL;
    uint32          size        = 0;
    
    sfs_handle = (MMIFILE_HANDLE)hFile; 
    
    size = MMIAPIFMM_GetFileSize(sfs_handle);       
    
    return size;   
}

/*
*this function is used to seek a position in file
*hFile: the file handle
*nOffset: the offset position of file
*nFrom: the start position 
*return: TURE or FALSE
*/
_BOOLEAN CM_File_Seek(_HANDLE hFile, _INT nOffset,_INT nFrom)
{
    MMIFILE_ERROR_E err     =   SFS_ERROR_NONE;
    MMIFILE_HANDLE  handle  =    0;
    uint32      offset  =    0 ;
    uint32      origin  =    0 ;
    
    handle =(SFS_HANDLE) hFile;
    
    offset = (uint32)nOffset;
    
    switch(nFrom)
    {
    case 0:
        origin = SFS_SEEK_BEGIN;
        break;
    case 1:
        origin = SFS_SEEK_END;
        break;
    case 2:
        origin = SFS_SEEK_CUR;
        break;
    default:
#ifdef DEBUG_FS
        //MV_TRACE_LOW:"[MV]--WD:[CM_File_Seek]file position parameter error = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_373_112_2_18_2_39_12_39,(uint8*)"d",nFrom);
#endif
        break;
    }
    
    err = MMIAPIFMM_SetFilePointer(handle,offset,origin);       
    
    if(err == SFS_ERROR_NONE)
    {
#ifdef DEBUG_FS     
        //MV_TRACE_LOW:"[MV]--WD:[CM_File_Seek] set file pointer error code = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_383_112_2_18_2_39_12_40,(uint8*)"d",err);
#endif      
        return _TRUE;
        
    }
    else
    {
#ifdef DEBUG_FS
        //MV_TRACE_LOW:"[MV]--WD:[CM_File_Seek] set file pointer error code = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_391_112_2_18_2_39_12_41,(uint8*)"d",err);
#endif          
        return _FALSE;
        
    }
    
}

/*
*this function is used to set the position in file
*hFile: the file handle
*
*
*/
_INT CM_File_GetPosition(_HANDLE hFile)
{  
    MMIFILE_HANDLE handle = (MMIFILE_HANDLE)hFile;
    int32 currentpos = 0;    

    currentpos = MMIAPIFMM_GetFilePointer(handle,SFS_SEEK_BEGIN);     
    
   return currentpos;        
  
}
/*
*this function is used to judge if the file is exist or not
*wstrFileName: the file name
*
*
*/
_BOOLEAN CM_FileExists(_WCHAR* wstrFileName)
{
    MMIFILE_HANDLE      handle = 0;
    _BOOLEAN            ret = FALSE;
    _UINT32             newMode = 0;
    MMIFILE_ERROR_E     err = SFS_ERROR_NONE;
    uint16              new_path[MMIFILE_FULL_PATH_MAX_LEN]= {0}; 

    MV_path_transfer(wstrFileName, new_path);
    newMode |= SFS_MODE_OPEN_EXISTING;
    newMode |= SFS_MODE_SHARE_READ;

    if(wstrFileName != PNULL)
    {
        handle = MMIAPIFMM_CreateFile((wchar*)new_path,newMode,NULL,NULL);       
    }
    
    if( 0 != handle )
    {        
        err = MMIAPIFMM_CloseFile(handle);
        if(err ==  SFS_ERROR_NONE)
        {
#ifdef  DEBUG_FS
            //MV_TRACE_LOW:"[MV]--WD:[CM_FileExists] file exists and close file ok "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_442_112_2_18_2_39_12_42,(uint8*)"");
#endif          
        }
        else
        {
#ifdef   DEBUG_FS     
            //MV_TRACE_LOW:"[MV]--WD:[CM_FileExists] file exists and  close file fail "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_448_112_2_18_2_39_12_43,(uint8*)"");
#endif
        }        
        ret = _TRUE;        
    }
    else
    {
#ifdef   DEBUG_FS  
        //MV_TRACE_LOW:"[MV]--WD:[CM_FileExists] file dose not exists "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_456_112_2_18_2_39_12_44,(uint8*)"");
#endif
        ret = _FALSE;
    }
    
    return ret;
}


/*
*this function is used to create a directory
*wstrFolderName: the directory name which will be create
*
*
*/
_BOOLEAN CM_MakeFolder(_CONST _WCHAR* wstrFolderName)
{
    MMIFILE_ERROR_E    err = SFS_ERROR_NONE;  
    _BOOLEAN        ret = FALSE;    
    uint16          new_path[MMIFILE_FULL_PATH_MAX_LEN]={0};

    MV_path_transfer((uint16 *)wstrFolderName, (uint16 *)new_path); 

    err = MMIAPIFMM_CreateDirectory((wchar *) new_path);  
    
#ifdef  DEBUG_FS
    //MV_TRACE_LOW:"[MV] CM_MakeFolder after MMIAPIFMM_CreateDirectory() err: %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_481_112_2_18_2_39_12_45,(uint8*)"d", err);
#endif

    if(err == SFS_ERROR_NONE)
    {       
        ret = _TRUE;        
    }
    else
    {        
        ret = _FALSE;        
    }

#ifdef  DEBUG_FS
    //MV_TRACE_LOW:"[MV]--WD:[CM_MakeFolder]create directory error code = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_494_112_2_18_2_39_12_46,(uint8*)"d",err);
#endif
    
    return ret;
    
}

/*
*this function is used to delete a directory
*wstrFolderName: the file name 
*
*
*/
_BOOLEAN CM_DeleteFolder(_CONST _WCHAR* wstrFolderName)
{
    MMIFILE_ERROR_E     err         = SFS_ERROR_NONE;
    _BOOLEAN            ret         = FALSE;    
    uint16              new_path[MMIFILE_FULL_PATH_MAX_LEN]   =   {0};

    MV_path_transfer((uint16 *)wstrFolderName, (uint16 *)new_path);  
    err = MMIAPIFMM_DeleteDirectory((wchar *)new_path);      
    
    if(err == SFS_ERROR_NONE)
    {        
        ret = _TRUE;        
    }
    else
    {        
        ret = _FALSE;        
    }

#ifdef  DEBUG_FS    
    //MV_TRACE_LOW:"[MV]--WD:[CM_DeleteFolder] Delete directory error code = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_525_112_2_18_2_39_12_47,(uint8*)"d",err);
#endif
    
    return ret;
    
}

/*
*this function is used to move a file to other place
*wstrExistingFileName: the existing file name
*wstrNewFileName: the new file name 
*
*/
_BOOLEAN CM_MoveFile(_CONST _WCHAR* wstrExistingFileName,_CONST _WCHAR* wstrNewFileName )
{   
    _BOOLEAN        ret     =   FALSE;    
    wchar          sour_path[MMIFILE_FULL_PATH_MAX_LEN]  =   {0};
    wchar          dest_path[MMIFILE_FULL_PATH_MAX_LEN]  =   {0};
#ifdef   DEBUG_FS 
    //MV_TRACE_LOW:"[MV]--WD:[CM_MoveFile] Start Move File \r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_543_112_2_18_2_39_12_48,(uint8*)"");
#endif   
    // MV_PRINT_STR("CM_MoveFile",wstrExistingFileName);
    // MV_PRINT_STR("CM_MoveFile",wstrNewFileName);
    MV_path_transfer((uint16 *)wstrExistingFileName, (uint16 *)sour_path);    
    MV_path_transfer((uint16 *)wstrNewFileName, (uint16 *)dest_path);

    // MV_PRINT_STR("CM_MoveFile",sour_path);
    // MV_PRINT_STR("CM_MoveFile",dest_path);

#ifdef   DEBUG_FS   
    //MV_TRACE_LOW:"[MV]--WD:[CM_MoveFile] Start Move File \r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_554_112_2_18_2_39_12_49,(uint8*)"");
#endif   

    if(SFS_ERROR_NONE == SFS_RenameFile(sour_path, dest_path, NULL))
    {
        ret = _TRUE;
    }
    else
    {
        ret = _FALSE;
    }
    
   
#ifdef  DEBUG_FS
    //MV_TRACE_LOW:"[MV]--WD:[CM_MoveFile] Rename File error code = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_568_112_2_18_2_39_12_50,(uint8*)"d",ret);
#endif
    return ret;
}

/*
*this function start scan files
*wstrFilter:
*pFindData:
*
*/
_HANDLE CM_FindFirstFile(const _WCHAR* wstrFilter, FINDFILE_DATA* pFindData)
{
    MMIFILE_HANDLE      handle  = 0;
    SFS_FIND_DATA_T fd      =   {0};
    _HANDLE         ret     = 0;
    uint16          new_path[MMIFILE_FULL_PATH_MAX_LEN]   =  {0};

    MV_path_transfer((uint16 *)wstrFilter, (uint16 *)new_path);
    
#ifdef      DEBUG_FS
    //MV_TRACE_LOW:"[MV]--WD:[CM_FindFirstFile] Start Find First File \r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_588_112_2_18_2_39_12_51,(uint8*)"");
    //MV_TRACE_LOW:"[MV]--WD:[CM_FindFirstFile] pathname = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_589_112_2_18_2_39_12_52,(uint8*)"s", new_path);
#endif
    
    handle = MMIAPIFMM_FindFirstFile((wchar *)new_path, (uint16)MMIAPICOM_Wstrlen((wchar *)new_path),&fd);
       
    if(handle != 0)
    {        
        memcpy(pFindData->wszFileName, fd.name, MMIFILE_FULL_PATH_MAX_LEN); 
        
        if( fd.attr & SFS_ATTR_DIR)
        {            
            pFindData->uAttr = 0x1;            
        }
        else
        {            
            pFindData->uAttr = 0x0;            
        }        
        ret = (_HANDLE)handle;
        
    }
    else
    {        
        ret = (_HANDLE)0;        
    }
    
#ifdef      DEBUG_FS    
    //MV_TRACE_LOW:"[MV]--WD:[CM_FindFirstFile] pFindData->uAttr= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_615_112_2_18_2_39_12_53,(uint8*)"d",pFindData->uAttr);
    //MV_TRACE_LOW:"[MV]--WD:[CM_FindFirstFile] Find First File handle= %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_616_112_2_18_2_39_12_54,(uint8*)"d",handle);
#endif  
    return ret;
    
}

/*
*this function scan file
*hFind: the file handle which will be find
*pFindData: 
*
*/
_BOOLEAN CM_FindNextFile(_HANDLE hFind,FINDFILE_DATA* pFindData)
{
    MMIFILE_ERROR_E     err     =   SFS_ERROR_NONE;
    MMIFILE_HANDLE      sfs_handle =    0;
    SFS_FIND_DATA_T fd      =   {0};
    _BOOLEAN        ret     =   _FALSE;

#ifdef  DEBUG_FS
    //MV_TRACE_LOW:"[MV] [CM_FindNextFile] start Find Next File "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_635_112_2_18_2_39_12_55,(uint8*)"");
#endif  
    sfs_handle = (MMIFILE_HANDLE)hFind;
    
    err = MMIAPIFMM_FindNextFile(sfs_handle, &fd);
    
    if(err == SFS_ERROR_NONE)
    {        
        memcpy(pFindData->wszFileName, fd.name, _MAX_FILENAME_LEN); 
        
        if( fd.attr&SFS_ATTR_DIR)
        {            
            pFindData->uAttr = 0x1;            
        }
        else
        {            
            pFindData->uAttr = 0x0;            
        }        
        ret = _TRUE;        
    }
    else
    {        
        ret = _FALSE;        
    }
    
#ifdef DEBUG_FS
    //MV_TRACE_LOW:"[MV]--WD:[CM_FindNextFile] Find Next File error code = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_661_112_2_18_2_39_12_56,(uint8*)"d",err);
    //MV_TRACE_LOW:"[MV]--WD:[CM_FindFirstFile] pFindData->uAttr= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_662_112_2_18_2_39_12_57,(uint8*)"d",pFindData->uAttr);
#endif  
    return ret;
    
}

/*
*
*
*
*
*/
_BOOLEAN CM_FindClose(_HANDLE bFind)
{
    MMIFILE_ERROR_E     err         = SFS_ERROR_NONE;
    MMIFILE_HANDLE      sfs_handle  = 0;
    _BOOLEAN            ret         = _FALSE;

#ifdef  DEBUG_FS
    //MV_TRACE_LOW:"[MV]--WD:[CM_FindClose] Find close \r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_680_112_2_18_2_39_12_58,(uint8*)"");
#endif  
    sfs_handle = (MMIFILE_HANDLE)bFind;
    
    err = MMIAPIFMM_FindClose(sfs_handle);       
    
    if(err == SFS_ERROR_NONE)
    {        
        ret = _TRUE;        
    }
    else
    {        
        ret = _FALSE;        
    }

#ifdef      DEBUG_FS
    //MV_TRACE_LOW:"[MV]--WD:[CM_FindClose] Find close error code = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_696_112_2_18_2_39_12_59,(uint8*)"d",err);
#endif  
    return ret;
}

/*
*
*
*
*
*
*/
_INT CM_File_Time(_WCHAR* wstrFilename ,_TIME* fileTime)
{
    
    MMIFILE_ERROR_E     err         = SFS_ERROR_NONE;
    uint32          total_Time  = 0;    
    wchar           new_path[MMIFILE_FULL_PATH_MAX_LEN]={0};
    
    UNUSED(fileTime);

    if (PNULL == wstrFilename)
    {
        return total_Time;
    }
    MV_path_transfer(wstrFilename, new_path);

#ifdef   DEBUG_FS
    //MV_TRACE_LOW:"[MV] [CM_File_Time]start get File Time "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_723_112_2_18_2_39_12_60,(uint8*)"");
#endif    
    
    err = SFS_GetAttr_MV(new_path, &total_Time);
    
#ifdef  DEBUG_FS    
    //MV_TRACE_LOW:"[MV][CM_File_Time] Get File Time error code = %x "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_729_112_2_18_2_39_12_61,(uint8*)"d",err);
#endif  

    return total_Time;
    
}


_BOOLEAN CM_DeleteFile(_CONST _WCHAR* wstrFileName)
{
    MMIFILE_ERROR_E     err     = SFS_ERROR_NONE;
    _BOOLEAN        ret     = FALSE;      
    wchar           new_path[MMIFILE_FULL_PATH_MAX_LEN]   =   {0};

    MV_path_transfer((uint16 *)wstrFileName, (uint16 *)new_path);

#ifdef      DEBUG_FS
    //MV_TRACE_LOW:"[MV] :[CM_DeleteFile] start Delete File"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_746_112_2_18_2_39_12_62,(uint8*)"");
#endif
    
    err = MMIAPIFMM_DeleteFile((wchar *)new_path,NULL);    
    
    if(err == SFS_ERROR_NONE)
    {        
        ret = _TRUE;        
    }
    else
    {        
        ret = _FALSE;        
    }

#ifdef  DEBUG_FS    
    //MV_TRACE_LOW:"[MV] [CM_DeleteFile] Delete File error code = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_761_112_2_18_2_39_12_63,(uint8*)"d",err);
#endif  
    return ret;
    
}

/*****************************************************************************/
//  Description : this function is used to get current folder 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_INT CM_GetCurFolder(_WCHAR* wstrBuffer, _INT nBufferSize)
{
    //uint16 full_path_len = (uint16)nBufferSize;
    wchar default_path[VP_MAX_NAME_LEN+1]={0};
    uint16 default_path_length = VP_MAX_NAME_LEN;
    _BOOLEAN default_result = _FALSE;
    
    //SCI_TRACE_LOW:"[MV] CM_GetCurFolder Entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_778_112_2_18_2_39_13_64,(uint8*)"");

    default_result = MMIAPIVP_GetDefaultFolderPath(default_path, &default_path_length);
        
    if(!default_result)
    {
        return -1;
    }
    else
    {
        return default_path_length;
    }
}

/*****************************************************************************/
//  Description : this function is used to get the MobileVideo folder's path 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_INT CM_GetAppFileName(_WCHAR* wstrBuffer, _INT nBufferSize, _BOOLEAN IsHide)
{
    //_WCHAR strBuffer[60] = {0};
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_SYSTEM;
    if(wstrBuffer == _NULL) 
    {
        return 0;
    }
    
    if(IsHide)
    {
        for(device_type = MMI_DEVICE_SYSTEM; device_type < MMI_DEVICE_NUM; device_type++)
        {
            if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type)))
            {
                if(nBufferSize < MMIAPICOM_Wstrlen(L"/MobileVideo/Mobilevideo.bin") +3 )/*lint !e574 !e737*/
                {
                    return 0;
                }
                wstrBuffer[0] = MMIFILE_SLASH;
                MMIAPICOM_Wstrncpy(&wstrBuffer[1], MMIAPIFMM_GetDevicePath(device_type), MMIFILE_DEVICE_LEN);
                CM_MemCopy(&wstrBuffer[2], L"/MobileVideo/Mobilevideo.bin", (MMIAPICOM_Wstrlen(L"/MobileVideo/Mobilevideo.bin") + 1) * sizeof(_WCHAR));
                //dev_name_len = MMIAPIFMM_GetDevicePathLen(device_type);
                return MMIAPICOM_Wstrlen(wstrBuffer) + 1;                
            }
        }        
    }
    else
    {
        for(device_type = MMI_DEVICE_UDISK; device_type < MMI_DEVICE_NUM; device_type++)
        {
            if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type)))
            {
                if(nBufferSize < MMIAPICOM_Wstrlen(L"/Videos/") +3 )/*lint !e574 !e737*/
                {
                    return 0;
                }
                wstrBuffer[0] = MMIFILE_SLASH;
                MMIAPICOM_Wstrncpy(&wstrBuffer[1], MMIAPIFMM_GetDevicePath(device_type), MMIFILE_DEVICE_LEN);
                CM_MemCopy(&wstrBuffer[2], L"/Videos/", (MMIAPICOM_Wstrlen(L"/Videos/") + 1) * sizeof(_WCHAR));
                //dev_name_len = MMIAPIFMM_GetDevicePathLen(device_type);
                return MMIAPICOM_Wstrlen(wstrBuffer) + 1;                
            }
        }  
    }
    return 0;
}

/*****************************************************************************/
//  Description : 获取手机UA 信息                               
//  Global resource dependence : none                                          
//  Author:                                                                    
//  Note:                                                                      
/*****************************************************************************/
_VOID CM_GetUserAgent(_WCHAR* lpUserAgent,_INT nSize)
{
	//有些手机的UA 信息是一串不能用来使用的混乱字符串, 
	//则关闭宏AUTO_GETUA,使用下面一种方式(主动设置UA )
#ifdef AUTO_GETUA
	char* ua = MMIAPICOM_GetUserAgent();
	//截取想要的手机UA  信息
	MMIAPICOM_StrToWstr((uint8 *)ua,  (wchar *)lpUserAgent);
     //SCI_TRACE_LOW:"[MV] CM_GetUserAgent ua: %s"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_FS_API_856_112_2_18_2_39_13_65,(uint8*)"s", ua);
#else

//	_wcsncpy(lpUserAgent,(_CONST _WCHAR *)MOBILE_VIDEO_USER_AGENT, nSize);
	MMIAPICOM_Wstrncpy((wchar *)lpUserAgent, MOBILE_VIDEO_USER_AGENT, (size_t)nSize);

#endif

}

/*****************************************************************************/
//  Description : this function is used to set current folder 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_SetCurFolder(_WCHAR* wstrBuffer)
{
//    SCI_TRACE_LOW("[MV] CM_SetCurFolder Entry");
    return FALSE;
}

/*****************************************************************************/
//  Description : this function is used to rename the file 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN  CM_ReName(_CONST _WCHAR* wstrExistingFileName, _CONST _WCHAR* wstrNewFileName)
{
//    SCI_TRACE_LOW("[MV] CM_ReName Entry");
    return MMIAPIFMM_RenameFile(wstrExistingFileName, MMIAPICOM_Wstrlen(wstrExistingFileName), (wchar*)wstrNewFileName, MMIAPICOM_Wstrlen(wstrNewFileName));
}

/*****************************************************************************/
//  Description : this function is used to check whether the Tcard is exist 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_IsTCardExist()
{
//    SCI_TRACE_LOW("[MV] CM_IsTCardExist Entry");
    return (MMIAPIFMM_GetFirstValidSD() < MMI_DEVICE_NUM ? TRUE : FALSE) ;
}

//#ifdef      DEBUG_FS
//LOCAL void MV_PRINT_STR (const char* function_name, const uint16* name)
//{
//	char *str = PNULL;
//	uint32 i = 0;
//
//	MV_TRACE_LOW("SFS FAT %s name is:",function_name);
//
//	if(NULL == name)
//	{
//		MV_TRACE_LOW("Zero");
//		return;
//	}
//	if(259 < unistrlength(name))
//	{
//		MV_TRACE_LOW("Too long");
//		return;
//	}
//	
//	str = SCI_ALLOCA(250);
//	SCI_MEMSET(str, 0, 250);
//	
//	while(name[i] && (259 > i))
//	{
//		if (250 - 10 > strlen(str))
//		{
//			sprintf( (char*)(str+strlen(str)), "0x%x,", name[i]);
//		}
//		else
//		{
//			MV_TRACE_LOW("%s",str);
//			SCI_MEMSET(str, 0, 250);
//			sprintf( (char*)(str+strlen(str)), "0x%x,", name[i]);			
//		}
//		i++;
//	}
//	MV_TRACE_LOW("%s",str);
//	SCI_FREE(str);
//}
//
// #endif
