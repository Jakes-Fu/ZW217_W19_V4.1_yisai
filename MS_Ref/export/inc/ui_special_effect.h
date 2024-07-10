/******************************************************************************
 ** File Name:      ui_special_effect.c                                       *
 ** Author:           Shan.He                                                 *
 ** DATE:             2008-7-3                                                *
 ** Copyright:       2008 Spreatrum, Incoporated. All Rights Reserved.        *
 ** Description:     UI special effect functions                              *
 ** Note:              All the functions only process RGB565 data             *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             * 
 ** 2008-07-3      Shan.He             Create.                                *
 *****************************************************************************/

#ifndef _UI_SPECIAL_EFFECT_H_
#define _UI_SPECIAL_EFFECT_H_

#ifdef UISE_VC
    #include "..\VC Code\MenuDemo\sci_types.h"
    #include "..\VC Code\MenuDemo\os_api.h"
#else
    #include "sci_types.h"
    #include "os_api.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    SE_DIRECTION_LEFT = 0, 
    SE_DIRECTION_RIGHT, 
    SE_DIRECTION_UP, 
    SE_DIRECTION_DOWN
}SE_DIRECTION_E;

typedef struct SE_RECT_T_tag
{
    int32 left;
    int32 top;
    int32 right;
    int32 bottom;
}SE_RECT_T;

typedef struct SE_INPUT_PARAM_T_tag
{
    uint16 *             selected_ptr;	                //buffer of selected icon (selected background), 
    uint16 *             selected_menu_bk_ptr;		    //buffer of selected icon (menu background)	
    uint16 *             next_ptr;			            //buffer of next icon (selected background)	
    uint16 *             next_menu_bk_ptr;			    //buffer of next icon (menu background)	
    uint16 *             temp_buf_ptr;		            //temp buffer
    SE_RECT_T            selected_rect;	                //rectangle of selected icon
    SE_RECT_T            next_rect;		                //rectangel of next icon
    SE_RECT_T            menu_rect;                     // the rectangle contains all the icons
    int32                key_color;		                //key color used to fill the background
    int32                frame_index;		            //frame index, index of the first frame is 0
    SE_DIRECTION_E       direction;		                //moving direction from selected icon to next icon    
    BOOLEAN              is_mem_ok;                     //alloc memory ok.
}SE_INPUT_PARAM_T;

typedef struct SE_OUT_PARAM_T_tag
{
    uint16 *                    output_buf_ptr;				    //output buffer
    SE_RECT_T                output_rect;				        //position and size of output icon
    BOOLEAN                   is_key_color_used;			    //is the key color used
}SE_OUTPUT_PARAM_T;

typedef enum 
{
    SE_EFFECT_BEGIN = 0,
    SE_DESKTOP_ROTATE,                         //桌面旋转                                                             
    SE_MENU_ITEM_MOVE,                        //菜单项滚动
    SE_MAGIC_MOVE,                                //移动魔术
    SE_ROTATE_MOVE_3D,                         // 3D 魔方
    SE_ROTATE_MOVE,                               //旋转乐趣
    SE_LUCKY_BOARD,                               //幸运翻板
    SE_SLIDE_MOVE,                                  //滑滑布丁
    SE_DISTORT_MOVE,                              //流转星云
    SE_WRAPPED_MOVE,                            //转动云彩
    SE_EFFECT_END
}SE_EFFECT_ID_E;

/******************************************************************************/
//  Description: get frame amount of each special effect
//  Author: Shan.He 
//  Input:  
//          effect_id: ID of each special effect
//  Output: none   
//  Return:   
//          0: unknown special effect ID
//          others: frame amount of the effect
//  Note:       
/******************************************************************************/
PUBLIC uint32 SE_GetEffectFrameAmount(SE_EFFECT_ID_E effect_id);

/******************************************************************************/
//  Description: Desktop rotate(桌面3D旋转效果)
//  Author: Shan.He 
//  Input:  input_param_ptr 
//          selected_menu_bk_ptr:   not use, should be NULL
//          next_menu_bk_ptr:       not use, should be NULL
//          temp_buf_ptr:           not use, should be NULL
//          frame_index:            0-2
//  Output: output_param_ptr   
//          output_buf_ptr:         size = icon_width * icon_height * 2
//  Return: SCI_SUCCESS:    successful
//             others               failed
//  Note:       
/******************************************************************************/
PUBLIC int32 SE_DesktopRotate(SE_INPUT_PARAM_T * input_param_ptr, SE_OUTPUT_PARAM_T * output_param_ptr);

/******************************************************************************/
//  Description: Move menu item (菜单项上下滚动)
//  Author: Shan.He 
//  Input:  input_param_ptr 
//       selected_menu_bk_ptr:   not use, should be NULL
//       next_menu_bk_ptr：      not use, should be NULL
//       temp_buf_ptr：          not use, should be NULL
//       frame_index：           0-4
//  Output:  output_param_ptr   
//        output_buf_ptr:        size = icon_width * icon_height * 2
//  Return: SCI_SUCCESS:    successful
//             others               failed
//  Note:       
/******************************************************************************/
PUBLIC int32 SE_MenuItemMove(SE_INPUT_PARAM_T * input_param_ptr, SE_OUTPUT_PARAM_T * output_param_ptr);

/******************************************************************************/
//  Description: Magic move effect (移动魔术)
//  Author: Shan.He 
//  Input:  input_param_ptr 
//          selected_menu_bk_ptr:   not use, should be NULL
//          next_menu_bk_ptr：      not use, should be NULL
//          temp_buf_ptr：          not use, should be NULL
//          frame_index：           0-2
//  Output: output_param_ptr   
//          output_buf_ptr:         size = icon_width * icon_height * 2
//  Return: SCI_SUCCESS:    successful
//             others               failed
//  Note:       
/******************************************************************************/
PUBLIC int32 SE_MagicMove(SE_INPUT_PARAM_T * input_param_ptr, SE_OUTPUT_PARAM_T * output_param_ptr);

/******************************************************************************/
//  Description: Slide move (滑滑布丁)
//  Author: Shan.He 
//  Input:  input_param_ptr 
//       selected_menu_bk_ptr:   not use, should be NULL
//       next_menu_bk_ptr：      not use, should be NULL
//       temp_buf_ptr：          use, size = 4 * (icon_width * icon_height * 2)
//       frame_index：           0-9
//  Output:  output_param_ptr   
//        output_buf_ptr:        size = 2 * (icon_width * icon_height * 2)
//  Return: SCI_SUCCESS:    successful
//             others               failed
//  Note:       
/******************************************************************************/
PUBLIC int32 SE_SlideMove(SE_INPUT_PARAM_T * input_param_ptr, SE_OUTPUT_PARAM_T * output_param_ptr);

/******************************************************************************/
//  Description: 3D rotate move (3D魔方)
//  Author: Shan.He 
//  Input:  input_param_ptr 
//       selected_menu_bk_ptr:   not use, should be NULL
//       next_menu_bk_ptr：      not use, should be NULL
//       temp_buf_ptr：          size = (icon_width * icon_height * 2)
//       frame_index：           0-6
//  Output:  output_param_ptr   
//        output_buf_ptr:        size = (icon_width * icon_height * 2)
//  Return: SCI_SUCCESS:    successful
//             others               failed
//  Note:       
/******************************************************************************/
PUBLIC int32 SE_RotateMove3D(SE_INPUT_PARAM_T * input_param_ptr, SE_OUTPUT_PARAM_T * output_param_ptr);

/******************************************************************************/
//  Description: rotate move (旋转乐趣)
//  Author: Shan.He 
//  Input:    input_param_ptr 
//              selected_menu_bk_ptr:   not use, should be NULL
//              next_menu_bk_ptr：     not use, should be NULL
//              temp_buf_ptr：           use, size = 4 * (icon_width * icon_height * 2)
//              frame_index：             0-4
//  Output:  output_param_ptr   
//              output_buf_ptr:             size = 2 * (icon_width * icon_height * 2)
//  Return: SCI_SUCCESS:                successful
//             others                           failed
//  Note:       
/******************************************************************************/
PUBLIC int32 SE_RotateMove(SE_INPUT_PARAM_T * input_param_ptr, SE_OUTPUT_PARAM_T * output_param_ptr);

/******************************************************************************/
//  Description: distort move (流转星云)
//  Author: Shan.He 
//  Input:   input_param_ptr 
//              selected_menu_bk_ptr:  not use, should be NULL
//              next_menu_bk_ptr:       not use, should be NULL
//              temp_buf_ptr：          use, size = (icon_width * icon_height * 2)
//              frame_index：            0-12
//  Output:  output_param_ptr   
//              output_buf_ptr:            size = (icon_width * icon_height * 2)
//  Return: SCI_SUCCESS:                successful
//             others                           failed
//  Note:       
/******************************************************************************/
PUBLIC int32 SE_DistortMove(SE_INPUT_PARAM_T * input_param_ptr, SE_OUTPUT_PARAM_T * output_param_ptr);

/******************************************************************************/
//  Description: Lucky board (幸运翻板)
//  Author: Shan.He 
//  Input:  input_param_ptr 
//       selected_menu_bk_ptr:   use, buffer of icon with menu background
//       next_menu_bk_ptr：      not use, should be NULL
//       temp_buf_ptr：          size = 2 * (icon_width * icon_height * 2)
//       frame_index：           0-15
//  Output:  output_param_ptr   
//        output_buf_ptr:        size = 2 * (icon_width * icon_height * 2)
//  Return: SCI_SUCCESS:    successful
//             others               failed
//  Note:       
/******************************************************************************/
PUBLIC int32 SE_LuckyBoard(SE_INPUT_PARAM_T * input_param_ptr, SE_OUTPUT_PARAM_T * output_param_ptr);

/******************************************************************************/
//  Description: Wrapped move (转动彩带)
//  Author: Shan.He 
//  Input:  input_param_ptr 
//              selected_menu_bk_ptr:   not use, should be NULL
//              next_menu_bk_ptr:        not use, should be NULL
//              temp_buf_ptr:               use, size = 4 * (icon_width * icon_height * 2)
//              frame_index：             0-11
//  Output:  output_param_ptr   
//              output_buf_ptr:             size = 2 * (icon_width * icon_height * 2)
//  Return: SCI_SUCCESS:    successful
//             others               failed
//  Note:       
/******************************************************************************/
PUBLIC int32 SE_WrappedMove(SE_INPUT_PARAM_T * input_param_ptr, SE_OUTPUT_PARAM_T * output_param_ptr);
                                               
#ifdef __cplusplus
}
#endif

#endif
