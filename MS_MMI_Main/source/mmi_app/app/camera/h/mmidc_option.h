#ifndef _MMIDC_OPTION_H
#define _MMIDC_OPTION_H

#include "mmidc_camera_image.h"
#include "mmidc_camera_text.h"
#include "mmidc_flow.h"
#include "mmidc_gui.h"
#include "mmidc_setting.h"
#include "mmidc_display.h"
#include "mmidc_guipda.h"

//#define  MAX_SETTING_ITEM_NUMBER      8
#define  MAX_SETTING_VAL_SET_NUMBERT  3

typedef enum
{  
    MMIDC_SETTINGS_SET_START,
    MMIDC_SETTINGS_SET_DC_FLASH,
    MMIDC_SETTINGS_SET_PHOTO_SIZE,
    MMIDC_SETTINGS_SET_QUALITY,
    MMIDC_SETTINGS_SET_DC_CORUSCATE,
    MMIDC_SETTINGS_SET_SHUTTERVOICE,   
    MMIDC_SETTINGS_SET_AUTO_SAVE,
    MMIDC_SETTINGS_SET_DC_STORAGE,
    MMIDC_SETTINGS_SET_PHOTO_DATE,
    MMIDC_SETTINGS_SET_DC_DISPLAY_TIP,
    MMIDC_SETTINGS_SET_ENVIRONMENT,
    MMIDC_SETTINGS_SET_PIC_MODE,
    MMIDC_SETTINGS_SET_SELF_SHOT,
    MMIDC_SETTINGS_SET_DV_FLASH,
    MMIDC_SETTINGS_SET_VIDEO_SIZE,
    MMIDC_SETTINGS_SET_DV_CORUSCATE,
    MMIDC_SETTINGS_SET_AUDIO,
    MMIDC_SETTINGS_SET_VIDEOFORMAT,
    MMIDC_SETTINGS_SET_DV_STORAGE,
    MMIDC_SETTINGS_SET_DV_DISPLAY_TIP,
    MMIDC_SETTINGS_SET_MAX_VALUE
}MMIDC_SETTINGS_SET_E;

typedef struct
{
    MMI_TEXT_ID_T  label;
    MMI_TEXT_ID_T  val_set[MAX_SETTING_VAL_SET_NUMBERT];
}MMIDC_SETTING_ITEM_T;

/*****************************************************************************/
// 	Description : handle adjust key function
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_HandleAdjustKey(DIRECTION_KEY key);
/*****************************************************************************/
// 	Description : open osd icons
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void	MMIDC_OpenOSDIcons(void);
/*****************************************************************************/
// 	Description : open desktop tip OSD
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void	MMIDC_OpenDesktopTipOSD(void);
/*****************************************************************************/
// 	Description : open photo option window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenPhotoOption(void);
/*****************************************************************************/
// 	Description : open video option window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenVideoOption(void);
/*****************************************************************************/
// 	Description : set current reviewing photo id
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_MoveCurReviewPhotoID(MOVE_DIRECTION_E direction);
/*****************************************************************************/
// 	Description : open photo review option window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenPhotoReviewOption(void);
/*****************************************************************************/
// 	Description : open video review option window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenVideoReviewOption(void);

/*****************************************************************************/
// 	Description : open send photo window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenSendPhotoWin(void);
/*****************************************************************************/
// 	Description : get current reviewing photo id
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetCurrentPhotoID(void);
/*****************************************************************************/
// 	Description : reset current reviewing photo id
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ResetCurrentPhotoID(void);
/*****************************************************************************/
// 	Description : handle adjust key function
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_HandleAdjustTP(int16 x, int16 y);

/*****************************************************************************/
// 	Description : open video settings window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenSetting(void);

/*****************************************************************************/
// 	Description : set lcd infor for rotate dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenHelpWin(void);

/*****************************************************************************/
// 	Description : set photo size 3 m
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_NeedReStartPreview(MMIDC_PHOTO_SIZE_E size);

/*****************************************************************************/
//  Description : open dc storage option menu
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenStorageOption(void);

#ifdef MMIDC_MINI_RESOURCE_SIZE
/*****************************************************************************/
//  Description : open effect option menu
//  Global resource dependence : none
//  Author: jinju.ma
//  Date: 2012-4-27
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenEffectOption(void);
#endif
#ifdef MMIDC_ADD_SPECIAL_EFFECT
/*****************************************************************************/
//  Description : open contrast adjust win
//  Global resource dependence : none
//  Author: jinju.ma
//  Date: 2012-7-23
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenContrastAdjustWin(void);
/*****************************************************************************/
//  Description : open brightness adjust win
//  Global resource dependence : none
//  Author: jinju.ma
//  Date: 2012-7-23
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenBrightnessAdjustWin(void);
/*****************************************************************************/
//  Description : open white balance option menu
//  Global resource dependence : none
//  Author: jinju.ma
//  Date: 2012-7-23
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenWhiteBalanceOption(void);
#endif
/*****************************************************************************/
// 	Description : clean adjust rect
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseAdjust(void);

/*****************************************************************************/
// 	Description : init adjust data
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_InitAdjustData(void);

/*****************************************************************************/
// 	Description : display ajust context 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayAdjust(void);

/*****************************************************************************/
// 	Description : clear adjust rect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearAdjustRect(void);

/*****************************************************************************/
// 	Description : ajust menu is opened or not
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsAdjustMenu(void);

/*****************************************************************************/
// 	Description : open setting window
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenSettingWin(void);

/*****************************************************************************/
// 	Description : open video settings window for setting config
//	Global resource dependence : none
//  Author: robert.wang 
//	Note:
/*****************************************************************************/
PUBLIC VIDEO_FILE_TYPE_E MMIDC_GetRecordFileTypeOfSetting(void);

/*****************************************************************************/
// 	Description : open video settings window for setting config
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetRecordFileTypeOfSetting(uint32 index);

/*****************************************************************************/
// 	Description : set photo size state
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC int32 SetPhotoSizeState(MMIDC_PHOTO_SIZE_E photo_size);

/*****************************************************************************/
//Description :  shortcut for switch sensor
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForSwitchSensor(void);

/*****************************************************************************/
//  Description : Get Storage Device List
//  Global resource dependence : none
//  Author: robert.wang
//  Note:  
/*****************************************************************************/
PUBLIC int8 GetStorageDeviceList(MMIFILE_DEVICE_E *dev_list, MMI_TEXT_ID_T *text_id_list) ;

#if defined(MMIDC_DESKTOP_PDA)
/*****************************************************************************/
//Description :  get dc mode list items
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetDCModeListItems(MMIDC_LIST_CTRL_ITEM_T *list_item_ptr);

/*****************************************************************************/
//Description :  get dc effect list items
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetDCEffectListItems(MMIDC_LIST_CTRL_ITEM_T *list_item_ptr);
/*****************************************************************************/
//Description :  get dc white blance list items
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetDCWhiteBlanceListItems(MMIDC_LIST_CTRL_ITEM_T *list_item_ptr);

/*****************************************************************************/
//Description :  get setting list items
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetDCSettingListItems(MMIDC_LIST_CTRL_ITEM_T *list_item_ptr);

/*****************************************************************************/
//Description :  set setting list items
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/                                      
PUBLIC BOOLEAN MMIDC_SetDCIconTypeList(MMI_HANDLE_T ctrl_id); 

/*****************************************************************************/
//Description :  MMIDC_GetDCSettingTypeListItems
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetDCSettingTypeListItems(MMIDC_LIST_CTRL_ITEM_T *list_item_ptr);
/*****************************************************************************/
//Description :  MMIDC_GetDVSettingTypeListItems
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetDVSettingTypeListItems(MMIDC_LIST_CTRL_ITEM_T *list_item_ptr //out
                                                );

/*****************************************************************************/
//Description :  MMIDC_SetDCIconSettingTypeList
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_SetDCIconSettingTypeList(MMI_HANDLE_T ctrl_id);
/*****************************************************************************/
//Description :  MMIDC_SetDCIconSettingList
//Global resource dependence : none
//Author: kunliu
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_SetDCIconSettingList(MMI_HANDLE_T ctrl_id);

#endif

/*****************************************************************************/
//Description : set camera mode photo
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
PUBLIC void SetPhotoMode(void);

/*****************************************************************************/
//Description : set camera mode video
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
PUBLIC void SetVideoMode(void);

/*****************************************************************************/
// 	Description : set camera mode photo
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void SetPhotoMode(void);

/*****************************************************************************/
// 	Description : set camera mode video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void SetVideoMode(void);

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
                                                               );
                                                    
/*****************************************************************************/
//Description : get phone video size list
//lobal resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetPhoneVideoSizeList(int16 max_item, //in
                                                     VIDEO_SIZE_E *size_list_ptr //out
                                                     );
                                         
/*****************************************************************************/
//Description :  get phone photo size function list
//Global resource dependence : none
//Author: robert.wang
//Note: size list from  low to high
// Return: total supported number
/*****************************************************************************/
PUBLIC int16 GetPhonePhotoSizeFunctionList(int16 max_item,                                       
                                                  MENU_ITEM_CALL_BACK *func_list_ptr //out
                                                  );
                                                  
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
                                      );
                                      
/*****************************************************************************/
// 	Description : set photo env normal
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void SetPhotoEnvNormal(void);

/*****************************************************************************/
// 	Description : set photo env night
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void SetPhotoEnvNight(void);

/*****************************************************************************/
//  Description : get setting items txt content
//  Global resource dependence : none
//  Author: chunyou
//  Date: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetSettingItemsTxtContent(uint16 type,//in
                                                        MMIDC_SETTING_ITEM_T *item_ptr //out
                                                        );

/*****************************************************************************/
//  Description : get setting items txt content
//  Global resource dependence : none
//  Author: chunyou
//  Date: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetCurModeSettingOption(MMIDC_SETTINGS_SET_E *option_ptr,//in
                                                        uint16 size_in, //in
                                                        uint16 *size_out_ptr //out
                                                        );

/*****************************************************************************/
//  Description : get setting items txt option
//  Global resource dependence : none
//  Author: chunyou
//  Date: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetSettingItemsTxtOption(uint16 type,//in
                                                        uint16 text_total_num, //in
                                                        MMI_TEXT_ID_T *text_ptr, //out                                                        
                                                        int8 *max_item_num, //out
                                                        MMI_TEXT_ID_T* win_text_id_ptr //out
                                                        );

/*****************************************************************************/
// 	Description : set camera mode video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoModeContinue(void) ;

#endif

