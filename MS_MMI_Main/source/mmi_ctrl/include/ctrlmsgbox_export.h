/*! @file guimsgbox.h
@brief ���ļ�������msgbox�ؼ��Ķ���ӿں���������
@author panda.luo 
@version 1.0
@date 16/04/2004 
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note  04/2004       panda.luo          Create   
*******************************************************************************/

/*! @page page_MsgBox MsgBox

(����)

*******************************************************************************/

/*! @addtogroup  msgboxGrp MsgBox
@brief  ���ļ�������msgbox�ؼ��Ķ���ӿں���������
@details  ���ļ�������msgbox�ؼ��Ķ���ӿں���������
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
@brief �ı�������ʾ
*/
#define GUIMSGBOX_TEXT_RIGHT        	0x0001     	/*!<�ı�������ʾ */
/*! @def GUIMSGBOX_TEXT_HCENTER
@brief �ı�ˮƽ������ʾ
*/
#define GUIMSGBOX_TEXT_HCENTER      	0x0002      /*!<�ı�ˮƽ������ʾ */
/*! @def GUIMSGBOX_TEXT_TOP
@brief �ı�������ʾ
*/
#define GUIMSGBOX_TEXT_TOP          	0x0004      /*!<�ı�������ʾ */
/*! @def GUIMSGBOX_TEXT_VCENTER
@brief �ı���ֱ������ʾ
*/
#define GUIMSGBOX_TEXT_VCENTER      	0x0008      /*!<�ı���ֱ������ʾ */
/*! @def GUIMSGBOX_TEXT_MASK
@brief 
*/
#define GUIMSGBOX_TEXT_MASK         	0x000F
/*! @def GUIMSGBOX_BITMAP_TOP
@brief ͼƬ��ʾ���ı��Ϸ� 
*/
#define GUIMSGBOX_BITMAP_TOP        	0X0000      /*!<ͼƬ��ʾ���ı��Ϸ� */
/*! @def GUIMSGBOX_BITMAP_RIGHT
@brief ͼƬ��ʾ���ı��ұ�
*/
#define GUIMSGBOX_BITMAP_RIGHT      	0X0010      /*!<ͼƬ��ʾ���ı��ұ� */
/*! @def GUIMSGBOX_BITMAP_BOTTOM
@brief ͼƬ��ʾ���ı��·� 
*/
#define GUIMSGBOX_BITMAP_BOTTOM     	0X0020      /*!<ͼƬ��ʾ���ı��·� */
/*! @def GUIMSGBOX_BITMAP_LEFT
@brief ͼƬ��ʾ���ı����
*/
#define GUIMSGBOX_BITMAP_LEFT       	0X0040      /*!<ͼƬ��ʾ���ı����*/
/*! @def GUIMSGBOX_STYLE_CUSTOMED
@brief ͼƬ���ı��ľ�����ʾλ�����û�����
*/
#define GUIMSGBOX_STYLE_CUSTOMED    	0x0080      /*!<ͼƬ���ı��ľ�����ʾλ�����û�����*/
/*! @def GUIMSGBOX_BITMAP_MASK
@brief 
*/
#define GUIMSGBOX_BITMAP_MASK       	0X00F0      
/*! @def GUIMSGBOX_BITMAP_LARGE
@brief ͼƬ��ʾ�������ͼƬ��Сʱ��ͼƬ�ڿؼ��ڲ���ʾ�������ұ߽���ڿؼ��߽�
*/
#define GUIMSGBOX_BITMAP_LARGE      	0X0100      /*!<ͼƬ��ʾ�������ͼƬ��Сʱ��ͼƬ�ڿؼ��ڲ���ʾ�������ұ߽���ڿؼ��߽�*/
/*! @def GUIMSGBOX_TEXT_MOVE_TIMEOUT
@brief ������ʾ��Ƶ��
*/
#define GUIMSGBOX_TEXT_MOVE_TIMEOUT    	2000		/*!<������ʾ��Ƶ��*/
/*! @def GUIMSGBOX_ANIMA_TIMEOUT
@brief ������ʾƵ�� 
*/
#define GUIMSGBOX_ANIMA_TIMEOUT			250			/*!<������ʾƵ�� */


/*--------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                */
/*--------------------------------------------------------------------------*/
// msgbox handle
typedef struct _msgbox_obj_tag* CTRL_MSGBOX_HANDLE_T;

/*! @enum  GUIMSGBOX_IMAGE_E 
@brief ��Ϣ������. ֧�� 1)��ͼƬ,2)���� ,3)ͼƬ 
*/
typedef enum
{
    GUIMSGBOX_IMAGE_NONE,/*!<�� */
    GUIMSGBOX_IMAGE_ANIMA,/*!<���� */
    GUIMSGBOX_IMAGE_BMP,/*!<ͼƬ*/
    GUIMSGBOX_IMAGE_INVALID/*!< */
}GUIMSGBOX_IMAGE_E;


/*! @struct  GUIMSGBOX_INIT_DATA_T 
@brief  ��ʼ������
*/
typedef struct 
{
    GUIMSGBOX_STYLE_T   style;/*!<���� */
    uint8               lcd_id;/*!<lcd��id */
    GUI_RECT_T			*rect_ptr;/*!<��ʾ����*/
    
}GUIMSGBOX_INIT_DATA_T;

/*!<
GUIMSGBOX_TEXT_INFO_T define message box's text attribute.
message don't have rollbar,so,if display infor excess the rect,auto roll will run.
*/

/*--------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                              */
/*--------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief ������Ϣ����ı�������ͼƬ��title�Ȳ���
@author panda.luo
@param    ctrl_id	          [in] 		   �ؼ�id
@param    title_text_id    [in]    title�ı�id
@param    image_type	   [in] 	   ͼƬ���ͣ�ͼƬ���߶���
@param    image_id	   [in]	   ͼƬid
@param    anim_id	          [in]		   ����id
@param    image_rect_ptr	[in]  ͼƬ��ʾ����
@param    string_ptr	 [in]	   ��ʾ�ı�������ΪPUNLL
@param    text_rect_ptr   [in]  �ı���ʾ����
@param    left_sk_id	[in]	   ������ı�id
@param    right_sk_id	[in]	   ������ı�id
@return void
@note   ���ݴ����ͼƬ���ַ��������������Դ�����Ϣ��
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
@brief  ������Ϣ������
@author lin.lin
@param     ctrl_id 	[in]	  �ؼ�id
@param     left_sk_id  [in]   ������ı�id
@param     right_sk_id  [in]  ������ı�id
@param     sk_rect_ptr [in]  �����ʾ����
@param     sk_bg_color  [in]  ���������ɫ
@return void
@note   ���õ�����Ϣ��ʱ����Ӧ��softkey����ʾ
*******************************************************************************/
void CTRLMSGBOX_SetSoftkey(   
                          MMI_CTRL_ID_T	    ctrl_id,
                   	      MMI_TEXT_ID_T     left_sk_id,
                          MMI_TEXT_ID_T     right_sk_id,
                    	  GUI_RECT_T		*sk_rect_ptr,
                    	  GUI_COLOR_T       sk_bg_color
                          );

	                   

/***************************************************************************//*!
@brief  ������Ϣ���ͼƬ�򶯻�
@author Lin.Lin
@param     ctrl_id 	[in]	 �ؼ�id
@param     image_type [in]   ͼƬ���ͣ�ͼƬ�򶯻�
@param     image_id  [in]	 ͼƬ��Դid  (����Ϊ��)
@param     anim_id  [in]	 ������Դid (����Ϊ��)
@param     rect_ptr  [in]	 ��ʾ���� ptr
@return void
@note   ������Ϣ���е�ͼƬ�򶯻���ʾ
*******************************************************************************/
void CTRLMSGBOX_SetImageAnim(  
                        	MMI_CTRL_ID_T	    ctrl_id,
	                    	GUIMSGBOX_IMAGE_E   image_type,
                        	MMI_IMAGE_ID_T      image_id,
                        	MMI_ANIM_ID_T       anim_id,
                        	GUI_RECT_T          *rect_ptr
    	                    );


/***************************************************************************//*!
@brief  ������Ϣ����ʾ���ı������Ӧ����ʾ���򣨽�����Ϣ������ΪGUIMSGBOX_STYLE_CUSTOMED��
        ʱ��Ч��        
@author Lin.Lin
@param      ctrl_id 	[in]   �ؼ�id
@param      string_ptr   [in] ��ʾ�ı�������Ϊ��
@param      rect_ptr	[in]  ��ʾ����
@return void 
@note   ������Ϣ���е��ַ�����ʾ
*******************************************************************************/
void CTRLMSGBOX_SetText(   
             		   MMI_CTRL_ID_T	ctrl_id,
             		   MMI_STRING_T		*string_ptr,
             		   GUI_RECT_T		*rect_ptr
	                   );


/***************************************************************************//*!
@brief  ������Ϣ��ı���ͼƬ����ɫ
@author Lin.Lin
@param     ctrl_id  	[in]   �ؼ�id
@param     image_id     [in]	  ͼƬ��Դid
@param     back_color    [in]  ������ɫ 
@return void
@note    ������Ϣ���еı�����ʾ
*******************************************************************************/
void CTRLMSGBOX_SetBackground(   
                    		 MMI_CTRL_ID_T		ctrl_id,
                    		 MMI_IMAGE_ID_T  	image_id,
                    		 GUI_COLOR_T     	back_color
	                         );


/***************************************************************************//*!
@brief  ������Ϣ���title�ı�
@author Lin.Lin
@param      ctrl_id	[in]	     �ؼ�id
@param     title_text_id  [in]   title�ı�id
@return void
@note   ������Ϣ�򵯳�ʽtitle����ʾ
*******************************************************************************/
void CTRLMSGBOX_SetTitleText(   
                    		MMI_CTRL_ID_T		ctrl_id,
                    		MMI_TEXT_ID_T       title_text_id
	                        );


/***************************************************************************//*!
@brief  ����title�߶Ⱥ���ɫ
@author Lin.Lin
@param      ctrl_id  [in]  �ؼ�id
@param      height  [in]   title�߶�
@param     color [in]      title��ɫ
@return void 
@note   ����title�ĸߺ���ɫ����
*******************************************************************************/
void CTRLMSGBOX_SetTitleParam(   
                    		 MMI_CTRL_ID_T		ctrl_id,
                    		 uint8               height,   
                    		 GUI_COLOR_T         color   
	                         );


/***************************************************************************//*!
@brief  ������Ϣ��������������ɫ
@author lin.lin
@param      ctrl_id	[in]	  �ؼ�id
@param      font	[in]	  ��������
@param    font_color   [in]   ������ɫ
@param     space_char	[in]   �ּ�ո�
@param      space_line	[in]  �пո�
@param      move_line	[in]  ÿ��ָ��ʱ���ڹ�������
@param     back_color	[in]   �ı���ʾ����ı�����ɫ
@return void 
@note   ������Ϣ��������ĸ�������(��ɫ���ո��)
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
@brief  ����title��ͼƬ���ı�����ʾ����
@author lin.lin
@param    ctrl_id [in]�ؼ�id
@retval void
@return void
@note   ��Ϣ������ʾͼƬ���ı��Ĳ���
*******************************************************************************/
void CTRLMSGBOX_Layout(MMI_CTRL_ID_T	ctrl_id);


/***************************************************************************//*!
@brief  ������Ϣ��
@author panda.luo
@param  ctrl_id  [in] control id
@retval void
@return void
@note   ˢ����Ϣ��ˢ�º��������ʾ�µ���Ϣ��
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
