#ifdef DYNAMIC_MODULE_SUPPORT
/****************************************************************************
** File Name:      mmk_shell.c                                             *
** Author:         James.Zhang                                             *
** Date:           05/07/2008                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2008        James.Zhang       Create
** 
****************************************************************************/
#define MMK_SHELL_C


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "cafshell.h"
#include "cafimage.h"
#include "mmk_app.h"

#include "mmi_theme.h"
#include "guicommon.h"
#include "mmk_modinfo.h"
#include "mmk_module_manager.h"
#include "mmk_timer.h"
#include "mmk_shell.h"
#include "mmi_textfun.h"
#include "mmi_imagefun.h"
#include "mmk_msg_internal.h"
#include "mmk_handle_internal.h"
#include "sfs.h"
#include "caf.h"
#include "app_tcp_if.h"
#include "cafmedia.h"
#include "gui_ucs2b_converter.h"
#include "mmk_authen.h"
#include "mmi_default.h"

#define CRC_TABLE_SIZE 256
//LOCAL uint32 *s_Crc32Table= 0;

// 根据全文件名获取路径名
extern BOOLEAN MMIAPICOM_GetFilePath(
                                     const uint16 *file_name_ptr, // [IN]
                                     uint16 file_name_len,  // [IN]
                                     uint16 *file_path_ptr, // [OUT]
                                     uint16 *file_path_len  // [OUT]
                                     );
/*****************************************************************************/
//  Description : 计算一块内存的CRC值
//  Global resource dependence : 
//  Author:peng
//  Note: 
/*****************************************************************************/
LOCAL CAF_RESULT_E  IsValidApp(CAF_GUID_T guid);
/*****************************************************************************/
//  Description : 小程序列表主菜单处理函数
//  Global resource dependence : 
//  Author:peng
//  Note: 
/*****************************************************************************/
LOCAL CAF_RESULT_E  IsFileExist(CAF_GUID_T guid);

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/

LOCAL CAF_RESULT_E GetSysDate( CAF_DATE_T* date_ptr ) 
{
    SCI_DATE_T date = {0};

    SCI_ASSERT( PNULL != date_ptr );/*assert verified*/

    TM_GetSysDate( &date );

    date_ptr->mday = date.mday;
    date_ptr->mon  = date.mon;
    date_ptr->year = date.year;

    return CAF_RESULT_SUCCEEDED;
}

LOCAL CAF_RESULT_E GetSysTime( CAF_TIME_T* time_ptr ) 
{
    SCI_TIME_T time = {0};

    SCI_ASSERT( PNULL != time_ptr );/*assert verified*/

    TM_GetSysTime( &time );
    
    time_ptr->hour = time.hour;
    time_ptr->min  = time.min;
    time_ptr->sec  = time.sec;

    return CAF_RESULT_SUCCEEDED;
}

LOCAL BOOLEAN GetTpPoint( DPARAM param, GUI_POINT_T* point_ptr ) 
{
    SCI_ASSERT( PNULL != param && PNULL != point_ptr );/*assert verified*/

    point_ptr->x = MMK_GET_TP_X(param);
    point_ptr->y = MMK_GET_TP_Y(param);

   return TRUE;
}

LOCAL BOOLEAN PointIsInRect( const GUI_POINT_T* point_ptr, const GUI_RECT_T* rect_ptr ) 
{
    SCI_ASSERT( PNULL != rect_ptr && PNULL != point_ptr );/*assert verified*/

    return GUI_PointIsInRect( *point_ptr, *rect_ptr );
}




/* for test dynamic module */
const CAF_SHELL_T g_caf_shell_vtbl = 
{
    ISHELL_CreateInstance,
    ISHELL_CreateWinTable,
    ISHELL_CloseWindow,
    ISHELL_SetWinSoftkeyId,
    ISHELL_GetDC,
    ISHELL_GetAppletByWin,
    ISHELL_GetCtrlByWin,
    ISHELL_SetActiveCtrl,
    GetSysDate,
    GetSysTime,
    GetTpPoint,
    PointIsInRect,
	ISHELL_LoadImage,
	ISHELL_LoadRing,  
	ISHELL_LoadAnim,  
	ISHELL_LoadResImage,
	ISHELL_LoadResText, 
	ISHELL_LoadResAnim, 
	ISHELL_LoadResRing, 
	ISHELL_LoadResData, 
    ISHELL_StartApplet,
    ISHELL_CloseApplet,
    ISHELL_PostMessage,
    ISHELL_SendMessage,
    ISHELL_CreateTimer,
    ISHELL_StartTimer,
    ISHELL_StopTimer,
    ISHELL_PauseTimer,
    ISHELL_ResumeTimer,
    ISHELL_IsTimerActive,
	ISHELL_GetScreenInfo,
	ISHELL_GetMemoryInfo,
    ISHELL_MessageBox,
    ISHELL_AllowTurnOffBackLight,
	ISHELL_CreateSemaphore,
	ISHELL_DeleteSemaphore,
	ISHELL_GetSemaphore,
	ISHELL_PutSemaphore,
	ISHELL_SetWinTitle, 
	ISHELL_SetWinTitleByID, 
    ISHELL_GetAppletInstallDirectory,
    ISHELL_IsFocusWindow,
    ISHELL_GetWinRect,
    ISHELL_CreateInstanceEx,
    ISHELL_CloseInstance,
    ISHELL_SendNotify,  
    ISHELL_PostNotify,
};

extern BOOLEAN MMIRES_GetRingInfo( MMI_RING_ID_T ring_id, CAF_HANDLE_T handle_id, void * pRingInfo );
extern void    MMIPUB_MessageBox( const CAF_MESSAGEBOX_CREATE_T* msgbox_ptr );
extern void    MMIPUB_OpenQueryWinStartApp( uint32 guid );
extern void    MMIPUB_OpenStartAppWaitWin( uint32 guid );
extern void    MMIPUB_OpenQueryWinFilesNoExist(void);
/*****************************************************************************/
//  Description :  create static instance of interface from ishell interface.
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
CAF_RESULT_E  ISHELL_CreateInstance(CAF_GUID_T guid, void ** object_pptr)
{
    return MMKMNG_CreateStaticClassInstance(guid, object_pptr);
}

/*****************************************************************************/
//  Description :  create instance of interface from ishell interface.
//  Global resource dependence : none
//  Author: Andrew.Zhang
//  Note:
/*****************************************************************************/
CAF_RESULT_E  ISHELL_CreateInstanceEx(CAF_GUID_T guid, void ** object_pptr, void * param_ptr)
{
    MMI_MODINFO_NODE_T *modinfo_node_ptr = PNULL;
    CAF_RESULT_E result = CAF_RESULT_FAILED;
    CAF_APPLET_TYPE_E   applet_type = CAF_APPLET_MAX;
    
    *object_pptr = PNULL;
    

    //先判断是否是静态类
    result = ISHELL_CreateInstance(guid, object_pptr);
    if(result == CAF_RESULT_SUCCEEDED)
    {
        return result;
    }

    modinfo_node_ptr = MMK_SearchModInfo (guid, MMI_QUERY_BY_GUID);
    
    if(modinfo_node_ptr == PNULL)
    {
        return result;
    }
    
    if(modinfo_node_ptr->mod_type == CAF_MOD_DYNAMIC) //判断是否是动态模块
    {
        if(GetAppletType(guid, &applet_type) == FALSE)
        {
            return result;
        }
        
        if(applet_type == CAF_APPLET_COMMON_DLL)
        {
            MMI_APPLET_START_T start ={0};
            MMI_HANDLE_T     applet_handle = 0;
            
            start.guid = guid;    
            //动态applet加上标记
            start.state |= MMI_APPLET_STATE_DYNAMIC;
                        
            applet_handle = MMK_StartDLL(&start, object_pptr);
            
            if(applet_handle != 0)
            {
                result = CAF_RESULT_SUCCEEDED;
            }
        }
        else if(applet_type == CAF_APPLET_CONTROL_DLL)
        {
            if(param_ptr != NULL)
            {
                MMI_CONTROL_CREATE_T control_create = {0};
                IGUICTRL_T* gui_ctrl_ptr = PNULL;
                CAF_CONTROL_PARAM_T * control_param_ptr = PNULL;
                
                control_param_ptr = (CAF_CONTROL_PARAM_T *)param_ptr;
                
                control_create.ctrl_id = control_param_ptr->ctrl_id;
                control_create.guid = guid;
                control_create.parent_win_handle = control_param_ptr->parent_handle;
                control_create.parent_ctrl_handle = 0;
                control_create.init_data_ptr = control_param_ptr->init_data_ptr;
                
                gui_ctrl_ptr = (void *)MMK_CreateControl(&control_create);
                
                if(gui_ctrl_ptr != PNULL)
                {
                    *object_pptr = (void *)gui_ctrl_ptr;

                    //设置控件基类函数
                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->QueryInterface == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->QueryInterface = GUICTRL_QueryInterface;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->AddRef == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->AddRef = GUICTRL_AddRef;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->Release == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->Release = GUICTRL_Release;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->GetCtrlGuid == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->GetCtrlGuid = GUICTRL_GetCtrlGuid;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->SetCtrlHandle == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->SetCtrlHandle = GUICTRL_SetCtrlHandle;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->GetCtrlHandle == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->GetCtrlHandle = GUICTRL_GetCtrlHandle;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->SetState == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->SetState = GUICTRL_SetState;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->GetState == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->GetState = GUICTRL_GetState;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->SetRect == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->SetRect = GUICTRL_SetRect;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->GetRect == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->GetRect = GUICTRL_GetRect;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->SetLcdDevInfo == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->SetLcdDevInfo = GUICTRL_SetLcdDevInfo;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->GetLcdDevInfo == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->GetLcdDevInfo = GUICTRL_GetLcdDevInfo;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->SetProperties == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->SetProperties = GUICTRL_SetProperties;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->GetProperties == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->GetProperties = GUICTRL_GetProperties;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->TestProperties == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->TestProperties = GUICTRL_TestProperties;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->Redraw == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->Redraw = GUICTRL_Redraw;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->SetActive == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->SetActive = GUICTRL_SetActive;
                    }

                    if(CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->IsActive == PNULL)
                    {
                        CAF_GET_VTBLPTR( gui_ctrl_ptr, IGUICTRL_T )->IsActive = GUICTRL_IsActive;
                    }

                    //初始化RECT
                    GUICTRL_SetRect(gui_ctrl_ptr, &(control_param_ptr->rect));

                    result = CAF_RESULT_SUCCEEDED;
                }
            }
        }
        else
        {
        }
    }
    else
    {
    }
    
    return result;
}

/*****************************************************************************/
//  Description :  close instance
//  Global resource dependence : none
//  Author: Andrew.Zhang
//  Note:
/*****************************************************************************/
CAF_RESULT_E  ISHELL_CloseInstance(CAF_GUID_T guid, void * object_ptr)
{
    MMI_MODINFO_NODE_T *modinfo_node_ptr = PNULL;
    CAF_RESULT_E result = CAF_RESULT_FAILED;
    CAF_APPLET_TYPE_E   applet_type = CAF_APPLET_MAX;
    
    //静态类不释放
    if(MMKMNG_IsStaticClass(guid) == TRUE)
    {
        return CAF_RESULT_SUCCEEDED;
    }

    modinfo_node_ptr = MMK_SearchModInfo (guid, MMI_QUERY_BY_GUID);
    
    if(modinfo_node_ptr == PNULL)
    {
        return result;
    }
    
    if(modinfo_node_ptr->mod_type == CAF_MOD_DYNAMIC) //判断是否是动态模块
    {
        if(GetAppletType(guid, &applet_type) == FALSE)
        {
            return result;
        }
        
        if(applet_type == CAF_APPLET_COMMON_DLL)
        {
            MMI_HANDLE_T applet_handle = 0;
            CAF_APPLET_T * caf_applet_ptr = PNULL;
            
            caf_applet_ptr = (CAF_APPLET_T *)object_ptr;
            
            if(caf_applet_ptr != PNULL)
            {
                applet_handle = caf_applet_ptr->app_handle;
            }
            
            /* check the handle validate */
            if( MMK_IsHandleValid( applet_handle ) )
            {
                if( MMK_CloseDLL(applet_handle) )
                {
                    result = CAF_RESULT_SUCCEEDED;
                }
            }
        }
        else if(applet_type == CAF_APPLET_CONTROL_DLL)
        {
            CAF_HANDLE_T ctrl_handle = 0;
            ctrl_handle = GUICTRL_GetCtrlHandle((IGUICTRL_T *)object_ptr);
            
            if(MMK_DestroyControl((MMI_HANDLE_T)ctrl_handle) == TRUE)
            {
                result = CAF_RESULT_SUCCEEDED;
            }
        }
        else
        {
        }
    }
    else //静态实例
    {
        //result = CAF_RESULT_SUCCEEDED;
    }
    
    return result;
}

//add by songliang.liu 
CAF_RESULT_E ISHELL_LoadImage( const wchar *  imagefile_ptr, IImage ** image_pptr)
{
  CAF_RESOURCE_INFO_T res_info;

  //创建文件对象实例，此时会自动调用IFILE_AddRef，使对象的计数为1
  ISHELL_CreateInstance( SPRD_GUID_IMAGE, (void**)image_pptr );

  memset((char *)&res_info, 0, sizeof(CAF_RESOURCE_INFO_T));
  res_info.resinfo.resfile_ptr = (wchar*)imagefile_ptr;
  res_info.res_source = CAF_RESOURCE_SOURCE_FILE;
  IIMAGE_SetParm((IIMAGE_T*)(*image_pptr), CAFIMAGE_PARM_IMAGE, (int)&res_info, 0);

  return CAF_RESULT_SUCCEEDED;
}

CAF_RESULT_E ISHELL_LoadRing(const wchar * ringfile_ptr, IMedia** media_pptr )
{
  CAFMediaData_T  md;
  CAF_MM_RESULT ret;
  ISHELL_CreateInstance(SPRD_GUID_MEDIA,(void**)media_pptr);
  if(*media_pptr == PNULL)
    return CAF_RESULT_FAILED;
  //设置数据
  md.clsdata=MMD_FILE_NAME;
  md.data_ptr=(void*)ringfile_ptr;
  md.dw_format=0;
  md.dw_size=0;
  md.user_ptr=PNULL;
  md.eworkmode = MMW_MODE_PLAY;
  ret=IMEDIA_SetMediaData(*media_pptr,&md);
  if(ret != CAF_MM_NOERROR)
  {
    IMEDIA_Release(*media_pptr);
    *media_pptr=PNULL;
    return CAF_RESULT_FAILED;   
  }

  return CAF_RESULT_SUCCEEDED;
}

CAF_RESULT_E ISHELL_LoadAnim(const wchar * animfile_ptr, IImage ** image_pptr)
{

  CAF_RESOURCE_INFO_T res_info;
  
  //创建文件对象实例，此时会自动调用IFILE_AddRef，使对象的计数为1
  ISHELL_CreateInstance( SPRD_GUID_IMAGE, (void**)image_pptr );
  
  memset((char *)&res_info, 0, sizeof(CAF_RESOURCE_INFO_T));
  res_info.resinfo.resfile_ptr = (wchar*)animfile_ptr;
  res_info.res_source = CAF_RESOURCE_SOURCE_FILE;
  IIMAGE_SetParm((IIMAGE_T*)(*image_pptr), CAFIMAGE_PARM_IMAGE, (int)&res_info, 0);

  return CAF_RESULT_SUCCEEDED;
}

CAF_RESULT_E ISHELL_LoadResImage( uint32 image_id,CAF_HANDLE_T applet_handle, IImage ** image_pptr)
{
  CAF_RESOURCE_INFO_T res_info;
    
  //创建文件对象实例，此时会自动调用IFILE_AddRef，使对象的计数为1
  ISHELL_CreateInstance( SPRD_GUID_IMAGE, (void**)image_pptr );
    
  memset((char *)&res_info, 0, sizeof(CAF_RESOURCE_INFO_T));
  res_info.resinfo.res_id = image_id;
  res_info.res_source = CAF_RESOURCE_SOURCE_ID;
  IIMAGE_SetParm((IIMAGE_T*)(*image_pptr), CAFIMAGE_PARM_IMAGE, (int)&res_info, 0);
  
    return CAF_RESULT_SUCCEEDED;
}
CAF_RESULT_E ISHELL_LoadResText( uint32 text_id, CAF_HANDLE_T applet_handle, CAF_STRING_T * str_ptr )
{
    uint8* ptr = (uint8*)MMITHEME_GetResText( text_id, applet_handle, str_ptr );
    /* check the handle validate */
    if(!MMK_IsHandleValid(applet_handle))
    {
        return CAF_RESULT_FAILED;
    }
    if(PNULL == ptr)
    {
        return CAF_RESULT_FAILED;
    }
    else
    {
        return CAF_RESULT_SUCCEEDED;
    }
}

CAF_RESULT_E ISHELL_LoadResAnim( uint32 anim_id, CAF_HANDLE_T applet_handle, IImage ** image_pptr )
{
  CAF_RESOURCE_INFO_T res_info;
    
  //创建文件对象实例，此时会自动调用IFILE_AddRef，使对象的计数为1
  ISHELL_CreateInstance( SPRD_GUID_IMAGE, (void**)image_pptr );
    
  memset((char *)&res_info, 0, sizeof(CAF_RESOURCE_INFO_T));
  res_info.resinfo.res_id = anim_id;
  res_info.res_source = CAF_RESOURCE_SOURCE_ID;
  IIMAGE_SetParm((IIMAGE_T*)(*image_pptr), CAFIMAGE_PARM_IMAGE, (int)&res_info, 0);
  
    return CAF_RESULT_SUCCEEDED;
}

CAF_RESULT_E ISHELL_LoadResRing( uint32 ring_id, uint32 ring_type,CAF_HANDLE_T applet_handle, IMedia** media_pptr )
{
  CAFMediaData_T  md;
  CAF_MM_RESULT ret;
  struct _AUD_RING_DATA_INFO_T
  {
    uint32    type;
    uint32     data_len;
    uint8    * data_ptr;
  } ri;
  ISHELL_CreateInstance(SPRD_GUID_MEDIA,(void**)media_pptr);
  if(*media_pptr == PNULL)
    return CAF_RESULT_FAILED;
  //获取资源数据
  if(FALSE == MMIRES_GetRingInfo(ring_id,applet_handle,&ri))
  {
    //SCI_TRACE_LOW:"[ISHELL_LoadResRing]: MMIRES_GetRingInfo  error! "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_SHELL_554_112_2_18_3_23_20_30,(uint8*)"");
    IMEDIA_Release(*media_pptr);
    *media_pptr=PNULL;
    return CAF_RESULT_FAILED;
  }

  //SCI_TRACE_LOW:"[ISHELL_LoadResRing]: ri.data_len = %d "
  SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_SHELL_560_112_2_18_3_23_20_31,(uint8*)"d", ri.data_len);
  //设置数据
  md.clsdata=MMD_BUFFER;
  md.data_ptr=(void*)ri.data_ptr;
  md.dw_format=ring_type;
  md.dw_size=ri.data_len;
  md.user_ptr=PNULL;
  md.eworkmode = MMW_MODE_PLAY;
  ret=IMEDIA_SetMediaData(*media_pptr,&md);
  if(ret != CAF_MM_NOERROR)
  {
    //SCI_TRACE_LOW:"[ISHELL_LoadResRing]: IMEDIA_SetMediaData  error! "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_SHELL_571_112_2_18_3_23_20_32,(uint8*)"");
    IMEDIA_Release(*media_pptr);
    *media_pptr=PNULL;
    return CAF_RESULT_FAILED;   
  }
  
  return CAF_RESULT_SUCCEEDED;
}

CAF_RESULT_E ISHELL_LoadResData(uint32 res_id, CAF_RESOURCE_TYPE_E type ,CAF_HANDLE_T applet_handle,void ** buffer_pptr)
{
  return CAF_RESULT_SUCCEEDED;
}

/*****************************************************************************/
//  Description : create a window in parseing window table
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T ISHELL_CreateWinTable(
                                          const CAF_WINDOW_TABLE_CREATE_T* create_ptr
                                          )
{    
    MMI_WINDOW_TABLE_CREATE_T create = {0};

    SCI_ASSERT( PNULL != create_ptr->func );/*assert verified*/

    create.add_data_ptr      = create_ptr->add_data_ptr;
    create.applet_handle     = create_ptr->applet_handle;
    create.func              = create_ptr->func;
    create.parent_win_handle = create_ptr->parent_win_handle;
    create.win_table_ptr     = create_ptr->win_table_ptr;

    return MMK_CreateWinTable( &create );
}

/*****************************************************************************/
//  Description : close a window 
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_CloseWindow(
                                  MMI_HANDLE_T win_handle
                                  )
{
    /* check the handle validate */
    if(!MMK_IsHandleValid(win_handle))
    {
        return FALSE;
    }
    return MMK_CloseWin(win_handle);
}

/*****************************************************************************/
//  Description : set window softkey id
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_SetWinSoftkeyId( 
                                      MMI_HANDLE_T  win_handle,
                                      MMI_TEXT_ID_T leftsoft_id,
                                      MMI_TEXT_ID_T middlesoft_id,
                                      MMI_TEXT_ID_T rightsoft_id,
                                      BOOLEAN       is_need_update
                                      )
{
    return GUIWIN_SetSoftkeyTextId( win_handle, leftsoft_id, middlesoft_id, rightsoft_id, is_need_update );
}

/*****************************************************************************/
//  Description : get dc
//  Global resource dependence : none
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC IDC_T* ISHELL_GetDC( 
                           MMI_HANDLE_T  handle
                           )
{
    if( !MMK_IsHandleValid( handle ) )
    {
        return PNULL;
    }

    return MMK_GetDC( handle );
}

/*****************************************************************************/
//  Description : get applet by win
//  Global resource dependence : none
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC IAPPLET_T* ISHELL_GetAppletByWin(
                                        MMI_HANDLE_T win_handle
                                        )
{
    if( !MMK_IsHandleValid( win_handle ) )
    {
        return PNULL;
    }
    
    return MMK_GetAppletPtrByWin( win_handle );
}

/*****************************************************************************/
//  Description : get ctrl by win
//  Global resource dependence : none
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC IGUICTRL_T* ISHELL_GetCtrlByWin(
                                       MMI_HANDLE_T win_handle,
                                       MMI_CTRL_ID_T ctrl_id
                                       )
{
    if( !MMK_IsHandleValid( win_handle ) )
    {
        return PNULL;
    }
    
    return MMK_GetCtrlPtrByWin( win_handle, ctrl_id );
}

/*****************************************************************************/
//  Description : set the active control 
//  Global resource dependence : 
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_SetActiveCtrl(
                                    MMI_HANDLE_T  ctrl_handle // the control handle
                                    )
{
    if( !MMK_IsHandleValid( ctrl_handle ) )
    {
        return PNULL;
    }

    //此处判断zorder是否dirty设置是否需要刷新,或增加是否需要刷新的参数
    return MMK_SetActiveCtrl( ctrl_handle, FALSE );
}

/*****************************************************************************/
//  Description : start an applet by guid
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC CAF_RESULT_E ISHELL_StartApplet(
                                       const CAF_APPLET_START_T* start_ptr
                                       )
{
    MMI_MODINFO_NODE_T* mod_info_ptr  = PNULL;

    mod_info_ptr = MMK_SearchModInfo(start_ptr->guid, MMI_QUERY_BY_GUID);

    if(PNULL != mod_info_ptr)
    {    
    CAF_RESULT_E isValid;
    
    /*check files are exist*/
    isValid = IsFileExist(start_ptr->guid);

    if ( CAF_RESULT_SUCCEEDED != isValid )
    {
      /*弹出告警窗口*/
      MMIPUB_OpenQueryWinFilesNoExist();

      return 0;
    }

    isValid = IsValidApp( start_ptr->guid );

    if( CAF_RESULT_SUCCEEDED == isValid )
    {     
      MMIPUB_OpenStartAppWaitWin( start_ptr->guid );      
    }
    else
    {               
      MMIPUB_OpenQueryWinStartApp( start_ptr->guid );
    }
    }

    return 0;   
}


/*****************************************************************************/
//  Description : close an applet 
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC CAF_RESULT_E ISHELL_CloseApplet(MMI_HANDLE_T applet_handle)
{    
    CAF_RESULT_E result = CAF_RESULT_FAILED;

    /* check the handle validate */
    if( MMK_IsHandleValid( applet_handle ) )
    {
        if( MMK_CloseApplet(applet_handle) )
        {
            result = CAF_RESULT_SUCCEEDED;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : post message
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_PostMessage(
                        MMI_HANDLE_T      handle,         // the handle
                        MMI_MESSAGE_ID_E    msg_id,         //the identify of message
                        DPARAM              param_ptr,      //the param of the message
                        uint32              size_of_param   //the size of the param
                        )
{    
    /* check the handle validate */
    if(!MMK_IsHandleValid(handle))
    {
        return FALSE;
    }
    return MMK_PostMsg(handle, msg_id, param_ptr, size_of_param);
}

/*****************************************************************************/
//  Description : send message
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_SendMessage(
                        MMI_HANDLE_T      handle,         // the handle
                        MMI_MESSAGE_ID_E    msg_id,         //the identify of message
                        DPARAM              param_ptr       //the param of the message                        
                        )
{    
    /* check the handle validate */
    if(!MMK_IsHandleValid(handle))
    {
        return FALSE;
    }
    return MMK_SendMsg(handle, msg_id, param_ptr);
}

/*****************************************************************************/
//  Description : create a timer
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC uint8 ISHELL_CreateTimer(MMI_HANDLE_T handle, uint32 time_out, BOOLEAN is_period)
{
    /* check the handle validate */
    if(!MMK_IsHandleValid(handle))
    {
        return 0;
    }
    return MMK_CreateWinTimer(handle, time_out, is_period);
}

/*****************************************************************************/
//  Description : start a timer
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_StartTimer(MMI_HANDLE_T handle, uint8 timer_id, uint32 time_out, BOOLEAN is_period)
{
    /* check the handle validate */
    if(!MMK_IsHandleValid(handle))
    {
        return FALSE;
    }
    return MMK_StartWinTimer(handle, timer_id, time_out, is_period);
}

/*****************************************************************************/
//  Description : stop a timer
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_StopTimer(uint8 timer_id)
{    
    return MMK_StopTimer(timer_id);
}

/*****************************************************************************/
//  Description : pause a timer
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_PauseTimer(uint8 timer_id)
{
    return MMK_PauseTimer(timer_id);
}

/*****************************************************************************/
//  Description : resume a timer
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_ResumeTimer(uint8 timer_id)
{
    return MMK_ResumeTimer(timer_id);
}

/*****************************************************************************/
//  Description : resume a timer
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_IsTimerActive(uint8 timer_id)
{
    return MMK_IsTimerActive(timer_id);
}


void ISHELL_GetScreenInfo( CAF_SCREENINFO_T* info_ptr ) 
{
  SCI_ASSERT( PNULL != info_ptr );/*assert verified*/
    
  GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&info_ptr->width,&info_ptr->height);
}


//add by songliang.liu for MMIFILE_DEVICE_UDISK
//don't include mmi head file here!!!
//#include "mmifmm_export.h"
CAF_RESULT_E ISHELL_GetMemoryInfo( CAF_MEMORYINFO_T* info_ptr,CAF_MEMORY_TYPE_E memory_type)
{
    
    uint32 free_space = 0;
    uint32 free_high_space = 0;
    
    switch(memory_type)
    {
    case CAF_MEMORY_SIM1:
        return CAF_RESULT_NOT_SUPPORT;    
        
    case CAF_MEMORY_SIM2:
        return CAF_RESULT_NOT_SUPPORT;    
        
    case CAF_MEMORY_C:
        return CAF_RESULT_NOT_SUPPORT;
        
    case CAF_MEMORY_D:
        if(SFS_ERROR_NONE == SFS_GetDeviceFreeSpace( L"D", &free_high_space,&free_space ))
        {
            info_ptr->unused_high = free_high_space;
            info_ptr->unused_low = free_space;
        }
        else
        {
            return CAF_RESULT_NOT_SUPPORT;
        }
        if(SFS_ERROR_NONE == SFS_GetDeviceUsedSpace( L"D",  &free_high_space,&free_space))
        {
            info_ptr->used_high = free_high_space;
            info_ptr->used_low = free_space;
        }
        else
        {
            return CAF_RESULT_NOT_SUPPORT;
        }
        info_ptr->memory_status = CAF_MEMORY_OK;
        info_ptr->memory_type = (uint16) memory_type;
        return CAF_RESULT_SUCCEEDED;
        
    case CAF_MEMORY_E:
        
        if(SFS_ERROR_NONE == SFS_GetDeviceFreeSpace( L"E", &free_high_space, &free_space ))
        {
            info_ptr->unused_high = free_high_space;
            info_ptr->unused_low = free_space;
        }
        else
        {
            return CAF_RESULT_NOT_SUPPORT;
        }
        
        if(SFS_ERROR_NONE == SFS_GetDeviceUsedSpace( L"C", &free_high_space,&free_space ))
        {
            info_ptr->used_high = free_high_space;
            info_ptr->used_low = free_space;
        }
        else
        {
            return CAF_RESULT_NOT_SUPPORT;
        }
        info_ptr->memory_status = CAF_MEMORY_OK;
        info_ptr->memory_type = (uint16)memory_type;
        return CAF_RESULT_SUCCEEDED;    
        
        
    case CAF_MEMORY_F:
        return CAF_RESULT_NOT_SUPPORT;
        
    case CAF_MEMORY_G:
        return CAF_RESULT_NOT_SUPPORT;
        
    case CAF_MEMORY_H:
        return CAF_RESULT_NOT_SUPPORT;
    }
    
    return CAF_RESULT_SUCCEEDED;
}

PUBLIC  CAF_SEMAPHORE_PTR ISHELL_CreateSemaphore( //if successful, return semaphore
                             //pointer, else return SCI_NULL
    const char *name_ptr, //name of the semaphore
    uint32 initial_count //initial value of semaphore counter
  )
{
  return  SCI_CreateSemaphore(name_ptr,initial_count);
}

PUBLIC  uint32 ISHELL_DeleteSemaphore ( //if successful return SCI_SUCCESS,
                         //else return SCI_ERROR
    CAF_SEMAPHORE_PTR sem_ptr //semaphore pointer
  )

{
  return  SCI_DeleteSemaphore(sem_ptr);

}

PUBLIC  uint32 ISHELL_GetSemaphore (//if successful return SCI_SUCCESS,
                                     //else return SCI_ERROR
    CAF_SEMAPHORE_PTR sem_ptr,//semaphore pointer
    uint32 wait_option       //wait option: SCI_WAIT_FOREVER, SCI_NO_WAIT,
                             //             1~0x0FFFFFFFE wait time(ms)
)
{
  return  SCI_GetSemaphore(sem_ptr,wait_option);
}



PUBLIC  uint32 ISHELL_PutSemaphore ( //if successful return SCI_SUCCESS,
                //else return SCI_ERROR
    CAF_SEMAPHORE_PTR sem_ptr //semaphore pointer
  )
{
  return  SCI_PutSemaphore(sem_ptr);
}

/*****************************************************************************/
//  Description : get device info
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
LOCAL CAF_RESULT_E  IsFileExist(CAF_GUID_T guid)
{
  CAF_RESULT_E isLoaded = CAF_RESULT_FAILED;
  MMI_MODINFO_NODE_T* mod_info_ptr   = PNULL;
  
  /*查找mod info*/
  mod_info_ptr = MMK_SearchModInfo( guid, MMI_QUERY_BY_GUID );
  SCI_ASSERT( PNULL != mod_info_ptr );/*assert verified*/

    //加载前, 校验版本是否有效
    if( ( mod_info_ptr->module_info.flag & CAF_MOD_FLAG_INVALID_VER ) == CAF_MOD_FLAG_INVALID_VER )
    {           
        return CAF_RESULT_INVALID_VERSION;
    }

  isLoaded = MMKMNG_SearchLoadedModule( mod_info_ptr->mod_id );

  if(CAF_RESULT_SUCCEEDED == isLoaded)
  {
    //the Mod has been loaded
    return CAF_RESULT_SUCCEEDED;
  }
  /*尚未加载，则察看文件是否完全*/
  else
  {
    SFS_HANDLE f_handle = SFS_CreateFile(mod_info_ptr->module_info.dll_file_name,
                        SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
    if (NULL == f_handle)
    {

      return CAF_RESULT_FAILED;
    }
    SFS_CloseFile(f_handle);

    f_handle = SFS_CreateFile(mod_info_ptr->module_info.res_file_name,
                        SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
    if (NULL == f_handle)
    {
      return CAF_RESULT_FAILED;
    }
    SFS_CloseFile(f_handle);
  }
  return CAF_RESULT_SUCCEEDED;
}

/*****************************************************************************/
//  Description : message box
//  Global resource dependence : none
//  Author: james.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void ISHELL_MessageBox( const CAF_MESSAGEBOX_CREATE_T* msgbox_ptr )
{
    MMIPUB_MessageBox( msgbox_ptr );
}

/*****************************************************************************/
//  Description : allow turn off back light
//  Global resource dependence : none
//  Author: james.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void ISHELL_AllowTurnOffBackLight( BOOLEAN allow_turn_off ) 
{
    MMIDEFAULT_AllowTurnOffBackLight( allow_turn_off );
}

/*****************************************************************************/
//  Description : 小程序列表主菜单处理函数
//  Global resource dependence : 
//  Author:peng
//  Modified:  wei.zhou
//  Note:      add sign function
/*****************************************************************************/
LOCAL CAF_RESULT_E  IsValidApp(CAF_GUID_T guid)
{
  CAF_RESULT_E isLoaded = CAF_RESULT_FAILED;
  MMI_MODINFO_NODE_T* mod_info_ptr   = PNULL;
  
  /*查找mod info*/
  mod_info_ptr = MMK_SearchModInfo( guid, MMI_QUERY_BY_GUID );

  SCI_ASSERT( PNULL != mod_info_ptr );/*assert verified*/
  /*在已经加载的mod中查找*/
  isLoaded = MMKMNG_SearchLoadedModule( mod_info_ptr->mod_id );
  if(CAF_RESULT_SUCCEEDED == isLoaded)
  {
    //the Mod has been loaded
    return CAF_RESULT_SUCCEEDED;
  }
  /*尚未加载，则要计算最后SFS_SetFilePointer个字节的CRC*/
  else
  {
    if(MMK_VerifySignFile(mod_info_ptr->module_info.dll_file_name)==TRUE)
    {
        return CAF_RESULT_SUCCEEDED;
    }
    
    #if  0
    uint32* crc_table_ptr = PNULL;
    uint32 size = 0;
    uint32 crc_data = 0;
    uint32 read_size = 0;
    uint8 *pCrcBuff = NULL;
    uint32 file_crc = 0;
    SFS_HANDLE f_handle = SFS_CreateFile(mod_info_ptr->module_info.dll_file_name,
                        SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);
    SCI_ASSERT(0 != f_handle);/*assert verified*/
    SFS_GetFileSize(f_handle, &size);
    if(size > MMK_DYN_MODULE_FILE_SIZE_MAX || size < DYNAMIC_FILE_CRC_NUM)
    {
        //SCI_TRACE_LOW:"[MOD MNG]: error! module code size is 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_SHELL_1106_112_2_18_3_23_21_33,(uint8*)"d", size);
        return CAF_RESULT_MODU_SIZE_ERROR;
    }
    pCrcBuff = SCI_ALLOC_APP(DYNAMIC_FILE_CRC_NUM);
    SCI_ASSERT(NULL != pCrcBuff);/*assert verified*/
    /*将指针指向要计算CRC的地方*/
    //SFS_SetFilePointer(f_handle, size - DYNAMIC_FILE_CRC_NUM, 0);
    SFS_SetFilePointer(f_handle, 0, 0);
    SFS_ReadFile(f_handle, pCrcBuff, DYNAMIC_FILE_CRC_NUM, &read_size, PNULL);
    SCI_ASSERT(read_size == DYNAMIC_FILE_CRC_NUM);/*assert verified*/
    SFS_CloseFile(f_handle);
        
    crc_table_ptr = MMK_CreateCRCTable();
    file_crc = MMK_CalculateBlockCRC32( crc_table_ptr, DYNAMIC_FILE_CRC_NUM, CRC_MASK, pCrcBuff );
    SCI_FREE(pCrcBuff);
    MMK_FreeCRCTable( crc_table_ptr );

    crc_data = ntohl( mod_info_ptr->module_info.crc_data );
    //crc_data = ntohl(crc_data);
    if(file_crc == crc_data)
    {
      /*验证通过*/
      return CAF_RESULT_SUCCEEDED;
    }
    #endif
  }
  return CAF_RESULT_FAILED; 
}

/*****************************************************************************/
//  Description : Free CRC table
//  Global resource dependence : 
//  Author:peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMK_FreeCRCTable( uint32* table_ptr )
{
  SCI_ASSERT( PNULL != table_ptr );/*assert verified*/
  SCI_FREE( table_ptr );
}

/*****************************************************************************/
//  Description : 建立CRC表
//  Global resource dependence : 
//  Author:peng
//  Note: 
/*****************************************************************************/
PUBLIC uint32* MMK_CreateCRCTable( void )
{
  uint32 i,j;
  uint32 value = 0;
    uint32* table_ptr = (uint32 *)SCI_ALLOC_APP( CRC_TABLE_SIZE * sizeof(uint32) );
  
  for (i = 0; i < CRC_TABLE_SIZE; i++)
  {
    value = i;

    for (j = 8; j > 0; j--)
    {
      if (value & 1)
            {
        value = (value >> 1) ^ CRC32_POLYNOMIAL;
            }
      else
            {
        value >>= 1;
            }
    }
    table_ptr[i] = value;
  }
    
    return table_ptr;
}

/*****************************************************************************/
//  Description : 计算一块内存的CRC值
//  Global resource dependence : 
//  Author:peng
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMK_CalculateBlockCRC32(
                                      uint32* table_ptr,
                                      uint32 count, 
                                      uint32 crc, 
                                      void *buffer
                                      )
{
  uint8 *ptr = (uint8 *) buffer;
  uint32 temp1;
  uint32 temp2;

  SCI_ASSERT( PNULL != table_ptr );/*assert verified*/

  while(count-- != 0)
  {
    temp1 = (crc>>8) & 0x00FFFFFFL;
    temp2 = table_ptr[((int)crc ^ *ptr++) & 0xff];
    crc = temp1 ^temp2;
  }

  return crc;
}

//added by andrew.zhang at 2009/05/26
/*****************************************************************************/
//  Description : set window title
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_SetWinTitle( CAF_HANDLE_T handle, CAF_STRING_T * str_ptr, BOOLEAN is_need_update )
{
    SCI_ASSERT( PNULL != str_ptr );/*assert verified*/
    
    /* check the handle validate */
    if(!MMK_IsHandleValid(handle))
    {
        return FALSE;
    }
    return GUIWIN_SetTitleText(handle, str_ptr->wstr_ptr, str_ptr->wstr_len, is_need_update );
}

/*****************************************************************************/
//  Description : set window title by id
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_SetWinTitleByID( CAF_HANDLE_T handle, uint32 text_id, BOOLEAN is_need_update )
{
    /* check the handle validate */
    if(!MMK_IsHandleValid(handle))
    {
        return FALSE;
    }
    return GUIWIN_SetTitleTextId(handle, text_id,is_need_update);
}

/*****************************************************************************/
//  Description : get applet install directory
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_GetAppletInstallDirectory( CAF_HANDLE_T applet_handle, wchar *path_ptr, uint16 * wstr_len_ptr )
{
    MMI_MODINFO_NODE_T * modinfo_ptr = NULL;
    CAF_GUID_T  guid = 0;
    size_t size = 0;
    BOOLEAN result = FALSE;
    wchar * applet_full_path_ptr = NULL;
    wchar head[] = {'f', 's', ':', '\\', 0};
    wchar D_disk[] = {'D', 0};
    wchar d_disk[] = {'d', 0};
    wchar E_disk[] = {'E', 0};
    wchar e_disk[] = {'e', 0};
    
    if(path_ptr == NULL)
    {
        return result;
    }
    if(wstr_len_ptr == NULL)
    {
        return result;
    }
    
    guid = MMK_GetGuidByHandle(applet_handle);
    if(guid == 0)
    {
        return result;
    }
    
    modinfo_ptr = MMK_SearchModInfo(guid, MMI_QUERY_BY_GUID);
    
    if(modinfo_ptr == NULL)
    {
        return result;
    }
    
    size = MMIAPICOM_Wstrlen( modinfo_ptr->module_info.dll_file_name );
    
    if(size + 3 > *wstr_len_ptr)
    {
        return result;
    }
    
    applet_full_path_ptr = (wchar*)SCI_ALLOC_APP( ( *wstr_len_ptr ) * sizeof(wchar) );
    SCI_MEMSET( applet_full_path_ptr, 0, (( *wstr_len_ptr ) * sizeof(wchar)) );				
    
    result = MMIAPICOM_GetFilePath(modinfo_ptr->module_info.dll_file_name, (uint16)size, applet_full_path_ptr, wstr_len_ptr);
    
    if(result)
    {
        if(*applet_full_path_ptr == D_disk[0])
        {
            *applet_full_path_ptr = d_disk[0];
        }
        else if(*applet_full_path_ptr == E_disk[0])
        {
            *applet_full_path_ptr = e_disk[0];
        }
        else
        {
            if((*applet_full_path_ptr != d_disk[0]) && (*applet_full_path_ptr != e_disk[0]))
            {
                SCI_ASSERT(0);/*assert verified*/
            }
        }
        
        MMIAPICOM_Wstrcpy(path_ptr, head);
        
        MMIAPICOM_Wstrcat(path_ptr, applet_full_path_ptr);
        
        *wstr_len_ptr = *wstr_len_ptr + MMIAPICOM_Wstrlen(head);
    }
    
    SCI_FREE(applet_full_path_ptr);
    
    return result;
}

/*****************************************************************************/
//  Description : whether the window is focus
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_IsFocusWindow( CAF_HANDLE_T win_handle )
{
    return MMK_IsFocusWin(win_handle);
}

/*****************************************************************************/
//  Description : get window rect
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_GetWinRect( CAF_HANDLE_T handle, CAF_RECT_T*    rect_ptr )
{
    return MMK_GetWinRect(handle, rect_ptr);
}

/*****************************************************************************/
//  Description : send notify message to window
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 ISHELL_SendNotify( CAF_HANDLE_T ctrl_handle, CAF_MESSAGE_ID_E msg_id )
{
    return GUICTRL_SendNotify((MMI_HANDLE_T)ctrl_handle, (MMI_MESSAGE_ID_E)msg_id);
}

/*****************************************************************************/
//  Description : post notify message to window
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 ISHELL_PostNotify( CAF_HANDLE_T ctrl_handle, CAF_MESSAGE_ID_E msg_id )
{
    return GUICTRL_PostNotify((MMI_HANDLE_T)ctrl_handle, (MMI_MESSAGE_ID_E)msg_id);
}

//added end
#endif



