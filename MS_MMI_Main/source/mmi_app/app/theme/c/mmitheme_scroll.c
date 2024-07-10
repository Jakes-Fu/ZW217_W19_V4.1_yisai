/*==============================================================================
File Name: mmitheme_scroll.c
Author: lianxiang.zhou & xiyuan.ma
Date: 2012/08/16
Copyright: 2012 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2012/08/16   lianxiang.zhou & xiyuan.ma  Create.
==============================================================================*/
/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/

#include "mmi_position.h"
#include "mmitheme_scroll.h"
#include "guires.h"
#include "mmi_image.h"

#define SCROLL_BAR_WIDTH_FULL (16)
#define SCROLL_BAR_WIDTH_FULL_THIN (5)
#define SCROLL_BAR_ARROW_HEIGHT SCROLL_BAR_WIDTH_FULL
#define SCROLL_BAR_THUMB_MIN_SIZE (8)

/*==============================================================================
Description: MMITHEME_ScrollIsThin
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC BOOLEAN MMITHEME_ScrollIsThin(void)
{
    return FALSE;
}


/*==============================================================================
Description: MMITHEME_ScrollIsThumbOnly
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC BOOLEAN MMITHEME_ScrollIsThumbOnly(void)
{
    return FALSE;//TRUE;
}

/*==============================================================================
Description: MMITHEME_ScrollGetThumbMinSize
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC uint32 MMITHEME_ScrollGetThumbMinSize(void)
{
    return SCROLL_BAR_THUMB_MIN_SIZE;
}

/*==============================================================================
Description: MMITHEME_ScrollGetFullWidth
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC int16 MMITHEME_ScrollGetFullWidth(void)
{
    return SCROLL_BAR_WIDTH_FULL;
}


/*==============================================================================
Description: MMITHEME_ScrollGetArrowHeight
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC int16 MMITHEME_ScrollGetArrowHeight(void)
{
    return SCROLL_BAR_ARROW_HEIGHT;
}


/*==============================================================================
Description: MMITHEME_ScrollGetFullImg
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC void MMITHEME_ScrollGetFullImg(
    MMI_IMAGE_ID_T *top_arrow_ptr,
    MMI_IMAGE_ID_T *top_arrow_pressed_ptr,
    MMI_IMAGE_ID_T *bottom_arrow_ptr,
    MMI_IMAGE_ID_T *bottom_arrow_pressed_ptr,
    MMI_IMAGE_ID_T *region_ptr,
    MMI_IMAGE_ID_T *thumb_ptr
)
{
    if (PNULL != top_arrow_ptr) *top_arrow_ptr = IMAGE_SCROLL_UP_ARROW_2;
    if (PNULL != top_arrow_pressed_ptr) *top_arrow_pressed_ptr = IMAGE_SCROLL_UP_ARROW_FOCUS_2;

    if (PNULL != bottom_arrow_ptr) *bottom_arrow_ptr = IMAGE_SCROLL_DOWN_ARROW_2;
    if (PNULL != bottom_arrow_pressed_ptr) *bottom_arrow_pressed_ptr = IMAGE_SCROLL_DOWN_ARROW_FOCUS_2;

    if (PNULL != region_ptr) *region_ptr = IMAGE_SCROLL_VER_GROOVE;
    if (PNULL != thumb_ptr) *thumb_ptr = IMAGE_SCROLL_VER_BAR_2;

    return;
}



/*==============================================================================
Description: MMITHEME_ScrollGetFullThinWidth
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC int16 MMITHEME_ScrollGetFullThinWidth(void)
{
    return SCROLL_BAR_WIDTH_FULL_THIN;
}


/*==============================================================================
Description: MMITHEME_ScrollGetFullImg
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC void MMITHEME_ScrollGetFullThinImg(
    MMI_IMAGE_ID_T *region_ptr,
    MMI_IMAGE_ID_T *thumb_ptr
)
{
    if (PNULL != region_ptr) *region_ptr = IMAGE_SCROLL_VER_GROOVE;
    if (PNULL != thumb_ptr) *thumb_ptr = IMAGE_SCROLL_VER_BAR_2;

    return;
}



/*==============================================================================
Description: MMITHEME_ScrollGetThumbWidth
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC uint16 MMITHEME_ScrollGetThumbWidth(MMI_HANDLE_T win_handle)
{
    uint16 thumb_width = 0;
    MMI_IMAGE_ID_T thumb_img = 0;

    MMITHEME_ScrollGetThumbImg(&thumb_img);

    GUIRES_GetImgWidthHeight(&thumb_width,
                             PNULL,
                             thumb_img,
                             win_handle);

    return thumb_width;
}



/*==============================================================================
Description: MMITHEME_ScrollGetThumbHeight
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC uint16 MMITHEME_ScrollGetThumbHeight(MMI_HANDLE_T win_handle)
{
    uint16 thumb_height = 0;
    MMI_IMAGE_ID_T thumb_img = 0;

    MMITHEME_ScrollGetThumbImg(&thumb_img);

    GUIRES_GetImgWidthHeight(PNULL,
                             &thumb_height,
                             thumb_img,
                             win_handle);

    return thumb_height;
}



/*==============================================================================
Description: MMITHEME_ScrollGetThumbImg
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC void MMITHEME_ScrollGetThumbImg(MMI_IMAGE_ID_T *thumb_ptr)
{
#if defined(GUIF_PRGBOX_AUTOHIDE)
    if (PNULL != thumb_ptr) *thumb_ptr = IMAGE_SCROLL_THUMB;
#endif

    return;
}


/*==============================================================================
Description: MMITHEME_ScrollGetThumbThinWidth
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC int16 MMITHEME_ScrollGetThumbThinWidth(void)
{
    return SCROLL_BAR_WIDTH_FULL_THIN;
}


/*==============================================================================
Description: MMITHEME_ScrollGetThumbThinImg
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC void MMITHEME_ScrollGetThumbThinImg(MMI_IMAGE_ID_T *thumb_ptr)
{
    if (PNULL != thumb_ptr) *thumb_ptr = IMAGE_SCROLL_VER_BAR_2;

    return;
}

