
/*****************************************************************************
** File Name:      mmi_baidu_main.c                                         *
** Author:                                                                  *
** Date:                                                                     *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
                                *
******************************************************************************/

#include "os_api.h"
#include "priority_app.h"
#include "mmibaidu_export.h"
#include "tasks_id.h"
#include "in_message.h"
//#include "mmimsa_export.h"
#include "mmi_appmsg.h"
#include "mmisrvfmm_export.h"

#define   P_BAIDU_DRV_STACK_SIZE              4096    //6K
#define   P_BAIDU_DRV_QUEUE_NUM               8

static BLOCK_ID g_baidu_drv_task_id = SCI_INVALID_BLOCK_ID;

uint32 g_current_module_id = 0;
BAIDU_DRV_TYPE_E s_baidu_drv_init_from = BAIDU_DRV_TYPE_NONE;

uint8      g_download_file_name[MMIFILE_FILE_NAME_MAX_LEN + 1]={0};
/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void Handle_download_thumbnail (void);

/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void Handle_upload_photo (void);

/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void Handle_Cancel_Task (void);

/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void BAIDUDRV_Task (uint32 argc, void * argv);

void MMISRV_BAIDUDRV_Delete_photo_handle(void *filename, uint16 filenamelen);
/*****************************************************************************/
//  Description : create camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
#ifdef WIN32
#define tx_pri_PRI_BAIDU_DRV_TASK   128
#endif
void BAIDUDRV_CreateTask (void)
{
    MMISRV_BAIDUDRV_MSG_T sig_data={0};	
	
	SCI_TRACE_LOW("[baidu_drv]Enter BAIDUDRV_CreateTask !!!!s_baidu_drv_init_from = %d " , s_baidu_drv_init_from);
		
     if(!MMIAPIBAIDU_IsAccountExist())
    {
        MMISRV_BAIDUDRV_Need_Get_Token();
    }
    MMISRV_BAIDUDRV_Load_Upload_File_array();
//	MMIAPIBAIDU_get_access_token();
	MMISRV_BAIDUDRV_PdpActive();

	if(SCI_INVALID_BLOCK_ID == g_baidu_drv_task_id)
	{
		g_baidu_drv_task_id = SCI_CreateAppThread (
			"T_P_APP_BAIDUDRV_TASK",
			"Q_P_APP_BAIDUDRV_TASK",
			BAIDUDRV_Task,
			0,
			0,
			1024*20,
			100,
			PRI_BAIDU_DRV_TASK,
			SCI_PREEMPT,
			SCI_AUTO_START);

    		SCI_TRACE_LOW("BAIDUDRV_CreateTask, g_baidu_drv_task_id = %d" , g_baidu_drv_task_id);
	}

#ifdef WIN32
	sig_data.init_type = s_baidu_drv_init_from;
	MMISRV_BAIDUDRV_SendSignalToMMI(MSG_BAIDU_DRV_INIT_CNF , &sig_data);	
#endif

}


/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void BAIDUDRV_Task (
    uint32 argc,
    void * argv
)
{
    xSignalHeaderRec    *psig = NULL;
    BAIDU_DRV_DELETE_PHOTO_SIG_T  *del_ptr;
    BAIDU_DRV_DOWNLOAD_PHOTO_SIG_T  *ptr;

    while (1) /*lint !e716*/
    {
        //MMI_RECEIVE_SIGNAL ( psig, g_baidu_drv_task_id);/*lint !e63*/
        psig = (xSignalHeaderRec *)SCI_GetSignal(g_baidu_drv_task_id);
       if(PNULL == psig){
            SCI_TRACE_LOW ("[baidu] BAIDUDRV_Task psig=null");/*lint !e613*/
            break;
        }        //SCI_TRACE_LOW ("[MMISRV CARMERA ROLL]BAIDUDRV_Task psig->SignalCode = 0x%x", psig->SignalCode);/*lint !e613*/

	SCI_TRACE_LOW ("[baidu]BAIDUDRV_Task psig->SignalCode = 0x%x", psig->SignalCode);
		
        switch (psig->SignalCode) /*lint !e613*/
        {
        case BAIDU_DRV_DOWNLOAD_THUMBNAIL:
			Handle_download_thumbnail();
			break;
		case BAIDU_DRV_UPLOAD_PHOTO:
			Handle_upload_photo();
			break;
             case BAIDU_DRV_DELETE_PHOTO:
                     del_ptr = (BAIDU_DRV_DELETE_PHOTO_SIG_T *)psig;
                     MMISRV_BAIDUDRV_Delete_photo_handle(del_ptr->filename_ptr, del_ptr->filename_len);
                     break;
	      //case BAIDU_DRV_DELETE_CLOAD_PHOTO:
		//  	 MMISRV_BAIDUDRV_Delete_CloadPhoto();
		//  	break;
              case BAIDU_DRV_DOWNLOAD_PHOTO:
                     ptr = (BAIDU_DRV_DOWNLOAD_PHOTO_SIG_T *)psig;
                     MMISRV_BAIDUDRV_Download_Original_photo(g_download_file_name, ptr->filename_len);
                     break;
		case BAIDU_DRV_CANCEL:
			Handle_Cancel_Task();
			break;
		case HTTP_SIG_INIT_CNF:
		case HTTP_SIG_NEED_AUTH_IND:
		case HTTP_SIG_REQUEST_ID_IND:
		case HTTP_SIG_GET_CNF:
		case HTTP_SIG_HEAD_CNF:
		case HTTP_SIG_DATA_IND:
		case HTTP_SIG_HEADER_IND:
		case HTTP_SIG_ERROR_IND:
		case HTTP_SIG_CLOSE_CNF:
		case HTTP_SIG_CANCEL_CNF:
		case HTTP_SIG_NET_PROG_IND:
		case HTTP_SIG_REDIRECT_IND:
		case HTTP_SIG_POST_CNF:
			MMISRV_BAIDUDRV_HandleHTTPMsg(PNULL , psig->SignalCode , (DPARAM)psig);
	              SCI_TRACE_LOW ("[baidu_drv] BAIDUDRV_Task: HTTP SigCode !!");
			break;
        default:
            SCI_TRACE_LOW ("[baidu_drv] BAIDUDRV_Task: SigCode unknown");
            break;
        }
        SCI_TRACE_LOW ("[baidu_drv] BAIDUDRV_Task: handle signal done , free Memory!");
        MMI_FREE_SIGNAL (psig);
    }
}

/**************************************************************************************/
// Description : delete camera roll thread
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
BOOLEAN MMISRV_BAIDUDRV_CreateTask(uint32 module_id , BAIDU_DRV_TYPE_E type)
{
	g_current_module_id =  module_id;
	s_baidu_drv_init_from = type;
	
	MMISRV_BAIDUDRV_CreateDir();
	BAIDUDRV_CreateTask();
	return TRUE;
}

/**************************************************************************************/
// Description : delete camera roll thread
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_DeleteBaiduDrvTaskThread(void)
{
   if(SCI_INVALID_BLOCK_ID != g_baidu_drv_task_id)
   {
	SCI_TRACE_LOW("[baidu_drv]: Enter MMISRV_DeleteBaiduDrvTaskThread !!!!");  
	//SCI_DeleteThread(g_baidu_drv_task_id);
	//g_baidu_drv_task_id = SCI_INVALID_BLOCK_ID;//0 zyj
	MMISRV_BAIDUDRV_Destroy();
   }

   return SCI_SUCCESS;
}


/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void Handle_download_thumbnail (void)
{
	MMISRV_BAIDUDRV_get_categorylist_info();
}

/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void Handle_upload_photo (void)
{
	//Get upload linking list
	//Start upload
	MMISRV_BAIDUDRV_start_upload_file();
}

/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void Handle_Cancel_Task (void)
{

}

void MMISRV_BAIDUDRV_Delete_photo_handle(void *filename, uint16 filenamelen)
{
    MMISRV_BAIDUDRV_Delete_original_Photo((uint8 *) filename,1);
    MMISRV_BAIDUDRV_Record_Delete_Photo((uint8 *) filename);
    SCI_FREE(filename);
}
BOOLEAN MMISRV_BAIDUDRV_Delete_CloadPhoto_note()
{
    MMISRV_BAIDUDRV_Delete_CloadPhoto();
    return TRUE;
}

BOOLEAN MMISRV_BAIDUDRV_Delete_Photo_note(uint8 *filename, uint16 filename_len)
{

	MMISRV_BAIDUDRV_Delete_photo_handle(filename, filename_len);

       return TRUE;	
}

BOOLEAN MMISRV_BAIDUDRV_Download_Photo(uint8 *filename, uint16 filename_len)
{

	return MMISRV_BAIDUDRV_Download_Original_photo(g_download_file_name, filename_len);
	
}
/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
BOOLEAN MMISRV_BAIDUDRV_Download_Thumbnail(void)
{

	MMISRV_BAIDUDRV_get_categorylist_info();
       return TRUE;
}


/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
BOOLEAN MMISRV_BAIDUDRV_Upload_photo(void)
{
	BAIDU_DRV_UPLOAD_PHOTO_SIG_T*sig_ptr = PNULL;

	MMI_CREATE_SIGNAL (sig_ptr,		
	               BAIDU_DRV_UPLOAD_PHOTO,
	               sizeof (BAIDU_DRV_UPLOAD_PHOTO_SIG_T),
	               P_APP
	              );

	MMI_SEND_SIGNAL (sig_ptr, g_baidu_drv_task_id);
	return TRUE;
}

/*****************************************************************************/
//  Description : init multim folder
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void InitBaiduDrvFolder (
    const wchar   *device_ptr,
    uint16  device_len,
    const wchar   *dir_ptr,
    uint16  dir_len
)
{
    uint16  full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16  full_path_len = MMIFILE_FULL_PATH_MAX_LEN + 2;

    if ( (PNULL == device_ptr) || (0 == device_len) || (PNULL == dir_ptr) || (0 == dir_len))
    {        
        SCI_TRACE_LOW ("InitBaiduDrvFolder InitMultimFolder param error!");
        return;
    }

    if (MMIAPIFMM_CombineFullPath (device_ptr, device_len,
                                   dir_ptr, dir_len,
                                   NULL, 0,
                                   full_path_name, &full_path_len))
    {
        if (!MMIAPIFMM_IsFolderExist (full_path_name, full_path_len))
        {
            MMIAPIFMM_CreateDir (full_path_name, full_path_len);
        }
    }
}


/*****************************************************************************/
//  Description : Create Baidu Dir
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMISRV_BAIDUDRV_CreateDir(void)
{
    MMIFILE_ERROR_E     error = SFS_ERROR_NONE;
    BOOLEAN             isCreateDir = FALSE;
    wchar              full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16              full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;

    MMIFILE_DEVICE_E fmm_dev = MMI_DEVICE_UDISK;
    //create default dir
    // D:/Photos, D:/Audio, D:/Video
    do
    {
        if (MMIAPIFMM_GetDeviceTypeStatus (fmm_dev))
        {


            InitBaiduDrvFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                            MMIMULTIM_DIR_PIC_BAIDUDRV_THUMBNAIL, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PIC_BAIDUDRV_THUMBNAIL));

            InitBaiduDrvFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                            MMIMULTIM_DIR_PIC_BAIDUDRV_DOWNLOAD, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PIC_BAIDUDRV_DOWNLOAD));

            InitBaiduDrvFolder (MMIAPIFMM_GetDevicePath (fmm_dev), MMIAPIFMM_GetDevicePathLen (fmm_dev),
                            MMIMULTIM_DIR_PIC_BAIDUDRV_UPLOAD, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PIC_BAIDUDRV_UPLOAD));
        }
    }
    while (++fmm_dev < MMI_DEVICE_NUM);
       
   { 
        if(MMIAPIFMM_IsFolderExist(full_path_name,  full_path_len))
        {
            return SFS_ERROR_HAS_EXIST;
        }
	   isCreateDir = MMIAPIFMM_CreateDir(full_path_name, full_path_len);
	    
	    if (isCreateDir)
		{
	        error = SFS_ERROR_NONE;
		}
	    else
		{
	        error = SFS_ERROR_HAS_EXIST;
		}
    }
    return error;
}



