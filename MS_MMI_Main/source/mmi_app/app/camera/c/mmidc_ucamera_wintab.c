/*****************************************************************************
** File Name:      mmidc_ucamera_wintab.c                                                *
** Author:         Jessica                                               *
** Date:           9/2/2012                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2/2012       Jessica              Creat
******************************************************************************/
#include "mmi_app_camera_trc.h"
#if defined(CAMERA_SUPPORT) && defined(MMIDC_F_U_CAMERA)

#define _MMIDC_UCAMERA_WINTAB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "mmk_type.h"
#include "mmi_default.h"
#include "window_parse.h"
#include "mmi_module.h"
#include "mmi_menutable.h"
#include "guiblock.h"
#include "mmidisplay_data.h"
#include "mmi_applet_conflict.h"

//other module
#include "guiiconlist.h"
#include "Guires.h"

//this module
#include "mmidc_camera_id.h"
#include "mmidc_option.h"
#include "mmidc_setting.h"
#include "mmidc_flow.h"
#include "mmidc_save.h"
#ifdef MMI_CAMERA_F_PHOTO_DATE
#include "mmidc_photodate.h"
#endif
#include "mmidc_imageprocess.h"
#include "mmidc_main.h"
#include "mmidc_ucamera_wintab.h"
#include "mmidc_srv.h"


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
typedef enum
{
    UCAMERA_TYPE_NONE,
#ifdef MMIDC_F_N_IN_ONE        
    UCAMERA_TYPE_NINONE,
#endif

#ifdef MMIDC_F_SMILE_AUTO_PHOTO
    UCAMERA_TYPE_SMILE,    
#endif

#ifdef MMIDC_F_LOMO
    UCAMERA_TYPE_LOMO,
#endif

#ifdef MMIDC_F_FISH_EYE
    UCAMERA_TYPE_FISH_EYE,
#endif

#ifdef MMIDC_F_HDR
    UCAMERA_TYPE_HDR,
#endif

#ifdef MMIDC_F_PENCIL_SKETCH
    UCAMERA_TYPE_PENCIL_SKETCH,
#endif

#ifdef MMIDC_F_X_RAY
    UCAMERA_TYPE_X_RAY,
#endif

#ifdef MMIDC_F_INFRARED
    UCAMERA_TYPE_INFRARED,
#endif

#ifdef MMIDC_F_EXPOSAL
    UCAMERA_TYPE_EXPOSAL,
#endif

#ifdef MMIDC_F_NEGATIVE
    UCAMERA_TYPE_NEGATIVE,
#endif

    UCAMERA_TYPE_MAX
}MMIDC_UCAMERA_TYPE_E;

typedef struct
{
    MMIDC_UCAMERA_TYPE_E id; //the id for this item.
    MMI_TEXT_ID_T text_id; //the text for this item
    MMI_IMAGE_ID_T image_id; //the image for this item.
    uint8* buffer_ptr; // the buffer for image
    MMIDC_IMAGE_PROCESS_TYPE_E effect_type; //the effect type for this item.
}MMIDC_UCAMERA_ICONLIST_ITEM;

typedef struct
{
    MMIDC_IMAGE_PROCESS_TYPE_E type;
    BOOLEAN is_switch;
    BOOLEAN is_ucamera;
}MMIDC_UCAMERA_SWITCH_T;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//Description : handle ucamera win msg
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUCameraWinMsg(
                                                 MMI_WIN_ID_T      win_id, 
                                                 MMI_MESSAGE_ID_E  msg_id, 
                                                 DPARAM            param
                                                 );


/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL MMIDC_UCAMERA_ICONLIST_ITEM s_iconlist_items[] = 
{
    {UCAMERA_TYPE_NONE, TXT_EFFECT_NONE, IMG_ICONLIST_EFFECT_NONE, PNULL, IMG_PROC_NORMAL}, 

#ifdef MMIDC_F_N_IN_ONE        
    {UCAMERA_TYPE_NINONE, TXT_EFFECT_NINONE, IMG_ICONLIST_EFFECT_NINONE, PNULL, IMG_PROC_NINONE}, 
#endif 

#ifdef MMIDC_F_SMILE_AUTO_PHOTO
    {UCAMERA_TYPE_SMILE, TXT_EFFECT_SMILE, IMG_ICONLIST_EFFECT_SMILE, PNULL, IMG_PROC_FACE_DECT}, 
#endif

#ifdef MMIDC_F_LOMO
    {UCAMERA_TYPE_LOMO, TXT_EFFECT_LOMO, IMG_ICONLIST_EFFECT_NONE, PNULL, IMG_PROC_LOMO}, 
#endif

#ifdef MMIDC_F_FISH_EYE
    {UCAMERA_TYPE_FISH_EYE, TXT_EFFECT_FISH, IMG_ICONLIST_EFFECT_NONE, PNULL, IMG_PROC_FISHEYE}, 
#endif

#ifdef MMIDC_F_HDR
    {UCAMERA_TYPE_HDR, TXT_EFFECT_HDR, IMG_ICONLIST_EFFECT_NONE, PNULL, IMG_PROC_HDR}, 
#endif

#ifdef MMIDC_F_PENCIL_SKETCH
    {UCAMERA_TYPE_PENCIL_SKETCH, TXT_EFFECT_PENCIL, IMG_ICONLIST_EFFECT_NONE, PNULL, IMG_PROC_BLACK_WHITE_DRAWING}, 
#endif

#ifdef MMIDC_F_X_RAY
    {UCAMERA_TYPE_X_RAY, TXT_EFFECT_XRAY, IMG_ICONLIST_EFFECT_NONE, PNULL, IMG_PROC_XRAY}, 
#endif 

#ifdef MMIDC_F_INFRARED
    {UCAMERA_TYPE_INFRARED, TXT_EFFECT_INFRARED, IMG_ICONLIST_EFFECT_NONE, PNULL, IMG_PROC_INFRARED}, 
#endif

#ifdef MMIDC_F_EXPOSAL
    {UCAMERA_TYPE_EXPOSAL, TXT_EFFECT_EXPOSAL, IMG_ICONLIST_EFFECT_NONE, PNULL, IMG_PROC_EXPOSAL}, 
#endif 

#ifdef MMIDC_F_NEGATIVE
    {UCAMERA_TYPE_NEGATIVE, TXT_EFFECT_NEGATIVE, IMG_ICONLIST_EFFECT_NONE, PNULL, IMG_PROC_NEGATIVE}, 
#endif

};

//uint8 s_current_item = 0;
LOCAL BOOLEAN s_is_first_entry_ucamera = FALSE;
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
WINDOW_TABLE( MMIDC_UCAMERA_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleUCameraWinMsg),    
    WIN_ID( MMIDC_UCAMERA_WIN_ID),
    WIN_STYLE( WS_DISABLE_COMMON_BG),
    WIN_TITLE(TXT_EFFECT_SELECT),  
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),  

    CREATE_ICONLIST_CTRL(MMIDC_UCAMERA_ICONLIST_CTRL_ID),
    END_WIN
};




/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//Description : process icon effect
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessIconEffect(uint32 handle, //in
                                    MMIDC_IMAGE_PROCESS_TYPE_E type, //in
                                    uint16 icon_width, //in
                                    uint16 icon_height, //in
                                    uint8 *src_buf_ptr, //in
                                    uint8 *dst_buf_ptr //out
                                    )
{
    BOOLEAN ret = FALSE;
    int32 rtn_ret = -1;

    IMG_PROC_DATA_FORMAT_E format = IMG_PROC_RGB565;
    IMG_PROC_PROCESS_ICON_T proc_param = {0};
    IMG_PROC_DATA_T proc_return = {0};
    IMG_PROC_SIZE_T image_size = {0};
    uint32 buf_size = 0;

    IMG_PROC_PENCILSKETCH_T pencil_sketch_param = {0};
    IMG_PROC_FISHEYE_T fisheye_param = {0};
    IMG_PROC_HDR_T hdr_param = {0};
    IMG_PROC_LOMO_PARAM_T lomo_param = {0};

    if(PNULL == src_buf_ptr
        || PNULL == dst_buf_ptr)
    {
        return ret;
    }    

    if (handle > 0)
    {
        image_size.w = icon_width;
        image_size.h = icon_height;        
        buf_size = icon_width * icon_height * 2;                                                                          

        proc_param.cmd = type;
        proc_param.size.w = image_size.w;
        proc_param.size.h = image_size.h;
        proc_param.input_format = format;
        proc_param.input_addr.chn0 = src_buf_ptr;
        proc_param.input_addr.chn0_len = buf_size;
        proc_param.output_format = format;
        proc_param.output_addr.chn0 = dst_buf_ptr;
        proc_param.output_addr.chn0_len = buf_size;     

        switch(proc_param.cmd)
        {
        case IMG_PROC_HDR:
            hdr_param.level = 1;
            proc_param.alg_ptr = &hdr_param;
            break;
            
        case IMG_PROC_FISHEYE:
            fisheye_param.distort_level = 100;
            proc_param.alg_ptr = &fisheye_param;
            break;
            
        case IMG_PROC_LOMO:
            lomo_param.width = icon_width;
            lomo_param.height = icon_height;
            lomo_param.mode = IMG_PROC_LOMO_MODE_FULL;
            proc_param.alg_ptr = &lomo_param;
            break;
            
        case IMG_PROC_BLACK_WHITE_DRAWING:
            proc_param.alg_ptr= &pencil_sketch_param;
            break;

        case IMG_PROC_XRAY:
            break;

        case IMG_PROC_INFRARED:
            break;
            
        case IMG_PROC_EXPOSAL:
            break;

        case IMG_PROC_NEGATIVE:
            break;  
            
        default:
            break;
        }        

        //SCI_TRACE_LOW:"[MMIDC] ProcessIconEffect proc_param.cmd=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_UCAMERA_WINTAB_281_112_2_18_2_9_56_756,(uint8*)"d",proc_param.cmd);
        //SCI_TRACE_LOW:"[MMIDC] ProcessIconEffect proc_param.size.w=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_UCAMERA_WINTAB_282_112_2_18_2_9_56_757,(uint8*)"d",proc_param.size.w);
        //SCI_TRACE_LOW:"[MMIDC] ProcessIconEffect proc_param.size.h=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_UCAMERA_WINTAB_283_112_2_18_2_9_56_758,(uint8*)"d",proc_param.size.h);
        //SCI_TRACE_LOW:"[MMIDC] ProcessIconEffect proc_param.input_addr.chn0=0x%x, proc_param.input_addr.chn0_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_UCAMERA_WINTAB_285_112_2_18_2_9_56_759,(uint8*)"dd",proc_param.input_addr.chn0, proc_param.input_addr.chn0_len);
        //SCI_TRACE_LOW:"[MMIDC] ProcessIconEffect proc_param.output_addr.chn0=0x%x, proc_param.output_addr.chn0_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_UCAMERA_WINTAB_287_112_2_18_2_9_56_760,(uint8*)"dd",proc_param.output_addr.chn0, proc_param.output_addr.chn0_len);
        
        rtn_ret = IMGPROC_DoProcessIcon(handle, &proc_param, &proc_return);

        //SCI_TRACE_LOW:"[MMIDC] ProcessIconEffect rtn_ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_UCAMERA_WINTAB_291_112_2_18_2_9_56_761,(uint8*)"d",rtn_ret);

        if(IMG_PROC_SUCCESS == rtn_ret)
        {
            ret = TRUE;
        }
    }
    
    return ret;
}
/*****************************************************************************/
//Description : Free icon list buffer
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void FreeIconListBuffer(void)
{
    int16 i = 0;
    uint16 iconlist_num = ARR_SIZE(s_iconlist_items);

    for(i = 0;i < iconlist_num; i++)
    {
        if(PNULL != s_iconlist_items[i].buffer_ptr)
        {
            SCI_FREE(s_iconlist_items[i].buffer_ptr);
            s_iconlist_items[i].buffer_ptr = PNULL;
        }
    }
}


/*****************************************************************************/
//Description : init icon effect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN InitIconEffect(void)
{
    BOOLEAN res = FALSE;
    BOOLEAN flag = TRUE;
    uint8 i = 0;
    uint16 icon_width = 0,  icon_height = 0;
    uint32 buffer_size = 0;
    uint32 img_data_size = 0;
    uint8 *src_ptr = PNULL;
    uint8 *src_rgb_ptr = PNULL;
    GUI_POINT_T   src_point = {0};
    uint16 iconlist_num = ARR_SIZE(s_iconlist_items);
    MMI_WIN_ID_T win_id = MMIDC_UCAMERA_WIN_ID;
    uint32 handle = 0;
    IMG_PROC_CREATE_T handle_param = {0};

    //SCI_TRACE_LOW:"[MMIDC] InitIconEffect iconlist_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_UCAMERA_WINTAB_341_112_2_18_2_9_56_762,(uint8*)"d",iconlist_num);

    handle = IMGPROC_CreateHandle(&handle_param);
    //SCI_TRACE_LOW:"InitIconEffect handle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_UCAMERA_WINTAB_344_112_2_18_2_9_56_763,(uint8*)"d",handle);
    
    if(0 == handle)
    {
        return TRUE;
    }
    
    for(i = 0;i < iconlist_num; i++)
    {
        flag = TRUE;
        
        switch(s_iconlist_items[i].effect_type)
        {
        case IMG_PROC_NORMAL:
        case IMG_PROC_NINONE:
        case IMG_PROC_FACE_DECT:    
            continue;
            break; /*lint !e527*/
        default:
            break;
        }
        
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height, s_iconlist_items[i].image_id, win_id);        

        buffer_size = icon_width * icon_height * sizeof(GUI_COLOR_T);        

        src_rgb_ptr = SCI_ALLOCA(buffer_size);
        if(PNULL == src_rgb_ptr) /*lint !e831*/
        {
            //SCI_TRACE_LOW:"[MMIDC] InitIconEffect src_rgb_ptr is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_UCAMERA_WINTAB_373_112_2_18_2_9_56_764,(uint8*)"");
            continue;
        }
        else
        {
            do
            {
                SCI_MEMSET(src_rgb_ptr, 0, buffer_size);

                src_ptr =(uint8 *)MMI_GetLabelImage(s_iconlist_items[i].image_id,win_id,&img_data_size);
                
                s_iconlist_items[i].buffer_ptr = SCI_ALLOCA(buffer_size);
                if(PNULL == s_iconlist_items[i].buffer_ptr)
                {
                    //SCI_TRACE_LOW:"[MMIDC] InitIconEffect buffer_ptr is null"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_UCAMERA_WINTAB_387_112_2_18_2_9_56_765,(uint8*)"");
                    flag = FALSE;
                    break;
                }
                
                SCI_MEMSET(s_iconlist_items[i].buffer_ptr, 0, buffer_size);

                //copy src buf
                res = GUIRES_CopyImgBuf(src_ptr,
                                icon_width,
                                icon_height,
                                buffer_size,
                                img_data_size,
                                &src_point,
                                PNULL,
                                (GUI_COLOR_T *)src_rgb_ptr);

                if(!res)
                {
                    //SCI_TRACE_LOW:"[MMIDC] InitIconEffect GUIRES_CopyImgBuf error 2"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_UCAMERA_WINTAB_406_112_2_18_2_9_56_766,(uint8*)"");
                    flag = FALSE;
                    break;
                }

                //process icon
                res = ProcessIconEffect(handle,
                                        s_iconlist_items[i].effect_type,
                                        icon_width,
                                        icon_height,
                                        src_rgb_ptr,
                                        s_iconlist_items[i].buffer_ptr                                
                                        );

                if(!res)
                {
                    //SCI_TRACE_LOW:"[MMIDC] InitIconEffect ProcessIconEffect error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_UCAMERA_WINTAB_422_112_2_18_2_9_56_767,(uint8*)"");
                    flag = FALSE;
                    break;
                }
            }while(0);            
        }

        if(!flag)
        {
            if(PNULL != s_iconlist_items[i].buffer_ptr)
            {
                SCI_FREE(s_iconlist_items[i].buffer_ptr);
                s_iconlist_items[i].buffer_ptr = PNULL;
            }            
        }
        
        if(PNULL != src_rgb_ptr) /*lint !e774*/
        {
            SCI_FREE(src_rgb_ptr);
            src_rgb_ptr = PNULL;
        }
    }

    if(PNULL != src_rgb_ptr)
    {
        SCI_FREE(src_rgb_ptr);
        src_rgb_ptr = PNULL;
    }

    if(handle > 0)
    {
        IMGPROC_DestroyHandle(handle);
        handle = 0;
    }

    //SCI_TRACE_LOW:"[MMIDC] InitIconEffect end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_UCAMERA_WINTAB_457_112_2_18_2_9_56_768,(uint8*)"");
    return TRUE;
}


/*****************************************************************************/
//Description : init u camera win
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN InitUCameraWin(void)
{
    BOOLEAN ret = FALSE;
    
    if(!MMIDC_IsUCameraEnable())
    {
        MMIDC_U_CAMERA_ERR_E err = UCAMERA_ERR_MAX;
            
        err = MMIDC_OpenUCamera();
        if (UCAMERA_ERR_NONE == err)
        {
            ret = TRUE;
        }
        else if(UCAMERA_ERR_NO_SD_CARD == err)
        {
            MMIDC_ErrorTip(TXT_PLEASE_INSERT_SD);
        }
        else
        {
            MMIDC_ErrorTip(TXT_DC_OPEN_FAIL);
        }
            
        if (ret)
        {
            //MMIDC_SetImageProcessType(IMG_PROC_LOMO);
        }
        else
        {
            return ret;
        }
    }

    return InitIconEffect();
}

/*****************************************************************************/
//Description : switch to u camera or normal dc
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void SwitchToUCameraOrNormalDC(void)
{
    MMIDCSRV_MEM_CFG_E mem_cfg = 0;
    MMIDCSRV_WORK_PARAM_T work_param = {0}; 

    //先保存NV参数，再切换
    MMIDC_SaveSettings();

    MMIDC_CloseAllOSDMenu();

    MMIDC_FlowFunction(DC_EXIT);
    MMIDC_SetCameraMode(CAMERA_MODE_DC);


    if (MMIDC_GetUCameraModuleFlag())
    {
        mem_cfg = MMIDCSRV_MEM_DC_UCAMERA;
    }

    work_param = MMIDCSRV_GetDCWorkParam(mem_cfg);
    MMIDCSRV_SetWorkParam(&work_param);


    if (SCREEN_MODE_HORIIZONTAL == MMIDC_GetDefaultScreenMode()  
        && MMIDC_GetPhotoSize() == MMIDC_GePhotoSizetVerLcd()        
        )
    {
        MMIDC_SetScreenMode(SCREEN_MODE_VERTICAL);
    }

#ifdef MMIDC_DESKTOP_PDA        
    MMIDC_ResetListItem();
    MMIDC_InitPDAKeyStatusParameter();
#endif
    MMIDC_RestoreRunningParameter(ACTION_SWITCH_CAMERA_MODE);
#ifdef MMIDC_DESKTOP_PDA        
    MMIDC_StartPreviewAnim(PREVIEW_ANIM_STEP_IMG);
#endif

    MMIDC_InitSettingInfoToDefaultValue();
    //        MMIDC_FlowStart();
}

/*****************************************************************************/
//Description : entry dc app
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E EntryDCApp(MMI_WIN_ID_T win_id, 
                                MMI_MESSAGE_ID_E msg_id,
                                DPARAM param //in
                                )
{
    BOOLEAN ret = FALSE;
    
    MMIDC_UCAMERA_SWITCH_T switch_param = {0};
    MMIDC_IMAGE_PROCESS_TYPE_E select_type = 0;
    BOOLEAN is_switch = FALSE;
    BOOLEAN is_ucamera = FALSE;

    if (PNULL != param)
    {
        switch_param = *(MMIDC_UCAMERA_SWITCH_T*)param;
        select_type = switch_param.type;
        is_switch = switch_param.is_switch;
        is_ucamera = switch_param.is_ucamera;
        
        //open new type
        MMIDC_SetImageProcessType(select_type);

        if(is_switch)
        {
            MMIDC_SetUCameraModuleFlag(is_ucamera);
            SwitchToUCameraOrNormalDC();
        }

    }
    SCI_TRACE_LOW("[MMIDC] EntryDCApp select_type=%d",
                 select_type); 
    SCI_TRACE_LOW("[MMIDC] EntryDCApp is_switch=%d",
                 is_switch);
    SCI_TRACE_LOW("[MMIDC] EntryDCApp is_ucamera=%d",
                 is_ucamera);
                                 
    SCI_TRACE_LOW("[MMIDC] EntryDCApp ret=%d",
                 ret);
    
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//Description : check conflict dc
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckConflictDC(MMIDC_UCAMERA_SWITCH_T *switch_param_ptr
                                )
{
    MMI_APPLET_CONFLICT_T conflict = {0};
    BOOLEAN ret = FALSE;
    BOOLEAN is_check_conflict = FALSE;
    MMIDCSRV_MEM_CFG_E mem_cfg = 0;


    if (PNULL != switch_param_ptr)
    {
        if (switch_param_ptr->is_ucamera)
        {
            mem_cfg = MMIDCSRV_MEM_DC_UCAMERA;
        }
        
        SCI_TRACE_LOW("[MMIDC] CheckConflictDC ");      
        
        is_check_conflict = MMIDC_Check2MSensorState(mem_cfg);
        
        if (is_check_conflict)
        {
            conflict.func = EntryDCApp;
            conflict.param = switch_param_ptr;
            conflict.size_of_param = sizeof(*switch_param_ptr);

            ret = MMI_CheckAppletConflict(&conflict);  
        }
        else
        {
            ret = EntryDCApp(0, 0, switch_param_ptr);
        }
    }
    
    SCI_TRACE_LOW("[MMIDC] CheckConflictDC ret=%d",
                 ret);

    return ret;
}

/*****************************************************************************/
//Description : init u camera win
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectEffectItem(uint8 index)
{
    BOOLEAN ret = FALSE;
    MMIDC_IMAGE_PROCESS_TYPE_E select_type = 0;
    
    MMIDC_UCAMERA_SWITCH_T switch_param = {0};
    BOOLEAN is_switch = FALSE;
    BOOLEAN is_ucamera = FALSE;

    select_type = s_iconlist_items[index].effect_type;

    //SCI_TRACE_LOW:"[MMIDC] SelectEffectItem select_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_UCAMERA_WINTAB_547_112_2_18_2_9_57_769,(uint8*)"d",select_type);
    
    if(IMG_PROC_NORMAL == select_type)
    {
        ret = TRUE;
        MMIDC_CloseUCamera();
    }


    {
        MMIDC_IMAGE_PROCESS_TYPE_E type = 0;

        //destroy old type
        type = MMIDC_GetImageProcessType();
        MMIDC_DestroyUCameraFeature(type);

        is_ucamera = MMIDC_GetUCameraModuleFlag();
        if(((IMG_PROC_NORMAL == select_type) && MMIDC_GetUCameraModuleFlag()) ||
            ((IMG_PROC_NORMAL != select_type) && !MMIDC_GetUCameraModuleFlag()))
        {
            is_switch = TRUE;
            if(IMG_PROC_NORMAL == select_type)
            {
                is_ucamera = FALSE;
            }
            else
            {
                is_ucamera = TRUE;
                
                #ifdef MMI_CAMERA_F_PHOTO_DATE
                MMIDC_ReleasePhotoDateData();
                #endif
            }
        }
        
        switch_param.type = select_type;
        switch_param.is_switch = is_switch;
        switch_param.is_ucamera = is_ucamera;
        CheckConflictDC(&switch_param);
        
    }

    ret = TRUE;

    //SCI_TRACE_LOW:"[MMIDC] SelectEffectItem ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_UCAMERA_WINTAB_611_112_2_18_2_9_57_770,(uint8*)"d",ret);
    
    return ret;    
}

/*****************************************************************************/
//Description : get current item index
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL uint16 GetCurrentItemIndex()
{
    uint16 i = 0;
    uint16                          index = 0;
    MMIDC_IMAGE_PROCESS_TYPE_E img_process_type = MMIDC_GetImageProcessType();

    for(i = 0; i < ARR_SIZE(s_iconlist_items); i++)
    {
        if(img_process_type == s_iconlist_items[i].effect_type)
        {
            index = i;
            break;
        }
    }

    return index;
}


/*****************************************************************************/
//Description : handle ucamera win msg
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUCameraWinMsg(
                                                 MMI_WIN_ID_T      win_id, 
                                                 MMI_MESSAGE_ID_E  msg_id, 
                                                 DPARAM            param
                                                 )
{
    MMI_RESULT_E                    result = MMI_RESULT_TRUE;
    uint16                          index = 0;
    MMI_CTRL_ID_T                   ctrl_id = MMIDC_UCAMERA_ICONLIST_CTRL_ID;
    GUIANIM_DATA_INFO_T             data_info = {0};
    GUIICONLIST_DISP_T              icon_list = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_FONT_ALL_T font={0};
            
            if(!InitUCameraWin())
            {
                //SCI_TRACE_LOW:"[MMIDC] HandleUCameraWinMsg InitUCameraWin fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_UCAMERA_WINTAB_663_112_2_18_2_9_57_771,(uint8*)"");
                MMK_CloseWin(win_id);
                return result;
            }
            font.font =  MMI_DEFAULT_SMALL_FONT;
            font.color = MMI_BLACK_COLOR;

            index = GetCurrentItemIndex();
            
            GUIICONLIST_SetStyle(ctrl_id, GUIICONLIST_STYLE_ICON_UIDT);
            GUIICONLIST_SetTotalIcon(ctrl_id,ARR_SIZE(s_iconlist_items));
            GUIICONLIST_SetIconListTextInfo(ctrl_id, font);
//        GUIICONLIST_SetMarkable(ctrl_id, TRUE);
            GUIICONLIST_SetMaxSelectedItem(ctrl_id, 1);
            GUIICONLIST_SetSelectedItem(ctrl_id, index,TRUE);
            GUIICONLIST_SetCurIconIndex(index,ctrl_id);
        
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;

    case MSG_FULL_PAINT:
        {
            uint16 sel_index = 0;

            index = GetCurrentItemIndex();

            GUIICONLIST_GetSelectedItemIndex(ctrl_id, &sel_index, 1);
            GUIICONLIST_SetSelectedItem(ctrl_id, sel_index, FALSE);

            GUIICONLIST_SetSelectedItem(ctrl_id, index, TRUE);
        }
        break;

    case MSG_CTL_ICONLIST_APPEND_TEXT:
        {
            index = *((uint16 *)param);

            if(index < ARR_SIZE(s_iconlist_items))
            {
                //set icon list       
                MMI_GetLabelTextByLang(s_iconlist_items[index].text_id, &icon_list.name_str);
                
                icon_list.softkey_id[0] = STXT_SELECT;
                icon_list.softkey_id[1] = TXT_NULL;
                icon_list.softkey_id[2] = STXT_RETURN;

                //append icon
                GUIICONLIST_AppendText(index,ctrl_id,&icon_list);
            }
        }
        break;

    case MSG_CTL_ICONLIST_APPEND_ICON:
        {
            GUIIMG_BITMAP_T     bitmap_data = {0};
            uint16 icon_width = 0;
            uint16 icon_height = 0;

            
            index = *((uint16 *)param);

            if(index < ARR_SIZE(s_iconlist_items))
            {
                if(PNULL != s_iconlist_items[index].buffer_ptr)
                {
                    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, s_iconlist_items[index].image_id, win_id);
                    
                    bitmap_data.date_type = GUIIMG_DATE_TYPE_RGB565;
                    bitmap_data.bit_ptr = s_iconlist_items[index].buffer_ptr;
                    bitmap_data.img_width = icon_width;
                    bitmap_data.img_height = icon_height;

                    data_info.is_bitmap = TRUE;
                    data_info.bitmap_data = bitmap_data;
                }
                else
                {
                    data_info.is_bitmap = FALSE;
                    data_info.img_id = s_iconlist_items[index].image_id;
                }

                //append icon
                GUIICONLIST_AppendIcon(index,ctrl_id,&data_info,NULL);
            }
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef MMI_PDA_SUPPORT
        if ((PNULL != param) 
             && (MSG_CTL_PENOK == msg_id)
           )
        {
            if ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //left button
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                break;
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //right button
                MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                break;
            }
        }
#endif //MMI_PDA_SUPPORT

        index = GUIICONLIST_GetCurIconIndex(ctrl_id);
        SelectEffectItem(index);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        if(MMIDC_GetFirstEntryUcameraFlag())
        {
            MMIDC_PostStateMsg();
        }
        else
        {
            index = GUIICONLIST_GetCurIconIndex(ctrl_id);
            SelectEffectItem(index);
            MMK_CloseWin( win_id);
        }
        break;
        
    case MSG_CLOSE_WINDOW: 
        //For direct exit window   
        MMIDC_SetFirstEntryUcameraFlag(FALSE);
        FreeIconListBuffer();
        break;          
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/**--------------------------------------------------------------------------*
 **                         PUBLIC FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//Description : open ucamera win
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenUCameraWin(void)
{
    MMIDC_CreateWinByApplet((uint32*)MMIDC_UCAMERA_WIN_TAB, (ADD_DATA)PNULL);
}

/*****************************************************************************/
//Description : open u camera alert win
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenUCameraAlertWin(MMI_TEXT_ID_T txt_id)
{
    //do not use applet win to exit dc module
    //txt_id = TXT_DC_OPEN_FAIL
    
    MMIPUB_OpenAlertWinByTextId(PNULL,txt_id,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    MMIDC_PostStateMsg();
}      

/*****************************************************************************/
//Description : get first entry ucamera flag
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_GetFirstEntryUcameraFlag(void)
{
    SCI_TRACE_LOW("[MMIDC] MMIDC_GetFirstEntryUcameraFlag flag = %d", s_is_first_entry_ucamera);
    return s_is_first_entry_ucamera;
}

/*****************************************************************************/
//Description : set first entry ucamera flag
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetFirstEntryUcameraFlag(BOOLEAN flag)
{
    s_is_first_entry_ucamera = flag;
}
#endif //defined(CAMERA_SUPPORT) && defined(MMIDC_F_U_CAMERA)




/*Edit by script, ignore 1 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
