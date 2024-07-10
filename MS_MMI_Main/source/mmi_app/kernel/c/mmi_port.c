/****************************************************************************
** File Name:      mmk_port.c                                              *
** Author:         Great.Tian                                              *
** Date:           14/01/2004                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the port modual.          *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 01/2004        Great.Tian       Create
** 
****************************************************************************/
#define MMI_PORT_C


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mmifmm_export.h"
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "mmi_osbridge.h"
#include "mmi_port.h"
#include "mmk_type.h"
#include "mmk_ext_app.h"
#include "tb_dal.h"
#include "sig_code.h"
#include "in_message.h"
//#include "mmi_id.h"
#include "mmi_appmsg.h"
#ifdef WIN32
#include "mmiudisk_simu.h"
#else
#include "upm_api.h" // @wancan.you udisk
#endif
#include "sig_code.h"
#include "mmi_appmsg.h"
#include "mmibt_export.h" // baokun add
/*
#ifdef MMI_BLUETOOTH
#include "mmi_btinterface.h"
#endif
*/
#include "mmiocr_export.h"
#ifdef CMMB_SUPPORT
#include "mtv_api.h"
#endif
#ifdef MMI_ENABLE_DCD
#include "mmidcd_export.h"
#endif //0
#ifdef MBBMS_SUPPORT
//#ifdef WIN32
#include "mbbms_api_dummy.h"
#include "mbbms_service.h"
//#endif
#endif
#ifdef ATV_SUPPORT
#include "mmiatv_export.h"
#endif
#if defined(MMI_3DUI_SUPPORT)
#include "mmise.h"
#endif
#ifdef PUSH_EMAIL_SUPPORT   
    extern MMI_APPLICATION_T g_mmimail_app;
#endif /* PUSH_EMAIL_SUPPORT  */

#define DRM_API_DUMMY      0

#if DRM_API_DUMMY
#include "drm_api.h"
#include "mime_type.h"
#endif

#ifdef MMIEMAIL_SUPPORT
#include "mmiemail_export.h"
#endif

//#ifdef QBTHEME_SUPPORT
#include "theme_gzip_export.h"
//#endif
#ifdef BLUETOOTH_SUPPORT
#include "ual_bt.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
#define REG_APP(_STARTMSG,_ENDMSG,_APP_PTR_)  {_STARTMSG,_ENDMSG,_APP_PTR_}, 

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//lint -esym(552, g_reg_app)
const REGISTER_APP_SIG_T g_reg_app[] =
{
#include "mmk_regapp.def"

#ifdef BLUETOOTH_SUPPORT
//add bluetooth signal for sub app g_bluetooth_app
#include "bt_regapp.def"
#endif
};

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get the number of the reg application
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
uint32 MMI_GetRegAppNum(
                        void
                       )
{
    return ARR_SIZE( g_reg_app );
}

/*****************************************************************************/
//  Description : get the total number of the window id
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
MMI_WIN_ID_T MMI_GetWinIdNumber(
                                   void
                                  )
{
    return 0xffffffff;//return TOTAL_WINDOW_ID_NUMBER;
}

/*****************************************************************************/
//  Description : get the total number of the control id
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
MMI_CTRL_ID_T MMI_GetCtrlIdNumber(
                                    void
                                   )
{
    return 0xffffffff;//TOTAL_CONTROL_ID_NUMBER;
}

//drm dummy

#if DRM_API_DUMMY

/*****************************************************************************/
// Description: Is it a DRM file
// Global resource dependence : 
// Author:
// Return: TRUE: it is,FALSE: it is not
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN DRM_IsDRMFile ( SFS_HANDLE handle )     // [IN]
{
    return FALSE;
}

/*****************************************************************************/
// Description: Get DRM context file type
// Global resource dependence : 
// Author:
// Return: 
// Note: 
/*****************************************************************************/
PUBLIC DRM_METHOD_LEVEL_E DRM_GetDRMFileType (SFS_HANDLE handle)  // [IN]
{
    return DRM_LEVEL_SD;
}

/*****************************************************************************/
// Description: Get DRM rights
// Global resource dependence : 
// Author:
// Return: 
// SFS_ERROR_NONE:get rights success
// Other: failed reason
// Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E DRM_GetRightsInfo( 
                                    SFS_HANDLE handle,                  // [IN]
                                    DRM_RIGHTS_T* rights_ptr         // [OUT]
                                    )
{
    //test
    if (PNULL != rights_ptr)
    {
        rights_ptr->mime_type = MIME_TYPE_TEXT_PLAIN;//MIME_TYPE_IMAGE_GIF;
    }

    return SFS_NO_ERROR;
}

/*****************************************************************************/
// Description: install DRM file
// Global resource dependence : 
// Author: 
// Return: DRM_DH_SUCCESS: Success
// Other: failed reason
// Note: 
/*****************************************************************************/
PUBLIC int32 DRM_InstallFileReq(const DRM_INSTALL_FILE_T *file_install_ptr )     // [IN] 
{
    return 0;
}

/*************************************************************************/
// Description: Is the corresponding rights expired?
// Global resource dependence : 
// Author: 
// Return: TRUE: Expired, FALSE: not expired
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN DRM_IsRightsExpired( 
                                SFS_HANDLE handle,   // [IN]
                                uint32 permission_flag  // [IN]
                                )
{
    return FALSE;
}

/****************************************************************************/
// Description: Is the corresponding rights expired?
// Global resource dependence : 
// Author: 
// Return: TRUE: Expired, FALSE: not expired
// Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E DRM_DownloadRightsFile (SFS_HANDLE handle) //[IN]
{
    return SFS_NO_ERROR;
}

/*****************************************************************************/
// Description: Init DRM agent callback and path info
// Global resource dependence : 
// Author: 
// Return:
// Note: 
/*****************************************************************************/
PUBLIC void DRM_InitAgent( const DRM_INIT_AGENT_T *init_agent_ptr ) //[IN]
{
}


/*****************************************************************************/
// Description: deInit DRM agent callback and path info
// Global resource dependence : 
// Author: 
// Return:
// Note: 
/*****************************************************************************/
PUBLIC void DRM_DeInitAgent(void)
{
}

/*****************************************************************************/
// Description: Init DRM agent MGR task
// Global resource dependence : 
// Author: 
// Return: TRUE: init success, FALSE: init failed
// Note: Before start download handler task and IO task, init agent first
/*****************************************************************************/
PUBLIC BOOLEAN DRM_InitDrmTaskMGR( void )
{
    return TRUE;
}

/*****************************************************************************/
// Description: Start DRM download handle task
// Global resource dependence : 
// Author: 
// Return: 
//      TRUE: success
//      FALSE: failed
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN DRM_StartDLHandlerTask( void )
{
    return TRUE;
}

/*****************************************************************************/
// Description: Start DRM IO task
// Global resource dependence : 
// Author: 
// Return: 
//      TRUE: start success
//      FALSE: start failed    
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN DRM_StartIOTask( void )
{
    return TRUE;
}

/*****************************************************************************/
// Description: Init DRM agent
// Global resource dependence : 
// Author: 
// Return: 
//      TRUE: init success
//      FALSE: init failed
// Note: 
//      Before start download handler task and IO task, init agent first
/*****************************************************************************/
PUBLIC BOOLEAN DRM_InitDrmAgent( void )
{
    return TRUE;
}

/*****************************************************************************/
// Description: Create file
// Author:
// Return: 
// Handler: file handler, DRM_INVALID_HANDLE: failed
// Note: 
/*****************************************************************************/
PUBLIC SFS_HANDLE DRM_CreateFile(
                                    const uint16* file_name_ptr,    // [IN] unicode file name
                                    uint32 access_mode,             // [IN] access mode, refer to SFS_MODE_E
                                    uint32 share_mode,              // [IN] reserved
                                    uint32 file_attribute         // [IN] reserved
                                    )
{
    return SFS_CreateFileInternal(file_name_ptr, access_mode,share_mode,file_attribute);
}

#define FATIN
#define FATOUT

SFS_ERROR_E DRM_CreateFileAsyn(
	FATIN const uint16* pathName,
	FATIN const uint16* fileName,
	FATIN uint32 access_mode,
	FATIN uint32 share_mode,	//Must be NULL,File System have not realize it 
	FATIN uint32 file_attri,	//Must be NULL,File System have not realize it 
	FATOUT SFS_HANDLE* handle,
	FATIN SFS_OVERLAPPED_T *overlapped_ptr
)
{
	return SFS_CreateFileAsynInternal(pathName,fileName,access_mode,share_mode,	file_attri,handle,overlapped_ptr);
}

/*****************************************************************************/
// Description: Read file content
// Author:
// Return: SFS_ERROR_NONE: read success, Other: failed reason
// Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E DRM_ReadFile(
                                    SFS_HANDLE handle,                  // [IN] 
                                    void *buffer,                       // [OUT]
                                    uint32 bytes_to_read,               // [IN] 
                                    uint32 *bytes_readed,               // [OUT]
                                    SFS_OVERLAPPED_T *overlapped_ptr   // [IN] 
                                    )
{
    return SFS_ReadFileInternal(handle,buffer,bytes_to_read,bytes_readed,overlapped_ptr);
}

/*****************************************************************************/
// Description: Write file content
// Author:
// Return: 
// SFS_ERROR_NONE: write success, Other: failed reason
// Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E DRM_WriteFile(
                                    SFS_HANDLE handle,                  // [IN] 
                                    const void *buffer_ptr,             // [OUT] 
                                    uint32 bytes_to_write,              // [IN] 
                                    uint32 *bytes_written,              // [OUT] 
                                    SFS_OVERLAPPED_T *overlapped_ptr    // [IN] 
                                    )
{
    return SFS_WriteFileInternal(handle,buffer_ptr,bytes_to_write,bytes_written,overlapped_ptr);
}

/*****************************************************************************/
// Description: Close file
// Author:
// Return: SFS_ERROR_NONE: write success, Other: failed reason
// Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E DRM_CloseFile( SFS_HANDLE handle)   // [IN] 
{
    return SFS_CloseFileInternal(handle);
}


/*****************************************************************************/
// Description: delete file
// Author:
// Return: SFS_ERROR_NONE: write success, Other: failed reason
// Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E DRM_DeleteFile(
                                    const uint16 *name, // [IN] 
                                    SFS_OVERLAPPED_T *overlapped_ptr    // [IN] 
                                    )
{
    return SFS_DeleteFileInternal(name,overlapped_ptr);
}


/*****************************************************************************/
// Description: Set file pointer
// Author: 
// Return: SFS_ERROR_NONE: write success, Other: failed reason
// Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E DRM_SetFilePointer(
                                    SFS_HANDLE handle,  // [IN] 
                                    __int64 offset,       // [IN] 
                                    uint32 origin    // [IN]     
                                    )
{
    return SFS_SetFilePointerInternal(handle,offset,origin );
}

/*****************************************************************************/
// Description: Get file pointer
// Author:
// Return: SFS_ERROR_NONE: write success, Other: failed reason
// Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E DRM_GetFilePointer(
                                    SFS_HANDLE handle,      // [IN]
                                    uint32 origin,          // [IN]
                                    int32 *current_pos    // [OUT]
                                    )
{
    return SFS_GetFilePointerInternal(handle,origin,current_pos);
}

/*****************************************************************************/
// Description: Set file size
// Author:
// Return: 
// SFS_ERROR_NONE: write success, Other: failed reason
// Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E DRM_SetFileSize(
                                    SFS_HANDLE handle,  // [IN]
                                    uint32 size       // [IN]
                                    )
{
    return SFS_SetFileSizeInternal(handle, size);
}

/*****************************************************************************/
// Description: Get file size
// Author:
// Return: SFS_ERROR_NONE: write success, Other: failed reason
// Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E DRM_GetFileSize(
                                    SFS_HANDLE handle,  // [IN]
                                    uint32 *size     // [OUT]
                                    )
{
    return SFS_GetFileSizeInternal(handle, size);
}

#endif

