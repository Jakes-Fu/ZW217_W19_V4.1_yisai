/*! @file guimsgbox.h
@brief 此文件介绍了msgbox控件的对外接口和数据类型
@author panda.luo 
@version 1.0
@date 16/04/2004 
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note  04/2004       panda.luo          Create   
*******************************************************************************/

/*! @page page_MsgBox MsgBox

(暂无)

*******************************************************************************/

/*! @addtogroup  msgboxGrp MsgBox
@brief  此文件介绍了msgbox控件的对外接口和数据类型
@details  此文件介绍了msgbox控件的对外接口和数据类型
@{
*******************************************************************************/



#ifndef _CTRLMSGBOX_EXPORT_H_
#define _CTRLMSGBOX_EXPORT_H_

/*--------------------------------------------------------------------------*/
/*                         Include Files                                    */
/*--------------------------------------------------------------------------*/
#include    "guicommon.h"
#include    "mmk_type.h"
#include    "guianim.h"
#include    "guisoftkey.h"
#include 	"guilcd.h"

#include "ctrlbase_export.h"

/*--------------------------------------------------------------------------*/
/*                         Compiler Flag                                    */
/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*--------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                 */
/*--------------------------------------------------------------------------*/


/*! @typedef GUIMSGBOX_STYLE_T
@brief 
*/
typedef uint16 					GUIMSGBOX_STYLE_T;
/*! @def GUIMSGBOX_TEXT_RIGHT
@brief 文本靠左显示
*/
#define GUIMSGBOX_TEXT_RIGHT        	0x0001     	/*!<文本靠左显示 */
/*! @def GUIMSGBOX_TEXT_HCENTER
@brief 文本水平居中显示
*/
#define GUIMSGBOX_TEXT_HCENTER      	0x0002      /*!<文本水平居中显示 */
/*! @def GUIMSGBOX_TEXT_TOP
@brief 文本靠上显示
*/
#define GUIMSGBOX_TEXT_TOP          	0x0004      /*!<文本靠上显示 */
/*! @def GUIMSGBOX_TEXT_VCENTER
@brief 文本垂直居中显示
*/
#define GUIMSGBOX_TEXT_VCENTER      	0x0008      /*!<文本垂直居中显示 */
/*! @def GUIMSGBOX_TEXT_MASK
@brief 
*/
#define GUIMSGBOX_TEXT_MASK         	0x000F
/*! @def GUIMSGBOX_BITMAP_TOP
@brief 图片显示在文本上方 
*/
#define GUIMSGBOX_BITMAP_TOP        	0X0000      /*!<图片显示在文本上方 */
/*! @def GUIMSGBOX_BITMAP_RIGHT
@brief 图片显示在文本右边
*/
#define GUIMSGBOX_BITMAP_RIGHT      	0X0010      /*!<图片显示在文本右边 */
/*! @def GUIMSGBOX_BITMAP_BOTTOM
@brief 图片显示在文本下方 
*/
#define GUIMSGBOX_BITMAP_BOTTOM     	0X0020      /*!<图片显示在文本下方 */
/*! @def GUIMSGBOX_BITMAP_LEFT
@brief 图片显示在文本左边
*/
#define GUIMSGBOX_BITMAP_LEFT       	0X0040      /*!<图片显示在文本左边*/
/*! @def GUIMSGBOX_STYLE_CUSTOMED
@brief 图片和文本的具体显示位置有用户定义
*/
#define GUIMSGBOX_STYLE_CUSTOMED    	0x0080      /*!<图片和文本的具体显示位置有用户定义*/
/*! @def GUIMSGBOX_BITMAP_MASK
@brief 
*/
#define GUIMSGBOX_BITMAP_MASK       	0X00F0      
/*! @def GUIMSGBOX_BITMAP_LARGE
@brief 图片显示区域大于图片大小时，图片在控件内部显示，其左右边界等于控件边界
*/
#define GUIMSGBOX_BITMAP_LARGE      	0X0100      /*!<图片显示区域大于图片大小时，图片在控件内部显示，其左右边界等于控件边界*/
/*! @def GUIMSGBOX_TEXT_MOVE_TIMEOUT
@brief 滚动显示的频率
*/
#define GUIMSGBOX_TEXT_MOVE_TIMEOUT    	2000		/*!<滚动显示的频率*/
/*! @def GUIMSGBOX_ANIMA_TIMEOUT
@brief 动画显示频率 
*/
#define GUIMSGBOX_ANIMA_TIMEOUT			250			/*!<动画显示频率 */


/*--------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                */
/*--------------------------------------------------------------------------*/
// msgbox handle
typedef struct _msgbox_obj_tag* CTRL_MSGBOX_HANDLE_T;

/*! @enum  GUIMSGBOX_IMAGE_E 
@brief 消息框类型. 支持 1)无图片,2)动画 ,3)图片 
*/
typedef enum
{
    GUIMSGBOX_IMAGE_NONE,/*!<无 */
    GUIMSGBOX_IMAGE_ANIMA,/*!<动画 */
    GUIMSGBOX_IMAGE_BMP,/*!<图片*/
    GUIMSGBOX_IMAGE_INVALID/*!< */
}GUIMSGBOX_IMAGE_E;


/*! @struct  GUIMSGBOX_INIT_DATA_T 
@brief  初始化数据
*/
typedef struct 
{
    GUIMSGBOX_STYLE_T   style;/*!<类型 */
    uint8               lcd_id;/*!<lcd的id */
    GUI_RECT_T			*rect_ptr;/*!<显示矩形*/
    
}GUIMSGBOX_INIT_DATA_T;

/*!<
GUIMSGBOX_TEXT_INFO_T define message box's text attribute.
message don't have rollbar,so,if display infor excess the rect,auto roll will run.
*/

/*--------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                              */
/*--------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief 设置消息框的文本，动画图片，title等参数
@author panda.luo
@param    ctrl_id	          [in] 		   控件id
@param    title_text_id    [in]    title文本id
@param    image_type	   [in] 	   图片类型：图片或者动画
@param    image_id	   [in]	   图片id
@param    anim_id	          [in]		   动画id
@param    image_rect_ptr	[in]  图片显示区域
@param    string_ptr	 [in]	   显示文本，不能为PUNLL
@param    text_rect_ptr   [in]  文本显示区域
@param    left_sk_id	[in]	   左软键文本id
@param    right_sk_id	[in]	   右软键文本id
@return void
@note   根据传入的图片、字符串、动画的属性创建消息框
*******************************************************************************/
void CTRLMSGBOX_SetParam(   
                		MMI_CTRL_ID_T	    ctrl_id,			
                		MMI_TEXT_ID_T       title_text_id,
                		GUIMSGBOX_IMAGE_E   image_type,
                		MMI_IMAGE_ID_T      image_id, 
                		MMI_ANIM_ID_T       anim_id,                     
                		GUI_RECT_T			*image_rect_ptr,
                		MMI_STRING_T		*string_ptr,
                		GUI_RECT_T			*text_rect_ptr,
                		MMI_TEXT_ID_T       left_sk_id,
                		MMI_TEXT_ID_T       right_sk_id
                        );


/***************************************************************************//*!
@brief  设置消息框的软键
@author lin.lin
@param     ctrl_id 	[in]	  控件id
@param     left_sk_id  [in]   做软键文本id
@param     right_sk_id  [in]  右软键文本id
@param     sk_rect_ptr [in]  软键显示区域
@param     sk_bg_color  [in]  软键背景颜色
@return void
@note   设置弹出消息框时，对应的softkey的显示
*******************************************************************************/
void CTRLMSGBOX_SetSoftkey(   
                          MMI_CTRL_ID_T	    ctrl_id,
                   	      MMI_TEXT_ID_T     left_sk_id,
                          MMI_TEXT_ID_T     right_sk_id,
                    	  GUI_RECT_T		*sk_rect_ptr,
                    	  GUI_COLOR_T       sk_bg_color
                          );

	                   

/***************************************************************************//*!
@brief  设置消息框的图片或动画
@author Lin.Lin
@param     ctrl_id 	[in]	 控件id
@param     image_type [in]   图片类型：图片或动画
@param     image_id  [in]	 图片资源id  (可以为空)
@param     anim_id  [in]	 动画资源id (可以为空)
@param     rect_ptr  [in]	 显示区域 ptr
@return void
@note   设置消息框中的图片或动画显示
*******************************************************************************/
void CTRLMSGBOX_SetImageAnim(  
                        	MMI_CTRL_ID_T	    ctrl_id,
	                    	GUIMSGBOX_IMAGE_E   image_type,
                        	MMI_IMAGE_ID_T      image_id,
                        	MMI_ANIM_ID_T       anim_id,
                        	GUI_RECT_T          *rect_ptr
    	                    );


/***************************************************************************//*!
@brief  设置消息框显示的文本和其对应的显示区域（仅在消息框类型为GUIMSGBOX_STYLE_CUSTOMED）
        时有效）        
@author Lin.Lin
@param      ctrl_id 	[in]   控件id
@param      string_ptr   [in] 显示文本，不能为空
@param      rect_ptr	[in]  显示区域
@return void 
@note   设置消息框中的字符串显示
*******************************************************************************/
void CTRLMSGBOX_SetText(   
             		   MMI_CTRL_ID_T	ctrl_id,
             		   MMI_STRING_T		*string_ptr,
             		   GUI_RECT_T		*rect_ptr
	                   );


/***************************************************************************//*!
@brief  设置消息框的背景图片和颜色
@author Lin.Lin
@param     ctrl_id  	[in]   控件id
@param     image_id     [in]	  图片资源id
@param     back_color    [in]  背景颜色 
@return void
@note    设置消息框中的背景显示
*******************************************************************************/
void CTRLMSGBOX_SetBackground(   
                    		 MMI_CTRL_ID_T		ctrl_id,
                    		 MMI_IMAGE_ID_T  	image_id,
                    		 GUI_COLOR_T     	back_color
	                         );


/***************************************************************************//*!
@brief  设置消息框的title文本
@author Lin.Lin
@param      ctrl_id	[in]	     控件id
@param     title_text_id  [in]   title文本id
@return void
@note   设置消息框弹出式title的显示
*******************************************************************************/
void CTRLMSGBOX_SetTitleText(   
                    		MMI_CTRL_ID_T		ctrl_id,
                    		MMI_TEXT_ID_T       title_text_id
	                        );


/***************************************************************************//*!
@brief  设置title高度和颜色
@author Lin.Lin
@param      ctrl_id  [in]  控件id
@param      height  [in]   title高度
@param     color [in]      title颜色
@return void 
@note   设置title的高和颜色参数
*******************************************************************************/
void CTRLMSGBOX_SetTitleParam(   
                    		 MMI_CTRL_ID_T		ctrl_id,
                    		 uint8               height,   
                    		 GUI_COLOR_T         color   
	                         );


/***************************************************************************//*!
@brief  设置消息框的字体和字体颜色
@author lin.lin
@param      ctrl_id	[in]	  控件id
@param      font	[in]	  字体类型
@param    font_color   [in]   字体颜色
@param     space_char	[in]   字间空格
@param      space_line	[in]  行空格
@param      move_line	[in]  每次指定时间内滚动行数
@param     back_color	[in]   文本显示区域的背景颜色
@return void 
@note   设置消息框中字体的各个参数(颜色、空格等)
*******************************************************************************/
void CTRLMSGBOX_SetFontParam(   
                    		MMI_CTRL_ID_T		ctrl_id,
                    		GUI_FONT_T          font,
                    		GUI_COLOR_T         font_color,
                    		uint8               space_char,
                    		uint8               space_line,
                    		uint8               move_line,
                   	 		GUI_COLOR_T         back_color
                           );


/***************************************************************************//*!
@brief  设置title，图片和文本的显示区域
@author lin.lin
@param    ctrl_id [in]控件id
@retval void
@return void
@note   消息框中显示图片和文本的布局
*******************************************************************************/
void CTRLMSGBOX_Layout(MMI_CTRL_ID_T	ctrl_id);


/***************************************************************************//*!
@brief  更新消息框
@author panda.luo
@param  ctrl_id  [in] control id
@retval void
@return void
@note   刷新消息框，刷新后会重新显示新的消息框
*******************************************************************************/
void CTRLMSGBOX_Update(MMI_CTRL_ID_T	ctrl_id);

/*--------------------------------------------------------------------------------*/
/*                         Compiler Flag                                          */
/*--------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
