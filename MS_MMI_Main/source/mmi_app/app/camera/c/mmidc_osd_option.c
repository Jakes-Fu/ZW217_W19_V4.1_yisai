#include "mmi_app_camera_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef CAMERA_SUPPORT
#include "os_api.h"
#include "mmi_applet_table.h"
#include "mmidc_display.h"
#include "block_mem.h"
#include "guiblock.h"
#include "guifont.h"
#include "mmi_theme.h"
#include "mmidc_export.h"
#include "mmidc_flow.h"
#include "mmidc_save.h"
#include "mmi_position.h"
#include "mmidisplay_data.h"
#include "mmi_text.h"
#include "mmidc_gui.h"
#include "mmi_common.h"
#include "mmi_nv.h"

//local module
#include "mmidc_camera_text.h"
#include "mmidc_camera_id.h"
#include "mmidc_setting.h"
#include "mmifilearray_export.h"
#include "mmi_image.h"
#include "mmifmm_export.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
#ifdef MMI_CAMERA_F_PHOTO_DATE
#include "mmidc_photodate.h"
#endif

#ifdef MMIDC_DESKTOP_PDA
#include "mmidc_guipda.h"
#else
#ifndef MMIDC_MINI_RESOURCE_SIZE
#include "guires.h"
#include "mmidc_guiiconbar.h"
#endif
#endif

#ifdef MMIDC_F_WORK_MODE
#include "mmidc_workmode.h"
#endif

#include "mmimultim_image.h"
#if !defined(MMIDC_DESKTOP_PDA)
#include "mmimultim_text.h"
#endif

#ifdef MMIDC_F_N_IN_ONE
#include "mmidc_ninone.h"
#endif
#ifdef MMIDC_F_U_CAMERA
#include "mmidc_imageprocess.h"
#include "mmidc_ucamera_wintab.h"
#endif

#include "mmidc_main.h"
#include "mmidc_option.h"



#define MMIDC_FILE_NAME_LEN     15
typedef enum
{
    BRIGHTNESS_ADJUST_MENU = 0,	
        CONTRAST_ADJUST_MENU,	
        EC_ADJUST_MENU,
        ADJUST_MENU_MAX
}ADJUST_MENU_E;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

typedef struct
{
    int16 x;
    int16 y;
    ADJUST_MENU_E menu;
    GUI_RECT_T    rect;
    GUI_RECT_T    increase_rect;
    GUI_RECT_T    decrease_rect;
    GUI_RECT_T    bg_rect;
}MMIDC_ADJUST_DATA_T;
static MMIDC_ADJUST_DATA_T	s_adjust_data = {0};
static uint32  s_focused_photo = 0;

static BOOLEAN  s_is_changed_folder = FALSE; 
#if defined (MMI_UDISK_MEM_ENABLE) && !defined (MMI_DV_UDISK_ENABLE)
LOCAL BOOLEAN  s_in_dv_save_path = FALSE;
#endif

#if !defined(MMIDC_DESKTOP_PDA)
/*****************************************************************************/
// 	Description : handle adjust key function
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void CloseAdjust(void);
#endif

/*****************************************************************************/
// 	Description : get option menu position
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
//  Param:
//        Output: x_ptr and y_ptr  position for showing
/*****************************************************************************/
LOCAL void GetOptionMenuPosition(int16 *x_ptr, //out
                                        int16 *y_ptr  //out
                                        );
                                        
/*****************************************************************************/
// 	Description : set camera mode photo
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void SetPhotoMode(void) 
{
    if(MMIDC_GetCameraMode() == CAMERA_MODE_DV)
    {
        //先保存NV参数，再切换
        MMIDC_SaveSettings();

        MMIDC_CloseAllOSDMenu();
        
        MMIDC_FlowFunction(DC_EXIT);
#ifdef MMIDC_F_LAST_IMAGE
        MMIDC_SaveLastThumbnailDataToFile();
        MMIDC_SetLastThumbnailDataValidFlag(FALSE);
        MMIDC_GetLastThumbnailData(CAMERA_MODE_DC);
#endif
        MMIDC_SetCameraMode(CAMERA_MODE_DC);
        
        if (SCREEN_MODE_HORIIZONTAL == MMIDC_GetDefaultScreenMode()  
            && MMIDC_GetPhotoSize() == MMIDC_GePhotoSizetVerLcd()        
           )
        {
            MMIDC_SetScreenMode(SCREEN_MODE_VERTICAL);
        }

        MMIDC_RestoreRunningParameter(ACTION_SWITCH_CAMERA_MODE);
#ifdef MMIDC_DESKTOP_PDA        
        MMIDC_StartPreviewAnim(PREVIEW_ANIM_STEP_IMG);
#endif
        MMIDC_FlowStart();
    }
}

/*****************************************************************************/
// 	Description : set camera mode video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void SetVideoMode(void) 
{
    if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
    {
        MMIDC_BackupRunningParameter(ACTION_SWITCH_CAMERA_MODE);
        
        //先保存NV参数，再切换
        MMIDC_SaveSettings();

        MMIDC_CloseAllOSDMenu();


        MMIDC_FlowFunction(DC_EXIT);
#ifdef MMIDC_F_LAST_IMAGE
        {
            MMIDC_GET_LAST_THUMBNAIL_DATA_E get_data_type = MMIDC_GET_LAST_THUMBNAIL_DATA_MAX;
            MMIDC_SaveLastThumbnailDataToFile();
            MMIDC_SetLastThumbnailDataValidFlag(FALSE);
            get_data_type = MMIDC_GetLastThumbnailData(CAMERA_MODE_DV);
            if(MMIDC_GET_LAST_DATA_VIDEO == get_data_type)
            {
                MMIDC_SetDCModuleCmd(MMIDC_CMD_SWITCH_VIDEO_MODE);
                return;
            }
        }
#endif

        MMIDC_SetVideoModeContinue();
    }
}

/*****************************************************************************/
// 	Description : set camera mode video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoModeContinue(void) 
{
    if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
    {
#ifdef MMI_CAMERA_F_PHOTO_DATE
        MMIDC_ReleasePhotoDateData();
#endif        
        MMIDC_SetCameraMode(CAMERA_MODE_DV);
        if(MMIDC_GetDefaultScreenMode() == SCREEN_MODE_HORIIZONTAL 
           && MMIDC_GetPhotoSize() == MMIDC_GePhotoSizetVerLcd()
           )
        {
            MMIDC_SetScreenMode(SCREEN_MODE_HORIIZONTAL);
        }
        
        MMIDC_SetFrameIndex(FRAME_DISABLE, FALSE);
#ifdef MMIDC_DESKTOP_PDA
        MMIDC_StartPreviewAnim(PREVIEW_ANIM_STEP_IMG);
#endif
        MMIDC_FlowStart();
    }
}

/*****************************************************************************/
// 	Description : open camera mode setting menu
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenDCModeMenu(uint32 x, uint32 y)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE    
    DC_MENU_ITEMS_OP* osd_menu = MMIDC_InitMenu();

    if (PNULL == osd_menu)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] OpenDCModeMenu osd_menu = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_550_112_2_18_2_9_29_478,(uint8*)"");
        return ;
    }
    
    osd_menu->MenuSetMenuTitle(TXT_DC_MODE);
    osd_menu->MenuAppendItemText(TXT_DC_SWITCH_PHOTO);
    osd_menu->MenuAppendItemText(TXT_DC_SWITCH_VIDEO);
    osd_menu->MenuSetStartPositionX(x);
    osd_menu->MenuSetStartPositionY(y);
    osd_menu->MenuSelectItem(MMIDC_GetCameraMode());
    osd_menu->MenuFocusItem(MMIDC_GetCameraMode());
    osd_menu->MenuSetDisplayMode(DISPLAY_DIRECT_VERTICAL);
    osd_menu->MenuSetFocusItemBackGroundColor(MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
    osd_menu->MenuSetItemCallback(0, SetPhotoMode);
    osd_menu->MenuSetItemCallback(1, SetVideoMode);
    osd_menu->MenuSetNotCallbackFocus();
#endif
}

/*****************************************************************************/
// 	Description : start switch sensor
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void StartSwitchSensor(void)

{
    BOOLEAN ret = FALSE;
    CAMERA_MODE_E mode = MMIDC_GetCameraMode();
    
#ifdef MMIDC_F_U_CAMERA
       if(CAMERA_MODE_DC == MMIDC_GetCameraMode() 
            && MMIDC_IsUCameraEnable()
          )
        {
//for spi sensor
#if 0        	
#ifdef MMIDC_F_N_IN_ONE
            if(!MMIDC_IsNInOneEnable())
#endif
#endif
            {
                 MMIDC_CloseUCamera();
            }           
        }
#endif

        MMIDC_CloseAllOSDMenu();
        
        MMIDC_FlowFunction(DC_EXIT);
#ifdef MMI_CAMERA_F_PHOTO_DATE        
        MMIDC_ReleasePhotoDateData();
#endif

        if (CAMERA_MODE_DC == mode)
        {
            ret = MMIDC_OpenDC();
        }
        else
        {
            ret = MMIDC_OpenDV();
        }
        
        if (!ret )
        {
            
            MMIDC_PostStateMsg();
            MMIDC_ErrorTipForExit(TXT_DC_OPEN_FAIL);
            
            return;
        }

        MMIDC_InitSwitchSensorParameter();


#ifdef MMIDC_F_U_CAMERA
        if(CAMERA_MODE_DC == MMIDC_GetCameraMode() 
            && MMIDC_GetUCameraModuleFlag() 
//for spi sensor
#if 0            
#ifdef MMIDC_F_N_IN_ONE
            && !MMIDC_IsNInOneEnable()
#endif
#endif
           )
        {
            MMIDC_U_CAMERA_ERR_E err = UCAMERA_ERR_MAX;

            if (!MMIDC_IsUCameraEnable())
            {
                err = MMIDC_OpenUCamera();
                if (UCAMERA_ERR_NONE != err)
                {
                    MMIDC_OpenUCameraAlertWin(TXT_DC_OPEN_FAIL);
                    return;
                }
            }
            MMIDC_OpenUCameraFeature(MMIDC_GetImageProcessType());

        }
#endif

       
        MMIDC_FlowStart();
}

/*****************************************************************************/
// 	Description : set front sensor
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void SetFrontSensor(void)
{
    SETTING_SENSOR_ID_E   sensor_id = 0;

    sensor_id = MMIDC_GetSettingSensorID();
    if (SETTING_SENSOR_FRONT != sensor_id)
    {
        //first, save postpositive sensor setting
        MMIDC_SaveSettingInfoBySwitchSensor(SETTING_SENSOR_REAR);
        
        MMIDC_SetSettingSensorID(SETTING_SENSOR_FRONT);
        StartSwitchSensor();
    }
}

/*****************************************************************************/
// 	Description : set rear sensor
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void SetRearSensor(void)
{
    SETTING_SENSOR_ID_E   sensor_id = 0;

    sensor_id = MMIDC_GetSettingSensorID();
    if (SETTING_SENSOR_REAR != sensor_id)
    {
        MMIDC_SetSettingSensorID(SETTING_SENSOR_REAR);
        StartSwitchSensor();
    }
}
/*****************************************************************************/
// 	Description : set photo size 3 m
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_NeedReStartPreview(MMIDC_PHOTO_SIZE_E size)
{
    return FALSE;
#if 0    
    MMIDC_PHOTO_SIZE_E old_size = MMIDC_GetPhotoSize();
    BOOLEAN need[PHOTO_SIZE_640X480][PHOTO_SIZE_640X480] = 
        { //176X220       220X176     240X320      240X400     400X240   320*480   480*320   480*640    320*240   
/*176X220*/  FALSE,     TRUE,      FALSE,     FALSE,     FALSE,  FALSE,  FALSE,    FALSE,     TRUE,  
/*220X176*/  TRUE,      FALSE,     FALSE,     FALSE,     FALSE,  FALSE,  FALSE,    FALSE,     TRUE,  
/*240X320*/  FALSE,     FALSE,     FALSE,     FALSE,     FALSE,  FALSE,  FALSE,    FALSE,     TRUE,   
/*240X400*/  FALSE,     FALSE,     FALSE,     FALSE,     TRUE,   FALSE,  FALSE,    FALSE,     TRUE,   
/*400X240*/  FALSE,     FALSE,     FALSE,     TRUE,      FALSE,  FALSE,  FALSE,    FALSE,     TRUE,  
/*320*480*/  FALSE,     FALSE,     FALSE,     FALSE,     FALSE,  FALSE,  TRUE,     FALSE,     TRUE,
/*480*320*/  FALSE,     FALSE,     FALSE,     FALSE,     FALSE,  TRUE,   FALSE,    FALSE,     TRUE,
/*480*640*/  FALSE,     FALSE,     FALSE,     FALSE,     FALSE,  FALSE,  FALSE,    FALSE,     TRUE,
/*320*240*/  TRUE,      TRUE,      TRUE,      TRUE,      TRUE,   TRUE,   TRUE,     FALSE,     FALSE 
        };


    if(old_size > PHOTO_SIZE_320X240)
    {
        old_size = PHOTO_SIZE_320X240;
    }
    if(size > PHOTO_SIZE_320X240)
    {
        size = PHOTO_SIZE_320X240;
    }

       
   return need[size][old_size];

    
   
    if(MMIDC_GetDefaultVisualMode() == CAMERA_ENG_FULL_VISUAL_ANGLE)
    {
        if(LCD_ANGLE_90 == MMIAPIDC_GetCameraSensorAngle()) //modified, @robert.wang, 09-11-23, cr161986
        {
            BOOLEAN need[PHOTO_SIZE_640X480][PHOTO_SIZE_640X480] = 
            { //176X220       220X176       240X320       240X400      400X240   320*480   480*320     QVGA   
   /*176X220*/  FALSE,     TRUE,      FALSE,     FALSE,     FALSE,  FALSE,  FALSE,   TRUE,  
   /*220X176*/  TRUE,      FALSE,     FALSE,     FALSE,     FALSE,  FALSE,  FALSE,   TRUE,  
   /*240X320*/  FALSE,     FALSE,     FALSE,     FALSE,     FALSE,  FALSE,  FALSE,   TRUE,   
   /*240X400*/  FALSE,     FALSE,     FALSE,     FALSE,     TRUE,   FALSE,  FALSE,   TRUE,   
   /*400X240*/  FALSE,     FALSE,     FALSE,     TRUE,      FALSE,  FALSE,  FALSE,   TRUE,  
   /*320*480*/  FALSE,     FALSE,     FALSE,     FALSE,     FALSE,  FALSE,  TRUE,    TRUE,
   /*480*320*/  FALSE,     FALSE,     FALSE,     FALSE,     FALSE,  TRUE,   FALSE,   TRUE,
   /*QVGA*/     TRUE,      TRUE,      TRUE,      TRUE,      TRUE,   TRUE,   TRUE,    FALSE 
            };
            return need[size][old_size];
        }
        else
        {
            BOOLEAN need[PHOTO_SIZE_640X480][PHOTO_SIZE_640X480] = 
            { //176X220       220X176       240X320       240X400      400X240    320*480   480*320   QVGA   
   /*176X220*/  FALSE,     FALSE,     FALSE,     FALSE,     FALSE,   FALSE,  FALSE,  TRUE,  
   /*220X176*/  FALSE,     FALSE,     FALSE,     FALSE,     FALSE,   FALSE,  FALSE,  FALSE,  
   /*240X320*/  FALSE,     FALSE,     FALSE,     FALSE,     FALSE,   FALSE,  FALSE,  TRUE,   
   /*240X400*/  FALSE,     FALSE,     FALSE,     FALSE,     FALSE,   FALSE,  FALSE,  TRUE,   
   /*400X240*/  FALSE,     FALSE,     FALSE,     FALSE,     FALSE,   FALSE,  FALSE,  FALSE,               
   /*320X480*/  FALSE,     FALSE,     FALSE,     FALSE,     FALSE,   FALSE,  TRUE,   TRUE,   
   /*480X320*/  FALSE,     FALSE,     FALSE,     FALSE,     FALSE,   TRUE,   FALSE,  TRUE,               
   /*QVGA*/     TRUE,      FALSE,     TRUE,      TRUE,      FALSE,   TRUE,   TRUE,   FALSE 
            };
            return need[size][old_size];
        }
    }
    else
    {
        if(LCD_ANGLE_90 == MMIAPIDC_GetCameraSensorAngle())
        {
            BOOLEAN need[PHOTO_SIZE_640X480][PHOTO_SIZE_640X480] = 
            { //176X220       220X176      240X320       240X400       400X240   320*480    480*480    QVGA   
   /*176X220*/  FALSE,     TRUE,      FALSE,     FALSE,     FALSE,  FALSE,  FALSE,   TRUE,  
   /*220X176*/  TRUE,      FALSE,     TRUE,      TRUE,      FALSE,  FALSE,  FALSE,   FALSE,  
   /*240X320*/  FALSE,     TRUE,      FALSE,     FALSE,     TRUE,   FALSE,  FALSE,   TRUE,   
   /*240X400*/  FALSE,     TRUE,      FALSE,     FALSE,     TRUE,   FALSE,  FALSE,   TRUE,   
   /*400X240*/  TRUE,      FALSE,     TRUE,      TRUE,      FALSE,  FALSE,  FALSE,   FALSE,  
   /*320*480*/  FALSE,     FALSE,     FALSE,     FALSE,     FALSE,  FALSE,  TRUE,    TRUE,
   /*480*320*/  FALSE,     FALSE,     FALSE,     FALSE,     FALSE,  TRUE,   FALSE,   TRUE,
   /*QVGA*/     TRUE,      FALSE,     TRUE,      TRUE,      FALSE,  TRUE,   TRUE,    FALSE 
            };
            return need[size][old_size];
        }
        else
        {
            return FALSE;
        }
    }

    return FALSE; /*lint !e527 */
    
#endif    
}
/*****************************************************************************/
// 	Description : set photo size state
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC int32 SetPhotoSizeState(MMIDC_PHOTO_SIZE_E photo_size)
{
    FRAME_E  frame_index = FRAME_DISABLE;
    
    //SCI_TRACE_LOW:"[MMIDC] SetPhotoSizeState photo_size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_720_112_2_18_2_9_29_479,(uint8*)"d", photo_size);
    
//    if(MMIDC_NeedReStartPreview(photo_size))
    {
        MMIDC_CloseAllOSDMenu();
        
        MMIDC_StopPreviewForSwitchSize();
        
        if (SCREEN_MODE_HORIIZONTAL == MMIDC_GetDefaultScreenMode())
        {
            MMIDC_SetScreenMode(SCREEN_MODE_HORIIZONTAL);
        }        
        
        if (SCREEN_MODE_HORIIZONTAL == MMIDC_GetDefaultScreenMode()  
            && photo_size == MMIDC_GePhotoSizetVerLcd()        
           )
        {
            MMIDC_SetScreenMode(SCREEN_MODE_VERTICAL);
        }
        
        //SCI_TRACE_LOW:"[MMIDC] SetPhotoSizeState screen mode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_740_112_2_18_2_9_29_480,(uint8*)"d", MMIDC_GetScreenMode());

        frame_index = MMIDC_GetSwitchFrameIndex();
        if (FRAME_DISABLE != frame_index)
        {
            MMIDC_SetFrameIndex(frame_index,FALSE);
            MMIDC_SetSwitchFrameIndex(FRAME_DISABLE);
        }
        MMIAPIDC_SetPhotoSize(photo_size); 
        MMIDC_StartPreviewForSwitchSize();
    }
//    else
//    {
//        MMIAPIDC_SetPhotoSize(photo_size);
//    }

    return 0;
}

/*****************************************************************************/
// Description : set photo size qvga
// Global resource dependence : none
// Author: ryan.xu
// Note:
/*****************************************************************************/
LOCAL void Set3264X2448(void) 
{ 
    SetPhotoSizeState(PHOTO_SIZE_3264X2448);
}

/*****************************************************************************/
// 	Description : set photo size 3 m
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void Set2592X1944(void)
{
    SetPhotoSizeState(PHOTO_SIZE_2592X1944);
}

/*****************************************************************************/
// 	Description : set photo size 3 m
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void Set2048X1536(void)
{
    SetPhotoSizeState(PHOTO_SIZE_2048X1536);
}

/*****************************************************************************/
// 	Description : set photo size 2 m
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void Set1600X1200(void)
{
    SetPhotoSizeState(PHOTO_SIZE_1600X1200);
}

/*****************************************************************************/
// 	Description : set photo size 1 m
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void Set1280X960(void)
{
    SetPhotoSizeState(PHOTO_SIZE_1280X960);
}

/*****************************************************************************/
//  Description : set photo size 960X1280
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void Set960X1280(void)
{
    SetPhotoSizeState(PHOTO_SIZE_960X1280);
}

/*****************************************************************************/
// 	Description : set photo size 176x220
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void Set176X220(void) 
{
    SetPhotoSizeState(PHOTO_SIZE_176X220);
}

/*****************************************************************************/
// 	Description : set photo size 176x220
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void Set220X176(void) 
{
    SetPhotoSizeState(PHOTO_SIZE_220X176);
}
/*****************************************************************************/
// 	Description : set photo size 128x160
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
LOCAL void Set128X160(void) 
{
    SetPhotoSizeState(PHOTO_SIZE_128X160);
}
/*****************************************************************************/
// 	Description : set photo size 160x128
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
LOCAL void Set160X128(void) 
{
    SetPhotoSizeState(PHOTO_SIZE_160X128);
}
/*****************************************************************************/
// 	Description : set photo size vga
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void Set640X480(void)
{
    SetPhotoSizeState(PHOTO_SIZE_640X480);
}

/*****************************************************************************/
// 	Description : set photo size qvga
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void Set320X240(void) 
{
    SetPhotoSizeState(PHOTO_SIZE_320X240);
}

/*****************************************************************************/
// 	Description : set photo size qvga
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void Set240X320(void) 
{
    SetPhotoSizeState(PHOTO_SIZE_240X320);
}

/*****************************************************************************/
// 	Description : set photo size qvga
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void Set400X240(void) 
{ 
    SetPhotoSizeState(PHOTO_SIZE_400X240);
}

/*****************************************************************************/
// 	Description : set photo size qvga
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void Set320X480(void) 
{ 
    SetPhotoSizeState(PHOTO_SIZE_320X480);
}

/*****************************************************************************/
// 	Description : set photo size qvga
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void Set480X320(void) 
{ 
    SetPhotoSizeState(PHOTO_SIZE_480X320);
}

/*****************************************************************************/
// Description : set photo size qvga
// Global resource dependence : none
// Author: ryan.xu
// Note:
/*****************************************************************************/
LOCAL void Set480X640(void) 
{ 
    SetPhotoSizeState(PHOTO_SIZE_480X640);
}

/*****************************************************************************/
// 	Description : set photo size qvga
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void Set240X400(void) 
{ 
    SetPhotoSizeState(PHOTO_SIZE_240X400);
}

/*****************************************************************************/
//Description :  get phone photo size list
//Global resource dependence : none
//Author: robert.wang
//Note: size list from high size to low size
// Return: total supported number
/*****************************************************************************/
PUBLIC int16 GetPhonePhotoSizeList(int16 max_item,
                                      int32  lcd_size,
                                      SCREEN_MODE_E screen_mode,                                            
                                      MMIDC_PHOTO_SIZE_E *size_list_ptr //out
                                      )
{
    MMIDC_PHOTO_SIZE_E hor_size = 0; 
    MMIDC_PHOTO_SIZE_E ver_size = 0; 
    MMIDC_PHOTO_SIZE_E size_array[PHOTO_SIZE_MAX] = {0};

    int16 total_3p4_array_number = 0;
    MMIDC_PHOTO_SIZE_E size_3p4_array[] = 
    {
        PHOTO_SIZE_960X1280,
        PHOTO_SIZE_480X640,
        PHOTO_SIZE_240X320
    };
    MMIDC_PHOTO_SIZE_E cur_size = 0;
    
    int16 index = 0;
    int32 i = 0;
    int16 total_item = 0; 
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = 0;

    BOOLEAN is_show_ver_size = TRUE;
    MMIDC_PHOTO_SIZE_E max_photo_size = 0;
    LCD_ANGLE_E  current_angle = GUILCD_GetLogicAngle((LCD_ID_E)GUI_MAIN_LCD_ID);
    
    total_item = ARR_SIZE(size_array);
    //init size array to PHOTO_SIZE_MAX
    //PHOTO_SIZE_MAX is not support size
    for (i = 0; i < total_item; i++)
    {
        size_array[i] = PHOTO_SIZE_MAX;
    }

    MMIDC_GetSupportSizeArray(size_array);

    max_photo_size = MMIDC_GetCurMaxPhotoSize();

    sensor_type = MMIAPIDC_GetDCModulSensorType();

    if (DCAMERA_IMG_SENSOR_TYPE_4P3 == sensor_type
        || (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type 
            && LCD_ANGLE_90 == current_angle
           )
        )
    {   
        index = 0;
        for (i = max_photo_size; i >= PHOTO_SIZE_640X480; i--)
        {
            if (size_array[i] <= max_photo_size)
            {
                if (index < max_item)
                {
                    size_list_ptr[index] = i;
                    index ++;
                }
            }
        }

        if((MMIDC_QCIF_PREVIEW_HEIGHT == lcd_size) || 
            (MMIDC_QVGA_PREVIEW_HEIGHT == lcd_size) ||
			(MMIDC_QQVGA_PREVIEW_HEIGHT == lcd_size))
        {
            if (size_array[PHOTO_SIZE_320X240] <= max_photo_size)
            {
                if (index < max_item)
                {
                    size_list_ptr[index] = PHOTO_SIZE_320X240;
                    index ++;
                }
            }
        }

        switch(lcd_size)
        {
        case MMIDC_HVGA_PREVIEW_HEIGHT:
            hor_size = PHOTO_SIZE_480X320;
            ver_size = PHOTO_SIZE_320X480;
            break;
            
        case MMIDC_WQVGA_PREVIEW_HEIGHT:  
            hor_size = PHOTO_SIZE_400X240;
            ver_size = PHOTO_SIZE_240X400;
            break;
            
        case MMIDC_QVGA_PREVIEW_HEIGHT:
            hor_size = PHOTO_SIZE_320X240;
            ver_size = PHOTO_SIZE_240X320;
            break;
            
        case MMIDC_QCIF_PREVIEW_HEIGHT:
            hor_size = PHOTO_SIZE_220X176;
            ver_size = PHOTO_SIZE_176X220;
            break;
		case MMIDC_QQVGA_PREVIEW_HEIGHT:            
            hor_size = PHOTO_SIZE_160X128;
 			ver_size = PHOTO_SIZE_128X160;
            break;     
		default:
            break;
        }
        
        if (((SCREEN_MODE_HORIIZONTAL == screen_mode)
            && (hor_size != PHOTO_SIZE_320X240)
            && (((LCD_ANGLE_90 == MMIAPIDC_GetCameraSensorAngle()) || (LCD_ANGLE_270 == MMIAPIDC_GetCameraSensorAngle()))
            || (((LCD_ANGLE_0 == MMIAPIDC_GetCameraSensorAngle()) || (LCD_ANGLE_180 == MMIAPIDC_GetCameraSensorAngle())) && MMIDC_IsRealHorLcd()))
           )
           || ( DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type 
                 && LCD_ANGLE_90 == current_angle
                 && (hor_size != PHOTO_SIZE_320X240)
              )
           )
        {
            if (index < max_item)
            {
                size_list_ptr[index] = hor_size;
                index ++;
            }
        }

        if (MMIDC_WQVGA_PREVIEW_HEIGHT == lcd_size)
        {
            if (size_array[PHOTO_SIZE_320X240] <= max_photo_size)
            {
                if (index < max_item)
                {
                    size_list_ptr[index] = PHOTO_SIZE_320X240;
                    index ++;
                }
            }
        }
        
    #if defined(MMIDC_DESKTOP_PDA)
        if (SCREEN_MODE_HORIIZONTAL == screen_mode)
        {
            is_show_ver_size = FALSE;
        }
    #endif

        if (is_show_ver_size) /*lint !e774*/
        {
            #if (defined(MAINLCD_LOGIC_ANGLE_0) || defined(MAINLCD_LOGIC_ANGLE_180))
                if (index < max_item)
                {
                    size_list_ptr[index] = ver_size;
                    index ++;
                }
            #endif 
        }

        if(MMIDC_HVGA_PREVIEW_HEIGHT == lcd_size)
        {
            if (size_array[PHOTO_SIZE_320X240] <= max_photo_size)
            {
                if (index < max_item)
                {
                    size_list_ptr[index] = PHOTO_SIZE_320X240;
                    index ++;
                }
            }
        }

  
    }
    else if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type)
    {
        //the size is  supported by 3p4 array
        total_3p4_array_number = ARR_SIZE(size_3p4_array);
        
        index = 0;
        for (i = 0; i < total_3p4_array_number; i++)
        {
            cur_size = size_3p4_array[i];
            if(cur_size > PHOTO_SIZE_240X320)
            {
                if (size_array[cur_size] <= max_photo_size)
                {
                    if (index < max_item)
                    {
                        size_list_ptr[index] = cur_size;
                        index ++;
                    }
                }
            }
        }

        if(MMIDC_QCIF_PREVIEW_HEIGHT == lcd_size)
        {
                if (size_array[PHOTO_SIZE_240X320] <= max_photo_size)
                {
                    if (index < max_item)
                    {
                        size_list_ptr[index] = PHOTO_SIZE_240X320;
                        index ++;
                    }
                }
        }

        switch(lcd_size)
        {
        case MMIDC_HVGA_PREVIEW_HEIGHT:
            ver_size = PHOTO_SIZE_320X480;
            break;
            
        case MMIDC_WQVGA_PREVIEW_HEIGHT:  
            ver_size = PHOTO_SIZE_240X400;
            break;
            
        case MMIDC_QVGA_PREVIEW_HEIGHT:
            ver_size = PHOTO_SIZE_240X320;
            break;
            
        case MMIDC_QCIF_PREVIEW_HEIGHT:
            ver_size = PHOTO_SIZE_176X220;
            break;
        case MMIDC_QQVGA_PREVIEW_HEIGHT:
            ver_size = PHOTO_SIZE_128X160;
            break;    
        default:
            break;
        }
        
    #if (defined(MAINLCD_LOGIC_ANGLE_0) || defined(MAINLCD_LOGIC_ANGLE_180))
        if (ver_size != PHOTO_SIZE_240X320)
        {
            if (index < max_item)
            {
                size_list_ptr[index] = ver_size;
                index ++;
            }
        }    
    #endif     

        if(MMIDC_QCIF_PREVIEW_HEIGHT != lcd_size)
        {
                if (size_array[PHOTO_SIZE_240X320] <= max_photo_size)
                {
                    if (index < max_item)
                    {
                        size_list_ptr[index] = PHOTO_SIZE_240X320;
                        index ++;
                    }
                }
        }

    
    }

    //SCI_TRACE_LOW:"[MMIDC] GetPhonePhotoSizeList index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_1139_112_2_18_2_9_30_481,(uint8*)"d", index);
    return index;
}

/*****************************************************************************/
//Description :  get phone photo size function list
//Global resource dependence : none
//Author: robert.wang
//Note: size list from  low to high
// Return: total supported number
/*****************************************************************************/
PUBLIC int16 GetPhonePhotoSizeFunctionList(int16 max_item,                                       
                                                  MENU_ITEM_CALL_BACK *func_list_ptr //out
                                                  )
{
    int16  total_item = 0;
    int16  copy_count = 0;
    MENU_ITEM_CALL_BACK callback[] =
    {
        Set128X160, 
        Set160X128,
        Set176X220, 
        Set220X176, 
        Set240X320,
        Set240X400,
        Set320X240,
        Set320X480,
        Set400X240,        
        Set480X320,
        Set480X640,         
        Set640X480,
        Set960X1280,        
        Set1280X960,
        Set1600X1200,
        Set2048X1536,
        Set2592X1944,
        Set3264X2448
    }; 
    uint32 total_bytes = 0;

    //check param
    total_item = ARR_SIZE(callback);
    if (PHOTO_SIZE_MAX != total_item) /*lint !e774*/
    {
        total_item = 0;
        
        //MMIDC_ASSERT_WARNING:"[MMIDC] GetPhonePhotoSizeFunctionList callback array"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_1180_112_2_18_2_9_30_482,(uint8*)"");
        return total_item;
    }

    if (PNULL != func_list_ptr)
    {
        copy_count = MIN(total_item, max_item);
        total_bytes = (uint16)copy_count * sizeof(MENU_ITEM_CALL_BACK);
        SCI_MEMCPY(func_list_ptr, callback, total_bytes);
    }
    //SCI_TRACE_LOW:"[MMIDC] GetPhonePhotoSizeFunctionList copy_count=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_1190_112_2_18_2_9_30_483,(uint8*)"d", copy_count);

    return copy_count;
}

/*****************************************************************************/
// 	Description : set photo env normal
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void SetPhotoEnvNormal(void)
{
    MMIDC_SetPhotoEnvironment(ENVIRONMENT_AUTO);
}

/*****************************************************************************/
// 	Description : set photo env night
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void SetPhotoEnvNight(void)
{
    MMIDC_SetPhotoEnvironment(ENVIRONMENT_NIGHT);
}

/*****************************************************************************/
// 	Description : set multishot 0
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetNoShot(void) 
{
    MMIDC_SetMultiShootEnum(MULTI_SHOOT_DISABLE);
}

/*****************************************************************************/
// 	Description : set multishot 3
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
//      目前除QVGA,VGA 尺寸外，其他尺寸都不支持连拍
//      例如176*220,220*176,240*320,240*400,400*240,1280*960
//      大于VGA尺寸的不支持连拍，在MMIDC_SetMultiShootEnum()中禁止的

/*****************************************************************************/
LOCAL void Set3Shot(void)
{

        MMIDC_SetMultiShootEnum(MULTI_SHOOT_3);
}

/*****************************************************************************/
// 	Description : set multishot 6
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
//      目前除QVGA,VGA 尺寸外，其他尺寸都不支持连拍
//      例如176*220,220*176,240*320,240*400,400*240,1280*960
//      大于VGA尺寸的不支持连拍，在MMIDC_SetMultiShootEnum()中禁止的

/*****************************************************************************/
LOCAL void Set6Shot(void)
{

        MMIDC_SetMultiShootEnum(MULTI_SHOOT_6);
}

/*****************************************************************************/
// 	Description : set multishot 9
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void Set9Shot(void) 
{
    MMIDC_SetMultiShootEnum(MULTI_SHOOT_9);
}

/*****************************************************************************/
// 	Description : set white balance
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetWBAuto(void)
{
    MMIDC_SetPhotoWhiteBalance(WHITE_BALANCE_AUTO);
}

/*****************************************************************************/
// 	Description : set white balance
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetWBIncandescence(void)
{
    MMIDC_SetPhotoWhiteBalance(WHITE_BALANCE_INCANDESCENCE);
}

/*****************************************************************************/
// 	Description : set white balance
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetWBFluorescent(void)
{
    MMIDC_SetPhotoWhiteBalance(WHITE_BALANCE_FLUORESCENT);
}

/*****************************************************************************/
// 	Description : set white balance
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetWBSun(void)
{
    MMIDC_SetPhotoWhiteBalance(WHITE_BALANCE_SUN);
}

/*****************************************************************************/
// 	Description : set white balance
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetWBCloudy(void)
{
    MMIDC_SetPhotoWhiteBalance(WHITE_BALANCE_CLOUDY);
}

/*****************************************************************************/
// 	Description : set self delay forbidden
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetNoDelay(void)
{
    MMIDC_SetSelfShootDelayTime(SELF_SHOOT_DISABLE);
}

/*****************************************************************************/
// 	Description : set self delay 3 s
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void Set3Delay(void) 
{
    MMIDC_SetSelfShootDelayTime(SELF_SHOOT_3S);
}

/*****************************************************************************/
// 	Description : set self delay 10 s
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void Set10Delay(void)
{
    MMIDC_SetSelfShootDelayTime(SELF_SHOOT_10S);
}

/*****************************************************************************/
// 	Description : set photo normal effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetPhotoEffectNormal(void)
{
    MMIDC_SetPhotoEffect(DC_EFFECT_NORMAL);
}

/*****************************************************************************/
// 	Description : set photo black white effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetPhotoEffectBlackWhite(void) 
{
    MMIDC_SetPhotoEffect(DC_EFFECT_BLACKWHITE);
}

/*****************************************************************************/
// 	Description : set photo red effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetPhotoEffectRed(void) 
{
    MMIDC_SetPhotoEffect(DC_EFFECT_RED);
}

/*****************************************************************************/
// 	Description : set photo green effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetPhotoEffectGreen(void) 
{
    MMIDC_SetPhotoEffect(DC_EFFECT_GREEN);
}

/*****************************************************************************/
// 	Description : set photo blue effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetPhotoEffectBlue(void) 
{
    MMIDC_SetPhotoEffect(DC_EFFECT_BLUE);
}

/*****************************************************************************/
// 	Description : set photo canvas effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetPhotoEffectCanvas(void) 
{
    MMIDC_SetPhotoEffect(DC_EFFECT_CANVAS);
}

/*****************************************************************************/
// 	Description : set photo negative effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetPhotoEffectNegative(void) 
{
    MMIDC_SetPhotoEffect(DC_EFFECT_NEGATIVE);
}

/*****************************************************************************/
// 	Description : set photo yellow effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetPhotoEffectYellow(void)
{
    MMIDC_SetPhotoEffect(DC_EFFECT_YELLOW);
}
#ifdef MMIDC_F_VIDEO_352X288
/*****************************************************************************/
// 	Description : set video size 352X288
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetVideo352X288(void)
{
    MMIDC_SetVideoSize(VIDEO_SIZE_352X288);
}
#endif
/*****************************************************************************/
// 	Description : set video size 320X240
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetVideo320X240(void)
{
    MMIDC_SetVideoSize(VIDEO_SIZE_320X240);
}

/*****************************************************************************/
//  Description : set video size 320X240
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void SetVideo240X176(void)
{
    MMIDC_SetVideoSize(VIDEO_SIZE_240X176);
}

/*****************************************************************************/
// 	Description : set video size 176x144
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetVideo176X144(void)
{
    MMIDC_SetVideoSize(VIDEO_SIZE_176X144);
}

/*****************************************************************************/
// 	Description : set video size 160X128
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetVideo160X128(void)
{
    MMIDC_SetVideoSize(VIDEO_SIZE_160X128);
}

/*****************************************************************************/
// 	Description : set video size 128X96
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetVideo128X96(void)
{
    MMIDC_SetVideoSize(VIDEO_SIZE_128X96);
}

/*****************************************************************************/
//Description : set video size
//lobal resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
LOCAL void SetVideo240X320(void)
{
    MMIDC_SetVideoSize(VIDEO_SIZE_240X320);
}

/*****************************************************************************/
//Description : set video size
//lobal resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
LOCAL void SetVideo176X240(void)
{
    MMIDC_SetVideoSize(VIDEO_SIZE_176X240);
}

/*****************************************************************************/
//Description : set video size
//lobal resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
LOCAL void SetVideo144X176(void)
{
    MMIDC_SetVideoSize(VIDEO_SIZE_144X176);
}

/*****************************************************************************/
//Description : set video size
//lobal resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
LOCAL void SetVideo128X160(void)
{
    MMIDC_SetVideoSize(VIDEO_SIZE_128X160);
}

/*****************************************************************************/
//Description : set video size
//lobal resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
LOCAL void SetVideo96X128(void)
{
    MMIDC_SetVideoSize(VIDEO_SIZE_96X128);
}

/*****************************************************************************/
//Description : get phone video size list
//lobal resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetPhoneVideoSizeList(int16 max_item, //in
                                                    VIDEO_SIZE_E *size_list_ptr //out
                                                    )
{
    VIDEO_SIZE_E size_array[VIDEO_SIZE_MAX] = {0};
    int32 total_item = 0;
    int32 i = 0;

    uint16 count = 0;
    VIDEO_SIZE_E max_size = 0;
    VIDEO_SIZE_E support_size = 0;

    max_size = MMIDC_GetCurMaxVideoSize();
    
    total_item = ARR_SIZE(size_array);

    total_item = MMIDC_GetSupportVideoSizeArray(total_item, size_array);

    total_item = MIN(total_item, max_item);

    for (i = total_item - 1; i >= 0; i--)
    {
        support_size = size_array[i];

        if (support_size <= max_size)
        {
            size_list_ptr[count] = support_size;
            count++;
        }    
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetPhoneVideoSizeList total_item=%d,count=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_1538_112_2_18_2_9_31_484,(uint8*)"dd",total_item, count);

    return count;
}
/*****************************************************************************/
//Description :  get phone video size function list
//Global resource dependence : none
//Author: robert.wang
//Note: size function lists from  high to low 
// function lists number and order are as same as size lists (MMIDC_GetPhoneVideoSizeList())
// Return: total supported number
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetPhoneVideoSizeFunctionList(int16 max_item, //in                                     
                                                               MENU_ITEM_CALL_BACK *func_list_ptr //out
                                                               )
{
    MENU_ITEM_CALL_BACK callback_array[] =
    {
        SetVideo96X128,     //VIDEO_SIZE_96X128,
        SetVideo128X160,    //VIDEO_SIZE_128X160,
        SetVideo144X176,    //VIDEO_SIZE_144X176,
        SetVideo176X240,    //VIDEO_SIZE_176X240,
        SetVideo240X320,    //VIDEO_SIZE_240X320,
        SetVideo128X96,     //VIDEO_SIZE_128X96,
        SetVideo160X128,    //VIDEO_SIZE_160X128,
        SetVideo176X144,    //VIDEO_SIZE_176X144,
        SetVideo240X176,    //VIDEO_SIZE_240X176,
        SetVideo320X240,     //VIDEO_SIZE_320X240,
#ifdef MMIDC_F_VIDEO_352X288
        SetVideo352X288     //VIDEO_SIZE_320X240,
#endif
    }; 

    
    uint16  copy_count = 0;
    uint32 total_bytes = 0;
    int16  total_item = 0;

    //check param
    total_item = ARR_SIZE(callback_array);
    if (VIDEO_SIZE_MAX != total_item) /*lint !e774*/
    {
        total_item = 0;
        
        return total_item;
    }
    
    if (PNULL != func_list_ptr)
    {
        copy_count = MIN(total_item, max_item);
        total_bytes = copy_count * sizeof(func_list_ptr[0]);
        SCI_MEMCPY(func_list_ptr, callback_array, total_bytes);
    }
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetPhoneVideoSizeFunctionList copy_count=%d, total_item=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_1588_112_2_18_2_9_31_485,(uint8*)"dd",copy_count, total_item);

    return copy_count;
}

/*****************************************************************************/
// 	Description : set video normal effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetVideoEffectNormal(void) 
{
    MMIDC_SetVideoEffect(DC_EFFECT_NORMAL);
}

/*****************************************************************************/
// 	Description : set video black and white effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetVideoEffectBlackWhite(void)
{
    MMIDC_SetVideoEffect(DC_EFFECT_BLACKWHITE);
}

/*****************************************************************************/
// 	Description : set video red effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetVideoEffectRed(void) 
{
    MMIDC_SetVideoEffect(DC_EFFECT_RED);
}

/*****************************************************************************/
// 	Description : set video green effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetVideoEffectGreen(void) 
{
    MMIDC_SetVideoEffect(DC_EFFECT_GREEN);
}

/*****************************************************************************/
// 	Description : set video green effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetVideoEffectYellow(void) 
{
    MMIDC_SetVideoEffect(DC_EFFECT_YELLOW);
}

/*****************************************************************************/
// 	Description : set video blue effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetVideoEffectBlue(void)
{
    MMIDC_SetVideoEffect(DC_EFFECT_BLUE);
}

/*****************************************************************************/
// 	Description : set video canvas effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetVideoEffectCanvas(void)
{
    MMIDC_SetVideoEffect(DC_EFFECT_CANVAS);
}

/*****************************************************************************/
// 	Description : set video negative effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetVideoEffectNegative(void) 
{
    MMIDC_SetVideoEffect(DC_EFFECT_NEGATIVE);
}

/*****************************************************************************/
// 	Description : open my photo folder
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenMyPhoto(void)
{
	MMIDC_OpenPictureWin();
}

/*****************************************************************************/
// 	Description : open video settings window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetRecordFileType(uint32 index)
{
#if defined MMIDC_MINI_DISPLAY_STYLE
	if(1 == index)
    {
#if defined(AVI_ENC_SUPPORT) && !defined(CMCC_TEST_FLAG)
        MMIDC_SetRecordFileType(VIDEO_FILE_TYPE_AVI);
#endif
    }
    else if(0 == index)
    {
        MMIDC_SetRecordFileType(VIDEO_FILE_TYPE_3GP);
    }   
#else
	if(0 == index)
    {
#if defined(AVI_ENC_SUPPORT) && !defined(CMCC_TEST_FLAG)
        MMIDC_SetRecordFileType(VIDEO_FILE_TYPE_AVI);
#else
        MMIDC_SetRecordFileType(VIDEO_FILE_TYPE_MP4);
#endif
    }
    else if(1 == index)
    {
        MMIDC_SetRecordFileType(VIDEO_FILE_TYPE_3GP);
    }   
#endif
}

/*****************************************************************************/
// 	Description : open video settings window for setting config
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetRecordFileTypeOfSetting(uint32 index)
{
    SetRecordFileType(index);
}

/*****************************************************************************/
// 	Description : open video settings window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL VIDEO_FILE_TYPE_E GetRecordFileType(void)
{
#if defined MMIDC_MINI_DISPLAY_STYLE 
    if(VIDEO_FILE_TYPE_AVI == MMIDC_GetRecordFileType())
    {
        return 1;/*lint !e64*/
    }
    else if(VIDEO_FILE_TYPE_3GP == MMIDC_GetRecordFileType())
    {       
		return 0;/*lint !e64*/
    }
#else
	if(VIDEO_FILE_TYPE_AVI == MMIDC_GetRecordFileType())
    {
        return 0;/*lint !e64*/
    }
    else if(VIDEO_FILE_TYPE_MP4 == MMIDC_GetRecordFileType())
    {
        return 0;/*lint !e64*/
    }
    else if(VIDEO_FILE_TYPE_3GP == MMIDC_GetRecordFileType())
    {       
		return 1;/*lint !e64*/
    }
#endif
    return 0;/*lint !e64*/
}

/*****************************************************************************/
// 	Description : open video settings window for setting config
//	Global resource dependence : none
//  Author: robert.wang 
//	Note:
/*****************************************************************************/
PUBLIC VIDEO_FILE_TYPE_E MMIDC_GetRecordFileTypeOfSetting(void)
{
    return GetRecordFileType();
}

/*****************************************************************************/
// 	Description : open video settings window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenSetting(void)
{
/*
    if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
    {
        OpenPhotoSettings();
    }
    else
    {
        OpenVideoSettings();
    }
*/    
}

extern PUBLIC MMI_RESULT_E MMIDC_HandleDeletePubWinBG(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E	msg_id, DPARAM param);

/*****************************************************************************/
// 	Description : continue to shoot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void ContinueShoot(void) 
{
    MMIDC_CloseAllOSDMenu();
    MMK_SendMsg(MMIDC_MAIN_WIN_ID, MSG_APP_CANCEL, 0);
}

/*****************************************************************************/
// 	Description : display delete tip
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void DeleteTip(void)
{
    MMIPUB_OpenQueryWinByTextIdEx(SPRD_CAMERA_APPLET_ID,\
                                     TXT_DELETE,\
                                     IMAGE_PUBWIN_QUERY,\
                                     PNULL,\
                                     MMIDC_HandleDeletePubWinBG,\
                                     PNULL \
                                     );
}

/*****************************************************************************/
// 	Description : open send window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SendWin(void) 
{
    MMIDC_OpenSendPhotoWin(); 
}

/*****************************************************************************/
// 	Description : open my video folder
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenMyVideo(void) 
{
    MMIDC_CloseAllOSDMenu();
    MMIDC_OpenMovieWin();
    MMIDC_SetCurrentMode(DV_PREVIEW_MODE);
}

/*****************************************************************************/
// 	Description : get option menu position
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
//  Param:
//        Output: x_ptr and y_ptr  position for showing
/*****************************************************************************/
LOCAL void GetOptionMenuPosition(int16 *x_ptr, //out
                                        int16 *y_ptr  //out
                                        )
{
    int16 screen_height = MMIDC_GetPreviewHeight();    
    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();
    int16 item_height = MMIDC_ITEM_TEXT_HEIGHT;

    if (MMIDC_LAYOUT_LEFTWARD == layout_style)
    {
        int16 key_width = MMIDC_GetMaxWidthOfSoftkey();
        
        *x_ptr = key_width + 1;
        *y_ptr = screen_height - 1;
    }
    else if(MMIDC_LAYOUT_RIGHTWARD == layout_style)
    {
        *x_ptr = 0;
        *y_ptr = screen_height - 1;
    }
    else
    {
        *x_ptr = 0;
        *y_ptr = screen_height - item_height;
    }
}

/*****************************************************************************/
// 	Description : open video option window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenVideoOption(void)
{
#if !defined(MMIDC_DESKTOP_PDA)
    DC_MENU_ITEMS_OP* osd_menu = MMIDC_InitMenu();
    int16 x = 0;
    int16 y = 0;
    int16 index = 0;
    MMIDC_DESKTOP_STYLE_E desktop = 0;

    if (PNULL == osd_menu)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_OpenVideoOption osd_menu = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_1845_112_2_18_2_9_31_486,(uint8*)"");
        return ;
    }

    desktop = MMIDC_GetDesktopStyle();

    GetOptionMenuPosition(&x, &y);
    osd_menu->MenuSetStartPositionX(x);
    osd_menu->MenuSetStartPositionY(y);

    osd_menu->MenuSetMenuTitle(STXT_OPTION);

#ifdef MMIDC_DESKTOP_ICON_BAR
    if (DESKTOP_STYLE_ICON == desktop)
    {
        osd_menu->MenuAppendItemText(TXT_MORE_OPTION);
    }
#endif    

#if defined (MMIDC_F_WORK_MODE)
    if(!MMIDC_IsWorkMode())
#endif
    {
        osd_menu->MenuAppendItemText(TXT_DV_RECORD_DIRECTORY);
    }

#ifdef MMIDC_MINI_RESOURCE_SIZE 
#if defined(MMIDC_F_WORK_MODE)
    if (!MMIDC_IsWorkMode())
#endif
    {
    	osd_menu->MenuAppendItemText(TXT_DC_SWITCH_PHOTO);
    }
	osd_menu->MenuAppendItemText(TXT_DC_EFFECT);
#endif

#ifdef MMIDC_ADD_SPECIAL_EFFECT
	osd_menu->MenuAppendItemText(TXT_COMM_CONTRAST);
	osd_menu->MenuAppendItemText(TXT_COMMON_COMMON_BRIGHTNESS);
#endif
    osd_menu->MenuAppendItemText(STXT_MAIN_SETTING);
#ifndef MMIDC_MINI_RESOURCE_SIZE     
    osd_menu->MenuAppendItemText(TXT_HELP);
#endif

    //set callback functions
#ifdef MMIDC_DESKTOP_ICON_BAR
    if (DESKTOP_STYLE_ICON == desktop)
    {
        osd_menu->MenuSetItemCallback(index++, MMIDC_OpenMoreOption);
    }
#endif 

#if defined (MMIDC_F_WORK_MODE)
    if(!MMIDC_IsWorkMode())
#endif
    {
        osd_menu->MenuSetItemCallback(index++, OpenMyVideo);
    }


#ifdef MMIDC_MINI_RESOURCE_SIZE
#if defined(MMIDC_F_WORK_MODE)
    if (!MMIDC_IsWorkMode())
#endif
    {
        osd_menu->MenuSetItemCallback(index++, SetPhotoMode);
    }
	osd_menu->MenuSetItemCallback(index++, MMIDC_OpenEffectOption);
#endif    

#ifdef MMIDC_ADD_SPECIAL_EFFECT 
	osd_menu->MenuSetItemCallback(index++, MMIDC_OpenContrastAdjustWin);
	osd_menu->MenuSetItemCallback(index++, MMIDC_OpenBrightnessAdjustWin);
#endif
    osd_menu->MenuSetItemCallback(index++, MMIDC_OpenSettingWin);
    /*if (DESKTOP_STYLE_ICON != desktop)
    {
        osd_menu->MenuSetItemCallback(index++, MMIDC_OpenStorageOption);
    }*/
#ifndef MMIDC_MINI_RESOURCE_SIZE
    osd_menu->MenuSetItemCallback(index++, MMIDC_OpenHelpWin);
#endif


    osd_menu->MenuSelectItem(0);
    osd_menu->MenuFocusItem(0);
    osd_menu->MenuSetDisplayMode(DISPLAY_DIRECT_VERTICAL);
    osd_menu->MenuSetFocusItemBackGroundColor(MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
    osd_menu->MenuHideSelectedSign();
    osd_menu->MenuSetNotCallbackFocus();    

    MMIDC_SetShowState(MMIDC_SHOW_MENU);
#endif    
}

/*****************************************************************************/
// 	Description : get current reviewing photo id
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetCurrentPhotoID(void)
{
    return s_focused_photo;
}

/*****************************************************************************/
// 	Description : reset current reviewing photo id
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ResetCurrentPhotoID(void)
{
    s_focused_photo = 0;
}

/*****************************************************************************/
// 	Description : move the current pos
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_MoveCurReviewPhotoID(MOVE_DIRECTION_E direction)
{	
    uint32 photo_total = MMIDC_GetCapturedPhotosNumber();
    
    switch(direction)
    {
    case MOVE_UP:
        if(s_focused_photo > 0)
        {
            s_focused_photo --;
        }
        else
        {
            s_focused_photo = photo_total - 1;
        }
        break;
        
    case MOVE_DOWN:
        if(s_focused_photo < photo_total - 1)
        {
            s_focused_photo ++;
        }
        else
        {
            s_focused_photo = 0;
        }
        break;
        
    case MOVE_LEFT:
        if(s_focused_photo > 0)
        {
            s_focused_photo --;
        }
        else
        {
            s_focused_photo = photo_total - 1;
        }
        break;
        
    case MOVE_RIGHT:
        if(s_focused_photo < photo_total - 1)
        {
            s_focused_photo ++;
        }
        else
        {
            s_focused_photo = 0;
        }
        break;
        
    default:
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_MoveCurReviewPhotoID() KEY"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_1972_112_2_18_2_9_32_487,(uint8*)"");
        break;
    }
}

/*****************************************************************************/
// 	Description : open video review option window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenVideoReviewOption(void)
{
#if !defined(MMIDC_DESKTOP_PDA)
    DC_MENU_ITEMS_OP* osd_menu = MMIDC_InitMenu();
    int16 x = 0;
    int16 y = 0;


    if (PNULL == osd_menu)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_OpenVideoReviewOption osd_menu = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_1991_112_2_18_2_9_32_488,(uint8*)"");
        return ;
    }

    osd_menu->MenuSetMenuTitle(STXT_OPTION);
    
    osd_menu->MenuAppendItemText(TXT_DV_SHOOT_NEW);
    osd_menu->MenuAppendItemText(TXT_DELETE);
    osd_menu->MenuAppendItemText(TXT_SEND);
#ifdef VIDEO_PLAYER_SUPPORT
    osd_menu->MenuAppendItemText(TXT_COMMON_PLAY);
#endif
    osd_menu->MenuAppendItemText(TXT_DV_RECORD_DIRECTORY);
    
    GetOptionMenuPosition(&x, &y);    
    osd_menu->MenuSetStartPositionX(x);
    osd_menu->MenuSetStartPositionY(y);
        
    osd_menu->MenuSelectItem(0);
    osd_menu->MenuFocusItem(0);
    osd_menu->MenuSetDisplayMode(DISPLAY_DIRECT_VERTICAL);
    osd_menu->MenuSetFocusItemBackGroundColor(MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
    osd_menu->MenuHideSelectedSign();
    osd_menu->MenuSetNotCallbackFocus();
    
    osd_menu->MenuSetItemCallback(0, ContinueShoot);
    osd_menu->MenuSetItemCallback(1, DeleteTip);
    osd_menu->MenuSetItemCallback(2, SendWin);
#ifdef VIDEO_PLAYER_SUPPORT
    osd_menu->MenuSetItemCallback(3, MMIDC_ReviewVideo);
    osd_menu->MenuSetItemCallback(4, OpenMyVideo);    
    osd_menu->MenuSetDisplayNumber(5);
#else
    osd_menu->MenuSetItemCallback(3, OpenMyVideo);    
    osd_menu->MenuSetDisplayNumber(4);
#endif

    MMIDC_SetShowState(MMIDC_SHOW_MENU);
#endif    
}

/*****************************************************************************/
// 	Description : set change folder state
//	Global resource dependence : none
//  Author: robert.wang
//   Date: 2009-9-8
//	Note:  查看图片管理中浏览默认路径下的图片
//     应允许浏览其他文件夹的图片，以此来区别
//    相关联的函数 mmipicview_wintab.c   
//    HandleLoadAllPicWinMsg() ,HandlePiclistOptWinMsg()
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetChangeFolderState(BOOLEAN is_changed)    
{
	s_is_changed_folder = is_changed;
}

/*****************************************************************************/
// 	Description : get change folder state
//	Global resource dependence : none
//  Author: robert.wang
//   Date: 2009-9-8
//	Note:  查看图片管理中浏览默认路径下的图片
//     应允许浏览其他文件夹的图片，以此来区别
//    相关联的函数 mmipicview_wintab.c   
//    HandleLoadAllPicWinMsg() ,HandlePiclistOptWinMsg()
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_GetChangeFolderState(void)    
{
	return s_is_changed_folder;
}

/*****************************************************************************/
//Description :  shortcut for switch sensor
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForSwitchSensor(void)
{
    SETTING_SENSOR_ID_E   sensor_id = 0;
    int32  sensor_number = 0;

    sensor_number = MMIDC_GetDCameraSensorNumber();
    if (MAX_MMI_SENSOR_NUM <= sensor_number)
    {
        sensor_id = MMIDC_GetSettingSensorID();

        //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForSwitchSensor sensor_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_2072_112_2_18_2_9_32_489,(uint8*)"d", sensor_id);
        if (SETTING_SENSOR_REAR == sensor_id)
        {
            SetFrontSensor();
        }
        else
        {
            SetRearSensor();
        }
    }
}

#if !defined(MMIDC_DESKTOP_PDA)
/*****************************************************************************/
// 	Description : set brightness function
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenBrightnessMenu(uint32 x, uint32 y)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE    
    static uint32 old_x = 0;
    static uint32 old_y = 0;

    if(x == 0 && y == 0)
    {
        x = old_x;
        y = old_y;
    }
    else
    {
        old_x = x;
        old_y = y;
    }
    
    //close menu 
    MMIDC_GetMenuHandle()->MenuClose();


    s_adjust_data.x = x;
    s_adjust_data.y = y;
    
    s_adjust_data.menu = BRIGHTNESS_ADJUST_MENU;
#endif
}

/*****************************************************************************/
// 	Description : set contrast function
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenContrastMenu(uint32 x, uint32 y)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE   
    static uint32 old_x = 0;
    static uint32 old_y = 0;

    if(x == 0 && y == 0)
    {
        x = old_x;
        y = old_y;
    }
    else
    {
        old_x = x;
        old_y = y;
    }
    //close menu 
    MMIDC_GetMenuHandle()->MenuClose();
    
    s_adjust_data.x = x;
    s_adjust_data.y = y;
    
    s_adjust_data.menu = CONTRAST_ADJUST_MENU;
#endif
}

/*****************************************************************************/
// 	Description : set exposal compensate function
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenExposalCompensateMenu(uint32 x, uint32 y)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE   
    static uint32 old_x = 0;
    static uint32 old_y = 0;

    if(x == 0 && y == 0)
    {
        x = old_x;
        y = old_y;
    }
    else
    {
        old_x = x;
        old_y = y;
    }
    //close menu 
    MMIDC_GetMenuHandle()->MenuClose();
    
    s_adjust_data.x = x;
    s_adjust_data.y = y;
    
    s_adjust_data.menu = EC_ADJUST_MENU;
#endif
}

/*****************************************************************************/
// 	Description : adjust lower
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void AdjustLow(void)
{
    switch(s_adjust_data.menu)
    {
    case BRIGHTNESS_ADJUST_MENU:
        if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
        {
            if(MMIDC_GetPhotoBrightness() > BRIGHTNESS_LEVEL_0)
            {
                MMIDC_SetPhotoBrightness((BRIGHTNESS_E)(MMIDC_GetPhotoBrightness() - 1));
            }
        }
        else
        {
            if(MMIDC_GetVideoBrightness() > BRIGHTNESS_LEVEL_0)
            {
                MMIDC_SetVideoBrightness((BRIGHTNESS_E)(MMIDC_GetVideoBrightness() - 1));
            }
        }
        OpenBrightnessMenu(0, 0);
        break;
    case CONTRAST_ADJUST_MENU:
        if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
        {
            if(MMIDC_GetContrast() > CONTRAST_0)
            {
                MMIDC_SetContrast((CONTRAST_E)(MMIDC_GetContrast() - 1));
            }
        }
        else
        {
            if(MMIDC_GetVideoContrast() > CONTRAST_0)
            {
                MMIDC_SetVideoContrast((CONTRAST_E)(MMIDC_GetVideoContrast() - 1));
            }
        }
        OpenContrastMenu(0, 0);
        break;
    case EC_ADJUST_MENU:
        if(MMIDC_GetExposalCompensate() > EXPOSAL_COMPENSATE_MINUS_3)
        {
            MMIDC_SetExposalCompensate((EXPOSAL_COMPENSATE_E)(MMIDC_GetExposalCompensate() - 1));
        }
        OpenExposalCompensateMenu(0, 0);
        break;
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : adjust higher
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void AdjustHigh(void)
{
    switch(s_adjust_data.menu)
    {
    case BRIGHTNESS_ADJUST_MENU:
        if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
        {
            if(MMIDC_GetPhotoBrightness() < BRIGHTNESS_LEVEL_6)
            {
                MMIDC_SetPhotoBrightness((BRIGHTNESS_E)(MMIDC_GetPhotoBrightness() + 1));
            }
        }
        else
        {
            if(MMIDC_GetVideoBrightness() < BRIGHTNESS_LEVEL_6)
            {
                MMIDC_SetVideoBrightness((BRIGHTNESS_E)(MMIDC_GetVideoBrightness() + 1));
            }
        }
        OpenBrightnessMenu(0, 0);
        break;
    case CONTRAST_ADJUST_MENU:
        if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
        {
            if(MMIDC_GetContrast() < CONTRAST_6)
            {
                MMIDC_SetContrast((CONTRAST_E)(MMIDC_GetContrast() + 1));
            }
        }
        else
        {
            if(MMIDC_GetVideoContrast() < CONTRAST_6)
            {
                MMIDC_SetVideoContrast((CONTRAST_E)(MMIDC_GetVideoContrast() + 1));
            }
        }
        OpenContrastMenu(0, 0);
        break;
    case EC_ADJUST_MENU:
        if(MMIDC_GetExposalCompensate() < EXPOSAL_COMPENSATE_PLUS_3)
        {
            MMIDC_SetExposalCompensate((EXPOSAL_COMPENSATE_E)(MMIDC_GetExposalCompensate() + 1));
        }
        OpenExposalCompensateMenu(0, 0);
        break;
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : handle adjust key function
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void CloseAdjust(void)
{
    MMIDC_ClearAdjustRect();    
    
    SCI_MEMSET(&s_adjust_data,0x00,sizeof(s_adjust_data));
    s_adjust_data.menu = ADJUST_MENU_MAX;
}

/*****************************************************************************/
// 	Description : open multishot setting menu
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenMultiShotMenu(uint32 x, uint32 y)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE    
    DC_MENU_ITEMS_OP* osd_menu = MMIDC_InitMenu();
    MULTI_SHOOT_E  max_photo_number = 0;
    int32   i = 0;
//    MMIDC_PHOTO_SIZE_E photo_size = 0;

    if (PNULL == osd_menu)
    {
        //SCI_TRACE_LOW:"[MMIDC] OpenMultiShotMenu osd_menu = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_2304_112_2_18_2_9_32_490,(uint8*)"");
        return ;
    }

    //像框不支持连拍
    if (FRAME_DISABLE != MMIDC_GetFrameIndex())
    {
        max_photo_number = (uint32)MULTI_SHOOT_DISABLE;
    }
    else
    {
        max_photo_number = MULTI_SHOOT_6;
        //photo_size = MMIDC_GetPhotoSize();                                         
        //max_photo_number = MMIDC_GetBurstNumberByPhotoSize(photo_size);           
    }

    osd_menu->MenuSetMenuTitle(TXT_DC_MULTI_SHOT);
    osd_menu->MenuAppendItemText(TXT_COMMON_COMMON_DISABLE);

    osd_menu->MenuAppendItemText(TXT_DC_3_MULTI_SHOT);
    osd_menu->MenuAppendItemText(TXT_DC_6_MULTI_SHOT);
    //osd_menu->MenuAppendItemText(TXT_DC_9_MULTI_SHOT);

    for (i = (int32)(MULTI_SHOOT_MAX - 1); i > (int32)max_photo_number; i--)
    {
        osd_menu->MenuSetItemGray(i);
    }
 

    osd_menu->MenuSetStartPositionX(x);
    osd_menu->MenuSetStartPositionY(y);
    osd_menu->MenuSelectItem(MMIDC_GetMultiShootEnum());
    osd_menu->MenuFocusItem(MMIDC_GetMultiShootEnum());
    osd_menu->MenuSetDisplayMode(DISPLAY_DIRECT_VERTICAL);
    osd_menu->MenuSetFocusItemBackGroundColor(MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
    
    osd_menu->MenuSetItemCallback(0, SetNoShot);
    osd_menu->MenuSetItemCallback(1, Set3Shot);
    osd_menu->MenuSetItemCallback(2, Set6Shot);
    //osd_menu->MenuSetItemCallback(3, Set9Shot);


    osd_menu->MenuSetNotCallbackFocus();
#endif
}

/*****************************************************************************/
// 	Description : open photo effect menu
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenPhotoEffectMenu(uint32 x, uint32 y)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE    
    DC_MENU_ITEMS_OP* osd_menu = MMIDC_InitMenu();

    if (PNULL == osd_menu)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] OpenPhotoEffectMenu osd_menu = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_2359_112_2_18_2_9_32_491,(uint8*)"");
        return ;
    }
    
    osd_menu->MenuSetMenuTitle(TXT_DC_EFFECT);
    osd_menu->MenuAppendItemText(TXT_COMMON_COMMON_NORMAL);
    osd_menu->MenuAppendItemText(TXT_DC_MODE_BANDW);
    osd_menu->MenuAppendItemText(TXT_DC_MODE_BLUE);
    osd_menu->MenuAppendItemText(TXT_DC_MODE_GREEN);
    osd_menu->MenuAppendItemText(TXT_DC_MODE_YELLOW);
    osd_menu->MenuAppendItemText(TXT_DC_MODE_RED);
    osd_menu->MenuAppendItemText(TXT_DC_MODE_CANVAS);
    osd_menu->MenuAppendItemText(TXT_DV_MODE_NEGATIVE);

    osd_menu->MenuSetItemCallback(0, SetPhotoEffectNormal);
    osd_menu->MenuSetItemCallback(1, SetPhotoEffectBlackWhite);
    osd_menu->MenuSetItemCallback(2, SetPhotoEffectBlue);
    osd_menu->MenuSetItemCallback(3, SetPhotoEffectGreen);
    osd_menu->MenuSetItemCallback(4, SetPhotoEffectYellow);
    osd_menu->MenuSetItemCallback(5, SetPhotoEffectRed);
    osd_menu->MenuSetItemCallback(6, SetPhotoEffectCanvas);
    osd_menu->MenuSetItemCallback(7, SetPhotoEffectNegative);
    
    osd_menu->MenuSetStartPositionX(x);
    osd_menu->MenuSetStartPositionY(y);
    osd_menu->MenuSelectItem(MMIDC_GetPhotoEffect());
    osd_menu->MenuFocusItem(MMIDC_GetPhotoEffect());
    osd_menu->MenuSetDisplayMode(DISPLAY_DIRECT_VERTICAL);
    osd_menu->MenuSetFocusItemBackGroundColor(MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
#endif
}

/*****************************************************************************/
// 	Description : open photo env setting menu
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenPhotoEnvironmentMenu(uint32 x, uint32 y)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE     
    DC_MENU_ITEMS_OP* osd_menu = MMIDC_InitMenu();

    if (PNULL == osd_menu)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] OpenPhotoEnvironmentMenu osd_menu = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_2400_112_2_18_2_9_32_492,(uint8*)"");
        return ;
    }
    
    osd_menu->MenuSetMenuTitle(TXT_NIGHT_MODE);
    osd_menu->MenuAppendItemText(TXT_ACTIVE_OPERATIVE);
    osd_menu->MenuAppendItemText(TXT_CLOSE);
    osd_menu->MenuSetStartPositionX(x);
    osd_menu->MenuSetStartPositionY(y);
    osd_menu->MenuSelectItem(MMIDC_GetPhotoEnvironment());
    osd_menu->MenuFocusItem(MMIDC_GetPhotoEnvironment());
    osd_menu->MenuSetDisplayMode(DISPLAY_DIRECT_VERTICAL);
    osd_menu->MenuSetFocusItemBackGroundColor(MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
    osd_menu->MenuSetItemCallback(0, SetPhotoEnvNight);
    osd_menu->MenuSetItemCallback(1, SetPhotoEnvNormal);
#endif
}

#ifdef MMIDC_F_FLASH
/*****************************************************************************/
// 	Description : set flash on
//	Global resource dependence : none
//  Author: chunyou
//	Note:
/*****************************************************************************/
LOCAL void SetFlashOn(void)
{
    MMIDC_SetFlashLamp(DCAMERA_FLASH_ON);
}

/*****************************************************************************/
// 	Description : set flash off
//	Global resource dependence : none
//  Author: chunyou
//	Note:
/*****************************************************************************/
LOCAL void SetFlashOff(void)
{
    MMIDC_SetFlashLamp(DCAMERA_FLASH_OFF);
}

/*****************************************************************************/
// 	Description : set flash auto
//	Global resource dependence : none
//  Author: chunyou
//	Note:
/*****************************************************************************/
LOCAL void SetFlashAuto(void)
{
    MMIDC_SetFlashLamp(DCAMERA_FLASH_AUTO);
}

/*****************************************************************************/
// 	Description : open photo flash setting menu
//	Global resource dependence : none
//  Author: chunyou
//	Note:
/*****************************************************************************/
LOCAL void OpenFlashMenu(uint32 x, uint32 y)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE     
    DC_MENU_ITEMS_OP* osd_menu = MMIDC_InitMenu();

#if defined(MMIDC_F_U_CAMERA) && defined(MMIDC_F_N_IN_ONE)
	int16 ninone_width = 0;
	int16 ninone_height = 0;
#endif

    if (PNULL == osd_menu)
    {
        SCI_TRACE_LOW("[MMIDC] OpenFlashMenu osd_menu = PNULL");
        return ;
    }

    osd_menu->MenuSetMenuTitle(TXT_DC_FLASH);
    osd_menu->MenuAppendItemText(TXT_CLOSE);
    osd_menu->MenuAppendItemText(TXT_OPEN);
    if(CAMERA_MODE_DC == MMIDC_GetCameraMode())
    {
        osd_menu->MenuAppendItemText(TXT_AUTO);
    }
    
#if defined(MMIDC_F_U_CAMERA) && defined(MMIDC_F_N_IN_ONE)
	MMIDC_GetNInOneDisplayWidthAndHeight(&ninone_width, &ninone_height);
	if(MMIDC_IsUCameraEnable() && MMIDC_IsNInOneEnable())
	{		
		if((x > ninone_width))
		{
			x = x - ninone_width;
		}
		else
		{
			x = 1;	
		}		
	}
#endif	
	
	osd_menu->MenuSetStartPositionX(x);
    osd_menu->MenuSetStartPositionY(y);
    osd_menu->MenuSelectItem(MMIDC_GetFlashLamp());
    osd_menu->MenuFocusItem(MMIDC_GetFlashLamp());
    osd_menu->MenuSetDisplayMode(DISPLAY_DIRECT_VERTICAL);
    osd_menu->MenuSetFocusItemBackGroundColor(MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
    osd_menu->MenuSetItemCallback(0, SetFlashOff);
    osd_menu->MenuSetItemCallback(1, SetFlashOn);
    if(CAMERA_MODE_DC == MMIDC_GetCameraMode())
    {
        osd_menu->MenuSetItemCallback(2, SetFlashAuto);
    }

    if(SETTING_SENSOR_FRONT == MMIDC_GetSettingSensorID())
    {
        osd_menu->MenuSetItemGray(0);
        osd_menu->MenuSetItemGray(1);
        if(CAMERA_MODE_DC == MMIDC_GetCameraMode())
        {
            osd_menu->MenuSetItemGray(2);
        }
    }
#endif
}
#endif

#ifdef CAMERA_FRAME_SUPPORT
extern PUBLIC void MMIDC_DisplayFrame0(void);
extern PUBLIC void MMIDC_DisplayFrame1(void);
extern PUBLIC void MMIDC_DisplayFrame2(void);
extern PUBLIC void MMIDC_DisplayFrame3(void);
extern PUBLIC void MMIDC_DisplayFrame4(void);

/*****************************************************************************/
// 	Description : open frame setting menu
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenPhotoFrameMenu(uint32 x, uint32 y)
{
    DC_MENU_ITEMS_OP* osd_menu = MMIDC_InitMenu();

    if (PNULL == osd_menu)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] OpenPhotoFrameMenu osd_menu = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_2434_112_2_18_2_9_32_493,(uint8*)"");
        return ;
    }
    
    osd_menu->MenuSetMenuTitle(TXT_DC_CIRCLE);
    
    osd_menu->MenuAppendItemText(TXT_DC_NO_CIRCLE);
    osd_menu->MenuSetItemCallback(0, MMIDC_DisplayFrame0);
    osd_menu->MenuAppendItemText(TXT_DC_CIRCLE1);
    osd_menu->MenuAppendItemText(TXT_DC_CIRCLE2);
    osd_menu->MenuAppendItemText(TXT_DC_CIRCLE3);
    osd_menu->MenuAppendItemText(TXT_DC_CIRCLE4);
    osd_menu->MenuSetItemCallback(1, MMIDC_DisplayFrame1);
    osd_menu->MenuSetItemCallback(2, MMIDC_DisplayFrame2);
    osd_menu->MenuSetItemCallback(3, MMIDC_DisplayFrame3);
    osd_menu->MenuSetItemCallback(4, MMIDC_DisplayFrame4);
    osd_menu->MenuSelectItem(MMIDC_GetFrameIndex());
    osd_menu->MenuFocusItem(MMIDC_GetFrameIndex());
    
    osd_menu->MenuSetStartPositionX(x);
    osd_menu->MenuSetStartPositionY(y);
    osd_menu->MenuSetDisplayMode(DISPLAY_DIRECT_VERTICAL);
    osd_menu->MenuSetFocusItemBackGroundColor(MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
}
#endif

/*****************************************************************************/
// 	Description : open photo size setting menu
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void PhotoSizeMenu(SCREEN_MODE_E screen_mode,int32 lcd_size,uint32 x, uint32 y)
{
    DC_MENU_ITEMS_OP* osd_menu = MMIDC_InitMenu();
    uint32 index = 0;
    uint32 focus_index = 0;
    int32  i = 0;
    MENU_ITEM_CALL_BACK func_list[PHOTO_SIZE_MAX] = {0};
    MMIDC_PHOTO_SIZE_E size_array[PHOTO_SIZE_MAX] = {0};
    MMI_TEXT_ID_T      text_id = TXT_NULL;
    int16  total_size_number = 0;
    MMIDC_PHOTO_SIZE_E  cur_photo_size = 0;
    MMIDC_PHOTO_SIZE_E  var_size = 0;
    int16  total_func_number = 0;
    

    osd_menu->MenuSetMenuTitle(TXT_COMM_SIZE);
    osd_menu->MenuSetStartPositionX(x);
    osd_menu->MenuSetStartPositionY(y);
    osd_menu->MenuSetDisplayMode(DISPLAY_DIRECT_VERTICAL);
    osd_menu->MenuSetFocusItemBackGroundColor(MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));


    cur_photo_size = MMIDC_GetPhotoSize();
    //size list from high to low
    total_size_number = GetPhonePhotoSizeList(ARR_SIZE(size_array), lcd_size, screen_mode, size_array);
    //get all size function lis
    total_func_number = GetPhonePhotoSizeFunctionList(ARR_SIZE(func_list), func_list);
    if (PHOTO_SIZE_MAX != total_func_number)
    {
        return ;
    }
    
    for (i = 0; i < total_size_number; i++)
    {
        if (size_array[i] < PHOTO_SIZE_MAX)
        {
            var_size = size_array[i];
            text_id = var_size;
            osd_menu->MenuAppendItemText(text_id);
            osd_menu->MenuSetItemCallback(index, func_list[var_size]);
            if(cur_photo_size == var_size)
            {
                focus_index = index;
            }
            index ++;
        }
    }

    osd_menu->MenuSelectItem(focus_index);
    osd_menu->MenuFocusItem(focus_index);
    osd_menu->MenuSetNotCallbackFocus();
}

/*****************************************************************************/
// 	Description : open photo size setting menu
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenPhotoSizeHorWQVGA(uint32 x, uint32 y)
{
    PhotoSizeMenu(SCREEN_MODE_HORIIZONTAL, MMIDC_WQVGA_PREVIEW_HEIGHT, x, y);
}

/*****************************************************************************/
// 	Description : open photo size setting menu
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenPhotoSizeVerWQVGA(uint32 x, uint32 y)
{
    PhotoSizeMenu(SCREEN_MODE_VERTICAL, MMIDC_WQVGA_PREVIEW_HEIGHT, x, y);
}

/*****************************************************************************/
// 	Description : open photo size setting menu
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenPhotoSizeVerQVGA(uint32 x, uint32 y)
{
    PhotoSizeMenu(SCREEN_MODE_VERTICAL, MMIDC_QVGA_PREVIEW_HEIGHT, x, y);
}

/*****************************************************************************/
// 	Description : open photo size setting menu
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenPhotoSizeHorQVGA(uint32 x, uint32 y)
{
    PhotoSizeMenu(SCREEN_MODE_HORIIZONTAL, MMIDC_QVGA_PREVIEW_HEIGHT, x, y);
}

/*****************************************************************************/
// 	Description : open photo size setting menu
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenPhotoSizeVerQCIF(uint32 x, uint32 y)
{
    PhotoSizeMenu(SCREEN_MODE_VERTICAL, MMIDC_QCIF_PREVIEW_HEIGHT, x, y);
}

/*****************************************************************************/
// 	Description : open photo size setting menu
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenPhotoSizeHorQCIF(uint32 x, uint32 y)
{
    PhotoSizeMenu(SCREEN_MODE_HORIIZONTAL, MMIDC_QCIF_PREVIEW_HEIGHT, x, y);
}

/*****************************************************************************/
// 	Description : open photo size setting menu
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void OpenPhotoSizeVerHVGA(uint32 x, uint32 y)
{
    PhotoSizeMenu(SCREEN_MODE_VERTICAL, MMIDC_HVGA_PREVIEW_HEIGHT, x, y);
}

/*****************************************************************************/
// 	Description : open photo size setting menu
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void OpenPhotoSizeHorHVGA(uint32 x, uint32 y)
{
    PhotoSizeMenu(SCREEN_MODE_HORIIZONTAL, MMIDC_HVGA_PREVIEW_HEIGHT, x, y);
}

/*****************************************************************************/
// 	Description : open self delay setting menu
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenSelfDelayMenu(uint32 x, uint32 y)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE    
    DC_MENU_ITEMS_OP* osd_menu = MMIDC_InitMenu();

#if defined(MMIDC_F_U_CAMERA) && defined(MMIDC_F_N_IN_ONE)
	int16 ninone_width = 0;
	int16 ninone_height = 0;
#endif

    if (PNULL == osd_menu)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] OpenSelfDelayMenu osd_menu = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_2599_112_2_18_2_9_33_494,(uint8*)"");
        return ;
    }
    
    osd_menu->MenuSetMenuTitle(TXT_DC_SELF_SHOT);
    osd_menu->MenuAppendItemText(TXT_COMMON_COMMON_DISABLE);
    osd_menu->MenuAppendItemText(TXT_COMM_3_SECONDS);
    osd_menu->MenuAppendItemText(TXT_COMM_10_SECONDS);

#if defined(MMIDC_F_U_CAMERA) && defined(MMIDC_F_N_IN_ONE)	
	MMIDC_GetNInOneDisplayWidthAndHeight(&ninone_width, &ninone_height);
	if(MMIDC_IsUCameraEnable() && MMIDC_IsNInOneEnable())
	{
		if((x > ninone_width))
		{
			x = x - ninone_width;
		}
		else
		{
			x = 1;	
		}
	}
#endif

    osd_menu->MenuSetStartPositionX(x);
    osd_menu->MenuSetStartPositionY(y);
    osd_menu->MenuSelectItem(MMIDC_GetSelfShootDelayTime());
    osd_menu->MenuFocusItem(MMIDC_GetSelfShootDelayTime());
    osd_menu->MenuSetDisplayMode(DISPLAY_DIRECT_VERTICAL);
    osd_menu->MenuSetFocusItemBackGroundColor(MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
    osd_menu->MenuSetItemCallback(0, SetNoDelay);
    osd_menu->MenuSetItemCallback(1, Set3Delay);
    osd_menu->MenuSetItemCallback(2, Set10Delay);
#endif
}

/*****************************************************************************/
// 	Description : switch sensor menu
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void OpenSwitchSensorMenu(uint32 x, uint32 y)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE    
    DC_MENU_ITEMS_OP* osd_menu = MMIDC_InitMenu();
    uint16 focus_index = 0;
    SETTING_SENSOR_ID_E  sensor_id = 0;

    if (PNULL == osd_menu)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] OpenDCModeMenu osd_menu = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_2630_112_2_18_2_9_33_495,(uint8*)"");
        return ;
    }
    
    osd_menu->MenuSetMenuTitle(TXT_SWITCH_SENSOR);
    osd_menu->MenuAppendItemText(TXT_PREPOSITIVE_SENSOR);
    osd_menu->MenuAppendItemText(TXT_POSTPOSITIVE_SENSOR);

#if defined(MMIDC_F_U_CAMERA) && defined(MMIDC_F_N_IN_ONE)	
	if(MMIDC_IsUCameraEnable() && MMIDC_IsNInOneEnable())
	{
		x = 1;		
	}
#endif

    osd_menu->MenuSetStartPositionX(x);
    osd_menu->MenuSetStartPositionY(y);

    sensor_id = MMIDC_GetSettingSensorID();
    switch(sensor_id)
    {
    case SETTING_SENSOR_REAR:
        focus_index = 1;
        break;
    case SETTING_SENSOR_FRONT:
        focus_index = 0;
        break;
    default:
        break;
    }
    
    osd_menu->MenuSelectItem(focus_index);
    osd_menu->MenuFocusItem(focus_index);
    osd_menu->MenuSetDisplayMode(DISPLAY_DIRECT_VERTICAL);
    osd_menu->MenuSetFocusItemBackGroundColor(MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
    osd_menu->MenuSetItemCallback(0, SetFrontSensor);
    osd_menu->MenuSetItemCallback(1, SetRearSensor);
    osd_menu->MenuSetNotCallbackFocus();
#endif
}

/*****************************************************************************/
// 	Description : open video effect menu
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenVideoEffectMenu(uint32 x, uint32 y)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE     
    DC_MENU_ITEMS_OP* osd_menu = MMIDC_InitMenu();

    if (PNULL == osd_menu)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] OpenVideoEffectMenu osd_menu = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_2673_112_2_18_2_9_33_496,(uint8*)"");
        return ;
    }
    
    osd_menu->MenuSetMenuTitle(TXT_DC_EFFECT);
    
    osd_menu->MenuAppendItemText(TXT_COMMON_COMMON_NORMAL);
    osd_menu->MenuAppendItemText(TXT_DC_MODE_BANDW);
    osd_menu->MenuAppendItemText(TXT_DC_MODE_BLUE);
    osd_menu->MenuAppendItemText(TXT_DC_MODE_GREEN);
    osd_menu->MenuAppendItemText(TXT_DC_MODE_YELLOW);
    osd_menu->MenuAppendItemText(TXT_DC_MODE_RED);
    osd_menu->MenuAppendItemText(TXT_DC_MODE_CANVAS);
    osd_menu->MenuAppendItemText(TXT_DV_MODE_NEGATIVE);
    
    osd_menu->MenuSetItemCallback(0, SetVideoEffectNormal);
    osd_menu->MenuSetItemCallback(1, SetVideoEffectBlackWhite);
    osd_menu->MenuSetItemCallback(2, SetVideoEffectBlue);
    osd_menu->MenuSetItemCallback(3, SetVideoEffectGreen);
    osd_menu->MenuSetItemCallback(4, SetVideoEffectYellow);
    osd_menu->MenuSetItemCallback(5, SetVideoEffectRed);
    osd_menu->MenuSetItemCallback(6, SetVideoEffectCanvas);
    osd_menu->MenuSetItemCallback(7, SetVideoEffectNegative);

    osd_menu->MenuSetStartPositionX(x);
    osd_menu->MenuSetStartPositionY(y);
    osd_menu->MenuSelectItem(MMIDC_GetVideoEffect());
    osd_menu->MenuFocusItem(MMIDC_GetVideoEffect());
    osd_menu->MenuSetDisplayMode(DISPLAY_DIRECT_VERTICAL);
    osd_menu->MenuSetFocusItemBackGroundColor(MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
#endif
}

/*****************************************************************************/
// 	Description : open video size setting menu
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenVideoSizeMenu(uint32 x, uint32 y)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE     
    DC_MENU_ITEMS_OP* osd_menu = MMIDC_InitMenu();
    

    MENU_ITEM_CALL_BACK func_list[VIDEO_SIZE_MAX] = {0};
    VIDEO_SIZE_E size_array[VIDEO_SIZE_MAX] = {0};
    MMI_TEXT_ID_T      text_id = TXT_NULL;
    VIDEO_SIZE_E  cur_size = 0;
    VIDEO_SIZE_E  var_size = 0;
    uint16  total_func_number = 0;
    uint16  total_size_number = 0;
    
    uint32 index = 0;
    uint32 focus_index = 0;
    int32  i = 0;

    
    if (PNULL == osd_menu)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] OpenVideoSizeMenu osd_menu = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_2729_112_2_18_2_9_33_497,(uint8*)"");
        return ;
    }

    osd_menu->MenuSetMenuTitle(TXT_COMM_SIZE);
    osd_menu->MenuSetStartPositionX(x);
    osd_menu->MenuSetStartPositionY(y);
    osd_menu->MenuSetDisplayMode(DISPLAY_DIRECT_VERTICAL);
    osd_menu->MenuSetFocusItemBackGroundColor(MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));

    cur_size = MMIDC_GetVideoSize();

    //SCI_TRACE_LOW:"[MMIDC] OpenVideoSizeMenu cur_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_2742_112_2_18_2_9_33_498,(uint8*)"d",cur_size);
        
    //size list from high to low
    total_size_number = MMIDC_GetPhoneVideoSizeList(ARR_SIZE(size_array),size_array);
    //get all size function list
    total_func_number = MMIDC_GetPhoneVideoSizeFunctionList(ARR_SIZE(func_list), func_list);
    if (VIDEO_SIZE_MAX != total_func_number)
    {
        //SCI_TRACE_LOW:"[MMIDC] OpenVideoSizeMenu total_size_number=%d, total_func_number=%d error!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_2750_112_2_18_2_9_33_499,(uint8*)"dd", total_size_number, total_func_number);
        return ;
    }

    index = 0;
    for (i = 0; i < total_size_number; i++)
    {
        if (size_array[i] < VIDEO_SIZE_MAX)
        {
            var_size = size_array[i];
            text_id = var_size;
            osd_menu->MenuAppendItemText(text_id);
            osd_menu->MenuSetItemCallback(index, func_list[var_size]);
            if(cur_size == var_size)
            {
                focus_index = index;
            }
            index ++;
        }
    }

    osd_menu->MenuSelectItem(focus_index);
    osd_menu->MenuFocusItem(focus_index);
    osd_menu->MenuSetNotCallbackFocus();
#endif
}

/*****************************************************************************/
// 	Description : set photo white balance function
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void OpenWhiteBalanceMenu(uint32 x, uint32 y)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE     
    DC_MENU_ITEMS_OP* osd_menu = MMIDC_InitMenu();

    if (PNULL == osd_menu)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] OpenWhiteBalanceMenu osd_menu = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_2786_112_2_18_2_9_33_500,(uint8*)"");
        return ;
    }

    
    osd_menu->MenuSetMenuTitle(TXT_DC_WHITE_BALANCE);
    osd_menu->MenuAppendItemText(TXT_AUTO);
    osd_menu->MenuAppendItemText(TXT_DC_INCANDESCENCE);
    osd_menu->MenuAppendItemText(TXT_DC_FLUORESCENT);
    osd_menu->MenuAppendItemText(TXT_DC_SUN);
    osd_menu->MenuAppendItemText(TXT_DC_CLOUDY);
    osd_menu->MenuSetStartPositionX(x);
    osd_menu->MenuSetStartPositionY(y);
    switch(MMIDC_GetPhotoWhiteBalance())
    {
    case WHITE_BALANCE_AUTO:
        osd_menu->MenuSelectItem(0);
        osd_menu->MenuFocusItem(0);
    	break;
        
    case WHITE_BALANCE_INCANDESCENCE:
        osd_menu->MenuSelectItem(1);
        osd_menu->MenuFocusItem(1);
        break;
        
    case WHITE_BALANCE_FLUORESCENT:
        osd_menu->MenuSelectItem(2);
        osd_menu->MenuFocusItem(2);
        break;
        
    case WHITE_BALANCE_SUN:
        osd_menu->MenuSelectItem(3);
        osd_menu->MenuFocusItem(3);
        break;
    
    case WHITE_BALANCE_CLOUDY:
        osd_menu->MenuSelectItem(4);
        osd_menu->MenuFocusItem(4);
        break;
    default:
        //MMIDC_ASSERT_WARNING:"[MMIDC] OpenWhiteBalanceMenu() TYPE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_2826_112_2_18_2_9_33_501,(uint8*)"");
        break;
    }   
    osd_menu->MenuSetDisplayMode(DISPLAY_DIRECT_VERTICAL);
    osd_menu->MenuSetFocusItemBackGroundColor(MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
    osd_menu->MenuSetItemCallback(0, SetWBAuto);
    osd_menu->MenuSetItemCallback(1, SetWBIncandescence);
    osd_menu->MenuSetItemCallback(2, SetWBFluorescent);
    osd_menu->MenuSetItemCallback(3, SetWBSun);
    osd_menu->MenuSetItemCallback(4, SetWBCloudy);
#endif
}

/*****************************************************************************/
// 	Description : open desktop tip OSD
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void	MMIDC_OpenDesktopTipOSD(void)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE
    DC_ICONS_OP* osd_icon = MMIAPIDC_InitIcon();
    int32       key_width = 0;
    int32  x = 0;
    int32  y = 0;
    int32  bottom_space = 0;
    uint16 icon_width = 0;
    uint16 focus_icon_width = 0;
    uint16 focus_icon_height = 0;
    uint16 icon_number = 0;    
    int16  icon_bar_width = 0;
    
    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();
    int16 screen_width = MMIDC_GetPreviewWidth();
    int16 screen_height = MMIDC_GetPreviewHeight();
    int32 lcd_size = 0;
    uint8   index = 0;

    int16  sensor_number = 0;
    
    BOOLEAN is_func_on = TRUE;

    if (PNULL == osd_icon)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] OpenPhotoEnvironmentMenu osd_icon = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_2867_112_2_18_2_9_33_502,(uint8*)"");
        return ;
    }

#if defined(MMIDC_F_U_CAMERA)	
    if(MMIDC_IsUCameraEnable())
    {
        is_func_on = FALSE;
    }
#endif

    osd_icon->IconSetDisplayMode(DISPLAY_DIRECT_HORIZONTAL);

    sensor_number = MMIDC_GetDCameraSensorNumber();
    if(CAMERA_MODE_DC == MMIDC_GetCameraMode())
    {
        if(is_func_on)/*lint !e774*/
       {
#if defined(MMIDC_F_WORK_MODE)
            if(!MMIDC_IsWorkMode())
#endif
            {
                osd_icon->IconAppendImage(IMG_DC_PHOTO_MODE, IMG_DC_PHOTO_MODE_FOCUS);
            }
        }
        
#ifdef MMIDC_F_FLASH
        osd_icon->IconAppendImage(IMG_DC_FLASH, IMG_DC_FLASH_FOCUS);//flash
#endif
        
        if (sensor_number >= MAX_MMI_SENSOR_NUM)
        {
            osd_icon->IconAppendImage(IMG_DC_SWITCH_SENSOR, IMG_DC_SWITCH_SENSOR_FOCUS);
        }
        if(is_func_on)/*lint !e774*/
       {
        osd_icon->IconAppendImage(IMG_DC_SIZE, IMG_DC_SIZE_FOCUS);
        osd_icon->IconAppendImage(IMG_DC_ENVIRONMENT, IMG_DC_ENVIRONMENT_FOCUS);
        osd_icon->IconAppendImage(IMG_DC_BRIGHTNESS, IMG_DC_BIRGHTNESS_FOCUS);
        osd_icon->IconAppendImage(IMG_DC_CONTRAST, IMG_DC_CONTRAST_FOCUS);

        //osd_icon->IconAppendImage(IMG_DC_ISO, IMG_DC_ISO_FOCUS);
        osd_icon->IconAppendImage(IMG_DC_EXPOSAL_COMPENSATE, IMG_DC_EXPOSAL_COMPENSATE_FOCUS);
#ifdef CAMERA_FRAME_SUPPORT        
        osd_icon->IconAppendImage(IMG_DC_FRAME, IMG_DC_FRAME_FOCUS);
#endif              
#if defined(MMIDC_F_WORK_MODE)
        if(!MMIDC_IsWorkMode())
#endif
        {

            osd_icon->IconAppendImage(IMG_DC_MULTISHOT, IMG_DC_MULTISHOT_FOCUS);
        }
        osd_icon->IconAppendImage(IMG_DC_WHITE_BALANCE, IMG_DC_WHITE_BALANCE_FOCUS);
        }

        osd_icon->IconAppendImage(IMG_DC_SELF_DELAY, IMG_DC_SELF_DELAY_FOCUS);
        if(is_func_on)/*lint !e774*/
        {
        osd_icon->IconAppendImage(IMG_DC_EFFECT, IMG_DC_EFFECT_FOCUS);
        }
        
        if(is_func_on)/*lint !e774*/
       {
#if defined(MMIDC_F_WORK_MODE)
            if(!MMIDC_IsWorkMode())
#endif
            {
                osd_icon->IconSetCallback(index, OpenDCModeMenu);
                index ++;
            }
        }
        
#ifdef MMIDC_F_FLASH
        osd_icon->IconSetCallback(index, OpenFlashMenu);
        index ++;
#endif
        
        if (sensor_number >= MAX_MMI_SENSOR_NUM)
        {
            osd_icon->IconSetCallback(index, OpenSwitchSensorMenu);
            index ++;
        }

        if(is_func_on)/*lint !e774*/
		{
        lcd_size = MMIDC_GetLCDSize();
        if(MMIDC_GetScreenMode() == SCREEN_MODE_HORIIZONTAL)
        {
            switch(lcd_size)
            {
            case MMIDC_HVGA_PREVIEW_HEIGHT:
                osd_icon->IconSetCallback(index, OpenPhotoSizeHorHVGA);
                break;
            case MMIDC_WQVGA_PREVIEW_HEIGHT:
                osd_icon->IconSetCallback(index, OpenPhotoSizeHorWQVGA);
                break;
            case MMIDC_QVGA_PREVIEW_HEIGHT:
                osd_icon->IconSetCallback(index, OpenPhotoSizeHorQVGA);
                break;
            case MMIDC_QCIF_PREVIEW_HEIGHT:
            default:
                osd_icon->IconSetCallback(index, OpenPhotoSizeHorQCIF);
                break;
            }
        }
        else
        {
            switch(lcd_size)
            {
            case MMIDC_HVGA_PREVIEW_HEIGHT:
                if(MMIDC_GetDefaultScreenMode() == SCREEN_MODE_VERTICAL)
                {
                    osd_icon->IconSetCallback(index, OpenPhotoSizeVerHVGA);
                }
                else
                {
                    osd_icon->IconSetCallback(index, OpenPhotoSizeHorHVGA);
                }
                break;
            case MMIDC_WQVGA_PREVIEW_HEIGHT:
                if(MMIDC_GetDefaultScreenMode() == SCREEN_MODE_VERTICAL)
                {
                    osd_icon->IconSetCallback(index, OpenPhotoSizeVerWQVGA);
                }
                else
                {
                    osd_icon->IconSetCallback(index, OpenPhotoSizeHorWQVGA);
                }
                break;
            case MMIDC_QVGA_PREVIEW_HEIGHT:
                osd_icon->IconSetCallback(index, OpenPhotoSizeVerQVGA);
                break;
            case MMIDC_QCIF_PREVIEW_HEIGHT:
            default:
                if(MMIDC_GetDefaultScreenMode() == SCREEN_MODE_VERTICAL)
                {
                    osd_icon->IconSetCallback(index, OpenPhotoSizeVerQCIF);
                }
                else
                {
                    osd_icon->IconSetCallback(index, OpenPhotoSizeHorQCIF);
                }
                break;
            }
        }
        index ++;
        
        osd_icon->IconSetCallback(index, OpenPhotoEnvironmentMenu);
        index ++;
        osd_icon->IconSetCallback(index, OpenBrightnessMenu);
        index ++;
        osd_icon->IconSetCallback(index, OpenContrastMenu);
        index ++;
        //osd_icon->IconSetCallback(index, OpenISOMenu);
        //index ++;
        osd_icon->IconSetCallback(index, OpenExposalCompensateMenu);
        index ++;
#ifdef CAMERA_FRAME_SUPPORT        
        osd_icon->IconSetCallback(index, OpenPhotoFrameMenu);
        index ++;
#endif        
#if defined(MMIDC_F_WORK_MODE)
        if(!MMIDC_IsWorkMode())
#endif
        {
            osd_icon->IconSetCallback(index, OpenMultiShotMenu);
            index ++;
        }
        osd_icon->IconSetCallback(index, OpenWhiteBalanceMenu);
        index ++;
        }

        
        osd_icon->IconSetCallback(index, OpenSelfDelayMenu);
        index ++;
        if(is_func_on)/*lint !e774*/
		{
            osd_icon->IconSetCallback(index, OpenPhotoEffectMenu);
            index ++;
        }
       
    }
    else if(CAMERA_MODE_DV == MMIDC_GetCameraMode())
    {
#if defined(MMIDC_F_WORK_MODE)
        if(!MMIDC_IsWorkMode())
#endif
        {
            osd_icon->IconAppendImage(IMG_DC_PHOTO_MODE, IMG_DC_PHOTO_MODE_FOCUS);
        }

#ifdef MMIDC_F_FLASH
        osd_icon->IconAppendImage(IMG_DC_FLASH, IMG_DC_FLASH_FOCUS);
#endif

        if (sensor_number >= MAX_MMI_SENSOR_NUM)
        {
            osd_icon->IconAppendImage(IMG_DC_SWITCH_SENSOR, IMG_DC_SWITCH_SENSOR_FOCUS);
        }
        osd_icon->IconAppendImage(IMG_DC_SIZE, IMG_DC_SIZE_FOCUS);
        //osd_icon->IconAppendImage(IMG_DC_ENVIRONMENT, IMG_DC_ENVIRONMENT_FOCUS);
        osd_icon->IconAppendImage(IMG_DC_BRIGHTNESS, IMG_DC_BIRGHTNESS_FOCUS);
        osd_icon->IconAppendImage(IMG_DC_CONTRAST, IMG_DC_CONTRAST_FOCUS);
        osd_icon->IconAppendImage(IMG_DC_EFFECT, IMG_DC_EFFECT_FOCUS);
        
#if defined(MMIDC_F_WORK_MODE)
        if(!MMIDC_IsWorkMode())
#endif
        {
            osd_icon->IconSetCallback(index, OpenDCModeMenu);
            index++;
        }

#ifdef MMIDC_F_FLASH
        osd_icon->IconSetCallback(index, OpenFlashMenu);
        index ++;
#endif

        if (sensor_number >= MAX_MMI_SENSOR_NUM)
        {
            osd_icon->IconSetCallback(index, OpenSwitchSensorMenu);
            index++;
        }
        osd_icon->IconSetCallback(index, OpenVideoSizeMenu);
        index++;
        //osd_icon->IconSetCallback(2, OpenVideoEnvironmentMenu);
        osd_icon->IconSetCallback(index, OpenBrightnessMenu);
        index++;
        osd_icon->IconSetCallback(index, OpenContrastMenu);
        index++;
        osd_icon->IconSetCallback(index, OpenVideoEffectMenu);
        index++;

    }

    icon_width = osd_icon->IconGetIconWidth();
    focus_icon_width = osd_icon->IconGetFocusIconWidth();
    focus_icon_height = osd_icon->IconGetFocusIconHeight();
    icon_number = osd_icon->IconGetDisplayNumber();
    icon_bar_width = focus_icon_width + icon_width * (icon_number - 1);
    
    key_width = MMIDC_GetMaxWidthOfSoftkey();       

    if(MMIDC_LAYOUT_LEFTWARD == layout_style)
    {
        bottom_space = 3;
        x = key_width + (screen_width - icon_bar_width - key_width)/2;
    }
    else if(MMIDC_LAYOUT_RIGHTWARD == layout_style)
    {
        bottom_space = 3;
        x = (screen_width - icon_bar_width - key_width)/2;
    }
    else
    {
        bottom_space = 2 + MMIDC_ITEM_TEXT_HEIGHT;
        x = (screen_width - icon_bar_width)/2;
    }
    y = screen_height - focus_icon_height - bottom_space;
    osd_icon->IconSetStartPositionX(x);
    osd_icon->IconSetStartPositionY(y);

    osd_icon->IconFocus(0);

    MMIDC_SetShowState(MMIDC_SHOW_ICON);
#endif //MMIDC_MINI_RESOURCE_SIZE
}	

//progress bar rect in background image 
LOCAL uint8 s_bar_bg_rect[5]=
{
#define MMIDC_HEIGHT_UNIT  12
    MMIDC_HEIGHT_UNIT, //unit height
    7, //left space
    4,//bottom space
    4,//top space
    0 //bg offset as "-" image
};

/*****************************************************************************/
// 	Description : display ajust context 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayAdjust(void)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE     
    int16 x = 0;
    int16 y = 0;

    MMI_STRING_T text_str = {0};
    int16 fontwidth = 0;
    int16 fontheight = 0;
    GUI_RECT_T text_rect = {0};
    GUI_RECT_T rect = {0};

    int16  item_height = MMIDC_ITEM_TEXT_HEIGHT; //added, @robert.wang, 09-9-10, cr150160

    uint16 icon_height = 0;
    uint16 icon_width = 0;   
 
    int16  bmp_x = 0;
    int16  bmp_y = 0;

    int32  adjust_start_space = MMIDC_ADJUST_ICON_SPACE;
    int16  screen_width = MMIDC_GetPreviewWidth();

    MMI_IMAGE_ID_T  img_id = IMAGE_NULL;
    MMI_IMAGE_ID_T  bmp_id = IMAGE_NULL;
    ADJUST_MENU_E   menu = s_adjust_data.menu;
    MMI_TEXT_ID_T   title_id = TXT_NULL;
    int16           level_value = 0;
    
    int16           left_offset = 0;
    int16           top_offset = 0;

    x = s_adjust_data.x;
    y = s_adjust_data.y;

    switch(menu)
    {
    case BRIGHTNESS_ADJUST_MENU:
        img_id = IMG_DC_BRIGHTNESS_ADJUST;
        title_id = TXT_COMMON_COMMON_BRIGHTNESS;
        bmp_id = IMG_DC_BRIGHTNESS_ADJUST;
        
        if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
        {
            level_value = MMIDC_GetPhotoBrightness();
        }
        else
        {
            level_value = MMIDC_GetVideoBrightness();
        }
        break;
    case CONTRAST_ADJUST_MENU:
        img_id = IMG_DC_CONTRAST_ADJUST;
        title_id = TXT_COMM_CONTRAST;
        bmp_id = IMG_DC_CONTRAST_ADJUST;
        
        if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
        {
            level_value = MMIDC_GetContrast();
        }
        else
        {
            level_value = MMIDC_GetVideoContrast();
        }
        break;
    case EC_ADJUST_MENU:
    default:
        img_id = IMG_DC_CONTRAST_ADJUST;
        title_id = TXT_DC_EXPOSAL;
        bmp_id = IMG_DC_EC_ADJUST;
        level_value = MMIDC_GetExposalCompensate();
        break;
    }
    //get icon width and height
    GUIRES_GetImgWidthHeight(&icon_width,&icon_height,img_id,MMIDC_MAIN_WIN_ID);

    //display title 
    MMI_GetLabelTextByLang(title_id, &text_str);	
    fontwidth = MMIDC_GetStringWidth(&text_str);
    fontheight = MMIDC_GetStringHeight(&text_str);

    rect.bottom = y;/*lint !e737*/
    rect.top = (rect.bottom - item_height + 1) ;
    rect.left = (int16)(x + (icon_width - fontwidth -adjust_start_space) / 2); /*lint !e737*/
    rect.right = rect.left + fontwidth +adjust_start_space - 1;    

    if(rect.right >= screen_width)
    {
        rect.right = screen_width - 1 - 1;
        rect.left = rect.right - fontwidth - adjust_start_space + 1;
    }
    if(rect.left <= 0)
    {
        rect.left = 1;
        rect.right = rect.left + fontwidth + adjust_start_space - 1;
    }

    //display background bmp
   MMIDC_DisplayTitleBackground(rect.left, rect.top,rect);

   text_rect.left =rect.left + (adjust_start_space)/2;
   text_rect.top =rect.top + (item_height - fontheight)/2;
   text_rect.right = rect.right;
    text_rect.bottom = rect.bottom;
    MMIDC_DisplayTitleString(text_rect, &text_str, MMIDC_GetTransparentColor(), FALSE);

    //display bmp
    bmp_x = rect.left + (fontwidth + adjust_start_space - icon_width)/2; /*lint !e737*/
    bmp_y = (int16)(y - icon_height - item_height - 1); /*lint !e737*/
    
    MMIDC_DisplayBmp(bmp_x, bmp_y, bmp_id);    

#ifdef MAINLCD_SIZE_320X480
    left_offset = 7;
    top_offset = 67;
#else
    left_offset = 3;
    top_offset = 31;
#endif
    
    if(level_value > 0)
    {
        MMIDC_DisplayBmp(bmp_x + left_offset, bmp_y+ top_offset, IMG_DC_ADJUST_1 + level_value - 1);
    }
    rect.top = bmp_y;

    s_adjust_data.rect.left = rect.left;
    s_adjust_data.rect.top = rect.top;
    s_adjust_data.rect.right = rect.right;
    s_adjust_data.rect.bottom = y;
#endif //MMIDC_MINI_RESOURCE_SIZE
}


#endif //#if !defined(MMIDC_DESKTOP_PDA)

/*****************************************************************************/
//Description :  open desktop layout
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenOSDIcons(void)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE
#if !defined(MMIDC_DESKTOP_PDA)
    MMIDC_DESKTOP_STYLE_E  layout = 0;

    layout = MMIDC_GetDesktopStyle();

    if (DESKTOP_STYLE_TIP == layout)
    {
        MMIDC_OpenDesktopTipOSD();
    }
#endif 
#endif
}
/*****************************************************************************/
// 	Description : ajust menu is opened or not
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsAdjustMenu(void)
{
    BOOLEAN        ret = FALSE;

    if (ADJUST_MENU_MAX != s_adjust_data.menu)
    {
        ret = TRUE;
    }
    
    return ret;
}

/*****************************************************************************/
// 	Description : clear adjust rect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearAdjustRect(void)
{
    GUI_COLOR_T color = MMIDC_GetTransparentColor();
    
     MMIDC_ClearRect(s_adjust_data.rect, color);
}
/*****************************************************************************/
// 	Description : init adjust data
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_InitAdjustData(void)
{
    SCI_MEMSET(&s_adjust_data,0x00,sizeof(s_adjust_data));
    s_adjust_data.menu = ADJUST_MENU_MAX;
}

/*****************************************************************************/
// 	Description : clean adjust rect
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseAdjust(void)
{
#if !defined(MMIDC_DESKTOP_PDA)
    CloseAdjust();
#endif    
}

/*****************************************************************************/
// 	Description : handle adjust key function
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_HandleAdjustTP(int16 x, int16 y)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE
#if !defined(MMIDC_DESKTOP_PDA)

    GUI_POINT_T point = {0};
#ifdef MAINLCD_SIZE_320X480
    int32  icon_decrease = 28 - 1;
    int32  icon_increase = 28 + 1;
#else    
    int32  icon_decrease = 12 - 1;
    int32  icon_increase = 12 + 1;
#endif    
    uint16 icon_width = 0;
    uint16 icon_height = 0;

    
    point.x = x;
    point.y = y;
    GUIRES_GetImgWidthHeight(&icon_width,&icon_height,IMG_DC_ADJUST_1,MMIDC_MAIN_WIN_ID);

    if(GUI_PointIsInRect(point, s_adjust_data.rect))
    {
        if(y <= s_adjust_data.rect.bottom && y > (s_adjust_data.rect.bottom - icon_decrease - MMIDC_ITEM_TEXT_HEIGHT))
        {
            AdjustLow();
        }
        else if(y <= (s_adjust_data.rect.top + 2*icon_increase) && (y > s_adjust_data.rect.top))
        {
            AdjustHigh();
        }
        else
        {
            int16 step = 0;
            int16 space = 0;

            space = (s_adjust_data.rect.bottom - MMIDC_ITEM_TEXT_HEIGHT - icon_decrease - 6 - y);
            if (space <= 4)
            {
                step = 0;
            }else
            {
                step =  space / ((icon_height + 1) / 6) + 1;
            }

            if(step <= 0)
            {
                step = 0;
            }
            else if(step > 6)
            {
                step = 6;
            }

            switch(s_adjust_data.menu)
            {
            case BRIGHTNESS_ADJUST_MENU:
                if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
                {
                    MMIDC_SetPhotoBrightness((BRIGHTNESS_E)step);
                }
                else
                {
                    MMIDC_SetVideoBrightness((BRIGHTNESS_E)step);
                }
                OpenBrightnessMenu(0, 0);
                break;
            case CONTRAST_ADJUST_MENU:
                if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
                {
                    MMIDC_SetContrast((CONTRAST_E)step);
                }
                else
                {
                    MMIDC_SetVideoContrast((CONTRAST_E)step);
                }
                OpenContrastMenu(0, 0);
                break;
            case EC_ADJUST_MENU:
                MMIDC_SetExposalCompensate((EXPOSAL_COMPENSATE_E)step);
                OpenExposalCompensateMenu(0, 0);
                break;
            default:
                break;
            }
        }

        return TRUE;
    }
#endif
#endif

    return FALSE;
}
/*****************************************************************************/
// 	Description : handle adjust key function
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_HandleAdjustKey(DIRECTION_KEY key)
{
#if !defined(MMIDC_DESKTOP_PDA)
    BOOLEAN res = TRUE;
    
    if(s_adjust_data.menu == ADJUST_MENU_MAX) // it indicates the adjust menu is not open yet
    {
        return FALSE;
    }
    switch(key)
    {
    case UP_KEY:
        AdjustHigh();
        break;
        
    case DOWN_KEY:
        AdjustLow();
        break;
        
    case LEFT_KEY:
        CloseAdjust();
        res = FALSE; 
        break;
        
    case RIGHT_KEY:
        CloseAdjust();
        res = FALSE; 
        break;
        
    case OK_KEY:
        break;
        
    case CANCEL_KEY:
        CloseAdjust();
        break;
        
    default:
        res = FALSE;
        break;
    }
    return res;
#else
    return FALSE;
#endif
}

/*****************************************************************************/
// 	Description : open photo option window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenPhotoOption(void)
{
#if !defined(MMIDC_DESKTOP_PDA)

    DC_MENU_ITEMS_OP* osd_menu = MMIDC_InitMenu();
    int16 x = 0;
    int16 y = 0;
    int16 index = 0;
    MMIDC_DESKTOP_STYLE_E desktop = 0;

    if (PNULL == osd_menu)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_OpenPhotoOption osd_menu = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_3774_112_2_18_2_9_35_503,(uint8*)"");
        return ;
    }

    desktop = MMIDC_GetDesktopStyle();

    GetOptionMenuPosition(&x, &y);
    osd_menu->MenuSetStartPositionX(x);
    osd_menu->MenuSetStartPositionY(y);
    //set title
    osd_menu->MenuSetMenuTitle(STXT_OPTION);

#if defined (MMIDC_F_U_CAMERA)
#if defined (MMIDC_F_WORK_MODE)
    if(!MMIDC_IsWorkMode())
#endif
    {
        osd_menu->MenuAppendItemText(TXT_EFFECT_SELECT);
    }
#endif


#ifdef MMIDC_DESKTOP_ICON_BAR
    if (DESKTOP_STYLE_ICON == desktop)
    {
        osd_menu->MenuAppendItemText(TXT_MORE_OPTION);
    }
#endif 

#if defined (MMIDC_F_WORK_MODE)
    if(!MMIDC_IsWorkMode())
#endif
    {
#ifdef PIC_VIEWER_SUPPORT
        osd_menu->MenuAppendItemText(TXT_ENTERTAIMENT_PIC);
#else
		osd_menu->MenuAppendItemText(TXT_DC_PIC);
#endif
    }
    
#ifdef MMIDC_MINI_RESOURCE_SIZE 
#if defined (MMIDC_F_WORK_MODE)
    if(!MMIDC_IsWorkMode())
#endif
	{
		osd_menu->MenuAppendItemText(TXT_DC_SWITCH_VIDEO);
	}
	
	osd_menu->MenuAppendItemText(TXT_DC_EFFECT);
#endif 	

#ifdef MMIDC_ADD_SPECIAL_EFFECT
	osd_menu->MenuAppendItemText(TXT_COMM_CONTRAST);
	osd_menu->MenuAppendItemText(TXT_COMMON_COMMON_BRIGHTNESS);
	osd_menu->MenuAppendItemText(TXT_DC_WHITE_BALANCE);
#endif
    osd_menu->MenuAppendItemText(STXT_MAIN_SETTING);
    
#ifndef MMIDC_MINI_RESOURCE_SIZE   
    osd_menu->MenuAppendItemText(TXT_HELP);
#endif
    

#if defined (MMIDC_F_U_CAMERA)
#if defined (MMIDC_F_WORK_MODE)
    if(!MMIDC_IsWorkMode())
#endif
    {
        osd_menu->MenuSetItemCallback(index++,MMIDC_OpenUCameraWin);
    }
#endif

#ifdef MMIDC_DESKTOP_ICON_BAR    
    if (DESKTOP_STYLE_ICON == desktop)
    {
        osd_menu->MenuSetItemCallback(index++, MMIDC_OpenMoreOption);
    }
#endif  

#if defined (MMIDC_F_WORK_MODE)
    if(!MMIDC_IsWorkMode())
#endif
    {
        osd_menu->MenuSetItemCallback(index++, OpenMyPhoto);
    }

#ifdef MMIDC_MINI_RESOURCE_SIZE 
#if defined (MMIDC_F_WORK_MODE)
    if(!MMIDC_IsWorkMode())
#endif
	{
		osd_menu->MenuSetItemCallback(index++, SetVideoMode);
	}
	osd_menu->MenuSetItemCallback(index++, MMIDC_OpenEffectOption);
#endif    

#ifdef MMIDC_ADD_SPECIAL_EFFECT 
	osd_menu->MenuSetItemCallback(index++, MMIDC_OpenContrastAdjustWin);
	osd_menu->MenuSetItemCallback(index++, MMIDC_OpenBrightnessAdjustWin);
	osd_menu->MenuSetItemCallback(index++, MMIDC_OpenWhiteBalanceOption);
#endif
    osd_menu->MenuSetItemCallback(index++, MMIDC_OpenSettingWin);
#ifndef MMIDC_MINI_RESOURCE_SIZE	
    osd_menu->MenuSetItemCallback(index++, MMIDC_OpenHelpWin);
#endif

    osd_menu->MenuSelectItem(0);
    osd_menu->MenuFocusItem(0);
    osd_menu->MenuSetDisplayMode(DISPLAY_DIRECT_VERTICAL);
    osd_menu->MenuSetFocusItemBackGroundColor(MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
    osd_menu->MenuHideSelectedSign();
    osd_menu->MenuSetNotCallbackFocus();
    
    MMIDC_SetShowState(MMIDC_SHOW_MENU);
#endif    
}

#if defined (MMI_UDISK_MEM_ENABLE) && !defined (MMI_DV_UDISK_ENABLE)  
/*****************************************************************************/
// 	Description : set dv is in save path state or not
//	Global resource dependence : none
//  Author: dream.chen
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetDVSavePathState(BOOLEAN is_in_dv)
{
       s_in_dv_save_path = is_in_dv;
}

/*****************************************************************************/
// 	Description : get dv is in save path state or not
//	Global resource dependence : none
//  Author: dream.chen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_GetDVSavePathState( void)
{
       return s_in_dv_save_path;
}
#endif

#if defined(MMIDC_DESKTOP_PDA)
/*****************************************************************************/
//Description :  get dc mode list items
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetDCModeListItems(MMIDC_LIST_CTRL_ITEM_T *list_item_ptr //out
                                       )
{
    uint16 max_item_num = 0;
    uint16 cur_item = 0;    
    MMI_TEXT_ID_T text_array[]=
    {
        TXT_DC_SWITCH_PHOTO,
        TXT_DC_SWITCH_VIDEO
    };
    
    cur_item = MMIDC_GetCameraMode();
    max_item_num = ARR_SIZE(text_array);
    
    list_item_ptr->cur_item = cur_item;
    list_item_ptr->max_item_num =  max_item_num;
    list_item_ptr->title_text_id = TXT_DC_MODE;
    SCI_MEMCPY(list_item_ptr->text_array, text_array, sizeof(text_array));
    list_item_ptr->item_callback[0] = SetPhotoMode;
    list_item_ptr->item_callback[1] = SetVideoMode;
    
    return max_item_num;
}

/*****************************************************************************/
//Description :  get dc white blance list items
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetDCWhiteBlanceListItems(MMIDC_LIST_CTRL_ITEM_T *list_item_ptr //out
                                              )
{
    uint16 max_item_num = 0;
    uint16 cur_item = 0;    
    MMI_TEXT_ID_T text_array[]=
    {
        TXT_AUTO,
        TXT_DC_INCANDESCENCE,
        TXT_DC_FLUORESCENT,
        TXT_DC_SUN,
        TXT_DC_CLOUDY
    };
    switch(MMIDC_GetPhotoWhiteBalance())
    {
    case WHITE_BALANCE_AUTO:
        cur_item = 0;
        break;
        
    case WHITE_BALANCE_INCANDESCENCE:
        cur_item = 1;
        break;
        
    case WHITE_BALANCE_FLUORESCENT:
        cur_item = 2;
        break;
        
    case WHITE_BALANCE_SUN:
        cur_item = 3;
        break;    
        
    case WHITE_BALANCE_CLOUDY:
        cur_item = 4;
        break;
        
    default:
        break;
    }   
    max_item_num = ARR_SIZE(text_array);
    
    list_item_ptr->cur_item = cur_item;
    list_item_ptr->max_item_num =  max_item_num;
    list_item_ptr->title_text_id = TXT_DC_WHITE_BALANCE;
    SCI_MEMCPY(list_item_ptr->text_array, text_array, sizeof(text_array));
    list_item_ptr->item_callback[0] = SetWBAuto;
    list_item_ptr->item_callback[1] = SetWBIncandescence;
    list_item_ptr->item_callback[2] = SetWBFluorescent;
    list_item_ptr->item_callback[3] = SetWBSun;
    list_item_ptr->item_callback[4] = SetWBCloudy;
    
    return max_item_num;
}

/*****************************************************************************/
//Description :  get dc effect list items
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetDCEffectListItems(MMIDC_LIST_CTRL_ITEM_T *list_item_ptr //out
                                         )
{
    uint16 max_item_num = 0;
    uint16 cur_item = 0;    
    CAMERA_MODE_E   mode = MMIDC_GetCameraMode();    
    MMI_TEXT_ID_T text_array[]=
    {
        TXT_COMMON_COMMON_NORMAL, 
        TXT_DC_MODE_BANDW,  
        TXT_DC_MODE_BLUE,    
        TXT_DC_MODE_GREEN,
        TXT_DC_MODE_YELLOW,
        TXT_DC_MODE_RED,
        TXT_DC_MODE_CANVAS,
        TXT_DV_MODE_NEGATIVE   
    };
    if(CAMERA_MODE_DC == mode)
    {
        cur_item = MMIDC_GetPhotoEffect();
        max_item_num = ARR_SIZE(text_array);
        
        list_item_ptr->cur_item = cur_item;
        list_item_ptr->max_item_num =  max_item_num;
        list_item_ptr->title_text_id = TXT_DC_EFFECT;
        SCI_MEMCPY(list_item_ptr->text_array, text_array, sizeof(text_array));
        list_item_ptr->item_callback[0] = SetPhotoEffectNormal;
        list_item_ptr->item_callback[1] = SetPhotoEffectBlackWhite;
        list_item_ptr->item_callback[2] = SetPhotoEffectBlue;
        list_item_ptr->item_callback[3] = SetPhotoEffectGreen;
        list_item_ptr->item_callback[4] = SetPhotoEffectYellow;
        list_item_ptr->item_callback[5] = SetPhotoEffectRed;
        list_item_ptr->item_callback[6] = SetPhotoEffectCanvas;
        list_item_ptr->item_callback[7] = SetPhotoEffectNegative;
    }
    else
    {
        cur_item = MMIDC_GetVideoEffect();
        max_item_num = ARR_SIZE(text_array);
        
        list_item_ptr->cur_item = cur_item;
        list_item_ptr->max_item_num =  max_item_num;
        list_item_ptr->title_text_id = TXT_DC_EFFECT;
        SCI_MEMCPY(list_item_ptr->text_array, text_array, sizeof(text_array));
        list_item_ptr->item_callback[0] = SetVideoEffectNormal;
        list_item_ptr->item_callback[1] = SetVideoEffectBlackWhite;
        list_item_ptr->item_callback[2] = SetVideoEffectBlue;
        list_item_ptr->item_callback[3] = SetVideoEffectGreen;
        list_item_ptr->item_callback[4] = SetVideoEffectYellow;
        list_item_ptr->item_callback[5] = SetVideoEffectRed;
        list_item_ptr->item_callback[6] = SetVideoEffectCanvas;
        list_item_ptr->item_callback[7] = SetVideoEffectNegative;
    }
    return max_item_num;
}

/*****************************************************************************/
//Description :  MMIDC_GetDVSettingTypeListItems
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetDVSettingTypeListItems(MMIDC_LIST_CTRL_ITEM_T *list_item_ptr //out
                                              )
{ 
    int8 max_item_num = 0;
    uint16 cur_item = 0;    

    switch(list_item_ptr->icon_id)
    {
#ifdef MMIDC_F_FLASH
    case PDA_ICON_SETTING_DV_FLASH:
        {
            cur_item = MMIDC_GetFlashLamp();
            MMIDC_GetSettingItemsTxtOption(MMIDC_SETTINGS_SET_DV_FLASH, 
                                            PDA_ITEM_MAX, 
                                            list_item_ptr->text_array, 
                                            &max_item_num, 
                                            &list_item_ptr->title_text_id);  
        }
        break;
#endif

    case PDA_ICON_SETTING_DV_SIZE:
        {
            MMI_TEXT_ID_T *text_ptr = PNULL;
            ITEM_STRING_TYPE_E *str_type_ptr = PNULL;
            PDA_ITEM_CALL_BACK *callback_ptr = PNULL;
            
            
            MENU_ITEM_CALL_BACK func_list[VIDEO_SIZE_MAX] = {0};
            VIDEO_SIZE_E size_array[VIDEO_SIZE_MAX] = {0};
            MMI_TEXT_ID_T      text_id = TXT_NULL;
            VIDEO_SIZE_E  cur_size = 0;
            VIDEO_SIZE_E  var_size = 0;
            uint16  total_func_number = 0;
            uint16  total_size_number = 0;
            
            uint32 index = 0;
            uint32 focus_index = 0;
            int32  i = 0;
            
            text_ptr = list_item_ptr->text_array;
            str_type_ptr = list_item_ptr->string_type_array;
            callback_ptr = list_item_ptr->item_callback;
            
            
            cur_size = MMIDC_GetVideoSize();

            
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetDVSettingTypeListItems cur_size=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_4072_112_2_18_2_9_36_504,(uint8*)"d",cur_size);
            
            //size list from high to low
            total_size_number = MMIDC_GetPhoneVideoSizeList(ARR_SIZE(size_array),size_array);
            //get all size function list
            total_func_number = MMIDC_GetPhoneVideoSizeFunctionList(ARR_SIZE(func_list), func_list);
            if (VIDEO_SIZE_MAX != total_func_number)
            {
                //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetDVSettingTypeListItems total_size_number=%d, total_func_number=%d error!!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_OSD_OPTION_4080_112_2_18_2_9_36_505,(uint8*)"dd", total_size_number, total_func_number);
                break;
            }
            
            index = 0;
            for (i = 0; i < total_size_number; i++)
            {
                if (size_array[i] < VIDEO_SIZE_MAX)
                {
                    var_size = size_array[i];
                    text_id = var_size;
                    text_ptr[index] = text_id;
                    str_type_ptr[index] = STRING_TYPE_TEXT_SIZE;
                    callback_ptr[index] = func_list[var_size];
                    if(cur_size == var_size)
                    {
                        focus_index = index;
                    }
                    index ++;
                }
            }         
            
            cur_item = focus_index;
            max_item_num = index;
            list_item_ptr->title_text_id = TXT_COMM_SIZE;       
        }
        break;
    case PDA_ICON_SETTING_DV_CORUSCATE:
        {
            cur_item = MMIDC_GetVideoFlicker();
            MMIDC_GetSettingItemsTxtOption(MMIDC_SETTINGS_SET_DV_CORUSCATE, 
                                            PDA_ITEM_MAX, 
                                            list_item_ptr->text_array, 
                                            &max_item_num, 
                                            &list_item_ptr->title_text_id);  
        }
        break;
    case PDA_ICON_SETTING_DV_VIDEOFORMAT:
        {
            cur_item = MMIDC_GetRecordFileTypeOfSetting();
            MMIDC_GetSettingItemsTxtOption(MMIDC_SETTINGS_SET_VIDEOFORMAT, 
                                            PDA_ITEM_MAX, 
                                            list_item_ptr->text_array, 
                                            &max_item_num, 
                                            &list_item_ptr->title_text_id);  
        }
        break;
        
    case PDA_ICON_SETTING_DV_STORAGE:
        {
            cur_item = MMIDC_GetSettingItemsTxtOption(MMIDC_SETTINGS_SET_DV_STORAGE,
                                            PDA_ITEM_MAX, 
                                            list_item_ptr->text_array, 
                                            &max_item_num, 
                                            &list_item_ptr->title_text_id);
        }
        break;
    default:
        break;
    }
    
    list_item_ptr->cur_item = cur_item;
    list_item_ptr->max_item_num =  (uint16)max_item_num; /*lint !e571*/
    
    return max_item_num;
}

/*****************************************************************************/
//Description :  MMIDC_GetDCQualitySettingListItems
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetDCSettingTypeListItems(MMIDC_LIST_CTRL_ITEM_T *list_item_ptr //out
                                              )
{ 
    int8 max_item_num = 0;
    uint16 cur_item = 0;
    
    switch(list_item_ptr->icon_id)
    {
#ifdef MMIDC_F_FLASH
    case PDA_ICON_SETTING_DC_FLASH:
        {
            cur_item = MMIDC_GetFlashLamp();
            MMIDC_GetSettingItemsTxtOption(MMIDC_SETTINGS_SET_DC_FLASH,
                                            PDA_ITEM_MAX, 
                                            list_item_ptr->text_array, 
                                            &max_item_num, 
                                            &list_item_ptr->title_text_id);  
        }
        break;
#endif
        
    case PDA_ICON_SETTING_DC_QUALITY:
        {
            cur_item = MMIDC_GetPhotoQuality();
            MMIDC_GetSettingItemsTxtOption(MMIDC_SETTINGS_SET_QUALITY,
                                            PDA_ITEM_MAX, 
                                            list_item_ptr->text_array, 
                                            &max_item_num, 
                                            &list_item_ptr->title_text_id);
        }
        break;
    case PDA_ICON_SETTING_DC_SIZE:
        {
            MMI_TEXT_ID_T *text_ptr = PNULL;
            ITEM_STRING_TYPE_E *str_type_ptr = PNULL;
            PDA_ITEM_CALL_BACK *callback_ptr = PNULL;
            
            int32  lcd_size = 0;
            SCREEN_MODE_E screen_mode = 0;  
            
            uint32 index = 0;
            uint32 focus_index = 0;
            int32  i = 0;
            MENU_ITEM_CALL_BACK func_list[PHOTO_SIZE_MAX] = {0};
            MMIDC_PHOTO_SIZE_E size_array[PHOTO_SIZE_MAX] = {0};
            MMI_TEXT_ID_T      text_id = TXT_NULL;
            int16  total_size_number = 0;
            MMIDC_PHOTO_SIZE_E  cur_photo_size = 0;
            MMIDC_PHOTO_SIZE_E  var_size = 0;
            int16  total_func_number = 0;
            
            text_ptr = list_item_ptr->text_array;
            str_type_ptr = list_item_ptr->string_type_array;
            callback_ptr = list_item_ptr->item_callback;
            
            lcd_size = MMIDC_GetLCDSize();
            screen_mode = MMIDC_GetScreenMode();  
            
            
            if (SCREEN_MODE_VERTICAL == screen_mode)
            {
                //for phone size list, may horizontal screen
                if (SCREEN_MODE_HORIIZONTAL == MMIDC_GetDefaultScreenMode())
                {
                    screen_mode = SCREEN_MODE_HORIIZONTAL;
                }
            }
            
            cur_photo_size = MMIDC_GetPhotoSize();
            //size list from high to low
            total_size_number = GetPhonePhotoSizeList(ARR_SIZE(size_array), lcd_size, screen_mode, size_array);
            //get all size function lis
            total_func_number = GetPhonePhotoSizeFunctionList(ARR_SIZE(func_list), func_list);
            if (PHOTO_SIZE_MAX != total_func_number)
            {
                break ;
            }
            
            for (i = 0; i < total_size_number; i++)
            {
                if (size_array[i] < PHOTO_SIZE_MAX)
                {
                    var_size = size_array[i];
                    text_id = var_size;
                    
                    text_ptr[index] = text_id;
                    str_type_ptr[index] = STRING_TYPE_TEXT_SIZE;
                    callback_ptr[index] = func_list[var_size];
                    
                    if(cur_photo_size == var_size)
                    {
                        focus_index = index;
                    }
                    index ++;
                }
            }
            
            cur_item = focus_index;
            max_item_num = index;
            list_item_ptr->title_text_id = TXT_COMM_SIZE;
        }
        break;
    case PDA_ICON_SETTING_DC_PIC_MODE:
        {
            cur_item = MMIDC_GetMultiShootEnum();
            MMIDC_GetSettingItemsTxtOption(MMIDC_SETTINGS_SET_PIC_MODE,
                                            PDA_ITEM_MAX, 
                                            list_item_ptr->text_array, 
                                            &max_item_num, 
                                            &list_item_ptr->title_text_id);  
        }
        break;
        
    case PDA_ICON_SETTING_DC_SELF_SHOT:
        {
            cur_item = MMIDC_GetSelfShootDelayTime();
            MMIDC_GetSettingItemsTxtOption(MMIDC_SETTINGS_SET_SELF_SHOT,
                                            PDA_ITEM_MAX, 
                                            list_item_ptr->text_array, 
                                            &max_item_num, 
                                            &list_item_ptr->title_text_id);  
        }
        break;
        
    case PDA_ICON_SETTING_DC_CORUSCATE:
        {
            cur_item = MMIDC_GetPhotoFlicker();
            MMIDC_GetSettingItemsTxtOption(MMIDC_SETTINGS_SET_DC_CORUSCATE,
                                            PDA_ITEM_MAX, 
                                            list_item_ptr->text_array, 
                                            &max_item_num, 
                                            &list_item_ptr->title_text_id);  
        }
        break;
        
    case PDA_ICON_SETTING_DC_STORAGE:
        {
            cur_item = MMIDC_GetSettingItemsTxtOption(MMIDC_SETTINGS_SET_DC_STORAGE,
                                            PDA_ITEM_MAX, 
                                            list_item_ptr->text_array, 
                                            &max_item_num, 
                                            &list_item_ptr->title_text_id);  
        }
        break;
    default:
        break;
    }
    
    list_item_ptr->cur_item = cur_item;
    list_item_ptr->max_item_num =  (uint16)max_item_num; /*lint !e571*/
    
    return max_item_num;
}

/*****************************************************************************/
//Description :  get setting list items
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetDCSettingListItems(MMIDC_LIST_CTRL_ITEM_T *list_item_ptr //out
                                          )
{
    uint16 max_item_num = 0;
    uint16 cur_index = 0;   
    uint16 i = 0;     
    CAMERA_MODE_E   mode = CAMERA_MODE_DC;    
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_TEXT_ID_T text1_id = TXT_NULL;
    MMI_TEXT_ID_T image_id = IMAGE_NULL;
    MMI_STRING_T text_str = {0};
    BOOLEAN is_string = FALSE;
    BOOLEAN is_gray = FALSE;
    uint32 user_data = 0;
    MMIDC_SETTINGS_SET_E optin_arr[MMIDC_SETTINGS_SET_MAX_VALUE] = {0};
    uint16 arr_size_out = 0;

    MMIDC_GetCurModeSettingOption(optin_arr, MMIDC_SETTINGS_SET_MAX_VALUE, &arr_size_out);
    mode = MMIDC_GetCameraMode();    

    for(i = 0; i < arr_size_out; i++)
    {
        MMIDC_SETTING_ITEM_T cur_item = 
        {
            TXT_NULL,
                {TXT_NULL,
                TXT_NULL,
                TXT_NULL
                }
        };
        user_data = optin_arr[i];        
        is_gray = FALSE;
        is_string = FALSE;

        switch (user_data)
        {
#ifdef MMIDC_F_FLASH
        case MMIDC_SETTINGS_SET_DC_FLASH:
        case MMIDC_SETTINGS_SET_DV_FLASH:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetFlashLamp();
                image_id = IMAGE_COMMON_RIGHT_ARROW;

                if(SETTING_SENSOR_FRONT == MMIDC_GetSettingSensorID())
                {
                    is_gray = TRUE;
                }
            }
            break;
#endif

        case MMIDC_SETTINGS_SET_QUALITY:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetPhotoQuality();
                image_id = IMAGE_COMMON_RIGHT_ARROW;
            }
            break;

        case MMIDC_SETTINGS_SET_PHOTO_SIZE:
            {
                MMIDC_PHOTO_SIZE_E photo_size = MMIDC_GetPhotoSize();

                cur_item.label = TXT_COMM_SIZE;
                is_string = TRUE;
                MMIDC_GetSizeString(photo_size, &text_str);                
                image_id = IMAGE_COMMON_RIGHT_ARROW;
            }
            break;

#if defined(MMI_CAMERA_F_PHOTO_DATE)
        case MMIDC_SETTINGS_SET_PHOTO_DATE:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetNVPhotoDate();
                if(PHOTO_DATE_ON == cur_index)
                {
                    image_id = IMAGE_CHECK_SELECTED_ICON;
                }
                else
                {
                    image_id = IMAGE_CHECK_UNSELECTED_ICON;
                }
            }
            break;
#endif

        case MMIDC_SETTINGS_SET_ENVIRONMENT:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetPhotoEnvironment();
                if(ENVIRONMENT_NIGHT == cur_index)
                {
                    image_id = IMAGE_CHECK_SELECTED_ICON;
                }
                else
                {
                    image_id = IMAGE_CHECK_UNSELECTED_ICON;
                }
            }
            break;

        case MMIDC_SETTINGS_SET_PIC_MODE:
            {
            #if defined(MMIDC_F_WORK_MODE)
                //not support burst in work mode
                if (MMIDC_IsWorkMode())
                {
                    is_gray = TRUE;
                }
            #endif

                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetMultiShootEnum();
                image_id = IMAGE_COMMON_RIGHT_ARROW;
            }
            break;

        case MMIDC_SETTINGS_SET_SELF_SHOT:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetSelfShootDelayTime();
                image_id = IMAGE_COMMON_RIGHT_ARROW;
            }
            break;

        case MMIDC_SETTINGS_SET_DC_CORUSCATE:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetPhotoFlicker();
                image_id = IMAGE_COMMON_RIGHT_ARROW;
            }
            break;

#if !defined(CMCC_UI_STYLE)               
        case MMIDC_SETTINGS_SET_SHUTTERVOICE:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetShutterVoice();
                if(SHUTTER_VOICE_1 == cur_index)
                {
                    image_id = IMAGE_CHECK_SELECTED_ICON;
                }
                else
                {
                    image_id = IMAGE_CHECK_UNSELECTED_ICON;
                }
            }
            break;
#endif

        case MMIDC_SETTINGS_SET_AUTO_SAVE:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetIsAutoSave();
                if(AUTO_SAVE_ON == cur_index)
                {
                    image_id = IMAGE_CHECK_SELECTED_ICON;
                }
                else
                {
                    image_id = IMAGE_CHECK_UNSELECTED_ICON;
                }
#ifdef MMIDC_F_WORK_MODE
                if (MMIDC_IsWorkMode())
                {
                    is_gray = TRUE;
                }
#endif    
            }
            break;
            
        case MMIDC_SETTINGS_SET_DC_STORAGE:
        case MMIDC_SETTINGS_SET_DV_STORAGE:
            {
                cur_index = MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                image_id = IMAGE_COMMON_RIGHT_ARROW;
            }
            break; 

        case MMIDC_SETTINGS_SET_VIDEO_SIZE:
            {
                VIDEO_SIZE_E video_size = MMIDC_GetVideoSize();

                cur_item.label = TXT_COMM_SIZE;
                is_string = TRUE;
                MMIDC_GetSizeString(video_size, &text_str);   
                image_id = IMAGE_COMMON_RIGHT_ARROW;
            }
            break;
            
        case MMIDC_SETTINGS_SET_DV_CORUSCATE:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetVideoFlicker();
                image_id = IMAGE_COMMON_RIGHT_ARROW;
            }
            break;
            
        case MMIDC_SETTINGS_SET_AUDIO:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetVideoAudioSetting();
                if(AUDIO_SETTING_ON == cur_index)
                {
                    image_id = IMAGE_CHECK_SELECTED_ICON;
                }
                else
                {
                    image_id = IMAGE_CHECK_UNSELECTED_ICON;
                }
            }
            break;
            
        case MMIDC_SETTINGS_SET_VIDEOFORMAT:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetRecordFileTypeOfSetting();
                image_id = IMAGE_COMMON_RIGHT_ARROW;
            }
            break;            
            
        default:
            break;
        }
        


        text_id = cur_item.label;

        if ((TXT_NULL != text_id)
            && (0 != text_id)
           )
        {
            if (is_string)
            {
                list_item_ptr->string_array[max_item_num] = text_str;
                list_item_ptr->string_type_array[max_item_num] = STRING_TYPE_STRING2;
            }
            else
            {
                text1_id = cur_item.val_set[cur_index];
                if (0 == text1_id)
                {
                    text1_id = TXT_NULL;                
                }
                list_item_ptr->text1_array[max_item_num] = text1_id;
                list_item_ptr->string_type_array[max_item_num] = STRING_TYPE_NONE;
            }
            
            list_item_ptr->text_array[max_item_num] = text_id;       
            list_item_ptr->image_array[max_item_num] = image_id;
            list_item_ptr->gray_array[max_item_num] = is_gray;
            list_item_ptr->user_data[max_item_num] = user_data;

            max_item_num++;
        }
    }
    
    //list_item_ptr->cur_item = 0;
    list_item_ptr->max_item_num =  max_item_num;
    list_item_ptr->title_text_id = STXT_MAIN_SETTING;
    
    return max_item_num;
}
#endif


#endif  //#ifdef  CAMERA_SUPPORT

