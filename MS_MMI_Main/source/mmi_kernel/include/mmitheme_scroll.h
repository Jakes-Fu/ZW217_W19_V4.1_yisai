/*==============================================================================
File Name: mmitheme_scroll.h
Author: Lianxiang.Zhou
Date: 2012/08/16
Copyright: 2012 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2012/08/16   Lianxiang.Zhou  Create.
==============================================================================*/
/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/

#ifndef _MMITHEME_SCROLL_H_
#define _MMITHEME_SCROLL_H_

/*==============================================================================
Description: MMITHEME_ScrollIsThin
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC BOOLEAN MMITHEME_ScrollIsThin(void);


/*==============================================================================
Description: MMITHEME_ScrollIsThumbOnly
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC BOOLEAN MMITHEME_ScrollIsThumbOnly(void);


/*==============================================================================
Description: MMITHEME_ScrollGetThumbMinSize
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC uint32 MMITHEME_ScrollGetThumbMinSize(void);


/*==============================================================================
Description: MMITHEME_ScrollGetFullWidth
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC int16 MMITHEME_ScrollGetFullWidth(void);


/*==============================================================================
Description: MMITHEME_ScrollGetArrowHeight
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC int16 MMITHEME_ScrollGetArrowHeight(void);


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
);



/*==============================================================================
Description: MMITHEME_ScrollGetFullThinWidth
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC int16 MMITHEME_ScrollGetFullThinWidth(void);


/*==============================================================================
Description: MMITHEME_ScrollGetFullImg
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC void MMITHEME_ScrollGetFullThinImg(
    MMI_IMAGE_ID_T *region_ptr,
    MMI_IMAGE_ID_T *thumb_ptr
);



/*==============================================================================
Description: MMITHEME_ScrollGetThumbWidth
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC uint16 MMITHEME_ScrollGetThumbWidth(MMI_HANDLE_T win_handle);



/*==============================================================================
Description: MMITHEME_ScrollGetThumbHeight
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC uint16 MMITHEME_ScrollGetThumbHeight(MMI_HANDLE_T win_handle);



/*==============================================================================
Description: MMITHEME_ScrollGetThumbImg
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC void MMITHEME_ScrollGetThumbImg(MMI_IMAGE_ID_T *thumb_ptr);


/*==============================================================================
Description: MMITHEME_ScrollGetThumbThinWidth
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC int16 MMITHEME_ScrollGetThumbThinWidth(void);


/*==============================================================================
Description: MMITHEME_ScrollGetThumbThinImg
Global resource dependence: NONE
Author: lianxiang.zhou & xiyuan.ma
Note:
==============================================================================*/
PUBLIC void MMITHEME_ScrollGetThumbThinImg(MMI_IMAGE_ID_T *thumb_ptr);






#endif /*_MMITHEME_SCROLL_H_*/




