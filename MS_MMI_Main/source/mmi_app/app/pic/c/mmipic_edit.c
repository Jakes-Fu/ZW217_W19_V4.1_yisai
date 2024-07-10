/*****************************************************************************
** File Name:      mmipic_edit.c                                                  *
** Author:                                                                   *
** Date:           01/10/2008                                                *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe pic editor                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2008       Vincent.shang       Create                                  *
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_pic_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef PIC_EDITOR_SUPPORT
#include "os_api.h"
#include "mmipic_edit.h"
#include "mmifmm_id.h"
#include "mmifmm_export.h"
#include "mmidisplay_data.h"
#include "mmipic_id.h"
#include "mmipic_text.h"
#include "mmipic_image.h"
#include "mmi_position.h"
//#include "mmi_id.h"
#include "guianim.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmipub.h"
#include "mmitheme_anim.h"
#include "mmi_appmsg.h"
#include "types.h"
#include "stdio.h"
#include "guiimg.h"
#include "guistring.h"
#include "guires.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define  ROTATE_ICON_NUMBER      MMI_ROTATION_DEGREE_MAX
#define MMIPIC_ICON_BG_COLOR              0xc6df  //icon bg color
#define MMIPIC_MIN_UNIT       8


//#define MMIPIC_FILEDATA_BUFFER_SIZE MMIPIC_PROCESS_BUFFER_SIZE  // (MMI_MAINSCREEN_WIDTH*MMI_MAINSCREEN_HEIGHT*4) to width*height*3+72K

#define LOOP_565_TO_555(_565)                       \
{                                                       \
    uint32 _555 = 0;                                    \
    _555 = (((_565) >> 1) & 0x7FE0) | ((_565) & 0x001F);\
    (_565) = _555;                                      \
}

/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                 *
**--------------------------------------------------------------------------*/
LOCAL  PIC_EDIT_PARAM_T               s_pic_editor_info={0};
LOCAL  PIC_EDIT_ROTATE_ICON_INFO_T    s_icon_image[ROTATE_ICON_NUMBER]={0};


static PEPROCESS_HUE_DEGREE_E s_degree=PEPROCESS_HUE_DEGREE_0;
static int16 s_constract_gain=0;
static int16 s_saturation_gain=0;
static int32 s_bright_offset=0;

static int32 s_R_offset=0;
static int32 s_G_offset=0;
static int32 s_B_offset=0;

LOCAL uint32 s_edit_buffer_size = 0;
LOCAL uint32 s_process_buffer_size = 0;
LOCAL uint32 s_filedata_buffer_size = 0;
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern wchar* g_random_name_ptr;

uint16 g_random_name_len = 0;
BOOLEAN g_can_be_default_saved = TRUE;

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
const wchar g_mmipic_save_subfix_wstr[] = { '.', 'j', 'p', 'g', 0 };//MMIPIC_SAVE_SUBFIX      "\x00\x2e\x00\x6a\x00\x70\x00\x67"      // pic文件后缀名.jpg
const wchar g_mmipic_miniature_path_wstr[] = { '_', '0', '1' ,0 }; //MMIPIC_MINIATURE_PATH              "\x00\x5f\x00\x30\x00\x31"  //_01

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get picture file size
//  Global resource dependence : none
//  Author: vincent.shang
//  Note:
/*****************************************************************************/
LOCAL uint32 PicGetPicFileSize( const wchar    *full_path_ptr);

// /*****************************************************************************/
// //  Description : get file latter name to combine full file name
// //  Global resource dependence : 
// //  Author: Robert.wang
// //  Note: 
// /*****************************************************************************/
// LOCAL void  MMIPIC_GetSaveLatterName(void);

/*****************************************************************************/
//  Description : get random number
//  Global resource dependence : 
//  Author: Robert.wang
//  Note: 
/*****************************************************************************/
LOCAL uint16  MMIPIC_GetRandomNum(void);


/*****************************************************************************/
//  Description : 文本风格设置
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void GetPicTextStyle(
                           GUISTR_STYLE_T *text_style_ptr,
                           uint16 space,
                           GUI_COLOR_T color,
                           GUI_FONT_T  font,
                           uint8 line_space,
                           GUI_COLOR_T edge_color,
                           GUI_ANGLE_E angle
                           );
                           
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get small(<1M) file data buffer
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC MMI_PIC_FILE_GET_ERR_E MMIPIC_GetCommonFileDataBuf(
                                           PIC_EDIT_FILE_INFO_T    *file_info_ptr  //in
                                           )
{
    BOOLEAN     result = FALSE;
    MMI_PIC_FILE_GET_ERR_E     get_error_type = MMI_PIC_FILE_GET_ERR_NONE;

    uint32 read_max_size=0;
    
    if (PNULL == file_info_ptr)
    {
        get_error_type = MMI_PIC_FILE_GET_ERROR_MAX;
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_GetCommonFileDataBuf file_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_147_112_2_18_2_45_41_0,(uint8*)"");
        return get_error_type;
    }
    
    if (PNULL == file_info_ptr->full_path_wstr_ptr)
    {
        get_error_type = MMI_PIC_FILE_GET_ERROR_MAX;
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_GetCommonFileDataBuf full_path_wstr_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_154_112_2_18_2_45_41_1,(uint8*)"");
        return get_error_type;
    }
    
    if (0 == file_info_ptr->full_path_wstr_len)
    {
        get_error_type = MMI_PIC_FILE_GET_ERROR_MAX;
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_GetCommonFileDataBuf full_path_wstr_len is 0 !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_161_112_2_18_2_45_41_2,(uint8*)"");
        return get_error_type;
    }
    
    read_max_size = s_filedata_buffer_size;
    
    s_pic_editor_info.file_data_size=PicGetPicFileSize(file_info_ptr->full_path_wstr_ptr);
    //判断图片是否过大
    if ((0 < s_pic_editor_info.file_data_size) && 
        (read_max_size > s_pic_editor_info.file_data_size))
    {
        if (PNULL == s_pic_editor_info.file_data_buffer)
        {
            s_pic_editor_info.file_data_buffer = (uint8*)MMI_BL_MALLOC(BLOCK_MEM_POOL_PIC_PROCESS);/*lint !e64*/
        }
        if (PNULL == s_pic_editor_info.file_data_buffer)
        {
            //MMIPIC_ASSERT_WARNING:"[MMIPIC] s_pic_editor_info.file_data_buffer is PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_175_112_2_18_2_45_41_3,(uint8*)"");
            get_error_type = MMI_PIC_FILE_ASY_READ_ERR;
            return get_error_type;
        }

        //read file
        result = MMIAPIFMM_ReadFilesDataAsyn( file_info_ptr->full_path_wstr_ptr,
            file_info_ptr->full_path_wstr_len,
            s_pic_editor_info.file_data_buffer,
            s_pic_editor_info.file_data_size,
            PNULL,
            MMIPIC_EDIT_MAIN_WIN_ID,
            0,
            MSG_PIC_GET_DATA_CNF);
        //SCI_TRACE_LOW:"MMIPIC_GetCommonFileDataBuf: read picture is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_190_112_2_18_2_45_41_4,(uint8*)"d",result);
        if(!result)
        {

            get_error_type = MMI_PIC_FILE_ASY_READ_ERR;

            if (PNULL != s_pic_editor_info.file_data_buffer)
            {
                MMI_BL_FREE(BLOCK_MEM_POOL_PIC_PROCESS);/*lint !e64*/
                s_pic_editor_info.file_data_buffer = PNULL;
            }
        }
    }
    else
    {
        get_error_type = MMI_PIC_FILE_ASY_READ_ERR;

        if(s_pic_editor_info.file_data_size >= read_max_size)
        {
            get_error_type = MMI_PIC_FILE_OVERSIZE;
            MMK_PostMsg(MMIPIC_EDIT_MAIN_WIN_ID,MSG_PIC_GET_DATA_CNF,PNULL,0);          
        }   
    }
    
    return (get_error_type);
}
/*****************************************************************************/
//  Description : get picture file size
//  Global resource dependence : none
//  Author: vincent.shang
//  Note:
/*****************************************************************************/
LOCAL uint32 PicGetPicFileSize(
                               const wchar *full_path_ptr
                               )
{
    uint32      file_size = 0;
    SFS_HANDLE  sfs_handle = 0;
    
    if (PNULL == full_path_ptr)
    {
        file_size = 0;
        //SCI_TRACE_LOW:"[MMIPIC] PicGetPicFileSize full_path_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_230_112_2_18_2_45_41_5,(uint8*)"");
        return file_size;
    }
    
    //open file
    sfs_handle = SFS_CreateFile( full_path_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
    if (0 != sfs_handle)
    {
        //get file size
        if(SFS_ERROR_NONE != SFS_GetFileSize(sfs_handle, &file_size))
        {
            file_size = 0;
        }
    }
    
    //close file
    SFS_CloseFile(sfs_handle);
    
    return (file_size);
}
/*****************************************************************************/
//  Description : Decode jpg image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
//     与函数MMIPIC_GetCommonFileDataBuf（）分配的内存
//      s_filedata_buffer_size 相关
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_DecodeJpgImage(
                                     PIC_EDIT_FILE_INFO_T   *file_info_ptr
                                     )
{
    BOOLEAN             result = FALSE;
    uint32              buf_size = 0;
    GUIIMG_SRC_T        img_src = {0};
    GUIIMG_DISPLAY_T    img_display = {0};
    GUIIMG_DEC_OUT_T    img_output = {0};
    
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;

    GUI_RECT_T      display_rect=MMIPIC_GetClientWinRect(MMIPIC_EDIT_MAIN_WIN_ID);
    uint16          client_height = display_rect.bottom - display_rect.top + 1;
    uint16          des_height = client_height;

#ifdef PIC_EDIT_FRAME_SUPPORT
    MMI_IMAGE_ID_T  image_id = 0;
    if (MMITHEME_IsMainScreenLandscape())
    {
        image_id = IMAGE_PIC_H_FRAME1;
    }
    else
    {
        image_id = IMAGE_PIC_FRAME1;
    }
    
    GUIRES_GetImgWidthHeight(PNULL,&des_height,image_id,MMIPIC_EDIT_MAIN_WIN_ID);
    if(des_height >0)
    {
        des_height = (client_height > des_height)? des_height: client_height;
    }  
#endif    
    if (PNULL == file_info_ptr)
    {
        result = FALSE;
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_DecodeJpgImage file_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_275_112_2_18_2_45_41_6,(uint8*)"");
        return result;
    }
    
    if (PNULL == file_info_ptr->full_path_wstr_ptr)
    {
        result = FALSE;
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_DecodeJpgImage full_path_wstr_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_282_112_2_18_2_45_41_7,(uint8*)"");
        return result;
    }
    
    if (0 == file_info_ptr->full_path_wstr_len)
    {
        result = FALSE;
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_DecodeJpgImage full_path_wstr_len is 0 !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_289_112_2_18_2_45_41_8,(uint8*)"");
        return result;
    }


    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    
    //判断图片是否过大
    if (s_pic_editor_info.file_data_size >= s_filedata_buffer_size ) //MMIMULTIM_FILEDATA_BUFFER_SIZE)
    {
        //set image full path
        img_src.is_file = TRUE;
        img_src.full_path_ptr = file_info_ptr->full_path_wstr_ptr;
    }
    else
    {
        img_src.src_buf_ptr = s_pic_editor_info.file_data_buffer;
        img_src.src_data_size = s_pic_editor_info.file_data_size;
    }
    
    img_display.dest_width = lcd_width;
    img_display.dest_height = (des_height < lcd_height) ? des_height : lcd_height;

    //set image decode output data buffer
    img_output.decode_data_ptr = (uint8*)MMI_BL_MALLOC(BLOCK_MEM_POOL_PIC_UNDO);/*lint !e64*/
    if (PNULL == img_output.decode_data_ptr)
    {
        //MMIPIC_ASSERT_WARNING:"[MMIPIC] img_output.decode_data_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_317_112_2_18_2_45_41_9,(uint8*)"");
        result = FALSE;
        return result;
    }
    img_output.decode_data_size = BL_GetSize(BLOCK_MEM_POOL_PIC_UNDO);/*lint !e64*/
    
    //decode jpg
    result = GUIIMG_Decode(&img_src,
                &img_display,
                &img_output);
    
    if (result)
    {
        do
        {
            //alloc bitmap
            s_pic_editor_info.display_bitmap_ptr = SCI_ALLOCA(sizeof(GUIIMG_BITMAP_T));
            if (PNULL == s_pic_editor_info.display_bitmap_ptr)
            {
                //MMIPIC_ASSERT_WARNING:"[MMIPIC] s_pic_editor_info.display_bitmap_ptr is PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_336_112_2_18_2_45_41_10,(uint8*)"");
                result = FALSE;
                break;
            }
            SCI_MEMSET(s_pic_editor_info.display_bitmap_ptr,0,sizeof(GUIIMG_BITMAP_T));

            //alloc bitmap vpbits
            s_pic_editor_info.display_bitmap_ptr->bit_ptr = (uint8*)MMI_BL_MALLOC(BLOCK_MEM_POOL_PIC_SOURCE);/*lint !e64*/
            if (PNULL == s_pic_editor_info.display_bitmap_ptr->bit_ptr)
            {
                //MMIPIC_ASSERT_WARNING:"[MMIPIC] s_pic_editor_info.display_bitmap_ptr->bit_ptr is PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_346_112_2_18_2_45_41_11,(uint8*)"");
                result = FALSE;
                break;
            }
            buf_size = BL_GetSize(BLOCK_MEM_POOL_PIC_SOURCE);/*lint !e64*/
            SCI_MEMSET(s_pic_editor_info.display_bitmap_ptr->bit_ptr,0,buf_size);

            //copy vpbits
            MMI_MEMCPY(s_pic_editor_info.display_bitmap_ptr->bit_ptr,buf_size,
                img_output.decode_data_ptr,img_output.decode_data_size,
                img_output.decode_data_size);

            //set others param
            s_pic_editor_info.display_bitmap_ptr->img_width = img_output.actual_width;
            s_pic_editor_info.display_bitmap_ptr->img_height = img_output.actual_height;

            s_pic_editor_info.display_rect.left   = (lcd_width - img_output.actual_width)/2;
            s_pic_editor_info.display_rect.top    = display_rect.top + (int16)(client_height - img_output.actual_height)/2;
            s_pic_editor_info.display_rect.right  = s_pic_editor_info.display_rect.left + (int16)img_output.actual_width -1;
            s_pic_editor_info.display_rect.bottom = s_pic_editor_info.display_rect.top + (int16)img_output.actual_height -1;

            MMK_PostMsg(MMIPIC_EDIT_MAIN_WIN_ID,MSG_PIC_DISPLAY_IND,PNULL,0);
        }while(0);
    }

    //free vpbits
    if (PNULL != img_output.decode_data_ptr)
    {
        MMI_BL_FREE(BLOCK_MEM_POOL_PIC_UNDO);/*lint !e64*/
        img_output.decode_data_ptr = PNULL;
    }
    
    //free file data buffer
    if (PNULL != s_pic_editor_info.file_data_buffer)
    {
        MMI_BL_FREE(BLOCK_MEM_POOL_PIC_PROCESS);/*lint !e64*/
        s_pic_editor_info.file_data_buffer = PNULL;
    }

    //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_DecodeJpgImage result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_385_112_2_18_2_45_42_12,(uint8*)"d", result);
    return (result);
}

/*****************************************************************************/
//  Description : init pic editor
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_PicEditorInit (PIC_EDIT_FILE_INFO_T    *file_info_ptr)
{
    if (PNULL == file_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_PicEditorInit file_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_397_112_2_18_2_45_42_13,(uint8*)"");
        return ;
    }
    
    if (PNULL == file_info_ptr->full_path_wstr_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_PicEditorInit full_path_wstr_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_403_112_2_18_2_45_42_14,(uint8*)"");
        return ;
    }
    
    if (0 == file_info_ptr->full_path_wstr_len)
    {
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_PicEditorInit full_path_wstr_len is 0 !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_409_112_2_18_2_45_42_15,(uint8*)"");
        return ;
    }
    
    //MEMCPY拷贝获得RGB565数据，原始图像宽高和缩略图宽高； 
    
    //SCI_TRACE_LOW:"MMIPIC_PicEditorInit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_415_112_2_18_2_45_42_16,(uint8*)"");
    s_pic_editor_info.lcd_dev.lcd_id=GUI_MAIN_LCD_ID;
    s_pic_editor_info.lcd_dev.block_id=GUI_BLOCK_MAIN; 
    
    //copy file info
    SCI_MEMSET(&(s_pic_editor_info.file_info), 0 ,sizeof(PIC_EDIT_FILE_INFO_T));
    MMI_MEMCPY(&(s_pic_editor_info.file_info), sizeof(PIC_EDIT_FILE_INFO_T),
        file_info_ptr, sizeof(PIC_EDIT_FILE_INFO_T), sizeof(PIC_EDIT_FILE_INFO_T));
}

/*****************************************************************************/
//  Description : alloc undo buffer
//  Global resource dependence : 
//  Author: jassmine
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIPIC_AllocUndoBuf(void)
{
    int32 ret = FALSE;
    
    s_pic_editor_info.undo_buf_ptr= (uint8*)MMI_BL_MALLOC(BLOCK_MEM_POOL_PIC_UNDO);/*lint !e64*/ //分配block mem; 
    if (PNULL == s_pic_editor_info.undo_buf_ptr)
    {
        //MMIPIC_ASSERT_WARNING:"[MMIPIC s_pic_editor_info.undo_buf_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_436_112_2_18_2_45_42_17,(uint8*)"");
        return ret;
    }
    ret = TRUE;
    
    return ret;
}

/*****************************************************************************/
//  Description : Malloc  PROCESS TARGET  Mem
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIPIC_MallocMemForPicProcess(void)
{
    int32 ret = FALSE;
    
    s_pic_editor_info.process_buf_ptr= (uint8*)MMI_BL_MALLOC(BLOCK_MEM_POOL_PIC_PROCESS);/*lint !e64*/   //分配block mem; 
    if (PNULL == s_pic_editor_info.process_buf_ptr)
    {
        //MMIPIC_ASSERT_WARNING:"[MMIPIC] s_pic_editor_info.process_buf_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_455_112_2_18_2_45_42_18,(uint8*)"");
        return ret;
    }
    
    s_pic_editor_info.output_buf_ptr= (uint8*)MMI_BL_MALLOC(BLOCK_MEM_POOL_PIC_TARGET);/*lint !e64*/   //分配block mem; 
    if (PNULL == s_pic_editor_info.output_buf_ptr)
    {
        //MMIPIC_ASSERT_WARNING:"[MMIPIC] s_pic_editor_info.output_buf_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_462_112_2_18_2_45_42_19,(uint8*)"");
        return ret;
    }
    ret = TRUE;
    
    return ret;
}
/*****************************************************************************/
//  Description : display image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_PicEditorExit(void)
{

    if(NULL !=s_pic_editor_info.process_buf_ptr)
    {
        MMI_BL_FREE(BLOCK_MEM_POOL_PIC_PROCESS);/*lint !e64*/ //释放block mem;  
        s_pic_editor_info.process_buf_ptr=NULL;
    }
    if(NULL !=s_pic_editor_info.output_buf_ptr)
    {
        MMI_BL_FREE(BLOCK_MEM_POOL_PIC_TARGET);/*lint !e64*/ //释放block mem; 
        s_pic_editor_info.output_buf_ptr=NULL;
    }
    if(NULL !=s_pic_editor_info.undo_buf_ptr)
    {
        MMI_BL_FREE(BLOCK_MEM_POOL_PIC_UNDO);/*lint !e64*/ //释放block mem;
        s_pic_editor_info.undo_buf_ptr=NULL;
    }
    if(NULL !=s_pic_editor_info.display_bitmap_ptr)
    {
        if(NULL !=s_pic_editor_info.display_bitmap_ptr->bit_ptr)
        {
            MMI_BL_FREE(BLOCK_MEM_POOL_PIC_SOURCE);/*lint !e64*/ //释放block mem
            s_pic_editor_info.display_bitmap_ptr->bit_ptr=NULL;
        }
        SCI_FREE(s_pic_editor_info.display_bitmap_ptr);
        s_pic_editor_info.display_bitmap_ptr=NULL;
    }
    //free file data buffer
    if (PNULL != s_pic_editor_info.file_data_buffer)
    {
        MMI_BL_FREE(BLOCK_MEM_POOL_PIC_PROCESS);/*lint !e64*/
        s_pic_editor_info.file_data_buffer = PNULL;
    }
}

// #ifndef PIC_EDIT_U200_SUPPORT
// /*****************************************************************************/
// //  Description : display image
// //  Global resource dependence : 
// //  Author: vincent
// //  Note: 
// /*****************************************************************************/
// 
// PUBLIC void MMIPIC_DisplayImage(void)
// {
//     GUI_RECT_T      display_rect=MMITHEME_GetClientRectEx(MMIPIC_EDIT_MAIN_WIN_ID);
//     GUI_POINT_T     dis_point = {0};
//     uint16          height_step=0;
//     uint16          width_step=0;
// 
//     uint16  lcd_width = 0;
//     uint16  lcd_height = 0;
// 
//     uint16  client_height = display_rect.bottom - display_rect.top + 1;
// 
//     int32   image_width = s_pic_editor_info.display_bitmap_ptr->img_width;
//     int32   image_height = s_pic_editor_info.display_bitmap_ptr->img_height;
//        
//        if (PNULL == s_pic_editor_info.display_bitmap_ptr)
//        {
//             SCI_TRACE_LOW("[MMIPIC] MMIPIC_DisplayImage display_bitmap_ptr is PNULL !!!");
//             return ;
//        }
//        
//        if (PNULL == s_pic_editor_info.display_bitmap_ptr->bit_ptr)
//        {
//             SCI_TRACE_LOW("[MMIPIC] MMIPIC_DisplayImage display_bitmap_ptr->bit_ptr is PNULL !!!");
//             return ;
//        }
// 
// 
//         height_step = display_rect.top / 2;
//        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
// 
//        
//        if(s_pic_editor_info.display_bitmap_ptr->img_width>lcd_width)
//        {
//            width_step=(s_pic_editor_info.display_bitmap_ptr->img_width-lcd_width)/2;
//        }
//        
//        GUIRES_DisplayImg(PNULL,
//             &display_rect,
//             &display_rect,
//             MMIPIC_EDIT_MAIN_WIN_ID,
//             IMAGE_COMMON_BG,
//             &s_pic_editor_info.lcd_dev);
//        
//        switch(msg_id)
//        {
//        case MSG_APP_UP:
//            if(s_pic_editor_info.display_rect.top < display_rect.top)
//            {
//                s_pic_editor_info.display_rect.top=s_pic_editor_info.display_rect.top+height_step;
//                s_pic_editor_info.display_rect.bottom =s_pic_editor_info.display_rect.bottom + height_step;
// 
//                if (s_pic_editor_info.display_rect.top > display_rect.top)
//                {
//                     s_pic_editor_info.display_rect.top = display_rect.top;
//                     s_pic_editor_info.display_rect.bottom = s_pic_editor_info.display_rect.top + image_height - 1;
//                }
//            }
//            break;
//            
//        case MSG_APP_DOWN:
//            if((s_pic_editor_info.display_bitmap_ptr->img_height>client_height)
//               &&(s_pic_editor_info.display_rect.top + MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT > 0)
//               )
//            {
//                s_pic_editor_info.display_rect.top=s_pic_editor_info.display_rect.top-height_step;
//                s_pic_editor_info.display_rect.bottom =s_pic_editor_info.display_rect.bottom - height_step;
// 
//                if (s_pic_editor_info.display_rect.bottom < display_rect.bottom)
//                {
//                     s_pic_editor_info.display_rect.bottom = display_rect.bottom;
//                     s_pic_editor_info.display_rect.top = s_pic_editor_info.display_rect.bottom - image_height + 1;
//                }
//            }
//            break;
//        case MSG_APP_LEFT:
//            if((s_pic_editor_info.display_bitmap_ptr->img_width>lcd_width)
//                &&(s_pic_editor_info.display_rect.left<0))
//            {
//                s_pic_editor_info.display_rect.left=s_pic_editor_info.display_rect.left+width_step;
//                s_pic_editor_info.display_rect.right = s_pic_editor_info.display_rect.right + width_step;
//                
//                if (s_pic_editor_info.display_rect.left > display_rect.left)
//                {
//                    s_pic_editor_info.display_rect.left = display_rect.left;
//                    s_pic_editor_info.display_rect.right = s_pic_editor_info.display_rect.left + image_width - 1;
//                }
//            }
//            break;
//        case MSG_APP_RIGHT:
//            if((s_pic_editor_info.display_bitmap_ptr->img_width>lcd_width)
//                &&(s_pic_editor_info.display_rect.left+2*width_step>0))
//            {
//                s_pic_editor_info.display_rect.left=s_pic_editor_info.display_rect.left-width_step;
//                s_pic_editor_info.display_rect.right = s_pic_editor_info.display_rect.right - width_step;
// 
//                if (s_pic_editor_info.display_rect.right < display_rect.right)
//                {
//                   s_pic_editor_info.display_rect.right = display_rect.right;
//                   s_pic_editor_info.display_rect.left = s_pic_editor_info.display_rect.right - image_width + 1;
//                }
//            }
//            break;
//        case MSG_PIC_DISPLAY_IND:
//            
//            break;
//            
//        default:
//            
//            break;
//        }
//        
//        dis_point.x = s_pic_editor_info.display_rect.left;
//        dis_point.y = s_pic_editor_info.display_rect.top;
//        GUIIMG_DisplayBmp(FALSE,
//             &display_rect,
//             &dis_point,
//             s_pic_editor_info.display_bitmap_ptr,
//             &s_pic_editor_info.lcd_dev);
// }
// 
// #else
/*****************************************************************************/
//  Description : display image
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/

PUBLIC void MMIPIC_DisplayImage(void)
{
    GUI_RECT_T      display_rect = MMITHEME_GetFullScreenRect();
    GUI_POINT_T     dis_point = {0};

       GUIRES_DisplayImg(PNULL,
            &display_rect,
            &display_rect,
            MMIPIC_EDIT_MAIN_WIN_ID,
            IMAG_PS_BG,
            &s_pic_editor_info.lcd_dev);

       if (PNULL == s_pic_editor_info.display_bitmap_ptr)
       {
            //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_DisplayImage display_bitmap_ptr is PNULL !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_645_112_2_18_2_45_42_20,(uint8*)"");
            return ;
       }
       
       if (PNULL == s_pic_editor_info.display_bitmap_ptr->bit_ptr)
       {
            //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_DisplayImage display_bitmap_ptr->bit_ptr is PNULL !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_651_112_2_18_2_45_42_21,(uint8*)"");
            return ;
       }
       //LCD_FillRect(&s_pic_editor_info.lcd_dev, display_rect, MMI_BLACK_COLOR);
       
       dis_point.x = s_pic_editor_info.display_rect.left;
       dis_point.y = s_pic_editor_info.display_rect.top;
       GUIIMG_DisplayBmp(FALSE,
            &display_rect,
            &dis_point,
            s_pic_editor_info.display_bitmap_ptr,
            &s_pic_editor_info.lcd_dev);
}

// /*****************************************************************************/
// //  Description : change the positon of display image by msg_id
// //  Global resource dependence : 
// //  Author: yaye.jiang
// //  Note: 
// /*****************************************************************************/
// PUBLIC void MMIPIC_MoveDisplayImage(MMI_MESSAGE_ID_E    msg_id)
// {
//     GUI_RECT_T      display_rect=MMITHEME_GetClientRectEx(MMIPIC_EDIT_MAIN_WIN_ID);
//     GUI_POINT_T     dis_point = {0};
//     uint16          height_step=0;
//     uint16          width_step=0;
// 
//     uint16  lcd_width = 0;
//     uint16  lcd_height = 0;
// 
//     uint16  client_height = MMITHEME_GetClientRectHeightEx(MMIPIC_EDIT_MAIN_WIN_ID);
// 
//     int32   image_width = s_pic_editor_info.display_bitmap_ptr->img_width;
//     int32   image_height = s_pic_editor_info.display_bitmap_ptr->img_height;
// 
//        if (PNULL == s_pic_editor_info.display_bitmap_ptr)
//        {
//             SCI_TRACE_LOW("[MMIPIC] MMIPIC_MoveDisplayImage display_bitmap_ptr is PNULL !!!");
//             return ;
//        }
//        
//        if (PNULL == s_pic_editor_info.display_bitmap_ptr->bit_ptr)
//        {
//             SCI_TRACE_LOW("[MMIPIC] MMIPIC_MoveDisplayImage display_bitmap_ptr->bit_ptr is PNULL !!!");
//             return ;
//        }
// 
// 
//         height_step = display_rect.top / 2;
//        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
// 
//        
//        if(s_pic_editor_info.display_bitmap_ptr->img_width>lcd_width)
//        {
//            width_step=(s_pic_editor_info.display_bitmap_ptr->img_width-lcd_width)/2;
//        }
//        
//        switch(msg_id)
//        {
//        case MSG_APP_UP:
//            if(s_pic_editor_info.display_rect.top < display_rect.top)
//            {
//                s_pic_editor_info.display_rect.top=s_pic_editor_info.display_rect.top+height_step;
//                s_pic_editor_info.display_rect.bottom =s_pic_editor_info.display_rect.bottom + height_step;
// 
//                if (s_pic_editor_info.display_rect.top > display_rect.top)
//                {
//                     s_pic_editor_info.display_rect.top = display_rect.top;
//                     s_pic_editor_info.display_rect.bottom = s_pic_editor_info.display_rect.top + image_height - 1;
//                }
//            }
//            break;
//            
//        case MSG_APP_DOWN:
//            if((s_pic_editor_info.display_bitmap_ptr->img_height>client_height)
//               &&(s_pic_editor_info.display_rect.top + MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT > 0)
//               )
//            {
//                s_pic_editor_info.display_rect.top=s_pic_editor_info.display_rect.top-height_step;
//                s_pic_editor_info.display_rect.bottom =s_pic_editor_info.display_rect.bottom - height_step;
// 
//                if (s_pic_editor_info.display_rect.bottom < display_rect.bottom)
//                {
//                     s_pic_editor_info.display_rect.bottom = display_rect.bottom;
//                     s_pic_editor_info.display_rect.top = s_pic_editor_info.display_rect.bottom - image_height + 1;
//                }
//            }
//            break;
//        case MSG_APP_LEFT:
//            if((s_pic_editor_info.display_bitmap_ptr->img_width>lcd_width)
//                &&(s_pic_editor_info.display_rect.left<0))
//            {
//                s_pic_editor_info.display_rect.left=s_pic_editor_info.display_rect.left+width_step;
//                s_pic_editor_info.display_rect.right = s_pic_editor_info.display_rect.right + width_step;
//                
//                if (s_pic_editor_info.display_rect.left > display_rect.left)
//                {
//                    s_pic_editor_info.display_rect.left = display_rect.left;
//                    s_pic_editor_info.display_rect.right = s_pic_editor_info.display_rect.left + image_width - 1;
//                }
//            }
//            break;
//        case MSG_APP_RIGHT:
//            if((s_pic_editor_info.display_bitmap_ptr->img_width>lcd_width)
//                &&(s_pic_editor_info.display_rect.left+2*width_step>0))
//            {
//                s_pic_editor_info.display_rect.left=s_pic_editor_info.display_rect.left-width_step;
//                s_pic_editor_info.display_rect.right = s_pic_editor_info.display_rect.right - width_step;
// 
//                if (s_pic_editor_info.display_rect.right < display_rect.right)
//                {
//                   s_pic_editor_info.display_rect.right = display_rect.right;
//                   s_pic_editor_info.display_rect.left = s_pic_editor_info.display_rect.right - image_width + 1;
//                }
//            }
//            break;
//        case MSG_PIC_DISPLAY_IND:
//            
//            break;
//            
//        default:
//            
//            break;
//        }       
// }

// #endif
/*****************************************************************************/
//  Description : copy original image  data  to  output image data
//  Global resource dependence : 
//  Author: robert.wang
//  Date: 09-10-26
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_CopyOrgImgDataToOutputImgData(void)
{
    //picture  width and height
    s_pic_editor_info.output_pic_width = s_pic_editor_info.display_bitmap_ptr->img_width;
    s_pic_editor_info.output_pic_height = s_pic_editor_info.display_bitmap_ptr->img_height;

    // copy display  rect
    MMI_MEMCPY(&s_pic_editor_info.output_display_rect,sizeof(s_pic_editor_info.output_display_rect),\
         &s_pic_editor_info.display_rect,sizeof(s_pic_editor_info.display_rect),sizeof(GUI_RECT_T));

   // copy picture  bitmap data to  output  data
    MMI_MEMCPY(s_pic_editor_info.output_buf_ptr, s_edit_buffer_size,
        s_pic_editor_info.display_bitmap_ptr->bit_ptr, s_edit_buffer_size, s_edit_buffer_size);
}

#if 0
// #ifndef PIC_EDIT_U200_SUPPORT
// /*****************************************************************************/
// //  Description : display output image
// //  Global resource dependence : 
// //  Author: robert.wang
// //  Date: 09-10-26
// //  Note: 
// //      显示处理后的图片
// /*****************************************************************************/
// 
// PUBLIC void MMIPIC_DisplayOutputImage(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id)
// {
//     GUI_RECT_T        display_rect=MMITHEME_GetClientRectEx(win_id);
//     GUI_POINT_T       dis_point = {0};
//     uint16            height_step=0;
//     uint16            width_step=0;
//     GUIIMG_BITMAP_T   output_bmp; //added, @robert.wang, 09-10-26, cr158180
// 
//     uint16  lcd_width = 0;
//     uint16  lcd_height = 0;
// 
//     uint16 client_height = display_rect.bottom - display_rect.top + 1;
// 
//     int32  image_width = 0;
//     int32  image_height = 0;
//        
//        if (PNULL == s_pic_editor_info.display_bitmap_ptr)
//        {
//             SCI_TRACE_LOW("[MMIPIC] MMIPIC_DisplayOutputImage display_bitmap_ptr is PNULL !!!");
//             return ;
//        }
//        
//        if (PNULL == s_pic_editor_info.output_buf_ptr)
//        {
//             SCI_TRACE_LOW("[MMIPIC] MMIPIC_DisplayOutputImage output_buf_ptr is PNULL !!!");
//             return ;
//        }
// 
// 
//         image_width = s_pic_editor_info.output_pic_width;
//         image_height = s_pic_editor_info.output_pic_height;
// 
// 
//        height_step=display_rect.top / 2;
//        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
// 
//        if(s_pic_editor_info.output_pic_width>lcd_width)
//        {
//            width_step=(s_pic_editor_info.output_pic_width-lcd_width)/2;
//        }
//        
//        GUIRES_DisplayImg(PNULL,
//             &display_rect,
//             &display_rect,
//             MMIPIC_EDIT_MAIN_WIN_ID,
//             IMAGE_COMMON_BG,
//             &s_pic_editor_info.lcd_dev);
//        
//        switch(msg_id)
//        {
//        case MSG_APP_UP:
//            if(s_pic_editor_info.output_display_rect.top < display_rect.top)
//            {
//                s_pic_editor_info.output_display_rect.top=s_pic_editor_info.output_display_rect.top+height_step;
//                s_pic_editor_info.output_display_rect.bottom = s_pic_editor_info.output_display_rect.bottom + height_step;
//                if (s_pic_editor_info.output_display_rect.top > display_rect.top)
//                {
//                    s_pic_editor_info.output_display_rect.top = display_rect.top;
//                    s_pic_editor_info.output_display_rect.bottom = s_pic_editor_info.output_display_rect.top + image_height - 1;
// 
//                }
//            }
//            break;
//            
//        case MSG_APP_DOWN:
//            if((s_pic_editor_info.output_pic_height > client_height)
//               &&(s_pic_editor_info.output_display_rect.top + MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT > 0)
//               )
//            {
//                s_pic_editor_info.output_display_rect.top=s_pic_editor_info.output_display_rect.top-height_step;
//                s_pic_editor_info.output_display_rect.bottom = s_pic_editor_info.output_display_rect.bottom - height_step;
// 
//                if (s_pic_editor_info.output_display_rect.bottom < display_rect.bottom)
//                {
//                     s_pic_editor_info.output_display_rect.bottom = display_rect.bottom;
//                     s_pic_editor_info.output_display_rect.top = s_pic_editor_info.output_display_rect.bottom - image_height + 1;
//                }
//            }
//            break;
//        case MSG_APP_LEFT:
//            if((s_pic_editor_info.output_pic_width>lcd_width)
//                &&(s_pic_editor_info.output_display_rect.left<0))
//            {
//                s_pic_editor_info.output_display_rect.left=s_pic_editor_info.output_display_rect.left+width_step;
//                s_pic_editor_info.output_display_rect.right = s_pic_editor_info.output_display_rect.right + width_step;
// 
//                if (s_pic_editor_info.output_display_rect.left > display_rect.left)
//                {
//                     s_pic_editor_info.output_display_rect.left = display_rect.left;
//                     s_pic_editor_info.output_display_rect.right = s_pic_editor_info.output_display_rect.left + image_width - 1;
//                }
//            }
//            break;
//        case MSG_APP_RIGHT:
//            if((s_pic_editor_info.output_pic_width>lcd_width)
//                &&(s_pic_editor_info.output_display_rect.left+2*width_step>0))
//            {
//                s_pic_editor_info.output_display_rect.left=s_pic_editor_info.output_display_rect.left-width_step;
//                s_pic_editor_info.output_display_rect.right = s_pic_editor_info.output_display_rect.right - width_step;
// 
//                if (s_pic_editor_info.output_display_rect.right < display_rect.right)
//                {
//                     s_pic_editor_info.output_display_rect.right = display_rect.right;
//                     s_pic_editor_info.output_display_rect.left = s_pic_editor_info.output_display_rect.right - image_width + 1;
//                }
//            }
//            break;
//        case MSG_PIC_DISPLAY_IND:
//            
//            break;
//            
//        default:
//            
//            break;
//        }
//        
//         // copy picture  bitmap  attribute   to  temp  output bitmap attribute
//         MMI_MEMCPY(&output_bmp,sizeof(output_bmp),\
//               s_pic_editor_info.display_bitmap_ptr,sizeof(*s_pic_editor_info.display_bitmap_ptr),sizeof(output_bmp));
// 
//         output_bmp.bit_ptr = s_pic_editor_info.output_buf_ptr ;
//         output_bmp.img_width = s_pic_editor_info.output_pic_width;
//         output_bmp.img_height = s_pic_editor_info.output_pic_height;
//        
//         dis_point.x = s_pic_editor_info.output_display_rect.left;
//         dis_point.y = s_pic_editor_info.output_display_rect.top;
//         GUIIMG_DisplayBmp(FALSE,
//             &display_rect,
//             &dis_point,
//             &output_bmp,
//             &s_pic_editor_info.lcd_dev);
// }
// #else
#endif
/*****************************************************************************/
//  Description : display output image
//  Global resource dependence : 
//  Author: yaye.jiang
//  Date: 
//  Note: 
//      显示图片
/*****************************************************************************/

PUBLIC void MMIPIC_DisplayOutputImage(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id)
{
    GUI_RECT_T        display_rect=MMITHEME_GetFullScreenRect();
    GUI_POINT_T       dis_point = {0};
    GUIIMG_BITMAP_T   output_bmp; //added, @robert.wang, 09-10-26, cr158180
    uint16            client_height = display_rect.bottom - display_rect.top + 1;
       
       if (PNULL == s_pic_editor_info.display_bitmap_ptr)
       {
            //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_DisplayOutputImage display_bitmap_ptr is PNULL !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_966_112_2_18_2_45_43_22,(uint8*)"");
            return ;
       }
       
       if (PNULL == s_pic_editor_info.output_buf_ptr)
       {
            //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_DisplayOutputImage output_buf_ptr is PNULL !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_972_112_2_18_2_45_43_23,(uint8*)"");
            return ;
       }

       GUIRES_DisplayImg(PNULL,
            &display_rect,
            &display_rect,
            MMIPIC_EDIT_MAIN_WIN_ID,
            IMAG_PS_BG,
            &s_pic_editor_info.lcd_dev);
       
       //LCD_FillRect(&s_pic_editor_info.lcd_dev, display_rect, MMI_BLACK_COLOR);
              
        // copy picture  bitmap  attribute   to  temp  output bitmap attribute
        MMI_MEMCPY(&output_bmp,sizeof(output_bmp),\
              s_pic_editor_info.display_bitmap_ptr,sizeof(*s_pic_editor_info.display_bitmap_ptr),sizeof(output_bmp));

        output_bmp.bit_ptr = s_pic_editor_info.output_buf_ptr ;
        output_bmp.img_width = s_pic_editor_info.output_pic_width;
        output_bmp.img_height = s_pic_editor_info.output_pic_height;
       
        dis_point.x = s_pic_editor_info.output_display_rect.left;
        dis_point.y = s_pic_editor_info.output_display_rect.top;
        GUIIMG_DisplayBmp(FALSE,
            &display_rect,
            &dis_point,
            &output_bmp,
            &s_pic_editor_info.lcd_dev);
}

//用不上了,因为decode 图片的时候,将范围限制在能显示的区域了
// /*****************************************************************************/
// //  Description : change the position of output image
// //  Global resource dependence : 
// //  Author: yaye.jiang
// //  Date: 11-12-07
// //  Note: 
// //      处理图片
// /*****************************************************************************/
// PUBLIC void MMIPIC_MoveOutputImage(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id)
// {
//     GUI_RECT_T        display_rect=MMITHEME_GetClientRectEx(win_id);
//     GUI_POINT_T       dis_point = {0};
//     uint16            height_step=0;
//     uint16            width_step=0;
//     GUIIMG_BITMAP_T   output_bmp; //added, @robert.wang, 09-10-26, cr158180
// 
//     uint16  lcd_width = 0;
//     uint16  lcd_height = 0;
// 
//     uint16 client_height = MMITHEME_GetClientRectHeightEx(win_id);
// 
//     int32  image_width = s_pic_editor_info.output_pic_width;
//     int32  image_height = s_pic_editor_info.output_pic_height;
//     
//     height_step=display_rect.top / 2;
//     GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
//     
//     if(s_pic_editor_info.output_pic_width>lcd_width)
//     {
//         width_step=(s_pic_editor_info.output_pic_width-lcd_width)/2;
//     }
//     
//     switch(msg_id)
//     {
//     case MSG_APP_UP:
//         if(s_pic_editor_info.output_display_rect.top < display_rect.top)
//         {
//             s_pic_editor_info.output_display_rect.top=s_pic_editor_info.output_display_rect.top+height_step;
//             s_pic_editor_info.output_display_rect.bottom = s_pic_editor_info.output_display_rect.bottom + height_step;
//             if (s_pic_editor_info.output_display_rect.top > display_rect.top)
//             {
//                 s_pic_editor_info.output_display_rect.top = display_rect.top;
//                 s_pic_editor_info.output_display_rect.bottom = s_pic_editor_info.output_display_rect.top + image_height - 1;
//     
//             }
//         }
//         break;
//         
//     case MSG_APP_DOWN:
//         if((s_pic_editor_info.output_pic_height > client_height)
//            &&(s_pic_editor_info.output_display_rect.top + MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT > 0)
//            )
//         {
//             s_pic_editor_info.output_display_rect.top=s_pic_editor_info.output_display_rect.top-height_step;
//             s_pic_editor_info.output_display_rect.bottom = s_pic_editor_info.output_display_rect.bottom - height_step;
//     
//             if (s_pic_editor_info.output_display_rect.bottom < display_rect.bottom)
//             {
//                  s_pic_editor_info.output_display_rect.bottom = display_rect.bottom;
//                  s_pic_editor_info.output_display_rect.top = s_pic_editor_info.output_display_rect.bottom - image_height + 1;
//             }
//         }
//         break;
//     case MSG_APP_LEFT:
//         if((s_pic_editor_info.output_pic_width>lcd_width)
//             &&(s_pic_editor_info.output_display_rect.left<0))
//         {
//             s_pic_editor_info.output_display_rect.left=s_pic_editor_info.output_display_rect.left+width_step;
//             s_pic_editor_info.output_display_rect.right = s_pic_editor_info.output_display_rect.right + width_step;
//     
//             if (s_pic_editor_info.output_display_rect.left > display_rect.left)
//             {
//                  s_pic_editor_info.output_display_rect.left = display_rect.left;
//                  s_pic_editor_info.output_display_rect.right = s_pic_editor_info.output_display_rect.left + image_width - 1;
//             }
//         }
//         break;
//     case MSG_APP_RIGHT:
//         if((s_pic_editor_info.output_pic_width>lcd_width)
//             &&(s_pic_editor_info.output_display_rect.left+2*width_step>0))
//         {
//             s_pic_editor_info.output_display_rect.left=s_pic_editor_info.output_display_rect.left-width_step;
//             s_pic_editor_info.output_display_rect.right = s_pic_editor_info.output_display_rect.right - width_step;
//     
//             if (s_pic_editor_info.output_display_rect.right < display_rect.right)
//             {
//                  s_pic_editor_info.output_display_rect.right = display_rect.right;
//                  s_pic_editor_info.output_display_rect.left = s_pic_editor_info.output_display_rect.right - image_width + 1;
//             }
//         }
//         break;
//     case MSG_PIC_DISPLAY_IND:
//         
//         break;
//         
//     default:
//         
//         break;
//     }
// }
// #endif

/*****************************************************************************/
//  Description : handle touch pen move
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_TouchPenMoveMainImage(GUI_POINT_T point,MMI_MESSAGE_ID_E msg_id)
{
    static int16           relative_x=0;
    static int16           relative_y=0;

    static BOOLEAN         is_tp_press_osd_down=FALSE;
    uint16 height_step = 0;
    uint16 width_step=0;
    GUI_RECT_T        client_rect=MMIPIC_GetClientWinRect(MMIPIC_EDIT_MAIN_WIN_ID);
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;

    int32   image_width = 0;
    int32   image_height = 0;

    height_step=client_rect.top / 2;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    switch(msg_id)
    {
    case MSG_TP_PRESS_DOWN:
        if(GUI_PointIsInRect(point, s_pic_editor_info.display_rect)&&GUI_PointIsInRect(point, client_rect))
        {
            relative_x=point.x-s_pic_editor_info.display_rect.left;
            relative_y=point.y-s_pic_editor_info.display_rect.top;
            is_tp_press_osd_down=TRUE;
        }
        else
        {
            is_tp_press_osd_down=FALSE;
        }
        break;

    case MSG_TP_PRESS_MOVE:
        //protect top and bottom boundary
        if(GUI_PointIsInRect(point, client_rect))
        {
            image_width = s_pic_editor_info.display_bitmap_ptr->img_width;
            image_height = s_pic_editor_info.display_bitmap_ptr->img_height;
            
            if((0<=s_pic_editor_info.display_rect.top+2*height_step)&&(s_pic_editor_info.display_rect.top<=2*height_step))
            {
                
                s_pic_editor_info.display_rect.top=point.y-relative_y;
                if(s_pic_editor_info.display_rect.top>2*height_step)
                {
                    s_pic_editor_info.display_rect.top=2*height_step;
                }
                if(s_pic_editor_info.display_rect.top+2*height_step<0)
                {
                    s_pic_editor_info.display_rect.top=-2*height_step;
                }
                
                s_pic_editor_info.display_rect.bottom = s_pic_editor_info.display_rect.top + image_height - 1;

                if (s_pic_editor_info.display_rect.top > client_rect.top)
                {
                    s_pic_editor_info.display_rect.top = client_rect.top;
                    s_pic_editor_info.display_rect.bottom = s_pic_editor_info.display_rect.top + image_height - 1;
                }

                if (s_pic_editor_info.display_rect.bottom < client_rect.bottom)
                {
                    s_pic_editor_info.display_rect.bottom = client_rect.bottom;
                    s_pic_editor_info.display_rect.top = s_pic_editor_info.display_rect.bottom - image_height + 1;
                }
#ifndef PIC_EDIT_U200_SUPPORT             
                MMIPIC_DisplayImage();
#endif                
            }
            
            
            //left and right protect
            if(s_pic_editor_info.display_bitmap_ptr->img_width>lcd_width)
            {
                width_step=(s_pic_editor_info.display_bitmap_ptr->img_width-lcd_width)/2;
                s_pic_editor_info.display_rect.left=point.x-relative_x;
                if(s_pic_editor_info.display_rect.left>0)
                {
                    s_pic_editor_info.display_rect.left=0;
                }
                if(s_pic_editor_info.display_rect.left+2*width_step<0)
                {
                    s_pic_editor_info.display_rect.left=-2*width_step;
                }
#ifndef PIC_EDIT_U200_SUPPORT             
                MMIPIC_DisplayImage();
#endif                
            }
            
        }
        break;

    default:
        break;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
}

/*****************************************************************************/
//  Description : handle touch pen move
//  Global resource dependence : 
//  Author: robert.wang
//  Date: 09-10-26
//  Note: 
//        显示OUTPUT 图片
/*****************************************************************************/
PUBLIC void MMIPIC_TouchPenMoveOutputImage(GUI_POINT_T  point,
                                           MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id)
{
    static int16           relative_x=0;
    static int16           relative_y=0;

    static BOOLEAN         is_tp_press_osd_down=FALSE;
    uint16 height_step = 0;
    uint16 width_step=0;
    GUI_RECT_T        client_rect=MMIPIC_GetClientWinRect(win_id);
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;

    int32   image_width = 0;
    int32   image_height = 0;

    height_step = client_rect.top / 2;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    switch(msg_id)
    {
    case MSG_TP_PRESS_DOWN:
        if(GUI_PointIsInRect(point, s_pic_editor_info.output_display_rect)&&GUI_PointIsInRect(point, client_rect))
        {
            relative_x=point.x-s_pic_editor_info.output_display_rect.left;
            relative_y=point.y-s_pic_editor_info.output_display_rect.top;
            is_tp_press_osd_down=TRUE;
        }
        else
        {
            is_tp_press_osd_down=FALSE;
        }
        break;

    case MSG_TP_PRESS_MOVE:
        //protect top and bottom boundary
        if(GUI_PointIsInRect(point, client_rect))
        {
            image_width = s_pic_editor_info.output_pic_width;
            image_height = s_pic_editor_info.output_pic_height;
            
            if((0<=s_pic_editor_info.output_display_rect.top+2*height_step)&&(s_pic_editor_info.output_display_rect.top<=2*height_step))
            {
                
                s_pic_editor_info.output_display_rect.top=point.y-relative_y;
                if(s_pic_editor_info.output_display_rect.top>2*height_step)
                {
                    s_pic_editor_info.output_display_rect.top=2*height_step;
                }
                if(s_pic_editor_info.output_display_rect.top+2*height_step<0)
                {
                    s_pic_editor_info.output_display_rect.top=-2*height_step;
                }
                
                s_pic_editor_info.output_display_rect.bottom = s_pic_editor_info.output_display_rect.top + image_height - 1;

                if (s_pic_editor_info.output_display_rect.top > client_rect.top)
                {
                    s_pic_editor_info.output_display_rect.top = client_rect.top;
                    s_pic_editor_info.output_display_rect.bottom = s_pic_editor_info.output_display_rect.top + image_height - 1;
                }

                if (s_pic_editor_info.output_display_rect.bottom < client_rect.bottom)
                {
                    s_pic_editor_info.output_display_rect.bottom = client_rect.bottom;
                    s_pic_editor_info.output_display_rect.top = s_pic_editor_info.output_display_rect.bottom - image_height + 1;
                }
#ifndef PIC_EDIT_U200_SUPPORT             
                MMIPIC_DisplayOutputImage(win_id, msg_id);
#endif
        }
            
            
            //left and right protect
            if(s_pic_editor_info.output_pic_width>lcd_width)
            {
                width_step=(s_pic_editor_info.output_pic_width -lcd_width)/2;
                s_pic_editor_info.output_display_rect.left=point.x-relative_x;
                if(s_pic_editor_info.output_display_rect.left>0)
                {
                    s_pic_editor_info.output_display_rect.left=0;
                }
                if(s_pic_editor_info.output_display_rect.left+2*width_step<0)
                {
                    s_pic_editor_info.output_display_rect.left=-2*width_step;
                }
#ifndef PIC_EDIT_U200_SUPPORT             
                MMIPIC_DisplayOutputImage(win_id, msg_id);
#endif
            }
            
        }
        break;

    default:
        break;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
}


/*****************************************************************************/
//  Description : undo buf
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/

PUBLIC void  MMIPIC_Undo(void)
{
    SCI_MEMSET(s_pic_editor_info.display_bitmap_ptr->bit_ptr, 0, s_edit_buffer_size);
    MMI_MEMCPY(s_pic_editor_info.display_bitmap_ptr->bit_ptr, s_edit_buffer_size,
        s_pic_editor_info.undo_buf_ptr, s_edit_buffer_size, s_edit_buffer_size);
}

/*****************************************************************************/
//  Description : undo width,heigh,x,y
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_UndoFileInfo(void)
{
    s_pic_editor_info.display_bitmap_ptr->img_width=s_pic_editor_info.undo_pic_width;
    s_pic_editor_info.display_bitmap_ptr->img_height=s_pic_editor_info.undo_pic_heigh;
    
    s_pic_editor_info.display_rect.left =s_pic_editor_info.undo_pic_x_start;
    s_pic_editor_info.display_rect.top=s_pic_editor_info.undo_pic_y_start;
    
    //added, @robert.wang, 09-12-3, cr163593
    s_pic_editor_info.display_rect.right = s_pic_editor_info.display_rect.left + s_pic_editor_info.display_bitmap_ptr->img_width - 1;
    s_pic_editor_info.display_rect.bottom = s_pic_editor_info.display_rect.top + s_pic_editor_info.display_bitmap_ptr->img_height - 1;
    //end cr163593
}

/*****************************************************************************/
//  Description : copy output image data to original image data
//  Global resource dependence : 
//  Author: robert.wang
//  Date: 09-10-26
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_CopyOutputImgDataToOrgImgData(void)
{
    // get width and height
    s_pic_editor_info.display_bitmap_ptr->img_width = s_pic_editor_info.output_pic_width ;
    s_pic_editor_info.display_bitmap_ptr->img_height = s_pic_editor_info.output_pic_height;

    //copy output image data to display image data 
    MMI_MEMCPY(s_pic_editor_info.display_bitmap_ptr->bit_ptr, s_edit_buffer_size,
       s_pic_editor_info.output_buf_ptr, s_edit_buffer_size, s_edit_buffer_size);
}

/*****************************************************************************/
//  Description : copy output image data to original image data
//  Global resource dependence : 
//  Author: robert.wang
//  Date: 09-10-26
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_CopyOutputRectToOrgRect(void)
{
    //copy output rect  to display rect
    MMI_MEMCPY(&s_pic_editor_info.display_rect,sizeof(s_pic_editor_info.display_rect),\
        &s_pic_editor_info.output_display_rect,sizeof(s_pic_editor_info.output_display_rect),\
         sizeof(GUI_RECT_T));
}

/*****************************************************************************/
//  Description : save to undo buf ,save width and height,save x,y
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_SaveToUndoBuf(void)
{
    //SCI_MEMSET(s_pic_editor_info.undo_buf_ptr, 0, s_edit_buffer_size);
    MMI_MEMCPY(s_pic_editor_info.undo_buf_ptr, s_edit_buffer_size,
        s_pic_editor_info.display_bitmap_ptr->bit_ptr, s_edit_buffer_size, s_edit_buffer_size);

    s_pic_editor_info.undo_pic_width=s_pic_editor_info.display_bitmap_ptr->img_width;
    s_pic_editor_info.undo_pic_heigh=s_pic_editor_info.display_bitmap_ptr->img_height;
    
    s_pic_editor_info.undo_pic_x_start=s_pic_editor_info.display_rect.left;
    s_pic_editor_info.undo_pic_y_start=s_pic_editor_info.display_rect.top;
    //SCI_TRACE_LOW:"MMIPIC_SaveToUndoBuf: save buf and width heigh and x,y "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_1388_112_2_18_2_45_44_24,(uint8*)"");

    //added, @robert.wang, 09-10-26, cr158180
    MMIPIC_CopyOutputImgDataToOrgImgData();
    //end cr158180
}

/*****************************************************************************/
//  Description : save to undo buf ,save width and height,save x,y,
//                     not copy output img data to org img data
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_OnlySaveOrgImgToUndoBuf(void)
{
    //SCI_MEMSET(s_pic_editor_info.undo_buf_ptr, 0, s_edit_buffer_size);
    MMI_MEMCPY(s_pic_editor_info.undo_buf_ptr, s_edit_buffer_size,
        s_pic_editor_info.display_bitmap_ptr->bit_ptr, s_edit_buffer_size, s_edit_buffer_size);

    s_pic_editor_info.undo_pic_width=s_pic_editor_info.display_bitmap_ptr->img_width;
    s_pic_editor_info.undo_pic_heigh=s_pic_editor_info.display_bitmap_ptr->img_height;
    
    s_pic_editor_info.undo_pic_x_start=s_pic_editor_info.display_rect.left;
    s_pic_editor_info.undo_pic_y_start=s_pic_editor_info.display_rect.top;

    //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_OnlySaveOrgImgToUndoBuf: save buf and width heigh and x,y "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_1412_112_2_18_2_45_44_25,(uint8*)"");
}

/*****************************************************************************/
//  Description : save image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC MMI_PIC_ERROR_TYPE_E  MMIPIC_Save(BOOLEAN is_overwrite)
{
    PEPROCESS_INPUT_T       input_param={0};
    PEPROCESS_OUTPUT_T      output_param={0};
    PEPROCESS_PARAMETER_T   process_param={0};
    
    //BOOLEAN     is_udisk = FALSE;
    
    MMI_PIC_ERROR_TYPE_E     ret_value = MMI_PIC_SUCCESS; 
    PEPROCESS_RESULT_E ref_return = PEPROCESS_OK;
    
    //added, @robert.wang, 09-10-23, cr158200
    MMI_STRING_T  text_str ; 
    wchar no_suffix_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16 no_suffix_name_len = 0;

    wchar           device_name[MMIFILE_DEVICE_NAME_MAX_LEN+2] = {0};
    uint16          device_name_len = 0;
    wchar           dir_name[MMIFILE_DIR_NAME_MAX_LEN+2] = {0};
    uint16          dir_name_len = 0;
    wchar           new_full_path[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16          new_full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
    wchar           new_file_name_ptr[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16          new_file_name_len = 0;

    BOOLEAN     is_valid_name = TRUE;
    //end cr158200
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
    
    if(g_can_be_default_saved)
    {
        g_can_be_default_saved = FALSE;
// #ifndef PIC_EDIT_U200_SUPPORT
//         MMIPIC_GetSaveLatterName();
// #endif
        SCI_MEMSET(s_pic_editor_info.file_info.full_path_wstr_ptr, 0 ,(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1)));
        MMI_WSTRNCPY( s_pic_editor_info.file_info.full_path_wstr_ptr, g_random_name_len,
            g_random_name_ptr, g_random_name_len, g_random_name_len);
        s_pic_editor_info.file_info.full_path_wstr_len = g_random_name_len;
    }

        //added, @robert.wang, 09-10-23, cr158200
        //检查文件名是否有效
        //先分解全路径
        if(MMIAPIFMM_SplitFullPath( g_random_name_ptr, g_random_name_len,\
                               device_name, &device_name_len,
                               dir_name, &dir_name_len,
                                no_suffix_name,
                                &no_suffix_name_len ))
        {

            text_str.wstr_ptr = no_suffix_name;
            text_str.wstr_len = no_suffix_name_len;
            
            //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_Save filename len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_1473_112_2_18_2_45_44_26,(uint8*)"d",text_str.wstr_len);
            //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_Save dir_name_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_1474_112_2_18_2_45_44_27,(uint8*)"d",dir_name_len);

            if (g_random_name_len > MMIFILE_FILE_NAME_MAX_LEN)
            {
                ret_value = MMI_PIC_ERR_LONG_FILENAME;                
                return ret_value;
            }


            //判断文件名是否有效，若无效，对空格,*,., 的文件直接去除后
            //直接得到有效文件名
             if (!MMIAPICOM_FileNameIsValid(&text_str))
            {
                new_file_name_len = MMIFILE_FILE_NAME_MAX_LEN;
                if(MMIAPICOM_GetValidFileName(1,text_str.wstr_ptr,text_str.wstr_len,\
                                new_file_name_ptr,new_file_name_len))
                {
                    new_file_name_len = MMIAPICOM_Wstrlen(new_file_name_ptr);
                    text_str.wstr_ptr = new_file_name_ptr;
                    text_str.wstr_len = new_file_name_len;
                    if(MMIAPICOM_FileNameIsValid(&text_str) && 
                       MMIAPIFMM_CombineFullPath(device_name, device_name_len,
                                               dir_name, dir_name_len,
                                               new_file_name_ptr, new_file_name_len,
                                               new_full_path, &new_full_path_len))
                    {
                            //得到新的全路径
                            g_random_name_len = new_full_path_len;
                            SCI_MEMSET(g_random_name_ptr,0,(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1)));
                            MMI_WSTRNCPY(g_random_name_ptr, g_random_name_len,
                                new_full_path, new_full_path_len, g_random_name_len);

                                SCI_MEMSET(s_pic_editor_info.file_info.full_path_wstr_ptr, 0 ,(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1)));
                                MMI_WSTRNCPY( s_pic_editor_info.file_info.full_path_wstr_ptr, g_random_name_len,
                                g_random_name_ptr, g_random_name_len, g_random_name_len);
                                s_pic_editor_info.file_info.full_path_wstr_len = g_random_name_len;

                    }else{
                          is_valid_name = FALSE;
                    }
                }else{
                      is_valid_name = FALSE;
                }
            } // end if  MMIAPICOM_FileNameIsValid()
        }else{
            is_valid_name = FALSE;
        }

        if(!is_valid_name)
        {
            ret_value = MMI_PIC_ERR_INVALID_NAME;
            return ret_value ;
        }
        //end cr158200

    dev = MMIAPIFMM_GetDeviceTypeByPath(
                                        s_pic_editor_info.file_info.full_path_wstr_ptr, 
                                        MMIFILE_DEVICE_LEN
                                      );
    
    if((MMI_DEVICE_NUM == dev) || !MMIAPIFMM_IsDevEnoughSpace(s_edit_buffer_size, (dev)))
    {
        ret_value = MMI_PIC_ERR_NO_SPACE;
        return ret_value;
    }
    
    //set input param   
    input_param.src_ptr=s_pic_editor_info.display_bitmap_ptr->bit_ptr;
    input_param.out_buf_ptr=s_pic_editor_info.output_buf_ptr;
    input_param.out_buf_size=s_edit_buffer_size;
    input_param.input_width=s_pic_editor_info.display_bitmap_ptr->img_width;
    input_param.input_height=s_pic_editor_info.display_bitmap_ptr->img_height;
    input_param.target_width=s_pic_editor_info.display_bitmap_ptr->img_width;
    input_param.target_height=s_pic_editor_info.display_bitmap_ptr->img_height;
    input_param.in_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    input_param.out_type=PEPROCESS_IMAGE_FORMAT_JPEG;
    
    //set process_param_ptr param
    process_param.process_buf_ptr = s_pic_editor_info.process_buf_ptr;
    process_param.process_buf_size = s_process_buffer_size;
    process_param.type = PEPROCESS_ENCODE_JPEG;
    
    //param
    process_param.process_param.encode_to_jpeg.quality_level=PEPROCESS_COMPRESS_LEVEL_LOW;
    
    //image param
    ref_return = PEPROCESS_ImageProcess(&input_param,&output_param,&process_param);
    if( PEPROCESS_OK != ref_return )
    {
        ret_value = MMI_PIC_ERR_EXIT;
        return ret_value;
    }
    
    if(!MMIAPIFMM_IsFileExist( g_random_name_ptr,g_random_name_len))
    {
        
        if(MMIAPIFMM_WriteFilesDataSyn(g_random_name_ptr, g_random_name_len, input_param.out_buf_ptr,
            output_param.compress_out_size))
        {
            ret_value = MMI_PIC_SUCCESS;
        }
        else
        {
            ret_value = MMI_PIC_ERR_NO_SPACE;
        }
    }
    else
    {
        if(is_overwrite)
        {       
            MMIAPIFMM_DeleteFileSyn( g_random_name_ptr,g_random_name_len);
            if (MMIAPIFMM_WriteFilesDataSyn(g_random_name_ptr, g_random_name_len, input_param.out_buf_ptr,output_param.compress_out_size))
            {
                ret_value = MMI_PIC_SUCCESS;
            }
            else
            {
                ret_value = MMI_PIC_ERR_EXIT;
            }
        }
        else
        {
            ret_value = MMI_PIC_ERR_QUERY_OVERWRITE;
        }           
    }
    return ret_value;    
}

/*****************************************************************************/
//  Description : get image file info
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIPIC_GetFileInfo(PIC_EDIT_FILE_INFO_T * param_to_save_as_win)
{
    if (PNULL == param_to_save_as_win)
    {
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_GetFileInfo param_to_save_as_win is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_1607_112_2_18_2_45_44_28,(uint8*)"");
        return;
    }
    
    param_to_save_as_win->full_path_wstr_ptr=s_pic_editor_info.file_info.full_path_wstr_ptr;
    param_to_save_as_win->full_path_wstr_len=s_pic_editor_info.file_info.full_path_wstr_len;
}

/*****************************************************************************/
//  Description : save as new file
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC MMI_PIC_ERROR_TYPE_E  MMIPIC_SaveAs(
                           const wchar *full_path_ptr,     //in
                           uint16     full_path_len       //in, 双字节为单位
                           )
{
    PEPROCESS_INPUT_T     input_param={0};
    PEPROCESS_OUTPUT_T      output_param={0};
    PEPROCESS_PARAMETER_T  process_param={0};
    BOOLEAN     result = FALSE;
    //BOOLEAN     is_udisk = FALSE;
    uint32      free_high_word = 0;
    uint32      free_low_word = 0;
    PEPROCESS_RESULT_E ref_return = PEPROCESS_OK;
    MMIFILE_DEVICE_E  device_type = MMI_DEVICE_UDISK;

    //added, @robert.wang, 09-8-20, cr147752
    BOOLEAN  is_changed=TRUE;
    uint32          time_out = 0;
    MMI_WIN_ID_T    wait_win_id = MMIPIC_COPY_FILE_WAIT_WIN_ID;
    //end cr147752

    MMI_PIC_ERROR_TYPE_E     ret_value = MMI_PIC_SUCCESS; 
    BOOLEAN                  is_copy_success = FALSE;
    BOOLEAN                  is_write_success = FALSE;
    
    if (PNULL == full_path_ptr)
    {
        ret_value = MMI_PIC_ERR_EXIT;
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_SaveAs full_path_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_1647_112_2_18_2_45_44_29,(uint8*)"");
        return ret_value;
    }
    
    if (0 == full_path_len)
    {
        ret_value = MMI_PIC_ERR_EXIT;
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_SaveAs full_path_len is 0 !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_1654_112_2_18_2_45_44_30,(uint8*)"");
        return ret_value;
    }


    //added, @robert.wang, 09-8-20, cr147752
    //directly  copy file while picture is not any changed.
    
    is_changed=MMIPIC_IsPictureChanged();

    //SCI_TRACE_LOW:"MMIPIC_SaveAs is_changed=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_1664_112_2_18_2_45_44_31,(uint8*)"d",is_changed);
        
    if( !is_changed)
     {
            //如果全路径相同，是覆盖，直接返回成功
            //会有询问是否覆盖的窗口
            //若是不同，直接拷贝文件
            if(0 == MMIAPICOM_Wstrcmp(s_pic_editor_info.file_info.full_path_wstr_ptr,full_path_ptr))
            {
                ret_value = MMI_PIC_SUCCESS;
            }
            else
            {

                MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandleCopyFileWaitWinMsg);

                is_copy_success = MMIAPIFMM_CopyOneFile(s_pic_editor_info.file_info.full_path_wstr_ptr, s_pic_editor_info.file_info.full_path_wstr_len, \
                         full_path_ptr, full_path_len, wait_win_id, MSG_PIC_COPY_FILE);

                //如果失败，直接关闭等待窗口
                if(!is_copy_success)        
                {
                    ret_value = MMI_PIC_ERR_EXIT;
                    MMK_CloseWin(wait_win_id);
                }
                else
                {                
                    SCI_MEMSET(s_pic_editor_info.file_info.full_path_wstr_ptr, 0 ,(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1)));
                    MMI_WSTRNCPY( s_pic_editor_info.file_info.full_path_wstr_ptr, full_path_len,
                    full_path_ptr, full_path_len, full_path_len);
                    s_pic_editor_info.file_info.full_path_wstr_len = full_path_len;

                    ret_value = MMI_PIC_SUCCESS;
                }
                
                //SCI_TRACE_LOW:"MMIAPIFMM_CopyOneFile ret=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_1699_112_2_18_2_45_44_32,(uint8*)"d",ret_value);
            }

            return ret_value; 

    }
    //end cr147752
    
    //set input param   
    input_param.src_ptr=s_pic_editor_info.display_bitmap_ptr->bit_ptr;
    input_param.out_buf_ptr=s_pic_editor_info.output_buf_ptr;
    input_param.out_buf_size=s_edit_buffer_size;
    input_param.input_width=s_pic_editor_info.display_bitmap_ptr->img_width;
    input_param.input_height=s_pic_editor_info.display_bitmap_ptr->img_height;
    input_param.target_width=s_pic_editor_info.display_bitmap_ptr->img_width;
    input_param.target_height=s_pic_editor_info.display_bitmap_ptr->img_height;
    input_param.in_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    input_param.out_type=PEPROCESS_IMAGE_FORMAT_JPEG;
    
    //set process_param_ptr param
    process_param.process_buf_ptr = s_pic_editor_info.process_buf_ptr;
    process_param.process_buf_size = s_process_buffer_size;	
    process_param.type = PEPROCESS_ENCODE_JPEG;
    
    //param
    process_param.process_param.encode_to_jpeg.quality_level=PEPROCESS_COMPRESS_LEVEL_LOW;
    
    //image param
    ref_return = PEPROCESS_ImageProcess(&input_param,&output_param,&process_param);
    if( PEPROCESS_OK != ref_return )
    {
        ret_value = MMI_PIC_ERR_EXIT;
        return ret_value;
    }
    
    SCI_MEMSET(s_pic_editor_info.file_info.full_path_wstr_ptr, 0 ,(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1)));
    MMI_WSTRNCPY( s_pic_editor_info.file_info.full_path_wstr_ptr, full_path_len,
        full_path_ptr, full_path_len, full_path_len);
    s_pic_editor_info.file_info.full_path_wstr_len = full_path_len;
    
    device_type = MMIAPIFMM_GetDeviceTypeByPath( s_pic_editor_info.file_info.full_path_wstr_ptr, MMIFILE_DEVICE_LEN);
    if(device_type < MMI_DEVICE_NUM)
    {
        MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(device_type),
            MMIAPIFMM_GetDevicePathLen(device_type),
            &free_high_word,
            &free_low_word);
    }
    if ((0 < free_high_word) || (s_edit_buffer_size < free_low_word))
    {
        result = TRUE;
    }
    
    if(result)
    {   
        is_write_success = MMIAPIFMM_WriteFilesDataSyn( s_pic_editor_info.file_info.full_path_wstr_ptr, s_pic_editor_info.file_info.full_path_wstr_len,\
                                                input_param.out_buf_ptr,output_param.compress_out_size);
        if (is_write_success)
        {
            ret_value = MMI_PIC_SUCCESS;
        }
        else
        {
            ret_value = MMI_PIC_ERR_EXIT;
        }
    }
    else
    {
        ret_value = MMI_PIC_ERR_NO_SPACE;
    }
    
    return ret_value;    
}

/*****************************************************************************/
//  Description : update file name
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIPIC_UpdateFileName(MMI_WIN_ID_T     win_id)
{
    wchar       image_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16      image_name_len = 0;

    MMIAPIFMM_SplitFullPath(s_pic_editor_info.file_info.full_path_wstr_ptr,s_pic_editor_info.file_info.full_path_wstr_len,
        PNULL,PNULL,PNULL,PNULL,
        image_name,&image_name_len);
    //set title
    MMIAPIFMM_SetFileNameWinTitle(win_id,image_name,image_name_len, FALSE);
}
/*****************************************************************************/
//  Description : add rotated image
//  Global resource dependence : 
//  Author: vincent
//  Note: display image in the center of client rect
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPIC_AppendRotatePreviewImageData(void)
{
    uint32  buf_size = 0;//buf_size=21k
    uint32  i=0;
    uint32  j=0;
    uint32  pic_height=0;
    uint32  pic_width=0; 
    PEPROCESS_INPUT_T     input_param={0};
    PEPROCESS_OUTPUT_T  output_param={0};
    PEPROCESS_PARAMETER_T  process_param={0};
    uint16 h_space = 0;
    uint16 v_space = 0; 	
    BOOLEAN ret_value = TRUE; 
    PEPROCESS_RESULT_E ref_return = PEPROCESS_OK;
    
    uint16  rotate_width = 0;
    uint16  rotate_height = 0;
    uint16  client_height = 0;
    uint16  client_width = 0;
    uint16  margin_pixel = 6;
    uint16  width_min = 0;
    int16   offset_h = 0;
    int16   offset_v = 0;
    const int8    each_line_num = 2; //picture numbers for each line

    GUI_RECT_T    display_rect = MMIPIC_GetClientWinRect(MMIPIC_EDIT_ROTATE_WIN_ID);
    
    client_width = MMITHEME_GetClientRectWidth();
    client_height = display_rect.bottom - display_rect.top + 1;

    width_min = MIN(client_width,client_height);

    rotate_width = (width_min - (each_line_num + 1) * margin_pixel) / each_line_num;
    rotate_height = rotate_width;
    buf_size = rotate_width * rotate_height * sizeof(GUI_COLOR_T);
    
    h_space =(client_width - rotate_width * each_line_num) / (each_line_num + 1);
    v_space = (client_height - rotate_height * each_line_num) / (each_line_num + 1);

    
    pic_height=s_pic_editor_info.display_bitmap_ptr->img_height;
    pic_width=s_pic_editor_info.display_bitmap_ptr->img_width;     
    
    //set input param   
    input_param.src_ptr=s_pic_editor_info.display_bitmap_ptr->bit_ptr;
    input_param.out_buf_ptr=s_pic_editor_info.output_buf_ptr;
    input_param.out_buf_size=s_edit_buffer_size;
    
    input_param.in_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    input_param.out_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    
    
    //set process_param_ptr param
    process_param.process_buf_ptr=s_pic_editor_info.process_buf_ptr;
    process_param.process_buf_size = s_process_buffer_size;	    
    output_param.actual_out_height=0;
    output_param.actual_out_width=0;
    
    SCI_MEMSET(s_icon_image, 0, sizeof(s_icon_image));
    
    //init icon struct
    for(i=MMI_ROTATION_DEGREE_90;i<=MMI_ROTATION_DEGREE_MIRROR;i++)
    {
        //alloc bitmap struct memory
        s_icon_image[i].icon_bitmap_ptr = SCI_ALLOCA(sizeof(GUIIMG_BITMAP_T));
        if (PNULL == s_icon_image[i].icon_bitmap_ptr)
        {
            //MMIPIC_ASSERT_WARNING:"[MMIPIC] s_icon_image[i].icon_bitmap_ptr is PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_1859_112_2_18_2_45_45_33,(uint8*)"");
            ret_value = FALSE;
            return ret_value;
        }
        SCI_MEMSET(s_icon_image[i].icon_bitmap_ptr, 0, sizeof(GUIIMG_BITMAP_T));
        MMI_MEMCPY(s_icon_image[i].icon_bitmap_ptr, sizeof(GUIIMG_BITMAP_T),
            s_pic_editor_info.display_bitmap_ptr, sizeof(GUIIMG_BITMAP_T), sizeof(GUIIMG_BITMAP_T));

        //alloc bitmap data memory
        s_icon_image[i].icon_bitmap_ptr->bit_ptr=SCI_ALLOCA(buf_size);
        if (PNULL == s_icon_image[i].icon_bitmap_ptr->bit_ptr)
        {
            //MMIPIC_ASSERT_WARNING:"[MMIPIC] s_icon_image[i].icon_bitmap_ptr->bit_ptr is PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_1871_112_2_18_2_45_45_34,(uint8*)"");
            ret_value = FALSE;
            return ret_value;
        }
        SCI_MEMSET(s_icon_image[i].icon_bitmap_ptr->bit_ptr, 0, buf_size);
        
    }
    
    
    //get 4 rotated image
    for(j=MMI_ROTATION_DEGREE_90;j<=MMI_ROTATION_DEGREE_MIRROR;j++)
    {
        //set param
        input_param.src_ptr=s_pic_editor_info.display_bitmap_ptr->bit_ptr;
        input_param.input_height=pic_height;
        input_param.input_width=pic_width;  
        input_param.target_width=input_param.input_width;
        input_param.target_height=input_param.input_height;
        
        input_param.out_buf_ptr=s_pic_editor_info.output_buf_ptr;
        input_param.out_buf_size=s_edit_buffer_size;
        
        SCI_MEMSET(s_pic_editor_info.output_buf_ptr, 0, s_edit_buffer_size);
        
        if(MMI_ROTATION_DEGREE_MIRROR==j)
        {
            process_param.type = PEPROCESS_MIRROR;
        }
        else
        {
            process_param.type = PEPROCESS_ROTATION;
            process_param.process_param.rotation.degree = (PEPROCESS_ROTATION_DEGREE_E)j;
        }
        
        
        //rotate image 
        ref_return = PEPROCESS_ImageProcess(&input_param,&output_param,&process_param);
        if( PEPROCESS_OK != ref_return )
        {
            ret_value = FALSE;
            return ret_value;
        }


        s_pic_editor_info.output_pic_width = output_param.actual_out_width;
        s_pic_editor_info.output_pic_height = output_param.actual_out_height;

        
        //scaling down
        input_param.src_ptr=s_pic_editor_info.output_buf_ptr;//s_pic_editor_info.undo_buf_ptr;
        input_param.out_buf_ptr=s_icon_image[j].icon_bitmap_ptr->bit_ptr;//s_pic_editor_info.output_buf_ptr;
        input_param.out_buf_size=buf_size;
        

        input_param.input_width=s_pic_editor_info.output_pic_width;
        input_param.input_height=s_pic_editor_info.output_pic_height;

        
        input_param.target_width = rotate_width;
        input_param.target_height = rotate_height;
        process_param.type = PEPROCESS_SCALINGDOWN;
        
        ref_return = PEPROCESS_ImageProcess(&input_param,&output_param,&process_param);
        if( PEPROCESS_OK != ref_return )
        {
            ret_value = FALSE;
            return ret_value;
        }
        
        s_icon_image[j].icon_bitmap_ptr->img_width=output_param.actual_out_width;
        s_icon_image[j].icon_bitmap_ptr->img_height=output_param.actual_out_height;

        //get image relative position from client rect
        offset_h = (rotate_width + h_space)*( j % each_line_num) + (rotate_width - output_param.actual_out_width) / 2;
        offset_v = (rotate_height + v_space) * (j / each_line_num) + (rotate_height - output_param.actual_out_height) / 2;

        s_icon_image[j].display_rect.left= h_space + offset_h;
        s_icon_image[j].display_rect.top = display_rect.top + v_space + offset_v;        
        s_icon_image[j].display_rect.right =s_icon_image[j].display_rect.left + s_icon_image[j].icon_bitmap_ptr->img_width - 1;
        s_icon_image[j].display_rect.bottom =s_icon_image[j].display_rect.top + s_icon_image[j].icon_bitmap_ptr->img_height - 1;
    }
    
    return ret_value;
}/*****************************************************************************/
//  Description : handle  tp down msg of rotated preview window
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN   MMIPIC_HandleRotatedPreviewTpDown(GUI_POINT_T point, BOOLEAN *is_in_rect)
{
    MMI_ROTATION_E i=MMI_ROTATION_DEGREE_90;
    
    for(i=MMI_ROTATION_DEGREE_90;i<=MMI_ROTATION_DEGREE_MIRROR;i++)
    {
        
        if(GUI_PointIsInRect(point, s_icon_image[i].display_rect))
        {
                //added, @robert.wang, 09-10-27, cr158180
                 //针对触摸屏，直接点击后，退出选择界面
                 MMIPIC_OnlySaveOrgImgToUndoBuf();//保存undo buf
                //end cr158180

            *is_in_rect = TRUE;
            if( MMIPIC_GetRotatedImageData((uint16)i) )
            {
                //added, @robert.wang, 09-10-26, cr158180
                MMIPIC_CopyOutputImgDataToOrgImgData();
                MMIPIC_CopyOutputRectToOrgRect();
                //end cr158180
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }    
    }
    *is_in_rect = FALSE;
    return FALSE;
}

/*****************************************************************************/
//  Description : free rotate preview image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void   MMIPIC_FreeRotatedPreviewImage(void)
{
    MMI_ROTATION_E i=MMI_ROTATION_DEGREE_90;
    //SCI_TRACE_LOW:"free rotate preview image buf"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_1999_112_2_18_2_45_45_35,(uint8*)"");
    for(i=MMI_ROTATION_DEGREE_90;i<=MMI_ROTATION_DEGREE_MIRROR;i++)
    {
    	  if(PNULL!=s_icon_image[i].icon_bitmap_ptr)
        {
          if(PNULL!=s_icon_image[i].icon_bitmap_ptr->bit_ptr)
          {
            SCI_FREE(s_icon_image[i].icon_bitmap_ptr->bit_ptr);
            s_icon_image[i].icon_bitmap_ptr->bit_ptr=PNULL;
          }
          SCI_FREE(s_icon_image[i].icon_bitmap_ptr);
          s_icon_image[i].icon_bitmap_ptr=PNULL;
        }
    }
    
}
/*****************************************************************************/
//  Description : display all rotated preview image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIPIC_DisplayAllRotatedPreviewImage(uint16 cur_index)
{
    GUI_RECT_T        display_rect = MMITHEME_GetClientRectEx(MMIPIC_EDIT_ROTATE_WIN_ID);
    GUI_POINT_T     dis_point = {0};
    uint32  i=0;
    //display bg image
    GUIRES_DisplayImg(PNULL,
        &display_rect,
        &display_rect,
        MMIPIC_EDIT_ROTATE_WIN_ID,
        IMAG_PS_BG,
        &s_pic_editor_info.lcd_dev);

    //DisplayIconListBg(iconlist_ctrl_ptr);
    for(i=MMI_ROTATION_DEGREE_90;i<=MMI_ROTATION_DEGREE_MIRROR;i++)
    {
        
        GUI_FillRect(&s_pic_editor_info.lcd_dev,s_icon_image[i].display_rect,MMIPIC_ICON_BG_COLOR);
        
        dis_point.x = s_icon_image[i].display_rect.left;
        dis_point.y = s_icon_image[i].display_rect.top;
        GUIIMG_DisplayBmp(FALSE,
            &s_icon_image[i].display_rect,
            &dis_point,
            s_icon_image[i].icon_bitmap_ptr,
            &s_pic_editor_info.lcd_dev);
    }
    //display select frame                                
    MMIPIC_DisplayCurSelectedFlag(s_icon_image[cur_index].display_rect);
}
/*****************************************************************************/
//  Description : draw selected flag
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPIC_DisplayCurSelectedFlag(GUI_RECT_T display_rect)
{
    //set the rect larger
    display_rect.left--;
    display_rect.top--;

    //modified, @robert.wang, 09-11-17, cr157217
    display_rect.right++;
    display_rect.bottom++;

    if(display_rect.left < 0)
    {
        display_rect.left = 0;
    }
    if(display_rect.top < 0)
    {
        display_rect.top = 0;
    }
    //end cr157217
    
    
    //draw rect
    LCD_DrawRect(&s_pic_editor_info.lcd_dev,
        display_rect,
        MMI_DARK_RED_COLOR);

    return TRUE;
}

/*****************************************************************************/
//  Description : get rotated image data
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_GetRotatedImageData(uint16 cur_image_index)
{
    PEPROCESS_INPUT_T         input_param={0};
    PEPROCESS_OUTPUT_T       output_param={0};
    PEPROCESS_PARAMETER_T  process_param={0};
    uint16 i=0;
    BOOLEAN ret_value = TRUE; 
    PEPROCESS_RESULT_E ref_return = PEPROCESS_OK;
    uint8 *pBitmap = PNULL;
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;

    GUI_RECT_T      display_rect = MMIPIC_GetClientWinRect(MMIPIC_EDIT_ROTATE_WIN_ID);
    uint16          client_height = display_rect.bottom - display_rect.top + 1;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    for(i=0;i<MMI_ROTATION_DEGREE_MAX;i++)
    {
        SCI_FREE(s_icon_image[i].icon_bitmap_ptr->bit_ptr);
        s_icon_image[i].icon_bitmap_ptr->bit_ptr=PNULL;
    }
    //set input param   
    input_param.src_ptr=s_pic_editor_info.display_bitmap_ptr->bit_ptr;
    input_param.out_buf_ptr=s_pic_editor_info.output_buf_ptr;
    input_param.out_buf_size=s_edit_buffer_size;
    input_param.input_height=s_pic_editor_info.display_bitmap_ptr->img_height;
    input_param.input_width=s_pic_editor_info.display_bitmap_ptr->img_width;
    input_param.target_width=input_param.input_width;
    input_param.target_height=input_param.input_height;
    input_param.in_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    input_param.out_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    
    //set process_param_ptr param
    process_param.process_buf_ptr=s_pic_editor_info.process_buf_ptr;
//process buffer size changes from 240*320*2 to 240*320*3
    process_param.process_buf_size = s_process_buffer_size;	    
    
    if(MMI_ROTATION_DEGREE_MIRROR == cur_image_index)
    {
        process_param.type = PEPROCESS_MIRROR;
    }
    else
    {
        process_param.type = PEPROCESS_ROTATION;
        process_param.process_param.rotation.degree = (PEPROCESS_ROTATION_DEGREE_E)cur_image_index;
    }
    
    //get rotated image data
    ref_return = PEPROCESS_ImageProcess(&input_param,&output_param,&process_param);
    if( PEPROCESS_OK != ref_return )
    {
        ret_value = FALSE;
        return ret_value;
    }

    //SCALING图片，以便能在屏幕中显示下
    pBitmap = (uint8*)SCI_ALLOCA(s_edit_buffer_size);
    if(pBitmap != PNULL)
    {
        SCI_MEMSET(pBitmap,0x00,s_edit_buffer_size);

        //set input param   
        input_param.src_ptr = s_pic_editor_info.output_buf_ptr;
        input_param.out_buf_ptr = pBitmap;
        input_param.out_buf_size = s_edit_buffer_size;
        input_param.input_height = output_param.actual_out_height;
        input_param.input_width = output_param.actual_out_width;
        input_param.in_type = PEPROCESS_IMAGE_FORMAT_RGB_565;
        input_param.out_type = PEPROCESS_IMAGE_FORMAT_RGB_565;
        
        //set process_param_ptr param
        process_param.process_buf_ptr=s_pic_editor_info.process_buf_ptr;
        process_param.process_buf_size = s_process_buffer_size;

        input_param.target_width = lcd_width;
        input_param.target_height = lcd_height;
        process_param.type = PEPROCESS_SCALINGDOWN;
        
        ref_return = PEPROCESS_ImageProcess(&input_param,&output_param,&process_param);
       
      // copy bitmap data to  output  data
       MMI_MEMCPY(s_pic_editor_info.output_buf_ptr, s_edit_buffer_size,
                         pBitmap, s_edit_buffer_size, s_edit_buffer_size);
       SCI_FREE(pBitmap);

        if( PEPROCESS_OK != ref_return )
        {
            ret_value = FALSE;
            return ret_value;
        }
    }
    else
    {
        //若是分配内存失败，继续执行
        MMIPIC_ASSERT_WARNING("[MMIPIC] pBitmap != PNULL");/*lint !e774*/
    }

    
    //modified, @robert.wang, 09-10-26, cr158180
     s_pic_editor_info.output_pic_width = output_param.actual_out_width;
     s_pic_editor_info.output_pic_height = output_param.actual_out_height;

    //recalculate the display  rect 
    s_pic_editor_info.output_display_rect.left=(lcd_width - s_pic_editor_info.output_pic_width)/2;
    s_pic_editor_info.output_display_rect.right=s_pic_editor_info.output_display_rect.left + s_pic_editor_info.output_pic_width -1;
    s_pic_editor_info.output_display_rect.top= display_rect.top + (int16)(client_height - s_pic_editor_info.output_pic_height) / 2;;
    s_pic_editor_info.output_display_rect.bottom=s_pic_editor_info.output_display_rect.top + s_pic_editor_info.output_pic_height -1;

    //end cr158180    

    return ret_value;        
}
/*****************************************************************************/
//  Description : get current paste image data
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIPIC_GetCurrentPasteImageData(uint32 img_id,MMI_WIN_ID_T win_id)
{
    uint8   *osd_buf = PNULL;
    uint16  img_width = 0;
    uint16  img_height = 0;
    uint32  img_data_size = 0;
    uint32  osd_buf_size = 0;
    int32   ret_value = FALSE;
    
    osd_buf=(uint8 *)MMI_GetLabelImage(img_id,win_id,&img_data_size);
    
    //get image width and height
    GUIRES_GetImgWidthHeight(&img_width,&img_height,img_id,win_id);
    
    //set width and height
    s_pic_editor_info.osd_image.width  = img_width;
    s_pic_editor_info.osd_image.height = img_height;
    
    
    s_pic_editor_info.osd_image.colorkey=MMI_BLACK_COLOR;
    s_pic_editor_info.osd_image.offset_x=0;
    s_pic_editor_info.osd_image.offset_y=0;
    
    osd_buf_size = img_width*img_height*2;
    if(PNULL==s_pic_editor_info.osd_image.osd_data_buffer)
    {
        s_pic_editor_info.osd_image.osd_data_buffer=SCI_ALLOCA(osd_buf_size);
        if (PNULL == s_pic_editor_info.osd_image.osd_data_buffer)
        {
            //MMIPIC_ASSERT_WARNING:"[MMIPIC] s_pic_editor_info.osd_image.osd_data_buffer is PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_2232_112_2_18_2_45_45_36,(uint8*)"");
            ret_value = FALSE;
            return ret_value;
        }
    }
    
    SCI_MEMSET(s_pic_editor_info.osd_image.osd_data_buffer, 0, osd_buf_size);
    MMI_MEMCPY(s_pic_editor_info.osd_image.osd_data_buffer, osd_buf_size,
        osd_buf, osd_buf_size, osd_buf_size);

    ret_value = TRUE;
    return ret_value;
}
/*****************************************************************************/
//  Description : display osd image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIPIC_FreeOsdBuf(void)
{
    if(PNULL!=s_pic_editor_info.osd_image.osd_data_buffer)
    {
        SCI_FREE(s_pic_editor_info.osd_image.osd_data_buffer);
        s_pic_editor_info.osd_image.osd_data_buffer=PNULL;
    }
}

/*****************************************************************************/
//  Description : Set osd image position
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_SetOSDImagePosition(void)
{
    int32 relative_width = 0;
    int32 relative_height = 0;

    relative_width = s_pic_editor_info.display_bitmap_ptr->img_width - s_pic_editor_info.osd_image.width;
    relative_height = s_pic_editor_info.display_bitmap_ptr->img_height - s_pic_editor_info.osd_image.height;
    
    s_pic_editor_info.osd_display_rect.left = s_pic_editor_info.display_rect.left + relative_width / 2;
    s_pic_editor_info.osd_display_rect.top = s_pic_editor_info.display_rect.top + relative_height / 2;
    s_pic_editor_info.osd_display_rect.right = s_pic_editor_info.osd_display_rect.left + s_pic_editor_info.osd_image.width - 1;
    s_pic_editor_info.osd_display_rect.bottom = s_pic_editor_info.osd_display_rect.top + s_pic_editor_info.osd_image.height - 1;
}

/*****************************************************************************/
//  Description : display osd image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_DisplayOSDImage(void)
{
    uint32              data_size = 0;
    GUI_LCD_DEV_INFO    lcd_dev={GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};            //lcd dev
    GUI_RECT_T          image_rect={0};
    
    
    lcd_dev.block_id=GUI_BLOCK_MAIN;//GUI_BLOCK_OSD;
    lcd_dev.lcd_id=GUI_MAIN_LCD_ID;
    
    image_rect.left=0;
    image_rect.top=0;
    image_rect.right=s_pic_editor_info.osd_image.width;
    image_rect.bottom=s_pic_editor_info.osd_image.height;
    
    
    IMG_EnableTransparentColor(TRUE);//decode osd
    data_size = s_pic_editor_info.osd_image.width*s_pic_editor_info.osd_image.height*2;
    GUIRES_DisplayImgByPtr(s_pic_editor_info.osd_image.osd_data_buffer,
        data_size,
        PNULL,
        &s_pic_editor_info.osd_display_rect,
        &image_rect,
        &lcd_dev);
    IMG_EnableTransparentColor(FALSE);
    
}

/*****************************************************************************/
//  Description : merge image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_MergeImage(void)
{
    PEPROCESS_INPUT_T       input_param={0};
    PEPROCESS_OUTPUT_T      output_param={0};
    PEPROCESS_PARAMETER_T   process_param={0};
    //GUI_COLOR_T             *lcd_buffer_ptr = PNULL;    
    GUI_LCD_DEV_INFO        lcd_dev={GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    uint32                  osd_buffer_size=0;
    GUI_POINT_T             imagerect={0};
    BOOLEAN                 ret_value = TRUE; 
    PEPROCESS_RESULT_E      ref_return = PEPROCESS_OK;
    
    //get lcd buf
   // lcd_buffer_ptr=GUIBLOCK_GetBuf(&lcd_dev);
    
    osd_buffer_size=s_pic_editor_info.osd_image.width*s_pic_editor_info.osd_image.height*2;
    s_pic_editor_info.osd_image.osd_ptr=SCI_ALLOCA(osd_buffer_size);
    if (PNULL == s_pic_editor_info.osd_image.osd_ptr)
    {
        //MMIPIC_ASSERT_WARNING:"[MMIPIC] s_pic_editor_info.osd_image.osd_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_2331_112_2_18_2_45_46_37,(uint8*)"");
        ret_value = FALSE;
        return ret_value;
    }
    SCI_MEMSET(s_pic_editor_info.osd_image.osd_ptr, 0 ,osd_buffer_size);
    
    imagerect.x=0;
    imagerect.y=0;
    
    //copy image buffer
    GUIRES_CopyImgBuf(s_pic_editor_info.osd_image.osd_data_buffer,
        s_pic_editor_info.osd_image.width,
        s_pic_editor_info.osd_image.height,
        osd_buffer_size,
        osd_buffer_size,
        &imagerect,
        PNULL,
        (GUI_COLOR_T *)s_pic_editor_info.osd_image.osd_ptr);
   
    //set input param   
    input_param.src_ptr=s_pic_editor_info.display_bitmap_ptr->bit_ptr;
    input_param.out_buf_ptr=s_pic_editor_info.output_buf_ptr;
    input_param.out_buf_size=s_edit_buffer_size;
    input_param.input_width=s_pic_editor_info.display_bitmap_ptr->img_width;
    input_param.input_height=s_pic_editor_info.display_bitmap_ptr->img_height;
    input_param.target_width=s_pic_editor_info.display_bitmap_ptr->img_width;
    input_param.target_height=s_pic_editor_info.display_bitmap_ptr->img_height;
    input_param.in_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    input_param.out_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    
    //set process_param_ptr param
    process_param.process_buf_ptr = s_pic_editor_info.process_buf_ptr;
//process buffer size changes from 240*320*2 to 240*320*3
    process_param.process_buf_size = s_process_buffer_size;	
    process_param.type = PEPROCESS_PHOTOFRAME;
    
    //osd param
    process_param.process_param.photoframe.osd_ptr=s_pic_editor_info.osd_image.osd_ptr;
    process_param.process_param.photoframe.colorkey=s_pic_editor_info.osd_image.colorkey;
    process_param.process_param.photoframe.offset_x=s_pic_editor_info.osd_display_rect.left-s_pic_editor_info.output_display_rect.left;
    process_param.process_param.photoframe.offset_y=s_pic_editor_info.osd_display_rect.top-s_pic_editor_info.output_display_rect.top;
    process_param.process_param.photoframe.width=s_pic_editor_info.osd_image.width;
    process_param.process_param.photoframe.height=s_pic_editor_info.osd_image.height;
    
    //image param
    ref_return = PEPROCESS_ImageProcess(&input_param,&output_param,&process_param);
    if( PEPROCESS_OK != ref_return )
    {
        ret_value = FALSE;
        if(PNULL!=s_pic_editor_info.osd_image.osd_ptr)
        {
            SCI_FREE(s_pic_editor_info.osd_image.osd_ptr);
            s_pic_editor_info.osd_image.osd_ptr=PNULL;
        } 
        return ret_value;
    }
    
    
     s_pic_editor_info.output_pic_width = output_param.actual_out_width;
     s_pic_editor_info.output_pic_height = output_param.actual_out_height;
    //end cr158180

    if(PNULL!=s_pic_editor_info.osd_image.osd_ptr)
    {
        SCI_FREE(s_pic_editor_info.osd_image.osd_ptr);
        s_pic_editor_info.osd_image.osd_ptr=PNULL;
    } 
    
    return ret_value;    
}

/*****************************************************************************/
//  Description : 文本风格设置
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void GetPicTextStyle(
                           GUISTR_STYLE_T *text_style_ptr,
                           uint16 space,
                           GUI_COLOR_T color,
                           GUI_FONT_T  font,
                           uint8 line_space,
                           GUI_COLOR_T edge_color,
                           GUI_ANGLE_E angle
                           )
{
    if (PNULL == text_style_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPIC] GetPicTextStyle text_style_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_2418_112_2_18_2_45_46_38,(uint8*)"");
        return ;
    }

    text_style_ptr->align = ALIGN_LEFT; // 这里设置为Left,因为Label的位置自有自己调整区域
    text_style_ptr->angle = angle;
    text_style_ptr->char_space = space;
    text_style_ptr->effect = FONT_EFFECT_CUSTOM;
    text_style_ptr->edge_color = edge_color;
    text_style_ptr->font = font;
    text_style_ptr->font_color = color;
    text_style_ptr->line_space = line_space;
    text_style_ptr->region_num = 0;
    text_style_ptr->region_ptr = PNULL;
}

#if 0
/*****************************************************************************/
//  Description : draw text on image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
//        //因为MMIPIC_CopyOrgImgDataToOutputImgData()
//         OUTPUT 大小 与     display_bitmap_ptr 相同
            //故此函数不再修改display_bitmap_ptr 为 OUTPUT 的内容
//       文字基于图片移动
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIPIC_DrawText(
                                  GUI_RECT_T * box,         //the fixed display area
                                  wchar * text_ptr,         //string to display
                                  uint16 len,               //length of string to display
                                  uint16 space,             //space between two chars.
                                  GUI_COLOR_T color,        //string color
                                  GUI_FONT_T  font,     //font pointer  
                                  uint16 line_space ,      //space between 2 lines       
                                  GUI_COLOR_T edge_color,//font edge color/font frame color)
                                  MMI_PIC_DIRECTION_E direction_type
                                  )
{
    GUI_RECT_T      actual_rect = {0};
    GUI_RECT_T      display_box={0};//box;
    GUI_ANGLE_E     angle=ANGLE_0;

    int16   text_pixel_size=0; 
    int16   char_pixel_height=0;
    uint16   screen_right_max_pixel = MMITHEME_GetMainScreenRightPixel();


    GUISTR_STYLE_T      text_style = {0};/*lint !e64*/
    MMI_STRING_T        str_data = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_EFFECT|GUISTR_STATE_WORDBREAK;
    GUISTR_BUF_T        des_buf = {0};

    GUI_RECT_T         client_rect = MMIPIC_GetClientWinRect(MMIPIC_CHARACTER_PREVIEW_WIN_ID);
    uint16             client_rect_top = 0;

    
    if (PNULL == text_ptr)
    {
        SCI_MEMSET(&actual_rect, 0x00, sizeof(actual_rect));
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_DrawText text_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_2476_112_2_18_2_45_46_39,(uint8*)"");
        return actual_rect;
    }

    client_rect_top = client_rect.top;
    
    // text
    str_data.wstr_len = len;
    str_data.wstr_ptr = text_ptr;
    
    if(!(box->left >= 0 && box->right >= box->left))
    {
        return actual_rect;
    }
    if (!(box->top >= 0 && box->bottom >= box->top))
    {
        return actual_rect;
    }

    if(s_pic_editor_info.output_display_rect.left>0)
    {
        box->right=s_pic_editor_info.output_pic_width;    
    }
    else
    {
        box->left=(-1)*s_pic_editor_info.output_display_rect.left;
        box->right=box->left+screen_right_max_pixel;
    }

    if(s_pic_editor_info.output_display_rect.top > client_rect_top)
    {
        box->top=0;
        box->bottom = box->top + s_pic_editor_info.output_pic_height -1 ;
    }
    //end cr158180
    
    if(MMI_PIC_DIRECTION_HORIZONTAL == direction_type)
    {           
        display_box=*box;
        angle=ANGLE_0;
        //added, @robert.wang,09-8-14, cr148300
        //根据字符串的长度调整左右坐标值
        text_pixel_size=GUISTR_GetStringWidth(&text_style, &str_data, state);
        if((display_box.left+text_pixel_size) > display_box.right)
        {
            display_box.left = display_box.right - text_pixel_size;
        }
        if(display_box.left < 0)
        {
            display_box.left = 0;
        }
        
        //根据字符串的最大高度调整上下坐标值
        char_pixel_height=GUI_GetStringHeight(font, text_ptr,len);
        if((display_box.top+char_pixel_height) > display_box.bottom)
        {
            display_box.top=display_box.bottom - char_pixel_height;
        }
        if(display_box.top < 0)
        {
            display_box.top=0;
        }
        //end cr148300
    }
    else
    {
        display_box=*box;
        angle=ANGLE_90;
        state |= GUISTR_STATE_ANGLE;

        //added, @robert.wang,09-8-14, cr148300
        //根据字符串的长度调整上下坐标值
        text_pixel_size=GUISTR_GetStringWidth(&text_style, &str_data, state);
        if((display_box.top + text_pixel_size) > display_box.bottom)
        {
            display_box.top = display_box.bottom - text_pixel_size;
        }
        if(display_box.top < 0)
        {
            display_box.top = 0;
        }
        
        //根据字符串的最大高度调整左右坐标值
        char_pixel_height=GUI_GetStringHeight(font, text_ptr,len);
        if((display_box.left+char_pixel_height) > display_box.right)
        {
            display_box.left=display_box.right - char_pixel_height;
        }
        if(display_box.left < 0)
        {
            display_box.left=0;
        }
        //end cr148300
    }


    // 获取文本风格
    GetPicTextStyle(&text_style, space, color, font, line_space, edge_color, angle);

    
    des_buf.buffer = (GUI_COLOR_T *)s_pic_editor_info.output_buf_ptr;
    des_buf.mem_width = des_buf.width = s_pic_editor_info.output_pic_width;
    des_buf.height = s_pic_editor_info.output_pic_height;


    actual_rect = GUISTR_DrawTextInRect( 
        &des_buf,
        &display_box,
        &display_box,
        &str_data,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

     *box = actual_rect;
    return actual_rect;
    
}
#endif
/*****************************************************************************/
//  Description : draw text on image
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
//       文字基于图片移动
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIPIC_DrawText(
                                  GUI_RECT_T * box,         //the fixed display area
                                  wchar * text_ptr,         //string to display
                                  uint16 len,               //length of string to display
                                  uint16 space,             //space between two chars.
                                  GUI_COLOR_T color,        //string color
                                  GUI_FONT_T  font,     //font pointer  
                                  uint16 line_space ,      //space between 2 lines       
                                  GUI_COLOR_T edge_color,//font edge color/font frame color)
                                  MMI_PIC_DIRECTION_E direction_type
                                  )
{
    GUI_RECT_T      actual_rect = {0};
    GUI_RECT_T      display_box = {0};//box;
    GUI_RECT_T      disp_rela_rect  = {0};
    GUI_ANGLE_E     angle=ANGLE_0;

    int16           text_pixel_size=0; 
    int16           char_pixel_height=0;
    uint16          screen_right_max_pixel = MMITHEME_GetMainScreenRightPixel();
    uint16          screen_bottom_max_pixel = MMITHEME_GetMainScreenBottomPixel();
    int16           num = 0;
    uint16          str_height  = 0;

    GUISTR_STYLE_T  text_style = {0};/*lint !e64*/
    MMI_STRING_T    str_data = {0};
    GUISTR_STATE_T  state = GUISTR_STATE_EFFECT|GUISTR_STATE_WORDBREAK;
    GUISTR_BUF_T    des_buf = {0};
    
    if (PNULL == text_ptr)
    {
        SCI_MEMSET(&actual_rect, 0x00, sizeof(actual_rect));
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_DrawText text_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_2633_112_2_18_2_45_46_40,(uint8*)"");
        return actual_rect;
    }
    
    // text
    str_data.wstr_len = len;
    str_data.wstr_ptr = text_ptr;
        
    //the max visible display rect(relative to the pic)
    disp_rela_rect = MMIPIC_GetRelativeRect(MMIPIC_CHARACTER_PREVIEW_WIN_ID);
    if(MMI_PIC_DIRECTION_HORIZONTAL == direction_type)
    {           
        display_box=*box;
        angle=ANGLE_0;
        // 获取文本风格
        GetPicTextStyle(&text_style, space, color, font, line_space, edge_color, angle);
        //added, @robert.wang,09-8-14, cr148300
        //根据字符串的长度调整左右坐标值
        text_pixel_size=GUISTR_GetStringWidth(&text_style, &str_data, state);
        if((display_box.left+text_pixel_size) > disp_rela_rect.right)
        {
            display_box.left = disp_rela_rect.right - text_pixel_size;
        }
        if(display_box.left < 0)
        {
            display_box.left = 0;
        }
        display_box.right = disp_rela_rect.right;
        
        //根据字符串的最大高度调整上下坐标值
        char_pixel_height=GUI_GetStringHeight(font, text_ptr,len);
        if((display_box.top+char_pixel_height) > disp_rela_rect.bottom)
        {
            display_box.top= disp_rela_rect.bottom - char_pixel_height;
        }
        if(display_box.top < 0)
        {
            display_box.top=0;
        }
        display_box.bottom = disp_rela_rect.bottom;

        if(text_pixel_size > screen_right_max_pixel)
        {
            num         = text_pixel_size / screen_right_max_pixel + 1;
            str_height  = char_pixel_height * num + line_space * (num - 1);
            if((disp_rela_rect.bottom - display_box.top + 1) < str_height)
            {
                display_box.top = display_box.bottom - str_height;
            }
        }
        //end cr148300
    }
    else
    {
        display_box=*box;
        angle=ANGLE_90;
        state |= GUISTR_STATE_ANGLE;
        // 获取文本风格
        GetPicTextStyle(&text_style, space, color, font, line_space, edge_color, angle);
        //added, @robert.wang,09-8-14, cr148300
        //根据字符串的长度调整上下坐标值
        char_pixel_height=GUISTR_GetStringWidth(&text_style, &str_data, state);
        if((display_box.top + char_pixel_height) > disp_rela_rect.bottom)
        {
            display_box.top = disp_rela_rect.bottom - char_pixel_height;
        }
        if(display_box.top < 0)
        {
            display_box.top = 0;
        }
        display_box.bottom = disp_rela_rect.bottom;
        
        //根据字符串的最大高度调整左右坐标值
        text_pixel_size=GUI_GetStringHeight(font, text_ptr,len);
        if((display_box.left+text_pixel_size) > disp_rela_rect.right)
        {
            display_box.left= disp_rela_rect.right - text_pixel_size;
        }
        if(display_box.left < 0)
        {
            display_box.left=0;
        }
        display_box.right = disp_rela_rect.right;

        if(char_pixel_height > screen_bottom_max_pixel)
        {
            num         = char_pixel_height / screen_bottom_max_pixel + 1;
            str_height  = text_pixel_size * num + line_space * (num - 1);
            if((disp_rela_rect.right - display_box.left + 1) < str_height)
            {
                display_box.left = display_box.right - str_height;
            }
        }
        //end cr148300
    }
    
    des_buf.buffer = (GUI_COLOR_T *)s_pic_editor_info.output_buf_ptr;
    des_buf.mem_width = des_buf.width = s_pic_editor_info.output_pic_width;
    des_buf.height = s_pic_editor_info.output_pic_height;


    actual_rect = GUISTR_DrawTextInRect( 
        &des_buf,
        &display_box,
        &display_box,
        &str_data,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

     *box = actual_rect;
    return actual_rect;
    
}

/*****************************************************************************/
//  Description : move text on image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
//        因为MMIPIC_CopyOrgImgDataToOutputImgData()
//         OUTPUT 大小 与     display_bitmap_ptr 相同
//       故此函数不再修改display_bitmap_ptr 为 OUTPUT 的内容
//       文字基于图片移动
//      modified the function  for cr158180
/*****************************************************************************/
PUBLIC void MMIPIC_MoveText(
                            GUI_RECT_T * box_ptr,       //the fixed display area
                            wchar * text_ptr,       //string to display
                            uint16 len,                 //length of string to display
                            uint16 space,           //space between two chars.
                            GUI_COLOR_T color,      //string color
                            GUI_FONT_T  font,       //font pointer  
                            uint16 line_space ,      //space between 2 lines     
                            GUI_COLOR_T edge_color,//font edge color/font frame color)
                            MMI_PIC_DIRECTION_E direction_type,
                            MMI_MESSAGE_ID_E    msg_id,
                            uint16   char_string_width,
                            uint16   char_string_heigh
                            )
{
    GUI_RECT_T        pic_display_rect= MMIPIC_GetClientWinRect(MMIPIC_CHARACTER_PREVIEW_WIN_ID);
    uint16            screen_right_max_pixel = MMITHEME_GetMainScreenRightPixel();

    GUIIMG_BITMAP_T    bitmap={0};
    int16 step=10;

    GUI_RECT_T  display_box={0};
    uint16 height_step = 0;
    int32 left_max_offset=0;
    GUI_ANGLE_E     angle=ANGLE_0;
    GUISTR_STYLE_T      text_style = {0};/*lint !e64*/
    MMI_STRING_T        str_data = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_EFFECT|GUISTR_STATE_WORDBREAK;
    GUISTR_BUF_T        des_buf = {0};

     //added, @robert.wang, 09-9-11, cr147547
    int16 string_above_height = 0;
    int16 adjust_pic_step = 0;
    int16 adjust_box_step = 0;
    //end cr147547
    GUI_POINT_T dis_point = {0};

    uint16 client_height = pic_display_rect.bottom - pic_display_rect.top + 1;
    int16  client_rect_top = 0;
    int16  client_rect_bottom = 0;

    int32  bg_image_width = 0;
    int32  bg_image_height = 0;
    
    if (PNULL == text_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_MoveText text_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_2804_112_2_18_2_45_46_41,(uint8*)"");
        return ;
    }
    
    if (box_ptr->left < 0 || box_ptr->right <= box_ptr->left)
    {
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_MoveText left or right is error !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_2810_112_2_18_2_45_46_42,(uint8*)"");
        return ;
    }
    
    if (box_ptr->top < 0 || box_ptr->bottom <= box_ptr->top)
    {
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_MoveText top or bottom is error !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_2816_112_2_18_2_45_46_43,(uint8*)"");
        return ;
    }

    if(s_pic_editor_info.output_pic_width > screen_right_max_pixel)
    {
        left_max_offset=s_pic_editor_info.output_pic_width - screen_right_max_pixel;
    }
    
    client_rect_top = pic_display_rect.top;
    client_rect_bottom = pic_display_rect.bottom;

    bg_image_width = s_pic_editor_info.output_pic_width;
    bg_image_height = s_pic_editor_info.output_pic_height;

    
    height_step = client_rect_top / 4;

#ifndef PIC_EDIT_U200_SUPPORT
    pic_display_rect = MMITHEME_GetClientRectEx(MMIPIC_CHARACTER_PREVIEW_WIN_ID);
#endif
     //刷新背景
    GUIRES_DisplayImg(PNULL,
     &pic_display_rect,
     &pic_display_rect,
     MMIPIC_EDIT_MAIN_WIN_ID,
     IMAG_PS_BG,
     &s_pic_editor_info.lcd_dev);

    switch(msg_id)
    {
    case MSG_APP_LEFT:
        if(MMI_PIC_DIRECTION_HORIZONTAL == direction_type)
        {       
            if(box_ptr->left>0)
            {           
                box_ptr->left=box_ptr->left -step;
                box_ptr->right=box_ptr->right-step;
                if(box_ptr->left<0)
                {
                    box_ptr->left=0;
                }
            }
            
            if((box_ptr->left+s_pic_editor_info.output_display_rect.left)<0)
            {
                if(s_pic_editor_info.output_display_rect.left<0)
                {                           
                    s_pic_editor_info.output_display_rect.left=s_pic_editor_info.output_display_rect.left+step;
                    box_ptr->left=(-1)*s_pic_editor_info.output_display_rect.left;
                    if(box_ptr->left<0)
                    {
                        box_ptr->left=0;
                    }
                    if(s_pic_editor_info.output_display_rect.left>0)
                    {
                        s_pic_editor_info.output_display_rect.left=0;
                    }
                }
            }
        }
        else//MMI_PIC_DIRECTION_VERTICAL
        {
            if(box_ptr->right>char_string_width)
            {           
                //box_ptr->left=box_ptr->left -step;
                box_ptr->right=box_ptr->right-step;
                if(box_ptr->right<char_string_width)
                {
                    box_ptr->right=char_string_width;
                }
            }
            
            if((box_ptr->right-char_string_width+s_pic_editor_info.output_display_rect.left)<0)
            {
                if(s_pic_editor_info.output_display_rect.left<0)
                {                           
                    s_pic_editor_info.output_display_rect.left=s_pic_editor_info.output_display_rect.left+step;
                    box_ptr->left=(-1)*s_pic_editor_info.output_display_rect.left;
                    
                    if(box_ptr->left<0)
                    {
                        box_ptr->left=0;
                    }
                    box_ptr->right=box_ptr->left+char_string_width;
                    if(s_pic_editor_info.output_display_rect.left>0)
                    {
                        s_pic_editor_info.output_display_rect.left=0;
                    }
                }
            }
        }
        break;
        
    case MSG_APP_RIGHT:
        if(MMI_PIC_DIRECTION_HORIZONTAL == direction_type)
        {
            if(box_ptr->left+char_string_width< s_pic_editor_info.output_pic_width)
            {                   
                box_ptr->left=box_ptr->left+step;
                box_ptr->right=box_ptr->right+step;
                if(box_ptr->left>s_pic_editor_info.output_pic_width - char_string_width)
                {
                    box_ptr->left=s_pic_editor_info.output_pic_width - char_string_width;
                }
            }
            
            if(box_ptr->left+s_pic_editor_info.output_display_rect.left>screen_right_max_pixel-char_string_width)
            {
                if(s_pic_editor_info.output_display_rect.left+left_max_offset>0 )
                {
                    s_pic_editor_info.output_display_rect.left=s_pic_editor_info.output_display_rect.left-step;
                    if(s_pic_editor_info.output_display_rect.left+left_max_offset<0)
                    {
                        s_pic_editor_info.output_display_rect.left=(-1)*left_max_offset;
                    }
                }
            }
        }
        else
        {
            //if(box_ptr->left+char_string_width<s_pic_editor_info.display_bitmap_ptr->img_width)
            if(box_ptr->right<s_pic_editor_info.output_pic_width )
            {                   
                //box_ptr->left=box_ptr->left+step;
                box_ptr->right=box_ptr->right+step;
                if(box_ptr->right>s_pic_editor_info.output_pic_width )
                {
                    box_ptr->right=s_pic_editor_info.output_pic_width ;
                }
            }
            
            if(box_ptr->right+s_pic_editor_info.output_display_rect.left>screen_right_max_pixel)
            {
                if(s_pic_editor_info.output_display_rect.left+left_max_offset>0 )
                {
                    s_pic_editor_info.output_display_rect.left=s_pic_editor_info.output_display_rect.left-step;
                    if(s_pic_editor_info.output_display_rect.left+left_max_offset<0)
                    {
                        s_pic_editor_info.output_display_rect.left=(-1)*left_max_offset;
                    }
                }
            }
        }
        break;
        
    case MSG_APP_UP:
        if(box_ptr->top>0)
        {           
            box_ptr->top=box_ptr->top-height_step;
            box_ptr->bottom=box_ptr->bottom-height_step;
            if(box_ptr->top<0)
            {
                box_ptr->top=0;
                box_ptr->bottom = box_ptr->top + char_string_heigh;
            }
        }
        
        if((box_ptr->top+s_pic_editor_info.output_display_rect.top) < client_rect_top)
        {
            if(s_pic_editor_info.output_display_rect.top < client_rect_top)
            {
                s_pic_editor_info.output_display_rect.top=s_pic_editor_info.output_display_rect.top+height_step;
                if(s_pic_editor_info.output_display_rect.top > client_rect_top)
                {
                    s_pic_editor_info.output_display_rect.top = client_rect_top;
                }
            }
        }
        break;
        
    case MSG_APP_DOWN:
        //调整字符串的显示位置
        if(box_ptr->top<s_pic_editor_info.output_pic_height - char_string_heigh)
        {
            
            box_ptr->top=box_ptr->top+height_step;
            box_ptr->bottom=box_ptr->bottom+height_step;
            if(box_ptr->top>s_pic_editor_info.output_pic_height -char_string_heigh)
            {
                box_ptr->top=s_pic_editor_info.output_pic_height - char_string_heigh;
            }
        }

           // modified, @robert.wang, 09-9-11, cr147547
           //调整图片的位置
           //当图片大于可视区域时，需要向上移动图片，
           //也需要向上移动文字，
           //  若是剩余空间显示不下一个字时，会换行
          if(s_pic_editor_info.output_pic_height  > client_height )
          {
                //文字显示 超过RECT BOTTOM                
               // if( (box_ptr->top + char_string_heigh) >  client_rect_bottom )  //marked for 先上移图片
                {
                    //向上移动图片
                    if((s_pic_editor_info.output_display_rect.top + MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT) > 0 )
                    {
                        adjust_box_step = height_step;
                        //top to current bottom is iamge up height
                        string_above_height = client_rect_bottom - s_pic_editor_info.output_display_rect.top;
                        adjust_pic_step = s_pic_editor_info.output_pic_height - string_above_height ;
                        if(height_step > adjust_pic_step)
                        {
                                s_pic_editor_info.output_display_rect.top -= adjust_pic_step;
                                adjust_box_step = adjust_pic_step ;
                         }else{
                                 s_pic_editor_info.output_display_rect.top = s_pic_editor_info.output_display_rect.top - height_step;
                         }
                        
                        if((s_pic_editor_info.output_display_rect.top + MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT) < 0 )
                        {
                            s_pic_editor_info.output_display_rect.top = (-1) * MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
                        }

                       //当文字的下端超过图片上移的高度时
                        //向上移动文字
                        if( (box_ptr->top + char_string_heigh + adjust_box_step) > s_pic_editor_info.output_pic_height )
                        {
                             box_ptr->top = box_ptr->top - adjust_box_step;
                            box_ptr->bottom = box_ptr->bottom - adjust_box_step;
                            if(box_ptr->top < 0)
                            {
                                box_ptr->top = 0;
                                box_ptr->bottom = box_ptr->top + char_string_heigh;
                            }
                          }

                     }                
                }
           }
         //end cr147547
        break;
    default:
        break;
    }

    s_pic_editor_info.output_display_rect.right = s_pic_editor_info.output_display_rect.left + bg_image_width - 1;
    s_pic_editor_info.output_display_rect.bottom = s_pic_editor_info.output_display_rect.top + bg_image_height - 1;
    
    if(MMI_PIC_DIRECTION_HORIZONTAL == direction_type)
    {           
        display_box=*box_ptr;
        angle=ANGLE_0;
        
        //根据字符串的长度调整左右坐标值
        if((display_box.left+char_string_width) > display_box.right)
        {
            display_box.left = display_box.right - char_string_width;
        }
        if(display_box.left < 0)
        {
            display_box.left = 0;
        }
        
        //根据字符串的最大高度调整上下坐标值
        if((display_box.top+char_string_heigh) > display_box.bottom)
        {
            display_box.top=display_box.bottom - char_string_heigh;
        }
        if(display_box.top < 0)
        {
            display_box.top=0;
        }       
    }
    else
    {
        display_box=*box_ptr;
        angle=ANGLE_90;
        state |= GUISTR_STATE_ANGLE;
        
        if((display_box.top + char_string_heigh) > display_box.bottom)
        {
            display_box.top = display_box.bottom - char_string_heigh;
        }
        if(display_box.top < 0)
        {
            display_box.top = 0;
        }
        
        //根据字符串的最大高度调整左右坐标值
        if((display_box.left+char_string_width) > display_box.right)
        {
            display_box.left=display_box.right - char_string_width;
        }
        if(display_box.left < 0)
        {
            display_box.left=0;
        }
    }

    *box_ptr=display_box; 
    

    // 获取文本风格
    GetPicTextStyle(&text_style, space, color, font, line_space, edge_color, angle);
    // text
    str_data.wstr_len = len;
    str_data.wstr_ptr = text_ptr;

    //再次将 原来的图片数据 拷贝 到 OUTPUT 
    MMI_MEMCPY(s_pic_editor_info.output_buf_ptr, s_edit_buffer_size,
        s_pic_editor_info.display_bitmap_ptr->bit_ptr, s_edit_buffer_size, s_edit_buffer_size);


    //buffer 
    des_buf.buffer = (GUI_COLOR_T *)s_pic_editor_info.output_buf_ptr;
    des_buf.mem_width = des_buf.width = s_pic_editor_info.output_pic_width;
    des_buf.height = s_pic_editor_info.output_pic_height;

    //end cr158180

    GUISTR_DrawTextInRect( 
        &des_buf,
        &display_box,
        &display_box,
        &str_data,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

     MMI_MEMCPY(&bitmap,sizeof(bitmap),\
              s_pic_editor_info.display_bitmap_ptr,sizeof(*s_pic_editor_info.display_bitmap_ptr),sizeof(bitmap));

    bitmap.bit_ptr=s_pic_editor_info.output_buf_ptr;
    bitmap.img_height=s_pic_editor_info.output_pic_height;
    bitmap.img_width=s_pic_editor_info.output_pic_width;

    dis_point.x = s_pic_editor_info.output_display_rect.left;
    dis_point.y = s_pic_editor_info.output_display_rect.top;
    GUIIMG_DisplayBmp(FALSE,
        &pic_display_rect,
        &dis_point,
        &bitmap,
        &s_pic_editor_info.lcd_dev);
        
    //end cr158180
    
}    


/*****************************************************************************/
//  Description : move osd image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_MoveOSDImage(MMI_MESSAGE_ID_E    msg_id)
{
    uint32              data_size = 0;
    GUI_LCD_DEV_INFO    lcd_dev={GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};            //lcd dev
    GUI_RECT_T        image_rect={0};
    GUI_RECT_T        display_rect={0};
    GUI_RECT_T      osd_background_rect={0};
  
    GUIIMG_BITMAP_T   osd_background_bitmap= {0};
    
    int16 step=10;
    
    GUI_POINT_T dis_point = {0};
    uint16            screen_right_max_pixel = MMITHEME_GetMainScreenRightPixel();
    GUI_RECT_T        client_rect= MMITHEME_GetClientRectEx(MMIPIC_PASTE_PREVIEW_WIN_ID);
    int16             image_above_height=0; //added, @robert.wang, 09-8-23, cr148161

    int32             image_width = s_pic_editor_info.osd_image.width;
    int32             image_height = s_pic_editor_info.osd_image.height;

    uint16   client_height = client_rect.bottom - client_rect.top + 1;
    int16    client_rect_top = 0;
    int16    client_rect_bottom = 0;

    int32    bg_image_width = 0;
    int32    bg_image_height = 0;

    client_rect_top = client_rect.top;
    client_rect_bottom = client_rect.bottom;

    bg_image_width = s_pic_editor_info.output_pic_width;
    bg_image_height = s_pic_editor_info.output_pic_height;
    
    image_rect.left=0;
    image_rect.top=0;
    image_rect.right = image_width - 1;
    image_rect.bottom = image_height - 1;
    
    
    display_rect.left=s_pic_editor_info.osd_display_rect.left;
    display_rect.top=s_pic_editor_info.osd_display_rect.top;
    
    osd_background_rect.left = display_rect.left;
    osd_background_rect.top = display_rect.top;
    osd_background_rect.right=osd_background_rect.left + image_width - 1;
    osd_background_rect.bottom=osd_background_rect.top + image_height - 1;
    
    s_pic_editor_info.osd_display_rect.right = s_pic_editor_info.osd_display_rect.left + image_width - 1;
    s_pic_editor_info.osd_display_rect.bottom = s_pic_editor_info.osd_display_rect.top + image_height - 1;

    switch(msg_id)
    {
    case MSG_APP_UP:
        if(s_pic_editor_info.output_pic_height> client_height)
        {                   
            if((client_rect_top + step)<s_pic_editor_info.osd_display_rect.top)
            {
                s_pic_editor_info.osd_display_rect.top=s_pic_editor_info.osd_display_rect.top-step;
                s_pic_editor_info.osd_display_rect.bottom=s_pic_editor_info.osd_display_rect.bottom-step;                   
            }
            else
            {   if(s_pic_editor_info.output_display_rect.top < client_rect_top)
                {
                    s_pic_editor_info.output_display_rect.top=s_pic_editor_info.output_display_rect.top+step;
                    if(s_pic_editor_info.output_display_rect.top > client_rect_top)
                    {
                        s_pic_editor_info.output_display_rect.top = client_rect_top;
                    }
                }
                else
                {
                    s_pic_editor_info.osd_display_rect.top = client_rect_top + 1;
                    s_pic_editor_info.osd_display_rect.bottom = s_pic_editor_info.osd_display_rect.top + image_height - 1;
                }
            }
        }
        else
        {
            if((s_pic_editor_info.output_display_rect.top+step)<s_pic_editor_info.osd_display_rect.top)
            {
                s_pic_editor_info.osd_display_rect.top=s_pic_editor_info.osd_display_rect.top-step;
                s_pic_editor_info.osd_display_rect.bottom=s_pic_editor_info.osd_display_rect.bottom-step;
            }
            else
            {
                s_pic_editor_info.osd_display_rect.top = s_pic_editor_info.output_display_rect.top + 1;
                s_pic_editor_info.osd_display_rect.bottom = s_pic_editor_info.osd_display_rect.top + image_height - 1;
            }
        }
        break;
        
    case MSG_APP_DOWN:
        if(s_pic_editor_info.output_pic_height> client_height)
        {                   
            if((client_rect_bottom - step)>s_pic_editor_info.osd_display_rect.bottom)
            {
                s_pic_editor_info.osd_display_rect.top=s_pic_editor_info.osd_display_rect.top+step;
                s_pic_editor_info.osd_display_rect.bottom=s_pic_editor_info.osd_display_rect.bottom+step;
            }
            else
            {
               //modified, @robert.wang, 09-8-24, cr148161
                //replace client_rect_top by MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT
                if(s_pic_editor_info.output_display_rect.top+MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT>0 )
                {
                    //top to current bottom is iamge up height
                    image_above_height=client_rect_bottom - s_pic_editor_info.output_display_rect.top;
                    if(step>(s_pic_editor_info.output_pic_height - image_above_height))
                    {
                            s_pic_editor_info.output_display_rect.top -=s_pic_editor_info.output_pic_height - image_above_height;
                     }else{
                             s_pic_editor_info.output_display_rect.top=s_pic_editor_info.output_display_rect.top-step;
                   }
                    
                    if(s_pic_editor_info.output_display_rect.top+MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT<0)
                    {
                        s_pic_editor_info.output_display_rect.top=(-1)*MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
                    }

                    
                }
                else
                {
                    s_pic_editor_info.osd_display_rect.bottom = client_rect_bottom - 1;
                    s_pic_editor_info.osd_display_rect.top = s_pic_editor_info.osd_display_rect.bottom - image_height + 1;
                }
                //end cr148161

            }
        }
        else
        {
            if((s_pic_editor_info.output_display_rect.bottom-step)>s_pic_editor_info.osd_display_rect.bottom)
            {
                s_pic_editor_info.osd_display_rect.top=s_pic_editor_info.osd_display_rect.top+step;
                s_pic_editor_info.osd_display_rect.bottom=s_pic_editor_info.osd_display_rect.bottom+step;
            }
            else
            {
                s_pic_editor_info.osd_display_rect.bottom = s_pic_editor_info.output_display_rect.bottom - 1;
                s_pic_editor_info.osd_display_rect.top = s_pic_editor_info.osd_display_rect.bottom - image_height + 1;
            }
        }
        break;
    case MSG_APP_LEFT:
        if(s_pic_editor_info.output_pic_width>screen_right_max_pixel)
        {                   
            if(step<s_pic_editor_info.osd_display_rect.left)
            {
                s_pic_editor_info.osd_display_rect.left=s_pic_editor_info.osd_display_rect.left-step;
                s_pic_editor_info.osd_display_rect.right=s_pic_editor_info.osd_display_rect.right-step;                 
            }
            else
            {
                if(s_pic_editor_info.output_display_rect.left<0)
                {                       
                    s_pic_editor_info.output_display_rect.left=s_pic_editor_info.output_display_rect.left+step;
                    if(s_pic_editor_info.output_display_rect.left>0)
                    {
                        s_pic_editor_info.output_display_rect.left=0;
                    }
                }
                else
                {
                    s_pic_editor_info.osd_display_rect.left = 1;
                    s_pic_editor_info.osd_display_rect.right = s_pic_editor_info.osd_display_rect.left + image_width - 1;
                }
            }
        }
        else
        {
            if((s_pic_editor_info.output_display_rect.left+step)<s_pic_editor_info.osd_display_rect.left)
            {
                s_pic_editor_info.osd_display_rect.left=s_pic_editor_info.osd_display_rect.left-step;
                s_pic_editor_info.osd_display_rect.right=s_pic_editor_info.osd_display_rect.right-step;
            }
            else
            {
                s_pic_editor_info.osd_display_rect.left = s_pic_editor_info.output_display_rect.left + 1;
                s_pic_editor_info.osd_display_rect.right = s_pic_editor_info.osd_display_rect.left + image_width - 1;
            }
        }
        break;
        
    case MSG_APP_RIGHT:
        if(s_pic_editor_info.output_pic_width>screen_right_max_pixel)
        {       
            int32 left_max_offset=0;            
            
            left_max_offset=s_pic_editor_info.output_pic_width-screen_right_max_pixel;
            
            if((screen_right_max_pixel-step)>s_pic_editor_info.osd_display_rect.right)
            {   
                s_pic_editor_info.osd_display_rect.left=s_pic_editor_info.osd_display_rect.left+step;
                s_pic_editor_info.osd_display_rect.right=s_pic_editor_info.osd_display_rect.right+step;
            }
            else
            {
                if(s_pic_editor_info.output_display_rect.left+left_max_offset>0)
                {
                    s_pic_editor_info.output_display_rect.left=s_pic_editor_info.output_display_rect.left-step;
                    if(s_pic_editor_info.output_display_rect.left+left_max_offset<0)
                    {
                        s_pic_editor_info.output_display_rect.left=(-1)*left_max_offset;
                    }
                }
                else
                {
                    s_pic_editor_info.osd_display_rect.right = screen_right_max_pixel - 1;
                    s_pic_editor_info.osd_display_rect.left = s_pic_editor_info.osd_display_rect.right - image_width + 1;
                }
            }
        }
        else
        {
            if((s_pic_editor_info.output_display_rect.right-step)>s_pic_editor_info.osd_display_rect.right)
            {   
                s_pic_editor_info.osd_display_rect.left=s_pic_editor_info.osd_display_rect.left+step;
                s_pic_editor_info.osd_display_rect.right=s_pic_editor_info.osd_display_rect.right+step;
            }
            else
            {
                s_pic_editor_info.osd_display_rect.right = s_pic_editor_info.output_display_rect.right - 1;
                s_pic_editor_info.osd_display_rect.left = s_pic_editor_info.osd_display_rect.right - image_width + 1;
            }
        }
        break;
        
    default:
        break;
    }

    s_pic_editor_info.output_display_rect.right = s_pic_editor_info.output_display_rect.left + bg_image_width - 1;
    s_pic_editor_info.output_display_rect.bottom = s_pic_editor_info.output_display_rect.top + bg_image_height - 1;


    //再次将 原来的图片数据 拷贝 到 OUTPUT 
    MMI_MEMCPY(s_pic_editor_info.output_buf_ptr, s_edit_buffer_size,
        s_pic_editor_info.display_bitmap_ptr->bit_ptr, s_edit_buffer_size, s_edit_buffer_size);


    MMI_MEMCPY(&osd_background_bitmap,sizeof(osd_background_bitmap),\
              s_pic_editor_info.display_bitmap_ptr,sizeof(*s_pic_editor_info.display_bitmap_ptr),sizeof(osd_background_bitmap));

    //display osd background
    osd_background_bitmap.bit_ptr=s_pic_editor_info.output_buf_ptr;
    osd_background_bitmap.img_height= s_pic_editor_info.output_pic_height ;
    osd_background_bitmap.img_width=  s_pic_editor_info.output_pic_width ;
    
    dis_point.x = s_pic_editor_info.output_display_rect.left;
    dis_point.y = s_pic_editor_info.output_display_rect.top;
    GUIIMG_DisplayBmp(FALSE,
        &client_rect,
        &dis_point,
        &osd_background_bitmap,
        &s_pic_editor_info.lcd_dev);
    
    //display osd image
    IMG_EnableTransparentColor(TRUE);
    data_size = s_pic_editor_info.osd_image.width*s_pic_editor_info.osd_image.height*2;
    GUIRES_DisplayImgByPtr(s_pic_editor_info.osd_image.osd_data_buffer,
        data_size,
        PNULL,
        &s_pic_editor_info.osd_display_rect,
        &image_rect,
        &lcd_dev);
    IMG_EnableTransparentColor(FALSE);
}

/*****************************************************************************/
//  Description : handle tp move osd image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
//  Modifier: Robert.wang,  Date:09-10-27, cr158180,modified by CR251783
/*****************************************************************************/
PUBLIC void MMIPIC_TouchPenMoveOSDImage(GUI_POINT_T point,MMI_MESSAGE_ID_E  msg_id)
{
    static int32     relative_x=0;
    static int32     relative_y=0;
    
    static BOOLEAN  is_tp_press_osd_down=FALSE;
    static BOOLEAN  is_tp_press_background = FALSE;

    BOOLEAN                   is_redraw = TRUE;
    GUI_RECT_T             change_rect = {0};
    GUI_RECT_T      new_osd_rect={0};

    
    uint16          screen_right_max_pixel = MMITHEME_GetMainScreenRightPixel();
    uint32          data_size = 0;
    GUI_RECT_T      image_rect = {0};
    GUI_POINT_T     dis_point = {0};
    GUIIMG_BITMAP_T   osd_background_bitmap= {0};
    
    int32           max_left = 0;
    int32           max_top = 0;
    int32           min_right = 0;
    int32           min_bottom = 0;

    GUI_RECT_T     client_rect = MMITHEME_GetClientRectEx(MMIPIC_PASTE_PREVIEW_WIN_ID);
 
    uint16      height_step = 0;
    uint16      width_step = 0;
    uint16      lcd_width = 0;
    uint16      lcd_height = 0;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    height_step = client_rect.top/2;
	
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    switch(msg_id)
    {
    case MSG_TP_PRESS_DOWN:
        if(GUI_PointIsInRect(point, s_pic_editor_info.osd_display_rect))
        {
            relative_x = point.x - s_pic_editor_info.osd_display_rect.left;
            relative_y = point.y - s_pic_editor_info.osd_display_rect.top;
            is_tp_press_osd_down=TRUE;
            is_tp_press_background = FALSE;
        }
        else
        {
            relative_x = 0;
            relative_y = 0;
            is_tp_press_osd_down=FALSE;
            
            if(GUI_PointIsInRect(point, s_pic_editor_info.output_display_rect)&&GUI_PointIsInRect(point, client_rect))
            {
                relative_x = point.x - s_pic_editor_info.output_display_rect.left;
                relative_y = point.y - s_pic_editor_info.output_display_rect.top;
                is_tp_press_background = TRUE;
            }
            else
            {
                relative_x = 0;
                relative_y = 0;
                is_tp_press_background = FALSE;
            }
        }
        break;

    case MSG_TP_PRESS_MOVE:
        
        //仅仅移动chip image
        if(is_tp_press_osd_down &&!is_tp_press_background)
        {
            //get new rect of osd image 
            new_osd_rect.left = point.x - relative_x;
            new_osd_rect.top = point.y - relative_y;
            new_osd_rect.right = new_osd_rect.left + s_pic_editor_info.osd_image.width - 1;
            new_osd_rect.bottom = new_osd_rect.top + s_pic_editor_info.osd_image.height - 1;

            max_left = MAX(s_pic_editor_info.display_rect.left, 0);
            max_top = MAX(s_pic_editor_info.display_rect.top, client_rect.top); 
            min_right = MIN((int32)s_pic_editor_info.display_rect.right, (int32)screen_right_max_pixel);
            min_bottom = MIN(s_pic_editor_info.display_rect.bottom, client_rect.bottom);


            if (new_osd_rect.left < max_left)
            {
                new_osd_rect.left = max_left ;
                new_osd_rect.right = new_osd_rect.left + s_pic_editor_info.osd_image.width - 1;
            }

            if (new_osd_rect.top < max_top)
            {
                new_osd_rect.top = max_top ;
                
                new_osd_rect.bottom = new_osd_rect.top + s_pic_editor_info.osd_image.height - 1;
            }

            if (new_osd_rect.right > min_right)
            {
                new_osd_rect.right = min_right;
                new_osd_rect.left = new_osd_rect.right - s_pic_editor_info.osd_image.width + 1;
            }

            if (new_osd_rect.bottom > min_bottom)
            {
                new_osd_rect.bottom = min_bottom ;
                new_osd_rect.top = new_osd_rect.bottom - s_pic_editor_info.osd_image.height + 1;
            }
            
        }

        //移动整个图片
        if(is_tp_press_background && !is_tp_press_osd_down)
        {
            change_rect = s_pic_editor_info.output_display_rect;
           if((0<=change_rect.top+2*height_step)&&(change_rect.top<=2*height_step))
            {
                
                change_rect.top=point.y-relative_y;
                if(change_rect.top>2*height_step)
                {
                    change_rect.top=2*height_step;
                }
                if(change_rect.top+2*height_step<0)
                {
                    change_rect.top=-2*height_step;
                }
                
                change_rect.bottom = change_rect.top + s_pic_editor_info.output_pic_height - 1;

                if (change_rect.top > client_rect.top)
                {
                    change_rect.top = client_rect.top;
                    change_rect.bottom = change_rect.top + s_pic_editor_info.output_pic_height - 1;
                }

                if (change_rect.bottom < client_rect.bottom)
                {
                    change_rect.bottom = client_rect.bottom;
                    change_rect.top = change_rect.bottom - s_pic_editor_info.output_pic_height + 1;
                }
                
            }
            
            
            //left and right protect
            if(s_pic_editor_info.output_pic_width>lcd_width)
            {
                width_step=(s_pic_editor_info.output_pic_width -lcd_width)/2;
                change_rect.left=point.x-relative_x;
                if(change_rect.left>0)
                {
                    change_rect.left=0;
                }
                if(change_rect.left+2*width_step<0)
                {
                    change_rect.left=-2*width_step;
                }
            }
            
            if(change_rect.left == s_pic_editor_info.output_display_rect.left && change_rect.top== s_pic_editor_info.output_display_rect.top)
            {
                is_redraw = FALSE;
            }
            else
            {
                is_redraw = TRUE;
                s_pic_editor_info.output_display_rect.left = change_rect.left;
                s_pic_editor_info.output_display_rect.top = change_rect.top;
                s_pic_editor_info.output_display_rect.right = change_rect.right;
                s_pic_editor_info.output_display_rect.bottom= change_rect.bottom;
            }
            
            new_osd_rect.left = s_pic_editor_info.osd_display_rect.left;
            new_osd_rect.top = s_pic_editor_info.osd_display_rect.top;
            new_osd_rect.right = s_pic_editor_info.osd_display_rect.right;
            new_osd_rect.bottom= s_pic_editor_info.osd_display_rect.bottom;
            
            
        }
        
        //重画
        if((is_tp_press_osd_down||is_tp_press_background)&& is_redraw)
        {   
            image_rect.left=0;
            image_rect.top=0;
            image_rect.right=s_pic_editor_info.osd_image.width - 1;
            image_rect.bottom=s_pic_editor_info.osd_image.height - 1;
                 
            s_pic_editor_info.osd_display_rect.left=new_osd_rect.left;
            s_pic_editor_info.osd_display_rect.top=new_osd_rect.top;
            s_pic_editor_info.osd_display_rect.right=new_osd_rect.right;
            s_pic_editor_info.osd_display_rect.bottom=new_osd_rect.bottom;
            
            //再次将 原来的图片数据 拷贝 到 OUTPUT 
            MMI_MEMCPY (
                                    s_pic_editor_info.output_buf_ptr, 
                                    s_edit_buffer_size,
                                    s_pic_editor_info.display_bitmap_ptr->bit_ptr,
                                    s_edit_buffer_size, 
                                    s_edit_buffer_size
                                    );


            MMI_MEMCPY(
                                    &osd_background_bitmap,
                                    sizeof(osd_background_bitmap),
                                    s_pic_editor_info.display_bitmap_ptr,
                                    sizeof(*s_pic_editor_info.display_bitmap_ptr),
                                    sizeof(osd_background_bitmap)
                                    );

            osd_background_bitmap.bit_ptr=s_pic_editor_info.output_buf_ptr;
            osd_background_bitmap.img_height=s_pic_editor_info.output_pic_height; 
            osd_background_bitmap.img_width=s_pic_editor_info.output_pic_width;

            dis_point.x = s_pic_editor_info.output_display_rect.left;
            dis_point.y = s_pic_editor_info.output_display_rect.top;
            GUIIMG_DisplayBmp(
                                                FALSE,
                                                &client_rect,
                                                &dis_point,
                                                &osd_background_bitmap,
                                                &s_pic_editor_info.lcd_dev
                                                );
            
            //display osd image at new position
            IMG_EnableTransparentColor(TRUE);
            data_size = s_pic_editor_info.osd_image.width*s_pic_editor_info.osd_image.height*2;
            GUIRES_DisplayImgByPtr(
                                                        s_pic_editor_info.osd_image.osd_data_buffer,
                                                        data_size,
                                                        PNULL,
                                                        &s_pic_editor_info.osd_display_rect,
                                                        &image_rect,
                                                        &s_pic_editor_info.lcd_dev
                                                        );
            IMG_EnableTransparentColor(FALSE);
            
        }
        
        break;
		
    default:
        break;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
}

//add for CR251783 
/*****************************************************************************/
//  Description : handle tp move osd image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_TouchPenMoveChar(PIC_CHAR_PARAM_T* add_char_info,GUI_COLOR_T* char_color,GUI_POINT_T point,MMI_MESSAGE_ID_E  msg_id)
{
    static int32                 relative_x = 0;
    static int32                 relative_y = 0;
    static BOOLEAN          is_tp_press_down = FALSE;
    static BOOLEAN          is_tp_image_down = FALSE;

    BOOLEAN                   is_redraw = TRUE;
    GUI_RECT_T             pic_display_rect = MMITHEME_GetClientRectEx(MMIPIC_CHARACTER_PREVIEW_WIN_ID);
    uint16                       screen_right_max_pixel = MMITHEME_GetMainScreenRightPixel();

    GUIIMG_BITMAP_T    bitmap = {0};

    GUI_RECT_T             display_box = {0};
    GUI_RECT_T             char_rect={0};
    GUI_RECT_T             change_rect = {0};
    PIC_CHAR_PARAM_T add_char = *(add_char_info);
    
    GUI_ANGLE_E           angle = ANGLE_0;
    GUISTR_STYLE_T      text_style = {0};/*lint !e64*/
    MMI_STRING_T         str_data = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_EFFECT|GUISTR_STATE_WORDBREAK;
    GUISTR_BUF_T          des_buf = {0};
    GUI_POINT_T           dis_point = {0};

    uint16                     bg_image_width = 0;
    uint16                     bg_image_height = 0;

    int32           max_left = 0;
    int32           max_top = 0;
    int32           min_right = 0;
    int32           min_bottom = 0;

    uint16      height_step = 0;
    uint16      width_step = 0;
    uint16      lcd_width = 0;
    uint16      lcd_height = 0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    height_step = pic_display_rect.top/2;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    switch(msg_id)
    {
    case MSG_TP_PRESS_DOWN:
        
        //计算字符显示的区域，绝对地址
        char_rect.left=s_pic_editor_info.output_display_rect.left+add_char.box.left;
        char_rect.top=s_pic_editor_info.output_display_rect.top+add_char.box.top;
        char_rect.right=char_rect.left+add_char.char_string_width;
        char_rect.bottom=char_rect.top+add_char.char_string_heigh;
        
        if(GUI_PointIsInRect(point, char_rect))//判断是否在字符区域
        {
            relative_x = point.x-char_rect.left;
            relative_y = point.y-char_rect.top;
            is_tp_press_down=TRUE;
            is_tp_image_down=FALSE;
        }
        else
        {
            relative_x = 0;
            relative_y = 0;
            is_tp_press_down=FALSE;

            //判断是否在图片显示区域
            if(GUI_PointIsInRect(point, s_pic_editor_info.output_display_rect)&&GUI_PointIsInRect(point, pic_display_rect))
            {
                relative_x = point.x-s_pic_editor_info.output_display_rect.left;
                relative_y = point.y-s_pic_editor_info.output_display_rect.top;
                is_tp_image_down = TRUE;
            }
            else
            {
                relative_x = 0;
                relative_y = 0;
                is_tp_image_down=FALSE;
            }
        }
        break;

    case MSG_TP_PRESS_MOVE:

        max_left = MAX(s_pic_editor_info.display_rect.left, 0);
        max_top = MAX(s_pic_editor_info.display_rect.top, pic_display_rect.top); 
        min_right = MIN((int32)s_pic_editor_info.display_rect.right, (int32)screen_right_max_pixel);
        min_bottom = MIN(s_pic_editor_info.display_rect.bottom, pic_display_rect.bottom);

        if(is_tp_press_down && !is_tp_image_down)
        {
            
            display_box.left = point.x-relative_x;
            display_box.top = point.y-relative_y;
            display_box.right = display_box.left +add_char.char_string_width -1;
            display_box.bottom = display_box.top + add_char.char_string_heigh- 1;
            
            //check left
            if(display_box.left < max_left)
            {
                display_box.left = max_left;
                display_box.right = display_box.left +add_char.char_string_width-1;
            }

           //check right
            if(display_box.right > min_right)
            {
                display_box.right=min_right;
                display_box.left = min_right-add_char.char_string_width+1;
            }
            //check top
            if(display_box.top < max_top)
            {
                display_box.top = max_top;
                display_box.bottom = display_box.top+add_char.char_string_heigh -1;
                
            }
            //check bottom
            if(display_box.bottom> min_bottom)
            {
                display_box.bottom = min_bottom;
                display_box.top = min_bottom-add_char.char_string_heigh+1;
            }

            //计算移动后显示区域，相对地址 
            display_box.left = display_box.left -s_pic_editor_info.output_display_rect.left;
            display_box.top = display_box.top - s_pic_editor_info.output_display_rect.top;
            display_box.right = display_box.left +add_char.char_string_width -1;
            display_box.bottom = display_box.top +add_char.char_string_heigh -1 ;


            add_char_info->box.left = display_box.left;
            add_char_info->box.top = display_box.top;
            add_char_info->box.right = display_box.right;
            add_char_info->box.bottom =  display_box.bottom;
                    
            //s_pic_editor_info.output_display_rect.right = s_pic_editor_info.output_display_rect.left + bg_image_width - 1;
            //s_pic_editor_info.output_display_rect.bottom = s_pic_editor_info.output_display_rect.top + bg_image_height - 1;
  
        }

        if(!is_tp_press_down && is_tp_image_down)
        {
            bg_image_width = s_pic_editor_info.output_pic_width;
            bg_image_height = s_pic_editor_info.output_pic_height;

            change_rect = s_pic_editor_info.output_display_rect;
            
            if((0<=change_rect.top+2*height_step)&&(change_rect.top<=2*height_step))
            {
                
                change_rect.top=point.y-relative_y;
                if(change_rect.top>2*height_step)
                {
                    change_rect.top=2*height_step;
                }
                if(change_rect.top+2*height_step<0)
                {
                    change_rect.top=-2*height_step;
                }
                
               change_rect.bottom = change_rect.top + bg_image_height - 1;

                if (change_rect.top > pic_display_rect.top)
                {
                    change_rect.top = pic_display_rect.top;
                    change_rect.bottom = change_rect.top + bg_image_height - 1;
                }

                if (change_rect.bottom < pic_display_rect.bottom)
                {
                    change_rect.bottom = pic_display_rect.bottom;
                    change_rect.top = change_rect.bottom - bg_image_height + 1;
                }
                
                //MMIPIC_DisplayOutputImage(msg_id);
            }
            
            
            //left and right protect
            if(s_pic_editor_info.output_pic_width>lcd_width)
            {
                width_step=(s_pic_editor_info.output_pic_width -lcd_width)/2;
                change_rect.left=point.x-relative_x;
                if(change_rect.left>0)
                {
                    change_rect.left=0;
                }
                if(change_rect.left+2*width_step<0)
                {
                    change_rect.left=-2*width_step;
                }
               //MMIPIC_DisplayOutputImage(msg_id);
            }

            if(change_rect.left == s_pic_editor_info.output_display_rect.left && change_rect.top== s_pic_editor_info.output_display_rect.top)
            {
                is_redraw = FALSE;
            }
            else
            {
                is_redraw = TRUE;
                s_pic_editor_info.output_display_rect.left = change_rect.left;
                s_pic_editor_info.output_display_rect.top = change_rect.top;
                s_pic_editor_info.output_display_rect.right = change_rect.right;
                s_pic_editor_info.output_display_rect.bottom= change_rect.bottom;
            }

            display_box.left = add_char_info->box.left;
            display_box.top= add_char_info->box.top;
            display_box.right= add_char_info->box.right;
            display_box.bottom= add_char_info->box.bottom;
            
        }

        if((is_tp_press_down || is_tp_image_down) && is_redraw)
        {
            if(MMI_PIC_DIRECTION_HORIZONTAL == add_char.direction_type)
            {
                angle = ANGLE_0; 
            }
            else
            {
                angle = ANGLE_90;
                state |= GUISTR_STATE_ANGLE;
            }

            // 获取文本风格
            GetPicTextStyle(
                                        &text_style, 
                                        add_char.space_two_chars, 
                                        char_color[add_char.color_index], 
                                        add_char.font, 
                                        add_char.space_two_lines,
                                        char_color[add_char.frame_color_index],
                                        angle
                                        );
            // text
            str_data.wstr_len = add_char.length;
            str_data.wstr_ptr = add_char.content;

            //再次将 原来的图片数据 拷贝 到 OUTPUT 
            MMI_MEMCPY(
                                    s_pic_editor_info.output_buf_ptr, 
                                    s_edit_buffer_size,
                                    s_pic_editor_info.display_bitmap_ptr->bit_ptr,
                                    s_edit_buffer_size, 
                                    s_edit_buffer_size
                                    );


            //buffer 
            des_buf.buffer = (GUI_COLOR_T *)s_pic_editor_info.output_buf_ptr;
            des_buf.mem_width = des_buf.width = s_pic_editor_info.output_pic_width;
            des_buf.height = s_pic_editor_info.output_pic_height;

            GUISTR_DrawTextInRect( 
                                                    &des_buf,
                                                    &display_box,
                                                    &display_box,
                                                    &str_data,
                                                    &text_style,
                                                    state,
                                                    GUISTR_TEXT_DIR_AUTO
                                                    );

            MMI_MEMCPY(
                                    &bitmap,
                                    sizeof(bitmap),
                                    s_pic_editor_info.display_bitmap_ptr,
                                    sizeof(*s_pic_editor_info.display_bitmap_ptr),
                                    sizeof(bitmap)
                                    );

            bitmap.bit_ptr=s_pic_editor_info.output_buf_ptr;
            bitmap.img_height=s_pic_editor_info.output_pic_height;
            bitmap.img_width=s_pic_editor_info.output_pic_width;

            dis_point.x = s_pic_editor_info.output_display_rect.left;
            dis_point.y = s_pic_editor_info.output_display_rect.top;

            GUIIMG_DisplayBmp(
                                                FALSE,
                                                &pic_display_rect,
                                                &dis_point,
                                                &bitmap,
                                                &s_pic_editor_info.lcd_dev
                                                );

        }
        
        break;
        
    default:
        break;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
}
//add for CR251783 


/*****************************************************************************/
// Description : convert 565 osd buffer,set original buffer to 555
// Global resource dependence : 
// Author: vincent
// Note:
/*****************************************************************************/
LOCAL void  ConvertBmp565to555OsdImage(
                                       GUI_COLOR_T    colorkey, 
                                       uint16      image_width,         /*image widht*/
                                       uint16      image_height,        /*image height*/
                                       GUI_COLOR_T      * input_565_buf,
                                       GUI_COLOR_T      * output_555_buf  
                                       )
{
    uint32                  pixel_number = 0;
    register uint32         src_pixel_color = 0;
    GUI_COLOR_T             color_key = 0;
    GUI_COLOR_T             *block_buffer_ptr = PNULL;
    register GUI_COLOR_T    *src_buffer_ptr = PNULL;
    register GUI_COLOR_T    *dest_buffer_ptr = PNULL;
    
    if (PNULL == input_565_buf)
    {
        //SCI_TRACE_LOW:"[MMIPIC] ConvertBmp565to555OsdImage input_565_buf is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_4004_112_2_18_2_45_49_44,(uint8*)"");
        return ;
    }

    if (PNULL == output_555_buf)
    {
        //SCI_TRACE_LOW:"[MMIPIC] ConvertBmp565to555OsdImage output_555_buf is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_4010_112_2_18_2_45_49_45,(uint8*)"");
        return ;
    }
    
    if (0 == image_height)
    {
        //SCI_TRACE_LOW:"[MMIPIC] ConvertBmp565to555OsdImage image_height is 0 !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_4016_112_2_18_2_45_49_46,(uint8*)"");
        return ;
    }
    
    if (0 == image_width)
    {
        //SCI_TRACE_LOW:"[MMIPIC] ConvertBmp565to555OsdImage image_width is 0 !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_4022_112_2_18_2_45_49_47,(uint8*)"");
        return ;
    }    
    
    //get pixel number
    pixel_number = image_height*((image_width+(MMIPIC_MIN_UNIT-1))/MMIPIC_MIN_UNIT)*MMIPIC_MIN_UNIT;
    //get block buffer
    block_buffer_ptr = output_555_buf;
    
    //set src and dest block buffer
    dest_buffer_ptr = ((block_buffer_ptr + pixel_number) - 1);
    src_buffer_ptr = ((input_565_buf + pixel_number) - 1);
    
    //get color key
    color_key = colorkey;
    
    do 
    {
        src_pixel_color = *(src_buffer_ptr);
        if(colorkey==src_pixel_color)
        {
            LOOP_565_TO_555(src_pixel_color);                   
            /*if((src_pixel_color & 0x7FFF0000) == color_key_32bit)
            {
            src_pixel_color = src_pixel_color & 0x7FFFFFFF;
        }*/
            if((src_pixel_color & 0x7FFF) == color_key)
            {
                src_pixel_color = src_pixel_color & 0x7FFF;
            }
        }
        *dest_buffer_ptr = src_pixel_color;
        dest_buffer_ptr = dest_buffer_ptr - 1;
        src_buffer_ptr = src_buffer_ptr - 1;
    } while(dest_buffer_ptr >= block_buffer_ptr);
    
}


/*****************************************************************************/
//  Description : get frame image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_GetFrameImage(MMI_IMAGE_ID_T  image_id,MMI_WIN_ID_T win_id)
{
    PEPROCESS_INPUT_T       input_param={0};
    PEPROCESS_OUTPUT_T      output_param={0};
    PEPROCESS_PARAMETER_T   process_param={0};
    uint8                   *frame_data_buffer=PNULL;
    uint8                   *frame_565_buf_ptr=PNULL;
    uint8                   *frame_555_buf_ptr=PNULL;
    uint16                  img_width = 0;
    uint16                  img_height = 0;
    uint32                  img_data_size=0;
    uint32                  frame_buffer_size=0;
    GUI_POINT_T             imagerect={0};
    BOOLEAN                 ret_value = TRUE; 
    PEPROCESS_RESULT_E      ref_return = PEPROCESS_OK;
    uint32 i = 0;   
    uint16 *tmp_ptr = 0;
    uint16 *src_ptr = 0;
    uint32 src_width = 0,src_height = 0;
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;

    GUI_RECT_T      display_rect = MMIPIC_GetClientWinRect(win_id);
    uint16          client_height = display_rect.bottom - display_rect.top + 1;
    int32 offset_x = 0;
    int32 offset_y = 0;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    //get image width and height
    GUIRES_GetImgWidthHeight(&img_width,&img_height,image_id,win_id);
    frame_buffer_size =img_width*img_height*2;


    if ((img_width > lcd_width)
       || (img_height > lcd_height)
       )
    {
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_GetFrameImage FRAME is bigger!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_4103_112_2_18_2_45_49_48,(uint8*)"");
        return FALSE;
    }

    
    //borrow s_pic_editor_info.output_buf_ptr for save frame 565 data
    frame_565_buf_ptr=s_pic_editor_info.output_buf_ptr;
    SCI_MEMSET(frame_565_buf_ptr, 0 ,frame_buffer_size);
    
    frame_data_buffer =(uint8 *)MMI_GetLabelImage(image_id,win_id,&img_data_size);

    //copy image buffer
    GUIRES_CopyImgBuf(frame_data_buffer,
        img_width,
        img_height,
        frame_buffer_size,
        img_data_size,
        &imagerect,
        PNULL,
        (GUI_COLOR_T *)frame_565_buf_ptr);
    
    frame_555_buf_ptr = s_pic_editor_info.process_buf_ptr;
    ConvertBmp565to555OsdImage(
        MMI_OSD_TRANSPARENT_COLOR, 
        img_width,         /*image widht*/
        img_height,               /*image height*/
        (GUI_COLOR_T *)frame_565_buf_ptr, //input
        (GUI_COLOR_T *)frame_555_buf_ptr  //output
        );

    //osd param
    process_param.process_param.photoframe.osd_ptr=frame_555_buf_ptr;
    process_param.process_param.photoframe.colorkey=MMI_BLACK_COLOR;//0x7c1f;//MMI_OSD_TRANSPARENT_COLOR;//MMI_BLACK_COLOR;
    process_param.process_param.photoframe.width = img_width;
    process_param.process_param.photoframe.height = img_height;

    offset_x = (process_param.process_param.photoframe.width - s_pic_editor_info.display_bitmap_ptr->img_width) / 2;
    offset_y = (process_param.process_param.photoframe.height - s_pic_editor_info.display_bitmap_ptr->img_height) / 2;

    process_param.process_param.photoframe.offset_x =(-1) * offset_x;
    process_param.process_param.photoframe.offset_y =(-1) * offset_y;
    
    //set input param  
    src_width = s_pic_editor_info.display_bitmap_ptr->img_width;
    src_height = s_pic_editor_info.display_bitmap_ptr->img_height;
    tmp_ptr = (uint16*)s_pic_editor_info.output_buf_ptr;
    src_ptr = (uint16*)s_pic_editor_info.display_bitmap_ptr->bit_ptr;
    
    if((img_width >= src_width) && (img_height >= src_height))
    {
        uint16 bg_color = MMI_GRAY_WHITE_COLOR;
        
        SCI_MEM16SET(tmp_ptr, bg_color, img_width * img_height * 2);
        tmp_ptr += offset_y * img_width + offset_x;
        
        for (i = 0; i < src_height ;i++)
        {
            SCI_MEMCPY(tmp_ptr, src_ptr, src_width * 2);
            tmp_ptr += img_width;
            src_ptr += src_width;
        }
        input_param.src_ptr=s_pic_editor_info.output_buf_ptr;
        input_param.out_buf_ptr=s_pic_editor_info.output_buf_ptr;
        input_param.out_buf_size=s_edit_buffer_size;
        input_param.input_width=img_width;
        input_param.input_height=img_height;
        input_param.target_width=img_width;
        input_param.target_height=img_height;
        process_param.process_param.photoframe.offset_x = 0;
        process_param.process_param.photoframe.offset_y = 0;
    }
    else
    {
        input_param.src_ptr=s_pic_editor_info.display_bitmap_ptr->bit_ptr;
        input_param.out_buf_ptr=s_pic_editor_info.output_buf_ptr;
        input_param.out_buf_size=s_edit_buffer_size;
        input_param.input_width=s_pic_editor_info.display_bitmap_ptr->img_width;
        input_param.input_height=s_pic_editor_info.display_bitmap_ptr->img_height;
        input_param.target_width= s_pic_editor_info.display_bitmap_ptr->img_width;
        input_param.target_height= s_pic_editor_info.display_bitmap_ptr->img_height;
    }
    input_param.in_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    input_param.out_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    
    //set process_param_ptr param
    process_param.process_buf_ptr = s_pic_editor_info.process_buf_ptr;
    process_param.process_buf_size = s_process_buffer_size;	
    process_param.type = PEPROCESS_PHOTOFRAME;   
    
    //image param
    ref_return = PEPROCESS_ImageProcess(&input_param,&output_param,&process_param);
    if( PEPROCESS_OK != ref_return )
    {
        ret_value = FALSE;
        return ret_value;
    }
    
    s_pic_editor_info.output_pic_width =output_param.actual_out_width;
    s_pic_editor_info.output_pic_height =output_param.actual_out_height;

    //recalculate the display  rect 
    s_pic_editor_info.output_display_rect.left=(lcd_width - s_pic_editor_info.output_pic_width)/2;
    s_pic_editor_info.output_display_rect.right=s_pic_editor_info.output_display_rect.left + s_pic_editor_info.output_pic_width -1;
    s_pic_editor_info.output_display_rect.top= display_rect.top + (int16)(client_height - s_pic_editor_info.output_pic_height) / 2;;
    s_pic_editor_info.output_display_rect.bottom=s_pic_editor_info.output_display_rect.top + s_pic_editor_info.output_pic_height -1;

    return ret_value;
}

/*****************************************************************************/
//  Description : get effect image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_GetEffectImage(MMI_PIC_EFFECT_E  mmi_effect)
{
    PEPROCESS_INPUT_T     input_param={0};
    PEPROCESS_OUTPUT_T      output_param={0};
    PEPROCESS_PARAMETER_T  process_param={0};
    BOOLEAN ret_value = TRUE; 
    PEPROCESS_RESULT_E ref_return = PEPROCESS_OK;

    //set input param   
    input_param.src_ptr=s_pic_editor_info.display_bitmap_ptr->bit_ptr;
    input_param.out_buf_ptr=s_pic_editor_info.output_buf_ptr;
    input_param.out_buf_size=s_edit_buffer_size;
    input_param.input_width=s_pic_editor_info.display_bitmap_ptr->img_width;
    input_param.input_height=s_pic_editor_info.display_bitmap_ptr->img_height;
    input_param.target_width=s_pic_editor_info.display_bitmap_ptr->img_width;
    input_param.target_height=s_pic_editor_info.display_bitmap_ptr->img_height;
    input_param.in_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    input_param.out_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    
    //set process_param_ptr param
    process_param.process_buf_ptr = s_pic_editor_info.process_buf_ptr;
//process buffer size changes from 240*320*2 to 240*320*3
    process_param.process_buf_size = s_process_buffer_size;	    
    switch(mmi_effect)
    {   
    case MMI_PIC_EFFECT_GRAY:
        process_param.process_param.effect.type=PEPROCESS_EFFECT_GRAY;
        process_param.type = PEPROCESS_EFFECT_PROCESS;
        break;
        
    case MMI_PIC_EFFECT_SEPIA:
        process_param.process_param.effect.type=PEPROCESS_EFFECT_SEPIA;
        process_param.type = PEPROCESS_EFFECT_PROCESS;
        break;
    case MMI_PIC_EFFECT_SMOOTH:
        process_param.process_param.effect.type=0;/*lint !e64*/
        process_param.type = PEPROCESS_SMOOTH;
        break;
        
    case MMI_PIC_EFFECT_EMBOSS:
        process_param.process_param.effect.type=PEPROCESS_EFFECT_EMBOSS;
        process_param.type = PEPROCESS_EFFECT_PROCESS;
        break;
        
    case MMI_PIC_EFFECT_SHARPEN:
        process_param.process_param.sharpen.threshold=32;
        process_param.process_param.sharpen.rate=32;
        process_param.type = PEPROCESS_SHARPEN;
        break;
        
    case MMI_PIC_EFFECT_DIFFUSION:
        process_param.process_param.effect.type=PEPROCESS_EFFECT_DIFFUSION;
        process_param.type = PEPROCESS_EFFECT_PROCESS;
        break;
        
    case MMI_PIC_EFFECT_EXPOSURE:
        process_param.process_param.effect.type=PEPROCESS_EFFECT_EXPOSURE;
        process_param.type = PEPROCESS_EFFECT_PROCESS;
        break;
    case MMI_PIC_EFFECT_NEGATIVE_RGB:
        process_param.process_param.effect.type=PEPROCESS_EFFECT_NEGATIVE_RGB;
        process_param.type = PEPROCESS_EFFECT_PROCESS;
        break;
    case MMI_PIC_EFFECT_DENOISE:
        process_param.process_param.denoise.threshold=8;
        process_param.type = PEPROCESS_IMAGE_DENOISE;
        break;
        
    default:
        //SCI_TRACE_LOW:"MMIPIC_GetEffectImage:effort error mmi_effect =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_4285_112_2_18_2_45_49_49,(uint8*)"d",mmi_effect);
        break;
    }
    
    
    //image param
    ref_return = PEPROCESS_ImageProcess(&input_param,&output_param,&process_param);
    if( PEPROCESS_OK != ref_return )
    {
        ret_value = FALSE;
        return ret_value;
    }
    
    s_pic_editor_info.output_pic_width =output_param.actual_out_width;
    s_pic_editor_info.output_pic_height =output_param.actual_out_height;

    return ret_value;
}




/*****************************************************************************/
//  Description : resize image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_ResizeImage(MMI_PIC_RESIZE_E resize)
{
    PEPROCESS_INPUT_T     input_param={0};
    PEPROCESS_OUTPUT_T  output_param={0};
    PEPROCESS_PARAMETER_T  process_param={0};
    BOOLEAN ret_value = TRUE; 
    PEPROCESS_RESULT_E ref_return = PEPROCESS_OK;
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;

    GUI_RECT_T      display_rect=MMITHEME_GetClientRectEx(MMIPIC_RESIZE_PREVIEW_WIN_ID);
    uint16          client_height = MMITHEME_GetClientRectHeightEx(MMIPIC_RESIZE_PREVIEW_WIN_ID);

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    
    input_param.src_ptr=s_pic_editor_info.display_bitmap_ptr->bit_ptr;//s_pic_editor_info.undo_buf_ptr;
    input_param.out_buf_ptr=s_pic_editor_info.output_buf_ptr;//s_pic_editor_info.output_buf_ptr;
    input_param.out_buf_size=s_edit_buffer_size;
    
    input_param.input_width=s_pic_editor_info.display_bitmap_ptr->img_width;
    input_param.input_height=s_pic_editor_info.display_bitmap_ptr->img_height;
    
    input_param.in_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    input_param.out_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    
    
    //set process_param_ptr param
    process_param.process_buf_ptr=s_pic_editor_info.process_buf_ptr;
//process buffer size changes from 240*320*2 to 240*320*2 +72K
    process_param.process_buf_size = s_process_buffer_size;	    
    output_param.actual_out_height=0;
    output_param.actual_out_width=0;
    
    switch(resize)
    {
    case MMI_PIC_RESIZE_LARGE:
        input_param.target_width=input_param.input_width;
        input_param.target_height=input_param.input_height;
        break;
    case MMI_PIC_RESIZE_MIDDLE:
        if(0==input_param.input_width/2)
        {
            input_param.target_width=input_param.input_width;
        }
        else
        {
            input_param.target_width=input_param.input_width/2;
        }
        if(0==input_param.input_height/2)
        {
            input_param.target_height=input_param.input_height;
        }
        else
        {
            input_param.target_height=input_param.input_height/2;
        }
        break;
    case MMI_PIC_RESIZE_LITTLE:
        if(0==input_param.input_width/4)
        {
            input_param.target_width=1;
        }
        else
        {
            input_param.target_width=input_param.input_width/4;
        }
        if(0==input_param.input_height/4)
        {
            input_param.target_height=1;
        }
        else
        {
            input_param.target_height=input_param.input_height/4;
        }
        break;
    default:
        break;
    }
    
    process_param.type = PEPROCESS_SCALINGDOWN;
    
    //image param
    ref_return = PEPROCESS_ImageProcess(&input_param,&output_param,&process_param);
    if( PEPROCESS_OK != ref_return )
    {
        ret_value = FALSE;
        return ret_value;
    }

    //modified, @robert.wang, 09-10-26, cr158180

     s_pic_editor_info.output_pic_width = output_param.actual_out_width;
     s_pic_editor_info.output_pic_height = output_param.actual_out_height;

    //recalculate the display  rect 
    s_pic_editor_info.output_display_rect.top= display_rect.top + (int16)(client_height - output_param.actual_out_height) / 2;
    s_pic_editor_info.output_display_rect.left=(lcd_width-output_param.actual_out_width)/2;
    s_pic_editor_info.output_display_rect.right=s_pic_editor_info.output_display_rect.left+output_param.actual_out_width -1;
    s_pic_editor_info.output_display_rect.bottom=s_pic_editor_info.output_display_rect.top+output_param.actual_out_height -1;
    //end cr158180

    return ret_value;
}



/*****************************************************************************/
//  Description : adjust offset value
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL uint32 AdjustOffset(int32 offset,BOOLEAN is_add,int32 down_limit,int32 up_limit)
{
    int32 step=(up_limit-down_limit)/8;//左右各四级。
    
    if (down_limit >= up_limit)
    {
        //SCI_TRACE_LOW:"[MMIPIC] AdjustOffset down is error !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_4428_112_2_18_2_45_50_50,(uint8*)"");
        return offset;
    }
    
    if(is_add)
    {
//        if(offset<up_limit)
        if((offset+step)<=up_limit)
        {
            offset=offset+step;
        }
        else
        {
            offset=up_limit;
        }
    }
    else
    {
//        if(offset>(down_limit))
        if((offset-step)>=(down_limit))
        {
            offset=offset-step;
        }
        else
        {
            offset=down_limit;
        }
    }
    return offset;
}
/*****************************************************************************/
//  Description : resert adjustment
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_ResetAdjustment(void)
{
    s_degree=PEPROCESS_HUE_DEGREE_0;
    s_constract_gain=64;
    s_saturation_gain=64;
    s_bright_offset=0;
}
/*****************************************************************************/
//  Description : resert RGB value
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_ResetRGB(void)
{
    s_R_offset=0;
    s_G_offset=0;
    s_B_offset=0;
}
/*****************************************************************************/
//  Description : adjust RGB value
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_AdjustRGB(PEPROCESS_COLOR_BALANCE_E color,BOOLEAN is_add_RGB)
{
    PEPROCESS_INPUT_T     input_param={0};
    PEPROCESS_OUTPUT_T  output_param={0};
    PEPROCESS_PARAMETER_T  process_param={0};
    BOOLEAN ret_value = TRUE; 
    PEPROCESS_RESULT_E ref_return = PEPROCESS_OK;

    
    input_param.src_ptr = s_pic_editor_info.display_bitmap_ptr->bit_ptr;
    input_param.out_buf_ptr=s_pic_editor_info.output_buf_ptr;//s_pic_editor_info.output_buf_ptr;
    input_param.out_buf_size=s_edit_buffer_size;

    //SCI_TRACE_LOW:"[MMIPIC]: MMIPIC_AdjustRGB, input_param.src_ptr = 0x%x, input_param.out_buf_ptr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_4496_112_2_18_2_45_50_51,(uint8*)"dd", input_param.src_ptr, input_param.out_buf_ptr);
    if(PNULL==input_param.src_ptr || PNULL==input_param.out_buf_ptr)
    {
        return FALSE;
    }

    input_param.input_width=s_pic_editor_info.display_bitmap_ptr->img_width;
    input_param.input_height=s_pic_editor_info.display_bitmap_ptr->img_height;
    
    input_param.in_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    input_param.out_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    
    input_param.target_width=input_param.input_width;
    input_param.target_height=input_param.input_height;
    
    //set process_param_ptr param
    process_param.process_buf_ptr=s_pic_editor_info.process_buf_ptr;
//process buffer size changes from 240*320*2 to 240*320*3
    process_param.process_buf_size = s_process_buffer_size;	    
    output_param.actual_out_height=0;
    output_param.actual_out_width=0;
    
    process_param.type = PEPROCESS_GROUP_COLOR_BALANCE;

    switch(color)
    {
    case PEPROCESS_CYAN_TO_RED:
        s_R_offset=AdjustOffset(s_R_offset,is_add_RGB,-100,100);
        break;
    case PEPROCESS_MAGENTA_TO_GREEN:
        s_G_offset=AdjustOffset(s_G_offset,is_add_RGB,-100,100);
        break;
    case PEPROCESS_YELLOW_TO_BLUE:
        s_B_offset=AdjustOffset(s_B_offset,is_add_RGB,-100,100);
        break;
    default:
        break;
    }

    process_param.process_param.group_color_balance.cyna_to_red = s_R_offset;
    process_param.process_param.group_color_balance.magenta_to_green = s_G_offset;
    process_param.process_param.group_color_balance.yellow_to_blue = s_B_offset;

    ref_return = PEPROCESS_ImageProcess(&input_param,&output_param,&process_param);
    if( PEPROCESS_OK != ref_return )
    {
        ret_value = FALSE;
        return ret_value;
    }
    

    s_pic_editor_info.output_pic_width = output_param.actual_out_width;
    s_pic_editor_info.output_pic_height = output_param.actual_out_height;

    return ret_value;
}
/*****************************************************************************/
//  Description : adjust bright ,constract,hue,saturation
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_Adjustment(MMI_PIC_ADJUSTMENT_E adjustment,BOOLEAN is_add)
{
    PEPROCESS_INPUT_T     input_param={0};
    PEPROCESS_OUTPUT_T  output_param={0};
    PEPROCESS_PARAMETER_T  process_param={0};
    BOOLEAN ret_value = TRUE; 
    PEPROCESS_RESULT_E ref_return = PEPROCESS_OK;
    
    input_param.src_ptr = s_pic_editor_info.display_bitmap_ptr->bit_ptr;
    input_param.out_buf_ptr=s_pic_editor_info.output_buf_ptr;//s_pic_editor_info.output_buf_ptr;
    input_param.out_buf_size=s_edit_buffer_size;
    
    input_param.input_width=s_pic_editor_info.display_bitmap_ptr->img_width;
    input_param.input_height=s_pic_editor_info.display_bitmap_ptr->img_height;
    
    input_param.in_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    input_param.out_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    
    input_param.target_width=input_param.input_width;
    input_param.target_height=input_param.input_height;
    
    //set process_param_ptr param
    process_param.process_buf_ptr=s_pic_editor_info.process_buf_ptr;
//process buffer size changes from 240*320*2 to 240*320*3
    process_param.process_buf_size = s_process_buffer_size;	    
    output_param.actual_out_height=0;
    output_param.actual_out_width=0;
    
    switch(adjustment)
    {
    case MMI_PIC_ADJUSTMENT_BRIGHTNESS:
        s_bright_offset=AdjustOffset(s_bright_offset,is_add,-100,100);
        break;
        
    case MMI_PIC_ADJUSTMENT_CONSTRACT:
        s_constract_gain=AdjustOffset(s_constract_gain,is_add,0,128);
        if(0>=s_constract_gain)
        {
            s_constract_gain=1;
        }
        break;
        
    case MMI_PIC_ADJUSTMENT_SATURATION:
        s_saturation_gain=AdjustOffset(s_saturation_gain,is_add,0,128);
        break;
        
    case MMI_PIC_ADJUSTMENT_HUE:
        if(is_add)
        {
            if(PEPROCESS_HUE_DEGREE_315 != s_degree)
            {
                s_degree++;
            }
        }
        else
        {
            if(PEPROCESS_HUE_DEGREE_0 != s_degree)
            {
                s_degree--;
            }
        }
        break;
        
    default:
        break;
    }
    if((MMI_PIC_ADJUSTMENT_BRIGHTNESS==adjustment)||(MMI_PIC_ADJUSTMENT_CONSTRACT==adjustment))    
    {
        process_param.type = PEPROCESS_GROUP_BRIGHTNESS_CONTRAST;
        process_param.process_param.group_brightness_contrast.brightness = s_bright_offset;
        process_param.process_param.group_brightness_contrast.contrast = s_constract_gain;        
    }
    else if((MMI_PIC_ADJUSTMENT_SATURATION==adjustment)||(MMI_PIC_ADJUSTMENT_HUE==adjustment))
    {
        process_param.type = PEPROCESS_GROUP_SATURATION_HUE;
        process_param.process_param.group_saturation_hue.saturation= s_saturation_gain;
        process_param.process_param.group_saturation_hue.hue = s_degree;        
    }
    ref_return = PEPROCESS_ImageProcess(&input_param,&output_param,&process_param);
    if( PEPROCESS_OK != ref_return )
    {
        ret_value = FALSE;
        return ret_value;
    }
    

    s_pic_editor_info.output_pic_width =output_param.actual_out_width;
    s_pic_editor_info.output_pic_height =output_param.actual_out_height;

    return ret_value;
}

/*****************************************************************************/
//  Description : judge if the bmp image can be saved as jpg file accroding to its width and height
//  Global resource dependence : 
//  Author: JerryLiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPIC_IfBmpCanBeProcessed(void)
{
    if( (16 > s_pic_editor_info.display_bitmap_ptr->img_width)
        ||( 16 > s_pic_editor_info.display_bitmap_ptr->img_height ))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description : judege if the bmp can be pasted the clip according its width and height
//  Global resource dependence : 
//  Author: JerryLiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPIC_IfBmpCanBePasted(void)
{
    BOOLEAN ret = FALSE;
    uint16 image_width = 0;
    uint16 image_height = 0;

    //get resources width or height
    ret = GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_PIC_ANGEL, MMIPIC_EDIT_MAIN_WIN_ID);
    if (!ret)
    {
        return FALSE;
    }
    
    if( (image_width > s_pic_editor_info.display_bitmap_ptr->img_width)
        ||( image_height > s_pic_editor_info.display_bitmap_ptr->img_height ))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description : judege if the bmp can be add character according to its width and height
//  Global resource dependence : 
//  Author: JerryLiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPIC_IfBmpCanBeAddCharacter(void)
{
    if( (MMI_PIC_CHAR_LARGE_FONT_SIZE > s_pic_editor_info.display_bitmap_ptr->img_width)
        ||( MMI_PIC_CHAR_LARGE_FONT_SIZE > s_pic_editor_info.display_bitmap_ptr->img_height ))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description : get random number
//  Global resource dependence : 
//  Author: Robert.wang
//  Note: 
/*****************************************************************************/
LOCAL uint16  MMIPIC_GetRandomNum(void)
{
    srand(SCI_GetTickCount());
    return rand();
}

/*****************************************************************************/
//  Description : get file latter name to combine full file name
//  Global resource dependence : 
//  Author: Robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIPIC_GetSaveLatterName(void)
{
    uint16 *full_path_16bit_ptr = PNULL;
    uint16 full_path_16bit_len = 0;
    wchar no_suffix_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16 no_suffix_name_len = 0;
    char byte_random_string[10] = {0};
    wchar wchar_random_string[10] = {0};
    int16 i=0;
    uint16 latter_name_len = 0;
    uint16 random_num = 0;
    BOOLEAN is_file_exist = FALSE;
    int8 count = 0;

    do
    {
        full_path_16bit_ptr = s_pic_editor_info.file_info.full_path_wstr_ptr;
        full_path_16bit_len = (uint16)(s_pic_editor_info.file_info.full_path_wstr_len);

        SCI_MEMSET(byte_random_string, 0, sizeof(byte_random_string));
        SCI_MEMSET(wchar_random_string, 0, sizeof(wchar_random_string));
        
        random_num = MMIPIC_GetRandomNum();
        sprintf(byte_random_string, "_p%03d", random_num % 1000);
        MMIAPICOM_StrToWstr( byte_random_string,wchar_random_string );/*lint !e64*/
        latter_name_len = strlen(byte_random_string);
        
        //find the last '.'
        for (i = (uint16)(full_path_16bit_len-1); i >= 0; i--)
        {
            if (MMIPIC_DOT == full_path_16bit_ptr[i])
            {
                break;
            }
        }
        
        //copy path ,copy "D:\picture\***"
        g_random_name_len = i;
        SCI_MEMSET(g_random_name_ptr,0,(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1)));
        MMI_WSTRNCPY(g_random_name_ptr,MMIFILE_FILE_NAME_MAX_LEN,
            s_pic_editor_info.file_info.full_path_wstr_ptr,g_random_name_len,
            g_random_name_len);

        MMIAPIFMM_SplitFullPath(
            g_random_name_ptr,
            g_random_name_len,
            PNULL, 
            PNULL,
            PNULL,
            PNULL,
            no_suffix_name,
            &no_suffix_name_len
            );

        if((full_path_16bit_len + latter_name_len)<=MMIFILE_FULL_PATH_MAX_LEN)
        {
            MMI_WSTRNCPY(g_random_name_ptr+g_random_name_len,(MMIFILE_FULL_PATH_MAX_LEN-full_path_16bit_len),
                wchar_random_string, latter_name_len, latter_name_len);
            g_random_name_len += latter_name_len;
        }
        else if(((full_path_16bit_len + latter_name_len)>MMIFILE_FULL_PATH_MAX_LEN)&&(no_suffix_name_len>= latter_name_len))
        {
            MMI_WSTRNCPY(g_random_name_ptr+g_random_name_len - latter_name_len, latter_name_len,
                wchar_random_string, latter_name_len, latter_name_len);
        }
        else if(((full_path_16bit_len + latter_name_len)>MMIFILE_FULL_PATH_MAX_LEN)&&(no_suffix_name_len < latter_name_len))
        {
            MMI_WSTRNCPY(g_random_name_ptr+g_random_name_len-no_suffix_name_len,no_suffix_name_len,
                (wchar_random_string+(latter_name_len - no_suffix_name_len)),no_suffix_name_len,no_suffix_name_len);
        }
        
        //copy file name suffix ".jpg"  MMIPIC_SAVE_SUBFIX
        MMI_WSTRNCPY((g_random_name_ptr+g_random_name_len),(MMIFILE_FULL_PATH_MAX_LEN - g_random_name_len),
            MMIPIC_SAVE_SUBFIX,MMIPIC_SAVE_SUBFIX_LEN,
            MMIPIC_SAVE_SUBFIX_LEN);
        g_random_name_len += MMIPIC_SAVE_SUBFIX_LEN;

        is_file_exist = MMIAPIFMM_IsFileExist(g_random_name_ptr, g_random_name_len);
        count++;
    }while(is_file_exist && count < 3);
    
}

/*****************************************************************************/
//  Description : get the current picture's width and height
//  Global resource dependence : 
//  Author: Robert.wang
//  Date:  2009-8-6
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_GetPictureHeightAndWidth(
                                           int16    *height,  //out
                                           int16    *width      //out
                                           )
{
    *height = s_pic_editor_info.display_bitmap_ptr->img_height;
    *width = s_pic_editor_info.display_bitmap_ptr->img_width;
}

/*****************************************************************************/
//  Description : reset picture rect
//  Global resource dependence : 
//  Author: Robert.wang
//  Date:  2009-10-26
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_ResetPictureDisplayRect(void)
{
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;
    GUI_RECT_T      display_rect=MMIPIC_GetClientWinRect(MMIPIC_CHARACTER_PREVIEW_WIN_ID);
    uint16          client_height = display_rect.bottom - display_rect.top + 1;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    s_pic_editor_info.display_rect.left   = (lcd_width - s_pic_editor_info.display_bitmap_ptr->img_width)/2;
    s_pic_editor_info.display_rect.top    = display_rect.top + (int16)(client_height - s_pic_editor_info.display_bitmap_ptr->img_height) / 2;
    s_pic_editor_info.display_rect.right  = s_pic_editor_info.display_rect.left + (int16)s_pic_editor_info.display_bitmap_ptr->img_width - 1;
    s_pic_editor_info.display_rect.bottom = s_pic_editor_info.display_rect.top + (int16)s_pic_editor_info.display_bitmap_ptr->img_height - 1;
}

/*****************************************************************************/
//  Description : set picture RGB value as grade
//  Global resource dependence : 
//  Author: Robert.wang
//  Note: 设置RGB颜色值的等级，共9级(0~8)
//          颜色值(-100 ~ 100)对应0~8级
//  Param: [Return] value of MMI_PIC_ERROR_TYPE_E
/*****************************************************************************/
PUBLIC MMI_PIC_ERROR_TYPE_E MMIPIC_SetRGBValue(int32 grade,  // [in] 0~8
                                    PEPROCESS_COLOR_BALANCE_E r_g_b  // [in] 0~2
                                    ) 
{
    int32  offset = 0;    
    
    PEPROCESS_INPUT_T     input_param={0};
    PEPROCESS_OUTPUT_T  output_param={0};
    PEPROCESS_PARAMETER_T  process_param={0};
    MMI_PIC_ERROR_TYPE_E ret_value = MMI_PIC_SUCCESS; 
    PEPROCESS_RESULT_E ref_return = PEPROCESS_OK;

    //检查参数是否有效
   //若是无效参数，返回成功，
   //不作响应
    if(grade < 0 || grade > 8)
    {
        return ret_value;
    }

    switch(grade)
    {
    case 0:
        offset = -100;
        break;
    case 1:
        offset = -75;
        break;
    case 2:
        offset = -50;
        break;
    case 3:
        offset = -25;
        break;
    case 4:
        offset =  0;
        break;
    case 5:
        offset = 25;
        break;
    case 6:
        offset = 50;
        break;
    case 7:
        offset = 75;
        break;
    case 8:
        offset = 100;
        break;
    default:
        offset = 0;
        break;
    }

    switch(r_g_b)
    {
    case PEPROCESS_CYAN_TO_RED:
        s_R_offset = offset; 
        break;
    case PEPROCESS_MAGENTA_TO_GREEN:
        s_G_offset = offset;
        break;
    case PEPROCESS_YELLOW_TO_BLUE:
        s_B_offset = offset;
        break;
    default:
        s_R_offset = offset;
        break;
    }

    input_param.src_ptr = s_pic_editor_info.display_bitmap_ptr->bit_ptr;

    input_param.out_buf_ptr=s_pic_editor_info.output_buf_ptr;//s_pic_editor_info.output_buf_ptr;
    input_param.out_buf_size=s_edit_buffer_size;

    //SCI_TRACE_LOW:"[MMIPIC]: MMIPIC_SetRGBGradeValue, input_param.src_ptr = 0x%x, input_param.out_buf_ptr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_4922_112_2_18_2_45_51_52,(uint8*)"dd", input_param.src_ptr, input_param.out_buf_ptr);
    if(PNULL==input_param.src_ptr || PNULL==input_param.out_buf_ptr)
    {
        ret_value = MMI_PIC_ERR_EXIT;
        return ret_value;
    }
    
    input_param.input_width=s_pic_editor_info.display_bitmap_ptr->img_width;
    input_param.input_height=s_pic_editor_info.display_bitmap_ptr->img_height;
    
    input_param.in_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    input_param.out_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    
    input_param.target_width=input_param.input_width;
    input_param.target_height=input_param.input_height;
    
    //set process_param_ptr param
    process_param.process_buf_ptr=s_pic_editor_info.process_buf_ptr;
    process_param.process_buf_size = s_process_buffer_size;	    
    output_param.actual_out_height=0;
    output_param.actual_out_width=0;
    
    process_param.type = PEPROCESS_GROUP_COLOR_BALANCE;

    process_param.process_param.group_color_balance.cyna_to_red = s_R_offset;
    process_param.process_param.group_color_balance.magenta_to_green = s_G_offset;
    process_param.process_param.group_color_balance.yellow_to_blue = s_B_offset;

    ref_return = PEPROCESS_ImageProcess(&input_param,&output_param,&process_param);
    if( PEPROCESS_OK != ref_return )
    {
        ret_value = MMI_PIC_ERR_EXIT;
        return ret_value;
    }
    
    s_pic_editor_info.output_pic_width = output_param.actual_out_width;
    s_pic_editor_info.output_pic_height = output_param.actual_out_height;

    return ret_value;
}

/*****************************************************************************/
//  Description : get picture adjustment value by grade
//  Global resource dependence : 
//  Author: Robert.wang
//  Note:     brightness (-100 ~ 100) 对应0~8级
//            constract  (0 ~ 128)    对应0~8级
//            saturation  (0 ~ 128)   对应0~8级
//            hue         (0 ~ 315)   对应0~8级
//  Param: [Return] adjustment value 
/*****************************************************************************/
LOCAL int32 MMIPIC_GetAdjustmentValue(int32 grade,   // [in]  0~8
                                    MMI_PIC_ADJUSTMENT_E adjustment  //[in] which adjustment
                                    )
{
    int adjustment_value = 0;
    
    switch(adjustment)
    {
    case MMI_PIC_ADJUSTMENT_BRIGHTNESS:
        //-100,100
        switch(grade)
        {
        case 0:
            adjustment_value = -100;
            break;
        case 1:
            adjustment_value = -75;
            break;
        case 2:
            adjustment_value = -50;
            break;
        case 3:
            adjustment_value = -25;
            break;
        case 4:
            adjustment_value =  0;
            break;
        case 5:
            adjustment_value = 25;
            break;
        case 6:
            adjustment_value = 50;
            break;
        case 7:
            adjustment_value = 75;
            break;
        case 8:
            adjustment_value = 100;
            break;
        default:
            adjustment_value = 0;
            break;
        }
        break;        
    case MMI_PIC_ADJUSTMENT_CONSTRACT:  // same grade 
    case MMI_PIC_ADJUSTMENT_SATURATION: // same grade
        // 0~ 128
        switch(grade)
        {
        case 0:
            if(MMI_PIC_ADJUSTMENT_CONSTRACT == adjustment)
            {
                adjustment_value = 1;
            }else{
                adjustment_value = 0;
            }
            break;
        case 1:
            adjustment_value = 16;
            break;
        case 2:
            adjustment_value = 32;
            break;
        case 3:
            adjustment_value = 48;
            break;
        case 4:
            adjustment_value = 64;
            break;
        case 5:
            adjustment_value = 80;
            break;
        case 6:
            adjustment_value = 96;
            break;
        case 7:
            adjustment_value = 112;
            break;
        case 8:
            adjustment_value = 128;
            break;
        default:
            adjustment_value = 64;
            break;
        }
        break;     
    case MMI_PIC_ADJUSTMENT_HUE:
        // PEPROCESS_HUE_DEGREE_0~ PEPROCESS_HUE_DEGREE_315
        switch(grade)
        {
        case 0:
            adjustment_value = PEPROCESS_HUE_DEGREE_0;
            break;
        case 1:
            adjustment_value = PEPROCESS_HUE_DEGREE_45;
            break;
        case 2:
            adjustment_value = PEPROCESS_HUE_DEGREE_90;
            break;
        case 3:
            adjustment_value = PEPROCESS_HUE_DEGREE_135;
            break;
        case 4:
            adjustment_value = PEPROCESS_HUE_DEGREE_180;
            break;
        case 5:
            adjustment_value = PEPROCESS_HUE_DEGREE_225;
            break;
        case 6:
            adjustment_value = PEPROCESS_HUE_DEGREE_270;
            break;
        case 7:
        case 8:
            adjustment_value = PEPROCESS_HUE_DEGREE_315;
            break;
        default:
            adjustment_value = PEPROCESS_HUE_DEGREE_0;
            break;
        }
        break;     
    default:
        break;
    }

    return adjustment_value;
}
    
/*****************************************************************************/
//  Description : set picture adjustment value as grade
//  Global resource dependence : 
//  Author: Robert.wang
//  Note: 
//  Param: [Return] MMI_PIC_ERROR_TYPE_E
/*****************************************************************************/
PUBLIC MMI_PIC_ERROR_TYPE_E MMIPIC_SetAdjustmentValue(int32 grade,  // [in]
                                            MMI_PIC_ADJUSTMENT_E adjustment  // [in] 
                                            )
{
    PEPROCESS_INPUT_T     input_param={0};
    PEPROCESS_OUTPUT_T  output_param={0};
    PEPROCESS_PARAMETER_T  process_param={0};
    PEPROCESS_RESULT_E ref_return = PEPROCESS_OK;
    
    MMI_PIC_ERROR_TYPE_E ret_value = MMI_PIC_SUCCESS; 

    //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_SetAdjustmentValue grade = %d,adjustment = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_5115_112_2_18_2_45_51_53,(uint8*)"dd",grade, adjustment);
    if ((grade < 0) || (grade > 8) || (adjustment >=MMI_PIC_ADJUSTMENT_MAX))
    {
        return ret_value;
    }
    input_param.src_ptr = s_pic_editor_info.display_bitmap_ptr->bit_ptr;

    input_param.out_buf_ptr=s_pic_editor_info.output_buf_ptr;
    input_param.out_buf_size=s_edit_buffer_size;
    
    input_param.input_width=s_pic_editor_info.display_bitmap_ptr->img_width;
    input_param.input_height=s_pic_editor_info.display_bitmap_ptr->img_height;
    
    input_param.in_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    input_param.out_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    
    input_param.target_width=input_param.input_width;
    input_param.target_height=input_param.input_height;
    
    //set process_param_ptr param
    process_param.process_buf_ptr=s_pic_editor_info.process_buf_ptr;
    process_param.process_buf_size = s_process_buffer_size;	    
    output_param.actual_out_height=0;
    output_param.actual_out_width=0;
    
    switch(adjustment)
    {
    case MMI_PIC_ADJUSTMENT_BRIGHTNESS:
        s_bright_offset = MMIPIC_GetAdjustmentValue(grade,adjustment);
        break;
    case MMI_PIC_ADJUSTMENT_CONSTRACT:
        s_constract_gain = MMIPIC_GetAdjustmentValue(grade,adjustment);
        break;
    case MMI_PIC_ADJUSTMENT_SATURATION:
        s_saturation_gain = MMIPIC_GetAdjustmentValue(grade,adjustment);
        break;
    case MMI_PIC_ADJUSTMENT_HUE:
        s_degree = (PEPROCESS_HUE_DEGREE_E)MMIPIC_GetAdjustmentValue(grade,adjustment);
        break;
    default:
        break;
    }
    if((MMI_PIC_ADJUSTMENT_BRIGHTNESS==adjustment)||(MMI_PIC_ADJUSTMENT_CONSTRACT==adjustment))    
    {
        process_param.type = PEPROCESS_GROUP_BRIGHTNESS_CONTRAST;
        process_param.process_param.group_brightness_contrast.brightness = s_bright_offset;
        process_param.process_param.group_brightness_contrast.contrast = s_constract_gain;        
    }
    else if((MMI_PIC_ADJUSTMENT_SATURATION==adjustment)||(MMI_PIC_ADJUSTMENT_HUE==adjustment))
    {
        process_param.type = PEPROCESS_GROUP_SATURATION_HUE;
        process_param.process_param.group_saturation_hue.saturation= s_saturation_gain;
        process_param.process_param.group_saturation_hue.hue = s_degree;        
    }
    
    ref_return = PEPROCESS_ImageProcess(&input_param,&output_param,&process_param);
    if( PEPROCESS_OK != ref_return )
    {
        ret_value = MMI_PIC_ERR_EXIT;
        return ret_value;
    }
    
    s_pic_editor_info.output_pic_width =output_param.actual_out_width;
    s_pic_editor_info.output_pic_height =output_param.actual_out_height;

    return ret_value;
}

/*****************************************************************************/
//  Description : show error or normal message
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param 
//      Input:  fs_error   value of MMIFILE_ERROR_E type
/*****************************************************************************/
PUBLIC void  MMIPIC_ShowMsg(MMI_PIC_ERROR_TYPE_E pic_error)
{
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_UDISK;
    BOOLEAN         ret = TRUE;
    //wchar           dev_char = 0;
    MMIFILE_ERROR_E fs_error = SFS_ERROR_SYSTEM;
    uint32          msg_type = 0; // fs :0, alert:1 ,query:2, NO Msg: 3
    MMI_TEXT_ID_T   text1_id = TXT_NULL;
    MMI_TEXT_ID_T   text2_id = TXT_NULL;
    MMI_IMAGE_ID_T  image_id = IMAGE_PUBWIN_SUCCESS;

    //convert error type
    switch(pic_error)
    {
    case MMI_PIC_SUCCESS:
        msg_type = 1;
        text1_id = TXT_COMM_SAVE_SUCCESS;
        image_id = IMAGE_PUBWIN_SUCCESS;				
        break;
    case MMI_PIC_ERR_QUERY_OVERWRITE:
        msg_type = 2;
        text1_id = TXT_FILE_EXIST_OVERWRITE;
        image_id = IMAGE_PUBWIN_QUERY;				
        break;
    case MMI_PIC_ERR_NO_SPACE:
        fs_error = SFS_ERROR_NO_SPACE;
        break;
    case MMI_PIC_ERR_LONG_FILENAME:
        fs_error = SFS_ERROR_LONG_DEPTH;
        break;
   case MMI_PIC_ERR_INVALID_NAME:
        fs_error = SFS_ERROR_INVALID_PARAM;
        break;
    default:
        msg_type = 1;
        text1_id = TXT_ERROR_OCCUR;
        image_id = IMAGE_PUBWIN_WARNING;				
        break;
    }


    switch(msg_type)
    {
    case 1:
        MMIPUB_OpenAlertWinByTextId(PNULL, text1_id, text2_id, image_id, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        break;
    case 2:
        MMIPUB_OpenQueryWinByTextId(text1_id,image_id,PNULL,PNULL);
        break;
    case 3:
        break;
    default:
        //dev_char = *(s_pic_editor_info.file_info.full_path_wstr_ptr);
        file_dev = MMIAPIFMM_GetDeviceTypeByPath(s_pic_editor_info.file_info.full_path_wstr_ptr, MMIFILE_DEVICE_LEN);
        ret= MMIAPIFMM_CheckMemoryCard(file_dev);
        //除磁盘格式错误外的
        if(ret)
        {
            MMIAPIFMM_PromptFileErrorType(PNULL,fs_error, file_dev);
        }
        break;
    }
}

//progress bar rect in background image 
//left space,top space, bg offset as "-" image, right space
LOCAL uint8 s_bar_bg_rect[4]=
{
    5,10,0,4
};
/*****************************************************************************/
//  Description : draw progressbar group
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
#ifdef MMIPIC_SUPPORT_PNG
// #ifndef PIC_EDIT_U200_SUPPORT
// PUBLIC void MMIPIC_DrawProgressBarPanel(MMI_WIN_ID_T            win_id,
//                                         MMIPIC_PROGRESSBAR_T    *progressbar_ptr, //in
//                                         MMIPIC_PROGRESSBAR_RECT_T  *progressbar_rect //out
//                                         )
// {
//     GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
// 
//     GUI_POINT_T         dis_point = {0};
//     GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
//     GUISTR_STATE_T      state =  GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
//     GUISTR_INFO_T       text_info = {0};
//     
//     MMIPIC_PROGRESSBAR_T *bar_ptr = progressbar_ptr;
//     int32           i = 0;
//     int32           j = 0;
//     int32           total = 0;
//     int32           text_height = 0;
//     int32           text_width = 0;
//     int32           text_width_max = 0;
//     uint16          image_bg_width = 0;
//     uint16          image_bg_height = 0;
//     MMI_IMAGE_ID_T  image_bg_id = IMAGE_NULL;
//     MMI_TEXT_ID_T   text_id = TXT_NULL;
//     GUI_FONT_T      font_type = MMIPIC_FONT_TYPE;
//     MMI_STRING_T    str = {0};
//     GUI_RECT_T      client_rect = MMITHEME_GetClientRectEx(win_id);
//     int32           client_width = 0;
//     int32           client_height = 0;
//     int32           group_height = 0;
//     uint16          image_up_width = 0;
//     uint16          image_up_height = 0;
//     int16           focus_item = 0;
//     int32           vertical_space = 0;
//     BOOLEAN         is_multi_line = FALSE;
//     int32           bottom_space = 5;
//     int32           text_x = 0;
//     int32           text_y = 0;
//     int32           image_x = 0;
//     int32           image_y = 0;
//     GUI_RECT_T      text_rect = {0};
//     GUI_RECT_T      full_rect = {0};
//     GUI_RECT_T      up_rect = {0};
//     GUI_RECT_T      down_rect = {0};
//     GUI_RECT_T      grade_rect = {0};
//     MMI_WIN_ID_T    cur_win_id = 0;
//     int16           grade = 0;
//     int16           unit_width = 0;
//     int8            image_text_space = 1;
//     MMI_IMAGE_ID_T  image_show_id = IMAGE_NULL;
//     int16           image_panel_width = 0;
//     int8            top_offset = 0;
//     uint16          image_color_width = 0;
//     uint16          image_color_height = 0;
// 
//     int16           total_level = 0;
//     int16           fill_bar_width = 0;
//     int16   string_height = 0;
// 
//     //get max width of text 
//     total = bar_ptr->total;
//     if (total <= 0)
//     {
//         return ;
//     }
//     
//     // 获取文本风格
//     GetPicTextStyle(&text_style, 0, MMI_DARK_RED_COLOR, font_type, 0, 0, ANGLE_0);
// 
//     for (i = 0; i < total; i++)
//     {
//         for (j = 0; j < 2; j++)
//         {
//             text_id = bar_ptr->group[i].item[j];
//             SCI_MEMSET(&str, 0, sizeof(MMI_STRING_T));
//             MMI_GetLabelTextByLang((MMI_TEXT_ID_T)text_id, &str);
//             GUISTR_GetStringInfo(&text_style, &str, state, &text_info);
//             text_width = text_info.width;
//             text_width_max = MAX(text_width, text_width_max);
//         }
//     }    
//     if (TXT_NULL != text_id)
//     {
//         text_height = 2 + text_info.height;
//     }
// 
//     //client region
//     client_width = client_rect.right - client_rect.left + 1;
//     client_height = client_rect.bottom - client_rect.top + 1;
//     
//     bottom_space = (int32)(client_height * 0.05);
// 
//     cur_win_id = bar_ptr->win_id;
//     image_bg_id = bar_ptr->img_bg_id;
//     focus_item = bar_ptr->focus_item;
//     vertical_space = bar_ptr->vertical_space;
//     
//     //get background image width or height
//     GUIRES_GetImgWidthHeight(&image_bg_width, &image_bg_height, image_bg_id, cur_win_id);
//     //get up image width or height
//     GUIRES_GetImgWidthHeight(&image_up_width, &image_up_height, IMAGE_PIC_UP, cur_win_id);
//     //get progress color image width or height
//     GUIRES_GetImgWidthHeight(&image_color_width, &image_color_height, IMAGE_PIC_PROGRESS_COLOR, cur_win_id);
// 
//     fill_bar_width = image_bg_width - s_bar_bg_rect[0] - s_bar_bg_rect[3];
//     image_panel_width = image_bg_width + 2 * image_up_width;
// 
//     if ((text_width_max * 2 + image_panel_width + 2 * image_text_space) >= client_width)
//     {
//         is_multi_line = TRUE;
//     }
//     if (is_multi_line)
//     {
//         group_height = image_bg_height + text_height;
//         vertical_space = 4;
//     }
//     else
//     {
//         group_height = image_bg_height;
//     }
//     text_y = client_rect.bottom - bottom_space - group_height - (total - 1) * (vertical_space + group_height);
//     image_x = (client_width - image_panel_width)/2;
// 
//     if (image_x < 0)
//     {
//         image_x = 0;
//     }
// 
//     for (i = 0; i < total; i++)
//     {
//         total_level = bar_ptr->group[i].total_level;
//     
//         text_x = 0;
//         image_y = text_y;
// 
//         //get left text and display
//         text_id = bar_ptr->group[i].item[0];
//         SCI_MEMSET(&str, 0, sizeof(MMI_STRING_T));
//         MMI_GetLabelTextByLang((MMI_TEXT_ID_T)text_id, &str);
//         GUISTR_GetStringInfo(&text_style, &str, state, &text_info);
//         text_width = text_info.width;
//         string_height = text_info.height;
//         top_offset = (image_up_height - string_height) / 2;
//         text_x = image_x - image_text_space - text_width - 1;
//         if (text_x < 0)
//         {
//             text_x = 0;
//         }
//         text_rect.left = text_x;
//         text_rect.right = text_rect.left + text_width - 1;
//         text_rect.top = text_y + top_offset;
//         text_rect.bottom = text_rect.top + string_height - 1;
//         GUISTR_DrawTextToLCDInRect( 
//             (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
//             (const GUI_RECT_T      *)&text_rect,       //the fixed display area
//             (const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
//             (const MMI_STRING_T    *)&str,
//             &text_style,
//             state,
//             GUISTR_TEXT_DIR_AUTO
//             );
//             
//         // get right text and display
//         text_id = bar_ptr->group[i].item[1];
//         SCI_MEMSET(&str, 0, sizeof(MMI_STRING_T));
//         MMI_GetLabelTextByLang((MMI_TEXT_ID_T)text_id, &str);
//         GUISTR_GetStringInfo(&text_style, &str, state, &text_info);
//         text_width = text_info.width;
//         string_height = text_info.height;
//         text_x = image_x + image_panel_width + image_text_space;
//         text_rect.left = text_x;
//         text_rect.right = text_rect.left + text_width - 1;
//         if (text_rect.right >= client_width)
//         {
//             text_rect.right = client_width - 1;
//             text_rect.left = text_rect.right - text_width + 1;
//         }
//         text_rect.top = text_y + top_offset;
//         text_rect.bottom = text_rect.top + string_height - 1;
//         GUISTR_DrawTextToLCDInRect( 
//             (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
//             (const GUI_RECT_T      *)&text_rect,       //the fixed display area
//             (const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
//             (const MMI_STRING_T    *)&str,
//             &text_style,
//             state,
//             GUISTR_TEXT_DIR_AUTO
//             );
// 
//         //if text display in single line
//         //display image in next line
//         if (is_multi_line)
//         {
//             image_y += text_height;
//         }
// 
//         //display background image 
//         dis_point.x = image_x + image_up_width;
//         dis_point.y = image_y + s_bar_bg_rect[2];
//         GUIRES_DisplayImg(&dis_point,
//             PNULL,
//             PNULL,
//             cur_win_id,
//             image_bg_id,
//             &lcd_dev_info); 
//         //full rect
//         full_rect.left = image_x;
//         full_rect.right = full_rect.left + image_panel_width - 1;
//         full_rect.top = image_y;
//         full_rect.bottom = full_rect.top + image_up_height - 1;
//     
// 
//         //display down image
//         if (focus_item == i)
//         {
//             image_show_id = IMAGE_PIC_FOCUS_DOWN;
//         }
//         else
//         {
//             image_show_id = IMAGE_PIC_DOWN;
//         }
//         dis_point.x = image_x;
//         dis_point.y = image_y;
//         GUIRES_DisplayImg(&dis_point,
//             PNULL,
//             PNULL,
//             cur_win_id,
//             image_show_id,
//             &lcd_dev_info);
//         //down rect
//         down_rect.left = dis_point.x;
//         down_rect.right = down_rect.left + image_up_width - 1;
//         down_rect.top = dis_point.y;
//         down_rect.bottom = down_rect.top + image_up_height - 1;
//     
// 
//         //display up image
//         if (focus_item == i)
//         {
//             image_show_id = IMAGE_PIC_FOCUS_UP;
//         }
//         else
//         {
//             image_show_id = IMAGE_PIC_UP;
//         }
//         dis_point.x = image_x + image_panel_width - image_up_width - 1;
//         dis_point.y = image_y;
//         GUIRES_DisplayImg(&dis_point,
//             PNULL,
//             PNULL,
//             cur_win_id,
//             image_show_id,
//             &lcd_dev_info);
//         //up rect
//         up_rect.left = dis_point.x;
//         up_rect.right = up_rect.left + image_up_width - 1;
//         up_rect.top = dis_point.y;
//         up_rect.bottom = up_rect.top + image_up_height - 1;
//           
//         
//         //fill grade value rect
//         unit_width = fill_bar_width / total_level;
//         grade = bar_ptr->group[i].grade;
// 
//         grade_rect.left = down_rect.right + 1 + s_bar_bg_rect[0];
//         if (grade <= 0)
//         {
//             grade_rect.right = grade_rect.left;
//         }
//         else
//         {
//             //
//             if ((fill_bar_width -  grade * unit_width) < unit_width)
//             {
//                 grade_rect.right = grade_rect.left + fill_bar_width - 1;
//             }
//             else
//             {
//                 grade_rect.right = grade_rect.left + grade * unit_width - 1;
//             }
//         }
//         grade_rect.top = down_rect.top + s_bar_bg_rect[1] + s_bar_bg_rect[2];
//         grade_rect.bottom = grade_rect.top + image_color_height - 1;
//         image_show_id = IMAGE_PIC_PROGRESS_COLOR;
//         if (grade > 0)
//         {
//             GUIRES_DisplayImg(PNULL,
//                 &grade_rect,
//                 PNULL,
//                 cur_win_id,
//                 image_show_id,
//                 &lcd_dev_info);
//         }
// 
//         //get rect for out parameter
//         //reset grade rect for touch pen
//         grade_rect.right = up_rect.left - 1;//up image left
//         grade_rect.top = down_rect.top;//down image top
//         grade_rect.bottom = down_rect.bottom;//down image bottom
//         progressbar_rect->group[i].item[FULL_RECT_INDEX] = full_rect;
//         progressbar_rect->group[i].item[DOWN_RECT_INDEX] = down_rect;
//         progressbar_rect->group[i].item[UP_RECT_INDEX] = up_rect;
//         progressbar_rect->group[i].item[GRADE_RECT_INDEX] = grade_rect;
// 
//         //next group bar
//         text_y += group_height + vertical_space;
//     }
// 
// }
// #else
PUBLIC void MMIPIC_DrawProgressBarPanel(MMI_WIN_ID_T            win_id,
                                        MMIPIC_PROGRESSBAR_T    *progressbar_ptr, //in
                                        MMIPIC_PROGRESSBAR_RECT_T  *progressbar_rect //out
                                        )
{
    GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};    
    MMIPIC_PROGRESSBAR_T *bar_ptr = progressbar_ptr;
    int32           i = 0;
    int32           total = 0;
    int16           image_panel_width = 0;
    int16           image_panel_height = 0;
    uint16          image_pro_width = 0; //progress line width
    uint16          image_pro_height= 0; //progress line height
    MMI_IMAGE_ID_T  image_bg_id     = IMAGE_NULL;
    MMI_IMAGE_ID_T  icon_img        = IMAGE_NULL;
    uint16          icon_color[2]   = {0};
    GUI_RECT_T      client_rect     = MMIPIC_GetClientWinRect(win_id);    
    GUI_RECT_T      pro_panel_rect  = client_rect;
    GUI_RECT_T      icon_rect       = {0};
    GUI_RECT_T      icon_back_rect  = {0};
    GUI_RECT_T      progress_full_rect      = {0};
    GUI_RECT_T      progress_line_rect      = {0};
    GUI_RECT_T      progress_line_tp_rect   = {0};
    GUI_RECT_T      progress_slide_rect     = {0}; 
    int32           client_width = 0;
    uint16          item_space = 0;
    uint16          icon_width = 0;
    uint16          icon_height = 0;
    int16           focus_item = 0;
    int32           vertical_space = 0;
    MMI_WIN_ID_T    cur_win_id = 0;
    int16           grade = 0;
    int16           unit_width = 0;
    int8            top_offset = 0;
    uint16          image_slide_width = 0;
    uint16          image_slide_height = 0;

    int16           total_level = 0;

    //get max width of text 
    total = bar_ptr->total;
    if (total <= 0)
    {
        return ;
    }
    
    cur_win_id = bar_ptr->win_id;
    image_bg_id = bar_ptr->img_bg_id;
    focus_item = bar_ptr->focus_item;
    vertical_space = bar_ptr->vertical_space;

    //get progress image height
    GUIRES_GetImgWidthHeight(&image_pro_width, &image_pro_height, image_bg_id, cur_win_id);
    //get icon image width & height
    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, IMAG_PS_ADJUST_ICON_BRIGHTNESS, cur_win_id);
    //get progress slide image width or height
    GUIRES_GetImgWidthHeight(&image_slide_width, &image_slide_height, IMAG_PS_ADJUST_PROCESS_SLIDE, cur_win_id);

    client_width        = client_rect.right - client_rect.left;
    image_pro_width     = client_width * 2 / 3;
    image_panel_width   = client_width;
    image_panel_height  = total * icon_height + 2 * MMIPICEDIT_PROPANEL_TOPSPACE + (total - 1) * vertical_space;
    pro_panel_rect.bottom   = client_rect.bottom;
    pro_panel_rect.top      = pro_panel_rect.bottom - image_panel_height + 1;
    
    //panel backgroud
    GUIRES_DisplayImg(PNULL,
        &pro_panel_rect,
        PNULL,
        cur_win_id,
        IMAG_PS_ADJUST_OSD_BG,
        &lcd_dev_info);
        
    icon_rect.top       = pro_panel_rect.top + MMIPICEDIT_PROPANEL_TOPSPACE;
    icon_rect.bottom    = icon_rect.top + icon_height - 1;
    progress_full_rect  = pro_panel_rect;
    progress_full_rect.right -= 1;
    
    if(IMG_ITEM_TYPE == bar_ptr->group->item_type)//adjust bri_cons_hue win
    {
        item_space          = (client_width - image_pro_width - icon_width) / 3;
        icon_rect.left      = item_space - 1;
        icon_rect.right     = icon_rect.left + icon_width - 1;
        
        
        for (i = 0; i < total; i++)
        {
            total_level = bar_ptr->group[i].total_level;
            icon_img = bar_ptr->group[i].item[0].img_id;

            progress_line_rect.top   = icon_rect.top + (icon_height - image_pro_height) / 2;
            progress_line_rect.bottom= progress_line_rect.top + image_pro_height - 1;
            progress_line_rect.left  = 2 * item_space + icon_width - 1;
            progress_line_rect.right = progress_line_rect.left + image_pro_width - 1;
            
            progress_line_tp_rect       = progress_line_rect;
            progress_line_tp_rect.top   = icon_rect.top;
            progress_line_tp_rect.bottom= icon_rect.bottom;
            
            //icon_img
            GUIRES_DisplayImg(PNULL,
                &icon_rect,
                PNULL,
                cur_win_id,
                icon_img,
                &lcd_dev_info);
                
            //progress line
            GUIRES_DisplayImg(PNULL,
                &progress_line_rect,
                PNULL,
                cur_win_id,
                image_bg_id,
                &lcd_dev_info);
                
            unit_width = image_pro_width / total_level;
            grade = bar_ptr->group[i].grade;
            progress_slide_rect.top     = progress_line_rect.top - (image_slide_height - image_pro_height) / 2;
            progress_slide_rect.bottom  = progress_slide_rect.top + image_slide_height - 1;
            if (grade <= 0)
            {
                progress_slide_rect.left    = progress_line_rect.left - image_slide_width / 2;
            }
            else
            {
                if ((image_pro_width -  grade * unit_width) < unit_width)
                {
                    progress_slide_rect.left    = progress_line_rect.right - image_slide_width / 2;
                }
                else
                {
                    progress_slide_rect.left    = progress_line_rect.left + grade * unit_width - image_slide_width / 2;
                }
            }
            progress_slide_rect.right   = progress_slide_rect.left + image_slide_width - 1;

            //progress slide
            GUIRES_DisplayImg(PNULL,
                &progress_slide_rect,
                PNULL,
                cur_win_id,
                IMAG_PS_ADJUST_PROCESS_SLIDE,
                &lcd_dev_info);

            progress_full_rect.top      = icon_rect.top;
            progress_full_rect.bottom   = icon_rect.bottom;
            
            progressbar_rect->group[i].item[FULL_RECT_INDEX]    = progress_full_rect;
            progressbar_rect->group[i].item[SLIDE_RECT_INDEX]   = progress_slide_rect;
            progressbar_rect->group[i].item[GRADE_RECT_INDEX]   = progress_line_tp_rect;
            icon_rect.top       = icon_rect.bottom + vertical_space;
            icon_rect.bottom    = icon_rect.top + icon_height - 1;
        }
    }
    else if(COL_ITEM_TYPE == bar_ptr->group->item_type)//adjust color win
    {
        item_space          = (client_width - image_pro_width - 2 * icon_width) / 4;
        icon_rect.left      = item_space - 1;
        icon_rect.right     = icon_rect.left + icon_width - 1;

        icon_back_rect      = icon_rect;
        icon_back_rect.right= client_rect.right - item_space;
        icon_back_rect.left = icon_back_rect.right - icon_width + 1;
        
        for (i = 0; i < total; i++)
        {
            total_level = bar_ptr->group[i].total_level;
            icon_color[0] = bar_ptr->group[i].item[0].col_num;
            icon_color[1] = bar_ptr->group[i].item[1].col_num;
            
            progress_line_rect.top   = icon_rect.top + (icon_height - image_pro_height) / 2;
            progress_line_rect.bottom= progress_line_rect.top + image_pro_height - 1;
            progress_line_rect.left  = 2 * item_space + icon_width - 1;
            progress_line_rect.right = progress_line_rect.left + image_pro_width - 1;
            
            progress_line_tp_rect       = progress_line_rect;
            progress_line_tp_rect.top   = icon_rect.top;
            progress_line_tp_rect.bottom= icon_rect.bottom;
            
            //icon_color
            LCD_FillRect(&lcd_dev_info, icon_rect, icon_color[0]);
            LCD_FillRect(&lcd_dev_info, icon_back_rect, icon_color[1]);

            //progress line
            GUIRES_DisplayImg(PNULL,
                &progress_line_rect,
                PNULL,
                cur_win_id,
                image_bg_id,
                &lcd_dev_info);
                
            unit_width = image_pro_width / total_level;
            grade = bar_ptr->group[i].grade;
            progress_slide_rect.top     = progress_line_rect.top - (image_slide_height - image_pro_height) / 2;
            progress_slide_rect.bottom  = progress_slide_rect.top + image_slide_height - 1;
            if (grade <= 0)
            {
                progress_slide_rect.left    = progress_line_rect.left - image_slide_width / 2;
            }
            else
            {
                if ((image_pro_width -  grade * unit_width) < unit_width)
                {
                    progress_slide_rect.left    = progress_line_rect.right - image_slide_width / 2;
                }
                else
                {
                    progress_slide_rect.left    = progress_line_rect.left + grade * unit_width - image_slide_width / 2;
                }
            }
            progress_slide_rect.right   = progress_slide_rect.left + image_slide_width - 1;

            //progress slide
            GUIRES_DisplayImg(PNULL,
                &progress_slide_rect,
                PNULL,
                cur_win_id,
                IMAG_PS_ADJUST_PROCESS_SLIDE,
                &lcd_dev_info);
                
            progress_full_rect.top      = icon_rect.top - 1;
            progress_full_rect.bottom   = icon_rect.bottom + 1;
            
            progressbar_rect->group[i].item[FULL_RECT_INDEX]    = progress_full_rect;
            progressbar_rect->group[i].item[SLIDE_RECT_INDEX]   = progress_slide_rect;
            progressbar_rect->group[i].item[GRADE_RECT_INDEX]   = progress_line_tp_rect;
            
            icon_rect.top           = icon_rect.bottom + vertical_space;
            icon_rect.bottom        = icon_rect.top + icon_height - 1;
            icon_back_rect.top      = icon_rect.top;
            icon_back_rect.bottom   = icon_rect.bottom;            
        }
    }
}
// #endif//PIC_EDIT_U200_SUPPORT
#else
PUBLIC void MMIPIC_DrawProgressBarPanel(MMI_WIN_ID_T            win_id,
                                        MMIPIC_PROGRESSBAR_T    *progressbar_ptr, //in
                                        MMIPIC_PROGRESSBAR_RECT_T  *progressbar_rect //out
                                        )
{
    GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    GUI_POINT_T         dis_point = {0};
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =  GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    GUISTR_INFO_T       text_info = {0};
    
    MMIPIC_PROGRESSBAR_T *bar_ptr = progressbar_ptr;
    int32           i = 0;
    int32           j = 0;
    int32           total = 0;
    int32           text_height = 0;
    int32           text_width = 0;
    int32           text_width_max = 0;
    uint16          image_bg_width = 0;
    MMI_IMAGE_ID_T  image_bg_id = IMAGE_NULL;
    MMI_TEXT_ID_T   text_id = TXT_NULL;
    GUI_FONT_T      font_type = MMIPIC_FONT_TYPE;
    MMI_STRING_T    str = {0};
    GUI_RECT_T      client_rect = MMITHEME_GetClientRectEx(win_id);
    int32           client_width = 0;
    int32           client_height = 0;
    int32           group_height = 0;
    uint16          image_up_width = 0;
    uint16          image_up_height = 0;
    uint16          image_bg_height = 0;
    int16           focus_item = 0;
    int32           vertical_space = 0;
    BOOLEAN         is_multi_line = FALSE;
    int32           bottom_space = 5;
    int32           text_x = 0;
    int32           text_y = 0;
    int32           image_x = 0;
    int32           image_y = 0;
    GUI_RECT_T      text_rect = {0};
    GUI_RECT_T      full_rect = {0};
    GUI_RECT_T      up_rect = {0};
    GUI_RECT_T      down_rect = {0};
    GUI_RECT_T      grade_rect = {0};
    MMI_WIN_ID_T    cur_win_id = 0;
    int16           grade = 0;
    int16           unit_width = 0;
    int8            image_text_space = 1;
    
    int16  total_level = 0;
    int16  fill_bar_width = 0;

    //get max width of text 
    total = bar_ptr->total;
    if (total <= 0)
    {
        return ;
    }
    
    // 获取文本风格
    GetPicTextStyle(&text_style, 0, MMI_DARK_RED_COLOR, font_type, 0, 0, ANGLE_0);

    for (i = 0; i < total; i++)
    {
        for (j = 0; j < 2; j++)
        {
            SCI_MEMSET(&str, 0, sizeof(MMI_STRING_T));
            text_id = bar_ptr->group[i].item[j].img_id;
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)text_id, &str);
            GUISTR_GetStringInfo(&text_style, &str, state, &text_info);
            text_width = text_info.width;
            text_width_max = MAX(text_width, text_width_max);
        }
    }    
    if (TXT_NULL != text_id)
    {
        text_height = 2 + text_info.height;
    }

    //client region
    client_width = client_rect.right - client_rect.left + 1;
    client_height = client_rect.bottom - client_rect.top + 1;
    bottom_space = (int32)(client_height * 0.05);
    
    cur_win_id = bar_ptr->win_id;
    image_bg_id = bar_ptr->img_bg_id;
    focus_item = bar_ptr->focus_item;
    vertical_space = bar_ptr->vertical_space;
    unit_width = bar_ptr->unit_width;

    
    //get background image width or height
    GUIRES_GetImgWidthHeight(&image_bg_width, &image_bg_height, image_bg_id, cur_win_id);
    //get up image width or height
    GUIRES_GetImgWidthHeight(&image_up_width, &image_up_height, IMAGE_PIC_UP, cur_win_id);

    fill_bar_width = image_bg_width - 2 * image_up_width ;

    if ((text_width_max * 2 + image_bg_width + 2 * image_text_space) >= client_width)
    {
        is_multi_line = TRUE;
    }
    if (is_multi_line)
    {
        group_height = image_bg_height + text_height;
        vertical_space = 4;
    }
    else
    {
        group_height = image_bg_height;
    }
    text_y = client_rect.bottom - bottom_space - group_height - (total - 1) * (vertical_space + group_height);
    image_x = (client_width - image_bg_width)/2;

    if (image_x < 0)
    {
        image_x = 0;
    }
    for (i = 0; i < total; i++)
    {
        total_level = bar_ptr->group[i].total_level;
        
        text_x = 0;
        image_y = text_y;

        //get left text and display
        text_id = bar_ptr->group[i].item[0].img_id;
        SCI_MEMSET(&str, 0, sizeof(MMI_STRING_T));
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)text_id, &str);
        text_width = GUISTR_GetStringWidth(&text_style, &str, state);
        text_x = image_x - image_text_space - text_width - 1;
        if (text_x < 0)
        {
            text_x = 0;
        }
        text_rect.left = text_x;
        text_rect.right = text_rect.left + text_width - 1;
        text_rect.top = text_y;
        text_rect.bottom = text_rect.top + text_height - 1;
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&text_rect,       //the fixed display area
            (const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
            
        // get right text and display
        text_id = bar_ptr->group[i].item[1].img_id;
        SCI_MEMSET(&str, 0, sizeof(MMI_STRING_T));
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)text_id, &str);
        text_width = GUISTR_GetStringWidth(&text_style, &str, state);
        text_x = image_x + image_bg_width + image_text_space;
        text_rect.left = text_x;
        text_rect.right = text_rect.left + text_width - 1;
        if (text_rect.right >= client_width)
        {
            text_rect.right = client_width - 1;
            text_rect.left = text_rect.right - text_width + 1;
        }
        text_rect.top = text_y;
        text_rect.bottom = text_rect.top + text_height - 1;
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&text_rect,       //the fixed display area
            (const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );

        //if text display in single line
        //display image in next line
        if (is_multi_line)
        {
            image_y += text_height;
        }

        //display background image 
        dis_point.x = image_x;
        dis_point.y = image_y;
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            cur_win_id,
            image_bg_id,
            &lcd_dev_info); 
        //full rect
        full_rect.left = dis_point.x;
        full_rect.right = full_rect.left + image_bg_width - 1;
        full_rect.top = dis_point.y;
        full_rect.bottom = full_rect.top + image_bg_height - 1;
    

        //display down image
        dis_point.x = image_x;
        dis_point.y = image_y;
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            cur_win_id,
            IMAGE_PIC_DOWN,
            &lcd_dev_info);
        //down rect
        down_rect.left = dis_point.x;
        down_rect.right = down_rect.left + image_up_width - 1;
        down_rect.top = dis_point.y;
        down_rect.bottom = down_rect.top + image_up_height - 1;
    

        //display up image
        dis_point.x = image_x + image_bg_width - image_up_width - 1 + 1;
        dis_point.y = image_y;
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            cur_win_id,
            IMAGE_PIC_UP,
            &lcd_dev_info);
        //up rect
        up_rect.left = dis_point.x;
        up_rect.right = up_rect.left + image_up_width - 1;
        up_rect.top = dis_point.y;
        up_rect.bottom = up_rect.top + image_up_height - 1;
          
        //fill grade value rect
        unit_width = fill_bar_width / total_level;
        grade = bar_ptr->group[i].grade;

        grade_rect.left = down_rect.right + 2;
        if (grade <= 0)
        {
            grade_rect.right = grade_rect.left;
        }
        else
        {
            //
            if ((fill_bar_width -  grade * unit_width) < unit_width)
            {
                grade_rect.right = grade_rect.left + fill_bar_width - 1;
            }
            else
            {
                grade_rect.right = grade_rect.left + grade * unit_width - 1;
            }
        }
        grade_rect.top = down_rect.top + 2;
        grade_rect.bottom = down_rect.bottom - 2;
        GUI_FillRect(&lcd_dev_info, grade_rect, 0x03b4);

        if (focus_item == i)
        {
            MMIPIC_EditWinDrawFocusRect(full_rect, MMI_DARK_RED_COLOR,0); 
        }
        else
        {
            MMIPIC_EditWinDrawFocusRect(full_rect, 0x9d38,0); 
        }

        //get rect for out parameter
        grade_rect.left = down_rect.right + 1;
        grade_rect.right = up_rect.left - 1;
        grade_rect.top = down_rect.top;
        grade_rect.bottom = down_rect.bottom;
        progressbar_rect->group[i].item[FULL_RECT_INDEX] = full_rect;
        //progressbar_rect->group[i].item[DOWN_RECT_INDEX] = down_rect;
        //progressbar_rect->group[i].item[UP_RECT_INDEX] = up_rect;
        progressbar_rect->group[i].item[GRADE_RECT_INDEX] = grade_rect;

        //next group bar
        text_y += group_height + vertical_space;
    }

}
#endif
/*****************************************************************************/
//  Description : get edit buffer size
//  Global resource dependence : 
//  Author: robert.wang
//  Note: lcd_width*lcd_height*2
/*****************************************************************************/
PUBLIC uint32 MMIPIC_GetEditBufferSize(void)
{
    return s_edit_buffer_size;
}

/*****************************************************************************/
//  Description : get process buffer size
//  Global resource dependence : 
//  Author: robert.wang
//  Note: lcd_width*lcd_height*3 + MMIPIC_PROCESS_BUFFER_FIXED_SIZE
/*****************************************************************************/
PUBLIC uint32 MMIPIC_GetProcessBufferSize(void)
{
    return s_process_buffer_size;
}

/*****************************************************************************/
//  Description : init pic process parameter 
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIPIC_InitProcessParam(void)
{
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;


    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    s_edit_buffer_size = lcd_width * lcd_height * sizeof(GUI_COLOR_T);
    s_process_buffer_size = lcd_width * lcd_height * 3 + MMIPIC_PROCESS_BUFFER_FIXED_SIZE;
    s_filedata_buffer_size = s_process_buffer_size;

    return TRUE;
}

/*****************************************************************************/
//  Description : get fill color rect width  
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIPIC_GetFillColorRectWidth(void)
{
    int32 width = 0;
    uint16 image_width = 0;
    uint16 image_height = 0;

    //get background image width or height
    #if defined(MMIPIC_SUPPORT_PNG)
        GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_PIC_ADJUST_BG, MMIPIC_EDIT_MAIN_WIN_ID);
        width = image_width - s_bar_bg_rect[0] - s_bar_bg_rect[3];
    #else
    {
        uint16 image_up_width = 0;
        uint16 image_up_height = 0;

        if (MMK_IsOpenWin(MMIPIC_EDIT_RGB_WIN_ID))
        {
            GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_PIC_COLOR_BG, MMIPIC_EDIT_MAIN_WIN_ID);
        }
        else
        {
            GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_PIC_ADJUST_BG, MMIPIC_EDIT_MAIN_WIN_ID);
        }
        
        GUIRES_GetImgWidthHeight(&image_up_width, &image_up_height, IMAGE_PIC_UP, MMIPIC_EDIT_MAIN_WIN_ID);
        width = image_width - 2 * image_up_width;
    }    
    #endif
    
    return width;
}

/*****************************************************************************/
//  Description : get the client rect of pic_edit wins  
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIPIC_GetClientWinRect(MMI_WIN_ID_T  win_id)
{
    GUI_RECT_T      client_rect = MMITHEME_GetClientRectEx(win_id);
    
#ifdef PIC_EDIT_U200_SUPPORT
    client_rect.top     += MMIPICEDIT_TITLE_HEIGHT;
    client_rect.bottom  -= MMIPICEDIT_FORM_HEIGHT;
    client_rect.bottom = MAX(client_rect.bottom, client_rect.top);
#else
    client_rect.top     += MMIPICEDIT_FORM_HEIGHT;
#endif

    return client_rect;
}

/*****************************************************************************/
//  Description : get the relative rect off pic  
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIPIC_GetRelativeRect(MMI_WIN_ID_T  win_id)
{
    GUI_RECT_T      client_rect = MMIPIC_GetClientWinRect(win_id);
    GUI_RECT_T      pic_rect    = s_pic_editor_info.display_rect;
    GUI_RECT_T      disp_rect   = {0};
    GUI_RECT_T      relat_rect  = {0};
    int16           x_offset    = 0;
    int16           y_offset    = 0;
    
    //get visible display_rect 
    disp_rect.left   = MAX(client_rect.left, pic_rect.left);
    disp_rect.right  = MIN(client_rect.right, pic_rect.right);
    disp_rect.top    = MAX(client_rect.top, pic_rect.top);
    disp_rect.bottom = MIN(client_rect.bottom, pic_rect.bottom);
    x_offset            = disp_rect.left - pic_rect.left;
    y_offset            = disp_rect.top - pic_rect.top;
    
    //get the relative rect to pic_rect
    relat_rect.left     = x_offset;
    relat_rect.right    = x_offset + (disp_rect.right - disp_rect.left);
    relat_rect.top      = y_offset;
    relat_rect.bottom   = y_offset + (disp_rect.bottom - disp_rect.top);
    
    return relat_rect;
}

/*****************************************************************************/
//  Description : judge if click the character rect  
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_IfClickCharRect(MMI_WIN_ID_T win_id, PIC_CHAR_PARAM_T* add_char_info, GUI_POINT_T tp_point)
{
    BOOLEAN             result = FALSE;
    GUI_RECT_T          char_rect = {0};
    GUI_RECT_T          client_rect = MMIPIC_GetClientWinRect(win_id);
    PIC_CHAR_PARAM_T    add_char = *(add_char_info);
    
    char_rect.left  = s_pic_editor_info.output_display_rect.left + add_char.box.left;
    char_rect.top   = s_pic_editor_info.output_display_rect.top + add_char.box.top;
    char_rect.right = char_rect.left + add_char.char_string_width - 1;
    char_rect.bottom= char_rect.top + add_char.char_string_heigh - 1;

    if(0 == (char_rect.left - client_rect.left))
    {
        char_rect.right += 6;
    }
    else if(0 == (char_rect.right - client_rect.right))
    {
        char_rect.left  -= 6;
    }
    else
    {
        char_rect.left  -= 3;
        char_rect.right += 3;
    }
    char_rect.top   -= 3;
    char_rect.bottom+= 3;
    
    if(GUI_PointIsInRect(tp_point, char_rect))
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : judge if click the character rect  
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_IfClickClipRect(GUI_POINT_T tp_point)
{
    BOOLEAN             result = FALSE;
    
    if(GUI_PointIsInRect(tp_point, s_pic_editor_info.osd_display_rect))
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Get the info of bitmap
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_GetBitmapInfo(GUIIMG_BITMAP_T * img_info_ptr)
{
	if(PNULL != img_info_ptr)
	{
		img_info_ptr->img_width	 = s_pic_editor_info.display_bitmap_ptr->img_width;
		img_info_ptr->img_height = s_pic_editor_info.display_bitmap_ptr->img_height;
		img_info_ptr->bit_ptr	 = s_pic_editor_info.display_bitmap_ptr->bit_ptr;
		img_info_ptr->date_type  = s_pic_editor_info.display_bitmap_ptr->date_type;
	}
}
#endif



/*Edit by script, ignore 8 case. Thu Apr 26 19:01:03 2012*/ //IGNORE9527
