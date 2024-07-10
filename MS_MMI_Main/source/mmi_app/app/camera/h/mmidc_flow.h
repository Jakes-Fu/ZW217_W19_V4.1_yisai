#ifndef _MMIDC_FLOW_H
#define _MMIDC_FLOW_H

#include "img_cascade.h"
#include "dal_dcamera.h"
#include "mmisrvaud_api.h"
#include "mmidc_camera_text.h"
#include "mmidc_setting.h"

#define MMI_DC_MIN_SPACE_SIZE       102400

typedef struct 
{
    uint8*                                  buffer[3];
    uint32                                  size;
    int32                                    index;
    uint32*                                 osd_panorama_buffer;//[240 * 64 / 2];
    uint32*                                 osd_full_panorama_buffer;//[240 * 320 / 2];
    IMG_PANORAMA_PARAM_T    panorama_mosaic; //modified, IMG_PANORAMA_MOSAIC_PARAM_T
}DC_PHOTO_PANORAMA_STRUCT;

typedef enum
{
    DC_LEFT_KEY = 0,
    DC_RIGHT_KEY,
    DC_UP_KEY,
    DC_DOWN_KEY,
    DC_LEFT_SOFT_KEY,
    DC_RIGHT_SOFT_KEY,
    DC_OK_KEY,
    DC_KEY_MAX
}DC_KEY_E;


typedef enum
{
    PDA_LEFT_KEY,
    PDA_RIGHT_KEY,
    PDA_UP_KEY,
    PDA_DOWN_KEY,
    PDA_LEFT_SOFT_KEY,
    PDA_RIGHT_SOFT_KEY,
    PDA_OK_KEY,
    PDA_BACK_KEY,
    PDA_MENU_KEY,
    PDA_KEY_MAX
}DC_PDA_KEY_E;

typedef enum
{
    DC_PREVIEW_MODE = 0,
        DC_CAPTURE_CONT_DOWN_MODE,
        DC_CAPTURE_MODE,
        DC_REVIEW_MODE,
        DC_SAVE_MODE,
        DV_PREVIEW_MODE,
        DV_RECORD_MODE,
        DV_PAUSE_MODE,
#ifdef DC_WATCH_UI_SUPPORT 
        DV_SAVE_MODE,   //on watch, there is no record pause, and add save phase display wait icon, no other icons.
#endif
        DV_REVIEW_MODE,
        DC_FLOW_MODE_MAX
}DC_FLOW_MODE_E;


typedef enum
{
    DC_FORWARD = 0,
    DC_BACKWARD,
    DV_PAUSERESUME,
    DC_INTERRUPT,
    DC_CONTINUE,
    DC_EXIT,
    DC_OPERATION_MAX
}DC_OPERATION_E;

typedef enum
{
    PREVIEW_TYPE_START,
    PREVIEW_TYPE_SWITCH_SIZE,
    PREVIEW_TYPE_MAX
}DC_PREVIEW_TYPE_E;

typedef enum
{
    DV_END_NORMAL = 0,
        DV_END_ERROR,
        DV_END_ROOM_INSUF_ERROR,
        DV_END_BY_APP,
        DV_END_LIMIT_SIZE,
        DV_END_BY_APP_CANCEL,
        DV_END_MAX
} DV_END_TYPE_E;


typedef struct 
{
    uint8*  image_addr; //图像数据指针，由驱动层分配，释放
    uint32  image_len;
    MMIDC_FILE_INFO_PTR_T file_list[MAX_DC_BURST_PHOTO_NUMBER];
    uint32 max_file_size;
    uint8* review_addr; //浏览图片数据指针，由MMI分配，释放
    uint32 review_file_size;
}MMIDC_IMAGE_INFO_T;



/*****************************************************************************/
// 	Description : get panorama photo info
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_PHOTO_PANORAMA_STRUCT* MMIDC_GetPhotoPanoramaInfo(void);

/*****************************************************************************/
// 	Description : dc file rename again
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_RenameFileAgain(void);

/*****************************************************************************/
// 	Description : get dc struct infor
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIDC_IMAGE_INFO_T* MMIDC_GetDCInfo(void);

/*****************************************************************************/
// 	Description : get number of multi shoot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetMulitShotNumber(void);

/*****************************************************************************/
// 	Description : start to run the flow of camera
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_FlowStart(void);

/*****************************************************************************/
// 	Description : set current mode
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetCurrentMode(DC_FLOW_MODE_E mode);

/*****************************************************************************/
// 	Description : get current mode
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_FLOW_MODE_E MMIDC_GetCurrentMode(void);

/*****************************************************************************/
// 	Description : get captured number of photos
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetCapturedPhotosNumber(void);

/*****************************************************************************/
// 	Description : return is review in full screen or not
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsReviewFullScreen(void);

/*****************************************************************************/
// 	Description : return dc is open or not
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsDCOpen(void);

/*****************************************************************************/
// 	Description : return dv is open or not
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsDVOpen(void);

/*****************************************************************************/
// 	Description : error handle for camera
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ErrorTip(MMI_TEXT_ID_T txt_id);

/*****************************************************************************/
// 	Description : general review photo 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ReviewPhotos(void);

/*****************************************************************************/
// 	Description : is camera preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsReviewing(void);

/*****************************************************************************/
// 	Description : is camera preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsCapturing(void);

/*****************************************************************************/
// 	Description : return is review in full screen or not
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_FlowFunction(DC_OPERATION_E op);

/*****************************************************************************/
// 	Description : dc flow operation
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShutterViceCallback(MMISRVAUD_REPORT_RESULT_E result, DPARAM param);  

/*****************************************************************************/
// 	Description : dc flow operation
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_PlayCountVoice(uint8 times);

/*****************************************************************************/
// 	Description : post dc state msg  for error
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_PostStateMsg(void);

/*****************************************************************************/
//  Description : get all device info and check if have enough space 
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param Input:  size        space is or no smaller  the size
//       Output:  file_dev    file device 
//       Return:  SFS_ERROR_NONE  or other value, see MMIFILE_ERROR_E
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E  MMIDC_CheckSpace(uint32 size,   //[in] min space size 
                                        MMIFILE_DEVICE_E *file_dev  //[out] file device
                                        );

/*****************************************************************************/
//  Description : show error message as error type 
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param Input:  fs_error    error  type as MMIFILE_ERROR_E
//        Input:  dev         file device
//       Return:  NONE
/*****************************************************************************/
PUBLIC void  MMIDC_ShowErrorMsg(MMIFILE_ERROR_E fs_error,   //[in] error value
                                          MMIFILE_DEVICE_E   dev     //[in] file device 
                                        );
                                        
/*****************************************************************************/
//  Description : show appointed error message by file device 
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param Input:  fs_error    error type
//       Return:  NONE
/*****************************************************************************/
PUBLIC void  MMIDC_ShowAppointedErrorMsg(MMIFILE_ERROR_E fs_error   //[in] error type 
                                         );
                                         
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ErrorTipForExit(MMI_TEXT_ID_T txt_id);

/*****************************************************************************/
//  Description : init flow parameter
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIDC_InitFlowParameter(void);

/*****************************************************************************/
// 	Description : is the photo of multi shoot photos deleted
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_CanPressSave(void);

/*****************************************************************************/
// 	Description : restore block setting
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_RestoreBlock(void);

/*****************************************************************************/
// 	Description : set block property
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_SetBlock(void);

/*****************************************************************************/
// 	Description : get distortion mode
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC DCAMERA_ROTATION_MODE_E MMIDC_GetPhotoRotationMode(void);

/*****************************************************************************/
// 	Description : get display mode
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC DCAMERA_DISP_MODE_E MMIDC_GetPhotoDisplayMode(void);

/*****************************************************************************/
// 	Description : free last dv frame data buffert
//	Global resource dependence : none
//  Author: robert.wang
//	Note:  
/*****************************************************************************/
PUBLIC void MMIDC_FreeLastFrame(void);

/*****************************************************************************/
// 	Description : get last frame data address
//	Global resource dependence : none
//  Author: robert.wang
//	Note:  
/*****************************************************************************/
PUBLIC GUI_COLOR_T * MMIDC_GetLastFramePtr(void);
/*****************************************************************************/
// 	Description : show photo frame or not
//	Global resource dependence : none
//  Author: robert.wang
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsShowPhotoFrame(void);

/*****************************************************************************/
// 	Description : show photo frame or not
//	Global resource dependence : none
//  Author: robert.wang
//	Note:  
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoFrameForShow(BOOLEAN is_show);

/*****************************************************************************/
//Description : start hit timer
//Global resource dependence : none
//Author: robert.wang
//Note: camera exit while do nothing in 1 minute 
/*****************************************************************************/
PUBLIC void MMIDC_ResetHitTimer(void);

/*****************************************************************************/
// 	Description : get the osd flag before preview
//	Global resource dependence : none
//  Author: robert.wang
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_GetOSDFlagBeforePreview(void);

/*****************************************************************************/
// 	Description : set the osd flag before preview
//	Global resource dependence : none
//  Author: robert.wang
//	Note:  
/*****************************************************************************/
PUBLIC void MMIDC_SetOSDFlagBeforePreview(BOOLEAN is_show);

/*****************************************************************************/
//Description : open dv
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_OpenDV(void);

/*****************************************************************************/
//Description : open dc
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_OpenDC(void);

/*****************************************************************************/
//Description : stop preview for switch size
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_StopPreviewForSwitchSize(void);

/*****************************************************************************/
//Description : start preview for switch size
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_StartPreviewForSwitchSize(void);


/*****************************************************************************/
//Description : get main osd layer device info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO MMIDC_GetMainOSDLayerDevInfo(void);

/*****************************************************************************/
//Description : destroy main osd layer
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DestroyMainOSDLayer(void);

/*****************************************************************************/
//Description : create main osd layer
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_CreateMainOSDLayer(void);

/*****************************************************************************/
//Description : clear main osd layer
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearMainOSDLayer(void);

/*****************************************************************************/
//Description : process dcamera flow msg
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_ProcessDcameraFlowMsg(MMI_MESSAGE_ID_E msg_id, //in
                                       void *param //in
                                       );
                                       
/*****************************************************************************/
//Description : pda review photos for edit pic
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_PDAReviewPhotosForEditPic(void);

/*****************************************************************************/
//Description : back to preview by cancel key
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_BackToPreview(void);
/*****************************************************************************/
//Description : pda cmd
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_PDACmd(DC_PDA_KEY_E key);

//#if defined(MMIDC_DESKTOP_PDA)
#if defined(MOCOR_SUPPORT_ARGB888_MODE)
/*****************************************************************************/
//Description : get last frame data as a888 format
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetA888LastFrameData(GUI_COLOR_T *src_ptr, //in
                                             uint32 src_width, //in
                                             uint32 src_height //in
                                             );
/*****************************************************************************/
//Description : get a888 last frame pointer
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint32* MMIDC_GetA888LastFrameByPtr(void);

/*****************************************************************************/
//Description : free a888 last frame pointer
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_FreeA888LastFrame(void);
#endif //#if defined(MMIDC_DESKTOP_PDA)

/*****************************************************************************/
//Description : recreate main osd layer
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_ReCreateMainOSDLayer(void);

/*****************************************************************************/
//Description : get memory addr for review file
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint8* MMIDC_GetMemoryAddrForReview(void);

/*****************************************************************************/
//Description : free memory for review file
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_FreeMemoryForReview(void);

/*****************************************************************************/
//Description : alloc memory for review file
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_AllocMemoryForReview(void);

/*****************************************************************************/
//Description : save data to file
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_SaveDataToFile(int32 photo_id,
                                         uint8 *data_buffer,//in
                                         uint32 data_size);

/*****************************************************************************/
//Description : display preview frame
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayPreviewFrame(uint32 start_preview_tick);

/*****************************************************************************/
//  Description : send capture fail msg
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_SendCaptureFailMsg(int32 ret,//return value
                                             void* param_ptr,//in
                                             uint32 param_size);
                                             
/*****************************************************************************/
//Description : set memory addr for review file
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint8* MMIDC_SetMemoryAddrForReview(uint8 *data_ptr, 
                                                        uint32 data_size
                                                        );

/*****************************************************************************/
//  Description : get capture callback semaphore ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC SCI_SEMAPHORE_PTR MMIDC_GetCaptureCallbackSemaphorePtr(void);

/*****************************************************************************/
//  Description : set capture callback semaphore ptr
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetCaptureCallbackSemaphorePtr(SCI_SEMAPHORE_PTR semaphore_ptr);

/*****************************************************************************/
//  Description : create capture callback semaphore ptr
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CreateCaptureCallbackSemaphorePtr(void);

/*****************************************************************************/
//  Description : delete capture callback semaphore ptr
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_DeleteCaptureCallbackSemaphorePtr(void);

/*****************************************************************************/
//  Description : push capture callback semaphore
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PushCaptureCallbackSemaphore(void);

/*****************************************************************************/
//  Description : pop capture callback semaphore
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PopCaptureCallbackSemaphore(void);

/*****************************************************************************/
//Description : free dc image info
//Global resource dependence : none
//Author: jinju.ma
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_FreeDCImageInfoMemory(void);

/*****************************************************************************/
//  Description : get start preview tick
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetStartPreviewTick(void);

/*****************************************************************************/
//  Description : set start preview tick
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetStartPreviewTick(uint32 tick);

/*****************************************************************************/
//  Description : play cpature ring only to capture callback
//  Global resource dependence : none
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PlayCaptureRing(void);

#ifdef DC_WATCH_UI_SUPPORT 
/*****************************************************************************/
//  Description: check whether vp module is inited.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, TRUE if vp is inited.
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WDC_isInVpState(void);

/*****************************************************************************/
//  Description: return vp play state.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, TRUE if playing.
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN WDC_isVpPlaying(void);

/*****************************************************************************/
//  Description: play or pause video play.
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC void WDC_DVReviewPlayOrPause(void);

/*****************************************************************************/
//  Description: stop dv review playback.
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC void WDC_DVReviewStopPlaying(void);
#endif

#endif

