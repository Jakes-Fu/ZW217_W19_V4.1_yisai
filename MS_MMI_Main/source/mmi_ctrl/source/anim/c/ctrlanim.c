/*****************************************************************************
** File Name:      ctrlanim.c                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012      hua.fang              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guires.h"
#include "mmi_theme.h"
#include "guicommon.h"
#include "ctrlanim.h"
#ifdef DYNAMIC_MODULE_SUPPORT   
#include "caffilesys.h"
#endif
#include "mmi_string.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GUIANIM_MINIATURE_SIZE_TIME_LEN     11  //缩略图原始文件size+creat time max len
#define GUIANIM_MINIATURE_PATH              (s_guianim_miniature_path_wstr)
#define GUIANIM_MINIATURE_PATH_LEN          5
#define GUIANIM_FILE_SLASH                  0x005c

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL const wchar       s_guianim_miniature_path_wstr[] = { '\\', '_', 'S', 'l', 't', 0 };

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

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
//  Description : init anim class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AnimCtrlInitVtbl(
    CTRLANIM_VTBL_T        *anim_vtbl_ptr
);

/*****************************************************************************/
//  Description : anim construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AnimCtrlConstruct (
    CTRLBASE_OBJ_T          *base_ctrl_ptr,
    CTRLANIM_INIT_PARAM_T   *anim_param_ptr
);

/*****************************************************************************/
//  Description : anim destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AnimCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AnimCtrlPackInitParam (
    GUIANIM_INIT_DATA_T     *anim_init_ptr,   //in
    CTRLANIM_INIT_PARAM_T   *anim_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle anim msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E AnimCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN AnimCtrlGetHeightByWidth (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
);

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN AnimCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
);

/*****************************************************************************/
//  Description : reset timer and free memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetAndFreeMemory(
                              CTRLANIM_OBJ_T    *anim_ctrl_ptr
                              );

/*****************************************************************************/
//  Description : free target buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeTargetBuffer(
                            CTRLANIM_OBJ_T  *anim_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : free wallpaper target buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeWallpaperTargetBuffer(
                                     CTRLANIM_OBJ_T *anim_ctrl_ptr
                                     );

/*****************************************************************************/
//  Description : set anim bitmap param
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetAnimBitmap(
                            CTRLANIM_OBJ_T          *anim_ctrl_ptr, //in:
                            GUIANIM_DATA_INFO_T     *data_info_ptr  //in:
                            );

/*****************************************************************************/
//  Description : set anim data param and decode data buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E SetAndDecodeData(
                                        CTRLANIM_OBJ_T      *anim_ctrl_ptr, //in:
                                        GUIANIM_DATA_INFO_T *data_info_ptr  //in:
                                        );

/*****************************************************************************/
//  Description : load anim or image resource
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN LoadAnimOrImgRes(
                               CTRLANIM_OBJ_T   *anim_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : save image file path
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveFilePath(
                           CTRLANIM_OBJ_T       *anim_ctrl_ptr, //in:
                           GUIANIM_FILE_INFO_T  *file_info_ptr  //in:may PNULL
                           );

/*****************************************************************************/
//  Description : decode image buffer or file
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E DecodeImgBufOrFile(
                                          BOOLEAN           is_first,       //in:is first decode
                                          BOOLEAN           is_thumb_fail,  //in:is decode thumbnail fail
                                          BOOLEAN           is_encode_fail, //in:is encode jpg fail
                                          CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in:
                                          );

/*****************************************************************************/
//  Description : set decode or encode target info
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E SetDecEncTarget(
                                       BOOLEAN          is_first,           //in:is first decode
                                       BOOLEAN          is_thumb_fail,      //in:is decode thumbnail fail
                                       BOOLEAN          is_encode_fail,     //in:is encode jpg fail
                                       BOOLEAN          *is_miniatrure_ptr, //in/out:is decode miniature
                                       CTRLANIM_OBJ_T   *anim_ctrl_ptr,     //in/out:
                                       GUIANIM_TARGET_T *target_ptr         //in/out:
                                       );

/*****************************************************************************/
//  Description : get image info
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E GetImageInfo(
                                    CTRLANIM_OBJ_T      *anim_ctrl_ptr, //in/out:
                                    GUIANIM_IMG_INFO_T  *img_info_ptr   //in/out:
                                    );

/*****************************************************************************/
//  Description : get jpg decode/encode info,is decode thumbnail,is encode jpg
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void GetJpgDecEncInfo(
                            BOOLEAN             is_first,           //in:is first decode
                            BOOLEAN             is_thumb_fail,      //in:is decode thumbnail fail
                            BOOLEAN             is_encode_fail,     //in:is encode jpg fail
                            BOOLEAN             *is_miniatrure_ptr, //in/out:is decode miniature
                            CTRLANIM_OBJ_T      *anim_ctrl_ptr,     //in/out:
                            GUIANIM_IMG_INFO_T  *img_info_ptr       //in:
                            );

/*****************************************************************************/
//  Description : is decode jpg thumbnail
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsDecodeThumbnail(
                                CTRLANIM_OBJ_T      *anim_ctrl_ptr, //in:
                                GUIANIM_IMG_INFO_T  *img_info_ptr   //in:
                                );

/*****************************************************************************/
//  Description : is encode jpg
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsEncodeJpg(
                          CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in
                          );

/*****************************************************************************/
//  Description : get current disk free memory is larger than 128K
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FreeSpaceIsEnough(
                                wchar           *full_path_ptr, //in:
                                CTRLANIM_OBJ_T  *anim_ctrl_ptr  //in
                                );

/*****************************************************************************/
//  Description : get image crop rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetCropRect(
                          BOOLEAN           is_miniature,   //in:
                          GUI_RECT_T        *crop_rect_ptr, //in:out
                          CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in
                          );

/*****************************************************************************/
//  Description : alloc target buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E AllocTargetBuffer(
                                         CTRLANIM_OBJ_T     *anim_ctrl_ptr, //in/out:
                                         GUIANIM_TARGET_T   *target_ptr     //in/out:
                                         );

/*****************************************************************************/
//  Description : create handle and get frame
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E CreateHandleAndGetFrame(
                                               BOOLEAN              is_decode_miniature,//in:
                                               CTRLANIM_OBJ_T       *anim_ctrl_ptr,     //in:
                                               GUIANIM_TARGET_T     *dec_target_ptr,    //in:
                                               GUIANIM_DEC_RESULT_T *dec_result_ptr     //in/out:
                                               );

/*****************************************************************************/
//  Description : save wallpaper bitmap info
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SaveWallpaperBitmap(
                               CTRLANIM_OBJ_T   *anim_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : destroy image handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyImgHandle(
                               CTRLANIM_OBJ_T           *anim_ctrl_ptr, //in
                               GUIANIM_DEC_RESULT_T     *dec_result_ptr //in
                               );

/*****************************************************************************/
//  Description : decode anim or image resource
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E DecodeAnimOrImgRes(
                                          CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in
                                          );

/*****************************************************************************/
//  Description : get image type,width and height by resource
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E GetImageInfoByRes(
                                         CTRLANIM_OBJ_T *anim_ctrl_ptr  //in/out:
                                         );

/*****************************************************************************/
//  Description : get miniature file name
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetMiniaturFileName(
                               CTRLANIM_OBJ_T   *anim_ctrl_ptr  //in
                               );

/*****************************************************************************/
//  Description : get the miniature name
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetMiniatureFullPath(
                                   const wchar                      *full_path_ptr,     //in
                                   uint16                           full_path_len,      //in
                                   uint16                           miniature_width,    //in
                                   uint16                           miniature_height,   //in
                                   GUIANIM_MINIATURE_FULL_PATH_T    *miniature_path_ptr //in/out
                                   );

/*****************************************************************************/
//  Description : the miniature is corresponding to the main picture
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsTwoFileCorresponding(
                                     wchar      *full_path_ptr,         //in
                                     uint16     full_path_len,          //in
                                     wchar      *miniature_full_path_ptr//in
                                     );

/*****************************************************************************/
//  Description : set miniature file folder hide
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetMiniatureFolderHide(
                                  CTRLANIM_OBJ_T    *anim_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description : post anim display indication msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PostDisplayIndMsg(
                             GUIANIM_RESULT_E   result,         //in
                             CTRLANIM_OBJ_T     *anim_ctrl_ptr  //in
                             );

/*****************************************************************************/
//  Description : handle anim get data confirm msg
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void HandleAnimGetDataCnf(
                                BOOLEAN                 is_display,     //in:
                                CTRLANIM_OBJ_T          *anim_ctrl_ptr, //in
                                GUIANIM_DEC_RESULT_T    *dec_result_ptr //in
                                );

/*****************************************************************************/
//  Description : set image info by decode/encode result
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL GUIANIM_SII_RESULT_E SetImageInfo(
                                        CTRLANIM_OBJ_T          *anim_ctrl_ptr, //in
                                        GUIANIM_DEC_RESULT_T    *dec_result_ptr //in
                                        );

/*****************************************************************************/
//  Description : set image info by decode/encode fail result
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL GUIANIM_SII_RESULT_E SetImageInfoFail(
                                            CTRLANIM_OBJ_T          *anim_ctrl_ptr, //in
                                            GUIANIM_DEC_RESULT_T    *dec_result_ptr //in
                                            );

/*****************************************************************************/
//  Description : creat miniature for jpg
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreatJpgMiniature(//创建缩略图是否成功
                                uint32          img_size,
                                CTRLANIM_OBJ_T  *anim_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : generate miniature file
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GenerateMiniatureFile(
                                    uint8               *data_ptr,      //in
                                    uint32              data_size,      //in
                                    CTRLANIM_OBJ_T      *anim_ctrl_ptr  //in
                                    );

/*****************************************************************************/
//  Description : get file size and creat time
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetFileSizeAndCreatTime(
                                      wchar             *full_path_ptr,     //in
                                      uint16            full_path_len,      //in
                                      SFS_FIND_DATA_T   *find_data_ptr      //in/out
                                      );

/*****************************************************************************/
//  Description : gif handle get data confirm msg
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void HandleGifGetDataCnf(
                               BOOLEAN                  is_display,     //in
                               CTRLANIM_OBJ_T           *anim_ctrl_ptr, //in
                               GUIANIM_DEC_RESULT_T     *dec_result_ptr //in
                               );

/*****************************************************************************/
//  Description : notify window gif anim update
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void NotifyWinGifAnimUpdate(
                                  CTRLANIM_OBJ_T    *anim_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description : handle anim timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleAnimTimer(
                           CTRLANIM_OBJ_T   *anim_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : handle gif timer
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void HandleGifTimer(
                          CTRLANIM_OBJ_T    *anim_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : handle anim get focus message
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void HandleAnimGetFocus(
                              CTRLANIM_OBJ_T    *anim_ctrl_ptr
                              );

/*****************************************************************************/
//  Description : handle anim lose focus message
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void HandleAnimLoseFocus(
                               CTRLANIM_OBJ_T   *anim_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : re-decode anim control
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RedecodeAnimCtrl(//is re-decode
                               GUI_RECT_T       old_rect,       //in:
                               CTRLANIM_OBJ_T   *anim_ctrl_ptr  //in:
                               );


/*****************************************************************************/
//  Description : set anim control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AnimCtrlSetRect(
                              CTRLBASE_OBJ_T    *base_ctrl_ptr,
                              const CAF_RECT_T  *rect_ptr
                              );

/*****************************************************************************/
//  Description : set anim border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AnimCtrlSetBorder(
                                CTRLBASE_OBJ_T  *base_ctrl_ptr,
                                GUI_BORDER_T    *border_ptr
                                );

/*****************************************************************************/
//  Description : set anim bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AnimCtrlSetBg(
                            CTRLBASE_OBJ_T  *base_ctrl_ptr,
                            GUI_BG_T        *bg_ptr
                            );

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get anim type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T ANIM_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                  // parent class type
                       "anim",                                         // control name
                       sizeof (CTRLANIM_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) AnimCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) AnimCtrlDestruct,      // control destructor function
                       sizeof (CTRLANIM_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) AnimCtrlPackInitParam,     // convert init param
                       sizeof (CTRLANIM_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) AnimCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init anim class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AnimCtrlInitVtbl (
    CTRLANIM_VTBL_T        *anim_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) anim_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = AnimCtrlHandleMsg;
    base_ctrl_vtbl_ptr->SetRect = AnimCtrlSetRect;
    base_ctrl_vtbl_ptr->SetBorder = AnimCtrlSetBorder;
    base_ctrl_vtbl_ptr->SetBg = AnimCtrlSetBg;

    // 以下接口用于实现内嵌form
    base_ctrl_vtbl_ptr->GetHeightByWidth = AnimCtrlGetHeightByWidth;
    base_ctrl_vtbl_ptr->SetDisplayRect = AnimCtrlSetDisplayRect;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 该方法用于将控件的对外初始化参数结构转换为内部的初始化参数结构
// 由于控件有继承关系，控件初始化时又带有参数，所以内部的初始化参数也必须有继承关系
/*****************************************************************************/
LOCAL void AnimCtrlPackInitParam (
    GUIANIM_INIT_DATA_T     *anim_init_ptr,   //in
    CTRLANIM_INIT_PARAM_T   *anim_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) anim_param_ptr;

    if (PNULL == anim_init_ptr || PNULL == anim_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = anim_init_ptr->both_rect;
}

/*****************************************************************************/
//  Description : anim construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AnimCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLANIM_INIT_PARAM_T   *anim_param_ptr
)
{
    BOOLEAN             result = FALSE;
    CTRLANIM_OBJ_T     *anim_ctrl_ptr = (CTRLANIM_OBJ_T*) base_ctrl_ptr;

    if ((PNULL != base_ctrl_ptr) && (PNULL != anim_param_ptr))
    {
        //get anim theme
        MMITHEME_GetAnimTheme(&anim_ctrl_ptr->theme);

        base_ctrl_ptr->lcd_dev_info    = anim_ctrl_ptr->theme.lcd_dev;
        anim_ctrl_ptr->display_info.bg = anim_ctrl_ptr->theme.default_bg;
        anim_ctrl_ptr->display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;

        //set anim other info
        anim_ctrl_ptr->is_form_bg   = anim_param_ptr->is_form_bg;
        anim_ctrl_ptr->win_handle   = MMK_GetWinHandleByCtrl(base_ctrl_ptr->handle);

        //get miniature width and height
        GUILCD_GetLogicWidthHeight(
            base_ctrl_ptr->lcd_dev_info.lcd_id,
            &anim_ctrl_ptr->miniature_width,
            &anim_ctrl_ptr->miniature_height);

        VTLBASE_SetTpEnabled(base_ctrl_ptr, FALSE);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : anim destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AnimCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLANIM_OBJ_T     *anim_ctrl_ptr = (CTRLANIM_OBJ_T*) base_ctrl_ptr;

    if (PNULL == anim_ctrl_ptr)
    {
        return FALSE;
    }

    ResetAndFreeMemory(anim_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : handle anim msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E AnimCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLANIM_OBJ_T     *anim_ctrl_ptr = (CTRLANIM_OBJ_T*) base_ctrl_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != ctrl_ptr); /*assert verified*/
    if (PNULL == anim_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_OPEN:
        anim_ctrl_ptr->is_focus = TRUE;
        break;

    case MSG_CTL_PAINT:
        //reload resource
        LoadAnimOrImgRes(anim_ctrl_ptr);

        //display image
        GUIANIM_Display(TRUE,FALSE,anim_ctrl_ptr);
        break;

    case MSG_CTL_ANIM_GET_DATA_CNF:
        HandleAnimGetDataCnf(TRUE,anim_ctrl_ptr,(GUIANIM_DEC_RESULT_T *)param);
        break;

    case MSG_TIMER:
        if (anim_ctrl_ptr->timer_id == *(uint8*)param)
        {
            //anim timer
            HandleAnimTimer(anim_ctrl_ptr);
        }
        else  if (anim_ctrl_ptr->gif_timer_id == *(uint8*)param)
        {
            //gif timer
            HandleGifTimer(anim_ctrl_ptr);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    case MSG_CTL_GET_FOCUS:
        HandleAnimGetFocus(anim_ctrl_ptr);
        break;

    case MSG_CTL_LOSE_FOCUS:
        HandleAnimLoseFocus(anim_ctrl_ptr);
        break;

    case MSG_CTL_GET_ACTIVE:
        //notify parent control
        result = GUICTRL_SendNotify(base_ctrl_ptr->handle,MSG_NOTIFY_GET_ACTIVE);
        break;

    case MSG_CTL_LOSE_ACTIVE:
        //notify parent control
        result = GUICTRL_SendNotify(base_ctrl_ptr->handle,MSG_NOTIFY_LOSE_ACTIVE);
        break;

    case MSG_CTL_CLOSE:
        anim_ctrl_ptr->is_focus = FALSE;
        break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        //MMK_DelayTpMove();
        if (TRUE != anim_ctrl_ptr->is_pressed)
        {
            //set pressed
            anim_ctrl_ptr->is_pressed = TRUE;
            GUICTRL_PostNotify(base_ctrl_ptr->handle, MSG_NOTIFY_PENOK);
        }
     break;
     case MSG_TP_PRESS_UP:
         if (TRUE == anim_ctrl_ptr->is_pressed)
         {
             //set release
             anim_ctrl_ptr->is_pressed = FALSE;
         }
     break;
#endif //TOUCH_PANEL_SUPPORT

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set anim control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AnimCtrlSetRect(
                              CTRLBASE_OBJ_T    *base_ctrl_ptr,
                              const CAF_RECT_T  *rect_ptr
                              )
{
    BOOLEAN             result = TRUE;
    CTRLANIM_OBJ_T      *anim_ctrl_ptr = (CTRLANIM_OBJ_T *)base_ctrl_ptr;

    CTRLANIM_SetCtrlRectByPtr(anim_ctrl_ptr,(GUI_RECT_T *)rect_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : set anim border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AnimCtrlSetBorder(
                                CTRLBASE_OBJ_T  *base_ctrl_ptr,
                                GUI_BORDER_T    *border_ptr
                                )
{
    BOOLEAN             result = TRUE;
    CTRLANIM_OBJ_T      *anim_ctrl_ptr = (CTRLANIM_OBJ_T *)base_ctrl_ptr;

    //set border
    anim_ctrl_ptr->theme.border = *border_ptr;

    return (result);
}

/*****************************************************************************/
//  Description : set anim bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AnimCtrlSetBg(
                            CTRLBASE_OBJ_T  *base_ctrl_ptr,
                            GUI_BG_T        *bg_ptr
                            )
{
    BOOLEAN             result = TRUE;
    CTRLANIM_OBJ_T      *anim_ctrl_ptr = PNULL;

    anim_ctrl_ptr = (CTRLANIM_OBJ_T *)base_ctrl_ptr;

    switch (bg_ptr->bg_type)
    {
    case GUI_BG_NONE:
        break;

    case GUI_BG_IMG:
        result = CTRLANIM_SetBgImgByPtr(anim_ctrl_ptr,bg_ptr->img_id);
        break;

    case GUI_BG_COLOR:
        CTRLANIM_SetBgColorByPtr(anim_ctrl_ptr,bg_ptr->color);
        break;

    default:
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get anim height,include border,anim etc
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AnimCtrlGetHeightByWidth(
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
)
{
    BOOLEAN         result = FALSE;
    BOOLEAN         is_border = TRUE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = (CTRLANIM_OBJ_T *)base_ctrl_ptr;

    //is file,asynchronous decode
    if ((0 < anim_ctrl_ptr->file_info.full_path_wstr_len) &&
        (!anim_ctrl_ptr->display_info.is_syn_decode))
    {
        //default width and height
    }
    else
    {
        if (anim_ctrl_ptr->is_decoded)
        {
            //anim is exist border
            if (GUI_BORDER_NONE == anim_ctrl_ptr->theme.border.type)
            {
                is_border = FALSE;
            }

            if (is_border)
            {
                *height_ptr = (uint16)(anim_ctrl_ptr->img_height + (anim_ctrl_ptr->theme.border.width << 1));
            }
            else
            {
                *height_ptr = anim_ctrl_ptr->img_height;
            }

            result = TRUE;
        }
    }

    GUI_INVALID_PARAM(width); /*lint !e522*/

    return (result);
}

/*****************************************************************************/
//  Description : set anim display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AnimCtrlSetDisplayRect(
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
)
{
    BOOLEAN         result = TRUE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    anim_ctrl_ptr = (CTRLANIM_OBJ_T *)base_ctrl_ptr;

    //kevin.lou modified:delete assert
    if ((PNULL != anim_ctrl_ptr)&&(PNULL != rect_ptr))
    {
        //SCI_ASSERT(PNULL != rect_ptr);  /*assert verified*/
        SCI_ASSERT(GUIANIM_TypeOf((CTRLBASE_OBJ_T *)anim_ctrl_ptr));  /*assert verified*/
    
        base_ctrl_ptr->display_rect = *rect_ptr;

        if (is_update)
        {
            //reload resource
            LoadAnimOrImgRes(anim_ctrl_ptr);

            //display
            GUIANIM_Display(TRUE,FALSE,anim_ctrl_ptr);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : reset timer and free memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetAndFreeMemory(
                              CTRLANIM_OBJ_T    *anim_ctrl_ptr
                              )
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    //set anim is not pause
    anim_ctrl_ptr->is_pause = FALSE;

    //not decoding
    anim_ctrl_ptr->is_decoding = FALSE;

    //not decode jpg thumbnail
    anim_ctrl_ptr->is_decode_thumb = FALSE;

    //not encode jpg
    anim_ctrl_ptr->is_encode_jpg = FALSE;

    //set anim is support
    anim_ctrl_ptr->is_support = TRUE;
    anim_ctrl_ptr->is_support_zoom = TRUE;

    //don't complete decode
    anim_ctrl_ptr->is_decoded = FALSE;

    //set image don't adjust display position
    anim_ctrl_ptr->is_adjust_disp_pos = FALSE;

    //set image don't send display indcation msg
    anim_ctrl_ptr->is_send_disp_ind = FALSE;

    //set image
    anim_ctrl_ptr->src_width  = 0;
    anim_ctrl_ptr->src_height = 0;
    anim_ctrl_ptr->img_width  = 0;
    anim_ctrl_ptr->img_height = 0;

    //set gif frame
    anim_ctrl_ptr->cur_frame = 0;
    anim_ctrl_ptr->total_frame = 0;
    anim_ctrl_ptr->is_gif_wallpaper_saved = FALSE;

    //stop anim timer
    GUIANIM_StopAnimTimer(&anim_ctrl_ptr->timer_id);

    //stop gif timer
    GUIANIM_StopGifTimer(&anim_ctrl_ptr->gif_timer_id);

    //free bg buffer
    if (PNULL != anim_ctrl_ptr->bg_buf_ptr)
    {
        SCI_FREE(anim_ctrl_ptr->bg_buf_ptr);
    }

    //free save data buffer
    if ((anim_ctrl_ptr->data_info.is_save_data) &&
        (PNULL != anim_ctrl_ptr->data_info.data_ptr) &&
        (PNULL != anim_ctrl_ptr->src_data_buffer))
    {
        SCI_FREE(anim_ctrl_ptr->src_data_buffer);
        anim_ctrl_ptr->data_info.is_save_data = FALSE;
    }

    //free data info
    anim_ctrl_ptr->data_info.img_id  = 0;
    anim_ctrl_ptr->data_info.anim_id = 0;
	anim_ctrl_ptr->data_info.is_free_bitmap = FALSE;

    //free file info
    if (PNULL != anim_ctrl_ptr->file_info.full_path_wstr_ptr)
    {
        SCI_FREE(anim_ctrl_ptr->file_info.full_path_wstr_ptr);
    }
    SCI_MEMSET(&anim_ctrl_ptr->file_info,0,sizeof(GUIANIM_FILE_INFO_T));
    
    //destroy image handle
    if (0 != anim_ctrl_ptr->img_handle)
    {
        GUIANIM_DestroyHandle(TRUE,
            anim_ctrl_ptr->img_handle,
            base_ctrl_ptr->handle);
        anim_ctrl_ptr->img_handle = 0;
    }

    //free gif resource handle
    if (0 != anim_ctrl_ptr->res_gif_handle)
    {
        GUIRES_FreeHandle(anim_ctrl_ptr->res_gif_handle);
        anim_ctrl_ptr->res_gif_handle = 0;

        anim_ctrl_ptr->is_dec_all_gif = FALSE;
    }

    //free target buffer
    FreeTargetBuffer(anim_ctrl_ptr);
    
    //free backup target buffer
    if (PNULL != anim_ctrl_ptr->target_backup_buf)
    {
        SCI_FREE(anim_ctrl_ptr->target_backup_buf);
        anim_ctrl_ptr->target_backup_offset = 0;
    }

    //set target data type
    anim_ctrl_ptr->target_data_type   = GUIIMG_DATE_TYPE_RGB565;

    //set zoom image
    anim_ctrl_ptr->zoom_dest.cur_img_width = 0;
    anim_ctrl_ptr->zoom_dest.cur_img_height = 0;

    //set zoom buffer width
    anim_ctrl_ptr->zoom_dest.buf_width  = 0;
    anim_ctrl_ptr->zoom_dest.buf_height = 0;

    //free zoom buffer
    if (PNULL != anim_ctrl_ptr->zoom_dest.buf_ptr)
    {
        SCI_FREE(anim_ctrl_ptr->zoom_dest.buf_ptr);
        anim_ctrl_ptr->zoom_dest.buf_size = 0;
        anim_ctrl_ptr->zoom_dest.buf_offset = 0;
    }
    //free rotate buffer
    if (PNULL != anim_ctrl_ptr->rotate_dest.buf_ptr)
    {
        SCI_FREE(anim_ctrl_ptr->rotate_dest.buf_ptr);
        anim_ctrl_ptr->rotate_dest.buf_size = 0;
        anim_ctrl_ptr->rotate_dest.buf_offset = 0;
    }
    //free mask path ptr and reset param
    if (PNULL != anim_ctrl_ptr->display_info.p_mask_path)
    {
        SCI_FREE(anim_ctrl_ptr->display_info.p_mask_path);
        anim_ctrl_ptr->display_info.mask_path_len = 0;
    }
    anim_ctrl_ptr->display_info.is_handle_mask = FALSE;
    anim_ctrl_ptr->display_info.mask_id = 0;

}

/*****************************************************************************/
//  Description : free target buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeTargetBuffer(
                            CTRLANIM_OBJ_T  *anim_ctrl_ptr
                            )
{
    //free target buffer
    if ((anim_ctrl_ptr->is_target_alloc) && 
        (PNULL != anim_ctrl_ptr->target_buffer))
    {
        SCI_FREE(anim_ctrl_ptr->target_buffer);
        anim_ctrl_ptr->target_buffer_size = 0;
        anim_ctrl_ptr->target_buf_offset = 0;
        anim_ctrl_ptr->is_target_alloc = FALSE;
    }
    
    anim_ctrl_ptr->target_buffer = PNULL;
}

/*****************************************************************************/
//  Description : free wallpaper target buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeWallpaperTargetBuffer(
                                     CTRLANIM_OBJ_T *anim_ctrl_ptr
                                     )
{
    if (anim_ctrl_ptr->is_wallpaper_ctrl)
    {
        if ((anim_ctrl_ptr->is_target_alloc) && 
            (PNULL != anim_ctrl_ptr->target_buffer) &&
            (anim_ctrl_ptr->theme.IsDynaAllocWallpaperBuf()))
        {
            //free buffer
            SCI_FREE(anim_ctrl_ptr->target_buffer);
            anim_ctrl_ptr->target_buffer_size = 0;
            anim_ctrl_ptr->target_buf_offset = 0;
            anim_ctrl_ptr->is_target_alloc = FALSE;

            anim_ctrl_ptr->is_decoded = FALSE;
        }
    }
}

/*****************************************************************************/
//  Description : set mask path
//  Global resource dependence :
//  Author: zirui.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetMaskPath(CTRLANIM_OBJ_T* p_anim_ctrl, GUIANIM_DISPLAY_INFO_T* p_display_info)
{
    if (PNULL == p_anim_ctrl || PNULL == p_display_info)
    {
        SCI_TRACE_LOW("p_anim_ctrl or p_display_info is PNULL!!!");
        return FALSE;
    }
    if (PNULL == p_display_info->p_mask_path || 0 == p_display_info->mask_path_len)
    {
        SCI_TRACE_LOW("p_display_info->p_mask_path is PNULL or p_display_info->mask_path_len is 0!!!");
        return FALSE;
    }
    p_anim_ctrl->display_info.p_mask_path = SCI_ALLOC_APPZ((p_display_info->mask_path_len + 1) * sizeof(wchar));
    if (PNULL == p_anim_ctrl->display_info.p_mask_path)
    {
        SCI_TRACE_LOW("p_anim_ctrl->display_info.p_mask_path malloc fail!!!");
        return FALSE;
    }
    MMI_WSTRNCPY(p_anim_ctrl->display_info.p_mask_path,p_display_info->mask_path_len + 1,
        p_display_info->p_mask_path,p_display_info->mask_path_len,
        p_display_info->mask_path_len);

    p_anim_ctrl->display_info.mask_path_len = p_display_info->mask_path_len;
    return TRUE;
}

/*****************************************************************************/
//  Description : set anim param
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUIANIM_RESULT_E CTRLANIM_SetParam(
                                         GUIANIM_CTRL_INFO_T    *ctrl_info_ptr,     //in:
                                         GUIANIM_DATA_INFO_T    *data_info_ptr,     //in:may PNULL
                                         GUIANIM_FILE_INFO_T    *file_info_ptr,     //in:may PNULL
                                         GUIANIM_DISPLAY_INFO_T *display_info_ptr   //in:
                                         )
{
    GUI_RECT_T          anim_rect = {0};
    CTRLANIM_OBJ_T      *anim_ctrl_ptr = PNULL;
    GUIANIM_RESULT_E    result = GUIANIM_RESULT_SUCC;

    //get anim control pointer
    if (ctrl_info_ptr->is_ctrl_id)
    {
        anim_ctrl_ptr = GUIANIM_GetPtr( ctrl_info_ptr->ctrl_id );
    }
    else
    {
        anim_ctrl_ptr = ctrl_info_ptr->ctrl_ptr;
    }

    if (PNULL != anim_ctrl_ptr)
    {
        //free memory and reset pointer
        ResetAndFreeMemory(anim_ctrl_ptr);

        //set is wallpaper control
        anim_ctrl_ptr->is_wallpaper_ctrl = ctrl_info_ptr->is_wallpaper_ctrl;

        //set display info
        anim_ctrl_ptr->display_info = *display_info_ptr;

        if (TRUE == display_info_ptr->is_handle_mask)
        {
            if (GUIANIM_MASK_TYPE_PATH == display_info_ptr->mask_type)
            {
                SetMaskPath(anim_ctrl_ptr,display_info_ptr);
            }
            else if (GUIANIM_MASK_TYPE_ID == display_info_ptr->mask_type)
            {
                if (0 == display_info_ptr->mask_id)
                {
                    SCI_TRACE_LOW("GUIANIM_MASK_TYPE_ID,mask_id is 0!!!");
                }
            }
        }

        //get anim rect,except border
        GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect);

        //set display position
        anim_ctrl_ptr->display_x = anim_rect.left;
        anim_ctrl_ptr->display_y = anim_rect.top;

        //get data buffer and decode
        if ((PNULL == data_info_ptr) && 
            (PNULL == file_info_ptr))
        {
            //nothing display
            anim_ctrl_ptr->type = GUIANIM_TYPE_NONE;
            anim_ctrl_ptr->is_support = FALSE;

            result = GUIANIM_RESULT_EMPTY;
        }
        else if (PNULL != data_info_ptr) //data buffer
        {
            //set data info
            anim_ctrl_ptr->data_info = *data_info_ptr;

            //set bitmap info
            if (SetAnimBitmap(anim_ctrl_ptr,data_info_ptr))
            {
                if ((anim_ctrl_ptr->is_wallpaper_ctrl) &&
                    (SaveFilePath(anim_ctrl_ptr,file_info_ptr)))
                {
                    anim_ctrl_ptr->data_info.is_bitmap = FALSE;
                }
            }
            else
            {
                //set and decode data
                result = SetAndDecodeData(anim_ctrl_ptr,data_info_ptr);
            }
        }
        else
        {
            if (SaveFilePath(anim_ctrl_ptr,file_info_ptr))
            {
                //decode image file
                result = DecodeImgBufOrFile(TRUE,FALSE,FALSE,anim_ctrl_ptr);
            }
            else
            {
                //nothing display
                //SCI_TRACE_LOW:"GUIANIM_SetParam:file name len is 0!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_976_112_2_18_3_14_50_0,(uint8*)"");
                anim_ctrl_ptr->type = GUIANIM_TYPE_NONE;
                anim_ctrl_ptr->is_support = FALSE;

                result = GUIANIM_RESULT_EMPTY;
            }
        }
    
        if (GUIANIM_RESULT_WAIT != result)
        {
            //post anim display indication msg
            PostDisplayIndMsg(result,anim_ctrl_ptr);
        }

        //display
        if (anim_ctrl_ptr->display_info.is_update)
        {
            GUIANIM_Display(FALSE,FALSE,anim_ctrl_ptr);
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set anim bitmap param
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetAnimBitmap(
                            CTRLANIM_OBJ_T          *anim_ctrl_ptr, //in:
                            GUIANIM_DATA_INFO_T     *data_info_ptr  //in:
                            )
{
    BOOLEAN         result = FALSE;
    uint16          img_width = 0;
    uint16          img_height = 0;
    GUI_RECT_T      anim_rect = {0};

    if ((PNULL != data_info_ptr) && 
        (data_info_ptr->is_bitmap) &&
        (PNULL != data_info_ptr->bitmap_data.bit_ptr))
    {
        //set image type,width,height and target buffer
        anim_ctrl_ptr->type               = GUIANIM_TYPE_IMAGE_BMP;
        anim_ctrl_ptr->src_width          = (uint16)(data_info_ptr->bitmap_data.img_width);
        anim_ctrl_ptr->src_height         = (uint16)(data_info_ptr->bitmap_data.img_height);
        anim_ctrl_ptr->img_width          = (uint16)(data_info_ptr->bitmap_data.img_width);
        anim_ctrl_ptr->img_height         = (uint16)(data_info_ptr->bitmap_data.img_height);
        anim_ctrl_ptr->target_buffer      = data_info_ptr->bitmap_data.bit_ptr;
        anim_ctrl_ptr->target_buffer_size = (uint32)(anim_ctrl_ptr->img_width * anim_ctrl_ptr->img_height * 2);
        anim_ctrl_ptr->target_data_type   = data_info_ptr->bitmap_data.date_type;
        if (data_info_ptr->is_free_bitmap)
        {
            anim_ctrl_ptr->is_target_alloc = TRUE;
        }

#if defined (GUIF_ANIM_ZOOM) && defined (GUIF_ANIM_ROTATE)
        //auto zoom or rotate image to display in anim rect
        if (GUIANIM_AutoZoom(anim_ctrl_ptr) &&
            GUIANIM_AutoRotate(anim_ctrl_ptr))
        {
            //get anim rect,except border
            if (GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect))
            {
                GUIANIM_GetImgWidthAndHeight(FALSE,FALSE,&img_width,&img_height,anim_ctrl_ptr);

                //adjust display position
                CTRLANIM_AdjustDisplayPosition(&anim_ctrl_ptr->display_x,
                    &anim_ctrl_ptr->display_y,
                    img_width,
                    img_height,
                    anim_rect,
                    anim_ctrl_ptr->display_info.align_style);

                //set adjust display pos flag
                anim_ctrl_ptr->is_adjust_disp_pos = TRUE;
            }

            //set image has decode
            anim_ctrl_ptr->is_decoded = TRUE;

            //set image is support
            anim_ctrl_ptr->is_support = TRUE;
        }
        else
#endif  //end of GUIF_ANIM_ZOOM && GUIF_ANIM_ROTATE
        {
            //set image is support
            anim_ctrl_ptr->is_support = FALSE;
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : adjust anim or image display position
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLANIM_AdjustDisplayPosition(
                                          int16                 *display_x_ptr, //in/out:
                                          int16                 *display_y_ptr, //in/out:
                                          uint16                pic_width,      //in:pic width
                                          uint16                pic_height,     //in:pic height
                                          GUI_RECT_T            rect,           //in
                                          GUIANIM_ALIGN_STYLE_E align_style     //in
                                          )
{
    int16       start_x = 0;
    int16       start_y = 0;
    uint16      shrect_width = 0;
    uint16      shrect_height = 0;

    if ((PNULL != display_x_ptr) && 
        (PNULL != display_y_ptr))
    {
        //set display x and y
        start_x = *display_x_ptr;
        start_y = *display_y_ptr;
    
        //set show rect width and height
        shrect_width = (uint16)(rect.right - rect.left + 1);
        shrect_height = (uint16)(rect.bottom - rect.top + 1);

        if ((0 != pic_width) && (0 != pic_height))
        {
            switch (align_style)
            {
            case GUIANIM_ALIGN_NONE:
                //don't adjust
                break;

            case GUIANIM_ALIGN_LEFT_TOP://左上
                //don/t adjust,default is left_top
                break;

            case GUIANIM_ALIGN_LEFT_VMIDDLE://左中
                if (shrect_height > pic_height+1)
                {
                    start_y = (int16)(((shrect_height-pic_height)>>1) + start_y);
                }
                break;

            case GUIANIM_ALIGN_LEFT_BOTTOM://左下
                if (shrect_height > pic_height)
                {
                    start_y = (int16)((shrect_height-pic_height) + start_y);
                }
                break;

            case GUIANIM_ALIGN_HMIDDLE_TOP://中上
                if (shrect_width > pic_width+1)
                {
                    start_x =(int16)(((shrect_width-pic_width)>>1) + start_x);
                }
                break;

            case GUIANIM_ALIGN_HVMIDDLE://中
                //set start x and start y
                if (shrect_width > pic_width+1)
                {
                    start_x = (int16)(((shrect_width-pic_width)>>1) + start_x);
                }

                if (shrect_height > pic_height+1)
                {
                    start_y = (int16)(((shrect_height-pic_height)>>1) + start_y);
                }
                break;

            case GUIANIM_ALIGN_HMIDDLE_BOTTOM://中下
                if (shrect_width > pic_width+1)
                {
                    start_x = (int16)(((shrect_width-pic_width)>>1) + start_x);
                }

                if (shrect_height > pic_height)
                {
                    start_y = (int16)((shrect_height-pic_height) + start_y);
                }
                break;

            case GUIANIM_ALIGN_RIGHT_TOP://右上
                if (shrect_width > pic_width)
                {
                    start_x = (int16)((shrect_width-pic_width) + start_x);
                }
                break;

            case GUIANIM_ALIGN_RIGHT_VMIDDLE://右中
                if (shrect_width > pic_width)
                {
                    start_x = (int16)((shrect_width-pic_width) + start_x);
                }

                if (shrect_height > pic_height+1)
                {
                    start_y = (int16)(((shrect_height-pic_height)>>1) + start_y);
                }
                break;

            case GUIANIM_ALIGN_RIGHT_BOTTOM://右下
                if (shrect_width > pic_width)
                {
                    start_x = (int16)((shrect_width-pic_width) + start_x);
                }

                if (shrect_height > pic_height)
                {
                    start_y = (int16)((shrect_height-pic_height) + start_y);
                }
                break;

            default:
                //SCI_TRACE_LOW:"GUIANIM_AdjustDisplayPosition:align_style %d is error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_1190_112_2_18_3_14_50_1,(uint8*)"d",align_style);
                break;
            }

            //adjust display x and y
            *display_x_ptr = start_x;
            *display_y_ptr = start_y;
        }
    }
}

/*****************************************************************************/
//  Description : set anim data param and decode data buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E SetAndDecodeData(
                                        CTRLANIM_OBJ_T      *anim_ctrl_ptr, //in:
                                        GUIANIM_DATA_INFO_T *data_info_ptr  //in:
                                        )
{
    GUIANIM_RESULT_E    result = GUIANIM_RESULT_SUCC;

    SCI_ASSERT(PNULL != data_info_ptr); /*assert verified*/
    SCI_ASSERT(!data_info_ptr->is_bitmap); /*assert verified*/

    //load resource data buffer
    if (!LoadAnimOrImgRes(anim_ctrl_ptr))
    {
        if (data_info_ptr->is_save_data)
        {
            //save data buffer
            anim_ctrl_ptr->src_data_size   = data_info_ptr->data_size;
            anim_ctrl_ptr->src_data_buffer = SCI_ALLOC(data_info_ptr->data_size);
            MMI_MEMCPY(anim_ctrl_ptr->src_data_buffer,data_info_ptr->data_size,
                data_info_ptr->data_ptr,data_info_ptr->data_size,
                data_info_ptr->data_size);
        }
        else
        {
            anim_ctrl_ptr->src_data_buffer = data_info_ptr->data_ptr;
            anim_ctrl_ptr->src_data_size   = data_info_ptr->data_size;
        }
    }

    if (PNULL == anim_ctrl_ptr->src_data_buffer)
    {
        //nothing display
        result = GUIANIM_RESULT_EMPTY;
    }
    else
    {
        if (PNULL != anim_ctrl_ptr->data_info.data_ptr)
        {
            //decode image buffer
            result = DecodeImgBufOrFile(TRUE,FALSE,FALSE,anim_ctrl_ptr);
        }
        else
        {
            //decode resource image or anim
            result = DecodeAnimOrImgRes(anim_ctrl_ptr);
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : load anim or image resource
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN LoadAnimOrImgRes(
                               CTRLANIM_OBJ_T   *anim_ctrl_ptr
                               )
{
    BOOLEAN     result = FALSE;
    uint32      img_data_size = 0;

    //发送full paint消息给背景窗口anim时,需要重新load resource
    //reload anim or image resource
    if (0 != anim_ctrl_ptr->data_info.anim_id)
    {
        anim_ctrl_ptr->cur_frame       = 0;
        anim_ctrl_ptr->src_data_buffer = (uint8 *)anim_ctrl_ptr->theme.GetLabelAnim(anim_ctrl_ptr->data_info.anim_id,anim_ctrl_ptr->win_handle,&img_data_size);
        anim_ctrl_ptr->src_data_size   = img_data_size;

        result = TRUE;
    }
    else
    {
        if (0 != anim_ctrl_ptr->data_info.img_id)
        {
            uint8 * buffer_ptr= (uint8 *)anim_ctrl_ptr->theme.GetLabelImg(anim_ctrl_ptr->data_info.img_id,anim_ctrl_ptr->win_handle,&img_data_size);
            if (((anim_ctrl_ptr->src_data_buffer != buffer_ptr)||(anim_ctrl_ptr->src_data_size != img_data_size)) 
                && (anim_ctrl_ptr->is_wallpaper_ctrl) 
                && (anim_ctrl_ptr->display_info.is_save_wallpaper))
            {
               
                FreeWallpaperTargetBuffer(anim_ctrl_ptr);

                /**///for cr247898
                if (/*(anim_ctrl_ptr->src_data_size != img_data_size)&&*/
                    (PNULL != buffer_ptr)&&
                    (0 != img_data_size)
                    )
                {
                    anim_ctrl_ptr->src_data_buffer = buffer_ptr;
                    anim_ctrl_ptr->src_data_size = img_data_size;
                    DecodeAnimOrImgRes(anim_ctrl_ptr);
                }                
            }

            anim_ctrl_ptr->src_data_buffer = buffer_ptr;
            anim_ctrl_ptr->src_data_size = img_data_size;
 
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : save image file path
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveFilePath(
                           CTRLANIM_OBJ_T       *anim_ctrl_ptr, //in:
                           GUIANIM_FILE_INFO_T  *file_info_ptr  //in:may PNULL
                           )
{
    BOOLEAN     result = FALSE;
    
    if ((PNULL != file_info_ptr) && 
        (0 < file_info_ptr->full_path_wstr_len))
    {
        //set file info
        anim_ctrl_ptr->file_info.full_path_wstr_len = file_info_ptr->full_path_wstr_len;
        anim_ctrl_ptr->file_info.full_path_wstr_ptr = SCI_ALLOC_APP((file_info_ptr->full_path_wstr_len + 1) * sizeof(wchar));

        SCI_MEMSET(anim_ctrl_ptr->file_info.full_path_wstr_ptr,0,((file_info_ptr->full_path_wstr_len + 1) * sizeof(wchar)));

        MMI_WSTRNCPY(anim_ctrl_ptr->file_info.full_path_wstr_ptr,file_info_ptr->full_path_wstr_len + 1,
            file_info_ptr->full_path_wstr_ptr,file_info_ptr->full_path_wstr_len,
            file_info_ptr->full_path_wstr_len);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : decode image buffer or file
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E DecodeImgBufOrFile(
                                          BOOLEAN           is_first,       //in:is first decode
                                          BOOLEAN           is_thumb_fail,  //in:is decode thumbnail fail
                                          BOOLEAN           is_encode_fail, //in:is encode jpg fail
                                          CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in:
                                          )
{
    BOOLEAN                 is_decode_miniature = FALSE;
    GUI_RECT_T              anim_rect = {0};
    GUIANIM_RESULT_E        result = GUIANIM_RESULT_FAIL;
    GUIANIM_TARGET_T        dec_target = {0};
    GUIANIM_DEC_RESULT_T    dec_result = {0};

    if (GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect))
    {
        //init decode result
        dec_result.result = GUIANIM_RESULT_FAIL;

        
        //set decode/encode targe
        result = SetDecEncTarget(is_first,
                    is_thumb_fail,
                    is_encode_fail,
                    &is_decode_miniature,
                    anim_ctrl_ptr,
                    &dec_target);
        if (GUIANIM_RESULT_SUCC == result)
        {
            //create handle and get frame
            result = CreateHandleAndGetFrame(is_decode_miniature,
                            anim_ctrl_ptr,
                            &dec_target,
                            &dec_result);
            if (GUIANIM_RESULT_MINIATURE_FAIL == result)
            {
                result = CreateHandleAndGetFrame(FALSE,
                            anim_ctrl_ptr,
                            &dec_target,
                            &dec_result);
            }
        }

        if (anim_ctrl_ptr->display_info.is_syn_decode)
        {
            //handle decode result
            HandleAnimGetDataCnf(FALSE,anim_ctrl_ptr,&dec_result);
        }
        else
        {
            if ((GUIANIM_RESULT_SUCC != result) && 
                (GUIANIM_RESULT_WAIT != result ) &&
                (GUIANIM_RESULT_GIF_MULTI_LACK_MEMORY != result))
            {
                //free target buffer
                FreeTargetBuffer(anim_ctrl_ptr);

                //free backup target buffer
                if (PNULL != anim_ctrl_ptr->target_backup_buf)
                {
                    SCI_FREE(anim_ctrl_ptr->target_backup_buf);
                    anim_ctrl_ptr->target_backup_offset = 0;
                }

                //not decode jpg thumbnail
                anim_ctrl_ptr->is_decode_thumb = FALSE;

                //not encode jpg
                anim_ctrl_ptr->is_encode_jpg = FALSE;

                //set image has decode
                anim_ctrl_ptr->is_decoded = FALSE;

                //set image is not support
                anim_ctrl_ptr->is_support = FALSE;
            }
        }
    }
    else
    {
        SCI_TRACE_LOW("GUIANIM:DecodeImgBufOrFile:anim rect is empty!");        
    }

    return (result);
}

/*****************************************************************************/
//  Description : set decode or encode target info
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E SetDecEncTarget(
                                       BOOLEAN          is_first,           //in:is first decode
                                       BOOLEAN          is_thumb_fail,      //in:is decode thumbnail fail
                                       BOOLEAN          is_encode_fail,     //in:is encode jpg fail
                                       BOOLEAN          *is_miniatrure_ptr, //in/out:is decode miniature
                                       CTRLANIM_OBJ_T   *anim_ctrl_ptr,     //in/out:
                                       GUIANIM_TARGET_T *target_ptr         //in/out:
                                       )
{
    GUI_RECT_T          crop_rect = {0};
    GUIANIM_RESULT_E    result = GUIANIM_RESULT_SUCC;
    GUIANIM_IMG_INFO_T  img_info = {0};

    //get image info
    if (is_first)
    {
        result = GetImageInfo(anim_ctrl_ptr,&img_info);

        if ((!anim_ctrl_ptr->display_info.is_no_miniatur)&&(GUIANIM_TYPE_IMAGE_ICO != anim_ctrl_ptr->type))
        {
            //get miniature file name
            GetMiniaturFileName(anim_ctrl_ptr);
        }      
        
    }
    
    if (GUIANIM_RESULT_SUCC == result)
    {
        //get jpg decode/encode info,is decode thumbnail,is encode jpg
        GetJpgDecEncInfo(is_first,
            is_thumb_fail,
            is_encode_fail,
            is_miniatrure_ptr,
            anim_ctrl_ptr,
            &img_info);

        //set target
        if (anim_ctrl_ptr->is_encode_jpg)
        {
            //set target type
            target_ptr->target_type     = GUIIMG_DATE_TYPE_JPG;
            target_ptr->is_decode_thumb = FALSE;

            //set target width,height
            target_ptr->target_width  = anim_ctrl_ptr->miniature_width;
            target_ptr->target_height = anim_ctrl_ptr->miniature_height;

            //set target buffer size
            target_ptr->target_buf_size = target_ptr->target_width*target_ptr->target_height;
        }
        else
        {
            //set is decode thumb
            target_ptr->is_decode_thumb = anim_ctrl_ptr->is_decode_thumb;

            //get target width,height
            GUIANIM_GetTargetWidthAndHeight(anim_ctrl_ptr->display_info.is_zoom,
                &target_ptr->target_width,
                &target_ptr->target_height,
                anim_ctrl_ptr);

            //get crop rect
            if (GetCropRect(*is_miniatrure_ptr,&crop_rect,anim_ctrl_ptr))
            {
                target_ptr->crop_rect = crop_rect;
            }

            //set target type and buffer size
            if ((anim_ctrl_ptr->display_info.is_handle_transparent) &&
                ((GUIANIM_TYPE_IMAGE_GIF == anim_ctrl_ptr->type) || (GUIANIM_TYPE_IMAGE_PNG == anim_ctrl_ptr->type)))
            {
                //gif or png,may transparent
                target_ptr->target_type     = GUIIMG_DATE_TYPE_ARGB888;
                target_ptr->target_buf_size = target_ptr->target_width*target_ptr->target_height*4;
            }
            else if (GUIANIM_TYPE_IMAGE_ICO == anim_ctrl_ptr->type)
            {
                target_ptr->target_type     = GUIIMG_DATE_TYPE_ARGB888;
                target_ptr->target_buf_size = target_ptr->target_width*target_ptr->target_height*4;
            }
            else if(GUIANIM_TYPE_IMAGE_WEBP == anim_ctrl_ptr->type)
            {
                target_ptr->target_type     = GUIIMG_DATE_TYPE_ARGB888;
                target_ptr->target_buf_size = target_ptr->target_width*target_ptr->target_height*4;
            }
            else
            {
                target_ptr->target_type     = GUIIMG_DATE_TYPE_RGB565;
                target_ptr->target_buf_size = target_ptr->target_width*target_ptr->target_height*2;
            }
        }

        //set target data type
        anim_ctrl_ptr->target_data_type = target_ptr->target_type;

        //alloc target buffer
        result = AllocTargetBuffer(anim_ctrl_ptr,target_ptr);
    }
    if (GUIANIM_TYPE_IMAGE_ICO == anim_ctrl_ptr->type)
    {
        anim_ctrl_ptr->display_info.is_syn_decode = TRUE;
        target_ptr->ico_frame_index = anim_ctrl_ptr->ico_frame_index;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get image info
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E GetImageInfo(
                                    CTRLANIM_OBJ_T      *anim_ctrl_ptr, //in/out:
                                    GUIANIM_IMG_INFO_T  *img_info_ptr   //in/out:
                                    )
{
    uint16              *file_path_ptr = 0;
    uint16              miniature_min = 0;
    uint16              miniature_max = 0;
    GUIANIM_RESULT_E    result = GUIANIM_RESULT_SUCC;
    GUIANIM_IMG_DATA_T  src_data = {0};
    uint32              img_handle = 0;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    //set src data
    src_data.src_buf_ptr   = anim_ctrl_ptr->src_data_buffer;
    src_data.src_data_size = anim_ctrl_ptr->src_data_size;

    //set file path
    file_path_ptr = anim_ctrl_ptr->file_info.full_path_wstr_ptr;

    //get image info
    //result = GUIANIM_GetInfo(anim_ctrl_ptr,file_path_ptr,&src_data,img_info_ptr);
    result = GUIANIM_CreateImgHandlAndGetInfo(file_path_ptr,&src_data,img_info_ptr,&base_ctrl_ptr->rect,&img_handle);

    if (GUIANIM_RESULT_SUCC == result)
    {
        if (img_info_ptr->is_support)
        {
            //set image type
            anim_ctrl_ptr->type             = img_info_ptr->type;            
            anim_ctrl_ptr->ico_frame_index  = img_info_ptr->ico_frame_index;
            anim_ctrl_ptr->img_handle       = img_handle;
            
            //set image width and height
            anim_ctrl_ptr->src_width  = img_info_ptr->img_width;
            anim_ctrl_ptr->src_height = img_info_ptr->img_height;
            anim_ctrl_ptr->img_width  = img_info_ptr->img_width;
            anim_ctrl_ptr->img_height = img_info_ptr->img_height;

            //设置缩略图长边和短边的值
            if (anim_ctrl_ptr->miniature_width > anim_ctrl_ptr->miniature_height)
            {
                miniature_min = anim_ctrl_ptr->miniature_height;
                miniature_max = anim_ctrl_ptr->miniature_width;
            }
            else
            {
                miniature_min = anim_ctrl_ptr->miniature_width;
                miniature_max = anim_ctrl_ptr->miniature_height;
            }

            //切换生成缩略图的长和宽，避免encode jpg压缩裁减
            if (anim_ctrl_ptr->img_width > anim_ctrl_ptr->img_height)
            {
                anim_ctrl_ptr->miniature_width  = miniature_max;
                anim_ctrl_ptr->miniature_height = miniature_min;
            }
            else
            {
                anim_ctrl_ptr->miniature_width  = miniature_min;
                anim_ctrl_ptr->miniature_height = miniature_max;
            }
        }
        else
        {
            SCI_TRACE_LOW("GUIANIM:GetImageInfo:image is not support!");            
            result = GUIANIM_RESULT_NO_SUPPORT;
        }
    }
    
    if ((GUIANIM_RESULT_SUCC != result)&&(0 != img_handle))
    {
        GUIANIM_DestroyHandle(FALSE,img_handle,0);
    }

    return (result);
}

/*****************************************************************************/
//  Description : get jpg decode/encode info,is decode thumbnail,is encode jpg
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void GetJpgDecEncInfo(
                            BOOLEAN             is_first,           //in:is first decode
                            BOOLEAN             is_thumb_fail,      //in:is decode thumbnail fail
                            BOOLEAN             is_encode_fail,     //in:is encode jpg fail
                            BOOLEAN             *is_miniatrure_ptr, //in/out:is decode miniature
                            CTRLANIM_OBJ_T      *anim_ctrl_ptr,     //in/out:
                            GUIANIM_IMG_INFO_T  *img_info_ptr       //in:
                            )
{
    if (GUIANIM_TYPE_IMAGE_JPG == anim_ctrl_ptr->type)
    {
        //is decode thumbnail,yes:decode directly
        if ((is_first) &&
            (!is_thumb_fail) &&
            (IsDecodeThumbnail(anim_ctrl_ptr,img_info_ptr)))
        {
            //decode thumbnail
            anim_ctrl_ptr->is_decode_thumb = TRUE;
        }
        else
        {
            //is exist miniature
            if (anim_ctrl_ptr->is_exist_miniature)
            {
                //decode directly miniature
                *is_miniatrure_ptr = TRUE;
            }
            else
            {
                //is encode jpg
                if ((!is_encode_fail) &&
                    (IsEncodeJpg(anim_ctrl_ptr)))
                {
                    //encode jpg
                    anim_ctrl_ptr->is_encode_jpg = TRUE;
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : is decode jpg thumbnail
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsDecodeThumbnail(
                                CTRLANIM_OBJ_T      *anim_ctrl_ptr, //in:
                                GUIANIM_IMG_INFO_T  *img_info_ptr   //in:
                                )
{
    BOOLEAN     result = FALSE;
    uint16      shrect_width = 0;
    uint16      shrect_height = 0;

    if ((GUIANIM_TYPE_IMAGE_JPG == img_info_ptr->type) &&
        (!anim_ctrl_ptr->is_wallpaper_ctrl) && 
        (!anim_ctrl_ptr->display_info.is_zoom) && 
        (img_info_ptr->jpg_thumbnail.is_existed))
    {
        //get show width,height
        GUIANIM_GetTargetWidthAndHeight(FALSE,
            &shrect_width,
            &shrect_height,
            anim_ctrl_ptr);

        //set thumbnail width and height
        anim_ctrl_ptr->thumb_width  = img_info_ptr->jpg_thumbnail.thumb_width;
        anim_ctrl_ptr->thumb_height = img_info_ptr->jpg_thumbnail.thumb_height;

        if ((anim_ctrl_ptr->thumb_width * anim_ctrl_ptr->thumb_height) >= (shrect_width*shrect_height))
        {
            result = TRUE;
            SCI_TRACE_LOW("GUIANIM:IsDecodeThumbnail:thumbnail %d*%d >= display %d*%d!",anim_ctrl_ptr->thumb_width,anim_ctrl_ptr->thumb_height,shrect_width,shrect_height);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : is encode jpg
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsEncodeJpg(
                          CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in
                          )
{
    BOOLEAN     result = FALSE;

    //judge width*heught>=640*480
    //wallpaper/synchronous decode/data buffer/ decode,not encode
    if ((GUIANIM_TYPE_IMAGE_JPG == anim_ctrl_ptr->type) && 
        (!anim_ctrl_ptr->is_wallpaper_ctrl) &&
        (!anim_ctrl_ptr->display_info.is_zoom) && 
        (!anim_ctrl_ptr->display_info.is_syn_decode) &&
        (PNULL != anim_ctrl_ptr->file_info.full_path_wstr_ptr) &&
        (0 < anim_ctrl_ptr->file_info.full_path_wstr_len) &&
        (anim_ctrl_ptr->theme.min_resolution <= (uint32)(((anim_ctrl_ptr->img_width)*(anim_ctrl_ptr->img_height)))))
    {
        //judge free size
        if (FreeSpaceIsEnough(anim_ctrl_ptr->file_info.full_path_wstr_ptr,anim_ctrl_ptr))
        {
            //encode
            result = TRUE;
        }
        else
        {
            SCI_TRACE_LOW("GUIANIM:IsEncodeJpg:current disk is no enough free space!");
            
        }
    }
    else
    {
        SCI_TRACE_LOW("GUIANIM:IsEncodeJpg:don't creat miniature pic,resolution is %d*%d",anim_ctrl_ptr->img_width,anim_ctrl_ptr->img_height);
    }

    return (result);
}

/*****************************************************************************/
//  Description : get current disk free memory is larger than 128K
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FreeSpaceIsEnough(
                                wchar           *full_path_ptr, //in:
                                CTRLANIM_OBJ_T  *anim_ctrl_ptr  //in
                                )
{
	BOOLEAN		result = FALSE;
	uint32		high_size = 0;
	uint32		low_size = 0;
	
	if ((PNULL != full_path_ptr) &&
		(PNULL != anim_ctrl_ptr))
	{
		anim_ctrl_ptr->theme.GetDeviceFreeSpace(full_path_ptr,1,&high_size,&low_size);
		if ((0 < high_size) ||
			(low_size >= (uint32)(anim_ctrl_ptr->miniature_width * anim_ctrl_ptr->miniature_height * 2)))
		{
			result = TRUE;
		}
	}

	return (result);
}

/*****************************************************************************/
//  Description : get image crop rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetCropRect(
                          BOOLEAN           is_miniature,   //in:
                          GUI_RECT_T        *crop_rect_ptr, //in:out
                          CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in
                          )
{
    BOOLEAN     result = FALSE;
    uint16      img_width = 0;
    uint16      img_height = 0;
    uint16      display_width = 0;
    uint16      display_height = 0;
    uint16      crop_width = 0;
    uint16      crop_height = 0;

    if ((PNULL != anim_ctrl_ptr) && 
        (PNULL != crop_rect_ptr) &&
        ((anim_ctrl_ptr->is_wallpaper_ctrl) || (anim_ctrl_ptr->display_info.is_crop_rect)))
    {
        //get image width and height
        GUIANIM_GetImgWidthAndHeight(is_miniature,TRUE,&img_width,&img_height,anim_ctrl_ptr);

        //get show width,height
        GUIANIM_GetTargetWidthAndHeight(FALSE,
            &display_width,
            &display_height,
            anim_ctrl_ptr);

        //宽,高大于显示宽高
        if ((img_width > display_width) &&
            (img_height > display_height))
        {
            if ((img_width*display_height) > (img_height*display_width))//crop width
            {
                //get crop width and height
                crop_width  = (uint16)((img_height*display_width)/display_height);
                crop_height = img_height;
                
                result = TRUE;
            }
            else if ((img_width*display_height) < (img_height*display_width))
            {
                //get crop width and height
                crop_height = (uint16)((img_width*display_height)/display_width);
                crop_width  = img_width;

                result = TRUE;
            }
            else    //宽,高和显示比例相同,不需要crop
            {
                result = FALSE;
            }
        }
        else if (img_width > display_width)  //宽大于显示宽
        {
            //get crop width and height
            crop_width  = (uint16)(display_width);
            crop_height = img_height;

            result = TRUE;
        }
        else if (img_height > display_height)    //高大于显示高
        {
            //get crop width and height
            crop_height = (uint16)(display_height);
            crop_width  = img_width;

            result = TRUE;
        }
        else
        {
            result = FALSE;
        }

        if (result)
        {
            //set crop rect
            crop_rect_ptr->left   = (int16)((img_width - crop_width) >> 1);
            crop_rect_ptr->right  = (int16)(crop_rect_ptr->left + crop_width - 1);
            crop_rect_ptr->top    = (int16)((img_height - crop_height) >> 1);
            crop_rect_ptr->bottom = (int16)(crop_rect_ptr->top + crop_height - 1);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : alloc target buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E AllocTargetBuffer(
                                         CTRLANIM_OBJ_T     *anim_ctrl_ptr, //in/out:
                                         GUIANIM_TARGET_T   *target_ptr     //in/out:
                                         )
{
    uint32              buf_size = 0;
    uint32              mode_num = 0;
    GUIANIM_RESULT_E    result = GUIANIM_RESULT_SUCC;

    if (PNULL == anim_ctrl_ptr->target_buffer)
    {
        //set target buffer size
        anim_ctrl_ptr->target_buffer_size = target_ptr->target_buf_size;

        buf_size = (uint32)(target_ptr->target_buf_size + GUIANIM_BUF_ALIGN);
        anim_ctrl_ptr->target_buffer = SCI_ALLOCA(buf_size);
        if (PNULL != anim_ctrl_ptr->target_buffer)
        {
            SCI_MEMSET(anim_ctrl_ptr->target_buffer,0,buf_size);
            anim_ctrl_ptr->is_target_alloc = TRUE;

            //调整 buffer 字节对齐
            mode_num = (uint32)(anim_ctrl_ptr->target_buffer) % GUIANIM_BUF_ALIGN;
            if (0 != mode_num)
            {
                anim_ctrl_ptr->target_buf_offset = GUIANIM_BUF_ALIGN - mode_num;
            }

            //set target buffer
            target_ptr->target_buf_ptr = anim_ctrl_ptr->target_buffer + anim_ctrl_ptr->target_buf_offset;
        }
        else
        {
            result = GUIANIM_RESULT_LACK_MEMORY;
            SCI_TRACE_LOW("GUIANIM:SetDecEncTarget:no target buffer! size %d " , buf_size);
        }
    }
    else
    {
        SCI_ASSERT(GUIANIM_TYPE_IMAGE_GIF == anim_ctrl_ptr->type); /*assert verified*/
        SCI_ASSERT(anim_ctrl_ptr->target_buffer_size >= target_ptr->target_buf_size); /*assert verified*/

        buf_size = (uint32)(target_ptr->target_buf_size + GUIANIM_BUF_ALIGN);

        if (PNULL == anim_ctrl_ptr->target_backup_buf)
        {
            anim_ctrl_ptr->target_backup_buf = SCI_ALLOCA(buf_size);
            if (PNULL != anim_ctrl_ptr->target_backup_buf)
            {
                SCI_MEMSET(anim_ctrl_ptr->target_backup_buf,0,buf_size);

                //调整 buffer 字节对齐
                mode_num = (uint32)(anim_ctrl_ptr->target_backup_buf) % GUIANIM_BUF_ALIGN;
                if (0 != mode_num)
                {
                    anim_ctrl_ptr->target_backup_offset = GUIANIM_BUF_ALIGN - mode_num;
                }

                //set target buffer
                target_ptr->target_buf_ptr = anim_ctrl_ptr->target_backup_buf + anim_ctrl_ptr->target_backup_offset;
            }
            else
            {
                anim_ctrl_ptr->total_frame = 1;
                result = GUIANIM_RESULT_GIF_MULTI_LACK_MEMORY;
                SCI_TRACE_LOW("GUIANIM:SetDecEncTarget:no gif backup buffer!");
            }
        }
        else
        {
            SCI_MEMSET(anim_ctrl_ptr->target_backup_buf,0,buf_size);

            //set target buffer
            target_ptr->target_buf_ptr = anim_ctrl_ptr->target_backup_buf + anim_ctrl_ptr->target_backup_offset;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : create handle and get frame
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E CreateHandleAndGetFrame(
                                               BOOLEAN              is_decode_miniature,//in:
                                               CTRLANIM_OBJ_T       *anim_ctrl_ptr,     //in:
                                               GUIANIM_TARGET_T     *dec_target_ptr,    //in:
                                               GUIANIM_DEC_RESULT_T *dec_result_ptr     //in/out:
                                               )
{
    uint16                  *file_path_ptr = PNULL;
    GUIANIM_RESULT_E        result = GUIANIM_RESULT_SUCC;
    GUIANIM_IMG_DATA_T      src_data = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    //set src data
    src_data.src_buf_ptr   = anim_ctrl_ptr->src_data_buffer;
    src_data.src_data_size = anim_ctrl_ptr->src_data_size;
    
    //set file path
    if (is_decode_miniature)
    {
        file_path_ptr = anim_ctrl_ptr->miniature_path.full_path_wstr;
        if (0 != anim_ctrl_ptr->img_handle)
        {
            GUIANIM_DestroyHandle(FALSE,anim_ctrl_ptr->img_handle,base_ctrl_ptr->handle);
            anim_ctrl_ptr->img_handle = 0;
        }
    }
    else
    {
        file_path_ptr = anim_ctrl_ptr->file_info.full_path_wstr_ptr;
    }

    //creat image handle
    if (0 == anim_ctrl_ptr->img_handle)
    {
        result = GUIANIM_CreatHandle(file_path_ptr,
                    &anim_ctrl_ptr->img_handle,
                    &src_data);
    }

    if (GUIANIM_RESULT_SUCC == result)
    {
        //decode/encode image
        anim_ctrl_ptr->is_decoding = TRUE;
        result = GUIANIM_GetFrame(anim_ctrl_ptr->display_info.is_syn_decode,
                                    anim_ctrl_ptr->img_handle,
                                    base_ctrl_ptr->handle,
                                    dec_target_ptr,
                                    dec_result_ptr);        
    }
    else
    {
        if (is_decode_miniature)
        {
            //delete the miniature file
            anim_ctrl_ptr->theme.DeleteFileSyn(anim_ctrl_ptr->miniature_path.full_path_wstr,
            anim_ctrl_ptr->miniature_path.full_path_wstr_len);

            anim_ctrl_ptr->is_exist_miniature = FALSE;
            
            result = GUIANIM_RESULT_MINIATURE_FAIL;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : save wallpaper bitmap info
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SaveWallpaperBitmap(
                               CTRLANIM_OBJ_T   *anim_ctrl_ptr
                               )
{
    GUIIMG_BITMAP_T     img_bitmap = {0};

    if ((anim_ctrl_ptr->is_wallpaper_ctrl) && 
        (anim_ctrl_ptr->display_info.is_save_wallpaper) &&
        (0 == anim_ctrl_ptr->data_info.img_id) && 
        (anim_ctrl_ptr->is_decoded) && 
        (PNULL != anim_ctrl_ptr->target_buffer))
    {
        //set bitmap
        GUIANIM_GetDisplayBitmap(anim_ctrl_ptr,&img_bitmap);

        //gif only save the first frame
        if (GUIANIM_TYPE_IMAGE_GIF == anim_ctrl_ptr->type)
        {
            if ((0 == anim_ctrl_ptr->cur_frame) &&
                (0 == anim_ctrl_ptr->total_frame)&&
                (!anim_ctrl_ptr->is_gif_wallpaper_saved))
            {
                //save wallpaper bitmap
                anim_ctrl_ptr->theme.SaveWallpaperBitmap(&img_bitmap);
                anim_ctrl_ptr->is_gif_wallpaper_saved = TRUE;
            }
        }
        else
        {
            //save wallpaper bitmap
            anim_ctrl_ptr->theme.SaveWallpaperBitmap(&img_bitmap);
        }
    }
}

/*****************************************************************************/
//  Description : destroy image handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyImgHandle(
                               CTRLANIM_OBJ_T           *anim_ctrl_ptr, //in
                               GUIANIM_DEC_RESULT_T     *dec_result_ptr //in
                               )
{
    BOOLEAN     result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    //set not decoding
    anim_ctrl_ptr->is_decoding = FALSE;

    if (GUIANIM_TYPE_IMAGE_GIF == anim_ctrl_ptr->type)
    {
        if (GUIANIM_RESULT_SUCC == dec_result_ptr->result)
        {
            if (dec_result_ptr->is_decode_gif_end)
            {
                //set total frame
                anim_ctrl_ptr->total_frame = dec_result_ptr->cur_frame_index;
                result = TRUE;
            }
            else if (anim_ctrl_ptr->display_info.is_disp_one_frame)
            {
                //set total frame is 1,lose focus,get focus不用重新加载
                anim_ctrl_ptr->total_frame = 1;
                result = TRUE;
            }
        }
        else
        {
            //set total frame
            anim_ctrl_ptr->total_frame = dec_result_ptr->cur_frame_index;
            result = TRUE;
        }
    }
    else
    {
        result = TRUE;
    }

    //destroy image handle
    if ((result) && 
        (0 < anim_ctrl_ptr->img_handle))
    {
        GUIANIM_DestroyHandle(FALSE,
            anim_ctrl_ptr->img_handle,
            base_ctrl_ptr->handle);
        anim_ctrl_ptr->img_handle = 0;
    }

    return (result);
}

/*****************************************************************************/
//  Description : decode anim or image resource
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E DecodeAnimOrImgRes(
                                          CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in
                                          )
{
    uint16              img_width = 0;
    uint16              img_height = 0;
    GUI_RECT_T          anim_rect = {0};
    GUIANIM_RESULT_E    result = GUIANIM_RESULT_FAIL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(!anim_ctrl_ptr->display_info.is_zoom); /*assert verified*/
    if (anim_ctrl_ptr->display_info.is_zoom)
    {
        return GUIANIM_RESULT_FAIL;
    }
   
    //get image type,width and height
    result = GetImageInfoByRes(anim_ctrl_ptr);
    
    if (GUIANIM_RESULT_SUCC == result)
    {
        //get anim rect,except border
        if (GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect))
        {
            GUIANIM_GetImgWidthAndHeight(FALSE,FALSE,&img_width,&img_height,anim_ctrl_ptr);

            //adjust display position
            anim_ctrl_ptr->display_x = anim_rect.left;
            anim_ctrl_ptr->display_y = anim_rect.top;
            CTRLANIM_AdjustDisplayPosition(&anim_ctrl_ptr->display_x,
                &anim_ctrl_ptr->display_y,
                img_width,
                img_height,
                anim_rect,
                anim_ctrl_ptr->display_info.align_style);

            //set adjust display pos flag
            anim_ctrl_ptr->is_adjust_disp_pos = TRUE;
        }

        //set image has decode
        anim_ctrl_ptr->is_decoded = TRUE;

        //set image is support
        anim_ctrl_ptr->is_support = TRUE;
    }
    else
    {
        //set image has decode
        anim_ctrl_ptr->is_decoded = FALSE;
        
        //set image is support
        anim_ctrl_ptr->is_support = FALSE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get image type,width and height by resource
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E GetImageInfoByRes(
                                         CTRLANIM_OBJ_T *anim_ctrl_ptr  //in/out:
                                         )
{
    GUIANIM_RESULT_E    result = GUIANIM_RESULT_FAIL;
    GUIRES_IMG_SRC_T    img_src = {0};
    GUIRES_IMG_INFO_T   img_info = {0};

    if ((0 != anim_ctrl_ptr->data_info.anim_id) || 
        (0 != anim_ctrl_ptr->data_info.img_id))
    {
        img_src.data_ptr  = anim_ctrl_ptr->src_data_buffer;
        img_src.data_size = anim_ctrl_ptr->src_data_size;

        //get image info
        if (GUIRES_GetImgInfo(&img_src,&img_info))
        {
            //set image type
            anim_ctrl_ptr->type = img_info.img_type;

            //set total frame and anim type
            if (0 != anim_ctrl_ptr->data_info.anim_id)
            {
                anim_ctrl_ptr->total_frame = img_info.frame_num;

                if (GUIANIM_TYPE_ANIM_ARGB != anim_ctrl_ptr->type)
                {
                    anim_ctrl_ptr->type = GUIANIM_TYPE_ANIM;
                }
            }

            //get image width,height
            anim_ctrl_ptr->src_width  = img_info.width;
            anim_ctrl_ptr->src_height = img_info.height;
            anim_ctrl_ptr->img_width  = img_info.width;
            anim_ctrl_ptr->img_height = img_info.height;

            result = GUIANIM_RESULT_SUCC;
        }
        else
        {
            result = GUIANIM_RESULT_NO_SUPPORT;
        }
    }
    else
    {
        SCI_TRACE_LOW("GUIANIM:DecodeAnimOrImgRes:is not resource!");
    }

    return (result);
}

/*****************************************************************************/
//  Description : get miniature file name
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetMiniaturFileName(
                               CTRLANIM_OBJ_T   *anim_ctrl_ptr  //in
                               )
{
    //not decode miniature
    anim_ctrl_ptr->is_exist_miniature = FALSE;

    if ((GUIANIM_TYPE_IMAGE_JPG == anim_ctrl_ptr->type) && 
        (0 < anim_ctrl_ptr->file_info.full_path_wstr_len) &&
        (!anim_ctrl_ptr->is_wallpaper_ctrl) &&
        (!anim_ctrl_ptr->display_info.is_zoom) &&
        (anim_ctrl_ptr->theme.min_resolution <= (uint32)(((anim_ctrl_ptr->img_width)*(anim_ctrl_ptr->img_height)))))
    {
        //set and get miniature full path
        if (SetMiniatureFullPath(anim_ctrl_ptr->file_info.full_path_wstr_ptr,
                anim_ctrl_ptr->file_info.full_path_wstr_len,
                anim_ctrl_ptr->miniature_width,
                anim_ctrl_ptr->miniature_height,
                &anim_ctrl_ptr->miniature_path))
        {
            //judge it is exist
            //if (anim_ctrl_ptr->theme.IsFileExist(anim_ctrl_ptr->miniature_path.full_path_wstr,anim_ctrl_ptr->miniature_path.full_path_wstr_len))
            //{
                //judge it is corresponding to the original pic
                if (IsTwoFileCorresponding(anim_ctrl_ptr->file_info.full_path_wstr_ptr,
                        anim_ctrl_ptr->file_info.full_path_wstr_len,
                        anim_ctrl_ptr->miniature_path.full_path_wstr))
                {
                    //exist miniature
                    anim_ctrl_ptr->is_exist_miniature = TRUE;
                }
                //else
                //{
                    //delete the miniature file
                //  anim_ctrl_ptr->theme.DeleteFileSyn(anim_ctrl_ptr->miniature_path.full_path_wstr,
                //  anim_ctrl_ptr->miniature_path.full_path_wstr_len);
                //}
            //}

            //set file folder _slt hide
            //SetMiniatureFolderHide(anim_ctrl_ptr);
        }
        else
        {
            SCI_TRACE_LOW("GUIANIM:GetMiniaturFileName: miniature name len is too long!");
        }
    }
}

/*****************************************************************************/
//  Description : get the miniature name
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetMiniatureFullPath(
                                   const wchar                      *full_path_ptr,     //in
                                   uint16                           full_path_len,      //in
                                   uint16                           miniature_width,    //in
                                   uint16                           miniature_height,   //in
                                   GUIANIM_MINIATURE_FULL_PATH_T    *miniature_path_ptr //in/out
                                   )
{
    BOOLEAN     result = FALSE;
    wchar       *miniature_full_path_ptr = PNULL;
    uint16      miniature_full_path_len = 0;
    int32       i = 0;
    uint8       suffix_str[GUIANIM_MINIATURE_SUFFIX_LEN] = {0};
    wchar       suffix_wstr[GUIANIM_MINIATURE_SUFFIX_LEN] = {0};
    uint16      suffix_len = 0;

    SCI_MEMSET(miniature_path_ptr,0,sizeof(GUIANIM_MINIATURE_FULL_PATH_T));

    //get suffix
    suffix_len = (uint16)sprintf((char*)suffix_str, "_%dx%d",miniature_width,miniature_height);

    //convert wchar
    MMI_STRNTOWSTR(suffix_wstr,GUIANIM_MINIATURE_SUFFIX_LEN,
        (const uint8 *)suffix_str,GUIANIM_MINIATURE_SUFFIX_LEN,
        suffix_len);

    if ((full_path_len + GUIANIM_MINIATURE_PATH_LEN + suffix_len) < (GUIANIM_MINIATURE_FULL_PATH_LEN + 1))
    {
        //init miniature full path
        miniature_full_path_ptr = miniature_path_ptr->full_path_wstr;

        //find the last '\'
        for (i = (int32)(full_path_len-1); i >= 0; i--)
        {
            if (GUIANIM_FILE_SLASH == full_path_ptr[i])
            {
                break;
            }
        }
        
        if (0 > i)
        {
            i = 0;
        }

        //init miniature full path
        SCI_MEMSET(miniature_full_path_ptr,0,GUIANIM_MINIATURE_FULL_PATH_LEN);

        //copy path
        miniature_full_path_len = (uint16)(i);
        MMI_WSTRNCPY(miniature_full_path_ptr,GUIANIM_MINIATURE_FULL_PATH_LEN,
            full_path_ptr,miniature_full_path_len,
            miniature_full_path_len);

        //add path\_Slt
        MMI_WSTRNCPY((miniature_full_path_ptr+miniature_full_path_len),(GUIANIM_MINIATURE_FULL_PATH_LEN-miniature_full_path_len),
            GUIANIM_MINIATURE_PATH,GUIANIM_MINIATURE_PATH_LEN,
            GUIANIM_MINIATURE_PATH_LEN);
        miniature_full_path_len += GUIANIM_MINIATURE_PATH_LEN;
    
        //copy file name
        MMI_WSTRNCPY((miniature_full_path_ptr+miniature_full_path_len),(GUIANIM_MINIATURE_FULL_PATH_LEN-miniature_full_path_len),
            (full_path_ptr+i),(full_path_len-i),
            (full_path_len-i));
        miniature_full_path_len = (uint16)(miniature_full_path_len + full_path_len - i);
    
        //add file name suffix
        MMI_WSTRNCPY((miniature_full_path_ptr+miniature_full_path_len),(GUIANIM_MINIATURE_FULL_PATH_LEN-miniature_full_path_len),
            suffix_wstr,suffix_len,
            suffix_len);
        miniature_full_path_len = (uint16)(miniature_full_path_len + suffix_len);

        //return result
        miniature_path_ptr->full_path_wstr_len = miniature_full_path_len;
        
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : the miniature is corresponding to the main picture
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsTwoFileCorresponding(
                                     wchar      *full_path_ptr,         //in
                                     uint16     full_path_len,          //in
                                     wchar      *miniature_full_path_ptr//in
                                     )
{
    BOOLEAN             result = FALSE;
    uint8               size_time[GUIANIM_MINIATURE_SIZE_TIME_LEN+2+1] = {0};
    uint32              size_time_len = 0;
    uint32              i = 0;
    uint32              miniature_size = 0;
    uint32              miniature_offset = 0;
    uint32              src_file_size = 0;
    SFS_HANDLE          file_handle = 0;
    SFS_DATE_T          src_creat_date = {0};
    SFS_TIME_T          src_creat_time = {0};
    SFS_FIND_DATA_T     find_data = {0};

    //open miniature file and get file size
    file_handle = SFS_CreateFile(miniature_full_path_ptr,SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,PNULL,PNULL);/*lint !e655*/

    if (0 != file_handle)
    {
        if (SFS_NO_ERROR == SFS_GetFileSize(file_handle,&miniature_size))
        {
            //read miniature file last GUIANIM_MINIATURE_SIZE_TIME_LEN byte
            miniature_offset = miniature_size - GUIANIM_MINIATURE_SIZE_TIME_LEN - 2;
            if(SFS_NO_ERROR == SFS_SetFilePointer(file_handle,(__int64)miniature_offset,SFS_SEEK_BEGIN))
            {
                if(SFS_ERROR_NONE == SFS_ReadFile(file_handle,size_time,(GUIANIM_MINIATURE_SIZE_TIME_LEN+2),&size_time_len,PNULL))
                {
                    if (((GUIANIM_MINIATURE_SIZE_TIME_LEN+2) == size_time_len) &&
                        (0xFF == size_time[i++]) &&
                        (0xD9 == size_time[i++]))
                    {
                        //get src file size and creat time
                        src_file_size = (size_time[i++] << 24) & 0xFF000000;
                        src_file_size += ((size_time[i++] << 16) & 0x00FF0000);
                        src_file_size += ((size_time[i++] << 8) & 0x0000FF00);
                        src_file_size += (size_time[i++] & 0x000000FF);

                        //get src file creat time year
                        src_creat_date.year = (uint16)((size_time[i++] << 8) & 0xFF00);
                        src_creat_date.year = (uint16)(src_creat_date.year + (size_time[i++] & 0x00FF));

                        //get src file creat time month
                        src_creat_date.mon = size_time[i++];

                        //get src file creat time day
                        src_creat_date.mday = size_time[i++];

                        //get src file creat time hour
                        src_creat_time.hour = size_time[i++];

                        //get src file creat time minute
                        src_creat_time.min = size_time[i++];

                        //get src file creat time second
                        src_creat_time.sec = size_time[i++];
                        SCI_ASSERT(i == (GUIANIM_MINIATURE_SIZE_TIME_LEN+2)); /*assert verified*/

                        result = TRUE;
                    }
                    else
                    {
                        SCI_TRACE_LOW("GUIANIM:IsTwoFileCorresponding: get miniature file size_time_len %d is error!", size_time_len);
                    }
                }
            }
        }
    }

    //close miniature file
    SFS_CloseFile(file_handle);

    if (result)
    {
        //get src file size and creat time
        if (GetFileSizeAndCreatTime(full_path_ptr,full_path_len,&find_data))
        {
            //compare two file is corresponding
            if ((src_file_size == find_data.length) &&
                (src_creat_date.year == find_data.create_Date.year) && 
                (src_creat_date.mon == find_data.create_Date.mon) && 
                (src_creat_date.mday == find_data.create_Date.mday) && 
                (src_creat_time.hour == find_data.create_time.hour) && 
                (src_creat_time.min == find_data.create_time.min) && 
                (src_creat_time.sec == find_data.create_time.sec))
            {
                result = TRUE;
            }
            else
            {
                result = FALSE;
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : set miniature file folder hide
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetMiniatureFolderHide(
                                  CTRLANIM_OBJ_T    *anim_ctrl_ptr
                                  )
{
    int16       i = 0;
    uint16      path_len = 0;
    wchar       *path_ptr = PNULL;

    //alloc memory
    path_len = anim_ctrl_ptr->miniature_path.full_path_wstr_len;
    path_ptr = SCI_ALLOC_APP(path_len * sizeof(wchar));
    SCI_MEMSET(path_ptr,0,(path_len * sizeof(wchar)));

    //find the last '\'
    for (i=(int16)(path_len-1); i>=0;i--)
    {
        if (GUIANIM_FILE_SLASH == anim_ctrl_ptr->miniature_path.full_path_wstr[i])
        {
            break;
        }
    }

    if (0 < i)
    {
        //set path len
        path_len = i;

        //get _slt path
        MMI_WSTRNCPY(path_ptr,anim_ctrl_ptr->miniature_path.full_path_wstr_len,
            anim_ctrl_ptr->miniature_path.full_path_wstr,anim_ctrl_ptr->miniature_path.full_path_wstr_len,
            path_len);

        //set hide
        anim_ctrl_ptr->theme.SetFileAttr(path_ptr,path_len,TRUE,TRUE,FALSE,FALSE);
    }

    SCI_FREE(path_ptr);
}

/*****************************************************************************/
//  Description : post anim display indication msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PostDisplayIndMsg(
                             GUIANIM_RESULT_E   result,         //in
                             CTRLANIM_OBJ_T     *anim_ctrl_ptr  //in
                             )
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    if ((PNULL != anim_ctrl_ptr) && 
        (!anim_ctrl_ptr->is_send_disp_ind))
    {
        GUIANIM_DISPLAY_IND_T   anim_display_ind = {0};

        anim_display_ind.result      = result;
        anim_display_ind.ctrl_handle = base_ctrl_ptr->handle;

        GUICTRL_PostNotifyEx(base_ctrl_ptr->handle,MSG_NOTIFY_ANIM_DISPLAY_IND, 
            &anim_display_ind,sizeof(GUIANIM_DISPLAY_IND_T));

        anim_ctrl_ptr->is_send_disp_ind = TRUE;
    }
}

/*****************************************************************************/
//  Description : post gif or anim play once end indication msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIANIM_PostPlayOnceEndIndMsg(
                                          CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in
                                          )
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    if (anim_ctrl_ptr->display_info.is_need_end_ind)
    {
        GUICTRL_PostNotifyEx(base_ctrl_ptr->handle,MSG_NOTIFY_ANIM_END_IND,
            PNULL,0);
    }
}

/*****************************************************************************/
//  Description : 询问父亲是否显示，默认为显示
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QueryParantDisplayOrNot(
									  CTRLANIM_OBJ_T    *anim_ctrl_ptr
									  )
{
    BOOLEAN   is_display = TRUE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;
	
    if (PNULL != anim_ctrl_ptr)
    {
        //notify gif update
        GUICTRL_SendNotifyEx(base_ctrl_ptr->handle, MSG_NOTIFY_ANIM_DISPLAY, (DPARAM)&is_display);
    }
	else
	{
		is_display = FALSE;
	}

	return is_display;
}

/*****************************************************************************/
//  Description : handle anim get data confirm msg
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void HandleAnimGetDataCnf(
                                BOOLEAN                 is_display,     //in:
                                CTRLANIM_OBJ_T          *anim_ctrl_ptr, //in
                                GUIANIM_DEC_RESULT_T    *dec_result_ptr //in
                                )
{
    BOOLEAN                 is_encode_fail = FALSE;
    GUIANIM_RESULT_E        dec_result = GUIANIM_RESULT_SUCC;
    GUIANIM_SII_RESULT_E    result = GUIANIM_SII_SUCC;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != dec_result_ptr); /*assert verified*/
    if (PNULL == dec_result_ptr)
    {
        return;
    }

    SCI_TRACE_LOW("GUIANIM:HandleAnimGetDataCnf:get data cnf,is_display=%d,handle=0x%x,result=%d,is_encode=%d,thumb=%d,frame=%d,end=%d!",
                  is_display,
                  anim_ctrl_ptr->img_handle,
                  dec_result_ptr->result,
                  anim_ctrl_ptr->is_encode_jpg,
                  anim_ctrl_ptr->is_decode_thumb,
                  dec_result_ptr->cur_frame_index,
                  dec_result_ptr->is_decode_gif_end);

    //destroy image handle
    DestroyImgHandle(anim_ctrl_ptr,dec_result_ptr);

    //set image info by decode/encode result
    result = SetImageInfo(anim_ctrl_ptr,dec_result_ptr);
    switch (result)
    {
    case GUIANIM_SII_SUCC:
    case GUIANIM_SII_FAIL:
        //display
        if (is_display && QueryParantDisplayOrNot(anim_ctrl_ptr))
        {
            GUIANIM_Display(FALSE,FALSE,anim_ctrl_ptr);
        }

        //gif handle get data confirm msg
        HandleGifGetDataCnf(is_display,anim_ctrl_ptr,dec_result_ptr);

        //post anim display indication msg
        PostDisplayIndMsg(dec_result_ptr->result,anim_ctrl_ptr);
        break;

    case GUIANIM_SII_JPG_ENCODE_SUCC:
    case GUIANIM_SII_JPG_ENCODE_FAIL:
        //set is encode fail
        if (GUIANIM_SII_JPG_ENCODE_FAIL == result)
        {
            is_encode_fail = TRUE;
        }

        //decode image file
        dec_result = DecodeImgBufOrFile(FALSE,FALSE,is_encode_fail,anim_ctrl_ptr);
        if (GUIANIM_RESULT_WAIT != dec_result)
        {
            //post anim display indication msg
            PostDisplayIndMsg(dec_result,anim_ctrl_ptr);

            //set image is not support
            anim_ctrl_ptr->is_support = FALSE;
        }
        break;

    case GUIANIM_SII_DECODE_THUMB_FAIL:
        //decode image file
        dec_result = DecodeImgBufOrFile(FALSE,TRUE,FALSE,anim_ctrl_ptr);
        if (GUIANIM_RESULT_WAIT != dec_result)
        {
            //post anim display indication msg
            PostDisplayIndMsg(dec_result,anim_ctrl_ptr);

            //set image is not support
            anim_ctrl_ptr->is_support = FALSE;
        }
        break;

    case GUIANIM_SII_DECODE_MINIATURE_FAIL:
        //decode image file
        dec_result = DecodeImgBufOrFile(FALSE,FALSE,TRUE,anim_ctrl_ptr);
        if (GUIANIM_RESULT_WAIT != dec_result)
        {
            //post anim display indication msg
            PostDisplayIndMsg(dec_result,anim_ctrl_ptr);

            //set image is not support
            anim_ctrl_ptr->is_support = FALSE;
        }
        break;

    default:
        SCI_PASSERT(FALSE,("HandleAnimGetDataCnf: set image info result %d is error!",result)); /*assert verified*/
        break;
    }
}

/*****************************************************************************/
//  Description : set image info by decode/encode result
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL GUIANIM_SII_RESULT_E SetImageInfo(
                                        CTRLANIM_OBJ_T          *anim_ctrl_ptr, //in
                                        GUIANIM_DEC_RESULT_T    *dec_result_ptr //in
                                        )
{
    uint16                  img_width = 0;
    uint16                  img_height = 0;
    GUI_RECT_T              anim_rect = {0};
    GUIANIM_SII_RESULT_E    result = GUIANIM_SII_SUCC;
    
    if (GUIANIM_RESULT_SUCC == dec_result_ptr->result)
    {
        //set decode thumbnail false
        anim_ctrl_ptr->is_decode_thumb = FALSE;

        if ((GUIANIM_TYPE_IMAGE_JPG == anim_ctrl_ptr->type) && 
            (anim_ctrl_ptr->is_encode_jpg))
        {
            anim_ctrl_ptr->is_encode_jpg = FALSE;
            if (CreatJpgMiniature(dec_result_ptr->img_size,anim_ctrl_ptr))
            {
                result = GUIANIM_SII_JPG_ENCODE_SUCC;

                //exist miniature
                anim_ctrl_ptr->is_exist_miniature = TRUE;
            }
            else
            {
                result = GUIANIM_SII_JPG_ENCODE_FAIL;
            }
        }
        else
        {
            //set image width and height
            anim_ctrl_ptr->img_width  = dec_result_ptr->img_width;
            anim_ctrl_ptr->img_height = dec_result_ptr->img_height;

            //set delay time and current frame index
            if (GUIANIM_TYPE_IMAGE_GIF == anim_ctrl_ptr->type)
            {
                if (PNULL != anim_ctrl_ptr->target_backup_buf)
                {
                    //copy backup target buffer
                    MMI_MEMCPY(anim_ctrl_ptr->target_buffer + anim_ctrl_ptr->target_buf_offset,
                        anim_ctrl_ptr->target_buffer_size,
                        anim_ctrl_ptr->target_backup_buf + anim_ctrl_ptr->target_backup_offset,
                        anim_ctrl_ptr->target_buffer_size,
                        anim_ctrl_ptr->target_buffer_size);
                }

                anim_ctrl_ptr->delay_time = dec_result_ptr->delay_time;
                SCI_ASSERT(0 < dec_result_ptr->cur_frame_index); /*assert verified*/
                anim_ctrl_ptr->cur_frame = (uint16)(dec_result_ptr->cur_frame_index - 1);
            }

            //display frame
            GUIANIM_DisplayFrame(anim_ctrl_ptr);

#if defined (GUIF_ANIM_ZOOM) && defined (GUIF_ANIM_ROTATE)
            //auto zoom or rotate image to display in anim rect
            if (GUIANIM_AutoZoom(anim_ctrl_ptr) &&
                GUIANIM_AutoRotate(anim_ctrl_ptr))
            {
                if (!anim_ctrl_ptr->is_adjust_disp_pos)
                {
                    //get anim rect,except border
                    GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect);

                    GUIANIM_GetImgWidthAndHeight(FALSE,FALSE,&img_width,&img_height,anim_ctrl_ptr);

                    //adjust display position
                    CTRLANIM_AdjustDisplayPosition(&anim_ctrl_ptr->display_x,
                        &anim_ctrl_ptr->display_y,
                        img_width,
                        img_height,
                        anim_rect,
                        anim_ctrl_ptr->display_info.align_style);

                    //set adjust display pos flag
                    anim_ctrl_ptr->is_adjust_disp_pos = TRUE;
                }

                //set image has decode
                anim_ctrl_ptr->is_decoded = TRUE;

                //set image is support
                anim_ctrl_ptr->is_support = TRUE;

                //save wallpaper bitmap
                SaveWallpaperBitmap(anim_ctrl_ptr);

                //free wallpaper buffer
                if (!MMK_IsFocusWin(anim_ctrl_ptr->win_handle))
                {
                    FreeWallpaperTargetBuffer(anim_ctrl_ptr);
                }
            }
            else
#endif  //end of GUIF_ANIM_ZOOM && GUIF_ANIM_ROTATE
            {
                //set image info by fail
                result = SetImageInfoFail(anim_ctrl_ptr,dec_result_ptr);
            }
        }
    }
    else
    {
        //set image info by fail
        result = SetImageInfoFail(anim_ctrl_ptr,dec_result_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : set image info by decode/encode fail result
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL GUIANIM_SII_RESULT_E SetImageInfoFail(
                                            CTRLANIM_OBJ_T          *anim_ctrl_ptr, //in
                                            GUIANIM_DEC_RESULT_T    *dec_result_ptr //in
                                            )
{
    GUIANIM_SII_RESULT_E    result = GUIANIM_SII_FAIL;

    //free backup target buffer
    if (PNULL != anim_ctrl_ptr->target_backup_buf)
    {
        SCI_FREE(anim_ctrl_ptr->target_backup_buf);
        anim_ctrl_ptr->target_backup_offset = 0;
    }

    //gif multi frame,the last frame is error!
    if ((GUIANIM_TYPE_IMAGE_GIF == anim_ctrl_ptr->type) && 
        (1 < dec_result_ptr->cur_frame_index))
    {
        //not free target buffer,not set image not support
        SCI_TRACE_LOW("GUIANIM:SetImageInfoFail: multi frame gif error!");
    }
    else
    {
        //free target buffer
        FreeTargetBuffer(anim_ctrl_ptr);

        //jpg encode fail,need decode jpg directly
        if (GUIANIM_TYPE_IMAGE_JPG == anim_ctrl_ptr->type)
        {
            if (anim_ctrl_ptr->is_encode_jpg)
            {
                anim_ctrl_ptr->is_encode_jpg = FALSE;
                result = GUIANIM_SII_JPG_ENCODE_FAIL;
            }
            else if (anim_ctrl_ptr->is_decode_thumb)
            {
                anim_ctrl_ptr->is_decode_thumb = FALSE;
                result = GUIANIM_SII_DECODE_THUMB_FAIL;
            }
            else if (anim_ctrl_ptr->is_exist_miniature)
            {
                //delete the miniature file
                anim_ctrl_ptr->theme.DeleteFileSyn(anim_ctrl_ptr->miniature_path.full_path_wstr,
                    anim_ctrl_ptr->miniature_path.full_path_wstr_len);

                anim_ctrl_ptr->is_exist_miniature = FALSE;

                result = GUIANIM_SII_DECODE_MINIATURE_FAIL;
            }
            else
            {
                //set image has decode
                anim_ctrl_ptr->is_decoded = FALSE;

                //set image is not support
                anim_ctrl_ptr->is_support = FALSE;
            }
        }
        else
        {
            //set image has decode
            anim_ctrl_ptr->is_decoded = FALSE;

            //set image is not support
            anim_ctrl_ptr->is_support = FALSE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : creat miniature for jpg
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreatJpgMiniature(//创建缩略图是否成功
                                uint32          img_size,
                                CTRLANIM_OBJ_T  *anim_ctrl_ptr
                                )
{
    BOOLEAN     result = TRUE;

    //generate miniature file
    if (!GenerateMiniatureFile(anim_ctrl_ptr->target_buffer + anim_ctrl_ptr->target_buf_offset,
            img_size,
            anim_ctrl_ptr))
    {
        result = FALSE;
        SCI_TRACE_LOW("GUIANIM:CreatJpgMiniature: generate miniature file fail!");
    }

    //free target buffer
    FreeTargetBuffer(anim_ctrl_ptr);
    
    return (result);
}

/*****************************************************************************/
//  Description : generate miniature file
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GenerateMiniatureFile(
                                    uint8               *data_ptr,      //in
                                    uint32              data_size,      //in
                                    CTRLANIM_OBJ_T      *anim_ctrl_ptr  //in
                                    )
{
    BOOLEAN             result = FALSE;
    uint8               size_time[GUIANIM_MINIATURE_SIZE_TIME_LEN+1] = {0};
    uint16              size_time_len = 0;
    uint32              i = 0;
    uint32              writen_size = 0;
    SFS_HANDLE          file_handle = 0;
    SFS_FIND_DATA_T     find_data = {0};

    SCI_ASSERT(PNULL != data_ptr); /*assert verified*/

    //write miniature file
    file_handle = SFS_CreateFile(anim_ctrl_ptr->miniature_path.full_path_wstr,SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS,PNULL,PNULL);/*lint !e655*/

    if(0 != file_handle)
    {
        if(SFS_ERROR_NONE == SFS_WriteFile(file_handle,(const void *)data_ptr,data_size,&writen_size,PNULL))
        {
            //将原文件的size和creat time写入缩略图文件末尾
            if (GetFileSizeAndCreatTime(anim_ctrl_ptr->file_info.full_path_wstr_ptr,
                    anim_ctrl_ptr->file_info.full_path_wstr_len,
                    &find_data))
            {
                //set size
                size_time[i++] = (uint8)((find_data.length >> 24) & 0x000000FF);
                size_time[i++] = (uint8)((find_data.length >> 16) & 0x000000FF);
                size_time[i++] = (uint8)((find_data.length >> 8) &  0x000000FF);
                size_time[i++] = (uint8)(find_data.length &  0x000000FF);

                //set creat time year
                size_time[i++] = (uint8)((find_data.create_Date.year >> 8) & 0x00FF);
                size_time[i++] = (uint8)(find_data.create_Date.year & 0x00FF);

                //set creat time month
                size_time[i++] = find_data.create_Date.mon;

                //set creat time day
                size_time[i++] = find_data.create_Date.mday;

                //set creat time hour
                size_time[i++] = find_data.create_time.hour;

                //set creat time minute
                size_time[i++] = find_data.create_time.min;

                //set creat time second
                size_time[i++] = find_data.create_time.sec;

                SCI_ASSERT(i == GUIANIM_MINIATURE_SIZE_TIME_LEN); /*assert verified*/
                size_time_len = (uint16)(i);

                if(SFS_ERROR_NONE == SFS_WriteFile(file_handle,(const void *)size_time,size_time_len,&writen_size,PNULL))
                {
                    result = TRUE;
                }
                else
                {
                    SCI_TRACE_LOW("GUIANIM:GenerateMiniatureFile: write src file size and creat time error!");
                }
            }
            else
            {
                SCI_TRACE_LOW("GUIANIM:GenerateMiniatureFile: get file size and creat timer error!");
            }
        }
        else
        {
            SCI_TRACE_LOW("GUIANIM:GenerateMiniatureFile: write miniature file error!");
        }
    }

    SFS_CloseFile(file_handle);

    //set file folder _slt hide
    SetMiniatureFolderHide(anim_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : get file size and creat time
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetFileSizeAndCreatTime(
                                      wchar             *full_path_ptr,     //in
                                      uint16            full_path_len,      //in
                                      SFS_FIND_DATA_T   *find_data_ptr      //in/out
                                      )
{
    BOOLEAN             result = FALSE;
    SFS_HANDLE          file_handle = {0};

    if ((PNULL != full_path_ptr) && 
        (0 < full_path_len) && 
        (PNULL != find_data_ptr))
    {
        //get src file size and creat time
        file_handle = SFS_FindFirstFile(full_path_ptr,find_data_ptr);
        if (0 != file_handle)
        {
            result = TRUE;
        }
        else
        {
            SCI_TRACE_LOW("GUIANIM:GetFileSizeAndCreatTime:SFS_FindFirstFile file_handle %d is error!",file_handle);
        }

        //close file
        SFS_FindClose(file_handle);
    }

    return (result);
}

/*****************************************************************************/
//  Description : gif handle get data confirm msg
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void HandleGifGetDataCnf(
                               BOOLEAN                  is_display,     //in
                               CTRLANIM_OBJ_T           *anim_ctrl_ptr, //in
                               GUIANIM_DEC_RESULT_T     *dec_result_ptr //in
                               )
{
    if (GUIANIM_TYPE_IMAGE_GIF == anim_ctrl_ptr->type)
    {
        if ((is_display) &&
            ((anim_ctrl_ptr->is_wallpaper_ctrl) ||
             (anim_ctrl_ptr->is_send_disp_ind)))
        {
            //notify the window to update
            NotifyWinGifAnimUpdate(anim_ctrl_ptr);
        }

        //Gif是否解码完成
        if (dec_result_ptr->is_decode_gif_end)
        {
            //post play once end indication msg
            GUIANIM_PostPlayOnceEndIndMsg(anim_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : notify window gif anim update
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void NotifyWinGifAnimUpdate(
                                  CTRLANIM_OBJ_T    *anim_ctrl_ptr
                                  )
{
    GUIANIM_DISPLAY_IND_T   anim_display_ind = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    if (PNULL != anim_ctrl_ptr)
    {
        anim_display_ind.result      = GUIANIM_RESULT_SUCC;
        anim_display_ind.ctrl_handle = base_ctrl_ptr->handle;

        //notify gif update
        GUICTRL_SendNotifyEx(base_ctrl_ptr->handle,
            MSG_CTL_ANIM_GIF_UPDATE,
            &anim_display_ind);
    }
}

/*****************************************************************************/
//  Description : handle anim timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleAnimTimer(
                           CTRLANIM_OBJ_T   *anim_ctrl_ptr
                           )
{
    if (PNULL != anim_ctrl_ptr)
    {
        //stop anim timer
        GUIANIM_StopAnimTimer(&anim_ctrl_ptr->timer_id);

        //set current frame
        anim_ctrl_ptr->cur_frame++;
        
        if (0 == anim_ctrl_ptr->total_frame)
        {
            anim_ctrl_ptr->cur_frame = 0;
        }
        else
        {
            anim_ctrl_ptr->cur_frame = (uint16)(anim_ctrl_ptr->cur_frame % anim_ctrl_ptr->total_frame);
        }

        if (0 == anim_ctrl_ptr->cur_frame)
        {
            //post play once end indication msg
            GUIANIM_PostPlayOnceEndIndMsg(anim_ctrl_ptr);
        }

        //play once, play the last frame,don't continue play
        if ((anim_ctrl_ptr->display_info.is_play_once) && 
            (0 == anim_ctrl_ptr->cur_frame))
        {
            SCI_TRACE_LOW("GUIANIM:HandleAnimTimer:only play anim once!");
        }
        else
        {
            if (anim_ctrl_ptr->is_focus)
            {
                //display anim
                GUIANIM_Display(FALSE,FALSE,anim_ctrl_ptr);
            }
        }
    }
}

/*****************************************************************************/
//  Description : handle gif timer
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void HandleGifTimer(
                          CTRLANIM_OBJ_T    *anim_ctrl_ptr
                          )
{
    if (PNULL != anim_ctrl_ptr)
    {
        //stop gif timer
        GUIANIM_StopGifTimer(&anim_ctrl_ptr->gif_timer_id);

        //当前帧是否已经显示
        if (anim_ctrl_ptr->is_focus)
        {
            if (0 == anim_ctrl_ptr->data_info.img_id)
            {
                //decode next gif
                DecodeImgBufOrFile(FALSE,FALSE,FALSE,anim_ctrl_ptr);

                //synchronous decode
                if (anim_ctrl_ptr->display_info.is_syn_decode)
                {
                    //display
                    GUIANIM_Display(FALSE,FALSE,anim_ctrl_ptr);

                    //notify the window to update
                    NotifyWinGifAnimUpdate(anim_ctrl_ptr);
                }
            }
            else
            {
                //display gif next frame
                anim_ctrl_ptr->is_next_frame = TRUE;
                GUIANIM_Display(FALSE,FALSE,anim_ctrl_ptr);
    
                //notify the window to update
                NotifyWinGifAnimUpdate(anim_ctrl_ptr);
            }
        }
    }
}

/*****************************************************************************/
//  Description : handle anim get focus message
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void HandleAnimGetFocus(
                              CTRLANIM_OBJ_T    *anim_ctrl_ptr
                              )
{
    BOOLEAN     is_decoded = FALSE;

    anim_ctrl_ptr->is_focus = TRUE;
        
    //reload resource
    if (LoadAnimOrImgRes(anim_ctrl_ptr))
    {
        //设置墙纸时角度和显示不一样时,需要重新解析
        if ((anim_ctrl_ptr->is_wallpaper_ctrl) &&
            (PNULL == anim_ctrl_ptr->target_buffer))
        {
            DecodeAnimOrImgRes(anim_ctrl_ptr);
        }
    }

    //not decode image,file or data buffer
    if ((0 == anim_ctrl_ptr->img_handle) &&
        ((PNULL != anim_ctrl_ptr->data_info.data_ptr) || (PNULL != anim_ctrl_ptr->file_info.full_path_wstr_ptr)))
    {
        //is decoded
        is_decoded = anim_ctrl_ptr->is_decoded;

        //multi frame gif,need decode
        if ((GUIANIM_TYPE_IMAGE_GIF == anim_ctrl_ptr->type) && 
            (1 != anim_ctrl_ptr->total_frame))
        {
            is_decoded = FALSE;
            anim_ctrl_ptr->is_decoded = FALSE;
        }

        //reset decode
        if ((!is_decoded) && 
            (anim_ctrl_ptr->is_support))
        {
            //reset not send display indication
            anim_ctrl_ptr->is_send_disp_ind = FALSE;

            //free target buffer
            if (anim_ctrl_ptr->is_wallpaper_ctrl)
            {
                FreeWallpaperTargetBuffer(anim_ctrl_ptr);
            }
            else
            {
                FreeTargetBuffer(anim_ctrl_ptr);
            }

            //decode
            DecodeImgBufOrFile(FALSE,FALSE,FALSE,anim_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : handle anim lose focus message
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void HandleAnimLoseFocus(
                               CTRLANIM_OBJ_T   *anim_ctrl_ptr
                               )
{
    BOOLEAN     is_backlight_on = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    anim_ctrl_ptr->is_focus = FALSE;

    //stop anim timer
    GUIANIM_StopAnimTimer(&anim_ctrl_ptr->timer_id);

    //stop gif timer
    GUIANIM_StopGifTimer(&anim_ctrl_ptr->gif_timer_id);

    //free bg buffer
    if (PNULL != anim_ctrl_ptr->bg_buf_ptr)
    {
        SCI_FREE(anim_ctrl_ptr->bg_buf_ptr);
    }

    if (0 != anim_ctrl_ptr->res_gif_handle)
    {
        GUIRES_FreeHandle(anim_ctrl_ptr->res_gif_handle);
        anim_ctrl_ptr->res_gif_handle = 0;

        anim_ctrl_ptr->is_dec_all_gif = FALSE;
    }

    //lose focus
    if (anim_ctrl_ptr->display_info.is_free_buf_lose_focus)
    {
        is_backlight_on = TRUE;

        if (0 != anim_ctrl_ptr->img_handle)
        {
            GUIANIM_DestroyHandle(TRUE,
                                anim_ctrl_ptr->img_handle,
                                base_ctrl_ptr->handle);
            anim_ctrl_ptr->img_handle = 0;
        }

        //free target buffer
        FreeTargetBuffer(anim_ctrl_ptr);

        //free backup target buffer
        if (PNULL != anim_ctrl_ptr->target_backup_buf)
        {
            SCI_FREE(anim_ctrl_ptr->target_backup_buf);
            anim_ctrl_ptr->target_backup_offset = 0;
        }

        anim_ctrl_ptr->is_decoded = FALSE;
    }
    else
    {
        is_backlight_on = MMITHEME_IsBacklightOn();
    }
    
    //非灭屏消息导致lose focus
    if ((is_backlight_on) && 
        (0 != anim_ctrl_ptr->img_handle))
    {
        //destroy image handle
        GUIANIM_DestroyHandle(TRUE,
            anim_ctrl_ptr->img_handle,
            base_ctrl_ptr->handle);
        anim_ctrl_ptr->img_handle = 0;

        //gif not free target buffer,may full paint display
        if (GUIANIM_TYPE_IMAGE_GIF != anim_ctrl_ptr->type)
        {
            //free target buffer
            FreeTargetBuffer(anim_ctrl_ptr);
        }

        //free backup target buffer
        if (PNULL != anim_ctrl_ptr->target_backup_buf)
        {
            SCI_FREE(anim_ctrl_ptr->target_backup_buf);
            anim_ctrl_ptr->target_backup_offset = 0;
        }
    }
    //free wallpaper buffer
	if ((anim_ctrl_ptr->is_wallpaper_ctrl) && (!anim_ctrl_ptr->display_info.is_save_wallpaper))
	{			
		//destroy image handle
        if (0 != anim_ctrl_ptr->img_handle)
        {
			GUIANIM_DestroyHandle(TRUE,
				anim_ctrl_ptr->img_handle,
				base_ctrl_ptr->handle);
			anim_ctrl_ptr->img_handle = 0;
		}
		FreeWallpaperTargetBuffer(anim_ctrl_ptr);
		//free backup target buffer
        if (PNULL != anim_ctrl_ptr->target_backup_buf)
        {
            SCI_FREE(anim_ctrl_ptr->target_backup_buf);
            anim_ctrl_ptr->target_backup_offset = 0;
		}		
	}     
}

/*****************************************************************************/
//  Description : get image width and height by file path
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetImgWidthHeight(
                                         uint16      *width_ptr,     //in/out:may PNULL
                                         uint16      *height_ptr,    //in/out:may PNULL
                                         wchar       *file_path_ptr, //in:
                                         uint16      file_len        //in:
                                         )
{
    BOOLEAN             result = FALSE;
    GUIANIM_RESULT_E    info_result = GUIANIM_RESULT_SUCC;
    GUIANIM_IMG_INFO_T  img_info = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(0 < file_len); /*assert verified*/
    if (0 == file_len)
    {
        return result;
    }

    //get image info
    info_result = GUIANIM_GetInfo(PNULL,file_path_ptr,PNULL,&img_info);
    if (GUIANIM_RESULT_SUCC == info_result)
    {
        if (PNULL != width_ptr)
        {
            *width_ptr = img_info.img_width;
        }
        if (PNULL != height_ptr)
        {
            *height_ptr = img_info.img_height;
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get image width and height by ctrl handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetImgWidthHeightEx(
                                           MMI_HANDLE_T     ctrl_handle,//in:
                                           uint16           *width_ptr, //in/out:may PNULL
                                           uint16           *height_ptr //in/out:may PNULL
                                           )
{
    BOOLEAN         result = FALSE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        //get current image width and height
        if ((anim_ctrl_ptr->display_info.is_zoom) &&
            (anim_ctrl_ptr->is_support_zoom))
        {
            if (PNULL != width_ptr)
            {
                *width_ptr  = anim_ctrl_ptr->zoom_dest.cur_img_width;
            }
            if (PNULL != height_ptr)
            {
                *height_ptr = anim_ctrl_ptr->zoom_dest.cur_img_height;
            }
        }
        else if (anim_ctrl_ptr->is_rotate)
        {
            if (PNULL != width_ptr)
            {
                *width_ptr  = anim_ctrl_ptr->rotate_dest.img_width;
            }
            if (PNULL != height_ptr)
            {
                *height_ptr = anim_ctrl_ptr->rotate_dest.img_height;
            }
        }
        else
        {
            if (PNULL != width_ptr)
            {
                *width_ptr  = anim_ctrl_ptr->img_width;
            }
            if (PNULL != height_ptr)
            {
                *height_ptr = anim_ctrl_ptr->img_height;
            }
        }

        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get image display point
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetDisplayPoint(
                                       MMI_HANDLE_T ctrl_handle,    //in:
                                       int16        *display_x_ptr, //in/out:
                                       int16        *display_y_ptr  //in/out:
                                       )
{
    BOOLEAN         result = FALSE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        if (PNULL != display_x_ptr)
        {
            *display_x_ptr = anim_ctrl_ptr->display_x;
        }

        if (PNULL != display_y_ptr)
        {
            *display_y_ptr = anim_ctrl_ptr->display_y;
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get anim type
//  Global resource dependence : 
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
PUBLIC GUIANIM_TYPE_E CTRLANIM_GetAnimType(
                                MMI_HANDLE_T    ctrl_handle
                                )
{
    GUIANIM_TYPE_E         anim_type = GUIANIM_TYPE_NONE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        anim_type = anim_ctrl_ptr->type;
    }

    return (anim_type);
}

/*****************************************************************************/
//  Description : play anim
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_PlayAnim(
                                MMI_HANDLE_T    ctrl_handle
                                )
{
    BOOLEAN         result = FALSE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        //play
        result = CTRLANIM_PlayAnimByPtr(anim_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : play anim by pointer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_PlayAnimByPtr(
                                     void   *ctrl_ptr
                                     )
{
    BOOLEAN         result = FALSE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = ctrl_ptr;

    if ((PNULL != anim_ctrl_ptr) &&
        (GUIANIM_TypeOf((CTRLBASE_OBJ_T *)anim_ctrl_ptr)))
    {
        //set not pause
        anim_ctrl_ptr->is_pause = FALSE;

        //display
        result = GUIANIM_Display(FALSE,FALSE,anim_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : pause anim
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLANIM_PauseAnim(
                              MMI_HANDLE_T ctrl_handle
                              )
{
    CTRLANIM_OBJ_T   *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        anim_ctrl_ptr->is_pause = TRUE;

        if (GUIANIM_TYPE_IMAGE_GIF == anim_ctrl_ptr->type)
        {
            //stop gif timer
            GUIANIM_StopGifTimer(&anim_ctrl_ptr->gif_timer_id);
        }
        else
        {
            //stop anim timer
            GUIANIM_StopAnimTimer(&anim_ctrl_ptr->timer_id);
        }
    }
}

/*****************************************************************************/
//  Description : resume anim
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLANIM_ResumeAnim(
                               MMI_HANDLE_T ctrl_handle
                               )
{
    CTRLANIM_OBJ_T   *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        anim_ctrl_ptr->is_pause = FALSE;

        if (anim_ctrl_ptr->is_focus)
        {
            if (GUIANIM_TYPE_IMAGE_GIF == anim_ctrl_ptr->type)
            {
                //start gif timer
                GUIANIM_StartGifTimer(anim_ctrl_ptr);
            }
            else
            {
                //start anim timer
                GUIANIM_StartAnimTimer(anim_ctrl_ptr);
            }
        }
    }
}

/*****************************************************************************/
//  Description : set anim control rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLANIM_SetCtrlRect(
                                MMI_HANDLE_T    ctrl_handle,
                                GUI_RECT_T      *rect_ptr
                                )
{
    CTRLANIM_OBJ_T   *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if ((PNULL != anim_ctrl_ptr) && 
        (PNULL != rect_ptr))
    {
        CTRLANIM_SetCtrlRectByPtr(anim_ctrl_ptr,rect_ptr);
    }
}

/*****************************************************************************/
//  Description : set anim control display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLANIM_SetCtrlRectByPtr(
                                     void           *ctrl_ptr,
                                     GUI_RECT_T     *rect_ptr
                                     )
{
    uint16          img_width = 0;
    uint16          img_height = 0;
    GUI_RECT_T      old_rect = {0};
    GUI_RECT_T      anim_rect = {0};
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = (CTRLANIM_OBJ_T*)ctrl_ptr;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)ctrl_ptr;

    if ((PNULL != anim_ctrl_ptr) &&
        (PNULL != rect_ptr) &&
        (GUIANIM_TypeOf((CTRLBASE_OBJ_T *)anim_ctrl_ptr)) &&
        (!GUI_EqualRect(base_ctrl_ptr->rect,*rect_ptr)))
    {   
        //set old rect
        old_rect = base_ctrl_ptr->rect;

        //set rect
        base_ctrl_ptr->rect         = *rect_ptr;
        base_ctrl_ptr->display_rect = *rect_ptr;
        
        GUI_SetVOrHRect( base_ctrl_ptr->handle, rect_ptr,&(base_ctrl_ptr->both_rect));

        //get miniature width and height
        GUILCD_GetLogicWidthHeight(
            base_ctrl_ptr->lcd_dev_info.lcd_id,
            &anim_ctrl_ptr->miniature_width,
            &anim_ctrl_ptr->miniature_height);

        //is need re-decode
        if (!RedecodeAnimCtrl(old_rect,anim_ctrl_ptr))
        {
            //get anim rect
            if (GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect))
            {
                //set display position
                anim_ctrl_ptr->display_x    = anim_rect.left;
                anim_ctrl_ptr->display_y    = anim_rect.top;
    
                //set adjust display pos flag
                anim_ctrl_ptr->is_adjust_disp_pos = FALSE;

                if (anim_ctrl_ptr->is_decoded)
                {
                    GUIANIM_GetImgWidthAndHeight(FALSE,FALSE,&img_width,&img_height,anim_ctrl_ptr);

                    //adjust display position
                    CTRLANIM_AdjustDisplayPosition(&anim_ctrl_ptr->display_x,
                        &anim_ctrl_ptr->display_y,
                        img_width,
                        img_height,
                        anim_rect,
                        anim_ctrl_ptr->display_info.align_style);

                    //set adjust display pos flag
                    anim_ctrl_ptr->is_adjust_disp_pos = TRUE;
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : re-decode anim control
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RedecodeAnimCtrl(//is re-decode
                               GUI_RECT_T       old_rect,       //in:
                               CTRLANIM_OBJ_T   *anim_ctrl_ptr  //in:
                               )
{
    BOOLEAN                 result = FALSE;
    BOOLEAN                 is_syn_decode = FALSE;
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_FILE_INFO_T     file_info = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    if (!GUI_IsRectSizeEqual(old_rect,base_ctrl_ptr->rect))
    {
        //set control info
        ctrl_info.ctrl_ptr          = anim_ctrl_ptr;
        ctrl_info.is_wallpaper_ctrl = anim_ctrl_ptr->is_wallpaper_ctrl;

        //is need re-decode
        if ((0 != anim_ctrl_ptr->data_info.img_id) || 
            (0 != anim_ctrl_ptr->data_info.anim_id) ||
            (PNULL != anim_ctrl_ptr->data_info.data_ptr) ||
            (anim_ctrl_ptr->data_info.is_bitmap))
        {
            //set data info
            data_info = anim_ctrl_ptr->data_info;

            if (anim_ctrl_ptr->data_info.is_save_data)
            {
                //save data buffer
                data_info.data_ptr = SCI_ALLOC(data_info.data_size);
                MMI_MEMCPY(data_info.data_ptr,data_info.data_size,
                    anim_ctrl_ptr->src_data_buffer,data_info.data_size,
                    data_info.data_size);
            }
            
            //lcd switch syn decode
            if (anim_ctrl_ptr->is_wallpaper_ctrl)
            {
                is_syn_decode = anim_ctrl_ptr->display_info.is_syn_decode;
                anim_ctrl_ptr->display_info.is_syn_decode = TRUE;
            }

            if ((data_info.is_bitmap) &&
                (data_info.is_free_bitmap) &&
                (PNULL != data_info.bitmap_data.bit_ptr) &&
                (data_info.bitmap_data.bit_ptr == anim_ctrl_ptr->target_buffer))
            {
                anim_ctrl_ptr->is_target_alloc = FALSE;
            }

            //set anim param
            CTRLANIM_SetParam(&ctrl_info,
                &data_info,
                PNULL,
                &anim_ctrl_ptr->display_info);

            if ((data_info.is_save_data) &&
                (PNULL != data_info.data_ptr))
            {
                SCI_FREE(data_info.data_ptr);
            }

            if (anim_ctrl_ptr->is_wallpaper_ctrl)
            {
                anim_ctrl_ptr->display_info.is_syn_decode = is_syn_decode;
            }

            result = TRUE;
        }
        else if (0 < anim_ctrl_ptr->file_info.full_path_wstr_len)
        {
            //set file info
            file_info.full_path_wstr_len = anim_ctrl_ptr->file_info.full_path_wstr_len;
            file_info.full_path_wstr_ptr = SCI_ALLOC_APP((file_info.full_path_wstr_len + 1) * sizeof(wchar));
            SCI_MEMSET(file_info.full_path_wstr_ptr,0,((file_info.full_path_wstr_len + 1) * sizeof(wchar)));
        
            MMI_WSTRNCPY(file_info.full_path_wstr_ptr,file_info.full_path_wstr_len + 1,
                anim_ctrl_ptr->file_info.full_path_wstr_ptr,file_info.full_path_wstr_len,
                file_info.full_path_wstr_len);

            //lcd switch syn decode
            if (anim_ctrl_ptr->is_wallpaper_ctrl)
            {
                is_syn_decode = anim_ctrl_ptr->display_info.is_syn_decode;
                anim_ctrl_ptr->display_info.is_syn_decode = TRUE;
            }

            //set anim param
            CTRLANIM_SetParam(&ctrl_info,
                PNULL,
                &file_info,
                &anim_ctrl_ptr->display_info);

            SCI_FREE(file_info.full_path_wstr_ptr);

            if (anim_ctrl_ptr->is_wallpaper_ctrl)
            {
                anim_ctrl_ptr->display_info.is_syn_decode = is_syn_decode;
            }

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get anim display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetDisplayRect(
                                      MMI_HANDLE_T  ctrl_handle,    //in:
                                      GUI_RECT_T    *rect_ptr       //in/out:
                                      )
{
    BOOLEAN         result = FALSE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if ((PNULL != anim_ctrl_ptr) &&
        (PNULL != rect_ptr))
    {
        result = VTLBASE_GetDisplayRect((CTRLBASE_OBJ_T*)anim_ctrl_ptr, rect_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : set anim display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 可以显示半幅图片
/*****************************************************************************/
PUBLIC void CTRLANIM_SetDisplayRect(
                                   MMI_HANDLE_T     ctrl_handle,
                                   GUI_RECT_T       *rect_ptr,
                                   BOOLEAN          is_update
                                   )
{
    CTRLANIM_OBJ_T   *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if ((PNULL != anim_ctrl_ptr) &&
        (PNULL != rect_ptr))
    {
        CTRLANIM_SetDisplayRectByPtr(anim_ctrl_ptr,rect_ptr,is_update);
    }
}

/*****************************************************************************/
//  Description : set anim display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 可以显示半幅图片
/*****************************************************************************/
PUBLIC void CTRLANIM_SetDisplayRectByPtr(
                                        void            *ctrl_ptr,
                                        GUI_RECT_T      *rect_ptr,
                                        BOOLEAN         is_update
                                        )
{
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = (CTRLANIM_OBJ_T*)ctrl_ptr;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)ctrl_ptr;

    if ((PNULL != anim_ctrl_ptr) &&
        (PNULL != rect_ptr) &&
        (GUIANIM_TypeOf((CTRLBASE_OBJ_T *)anim_ctrl_ptr)))
    {
        base_ctrl_ptr->display_rect = *rect_ptr;

        if (is_update)
        {
            //reload resource
            LoadAnimOrImgRes(anim_ctrl_ptr);

            //display
            GUIANIM_Display(FALSE,FALSE,anim_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : set anim background image
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetBgImg(
                                MMI_HANDLE_T    ctrl_handle,
                                MMI_IMAGE_ID_T  bg_img_id
                                )
{
    BOOLEAN         result = FALSE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        result = CTRLANIM_SetBgImgByPtr(anim_ctrl_ptr,bg_img_id);
    }

    return (result);
}

/*****************************************************************************/
//  Description : set anim background image
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetBgImgByPtr(
                                     void               *ctrl_ptr,
                                     MMI_IMAGE_ID_T     bg_img_id
                                     )
{
    BOOLEAN         result = FALSE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = ctrl_ptr;

    if ((PNULL != anim_ctrl_ptr) &&
        (GUIANIM_TypeOf((CTRLBASE_OBJ_T *)anim_ctrl_ptr)))
    {
        //set background image
        if (MMITHEME_CheckImageID(bg_img_id))
        {
            anim_ctrl_ptr->display_info.bg.bg_type = GUI_BG_IMG;
            anim_ctrl_ptr->display_info.bg.img_id  = bg_img_id;

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : set anim background color
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLANIM_SetBgColor(
                               MMI_HANDLE_T     ctrl_handle,
                               GUI_COLOR_T      bg_color
                               )
{
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        CTRLANIM_SetBgColorByPtr(anim_ctrl_ptr,bg_color);
    }
}

/*****************************************************************************/
//  Description : set anim background color
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLANIM_SetBgColorByPtr(
                                    void            *ctrl_ptr,
                                    GUI_COLOR_T     bg_color
                                    )
{
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = ctrl_ptr;

    if ((PNULL != anim_ctrl_ptr) &&
        (GUIANIM_TypeOf((CTRLBASE_OBJ_T *)anim_ctrl_ptr)))
    {
        //set background color
        anim_ctrl_ptr->display_info.bg.bg_type = GUI_BG_COLOR;
        anim_ctrl_ptr->display_info.bg.color   = bg_color;
    }
}

/*****************************************************************************/
//  Description : set anim border
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLANIM_SetBorder(
                              MMI_HANDLE_T  ctrl_handle,
                              GUI_BORDER_T  *border_ptr
                              )
{
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        AnimCtrlSetBorder((CTRLBASE_OBJ_T *)anim_ctrl_ptr,border_ptr);
    }
}

/*****************************************************************************/
//  Description : set anim border
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void GUIANIM_NotifyUpdateByPtr(
                                      CTRLANIM_OBJ_T *anim_ctrl_ptr,
                                      BOOLEAN  is_notify_update// // 当背景为GUI_BG_NONE时，是否无条件给父亲发送MSG_NOTIFY_UPDATE
                                      )
{
    if (PNULL != anim_ctrl_ptr)
    {
        anim_ctrl_ptr->is_notify_update = is_notify_update;
    }
}

/*****************************************************************************/
//  Description : set anim frame
//  Global resource dependence : 
//  Author: Jassmine
//  Note: only for rotate image
/*****************************************************************************/
PUBLIC void CTRLANIM_SetFrame(
                             MMI_HANDLE_T   ctrl_handle,
                             GUI_BORDER_T   *frame_ptr
                             )
{
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if ((PNULL != anim_ctrl_ptr) &&
        (PNULL != frame_ptr))
    {
        anim_ctrl_ptr->theme.frame = *frame_ptr;
    }
}

/*****************************************************************************/
//  Description : set anim default image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetDefaultIcon(
                                      MMI_HANDLE_T      ctrl_handle,
                                      MMI_IMAGE_ID_T    def_img_id, //default image id
                                      MMI_IMAGE_ID_T    err_img_id  //error image id
                                      )
{
    BOOLEAN         result = FALSE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        result = CTRLANIM_SetDefaultIconByPtr(anim_ctrl_ptr, def_img_id, err_img_id);
    }

    return (result);
}

/*****************************************************************************/
//  Description : set anim default image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetDefaultIconByPtr(
                                           void             *ctrl_ptr,
                                           MMI_IMAGE_ID_T   def_img_id,
                                           MMI_IMAGE_ID_T   err_img_id
                                           )
{
    BOOLEAN         result = FALSE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = ctrl_ptr;

    if ((PNULL != anim_ctrl_ptr) &&
        (GUIANIM_TypeOf((CTRLBASE_OBJ_T *)anim_ctrl_ptr)))
    {
        //set default image
        anim_ctrl_ptr->theme.default_img_id = def_img_id;

        //set error image
        anim_ctrl_ptr->theme.error_bg.bg_type = GUI_BG_IMG;
        anim_ctrl_ptr->theme.error_bg.img_id  = err_img_id;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set anim error bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetErrorBg(
                                  MMI_HANDLE_T  ctrl_handle,
                                  GUI_BG_T      *error_bg_ptr
                                  )
{
    BOOLEAN         result = FALSE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if ((PNULL != anim_ctrl_ptr) &&
        (PNULL != error_bg_ptr))
    {
        anim_ctrl_ptr->theme.error_bg = *error_bg_ptr;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set anim display horizontal or vertical by image width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLANIM_SetAutoAdaptDisplay(
                                        BOOLEAN         is_adapt,   //in
                                        MMI_HANDLE_T    ctrl_handle //in
                                        )
{
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        //set auto adapt display
        anim_ctrl_ptr->is_auto_adapt_disp = is_adapt;
    }
}

/*****************************************************************************/
//  Description : 设置动画间隔最小时间,默认100ms
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLANIM_SetPeriod(
                              uint32        min_period, //in
                              MMI_HANDLE_T  ctrl_handle //in
                              )
{
    CTRLANIM_OBJ_T   *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        anim_ctrl_ptr->theme.period = min_period;
    }
}

/*****************************************************************************/
//  Description : update anim display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLANIM_UpdateDisplay(
                                  MMI_HANDLE_T  ctrl_handle
                                  )
{
    CTRLANIM_OBJ_T   *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        //reload resource
        LoadAnimOrImgRes(anim_ctrl_ptr);

        //display
        GUIANIM_Display(FALSE,TRUE,anim_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : update anim display rect by pointer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_UpdateRect(
                                  GUI_RECT_T        rect,
                                  MMI_HANDLE_T      ctrl_handle
                                  )
{
    BOOLEAN         result = FALSE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        result = CTRLANIM_UpdateRectByPtr(rect,anim_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : update anim display rect by pointer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_UpdateRectByPtr(
                                       GUI_RECT_T   rect,
                                       void         *ctrl_ptr
                                       )
{
    BOOLEAN         result = FALSE;
    GUI_RECT_T      anim_rect = {0};
    GUI_RECT_T      update_rect = {0};
    GUI_RECT_T      display_rect = {0};
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = (CTRLANIM_OBJ_T*)ctrl_ptr;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)ctrl_ptr;

    //get anim rect,except border
    if ((PNULL != anim_ctrl_ptr) &&
        (GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect)))
    {
        //get cross rect from control rect and update rect
        if (GUI_IntersectRect(&update_rect,anim_rect,rect))
        {
            //reload resource
            LoadAnimOrImgRes(anim_ctrl_ptr);

            //set display rect
            display_rect = base_ctrl_ptr->display_rect;
            base_ctrl_ptr->display_rect = update_rect;

            //display
            result = GUIANIM_Display(FALSE,TRUE,anim_ctrl_ptr);

            //reset display rect
            base_ctrl_ptr->display_rect = display_rect;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : delete the miniature file
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_DeleteMiniatureFile(
                                           const wchar  *full_path_ptr,     //in
                                           uint16       full_path_len       //in
                                           )
{
    BOOLEAN                         result = FALSE;
    uint16                          miniature_width = 0;
    uint16                          miniature_height = 0;
    GUIANIM_MINIATURE_FULL_PATH_T   miniature_path = {0};
    
    //get miniature width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&miniature_width,&miniature_height);

    //get miniature name
    SetMiniatureFullPath(full_path_ptr,
        full_path_len,
        miniature_width,
        miniature_height,
        &miniature_path);
    
    //delete file
    if(SFS_ERROR_NONE == SFS_DeleteFile(miniature_path.full_path_wstr,PNULL))
    {
        result = FALSE;
    }

    //get miniature name
    SetMiniatureFullPath(full_path_ptr,
        full_path_len,
        miniature_height,
        miniature_width,
        &miniature_path);
    
    //delete file
    if(SFS_ERROR_NONE == SFS_DeleteFile(miniature_path.full_path_wstr,PNULL))
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : anim is decoded and support
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_IsSupport(
                                 MMI_HANDLE_T   ctrl_handle
                                 )
{
    BOOLEAN         result = FALSE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        if ((anim_ctrl_ptr->is_support) &&
            (anim_ctrl_ptr->is_decoded))
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set anim is visible
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetVisible(
                                  MMI_HANDLE_T  ctrl_handle,
                                  BOOLEAN       is_visible,
                                  BOOLEAN       is_update
                                  )
{
    BOOLEAN         result = FALSE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

        //set visible
        VTLBASE_SetVisible(base_ctrl_ptr, is_visible);
        //IGUICTRL_SetState((CTRLBASE_OBJ_T *)anim_ctrl_ptr,GUICTRL_STATE_INVISIBLE,(BOOLEAN)(!is_visible));

        if (is_update)
        {
            if (is_visible)
            {
                //reload resource
                LoadAnimOrImgRes(anim_ctrl_ptr);

                //display
                GUIANIM_Display(FALSE,FALSE,anim_ctrl_ptr);
            }
            else
            {
                //notify parent update bg
                GUICTRL_SendNotifyEx(base_ctrl_ptr->handle,MSG_NOTIFY_UPDATE,&base_ctrl_ptr->rect);
            }
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set anim owner draw callback function
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLANIM_SetOwnerDraw(
                                 MMI_HANDLE_T               ctrl_handle,
                                 GUIANIM_OWNER_DRAW_FUNC    func
                                 )
{
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        //set function
        anim_ctrl_ptr->CallBackFunc.OwnerDrawFunc = func;
    }
}

/*****************************************************************************/
//  Description : set anim owner draw callback function
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLANIM_SetOwnerDrawEx(
                                   MMI_HANDLE_T         ctrl_handle,
                                   GUIANIM_CALL_BACK_T  *call_back_ptr
                                   )
{
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        //set call back
        anim_ctrl_ptr->CallBackFunc = *call_back_ptr;
    }
}

/*****************************************************************************/
//  Description : get anim display bitmap
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLANIM_GetAnimDisplayBitmap(
    MMI_HANDLE_T  ctrl_handle,
    GUIIMG_BITMAP_T *bitmap_ptr
)
{
    GUIANIM_GetDisplayBitmap(GUIANIM_GetPtr(ctrl_handle), bitmap_ptr);
}

/*****************************************************************************/
//  Description : anim is tp enable
//  Global resource dependence :
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
PUBLIC void CTRLANIM_IsTpEnable(MMI_HANDLE_T   ctrl_handle, BOOLEAN is_tp_enable)
{
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL == anim_ctrl_ptr)
    {
        return;
    }
     anim_ctrl_ptr->base_ctrl.is_tp_enabled = is_tp_enable;
}

