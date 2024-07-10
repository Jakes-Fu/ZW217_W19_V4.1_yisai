/*************************************************************************
 ** File Name:      mmidm_pl_os.c                                        *
 ** Author:         jian.ma                                              *
 ** Date:           2009/07/03                                           *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about dm function     *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2009/07/03     jian.ma          Create.                              *
*************************************************************************/
#define _MMIDM_PL_OS_C_
#include "mmi_app_dm_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef DM_SUPPORT
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "vdm_pl_types.h"
#include "vdm_client_pl_assert.h"
#include "vdm_error.h"
#include "vdm_client_pl_dlpkg.h"
#include "vdm_client_pl_global.h"
#include "vdm_client_pl_registry.h"
#include "vdm_client_pl_storage.h"
#include "vdm_pl_alloc.h"
#include "vdm_pl_async.h"
#include "vdm_pl_memory.h"
#include "vdm_pl_mutex.h"
#include "vdm_pl_string_utils.h"
#include "vdm_pl_stdlib.h"
#include "mmifmm_export.h"
#include "mmidm_config.h"
#include "mmidm_app.h"
//#include "vdm_pl_dlresume.h"
#include "vdm_pl_string.h"
#ifdef __cplusplus
extern   "C"
{
#endif
    //int __mb_cur_max;
//    unsigned short * _pctype;
    int errno;
#ifdef   __cplusplus
    }
#endif
        
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define DM_REG_VALUE_SIZE                   128
#define DM_REG_KEY_NUMBER                   12
#define DM_REG_KEY_LENGTH_MAX               256
#define DM_REG_KEY_VALUE_LENGTH_MAX         DM_REG_VALUE_SIZE
#define DM_FILENAME_SIZE                    MMIFMM_FILE_FILE_NAME_MAX_LEN
#define DM_MAX_LOG_BUFFER_SIZE              (10*1024)

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct  
{
    const char* filename;
    const char* tmpFilename;
    FILE*       file;
} VDM_Client_PL_StorageItem_t;

typedef struct{
    char reg_key[DM_REG_KEY_NUMBER][DM_REG_KEY_LENGTH_MAX];
	char reg_val[DM_REG_KEY_NUMBER][DM_REG_KEY_VALUE_LENGTH_MAX];
    int  reg_key_count;
}MMIDM_REG_T;


/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
const char *s_VDMTreeFile = NULL;
const char *s_VDMConfigFile = NULL; 

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL void*         s_vdmData_ptr = PNULL;
LOCAL SCI_MUTEX_PTR s_dmtask_mutex_p = PNULL;
LOCAL VDM_Handle_t  s_logger_context = PNULL;
LOCAL uint8*        s_msgbuffer_ptr = PNULL;
//LOCAL MMIDM_REG_T   s_vdm_reg = {0};
LOCAL MMIDM_REG_T*  s_vdm_reg_ptr = PNULL;

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Get ID of DL package handle for download data by allocated name
//  Global resource dependence : none
//  Author: jianhui.luo
//  Note: 
/*****************************************************************************/
LOCAL char* VDM_Client_PL_Dlpkg_allocNameFromId(char* inURI);

/*****************************************************************************/
//  Description : get the filename of item
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL const char* GetItemFilename(E_VDM_CLIENT_PL_StorageItem_t inType);

/*****************************************************************************/
//  Description : get the temp filename of item
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL char* GetItemTmpFilename(E_VDM_CLIENT_PL_StorageItem_t    inType);
/********************************************************************************
//  Description: register save.
//  Global resource dependence: 
//  Author: jianhui.luo
//  Note:
 *******************************************************************************/
LOCAL void Reg_Save(void);

/********************************************************************************
//  Description: reguister Init.
//  Global resource dependence: 
//  Author: jianhui.luo
//  Note:
 ********************************************************************************/
LOCAL MMIDM_REG_T* Reg_Init(void);

/********************************************************************************
//  Description: register uninit.
//  Global resource dependence: 
//  Author: jianhui.luo
//  Note:
 ********************************************************************************/
LOCAL void Reg_Uninit(void);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Get ID of DL package handle for download data by allocated name
//  Global resource dependence : none
//  Author: jianhui.luo
//  Note: 
/*****************************************************************************/
LOCAL char* VDM_Client_PL_Dlpkg_allocNameFromId(char* inURI)
{
    uint32  len = 0;
    char*   filename_ptr = PNULL;
    uint32  i = 0;
    
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_allocNameFromId"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_143_112_2_18_2_14_20_738,(uint8*)"");

    if(PNULL == inURI)
    {
        filename_ptr = DM_CLIENT_PL_DP_DEFAULTFILENAME;
    }
    else
    {
        filename_ptr = inURI;
    }

    len = strlen(filename_ptr);

    //if (filename_ptr != NULL)
    {
        if (filename_ptr[0] == '.')
        {    
            filename_ptr[0] = '_';
        }
        for (i=0; i<len ; i++)
        {
            if (filename_ptr[i] == '/')
            {
                filename_ptr[i] = '_';
            }
        }
    }
    
    return filename_ptr;
}

/*!
 *******************************************************************************
 * Allocate a memory block.
 *
 *
 * \param   size        Bytes to allocate.
 *
 * \return  void pointer to the allocated space 
 *          or NULL if there is insufficient memory available
 *******************************************************************************
 */
//void* DM_PL_malloc(IU32 size,
//                   const char          *file,
//                    uint32              line
//                    )
//{
//    void* ptr = PNULL;
//    SCI_TRACE_LOW("MMI_BL_Malloc: %d,%s, line=%ld ptr=0x%x",size,file,line,ptr);
//    ptr = SCI_ALLOC_APP(size);
//    return ptr;
// }
void* VDM_PL_malloc(IU32 size)
{
    //SCI_TRACE_LOW("MMIDM:VDM_PL_malloc:size = %d",size);
    return SCI_ALLOC_APP(size);
}

/*!
 *******************************************************************************
 * Free a memory block.
 *
 * \param   p       Previously allocated memory block to be freed.
 *******************************************************************************
 */
void VDM_PL_free(void *p)
{
    //SCI_TRACE_LOW("MMIDM:VDM_PL_free: 0x%x",p);
    SCI_FREE(p);
}

/*!
 *******************************************************************************
 * Report an assertion failure.
 *
 * \param   inFile      Filename where assertion failure occurred.
 * \param   inLine      Line number where assertion failure occurred.
 *******************************************************************************
 */
void VDM_Client_PL_assertFail(const char *inFile, int inLine)
{   
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_assertFail %s,%d, line=%ld"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_221_112_2_18_2_14_20_739,(uint8*)"sdd",inFile,inLine);
}


/*!
 *******************************************************************************
 * Get a DL package handle for download data. Use input unique id if desired.
 *
 * \param   inURI           URI string set by the the caller.
 *                          Implementation may use this string to generate
 *                          a filename, or may choose to ignore this value.
 * \param   outDlpkgHandle  A handle to a DL package. Can be full path and file name of the file, or
 *                          any other string id, if not using a file system.
 *                          This string will be later used to identify the file.
 *                          Note: Caller must free the allocated string.
 *                      
 *
 * \return  VDM_ERR_OK on success, or an error code on failure
 *******************************************************************************
 */
VDM_Error VDM_Client_PL_Dlpkg_create(
    UTF8CStr inURI, 
    UTF8CStr *outDlpkgHandle)
{
    VDM_Error result = VDM_ERR_OK;
    wchar*    file_fullname_ptr = PNULL;
    char*     file_name_ptr = PNULL;
    wchar*    file_ptr = PNULL;
    uint16    full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    char *    inurl_ptr = PNULL;
    uint32     len= 0;

    if(PNULL == outDlpkgHandle)
    {
    	//SCI_TRACE_LOW:"MMIDM==>VDM_Client_PL_Dlpkg_create PNULL == outDlpkgHandle"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_254_112_2_18_2_14_20_740,(uint8*)"");
	return VDM_ERR_MEMORY;
    }
    else
    {
    	//SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_create: outDlpkgHandle = %s "
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_259_112_2_18_2_14_20_741,(uint8*)"s",outDlpkgHandle);
    }
    VDM_Client_PL_assert(outDlpkgHandle);/*lint !e613 !e831 */
    if (PNULL == inURI)
    {
        *outDlpkgHandle = NULL;/*lint !e613*/
        result = VDM_ERR_MEMORY;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_create: len=%d, inURI= %s "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_269_112_2_18_2_14_20_742,(uint8*)"ds",strlen((char*)inURI),inURI);
	 len=(uint32)strlen((char*)inURI);
        inurl_ptr = SCI_ALLOC_APP(len*2);
        SCI_MEMSET(inurl_ptr,0,(len*2));
        SCI_MEMCPY(inurl_ptr,inURI,len);

        //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_create: inurl_ptr1= %s "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_275_112_2_18_2_14_20_743,(uint8*)"s",inurl_ptr);
        file_name_ptr = VDM_Client_PL_Dlpkg_allocNameFromId(inurl_ptr);

        //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_create: inurl_ptr2= %s "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_278_112_2_18_2_14_20_744,(uint8*)"s",inurl_ptr);
        //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_create: file_name_ptr= %s "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_279_112_2_18_2_14_20_745,(uint8*)"s",file_name_ptr);
        file_ptr = SCI_ALLOC_APP(sizeof(wchar)*(strlen((char*)file_name_ptr)+1));
        SCI_ASSERT(file_ptr);/*assert verified*/
        SCI_MEMSET(file_ptr,0x00,(sizeof(wchar)*(strlen((char*)file_name_ptr)+1)));/*lint !e666*/

        MMIAPICOM_StrToWstr((uint8*)file_name_ptr,file_ptr);
        
        file_fullname_ptr = SCI_ALLOC_APP(sizeof(wchar)*MMIFMM_FULL_FILENAME_LEN);
        SCI_ASSERT(file_fullname_ptr);/*assert verified*/
        SCI_MEMSET(file_fullname_ptr,0x00,(sizeof(wchar)*MMIFMM_FULL_FILENAME_LEN));
        
        file_name_ptr = SCI_ALLOC_APP(sizeof(char)*MMIFMM_FULL_FILENAME_LEN);
        SCI_ASSERT(file_name_ptr);/*assert verified*/
        SCI_MEMSET(file_name_ptr,0x00,(sizeof(char)*MMIFMM_FULL_FILENAME_LEN));
        if(MMIDM_GetFileFullPath(file_ptr,(uint16)MMIAPICOM_Wstrlen(file_ptr),
            file_fullname_ptr,&full_path_len))
        {
            MMIAPICOM_WstrToStr(file_fullname_ptr,file_name_ptr); /*lint !e64*/
            SCI_FREE(file_fullname_ptr);
            *outDlpkgHandle = file_name_ptr; /*lint !e64 !e613*/
        }
        //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_create: file_fullname_ptr= %s "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_300_112_2_18_2_14_21_746,(uint8*)"s",file_name_ptr);
        SCI_FREE(file_ptr);
        SCI_FREE(inurl_ptr);
    }

    SCI_TRACE_LOW("MMIDM==> VDM_Client_PL_Dlpkg_create: result = %x *outDlpkgHandle=%s",result,*outDlpkgHandle);/*lint !e613*/
    return result;
}
/*!
 *******************************************************************************
 * Write the next chunk of bytes from a current DL session
 *
 * \param   inDlpkgHandle   DL package handle as returned in \ref 
 *                          VDM_Client_PL_Dlpkg_create. 
 * \param   inOffset        Offset of the current chunk.
 * \param   inData          The data of the current chunk.
 * \param   inDataSize      Size of data chunk.
 *
 * \return  VDM_ERR_OK on success, or an error code on failure
 *******************************************************************************
 */
VDM_Error VDM_Client_PL_Dlpkg_writeChunk(
    UTF8CStr    inDlpkgHandle,
    IU32        inOffset, 
    IU8*        inData, 
    IU32        inDataSize)
{
    VDM_Error           result = VDM_ERR_OK;
    wchar*              filename_ptr = PNULL;
    MMIFILE_HANDLE      file_handle = 0;
    uint32              writelen = 0;
    wchar*              file_fullname_ptr = PNULL;
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: inOffset = %d, inDataSize=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_330_112_2_18_2_14_21_747,(uint8*)"dd",inOffset,inDataSize);

    if (PNULL == inDlpkgHandle)
    {
        return result;
    }

    if ( 0== inDataSize)
    {
        return result;
    }
    
    filename_ptr = (wchar*)inDlpkgHandle;
    
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: filename_ptr = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_344_112_2_18_2_14_21_748,(uint8*)"s",filename_ptr);

    file_fullname_ptr = SCI_ALLOC_APP(sizeof(wchar)*MMIFMM_FULL_FILENAME_LEN);
    SCI_ASSERT(file_fullname_ptr);/*assert verified*/
    SCI_MEMSET(file_fullname_ptr,0x00,(sizeof(wchar)*MMIFMM_FULL_FILENAME_LEN));

    MMIAPICOM_StrToWstr((uint8*)filename_ptr,file_fullname_ptr);
    if (0==inOffset)
    {
        if(MMIAPIFMM_IsFileExist(file_fullname_ptr,MMIAPICOM_Wstrlen(file_fullname_ptr))) /*lint !e64*/
        {
            SFS_ERROR_E error = MMIAPIFMM_DeleteFile(filename_ptr, NULL);
            //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: error=%d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_356_112_2_18_2_14_21_749,(uint8*)"d",error);
        }
    }
    file_handle= MMIAPIFMM_CreateFile(file_fullname_ptr, SFS_MODE_READ| SFS_MODE_WRITE | SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655 */

    if (0 ==file_handle)
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: 0 == file_handle "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_363_112_2_18_2_14_21_750,(uint8*)"");
        result = VDM_ERR_MO_STORAGE;
    }
    else
    {
        if(SFS_ERROR_NONE != MMIAPIFMM_SetFilePointer(file_handle,inOffset,SFS_SEEK_BEGIN))
        {
            //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: SFS_SEEK error "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_370_112_2_18_2_14_21_751,(uint8*)"");
            result = VDM_ERR_MO_STORAGE;
        }
        else
        {
            MMIAPIFMM_WriteFile(file_handle, (const void *)inData,inDataSize,&writelen, NULL);//||writelen!=inDataSize)
            {
                //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: Write writelen %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_377_112_2_18_2_14_21_752,(uint8*)"d",writelen);
                //result = VDM_ERR_MO_STORAGE;
            }
        }
        MMIAPIFMM_CloseFile(file_handle);
    }
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_writeChunk writelen= %d ,result=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_383_112_2_18_2_14_21_753,(uint8*)"dd",writelen,result);
    SCI_FREE(file_fullname_ptr);

    return result;
}


/*!
 *******************************************************************************
 * Remove DL package from device
 *
 * \param   inDlpkgHandle   File name as returned in \ref 
 *                          VDM_Client_PL_Dlpkg_create or NULL.
 *
 * \return  VDM_ERR_OK on success, or an error code on failure
 *******************************************************************************
 */
VDM_Error VDM_Client_PL_Dlpkg_remove(UTF8CStr inDlpkgHandle)
{
    wchar* filename_ptr = PNULL;
    VDM_Error result =  VDM_ERR_OK;
    uint16 len = 0;
    wchar               full_path_arr[MMIFILE_FULL_PATH_MAX_LEN] = {0};
    uint16              full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_remove"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_407_112_2_18_2_14_21_754,(uint8*)"");
    
    if (PNULL == inDlpkgHandle)
    {
        return result;
    }
    len = (uint16)strlen((char*)inDlpkgHandle);
    
    filename_ptr = SCI_ALLOC_APP(sizeof(wchar)*(len+1));

    SCI_MEMSET(filename_ptr,0x00,(sizeof(wchar)*(len+1)));
    MMIAPICOM_StrToWstr((uint8*)inDlpkgHandle,filename_ptr);
    
    if(MMIDM_GetFileFullPath(filename_ptr,(uint16)MMIAPICOM_Wstrlen(filename_ptr),full_path_arr,&full_path_len))
    {
        if(MMIAPIFMM_IsFileExist(full_path_arr,len))
        {
            if (SFS_ERROR_NONE != MMIAPIFMM_DeleteFile(full_path_arr, NULL))
            {
                result = VDM_ERR_STORAGE_COMMIT;
            }
        }

    }
    //if (PNULL!=filename_ptr)
    {
        SCI_FREE(filename_ptr);
    }
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_remove result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_435_112_2_18_2_14_21_755,(uint8*)"d",result);
    return result;
}

/*!
*******************************************************************************
* Remove DL package max size
*
* \param    outDlpkgMaxSize Where to store the max size. 
*
* \return   VDM_ERR_OK on success, or an error code on failure
*******************************************************************************
*/
VDM_Error VDM_Client_PL_Dlpkg_getMaxSize(IU32 *outDlpkgMaxSize)
{
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Dlpkg_getMaxSize: result "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_449_112_2_18_2_14_21_756,(uint8*)"");
    *outDlpkgMaxSize = MMIDM_GetDevFreeSpace();
    return VDM_ERR_OK;
}


/*!
*******************************************************************************
 * Store vDM opaque data to a globally available pointer.
 *
 * \param   inAnchor    vDM's anchor data
 * 
 * \return  TRUE on success, 
 *          FALSE if instance is already set or on any other failure.
 ******************************************************************************
 */
IBOOL VDM_Client_PL_Global_set(void* inData)
{
    IBOOL result = FALSE;
    
    if (s_vdmData_ptr == PNULL)
    {
        s_vdmData_ptr = inData;
        result = TRUE;
    }
    //SCI_TRACE_LOW("MMIDM==> VDM_Client_PL_Global_set: result = %x ",result);
    return result;
}

/*!
*******************************************************************************
* Get the vDM Anchor object.
*
* \return   vDM Anchor object or NULL.
******************************************************************************
*/
void* VDM_Client_PL_Global_get(void)
{
    //SCI_TRACE_LOW("MMIDM==> VDM_Client_PL_Global_get 0x%x",s_vdmData_ptr);
    return s_vdmData_ptr;
}

/*!
*******************************************************************************
* Reset. Typical implementation will set the global variable to NULL.
******************************************************************************
*/
void VDM_Client_PL_Global_reset(void)
{
    //SCI_TRACE_LOW("MMIDM==>  VDM_Client_PL_Global_reset");
    s_vdmData_ptr = PNULL;
}

/*!
 *******************************************************************************
//  Description: register save.
//  Global resource dependence: 
//  Author: jianhui.luo
//  Note:
 *******************************************************************************
*/
LOCAL void Reg_Save(void)
{
    uint32              write_size = 0;
    MMIFILE_HANDLE      file_handle = 0;
    wchar*              filename_ptr = PNULL;
    MMIFILE_ERROR_E     error = SFS_NO_ERROR;
    wchar               full_path_arr[MMIFILE_FULL_PATH_MAX_LEN] = {0};
    uint16              full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32 			len =0;
    //SCI_TRACE_LOW:"MMIDM==> Reg_Save"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_515_112_2_18_2_14_21_757,(uint8*)"");

    filename_ptr = SCI_ALLOC_APP((strlen(DM_CLIENT_PL_REG_FILE)+1) * sizeof(wchar));
    if (PNULL == filename_ptr)
    {
        //SCI_TRACE_LOW:"MMIDM==> Reg_Save PNULL == filename_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_520_112_2_18_2_14_21_758,(uint8*)"");
        return;
    }
	len =(uint32) strlen(DM_CLIENT_PL_REG_FILE)+1;
    SCI_MEMSET(filename_ptr,0,(len* sizeof(wchar)));

    MMIAPICOM_StrToWstr((uint8*)DM_CLIENT_PL_REG_FILE,filename_ptr);
    if(MMIDM_GetFileFullPath(filename_ptr,(uint16)MMIAPICOM_Wstrlen(filename_ptr),full_path_arr,&full_path_len))
    {
        file_handle = MMIAPIFMM_CreateFile(full_path_arr,SFS_MODE_READ| SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS,PNULL,PNULL);/*lint !e655 */

        if(0 !=file_handle)
        {
            error = MMIAPIFMM_WriteFile(file_handle,s_vdm_reg_ptr,sizeof(MMIDM_REG_T),&write_size,PNULL);

            //SCI_TRACE_LOW:"MMIDM==> Reg_Save error =%d,write_size=%d,sizeof(MMIDM_REG_T) size=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_535_112_2_18_2_14_21_759,(uint8*)"ddd",error,write_size,sizeof(MMIDM_REG_T));
      
            MMIAPIFMM_CloseFile(file_handle);
        }
        else
        {
            //SCI_TRACE_LOW:"MMIDM==> Reg_Save open file fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_541_112_2_18_2_14_21_760,(uint8*)"");
        }
     }
    
     SCI_FREE(filename_ptr);
}

/*!
 *******************************************************************************
//  Description: register save.
//  Global resource dependence: 
//  Author: jianhui.luo
//  Note:
 *******************************************************************************
*/
LOCAL MMIDM_REG_T* Reg_Init(void)
{
    MMIFILE_HANDLE  file_handle = 0;
    wchar*          filename_ptr = PNULL;
    wchar           full_path_arr[MMIFILE_FULL_PATH_MAX_LEN] = {0};
    uint16          full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32          read_size = 0;
    MMIFILE_ERROR_E     error = SFS_NO_ERROR;
    uint32			len = 0;
    //SCI_TRACE_LOW:"MMIDM==> Reg_Init"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_564_112_2_18_2_14_21_761,(uint8*)"");


    if (PNULL == s_vdm_reg_ptr)
    {
        s_vdm_reg_ptr = SCI_ALLOC_APP(sizeof(MMIDM_REG_T));
        if (PNULL == s_vdm_reg_ptr)
        {
            //SCI_TRACE_LOW:"MMIDM==> Reg_Init PNULL == s_vdm_reg_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_572_112_2_18_2_14_21_762,(uint8*)"");
            return PNULL;
        }
    }
    SCI_MEMSET(s_vdm_reg_ptr,0x00,sizeof(MMIDM_REG_T));
    len = (uint32)strlen(DM_CLIENT_PL_REG_FILE)+1;
    filename_ptr = SCI_ALLOC_APP(len * sizeof(wchar));
    if (PNULL == filename_ptr)
    {
        //SCI_TRACE_LOW:"MMIDM==> Reg_Init PNULL == filename_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_581_112_2_18_2_14_21_763,(uint8*)"");
        return s_vdm_reg_ptr;
    }
	len = (uint32)strlen(DM_CLIENT_PL_REG_FILE)+1;
    SCI_MEMSET(filename_ptr,0,(len * sizeof(wchar)));
    
    MMIAPICOM_StrToWstr((uint8*)DM_CLIENT_PL_REG_FILE,filename_ptr);
    if(MMIDM_GetFileFullPath(filename_ptr,(uint16)MMIAPICOM_Wstrlen(filename_ptr),full_path_arr,&full_path_len))
    {
        file_handle = MMIAPIFMM_CreateFile(full_path_arr,SFS_MODE_READ| SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS,PNULL,PNULL);/*lint !e655 */

        if(0 != file_handle)
        {                          
            error = MMIAPIFMM_ReadFile(file_handle,s_vdm_reg_ptr,sizeof(MMIDM_REG_T),&read_size,PNULL);

            //SCI_TRACE_LOW:"MMIDM==> Reg_init error =%d,read_size=%d,sizeof(MMIDM_REG_T) size=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_596_112_2_18_2_14_21_764,(uint8*)"ddd",error,read_size,sizeof(MMIDM_REG_T));
        
            MMIAPIFMM_CloseFile(file_handle);
        }
        else
        {

            //SCI_TRACE_LOW:"MMIDM==> Reg_Init open file fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_603_112_2_18_2_14_21_765,(uint8*)"");
        }
        SCI_FREE(filename_ptr);
    }  
    //SCI_TRACE_LOW:"MMIDM==> Reg_Init end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_607_112_2_18_2_14_21_766,(uint8*)"");
    
    return s_vdm_reg_ptr;
}

/********************************************************************************
//  Description: register uninit.
//  Global resource dependence: 
//  Author: jianhui.luo
//  Note:
 ********************************************************************************/
LOCAL void Reg_Uninit(void)
{
    //SCI_TRACE_LOW:"MMIDM==> Reg_Uninit s_vdm_reg_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_619_112_2_18_2_14_21_767,(uint8*)"d",s_vdm_reg_ptr);
    if (PNULL!=s_vdm_reg_ptr)
    {
        SCI_FREE(s_vdm_reg_ptr);
        s_vdm_reg_ptr = PNULL;
    }
}
/*!
 *******************************************************************************
//  Description: Registry create key.
//  Global resource dependence: 
//  Author: jianhui.luo
//  Note:
 *******************************************************************************
*/
IS32 VDM_Client_PL_Registry_createKey(
    const char* inKey,
    IBOOL       *outAlreadyExist)
{    
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_createKey "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_637_112_2_18_2_14_21_768,(uint8*)"");
    return DM_CLIENT_PL_REGISTRY_KEY;
}

/*!
 *******************************************************************************
//  Description: set string value.
//  Global resource dependence: 
//  Author: jianhui.luo
//  Note:
 *******************************************************************************
*/
IS32 VDM_Client_PL_Registry_setStringValue(
	const char* inKey,
	const char* inValueName,
	char*		inData)
{
    
    uint32 l = strlen(inValueName) + 2;
    char s[DM_REG_KEY_LENGTH_MAX] = {0};
    int key_index=0;
    MMIDM_REG_T* reg_ptr = PNULL;
    
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_setStringValue,inValueName=%s, inData=%s\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_659_112_2_18_2_14_21_769,(uint8*)"ss",inValueName,inData);
    VDM_PL_memset(s, 0 , sizeof(s));

    if (inKey)
    {
        l += VDM_PL_strlen(inKey);
    }
    VDM_PL_snprintf(s, l, "%s%s%s", inKey ? inKey : "",inKey ? "/" : "", inValueName);
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_setStringValue,s=%s\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_667_112_2_18_2_14_22_770,(uint8*)"s",s);
    
    reg_ptr = Reg_Init();

    if (PNULL!=reg_ptr)
    {
        if(reg_ptr->reg_key_count==0)
        {         
            if(VDM_PL_strlen(s))
            {
                VDM_PL_memset(reg_ptr->reg_key[reg_ptr->reg_key_count],0x00,DM_REG_KEY_LENGTH_MAX);
                VDM_PL_strcpy(reg_ptr->reg_key[reg_ptr->reg_key_count],s);
            }
            if( inData!=NULL && VDM_PL_strlen(inData)>0)
            {
                VDM_PL_memset(reg_ptr->reg_val[reg_ptr->reg_key_count],0x00,DM_REG_KEY_VALUE_LENGTH_MAX);
                VDM_PL_strcpy(reg_ptr->reg_val[reg_ptr->reg_key_count],inData);
            }
            reg_ptr->reg_key_count++;
        }
        else
        {
            for(;key_index<reg_ptr->reg_key_count && key_index<DM_REG_KEY_NUMBER;key_index++)
            {

                if( VDM_PL_strlen(reg_ptr->reg_key[key_index])>0 && !(VDM_PL_strncasecmp(reg_ptr->reg_key[key_index],s,VDM_PL_strlen(s))) )
                {
                    if( inData!=NULL && VDM_PL_strlen(inData)>0)
                    {
                        VDM_PL_memset(reg_ptr->reg_val[key_index],0x00,DM_REG_KEY_VALUE_LENGTH_MAX);                
                        VDM_PL_strcpy(reg_ptr->reg_val[key_index],inData);
                    }
                    break;
                }          
            }
            if(key_index >= reg_ptr->reg_key_count)//save a new key
            {
                if(VDM_PL_strlen(s))
                {
                    VDM_PL_memset(reg_ptr->reg_key[reg_ptr->reg_key_count],0x00,DM_REG_KEY_LENGTH_MAX);
                    VDM_PL_strcpy(reg_ptr->reg_key[reg_ptr->reg_key_count],s);
                }
                if( inData!=NULL && VDM_PL_strlen(inData)>0)
                {
                    VDM_PL_memset(reg_ptr->reg_val[reg_ptr->reg_key_count],0x00,DM_REG_KEY_VALUE_LENGTH_MAX);
                    VDM_PL_strcpy(reg_ptr->reg_val[reg_ptr->reg_key_count],inData);
                }
                reg_ptr->reg_key_count++;          
            }
       }
    }
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_setStringValue,key_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_718_112_2_18_2_14_22_771,(uint8*)"d",key_index);
    Reg_Save();
    Reg_Uninit();
   return 0;
}

IS32 VDM_Client_PL_Registry_getStringValue(
	const char* inKey,
	const char* inValueName,
	char*		ioDataBuffer,
	IU32*		ioLength)
{
	VDM_Error result = VDM_ERR_OK;
  
    char *s = NULL;
    uint32 l = 0;
    int key_index=0;
    IU32 outLen = 0;
    MMIDM_REG_T* reg_ptr = PNULL;

    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_getStringValue,inKey=%s\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_738_112_2_18_2_14_22_772,(uint8*)"s",inKey);

    if(ioLength == PNULL || inValueName == PNULL || inKey == PNULL)
    {
        result = VDM_ERR_INVALID_CALL;
        
        //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_getStringValue,result=%x\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_744_112_2_18_2_14_22_773,(uint8*)"d",result);
        return result;
    }
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_getStringValue,inValueName=%s\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_747_112_2_18_2_14_22_774,(uint8*)"s",inValueName);
    l = VDM_PL_strlen(inValueName) + 2;

    l += VDM_PL_strlen(inKey);
    s = SCI_ALLOC_APP(l);

    if (PNULL== s)
    {
        result = VDM_ERR_MEMORY;       
        //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_getStringValue,result=%x, *ioLength=%d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_756_112_2_18_2_14_22_775,(uint8*)"dd",result, *ioLength);
        return result;
    }
    VDM_PL_memset(s, 0 , l);

    VDM_PL_snprintf(s,l, "%s%s%s",inKey ? inKey : "", inKey ? "/" : "" , inValueName);  
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_getStringValue,s=%s\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_762_112_2_18_2_14_22_776,(uint8*)"s",s);
    reg_ptr = Reg_Init();
    for(;key_index<reg_ptr->reg_key_count && key_index<DM_REG_KEY_NUMBER;key_index++)
    {
       if( !(VDM_PL_strncasecmp(reg_ptr->reg_key[key_index],s,VDM_PL_strlen(s))) )
       {
             //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_getStringValue,key_index = %d,reg_ptr->reg_val[key_index]0x%x\n"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_768_112_2_18_2_14_22_777,(uint8*)"dd",key_index,reg_ptr->reg_val[key_index]);
           if (PNULL!=reg_ptr->reg_val[key_index])
           {
                //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_getStringValue,key_index =%d,reg_ptr->reg_val[key_index]=%s\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_771_112_2_18_2_14_22_778,(uint8*)"ds",key_index,reg_ptr->reg_val[key_index]);
                outLen = VDM_PL_strlen(reg_ptr->reg_val[key_index]);
                //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_getStringValue 11,*ioLength =%d,outLen=%d\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_773_112_2_18_2_14_22_779,(uint8*)"dd",*ioLength,outLen);
                if (ioDataBuffer && *ioLength >= outLen)
                {
                   VDM_PL_strncpy(ioDataBuffer, reg_ptr->reg_val[key_index], outLen);
                }
                *ioLength = outLen;
                //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_getStringValue 22,*ioLength =%d,outLen=%d\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_779_112_2_18_2_14_22_780,(uint8*)"dd",*ioLength,outLen);
                break;
           }
       }
    }
    if(key_index>=reg_ptr->reg_key_count)
    {
        *ioLength=0;
    }

    VDM_PL_free(s);
    Reg_Uninit();
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_getStringValue,result=%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_791_112_2_18_2_14_22_781,(uint8*)"d",result);
    return result;
}

/*!
 *******************************************************************************
//  Description: set int value.
//  Global resource dependence: 
//  Author: jianhui.luo
//  Note:
 *******************************************************************************
*/
IS32 VDM_Client_PL_Registry_setIntValue(
    const char* inKey,
    const char* inValueName,
    IU32        inValue)
{
    IS32 result = 0;
    char strValue[DM_REG_VALUE_SIZE] = {0};

    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_setIntValue inKey=%s,inValueName=%s,inValue =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_810_112_2_18_2_14_22_782,(uint8*)"ssd",inKey,inValueName,inValue);

    VDM_PL_IU32toa(inValue, strValue, DM_RADIX_TYPE);

    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_setIntValue strValue =%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_814_112_2_18_2_14_22_783,(uint8*)"s",strValue);

    result = VDM_Client_PL_Registry_setStringValue(inKey, inValueName, strValue);
    return result;
}

/*!
 *******************************************************************************
//  Description: get int value.
//  Global resource dependence: 
//  Author: jianhui.luo
//  Note:
 *******************************************************************************
*/
IS32 VDM_Client_PL_Registry_getIntValue(
    const char* inKey,
    const char* inValueName,
    IU32*       outValue)
{
    char strValue[DM_REG_VALUE_SIZE] = {0};
    IU32 len = DM_REG_VALUE_SIZE;
    int32 ret = 0;
    IBOOL bSuccess = FALSE;

    VDM_PL_memset(strValue,0x00,DM_REG_VALUE_SIZE);
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_getIntValue "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_838_112_2_18_2_14_22_784,(uint8*)"");
    ret =VDM_Client_PL_Registry_getStringValue(inKey, inValueName, strValue, &len);
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_getIntValue len=%d,strValue=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_840_112_2_18_2_14_22_785,(uint8*)"ds",len,strValue);
    if(0 == ret )
    {
        *outValue = VDM_PL_atoIU32(strValue, DM_RADIX_TYPE, &bSuccess);
        if(!bSuccess)
        {
            ret = -1;
        }
    }
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Registry_getIntValue ret=%d,bSuccess=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_849_112_2_18_2_14_22_786,(uint8*)"dd",ret,bSuccess);
    return ret;
}

//This for the part of vdm_client_pl_storage
/*****************************************************************************/
//  Description : get the filename of item
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL const char* GetItemFilename(E_VDM_CLIENT_PL_StorageItem_t inType)
{
    const char* filename = NULL;
    //SCI_TRACE_LOW:"MMIDM==> GetItemFilename inType = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_861_112_2_18_2_14_22_787,(uint8*)"d",inType);

    switch(inType)
    {
    case E_VDM_CLIENT_PL_StorageItem_DMTree:
        filename = s_VDMTreeFile ? s_VDMTreeFile : DM_CLIENT_PL_FILENAME_TREE;
        break;
    case E_VDM_CLIENT_PL_StorageItem_Config:
        filename = s_VDMConfigFile ? s_VDMConfigFile : DM_CLIENT_PL_FILENAME_CONFIG;
        break;
    case E_VDM_CLIENT_PL_StorageItem_DLResume:
        filename = DM_CLIENT_PL_FILENAME_DLRESUME;
        break;
    default:
        break;
    }
    return filename;
}

/*****************************************************************************/
//  Description : get the temp filename of item
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL char* GetItemTmpFilename(E_VDM_CLIENT_PL_StorageItem_t    inType)
{
    char* filename = NULL;
    //SCI_TRACE_LOW:"MMIDM==> GetItemTmpFilename: inType = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_887_112_2_18_2_14_22_788,(uint8*)"d",inType);
    switch(inType)
    {
    case E_VDM_CLIENT_PL_StorageItem_DMTree:
        filename = DM_CLIENT_PL_TMPFILENAME_TREE;
        break;
    case E_VDM_CLIENT_PL_StorageItem_Config:
        filename = DM_CLIENT_PL_TMPFILENAME_CONFIG;
        break;
    case E_VDM_CLIENT_PL_StorageItem_DLResume:
        filename = DM_CLIENT_PL_FILENAME_DLRESUMETMP;
        break;
    default:
        break;
    }
    return filename;
}

/*!
********************************************************************************
* Open (stream) item in storage.
*
* \param    outHandle   Handle to the open storage item.
* \param    inType      Type of storage item: DMTree, Configuration file, etc.
*                       (Must be one of E_VDM_Client_PL_StorageItem_t values).
* \param    inMode      Read or Write (See E_VDM_Client_PL_Storage_Access_t).
*
* \return   VDM_ERR_OK on success; VDM_ERR_MEMORY or VDM_ERR_STORAGE_OPEN on error.
********************************************************************************
*/
VDM_Error VDM_Client_PL_Storage_open(
    void                        **outHandle,
    E_VDM_CLIENT_PL_StorageItem_t       inType, 
    E_VDM_CLIENT_PL_Storage_Access_t    inMode)
{

    VDM_Client_PL_StorageItem_t*    storage_item_ptr = PNULL;
    wchar*                          filename_ptr = PNULL;
    wchar*                          tempfilename_ptr = PNULL;
    VDM_Error                       result = VDM_ERR_OK;
    char*               file_ptr = PNULL;
    char*               temp_ptr = PNULL;
    wchar               full_path_arr[MMIFILE_FULL_PATH_MAX_LEN] = {0};
    uint16              full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_open inType=%d,inMode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_930_112_2_18_2_14_22_789,(uint8*)"dd",inType,inMode);

    storage_item_ptr = SCI_ALLOC_APP(sizeof(VDM_Client_PL_StorageItem_t));
    VDM_Client_PL_assert(storage_item_ptr);
	if(PNULL == storage_item_ptr)
	{
	    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_open ! PNULL == storage_item_ptr"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_936_112_2_18_2_14_22_790,(uint8*)"");
	    return VDM_ERR_MEMORY;
	}
    SCI_MEMSET(storage_item_ptr,0x00,sizeof(VDM_Client_PL_StorageItem_t));
    file_ptr = GetItemFilename(inType);/*lint !e605*/
    temp_ptr = GetItemTmpFilename(inType);

    filename_ptr = SCI_ALLOC_APP(strlen((char*)file_ptr) * sizeof(wchar)+2); 
    VDM_Client_PL_assert(filename_ptr);
	if(PNULL == filename_ptr)
	{
	    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_open ! PNULL == filename_ptr"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_947_112_2_18_2_14_22_791,(uint8*)"");
	    SCI_FREE(storage_item_ptr);
	    storage_item_ptr = PNULL;
		return VDM_ERR_MEMORY;
	}
   SCI_MEMSET(filename_ptr,0x00,(strlen((char*)file_ptr) * sizeof(wchar)+2));/*lint !e666*/
    tempfilename_ptr = SCI_ALLOC_APP(strlen((char*)temp_ptr) * sizeof(wchar)+2); /*lint !e666*/
    VDM_Client_PL_assert(tempfilename_ptr);
	if(PNULL == tempfilename_ptr)
	{
	    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_open ! PNULL == tempfilename_ptr"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_957_112_2_18_2_14_22_792,(uint8*)"");
	    SCI_FREE(storage_item_ptr);
	    storage_item_ptr = PNULL;
	    SCI_FREE(filename_ptr);
	    filename_ptr = PNULL;
	    return VDM_ERR_MEMORY;
	}
    SCI_MEMSET(tempfilename_ptr,0x00,(strlen((char*)temp_ptr) * sizeof(wchar)+2));/*lint !e666*/

    MMIAPICOM_StrToWstr((uint8*)file_ptr,filename_ptr);
    MMIAPICOM_StrToWstr((uint8*)temp_ptr,tempfilename_ptr);

    storage_item_ptr->filename = filename_ptr; /*lint !e64*/
    storage_item_ptr->tmpFilename = tempfilename_ptr; /*lint !e64*/

    if (E_VDM_CLIENT_PL_Storage_Access_read ==inMode)
    {
        if(MMIDM_GetFileFullPath(filename_ptr,(uint16)MMIAPICOM_Wstrlen(filename_ptr),full_path_arr,&full_path_len))
        {
            storage_item_ptr->file = (FILE*)MMIAPIFMM_CreateFile(full_path_arr, SFS_MODE_READ| SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655 */
        }
    }
    else
    {
        VDM_Client_PL_assert(inMode == E_VDM_CLIENT_PL_Storage_Access_write);
        
        if(MMIDM_GetFileFullPath(tempfilename_ptr,(uint16)MMIAPICOM_Wstrlen(tempfilename_ptr),full_path_arr,&full_path_len))
        {
            storage_item_ptr->file = (FILE*)MMIAPIFMM_CreateFile(full_path_arr, SFS_MODE_READ| SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655 */
        }
    }

    if (PNULL ==storage_item_ptr->file)
    {
        //SCI_TRACE_LOW:"MMIDM==> Cannot open file !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_991_112_2_18_2_14_22_793,(uint8*)"");
        result = VDM_ERR_BAD_INPUT;
    }
    else
    {
        *outHandle = storage_item_ptr;
    }

    return result;
}


/*!
********************************************************************************
* Read data from storage.
*
* \param    inHandle        Handle to the storage item.
* \param    inBuffer        Buffer to store the output read data. 
* \param    inBufSize       Buffer size (in bytes)
* \param    outReadCount    Number of bytes actually read.
*                           (Zero  when no more data is available).
* 
* \return   VDM_ERR_OK on success; VDM_ERR_MEMORY or VDM_ERR_STORAGE_READ on error
********************************************************************************
*/
VDM_Error VDM_Client_PL_Storage_read(
    void*       inHandle, 
    void*       inBuffer,
    IU32        inBufSize, 
    IU32*       outReadCount)
{    
    VDM_Client_PL_StorageItem_t* stream_ptr = PNULL;
    VDM_Error result = VDM_ERR_OK;
    SFS_ERROR_E error = SFS_NO_ERROR;
    uint32  read_size  = 0;
    VDM_Client_PL_assert(inHandle);
    
    stream_ptr = (VDM_Client_PL_StorageItem_t*)inHandle;
    //SCI_TRACE_LOW("MMIDM:VDM_Client_PL_Storage_read");

    if (PNULL ==stream_ptr)
    {
        result = VDM_ERR_STORAGE_READ;
        //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_read PNULL ==stream_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1033_112_2_18_2_14_22_794,(uint8*)"");
        return result;
    }

    VDM_Client_PL_assert(stream_ptr->file);
    VDM_Client_PL_assert(inBuffer);
    
    error = MMIAPIFMM_ReadFile(
                (MMIFILE_HANDLE)stream_ptr->file, 
                inBuffer, 
                inBufSize, 
                &read_size, 
                NULL
                );
    if (SFS_ERROR_NONE != error)
    {
        result = VDM_ERR_STORAGE_READ;
        //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_read VDM_ERR_STORAGE_READ"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1050_112_2_18_2_14_22_795,(uint8*)"");
    }
    else
    {
        *outReadCount = read_size;
    }

    return result;
}

/*!
********************************************************************************
* Write data to storage.
*
* \param    inHandle        Handle to the storage item.
* \param    inData          Data to write. 
* \param    inLength        The length of the data to write (in bytes).
* 
* \return   VDM_ERR_OK on success; VDM_ERR_MEMORY or VDM_ERR_STORAGE_WRITE on error
********************************************************************************
*/
VDM_Error VDM_Client_PL_Storage_write(
    void *      inHandle, 
    const void* inData, 
    IU32        inLength)
{
    VDM_Client_PL_StorageItem_t* stream_ptr = PNULL;
    int16 result = VDM_ERR_OK;
    SFS_ERROR_E error = SFS_NO_ERROR;
    uint32 writelen = 0;
    //uint8* file_buf   = PNULL;

    VDM_Client_PL_assert(inHandle);

    //SCI_TRACE_LOW("MMIDM:VDM_Client_PL_Storage_write ");
    stream_ptr = (VDM_Client_PL_StorageItem_t*)inHandle;
    if (PNULL == stream_ptr)
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_write VDM_ERR_STORAGE_WRITE 1"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1087_112_2_18_2_14_22_796,(uint8*)"");
        result = VDM_ERR_STORAGE_WRITE;
        return result;
    }
    
    error = MMIAPIFMM_WriteFile(
                (MMIFILE_HANDLE)stream_ptr->file, 
                (const void *)inData, 
                inLength, 
                &writelen, 
                NULL
                );
    if (writelen < inLength ||SFS_ERROR_NONE != error) 
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_write VDM_ERR_STORAGE_WRITE 2"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1101_112_2_18_2_14_22_797,(uint8*)"");
        result = VDM_ERR_STORAGE_WRITE;
    }
    
    return result;
}

/*!
********************************************************************************
* Close storage item.
*
* \param    inHandle        Handle to the storage item.
* \param    inCommit        TRUE if written data should be committed.
*                           (Must be FALSE if storage was open for reading).
* 
* \return   VDM_ERR_OK on success; VDM_ERR_MEMORY or VDM_ERR_STORAGE_READ on error
********************************************************************************
*/
VDM_Error VDM_Client_PL_Storage_close(
    void *      inHandle, 
    IBOOL       inCommit)
{
    VDM_Error result = VDM_ERR_OK;
    VDM_Client_PL_StorageItem_t* storage_item_ptr = PNULL;
    wchar               full_path_arr[MMIFILE_FULL_PATH_MAX_LEN] = {0};
    uint16              full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar               tempfilename_arr[MMIFILE_FULL_PATH_MAX_LEN] = {0};
    uint16              tempfilename_len = MMIFILE_FULL_PATH_MAX_LEN;
    VDM_Client_PL_assert(inHandle);

    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_close "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1130_112_2_18_2_14_23_798,(uint8*)"");
    storage_item_ptr = (VDM_Client_PL_StorageItem_t*)inHandle;
    if (PNULL == storage_item_ptr)
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_close VDM_ERR_STORAGE_WRITE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1134_112_2_18_2_14_23_799,(uint8*)"");
        VDM_PL_freeAndNullify(storage_item_ptr);
        return result;
    }

    if(0 != (MMIFILE_HANDLE)storage_item_ptr->file)
    {
        MMIAPIFMM_CloseFile((MMIFILE_HANDLE)storage_item_ptr->file);
    }

    //if file was a temporary file, rename it to original file name.
    if (inCommit && storage_item_ptr->tmpFilename)
    {
        // remove old file    
        if(MMIDM_GetFileFullPath(storage_item_ptr->filename,(uint16)MMIAPICOM_Wstrlen(storage_item_ptr->filename),full_path_arr,&full_path_len)) /*lint !e64*/
        {
            if (SFS_ERROR_NONE != MMIAPIFMM_DeleteFile(full_path_arr, NULL))
            {
                //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_close filename_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1152_112_2_18_2_14_23_800,(uint8*)"");
                result = VDM_ERR_STORAGE_COMMIT;
            }
        }
        if (result ==VDM_ERR_OK)
        {
            if(MMIDM_GetFileFullPath(storage_item_ptr->tmpFilename,(uint16)MMIAPICOM_Wstrlen(storage_item_ptr->tmpFilename),tempfilename_arr,&tempfilename_len)) /*lint !e64*/
            {
                // rename temp file to original file name
                if(SFS_ERROR_NONE !=SFS_RenameFile(tempfilename_arr,full_path_arr,NULL))
                {
                    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_close tempfilename_ptr"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1163_112_2_18_2_14_23_801,(uint8*)"");
                    result = VDM_ERR_STORAGE_COMMIT;
                }
            }
        }
    }

    if (PNULL!=storage_item_ptr->filename)
    {
        SCI_FREE(storage_item_ptr->filename);/*lint !e605*/
    }
    if (PNULL!=storage_item_ptr->tmpFilename)
    {
        SCI_FREE(storage_item_ptr->tmpFilename);/*lint !e605*/
    }
    SCI_FREE(storage_item_ptr);

    return result;
}


/*!
********************************************************************************
* Delete an item from storage.
*
* \param    inType      Type of storage item: DMTree, Configuration file, etc.
*                       (Must be one of E_VDM_Client_PL_StorageItem_t values).
********************************************************************************
*/
void VDM_Client_PL_Storage_delete(E_VDM_CLIENT_PL_StorageItem_t inType)
{
    //SFS_HANDLE      sfs_handle = 0;
    wchar* wfilename_ptr = PNULL;
    wchar* wtempfilename_ptr = PNULL;
    const char* filename = GetItemFilename(inType);
    wchar               full_path_arr[MMIFILE_FULL_PATH_MAX_LEN] = {0};
    uint16              full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_delete inType=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1199_112_2_18_2_14_23_802,(uint8*)"d",inType);
   if(PNULL == filename)
   {
   	//SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_delete PNULL == filename"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1202_112_2_18_2_14_23_803,(uint8*)"");
	return;
   }

    wfilename_ptr = SCI_ALLOC_APP(strlen((char*)filename) * sizeof(wchar)+2); 
    if(PNULL == wfilename_ptr)
    {
    	//SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_delete PNULL == wfilename_ptr"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1209_112_2_18_2_14_23_804,(uint8*)"");
	return;
    }
//    VDM_Client_PL_assert(wfilename_ptr);
    SCI_MEMSET(wfilename_ptr,0x00,(strlen((char*)filename) * sizeof(wchar)+2));/*lint !e666*/

    MMIAPICOM_StrToWstr((uint8*)filename,wfilename_ptr);
    
    if (SFS_ERROR_NONE != MMIAPIFMM_DeleteFile(wfilename_ptr, NULL))
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_delete Could not delete wfilename_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1219_112_2_18_2_14_23_805,(uint8*)"");
    }
    //tmp file shouldn't exist, but just in case...
    filename = GetItemTmpFilename(inType);
    wtempfilename_ptr = SCI_ALLOC_APP(strlen((char*)filename) * sizeof(wchar)+2); 
    if(PNULL == wtempfilename_ptr)
    {
    	//SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_delete PNULL == wtempfilename_ptr"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1226_112_2_18_2_14_23_806,(uint8*)"");
	SCI_FREE(wfilename_ptr);
	wfilename_ptr = PNULL;
	return;
    }
    /*lint -save -e831 */
    VDM_Client_PL_assert(wtempfilename_ptr);/*lint !e831 */
    SCI_MEMSET(wtempfilename_ptr,0x00,(strlen((char*)filename) * sizeof(wchar)+2));/*lint !e666 !e831 !e668 */ 
     /*lint -restore */
    
    MMIAPICOM_StrToWstr((uint8*)filename,wtempfilename_ptr);
    VDM_Client_PL_assert(wtempfilename_ptr);
    if(MMIDM_GetFileFullPath(wtempfilename_ptr,(uint16)MMIAPICOM_Wstrlen(wtempfilename_ptr),full_path_arr,&full_path_len))
    {
        if (SFS_ERROR_NONE != MMIAPIFMM_DeleteFile(full_path_arr, NULL))
        {
            //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_Storage_delete Could not delete tempfilename_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1242_112_2_18_2_14_23_807,(uint8*)"");
        }
    }
    
    //if (PNULL!=wfilename_ptr)
    {
        SCI_FREE(wfilename_ptr);
    }
    if (PNULL!=wtempfilename_ptr)
    {
        SCI_FREE(wtempfilename_ptr);
    }
}

/*!
*******************************************************************************
 * Compare two buffers of bytes.
 *
 * \param   buf1        A buffer.
 * \param   buf2        Another buffer.
 * \param   length      Number of bytes to compare.
 *
 * \retval  Negative if \a buf1 < \a buf2
 * \retval  Zero if \a buf1 == \a buf2
 * \retval  Positive if \a buf1 > \a buf2
*******************************************************************************
 */
int VDM_PL_memcmp(const void *buf1, const void *buf2, IU32 length)
{
    return memcmp(buf1, buf2, length);
}

/*!
*******************************************************************************
 * Copies bytes from one place to another. The source and destination bytes
 * must not overlap.
 *
 * \param   dst         Destination pointer.
 * \param   src         Source pointer.
 * \param   length      Number of bytes to copy.
*******************************************************************************
 */
void VDM_PL_memcpy(void *dst, const void *src, IU32 length)
{
    SCI_MEMCPY(dst, src, length);
}


/*!
*******************************************************************************
 * Copies bytes from one place to another. The source and destination bytes
 * may overlap.
 *
 * \param   dst         Destination pointer.
 * \param   src         Source pointer.
 * \param   length      Number of bytes to copy.
*******************************************************************************
 */
void VDM_PL_memmove(void *dst, const void *src, IU32 length)
{
    memmove(dst, src, length);
}

/*!
*******************************************************************************
 * Set a byte sequence to a common value.
 *
 * \param   dst         Pointer to first byte in memory to be set.
 * \param   val         Value to which bytes are set.
 * \param   length      Number of bytes to set.
*******************************************************************************
 */
void VDM_PL_memset(void *dst, IU8 val, IU32 length)
{
    SCI_MEMSET(dst, val, length);
}


//This for the part of vdm_pl_mutex
//Create and Open a mutex.
/*!
 *******************************************************************************
 * Create and Open a mutex.
 *
 * \return  Handle to the mutex.
 *******************************************************************************
 */
VDM_Handle_t VDM_PL_Mutex_create(void)
{
    //SCI_TRACE_LOW:"MMIDM==> VDM_PL_Mutex_create "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1326_112_2_18_2_14_23_808,(uint8*)"");
    if(PNULL != s_dmtask_mutex_p)
    {
        SCI_DeleteMutex(s_dmtask_mutex_p);
        s_dmtask_mutex_p = PNULL;
    }
    s_dmtask_mutex_p = SCI_CreateMutex("dmtask_mutex", SCI_INHERIT);
    
    return s_dmtask_mutex_p;
}

//Lock mutex
/*!
 *******************************************************************************
 * Unlock mutex
 *
 * \param   inMutex     Mutex handle to unlock.
 *******************************************************************************
 */
void VDM_PL_Mutex_lock(VDM_Handle_t inMutex)
{
    if (PNULL !=s_dmtask_mutex_p)
    {
        if(0 == SCI_GetMutex(s_dmtask_mutex_p,SCI_WAIT_FOREVER))
        {
            //SCI_TRACE_LOW("MMIDM==> VDM_PL_Mutex_lock success");
        }
        else
        {
            //SCI_TRACE_LOW:"MMIDM==> VDM_PL_Mutex_lock fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1354_112_2_18_2_14_23_809,(uint8*)"");
        }
    }


}

/*!
 *******************************************************************************
 * Unlock mutex
 *
 * \param   inMutex     Mutex handle to unlock.
 *******************************************************************************
 */
void VDM_PL_Mutex_unlock(VDM_Handle_t inMutex)
{
    if (PNULL !=s_dmtask_mutex_p)
    {
        if (0 ==SCI_PutMutex(s_dmtask_mutex_p))
        {
            //SCI_TRACE_LOW("MMIDM==> VDM_PL_Mutex_unlock success");
        }    
        else
        {
            //SCI_TRACE_LOW:"MMIDM==> VDM_PL_Mutex_unlock fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1377_112_2_18_2_14_23_810,(uint8*)"");
        }
    }
}


/*!
 *******************************************************************************
 * Close mutex
 *
 * \param   inMutex     Mutex handle to close.
 *******************************************************************************
 */

void VDM_PL_Mutex_close(VDM_Handle_t inMutex)
{
    if (PNULL!=s_dmtask_mutex_p)
    {
        if (0 ==SCI_DeleteMutex(s_dmtask_mutex_p))
        {
            s_dmtask_mutex_p= PNULL;
            //SCI_TRACE_LOW:"MMIDM==> VDM_PL_Mutex_close success"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1397_112_2_18_2_14_23_811,(uint8*)"");
        }
        else
        {
            //SCI_TRACE_LOW("MMIDM==> VDM_PL_Mutex_close fail");
        }
    }
}

//This for the part of vdm_pl_async
/*!
*******************************************************************************
* Allow asynchronous object to explicitly yield the processor.
* 
* This is useful for non-preemptive environments, such as BREW.
******************************************************************************
*/
IBOOL VDM_PL_Async_signal(VDM_Handle_t inAsyncHandle)
{
    uint32  task_id = (uint32)inAsyncHandle;
    MMIDM_SESSION_T session ={0};
    session.type = DM_SESSION_NONE;
    session.message_buffer = PNULL;
    session.msg_size = 0;
    //SCI_TRACE_LOW:"MMIDM==> VDM_PL_Async_signal "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1420_112_2_18_2_14_23_812,(uint8*)"");
    
    MMIDM_SendSigToDmTask(DM_TASK_RUN_MESSAGE,task_id,&session);
    return TRUE;
}

/*!
*******************************************************************************
* Allow asynchronous object to explicitly yield the processor.
* 
* This is useful for non-preemptive environments, such as BREW.
******************************************************************************
*/
void VDM_PL_Async_yield(VDM_Handle_t inAsyncHandle)
{
    //uint32  task_id = (uint32)inAsyncHandle;
    //SCI_TRACE_LOW:"MMIDM==> VDM_PL_Async_yield "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1435_112_2_18_2_14_23_813,(uint8*)"");
    //nothing to do
    //MMIDM_SendSigToDmTask(DM_TASK_EXIT_MESSAGE,task_id);
}

/*!
 *******************************************************************************
 * Initialize platform specific logging data and store logger context.
 *
 * \param   inLoggerContext handle to the logger utility context that
 *          should be available for retrieval by the logger.
 *
 * \return  TRUE on success, otherwise FALSE.
 *******************************************************************************
 */
IBOOL VDM_Client_PL_logInit(VDM_Handle_t inLoggerContext)
{
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_logInit "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1451_112_2_18_2_14_23_814,(uint8*)"");
    s_logger_context = inLoggerContext;

    if (PNULL == s_msgbuffer_ptr)
    {
        s_msgbuffer_ptr = SCI_ALLOC_APP(DM_MAX_LOG_BUFFER_SIZE);
        SCI_MEMSET(s_msgbuffer_ptr,0x00,DM_MAX_LOG_BUFFER_SIZE);
    }
    return TRUE;
}

/*!
 *******************************************************************************
 * Terminate logging. Free allocated data if needed.
 *******************************************************************************
 */
void VDM_Client_PL_logTerm(void)
{
    //SCI_TRACE_LOW:"MMIDM==> VDM_Client_PL_logTerm "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1468_112_2_18_2_14_23_815,(uint8*)"");
    s_logger_context = PNULL;
    if (PNULL!=s_msgbuffer_ptr)
    {
        SCI_FREE(s_msgbuffer_ptr);
        s_msgbuffer_ptr = PNULL;
    }
}

/*!
 *******************************************************************************
 * Get logger utility context.
 *
 * \return  Handle to context passed during initialization.
 *******************************************************************************
 */
VDM_Handle_t VDM_Client_PL_logGetContext(void)
{
    //SCI_TRACE_LOW("MMIDM==> VDM_Client_PL_logGetContext ");
    return s_logger_context;
}

/*!
 *******************************************************************************
 * Log input message.
 *
 * This function is always preceded by a call to \ref VDM_Client_PL_logPrefix.
 * No extra decorations should be added to the input message
 *
 * \param   inFormat    Format string.
 * \param   ...         Optional arguments.
 *******************************************************************************
 */
void VDM_Client_PL_logMsg(const char* inFormat, ...)
{
    if (PNULL!=s_msgbuffer_ptr)
    {
        va_list argp;
        va_start(argp, inFormat);/*lint !e718 !e64 !e746*/
        vsprintf(s_msgbuffer_ptr,inFormat,argp); /*lint !e64*/
        va_end(argp);
    
        //SCI_TRACE_LOW:"MMIDM==> VDMLIB:%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1508_112_2_18_2_14_23_816,(uint8*)"s",s_msgbuffer_ptr);
    }
}

/*!
 *******************************************************************************
 * Log prefix to an upcoming log message.
 *
 * This function is always followed by a call to \ref VDM_Client_PL_logMsg.
 * The implementation may add additional text to the prefix, e.g. thread id,
 * time stamp, etc.
 *
 *
 * \param   inLevel     Severity level. Implementor may choose to assign
 *                      different display attributes according to severity level,
 *                      e.g.display different levels in different colors.
 * \param   inFormat    Format string.
 * \param   ...         Optional arguments.
 *
 *******************************************************************************
 */
void VDM_Client_PL_logPrefix(IU32 inLevel, const char* inFormat, ...)
{
    if (PNULL!=s_msgbuffer_ptr)
    {
        va_list argp;
        va_start(argp, inFormat);/*lint !e64*/
        vsprintf(s_msgbuffer_ptr,inFormat,argp); /*lint !e64*/
        va_end(argp);
        //SCI_TRACE_LOW:"MMIDM==> VDMLIB:%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1536_112_2_18_2_14_23_817,(uint8*)"s",s_msgbuffer_ptr);
    }
    
}

void RDM_DBG_debugPrintf(const char* inFormat,...)
{
    if (PNULL!=s_msgbuffer_ptr)
    {
        va_list argp;
        va_start(argp, inFormat); /*lint !e64*/
        vsprintf(s_msgbuffer_ptr,inFormat,argp); /*lint !e64*/
        va_end(argp);
        //SCI_TRACE_LOW:"MMIDM==> VDMLIB:%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1549_112_2_18_2_14_23_818,(uint8*)"s",s_msgbuffer_ptr);
    }
}

void RDM_DBG_diagPrintf(const char* inFormat,...)
{
    if (PNULL!=s_msgbuffer_ptr)
    {
        va_list argp;
        va_start(argp, inFormat);/*lint !e64*/
        vsprintf(s_msgbuffer_ptr,inFormat,argp); /*lint !e64*/
        va_end(argp);
        //SCI_TRACE_LOW:"MMIDM==> VDMLIB:%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1561_112_2_18_2_14_23_819,(uint8*)"s",s_msgbuffer_ptr);
    }
}

/*****************************************************************************/
//  Description : trace string content
//  Global resource dependence : none
//  Author:jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_TraceString(char* inBuffer, uint32 bufferlen)
{

    uint32 i = 0;
    if (PNULL == inBuffer||0== bufferlen)
    {
        return;
    }

    for (i=0;i<bufferlen;)
    {
        //SCI_TRACE_LOW:"MMIDM==> 0x%x:%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1582_112_2_18_2_14_23_820,(uint8*)"ddddddddddddddddd",i,inBuffer[i],inBuffer[i+1],inBuffer[i+2],inBuffer[i+3],inBuffer[i+4],inBuffer[i+5],inBuffer[i+6],inBuffer[i+7],inBuffer[i+8],inBuffer[i+9],inBuffer[i+10],inBuffer[i+11],inBuffer[i+12],inBuffer[i+13],inBuffer[i+14],inBuffer[i+15]);
        //SCI_TRACE_LOW:"MMIDM==> %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1585_112_2_18_2_14_23_821,(uint8*)"cccccccccccccccc",inBuffer[i],inBuffer[i+1],inBuffer[i+2],inBuffer[i+3],inBuffer[i+4],inBuffer[i+5],inBuffer[i+6],inBuffer[i+7],inBuffer[i+8],inBuffer[i+9],inBuffer[i+10],inBuffer[i+11],inBuffer[i+12],inBuffer[i+13],inBuffer[i+14],inBuffer[i+15]);
        SCI_Sleep(10);
        i+=16;
    }
}


PUBLIC void VDM_Notify_PL_Task_Finished(void)
{
    MMIDM_SESSION_T session ={0};
    session.type = DM_SESSION_NONE;
    session.message_buffer = PNULL;
    session.msg_size = 0;
    //SCI_TRACE_LOW:"MMIDM==> VDM_Notify_PL_Task_Finished "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_OS_1598_112_2_18_2_14_23_822,(uint8*)"");
    
    MMIDM_SendSigToDmTask(DM_TASK_EXIT_MESSAGE,MMIDM_GetDmTaskID(),&session);
}
#endif