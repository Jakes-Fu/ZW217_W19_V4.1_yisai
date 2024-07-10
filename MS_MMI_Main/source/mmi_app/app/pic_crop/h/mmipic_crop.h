/*****************************************************************************
** File Name:      mmipic_crop.h                                                  *
** Author:           chunyou                                                          *
** Date:             12/25/2011                                                 *
** Copyright:      2011 preadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmipic_crop.c  mmipic_crop_wintab.c                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011       chunyou           Create
******************************************************************************/

#ifdef MMI_PIC_CROP_SUPPORT
#ifndef _MMIPIC_CROP_H_
#define _MMIPIC_CROP_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmifmm_export.h"
#include "pe_process.h"
#include "guiimg.h"
#include "sysTypes.h"
#include "block_mem.h"
#include "guifont.h"
#include "guianim.h"
#include "mmipic_crop_export.h"
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *

**--------------------------------------------------------------------------*/

//#ifndef MMIPIC_ASSERT_MODE
//#define MMIPIC_ASSERT_MODE
//#endif

#ifdef MMIPICCROP_ASSERT_MODE
    #define MMIPICCROP_ASSERT_WARNING    MMI_ASSERT_WARNING
#else
    #define MMIPICCROP_ASSERT_WARNING    SCI_TRACE_LOW
#endif

#define MMIPICCROP_DOT                         MMIFILE_DOT
#define MMIPICCROP_SAVE_FILE_TYPE           "bmp"
#define MMIPICCROP_SAVE_SUBFIX           (g_mmipiccrop_save_subfix_wstr)//"\x00\x2e\x00\x6a\x00\x70\x00\x67"      // pic�ļ���׺��.jpg
#define MMIPICCROP_SAVE_SUBFIX_LEN        4

#define MMIPICCROP_PROCESS_BUFFER_FIXED_SIZE   (100*1024)

#define MMIPICCROP_MAX_WIDTH                       480
#define MMIPICCROP_MAX_HEIGHT                      640

#define MMIPICCROP_TITLE_HEIGHT                   0
#define MMIPICCROP_BOTTOM_HEIGHT                44
//#define MMIPICCROP_BUTTON_SPACE                  10

#define MMIPICCROP_MOVE_STEP_LEN                 1
#define MMIPICCROP_MOVE_POINT_OFFSET         10

#define MMIPICCROP_BG_COLOR                           MMI_BLACK_COLOR
#define MMIPICCROP_TITLE_BG_COLOR                MMI_BLUE_COLOR //144, 72, 156
#define MMIPICCROP_FILENAME_FONT                 CAF_FONT_16



/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef GUIANIM_FILE_INFO_T PICCROP_EDIT_FILE_INFO_T;

typedef enum
{
    MMI_PICCROP_FILE_GET_ERR_NONE=0,
    MMI_PICCROP_FILE_OVERSIZE,
    MMI_PICCROP_FILE_ASY_READ_ERR,
    MMI_PICCROP_FILE_GET_ERROR_MAX
} MMI_PICCROP_FILE_GET_ERR_E;

typedef enum
{
    MMIPIC_CROP_TP_NONE = 0,
    MMIPIC_CROP_LEFT_TOP,
    MMIPIC_CROP_RIGHT_TOP,
    MMIPIC_CROP_LEFT_BOTTOM,
    MMIPIC_CROP_RIGHT_BOTTOM,
    MMIPIC_CROP_RECT_MIDDLE,
    MMIPIC_CROP_TP_RECT_MAX
}MMIPICCROP_TP_RECT_E;

typedef struct 
{
    uint32              crop_file_id;
}MMIPICCROP_SETTING_INFO_T;

typedef struct 
{
    MMIPICCROP_TYPE_E crop_type;    //�ü�����
    GUI_RECT_T      crop_frame_rect;    //�ü�ʱ��ʾ�Ŀ������
    uint16 frame_img_width;     //�ü������ܵ�Բ���
    uint16 frame_img_height;    //�ü������ܵ�Բ���    
    //wchar* selected_folder_ptr;
    wchar* random_name_ptr;     //������ļ���ָ��
    uint16 random_name_len;     //������ļ�������
    //uint32 edit_buffer_size;
    //uint32 process_buffer_size;
    GUI_LCD_DEV_INFO            lcd_dev;            //lcd dev
    PICCROP_EDIT_FILE_INFO_T      file_info;            //�ļ���Ϣ
    GUIIMG_BITMAP_T                  *display_bitmap_ptr;              //��ǰ��ʾbufָ��
    GUIIMG_BITMAP_T                  *crop_bitmap_ptr;      //�ü���buf
    uint32      crop_bitmap_size;       //�ü���buf ��С
    GUI_RECT_T                     display_rect;                    //display rect
    uint8                             *process_buf_ptr;                      //�м� ����ָ��
    //uint8                           *output_buf_ptr;                //�༭������bufָ��
    //uint8                           *jpg_buf_ptr; 
    //uint32 output_buffer_size;
    //uint32 jpg_buffer_size;
    uint32 display_buffer_size;     //��ʾ��buf��С
    //uint16 contact_img_min_width;
    uint16 scale_width;     //�ü�������
    uint16 scale_height;       //�ü�������
    GUIIMG_INFO_T original_img;     //ԭͼ��Ϣ
    MMIPICCROP_CALLBACK callback_fun;   //�ü���ص�����
    MMIPICCROP_CB_T callback_t; //�ص��Ľṹ
    BOOLEAN is_file;    //�Ƿ���������ļ�����trueΪ�ļ�����falseΪbuffer
    uint8       *src_buf_ptr;       /*!< ͼ��buffer��ַ @��Ϊpnullʱ��Ч */
    uint32      src_data_size;      /*!< ͼ��buffer��С @src_buf_ptr��Ϊpnull src_data_size��Ϊ0ʱ��Ч */
    uint16      min_width;                /*!< �ü���ͼƬ����С��ߣ�Ϊ0��ʾ������ */
    uint16      min_height;               /*!< �ü���ͼƬ����С��ߣ�Ϊ0��ʾ������ */
    BOOLEAN      is_pre_wallpaper; //�Ƿ��ǻָ�ǰһ��ǽֽ
    MMIPICCROP_SETTING_INFO_T setting_info;     //������Ϣ
} MMIPICCROP_INFO_T;

extern const wchar g_mmipiccrop_save_subfix_wstr[];
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/                                              
/*****************************************************************************/
//  Description : open pic crop 
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICCROP_OpenPicEditorMainWin(
                                           MMIPICCROP_SRC_T* crop_src     //in:
                                           );

/*****************************************************************************/
//  Description : get crop info ptr
//  Global resource dependence : s_crop_info_ptr
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC MMIPICCROP_INFO_T* MMIPICCROP_GetCropInfoPtr(void);

/*****************************************************************************/
//  Description : init pic crop
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC MMI_PICCROP_ERROR_TYPE_E MMIPICCROP_Init (MMIPICCROP_SRC_T* crop_src);

/*****************************************************************************/
//  Description : Decode jpg image
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC MMI_PICCROP_ERROR_TYPE_E MMIPICCROP_DecodeJpgImage(
                                     void
                                     );

/*****************************************************************************/
//  Description : display image
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPICCROP_DisplayImage(MMI_MESSAGE_ID_E    msg_id);

/*****************************************************************************/
//  Description : pic crop exit
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPICCROP_Exit(void);

/*****************************************************************************/
//  Description : Malloc  PROCESS TARGET  Mem
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIPICCROP_MallocMemForPicProcess(void);

/*****************************************************************************/
//  Description : Register pic menu group
//  Global resource dependence : none
//  Author: CHUNYOU
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICCROP_RegMenuGroup(void);

/*****************************************************************************/
//  Description : get crop client rect
//  Global resource dependence : 
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIPICCROP_GetClientRect(void);

/*****************************************************************************/
//  Description : get crop client rect height
//  Global resource dependence : 
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPICCROP_GetClientRectHeight(void);

/*****************************************************************************/
//  Description : handle touch pen
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPICCROP_HandleTpMsg(GUI_POINT_T point,MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description : is permit crop
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICCROP_IsPermitCrop(void);

/*****************************************************************************/
//  Description : get crop image data
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC MMI_PICCROP_ERROR_TYPE_E MMIPICCROP_GetCropImageData(void);

/*****************************************************************************/
//  Description : save image
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC MMI_PICCROP_ERROR_TYPE_E  MMIPICCROP_Save(void);

/*****************************************************************************/
//  Description : get file latter name to combine full file name
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
LOCAL void  MMIPICCROP_GetSaveLatterName(MMIPICCROP_TYPE_E crop_type);

/*****************************************************************************/
//  Description : show error or normal message
//  Global resource dependence : 
//  Author:chunyou
//  Note: 
//  Param 
//  nput:  pic_error   value of MMI_PICCROP_ERROR_TYPE_E type
/*****************************************************************************/
PUBLIC void  MMIPICCROP_ShowMsg(MMI_PICCROP_ERROR_TYPE_E pic_error);

/*****************************************************************************/
//  Description : exit from pic crop
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICCROP_ExitFromPicCrop(void);

/*****************************************************************************/
//  Description : error callback
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPICCROP_ErrorCallback(MMI_PICCROP_ERROR_TYPE_E err_type);
#endif
#endif

