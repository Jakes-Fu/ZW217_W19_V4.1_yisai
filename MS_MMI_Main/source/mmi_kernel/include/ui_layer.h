/******************************************************************************
** File Name:      ui_layer.h                                                  *
** Author:                                                                    *
** Date:            		                                                  *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.          *
** Description:      								                          *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
**                                       									  *
******************************************************************************/


/*!
 *  \addtogroup mmk_mmiutil_group_label
 *  @{
 */


/*! 
 *  \file 	ui_layer.h
 *  \author	
 *  \date	
 *  \brief 	
 */
 
#ifndef UI_LAYER_H
#define UI_LAYER_H

#include "mmk_type.h"
#include "dal_lcd.h"
//#include "guilcd.h"
#include "dal_display.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         DEPENDANCY                                         */
/*----------------------------------------------------------------------------*/

#define UILAYER_TOTAL_LAYER_COUNT       GUI_UILAYER_TOTAL_LAYER_COUNT//16      //�߼����ĸ���
#define UILAYER_BLEND_MAX_NUM           4       //�ײ�Ӳ�����ϳ�ͼ�����������Ļ����ֶ�κϳɣ�
#define UILAYER_TOTAL_BLT_COUNT         16//8       //ˢ�²㣨���Ӷ��У�������
#define UILAYER_TRANSPARENT_COLOR       0x8080  //͸��ɫ
//#define UILAYER_MAIN_HANDLE             (GUI_BLOCK_MAIN)    //main_handle
#define UILAYER_NULL_HANDLE             (0xffff)    //null_handle

#define MMI_MEMMONITER_START    MMI_MemMoniterStart
#define MMI_MEMMONITER_END      MMI_MemMoniterEnd

#define UILAYER_RELEASELAYER UILAYER_ReleaseLayerExt
// #define UILAYER_RELEASELAYER(dev_info_ptr)     \
//     do  {   \
//         if (PNULL != (dev_info_ptr))    \
//         {   \
//             UILAYER_ReleaseLayer(dev_info_ptr); \
//             (dev_info_ptr)->lcd_id = 0;   \
//             (dev_info_ptr)->block_id = UILAYER_NULL_HANDLE;   \
//         }   \
//     }   \
//     while(0);

typedef uint32 UILAYER_HANDLE_T;    //layer���

//��buffer������
typedef uint8 UILAYER_MEM_FORMAT_T;

#define UILAYER_MEM_FORMAT_IMMUTABLE  0x01
#define UILAYER_MEM_FORMAT_DOUBLE     0x02
//#define UILAYER_MEM_FORMAT_PINGPONG   0x04

#define UILAYER_MEM_NONE		    0
#define UILAYER_MEM_IMMUTABLE       UILAYER_MEM_FORMAT_IMMUTABLE                            //�����ݲ���
#define UILAYER_MEM_DOUBLE_COPY     (UILAYER_MEM_FORMAT_DOUBLE)  //������Ϊ˫���忽��ģ�ͣ��ٶ�Ҫ��ƹ���������˿���ʱ��
#define UILAYER_MEM_DOUBLE_PINGPONG (UILAYER_MEM_FORMAT_IMMUTABLE|UILAYER_MEM_FORMAT_DOUBLE)  //������Ϊ˫����ƹ��ģ��

//�ڴ治��ʱ��˫buffer��Ϊ��buffer
#ifdef MMI_FPS_ACCELERATE_SUPPORT
#define UILAYER_MEM_FPS_ACCELERATE UILAYER_MEM_DOUBLE_COPY
#else
#define UILAYER_MEM_FPS_ACCELERATE UILAYER_MEM_IMMUTABLE
#endif

//layer����ִ�н��
typedef enum
{
    UILAYER_RESULT_SUCCESS,         //�����ɹ�
    UILAYER_RESULT_USEDMAXNUM,      //���ʹ���Ѿ��ﵽ�����Ŀ
    UILAYER_RESULT_MULTILAYER_DISABLED,      //����ֹʹ��
    UILAYER_RESULT_ERROR      //����
}UILAYER_RESULT_E;

//layer����Ϣ
typedef struct
{
    uint16 layer_width;
    uint16 layer_height;
    uint16 mem_width;
//    uint8  *layer_buf_ptr;
    DISPLAY_DATA_TYPE_E data_type;
}UILAYER_INFO_T;


typedef struct  
{
    void* (*Malloc)( uint32 size );
    void  (*Free)( void *memory_ptr );
}UILAYER_MEM_FUNC_T;

//����layer����Ϣ
typedef struct
{
    LCD_ID_E lcd_id;
    MMI_HANDLE_T owner_handle;
    int16 offset_x;
    int16 offset_y;
    int16 width;
    int16 height;
    BOOLEAN is_bg_layer;
    BOOLEAN is_static_layer;
    UILAYER_MEM_FORMAT_T format;
    UILAYER_MEM_FUNC_T* func_ptr;
}UILAYER_CREATE_T;

//bltͼ��ļ���
typedef enum
{
    UILAYER_LEVEL_LOW,
    UILAYER_LEVEL_NORMAL,
    UILAYER_LEVEL_HIGH,
    UILAYER_LEVEL_DEBUG,

    UILAYER_LEVEL_MAX
}UILAYER_LEVEL_E;

//���bltͼ�����Ϣ
typedef struct
{
    GUI_LCD_DEV_INFO lcd_dev_info;
    UILAYER_LEVEL_E layer_level;
}UILAYER_APPEND_BLT_T;

//layer��Ļץͼ�ṹ
typedef struct
{
    DISPLAY_BLEND_LAYER_T lcd_layer_blend;
    DISPLAY_BLEND_LAYER_T sublcd_layer_blend;
    const DISPLAY_BLEND_LAYER_T layer_stuct_arr[UILAYER_TOTAL_BLT_COUNT];
    uint32 arr_size;
}UILAYER_PRINTSCREEN_INFO_T;

//��װblend��Ϣ
typedef struct
{
    DISPLAY_BLEND_LAYER_T dst_layer;
    DISPLAY_BLEND_LAYER_T src_layer[UILAYER_TOTAL_BLT_COUNT];
    uint32 layer_num;
    DISPLAY_RECT_T rect;
    DISPLAY_CALLBACK callback;
    uint32 param;
}UILAYER_BLEND_T;

//��ͼ��handle��type
typedef enum
{
    UILAYER_HANDLE_INVALID,     //��Чhandle
    UILAYER_HANDLE_MAIN,        //����handle
    UILAYER_HANDLE_BLOCK,       //block handle
    UILAYER_HANDLE_MULTI        //multi handle
}UILAYER_HANDLE_TYPE_E;

//copy buffer info
typedef struct 
{
    DISPLAY_DATA_TYPE_E data_type;
    uint8 *buf_ptr;
    uint16 width;
    uint16 height;
}UILAYER_COPYBUF_T;

//copy buffer param
typedef struct 
{
    GUI_RECT_T  rect;
    GUI_RECT_T* clip_rect_ptr;
}UILAYER_COPYBUF_PARAM_T;

typedef void (*UILAYER_BLEND_CALLBACK)( 
                                       uint8* buffer, 
                                       UILAYER_INFO_T *layer_info_ptr,
                                       uint32 param
                                       );

/*----------------------------------------------------------------------------*/
/*                         FUNCTIONS                                          */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : ����layer͸����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetLayerAlpha(
                                  GUI_LCD_DEV_INFO const *dev_info_ptr,
                                  uint8 alpha_value            //͸����
                                  );

/*****************************************************************************/
//     Description : ����layer͸��ɫ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetLayerColorKey(
                                     GUI_LCD_DEV_INFO const *dev_info_ptr,
                                     BOOLEAN is_color_key_enable,
                                     GUI_COLOR_T color_key              //͸��ɫ
                                     );

/*****************************************************************************/
//     Description : ����layer������layer���
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC UILAYER_RESULT_E UILAYER_CreateLayerEx(
                                            const UILAYER_CREATE_T *create_ptr,
                                            GUI_LCD_DEV_INFO *dev_info_ptr,     //out
                                            const char*       file_name_ptr,
                                            uint32            file_line
                                            );

#define UILAYER_CreateLayer(create_ptr, dev_info_ptr)   \
    UILAYER_CreateLayerEx(create_ptr, dev_info_ptr, _D_FILE_NAME, _D_FILE_LINE)

/*****************************************************************************/
//     Description : ��s_layer_blt��ȥ��layer_handle
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_RemoveBltLayer(
                                   GUI_LCD_DEV_INFO const *dev_info_ptr
                                   );

/*****************************************************************************/
//     Description : ����һ����
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_AppendBltLayer(
                                   UILAYER_APPEND_BLT_T *append_layer_ptr
                                   );

/*****************************************************************************/
//     Description : �ϲ���ˢ�µ�lcd��ȥ��ĳһʱ�̣����øú����Ľ�����ǽ�s_layer_blt[0], s_layer_blt[1], 
//s_layer_blt[2], s_layer_blt[3]���κϲ������is_refesh_to_lcd=TRUE����ˢ�µ�LCD�ϡ�����ˢ��lcd_buf��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_InvalidateRect(//Return the operate status
	LCD_ID_E lcd_id,				//id of lcd to operate
    const GUI_RECT_T *rect_ptr
	);

/*****************************************************************************/
//     Description : ��Ŀ�����²������ˢ����Ŀ�����ȥ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_BltLowerLayer(
                                  GUI_LCD_DEV_INFO const *dev_info_ptr,
                                  const GUI_RECT_T *rect_ptr
                                  );

/*****************************************************************************/
//     Description : ��ʼ��layerϵͳ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
//PUBLIC void UILAYER_InitSystem(
//                               GUI_COLOR_T *mainlcd_buf_ptr,
//                               uint32 mainlcd_buf_size,
//                               DISPLAY_DATA_TYPE_E data_type
//                               );
/*****************************************************************************/
//     Description : ��ʼ��layerϵͳ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_InitSystem(void);

/*****************************************************************************/
//     Description : ���layer��bufferָ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC uint8 *UILAYER_GetLayerBufferPtr(
                                   GUI_LCD_DEV_INFO const *dev_info_ptr
                                   );

/*****************************************************************************/
//     Description : ���layer�������LCD���Ͻǵ�����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_GetLayerPosition(
                                GUI_LCD_DEV_INFO const *dev_info_ptr,
                                int16 *offset_x_ptr, //output
                                int16 *offset_y_ptr //output
                                );

/*****************************************************************************/
//     Description : ���layer����Ϣ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_GetLayerInfo(
                                 GUI_LCD_DEV_INFO const *dev_info_ptr,
                                 UILAYER_INFO_T *layer_info_ptr     //output
                                 );

/*****************************************************************************/
//     Description : ����layer��λ�ã������lcd���Ͻǵ����꣩
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetLayerPosition(
                                GUI_LCD_DEV_INFO const *dev_info_ptr,
                                int16 offset_x,
                                int16 offset_y
                                );

/*****************************************************************************/
//     Description : ��㹦���Ƿ���
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:  FALSE һ�㣨���㣩 TRUE ���
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsMultiLayerEnable(void);

/*****************************************************************************/
//     Description : ��ͼ�����͸��ɫ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_Clear(
                          GUI_LCD_DEV_INFO const *dev_info_ptr
                          );

/*****************************************************************************/
//     Description : ��ͼ�������ɫ
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_FillColor(
                              GUI_LCD_DEV_INFO const *dev_info_ptr,
                              uint32 color
                              );

/*****************************************************************************/
//     Description : ����Handle����������layer��Handle get focus��ʱ�����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ActiveAllLayerOfHandle(
                                           MMI_HANDLE_T mmi_handle
                                           );

/*****************************************************************************/
//     Description : ȥ����Handle����������layer��Handle lose focus��ʱ�����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_DeactiveAllLayerOfHandle(
                                             MMI_HANDLE_T mmi_handle
                                             );

/*****************************************************************************/
//     Description : �ͷ�Handle����������layer��Handle close��ʱ�����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ReleaseAllLayerOfHandle(
                                            MMI_HANDLE_T mmi_handle
                                            );

/*****************************************************************************/
//     Description : �ͷ�layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ReleaseLayer(
                                 GUI_LCD_DEV_INFO const *dev_info_ptr
                                 );

//#if  defined(LIVE_WALLPAPER_FRAMEWORK_SUPPORT)
/*****************************************************************************/
//     Description : �ͷ�layerEX
//    Global resource dependence : 
//  Author:   Arvin.wu
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ReleaseLayerExt(
                                 GUI_LCD_DEV_INFO  *dev_info_ptr
                                 );
//#endif
/*****************************************************************************/
//     Description : �ж�layer�Ƿ񼤻�״̬
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsLayerActive(
                                     GUI_LCD_DEV_INFO const *dev_info_ptr
                                     );

/*****************************************************************************/
//     Description : ���rect�������Ƿ�͸�������͸���򽫵��²������ˢ����Ŀ�����ȥ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_BltLowerLayerWhenTransparent(
                                                 GUI_LCD_DEV_INFO const *dev_info_ptr,
                                                 const GUI_RECT_T *rect_ptr
                                                 );

/*****************************************************************************/
// Description : modify block width and rect info for rotation
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC void UILAYER_Rotation(
                             GUI_LCD_DEV_INFO   *lcd_dev_ptr
                             );
             
/*****************************************************************************/
//     Description : ����ڴ����ַ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC uint32 *UILAYER_GetMemCheckAddr(
                                       GUI_LCD_DEV_INFO const *dev_info_ptr
                                       );

/*****************************************************************************/
//     Description : �ڴ��⣺���һ��ʱ����ĳ��ַ�����Ƿ�ı�
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void MMI_MemMoniterStart(
                                uint32 *mem_ptr
                                );
                           
/*****************************************************************************/
//     Description : �ڴ��⣺���һ��ʱ����ĳ��ַ�����Ƿ�ı�
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void MMI_MemMoniterEnd(void);

/*****************************************************************************/
//     Description : ��lcd���ݱ��ݵ�mainlayer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_CopyLcdToMainLayer(void);

/*****************************************************************************/
//     Description : �ж�layer�Ƿ񼤻�״̬
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC DISPLAY_DATA_TYPE_E UILAYER_GetLayerColorType(
                                     GUI_LCD_DEV_INFO const *dev_info_ptr
                                     );

/*****************************************************************************/
//     Description : ��ͼ�����ĳ����ɫ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ClearRect(
                          GUI_LCD_DEV_INFO const *dev_info_ptr,
                          GUI_RECT_T rect
                          );

/*****************************************************************************/
//     Description : ����layer͸����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_WeakLayerAlpha(
                                  GUI_LCD_DEV_INFO const *dev_info_ptr,
                                  uint8 alpha_value            //͸����
                                  );

/*****************************************************************************/
//     Description : ��ĳ������ˢ����һ����ȥ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_BltLayerToLayer(
                                GUI_LCD_DEV_INFO const *dst_dev_ptr,
                                GUI_LCD_DEV_INFO const *src_dev_ptr,
                                const GUI_RECT_T *rect_ptr,
                                BOOLEAN is_compound     //TRUE, ��ϣ�FALSE��ֱ�Ӹ���
                                );

/*****************************************************************************/
//     Description : �ж�layer�Ƿ�blt layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsBltLayer(
                                  GUI_LCD_DEV_INFO const *dev_info_ptr
                                  );

/*****************************************************************************/
//     Description : ��s_layer_blt��ȥ��main layer handle
//    Global resource dependence : 
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_RemoveMainLayer(void);

/*****************************************************************************/
//     Description : restore main layer.
//    Global resource dependence : 
//  Author:   
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_RestoreMainLayer(void);

/*****************************************************************************/
//     Description : ��Ϊ��Ļץͼ���ܣ����lcd_buffer�Լ�ͼ������
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_GetBltInfoForPrintScreen(
                                                UILAYER_PRINTSCREEN_INFO_T *print_info_ptr
                                                );

/*****************************************************************************/
//     Description : ���blend info
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_GetBlendInfo(
                                    LCD_ID_E lcd_id,    //in
                                    const GUI_RECT_T *rect_ptr,   //in
                                    UILAYER_BLEND_T *blend_ptr  //out
                                    );

/*****************************************************************************/
//     Description : get layer info of input blt layer array.
//     Global resource dependence : 
//     Author:  fulu.song
//     Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_GetLayersBlendInfo(
                                    const UILAYER_APPEND_BLT_T *blt_layers,   //in
                                    const uint32 num_layers,//in
                                    const GUI_LCD_DEV_INFO *dst_layer,
                                    const GUI_RECT_T *rect_ptr,   //in
                                    UILAYER_BLEND_T *blend_ptr  //out
                                    );

/*****************************************************************************/
//     Description : ���handle����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC UILAYER_HANDLE_TYPE_E UILAYER_GetHandleType(
                                                   GUI_LCD_DEV_INFO const *dev_info_ptr
                                                   );
/*****************************************************************************/
//     Description : �ж��Ƿ���handleΪ��
//    Global resource dependence : 
//  Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsEmptyLayerHandle(
                                        GUI_LCD_DEV_INFO const *dev_info_ptr
                                        );
/*****************************************************************************/
//     Description : ���invalidate param
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: Ϊ˫����main lcd buffer�Ż�
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_GetInvalidateParam(
                                          LCD_ID_E lcd_id,                     //in
                                          const GUI_RECT_T *rect_ptr,
                                          DISPLAY_INVALIDATE_PARM_T* param_ptr //in out
                                          );

/*****************************************************************************/
//     Description : ��������mem format
//    Global resource dependence : 
//  Author:  bin.ji
//    Note: 
/*****************************************************************************/
PUBLIC void UILAYER_SetMainLayerMemFormat(
                                    UILAYER_MEM_FORMAT_T type
                                    );

/*****************************************************************************/
//     Description : ����layer���ݲ���
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: �����Ż�ר�ã��������ݲ��䣬�ײ��첽ˢ���ɽ�ʡ���������ݵ�ʱ��
/*****************************************************************************/
PUBLIC void UILAYER_SetLayerMemFormat(
                                      GUI_LCD_DEV_INFO const *dev_info_ptr,
                                      UILAYER_MEM_FORMAT_T format
                                      );

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: 
/*****************************************************************************/
PUBLIC UILAYER_MEM_FORMAT_T UILAYER_GetLayerMemFormat(
                                                      GUI_LCD_DEV_INFO const *dev_info_ptr
                                                      );


/*****************************************************************************/
//     Description : ˫buffer��д
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: ֻ��˫buffer��ͼ����ܵ��ã�����˫buffer�л������
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_DoubleMemWriteBack(
                                          GUI_LCD_DEV_INFO const *dev_info_ptr,
                                          const GUI_RECT_T       *dst_rect_ptr,
                                          const GUI_RECT_T       *clip_rect_ptr,
                                          const GUI_RECT_T       *src_rect_ptr
                                          );

/*****************************************************************************/
//     Description : �л�double mem
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: �����Ż�ר�ã��л�double mem�����ٿ������Ƽ��ڻ���ʱˢ����������ʱʹ��
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_SwitchDoubleMem(
                                       GUI_LCD_DEV_INFO const *dev_info_ptr,
                                       BOOLEAN is_switch
                                       );

/*****************************************************************************/
//     Description : �����������ݹ̶�
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: �����Ż�ר�ã��ٶ��������ݹ̶����ٽ���˫���忽�����Ƽ��ڻ���ʱʱʹ��
/*****************************************************************************/
PUBLIC void UILAYER_SetMainLayerImmutable(
                                          BOOLEAN is_immutable
                                          );

/*****************************************************************************/
//     Description : direct draw
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: �����Ż�ר�ã�����Ӳ�����ԣ��ϲ���ֱ��ˢ��lcdӲ����
//          �����ںϲ��������lcd buffer���������������ʾ����ȷ������ʹ��
/*****************************************************************************/
PUBLIC void UILAYER_SetDirectDraw(
                                  BOOLEAN is_direct
                                  );
                                  
/*****************************************************************************/
//     Description : is pre direct draw
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: �����Ż�ר�ã�����Ӳ�����ԣ��ϲ���ֱ��ˢ��lcdӲ����
//          �ж���һ֡�Ƿ�Ϊdricet draw
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsPerDirectDraw( void );

/*****************************************************************************/
//     Description : ��ò���ϳɵ�ͼ����Ŀ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC uint32 UILAYER_GetBltNum(
                                LCD_ID_E lcd_id,    //in
                                const GUI_RECT_T *rect_ptr   //in
                                );
                                       
/*****************************************************************************/
//     Description : ���ü�������rect_ptr�������ͼ��λ�ã�
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
//PUBLIC void UILAYER_SetClipRect(
//                                GUI_LCD_DEV_INFO const *dev_info_ptr,
//                                BOOLEAN is_clip_enable,
//                                const GUI_RECT_T *rect_ptr   //in
//                                );

/*****************************************************************************/
//     Description : �õ�layer��ȫ������
//    Global resource dependence : 
//  Author:   hongbo.lan
//    Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T UILAYER_GetLayerRect(GUI_LCD_DEV_INFO *dev_info_ptr);                           
/*****************************************************************************/
//     Description : copy layer to buffer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_CopyLayerToBuffer(
                               GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                               UILAYER_COPYBUF_PARAM_T *param_ptr,  //in
                               UILAYER_COPYBUF_T *copybuf_ptr   //in,out
                               );

/*****************************************************************************/
//     Description : store layer update rect
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_StoreLayerUpdateRect(
                                GUI_LCD_DEV_INFO const *dev_info_ptr
                                );

/*****************************************************************************/
//     Description : copy buffer to layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_CopyBufferToLayer(
                               GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                               UILAYER_COPYBUF_PARAM_T *param_ptr,  //in
                               UILAYER_COPYBUF_T *copybuf_ptr   //in,out
                               );                                                          

/*****************************************************************************/
//     Description : ��������lcd_buffer��osd layer�Ƿ���
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsOsdLayerEnable(void);

/*****************************************************************************/
//     Description : ����/�ر���������lcd_buffer��osd layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_EnableOsdLayer(
                                   BOOLEAN is_enable
                                   );

#ifdef WRE_SUPPORT
/*****************************************************************************/
//����Description : ��ĳ���ƶ���ָ���ĺ���
//����Global resource dependence : 
//����Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_MoveLayer(
                            GUI_LCD_DEV_INFO const *dst_dev_ptr,
                            GUI_LCD_DEV_INFO const *after_dev_ptr);
#endif
#if defined(MENU_CRYSTALCUBE_SUPPORT) || defined (MENU_CYCLONE_SUPPORT)  || defined (MET_MEX_SUPPORT)
/*****************************************************************************/
//  Description : change buffer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void UILAYER_SetLayerBufferPtr(
                                   GUI_LCD_DEV_INFO const *dev_info_ptr,
                                   uint8 *new_buf_addr,
                                   uint8 *new_raw_buf_addr,
                                   uint8 **old_buf_addr,
                                   uint8 **old_raw_buf_addr
                                   );
/*****************************************************************************/
//  Description : get raw layer buffer ptr
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8* UILAYER_GetLayerRawBufferPtr(GUI_LCD_DEV_INFO const *dev_info_ptr);
#endif

/*****************************************************************************/
//     Description : set mainlayer type
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetMainLayerType(
                                     DISPLAY_DATA_TYPE_E data_type
                                     );

/*****************************************************************************/
//     Description : ȥ����win_handle����win_handle�Ͽؼ�����������layer��Ϊ����
// camera�ȴ��ڴ�Ӧ��ʱ����ǰ�ͷ��ڴ����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
//PUBLIC void UILAYER_DeactiveAllLayerOfWinHandle(
//												MMI_HANDLE_T mmi_handle
//												);

/*****************************************************************************/
// Description : ������ʾʱ���Ƿ�565����תΪ888���ݵ�״̬
// Global resource dependence : 
// Author: Xiaoqing.Lu
// Note:
/*****************************************************************************/
//PUBLIC void UILAYER_SetDisplayRGB565TOARGB888State(BOOLEAN is_true);

/*****************************************************************************/
// Description : �����ʾʱ���Ƿ�565����תΪ888���ݵ�״̬
// Global resource dependence : 
// Author: Xiaoqing.Lu
// Note:
/*****************************************************************************/
//PUBLIC BOOLEAN UILAYER_GetDisplayRGB565TOARGB888State(void);

/*****************************************************************************/
//  Description : ����4��ֶ�κϳ�
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_MultiBlendLayer(
                                       LCD_ID_E    lcd_id,
                                       DISPLAY_BLENDLAYER_PARM_T*  blend_param_ptr
                                       );

/*****************************************************************************/
//  Description : blend src layers to dst layers.
//  Global resource dependence : 
//  Author: fulu.song
//  Note: compare to UILAYER_MultiBlendLayer, this function also works when less
//  than 4 src layers 
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_MultiBlendLayerEx(
                                       LCD_ID_E    lcd_id,
                                       DISPLAY_BLENDLAYER_PARM_T* blend_param_ptr
                                       );


/*****************************************************************************/
//     Description : get all blt layers of handle 
//    Global resource dependence : 
//  Author:   fulu.song
//    Note:
/*****************************************************************************/
PUBLIC uint32 UILAYER_GetBltLayersOfHandle(
                                  MMI_HANDLE_T mmi_handle,
                                  UILAYER_APPEND_BLT_T *layer_handle_arr,    //in/out
                                  uint32 arr_num    //in
                                  );

/*****************************************************************************/
//     Description : �����ļ���handleԤ��ˢ��lcd�ϣ�������˳������˸�����ӵ�˳��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetBltLayer(
                                UILAYER_APPEND_BLT_T *layer_handle_arr,
                                uint32 arr_size
                                );

/*****************************************************************************/
//     Description : �õ���ǰ��blt layer���ã�����blt��Ŀ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC uint32 UILAYER_GetBltLayer(
                                  UILAYER_APPEND_BLT_T *layer_handle_arr,    //in/out
                                  uint32 arr_num    //in
                                  );

/*****************************************************************************/
//     Description : ����ARGB888��ͼ����ΪԤ��
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetPreMultiplyAlpha(
                                        GUI_LCD_DEV_INFO const *dev_info_ptr,
                                        BOOLEAN                 is_pre_multiply_alpha,//ͼ���Ƿ�Ԥ��alpha�ĸ�ʽ
                                        BOOLEAN                 is_need_convert       //�Ƿ�ͼ�����ݽ���ת��
                                        );

/*****************************************************************************/
//     Description : ����ͼ��Ļص�����,��ˢ���󱻵���
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetBlendCallback(
                                     GUI_LCD_DEV_INFO const *dev_info_ptr,
                                     UILAYER_BLEND_CALLBACK callback,
                                     uint32                 param
                                     );

/*****************************************************************************/
//     Description : set clip rect
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetClipRect(
                                GUI_LCD_DEV_INFO const *dev_info_ptr,
                                GUI_RECT_T clip_rect
                                );

/*****************************************************************************/
//     Description : �������ͼ���˫����
//    Global resource dependence : 
//  Author:  James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_FreeAllLayerDoubleBuffer( void );

/*****************************************************************************/
//  Description : deactive layer handle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void UILAYER_DeactiveLayer(GUI_LCD_DEV_INFO const *dev_info_ptr);

/*****************************************************************************/
//  Description : active layer handle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void UILAYER_ActiveLayer(GUI_LCD_DEV_INFO const *dev_info_ptr);

/*****************************************************************************/
// 	Description : print info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void UILAYER_PrintInfo(void);


/*****************************************************************************/
//     Description : switch mainlayer������cameraʹ�ã����Խ�ʡһ��buffer��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note: is_setto_mainlcd_buffer=TRUE������Ϊlcd_buffer��FALSE����lcd_buffer���û�������̬���䣩
/*****************************************************************************/
PUBLIC void UILAYER_SwitchMainLayer(
                                    BOOLEAN is_setto_mainlcd_buffer
                                    );

/*****************************************************************************/
//     Description : ����RGB565 ��ͼ����ΪARGB555
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ChangeLayerRGB565ToARGB555(
									                                        GUI_LCD_DEV_INFO const *dev_info_ptr,
									                                        BOOLEAN                 is_need_convert ,
									                                        BOOLEAN 			Alpha_Bit
									                                        );
/*****************************************************************************/
//     Description : ����RGB565 ��ͼ����ΪARGB5515
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ChangeLayerRGB565ToARGB5515(
									                                        GUI_LCD_DEV_INFO const *dev_info_ptr,
									                                        BOOLEAN                 is_need_convert ,
									                                        BOOLEAN       		Alpha_Bit
									                                        );
/*****************************************************************************/
//     Description : ����ARGB555 ��ͼ����ΪRGB565
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ChangeLayerARGB555ToRGB565(
									                                        GUI_LCD_DEV_INFO const *dev_info_ptr,
									                                        BOOLEAN                 is_need_convert 
									                                        );
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus   
    }
#endif


#endif  /* FILENAME_H */
/*! @} */
