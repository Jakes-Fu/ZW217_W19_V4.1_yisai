/****************************************************************************
** File Name:      ui_layer.c                                               *
** Author:                                                                 *
** Date:           07/08/2009                                              *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/08/2009     bin.ji        Create
** 
****************************************************************************/
#define UI_LAYER_C
//#define UI_LAYER_SUPPORT_LOW_MEMORY
/**-------------------------------------------------------------------------*
 **                         Include Files                                   *
 **------------------------------------------------------------------------*/

#ifdef UI_MULTILAYER_SUPPORT	//֧�ֶ�ͼ��

#include "mmi_kernel_trc.h"
#include "ui_layer.h"
#include "guilcd.h"
#include "mmk_app.h"
//#include "lcd_cfg.h"
#include "guicommon.h"
#include "mmi_theme.h"
#include "dal_display.h"
#include "sci_api.h"
#include "mmi_utility.h"
#include "os_import.h"
#include "os_api.h"
/**-----------------------------------------------------------------------------*
 **                         MACRO Declaration                                   *
 **----------------------------------------------------------------------------*/

//#define UILAYER_MAIN_HANDLE             (GUI_BLOCK_MAIN)    //main_handle

//#define RGB5652ARGB8888(rgb565,alpha) ( ((alpha)<<24) |((((rgb565) >> 8) & 0xf8)<<16)|((((rgb565) >> 3) & 0xfc)<<8)|(((rgb565) << 3)  & 0xf8))
//#define RGB8882RGB565(rgb888)   ((((rgb888)&0xf80000)>>8)|(((rgb888)&0xfc00)>>5)|(((rgb888)&0xf8)>>3))

//����layer�Ŀ��ߣ�ÿ�����ص�bit������buf_size���ֽ���������������8�������ҪԤ��8���㣩
//#define UILAYER_SIZE_OF_PIXELS(width, height, bits_per_pixel)       (((width)*(height)+8)*(int32)((bits_per_pixel)>>3))
#define UILAYER_SIZE_OF_PIXELS(width, height, bits_per_pixel)       (((width)*(height))*(int32)((bits_per_pixel)>>3))
#define MAX_ALPHA_VALUE     255 //ȫ��͸��ֵ

//����8�ֽ�������ַ
// #ifdef UILAYER_NEED_ALIGN8
//     #define ALIGN_8BYTES_POS_LEFT(pos)     ((((uint16)pos)>>3)<<3)
//     #define ALIGN_8BYTES_POS_RIGHT(pos)     (((((uint16)pos)>>3)<<3)+7)
// #else
//     #define ALIGN_8BYTES_POS_LEFT(pos)      (pos)
//     #define ALIGN_8BYTES_POS_RIGHT(pos)     (pos)
// #endif

//����8�ֽ��Ҷ����ַ
//#define ALIGN_8BYTES_ADDR_RIGHT(addr)    (((((uint32)addr)+7)>>3)<<3)
#define ALIGN_256BYTES_ADDR_RIGHT(addr)    (((((uint32)addr)+255)>>8)<<8)
//#define ALIGN_256BYTES_ADDR_RIGHT(addr)  (((uint32)addr+255)/256*256)

//���ĳ����ɫ
#define GET_COLOR_BY_POS(buf_ptr, buf_width, x, y)          *((buf_ptr) + (y)*(buf_width)+(x))
#define SET_COLOR_BY_POS(buf_ptr, buf_width, x, y, color)   { *((buf_ptr) + (y)*(buf_width)+(x)) = (color); }

#define MAX_8ALIGN_EDGE_PIXEL           16      //���Ҹ���8������

//layer buffer��β�������ֽ�
#define LAYER_BUF_TAIL_SIZE     (256+sizeof(uint32))          //8�ֽڶ��룬�ټ���uint32��memcheck��־

#define NO_ALPHA_COLOR  0x00
#define PM_NO_ALPHA_COLOR  0xFF000000 //pre multiply alpha

//Ӳ��ͼ��������<1024
#define HARDWARE_LAYER_WIDTH_LIMIIT         1024
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

//��lcd��handle
LOCAL DISPLAY_BLEND_LAYER_T s_mainlcd_handle = {0};
LOCAL BOOLEAN s_is_pre_direct_draw = FALSE;
//mutex
LOCAL    SCI_MUTEX_PTR  s_uilayer_mutex = PNULL;
LOCAL uint32*			s_layer_doublebuffer_flag_ptr = PNULL;  //�������ձ�־
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

//blt pixel����ָ��
typedef void (*BLTPIXEL_FUNC_T)(
                    uint8 *src_buf_ptr,
                    uint8 *dst_buf_ptr
                    );

//layer�ṹ
/*
typedef struct
{
//    BOOLEAN         is_clip_enable;     //���������Ƿ���
    GUI_RECT_T      clip_rect;          //��������
    uint8*          raw_layer_buf_ptr;  //ԭʼ��buf��ַ����Ҫ��8�ֽڶ���ת���������õ�layer_struct
    uint8*          raw2_layer_buf_ptr; //˫�������ר��
    GUI_RECT_T      rect;               //ʵ�ʵ��������򣬴��ݸ�DISPLAY_BLEND_LAYER_T�ṹoffset_x/width��Ҫת��8�ֽڶ���
    DISPLAY_BLEND_LAYER_T  layer_struct;
    MMI_HANDLE_T    owner_handle;       //�������ڻ�ؼ����
    UILAYER_MEM_FORMAT_T  format;       //Ĭ��ΪUILAYER_MEM_NONE������Ϊ�Ż�ʹ��
    BOOLEAN         is_immutable_drawing;//�������ò����ݲ�������Խ����첽ˢ��
    BOOLEAN         is_raw2;
    BOOLEAN         is_autofreememory;  //����lose focus��ʱ���Զ��ͷ��ڴ�
    uint32          index;              //��s_layer_arr��������λ��
    GUI_LCD_DEV_INFO    lcd_dev_info;   //������lcd_dev_info
}UILAYER_STRUCT_T;
*/

//layer�ṹ
typedef struct
{
    GUI_LCD_DEV_INFO        lcd_dev_info;   //������lcd_dev_info
    MMI_HANDLE_T            owner_handle;       //�������ڻ�ؼ����
    UILAYER_BLEND_CALLBACK  callback;              //�ص�����
    uint32                  param;
    uint8*                  raw_layer_buf_ptr;  //ԭʼ��buf��ַ����Ҫ��8�ֽڶ���ת���������õ�layer_struct
    uint32                  buf_addr;      //it is buffer address of display data for ARGB888/ARGB666/ARGB565/RGB565/RGB555
    uint32                  layer_buf_size; 
    int16                   x;
    int16                   y;  
    uint16		            width;     //widht of block ,uinit is pixel
    uint16 		            height;	  //height of block,unit is pixel
    DISPLAY_DATA_TYPE_E     type;       //data type
    UILAYER_MEM_FORMAT_T    format;       //Ĭ��ΪUILAYER_MEM_NONE������Ϊ�Ż�ʹ��
    BOOLEAN                 is_autofreememory;  //����lose focus��ʱ���Զ��ͷ��ڴ�
    BOOLEAN                 is_enable;     // 1:enable this blcok,0: don't enable this block
    BOOLEAN                 immutable;
    BOOLEAN                 is_immutable_drawing;//�������ò����ݲ�������Խ����첽ˢ��
#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
    BOOLEAN                 is_raw2;
    BOOLEAN                 is_switch_double_buffer;
    uint8                   *raw2_layer_buf_ptr; //˫�������ר��
#endif
	BOOLEAN                 colorkey_en;// 1:use colorkey;0: don't use colorkey
	uint32                  colorkey;   // colorkey value
    uint32                  alpha;      //alpha value 
	DISPLAY_ALPHA_TYPE_E    alpha_sel;  //pixel alpha or block alpha
    UILAYER_MEM_FUNC_T      mem_func;
	const char*			file_name_ptr;
	uint32				file_line;
    DISPLAY_RECT_T      clip_rect;  //��������
}UILAYER_STRUCT_T;

//�ڴ��⣺ͬʱ��¼��ַ�͵�ַ�ڵ�����
typedef struct 
{
    uint32 *mem_ptr;
    uint32 mem_value;
}MMI_MEMMONITER_INFO_T;

//bltͼ�����Ϣ
typedef struct
{
    BOOLEAN is_valid;
    GUI_LCD_DEV_INFO lcd_dev_info;
    UILAYER_LEVEL_E layer_level;
}UILAYER_BLT_T;

//bltͼ�����Ϣ
typedef struct
{
    GUI_LCD_DEV_INFO lcd_dev_info;
    uint32           buf_addr;
}UILAYER_BLEND_CALLBACK_PARAM_T;

// typedef struct 
// {
//     BOOLEAN is_enable;
//     DISPLAY_BLEND_LAYER_T layer;
// }BLEND_DST_LAYER_T;

//LOCAL BLEND_DST_LAYER_T s_osd_layer = {0};
LOCAL GUI_LCD_DEV_INFO s_osd_layer = {0};

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

#ifdef MMI_SOFTBLTLAYER_ENABLE

extern MMISET_THEME_TYPE_E MMIAPISET_GetCurrentThemeType(void);

/*****************************************************************************/
//  Description:  multi layer blend interface for the APP
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
extern ERR_LCD_E SOFT_InvalidateRectUsingLayer(LCD_ID_E lcd_id,
				DISPLAY_BLEND_LAYER_T *dst_layer_handle,
    				DISPLAY_BLEND_LAYER_T *layer0_ptr,
    				DISPLAY_BLEND_LAYER_T *layer1_ptr,
    				DISPLAY_BLEND_LAYER_T *layer2_ptr,
    				DISPLAY_BLEND_LAYER_T *layer3_ptr,
    				DISPLAY_RECT_T *rect_ptr);
#endif

//#ifdef WIN32
//extern ERR_DISPLAY_E MSDev_DISPLAY_BlendLayer( LCD_ID_E lcd_id,
//											   DISPLAY_BLEND_LAYER_T *layer_arr[],
//											   uint32 layer_num,
//											   DISPLAY_RECT_T *rect_ptr);
//#endif

extern BOOLEAN GUILCD_MultiBlendRect(
                                     LCD_ID_E       lcd_id,             //in:
                                     GUI_RECT_T     rect,               //in:
                                     uint32         update_block_set    //in:for tv out
                                     );

/**-------------------------------------------------------------------------*
 **                         Global Variables                                *
 **------------------------------------------------------------------------*/
LOCAL UILAYER_BLT_T s_layer_blt[UILAYER_TOTAL_BLT_COUNT] = {0};       //����Ԥ��ˢ��lcd�ϵ�layer���

LOCAL UILAYER_BLEND_CALLBACK_PARAM_T s_blend_callback_array[UILAYER_TOTAL_BLT_COUNT] = {0}; 
LOCAL uint8 s_blend_callback_num = 0;

LOCAL UILAYER_STRUCT_T s_layer_arr[UILAYER_TOTAL_LAYER_COUNT] = {0};   //UI�߼���layer�����
LOCAL UILAYER_HANDLE_T s_alpha_src_layer = UILAYER_NULL_HANDLE;

LOCAL MMI_MEMMONITER_INFO_T s_mmi_memmoniter_info = {0};
LOCAL BOOLEAN s_is_setto_mainlcd_buffer = FALSE;

//LOCAL BOOLEAN s_IsDisplayRGB565TOARGB888 = TRUE;

/*--------------------------------------------------------------------------*/
/*                    LOCAL FUNCTION DECLARE                                */
/*--------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : active mainlayer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void ActiveMainLayer( void );

/*****************************************************************************/
//     Description : ����layer���ݲ���
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: �����Ż�ר�ã��������ݲ��䣬�ײ��첽ˢ���ɽ�ʡ���������ݵ�ʱ��
/*****************************************************************************/
LOCAL void SetLayerMemFormat(
                             GUI_LCD_DEV_INFO const *dev_info_ptr,
                             UILAYER_MEM_FORMAT_T type
                             );

/*****************************************************************************/
//     Description : copy display blend layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CopyDisplayBlendLayer(
                                    DISPLAY_BLEND_LAYER_T *dst_layer,   //dst
                                    UILAYER_STRUCT_T *layer
                                    );

/*****************************************************************************/
//     Description : is enable
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsLayerStructEnable(
                              UILAYER_STRUCT_T *layer_ptr
                              );

/*****************************************************************************/
//  Description : destroy osd layer
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void DestroyOsdLayer(void);

/*****************************************************************************/
//  Description : create osd layer
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void CreateOsdLayer(void);

/*****************************************************************************/
//     Description : ת��rect��ʽ��GUI_RECT_T->DISPLAY_RECT_T
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL DISPLAY_RECT_T Convert2DispRect(
                                     GUI_RECT_T rect
                                     );

/*****************************************************************************/
//     Description : ת��rect��ʽ��DISPLAY_RECT_T->GUI_RECT_T
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T Convert2GuiRect(
                                DISPLAY_RECT_T disp_rect
                                );

/*****************************************************************************/
//     Description : blend layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL ERR_DISPLAY_E BlendLayer(  LCD_ID_E lcd_id,
				                                 DISPLAY_BLEND_LAYER_T *dst_layer_ptr,
    				                             DISPLAY_BLEND_LAYER_T *layer_arr[],
				                                 uint32 array_size,
    				                             DISPLAY_RECT_T *rect_ptr);

/*****************************************************************************/
//     Description : store layer update rect
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void StoreLayerUpdateRect(
                                GUI_LCD_DEV_INFO const *dev_info_ptr
                                );

/*****************************************************************************/
//     Description : blt pixel
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL __inline void BltPixel_ARGB888ToARGB888(
                    uint8 *src_buf_ptr,
                    uint8 *dst_buf_ptr
                    );

/*****************************************************************************/
//     Description : blt pixel
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL __inline void BltPixel_RGB565ToRGB565(
                    uint8 *src_buf_ptr,
                    uint8 *dst_buf_ptr
                    );

/*****************************************************************************/
//     Description : blt pixel
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL __inline void BltPixel_RGB565ToARGB888(
                    uint8 *src_buf_ptr,
                    uint8 *dst_buf_ptr
                    );

/*****************************************************************************/
//     Description : blt pixel
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL __inline void BltPixel_ARGB888ToRGB565(
                    uint8 *src_buf_ptr,
                    uint8 *dst_buf_ptr
                    );

/*****************************************************************************/
//     Description : �����������͵õ�blt pixel�ĺ���
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BLTPIXEL_FUNC_T GetBltPixelFunc(
                              DISPLAY_DATA_TYPE_E src_data_type,
                              DISPLAY_DATA_TYPE_E dst_data_type
                              );

/*****************************************************************************/
//     Description : �����������͵õ�pixel���ֽ���
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL uint32 GetBytesPerPixel(
                              DISPLAY_DATA_TYPE_E data_type
                              );

/*****************************************************************************/
//     Description : ��ĳ������ˢ����һ����ȥ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void BltBuffer(
                     uint8 *src_buf_ptr,
                     uint32 src_buf_width,
                     DISPLAY_DATA_TYPE_E src_data_type,
                     uint8 *dst_buf_ptr,
                     uint32 dst_buf_width,
                     DISPLAY_DATA_TYPE_E dst_data_type,
                     uint32 blt_width,
                     uint32 blt_height
                     );

/*****************************************************************************/
//     Description : �Ƚ�lcd dev
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CompareLcdDev(
                            GUI_LCD_DEV_INFO const *dev_info_ptr1,
                            GUI_LCD_DEV_INFO const *dev_info_ptr2
                            );

/*****************************************************************************/
//     Description : ����layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL uint32 GetLayerBytesPerPixel(
                       UILAYER_STRUCT_T *layer
                       );

/*****************************************************************************/
//     Description : �ж�handle�Ƿ�focus
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsFocusHandle(
                            MMI_HANDLE_T owner_handle
                            );

/*****************************************************************************/
//     Description : ����blend�ṹĬ������
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void SetBlendDefaultData(
                               DISPLAY_BLEND_LAYER_T *blend_info_ptr
                               );

/*****************************************************************************/
//     Description : ����layer��λ�ã������lcd���Ͻǵ����꣩
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN SetLayerPosition(
                                UILAYER_STRUCT_T *layer,
                                int16 offset_x,
                                int16 offset_y
                                );

/*****************************************************************************/
//     Description : �õ�lcd��ȫ������
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetLCDRect(
                            GUI_LCD_ID_E lcd_id
                            );

/*****************************************************************************/
//     Description : check param rect
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckParamRect(
                             const GUI_RECT_T *rect_ptr
                             );

// #ifdef UILAYER_NEED_ALIGN8
// /*****************************************************************************/
// //     Description : ����8�����Ե���ݣ�Ϊ������refҪ���8����ˢ�£�������ˢ������
// //    Global resource dependence : 
// //  Author:   bin.ji
// //    Note:
// /*****************************************************************************/
// LOCAL void Save8AlignData(
//                           UILAYER_STRUCT_T *layer,
//                           const GUI_RECT_T *rect_ptr,
//                           void **data_ptr
//                           );
// 
// /*****************************************************************************/
// //     Description : ����8�����Ե���ݣ�Ϊ������refҪ���8����ˢ�£�������ˢ������
// //    Global resource dependence : 
// //  Author:   bin.ji
// //    Note:
// /*****************************************************************************/
// LOCAL void Load8AlignData(
//                           UILAYER_STRUCT_T *layer,
//                           const GUI_RECT_T *rect_ptr,
//                           void **data_ptr
//                           );
// #endif

/*****************************************************************************/
//     Description : ����ü�lcd/layer��Χ�ڵ���Ч����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN GetValidRect(
                            UILAYER_STRUCT_T *layer,
                            const GUI_RECT_T *src_rect_ptr,
                            GUI_RECT_T *dst_rect_ptr    //out
                            );

/*****************************************************************************/
//     Description : ���layer����Ϣ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void GetLayerInfo(
                        UILAYER_STRUCT_T *layer,
                        UILAYER_INFO_T *layer_info_ptr     //output
                        );

/*****************************************************************************/
//     Description : �õ�layer��ȫ������
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetLayerRect(
                              UILAYER_STRUCT_T   *layer_ptr
                              );

/*****************************************************************************/
//     Description : �õ�layer��ȫ������
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetLayerBlendRect(
                              DISPLAY_BLEND_LAYER_T  *layer_blend_ptr
                              );

/*****************************************************************************/
//     Description : �������layer������λ��ת��Ϊ�����lcd������λ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN ConvertLayer2LCDRect(
                                 UILAYER_STRUCT_T   *layer_ptr,
                                 const GUI_RECT_T   *layer_rect_ptr,
                                 GUI_RECT_T         *lcd_rect_ptr      //out
                                 );

/*****************************************************************************/
//     Description : �������lcd������λ��ת��Ϊ�����layer������λ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN ConvertLCD2LayerRect(
                                 UILAYER_STRUCT_T   *layer_ptr,
                                 const GUI_RECT_T   *lcd_rect_ptr,
                                 GUI_RECT_T         *layer_rect_ptr    //out
                                 );

/*****************************************************************************/
//     Description : ����8��������
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetAlign8Rect(
                               GUI_RECT_T rect
                               );

/*****************************************************************************/
//     Description : check layer id
//    Global resource dependence : 
//  Author:   bin.ji
//    Note: TRUE ͼ��id    FALSE��ͼ�� id
/*****************************************************************************/
LOCAL BOOLEAN CheckLayerId(
                           GUI_LCD_DEV_INFO const *dev_info_ptr
                           );

/*****************************************************************************/
//     Description : multi layer blend interface for the APP
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL ERR_LCD_E InvalidateRectUsingLayer(
                    LCD_ID_E lcd_id,
				    DISPLAY_BLEND_LAYER_T *dst_layer_handle_ptr,
    				DISPLAY_BLEND_LAYER_T *layer_arr[],
    				uint32 array_size,
    				DISPLAY_RECT_T *rect_ptr
                    );

/*****************************************************************************/
//     Description : ��softblt����refblt
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN GetIsSoftBlt(void);

/*****************************************************************************/
//     Description : �õ�mainlcd layer�ľ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL DISPLAY_BLEND_LAYER_T *GetMainLcdhandle(void);

/*****************************************************************************/
//     Description : �õ�sublcd layer�ľ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL DISPLAY_BLEND_LAYER_T GetSubLcdhandle(void);

/*****************************************************************************/
//     Description : ��ʼ��s_mainlcd_handle�ṹ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void InitMainLcdHandle(void);

/*****************************************************************************/
//     Description : ��㹦���Ƿ���
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:  FALSE һ�㣨���㣩 TRUE ���
/*****************************************************************************/
LOCAL BOOLEAN IsMultiLayerEnable(void);

/*****************************************************************************/
//     Description : ���rect�������Ƿ�͸��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsRectTransparent(
                                GUI_LCD_DEV_INFO const *dev_info_ptr,
                                const GUI_RECT_T *rect_ptr
                                );

/*****************************************************************************/
//     Description : �ж��Ƿ�������
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMainLayerHandle(
                                GUI_LCD_DEV_INFO const *dev_info_ptr
                                );

/*****************************************************************************/
//     Description : �����̶���layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void ProtectLayerBuf(
                           UILAYER_STRUCT_T *layer
                           );

/*****************************************************************************/
//     Description : �ͷ�layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void ReleaseLayer(
                        UILAYER_STRUCT_T *layer
                        );

/*****************************************************************************/
//     Description : ��block_idתΪlayerָ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL UILAYER_STRUCT_T *GetLayerPtr(
                                    GUI_LCD_DEV_INFO const *dev_info_ptr
                                    );

/*****************************************************************************/
//     Description : ��block_idתΪlayerָ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL UILAYER_STRUCT_T *GetLayerPtrEx(
                                      GUI_LCD_DEV_INFO const *dev_info_ptr,
                                      BOOLEAN is_protect
                                      );

/*****************************************************************************/
//     Description : ����layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void ActiveLayer(
                       UILAYER_STRUCT_T *layer
                       );

/*****************************************************************************/
//     Description : ȥ����layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void DeactiveLayer(
                         UILAYER_STRUCT_T *layer
                         );


/*****************************************************************************/
//     Description : �����ڴ�
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void *AllocLayerBuf(
                          UILAYER_MEM_FUNC_T* func_ptr,
                          uint32 buf_size,
                          const char* file_name,
                          uint32 file_line
                          );

/*****************************************************************************/
//     Description : �ͷ��ڴ�
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void FreeLayerBuf(
                        UILAYER_MEM_FUNC_T* func_ptr,
                        void *buf_ptr
                        );

/*****************************************************************************/
//     Description : ����˫����
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:�����ڴ�ˮ��ʱ,�򲻷���˫����
/*****************************************************************************/
LOCAL void AllocLayerRaw2Buf(
                             UILAYER_STRUCT_T *layer
                             );

/*****************************************************************************/
//     Description : ����layer������layer���
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL UILAYER_RESULT_E CreateLayer(
                                   LCD_ID_E lcd_id,
                                   MMI_HANDLE_T owner_handle,
                                   int16 offset_x,
                                   int16 offset_y,
                                   int16 width,
                                   int16 height,
                                   BOOLEAN is_autofreememory,
                                   BOOLEAN is_bg_layer,
                                   UILAYER_MEM_FORMAT_T format,
                                   UILAYER_MEM_FUNC_T* func_ptr,
                                   GUI_LCD_DEV_INFO *dev_info_ptr,   //out
                                   const char*			file_name_ptr,
                                   uint32				file_line
                                   );

/*****************************************************************************/
//     Description : ���ݴ�����������layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateHandle(
                        MMI_HANDLE_T owner_handle,
                        GUI_LCD_DEV_INFO const *dev_info_ptr,
                        const DISPLAY_BLEND_LAYER_T *blend_ptr,
                        uint8 *buf_ptr,
                        uint32 buf_size,
                        BOOLEAN is_autofreememory,
                        UILAYER_MEM_FORMAT_T format,
                        UILAYER_MEM_FUNC_T* func_ptr,
                        const char*			file_name_ptr,
                        uint32				file_line
                        );

/*****************************************************************************/
//     Description : ����һ�����е�layer handle
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN FindValidHandle(
                              UILAYER_HANDLE_T *layer_handle_ptr    //out
                              );

/*****************************************************************************/
//     Description : ��blt�в���layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN FindBltLayer(
                           GUI_LCD_DEV_INFO const *dev_info_ptr
                           );
                           
/*****************************************************************************/
//     Description : test all blend info
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void TestAllBlendInfo(
                            LCD_ID_E lcd_id,         //in
                            GUI_RECT_T align_rect,   //in
                            BOOLEAN* is_all_immutable_ptr,
                            BOOLEAN* is_all_valid_width_ptr
                            );

/*****************************************************************************/
//     Description : deactive main layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void DeactiveMainLayer( void );

/*****************************************************************************/
//     Description : free main layer buffer2
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void FreeMainLayerBuffer2( void );

/*****************************************************************************/
//     Description : ����ͼ��Ļص�����,��ˢ���󱻵���
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void BlendCallback(
                         uint32 param
                         );

/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : is enable
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsLayerStructEnable(
                              UILAYER_STRUCT_T *layer_ptr
                              )
{
    if ((PNULL != layer_ptr) && layer_ptr->is_enable)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//     Description : ����һ�����е�layer handle
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN FindValidHandle(
                              UILAYER_HANDLE_T *layer_handle_ptr    //out
                              )
{
    BOOLEAN result = FALSE;
    uint32 i = 0;
//    uint32 used_num = 0;

    if (PNULL == layer_handle_ptr)
    {
        //SCI_TRACE_LOW:"FindValidHandle() PNULL == layer_handle_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_737_112_2_18_3_22_51_39,(uint8*)"");
        return result;/*lint !e527*/
    }
    SCI_GetMutex(s_uilayer_mutex, SCI_WAIT_FOREVER);
    for (i = 0; i < UILAYER_TOTAL_LAYER_COUNT; i++)
    {
        if (!s_layer_arr[i].is_enable)
        {
            //initialize s_layer_arr[i]
            SCI_MEMSET(&(s_layer_arr[i]), 0x00, sizeof(UILAYER_STRUCT_T));
            s_layer_arr[i].is_enable = TRUE;
            if (0 == i)
            {
                //0λ��Ĭ�ϱ�������
                *layer_handle_ptr = GUI_BLOCK_MAIN; 
            }
            else
            {
                *layer_handle_ptr = (i + GUI_BLOCK_MAX); 
            }
            result = TRUE;
            break;
        }
    }
    SCI_PutMutex(s_uilayer_mutex);
//    for (i = 0; i < UILAYER_TOTAL_LAYER_COUNT; i++)
//    {
////        if (!s_layer_arr[i].layer_struct.layer_info.is_enable)
//        if (!IsLayerStructEnable(&s_layer_arr[i]))
//        {
//            if (!result)
//            {
//                if (0 == i)
//                {
//                    //0λ��Ĭ�ϱ�������
//                    *layer_handle_ptr = GUI_BLOCK_MAIN; 
//                }
//                else
//                {
//                    *layer_handle_ptr = (i + GUI_BLOCK_MAX); 
//                }
//                result = TRUE;
//            }
//        }
//        else
//        {
//            used_num++;
//        }
//    }    
    return result;
}

/*****************************************************************************/
//     Description : ����layer͸����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetLayerAlpha(
                                  GUI_LCD_DEV_INFO const *dev_info_ptr,
                                  uint8 alpha_value            //͸����
                                  )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    
    if (IsMainLayerHandle(dev_info_ptr))
    {
        //��ֹ���㱻�����
        return;
    }
    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }

    if (PNULL != layer)
    {
        layer->alpha = alpha_value;
        if (DATA_TYPE_ARGB888 == layer->type)
        {
            layer->alpha_sel = (255 == alpha_value) ? DISPLAY_PIXEL_ALPHA : DISPLAY_COMBO_ALPHA;
        }
//        layer->layer_struct.layer_info.layer_data.alpha = alpha_value;
    }
    else
    {
        //SCI_TRACE_LOW:"UILAYER_SetLayerAlpha() PNULL == layer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_797_112_2_18_3_22_51_40,(uint8*)"");
    }
}

/*****************************************************************************/
//     Description : ����layer͸����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_WeakLayerAlpha(
                                  GUI_LCD_DEV_INFO const *dev_info_ptr,
                                  uint8 alpha_value            //͸����
                                  )
{
    UILAYER_STRUCT_T *layer = GetLayerPtrEx(dev_info_ptr, TRUE);
    GUI_COLOR_T *p = PNULL;
    uint32 *u32_p = PNULL;
    uint32 count = 0;
    uint32 n = 0;

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }
    if ((PNULL != layer) && (PNULL != layer->buf_addr))
    {
        p = (GUI_COLOR_T*)layer->buf_addr;
        u32_p = (uint32*)layer->buf_addr;
        switch (layer->type)
        {
        case DATA_TYPE_ARGB888:
            count = layer->layer_buf_size / sizeof(uint32);
            while (count)
            {
                n = (*u32_p)>>24;
                if (0 != n)
                {
                    n = (n*alpha_value)>>8;
                    *u32_p = ((n<<24)|((*u32_p)&0xffffff));
                }
                u32_p++;
                count--;
            }
            break;

        case DATA_TYPE_RGB565:
        default:
//            color = layer->layer_struct.layer_info.layer_data.colorkey;
//            SCI_MEM16SET(p, color, layer->layer_struct.layer_buf_size);
            break;
        }
        StoreLayerUpdateRect(dev_info_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"UILAYER_WeakLayerAlpha() PNULL == layer || PNULL == layer->buf_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_851_112_2_18_3_22_51_41,(uint8*)"");
    }
}

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
                                     )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);

//    if (IsMainLayerHandle(dev_info_ptr))
//    {
//        //��ֹ���㱻�����
//        return;
//    }
    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }
    if (PNULL != layer)
    {
        if (DATA_TYPE_RGB565 == layer->type)
        {
            layer->colorkey_en = is_color_key_enable;
            layer->colorkey = color_key;
        }
        else
        {
            //arg888��ʽ��alpha���ƣ�����color_key
            layer->colorkey_en = FALSE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"UILAYER_SetLayerColorKey() PNULL == layer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_891_112_2_18_3_22_51_42,(uint8*)"");
    }
}

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
                                              )
{
    UILAYER_RESULT_E result = UILAYER_RESULT_SUCCESS;

//#ifdef UI_MULTILAYER_SUPPORT
    LCD_ID_E lcd_id = 0;
    MMI_HANDLE_T owner_handle = 0;
    int16 offset_x = 0;
    int16 offset_y = 0;
    int16 width = 0;
    int16 height = 0;    
    BOOLEAN is_autofreememory = FALSE;
    BOOLEAN is_bg_layer = FALSE;
    
    if (PNULL == create_ptr)
    {
        return UILAYER_RESULT_ERROR;
    }
    lcd_id = create_ptr->lcd_id;
    owner_handle = create_ptr->owner_handle;
    offset_x = create_ptr->offset_x;
    offset_y = create_ptr->offset_y;
    width = create_ptr->width;
    height = create_ptr->height;
    is_autofreememory = !create_ptr->is_static_layer;
    is_bg_layer = create_ptr->is_bg_layer;

    result = CreateLayer(lcd_id, owner_handle, offset_x, offset_y, width, height, is_autofreememory, is_bg_layer, 
        create_ptr->format, create_ptr->func_ptr, dev_info_ptr,file_name_ptr, file_line);
//#else
//	dev_info_ptr->block_id = UILAYER_NULL_HANDLE;
//	result = UILAYER_RESULT_ERROR;
//#endif

    return result;
}

/*****************************************************************************/
//     Description : ����layer������layer���
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL UILAYER_RESULT_E CreateLayer(
                                   LCD_ID_E lcd_id,
                                   MMI_HANDLE_T owner_handle,
                                   int16 offset_x,
                                   int16 offset_y,
                                   int16 width,
                                   int16 height,
                                   BOOLEAN is_autofreememory,
                                   BOOLEAN is_bg_layer,
                                   UILAYER_MEM_FORMAT_T format,
                                   UILAYER_MEM_FUNC_T* func_ptr,
                                   GUI_LCD_DEV_INFO *dev_info_ptr,   //out
                                   const char*       file_name_ptr,
                                   uint32            file_line
                                   )
{
    uint8 *buf_ptr = PNULL;
    uint32 buf_size = 0;
    UILAYER_HANDLE_T layer_handle = 0;
    DISPLAY_BLEND_LAYER_T blend_layer = {0};
    UILAYER_RESULT_E result = UILAYER_RESULT_SUCCESS;
    uint32 bits_per_pixel = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    BOOLEAN create_result = FALSE;

    if (GUI_MAIN_LCD_ID != lcd_id)
    {
        //Ŀǰֻ֧��GUI_MAIN_LCD_ID�ϴ����²�
        return UILAYER_RESULT_ERROR;
    }
    if (PNULL == dev_info_ptr)
    {
        return UILAYER_RESULT_ERROR;
    }
    if (!IsMultiLayerEnable())
    {
        return UILAYER_RESULT_MULTILAYER_DISABLED;
    }

    lcd_dev_info.lcd_id = lcd_id;
    if (FindValidHandle(&layer_handle))
    {
        
        if ( PNULL != func_ptr
            && PNULL != func_ptr->Malloc
            && PNULL != func_ptr->Free )
        {
        }

        SCI_MEMSET(&blend_layer, 0x00, sizeof(DISPLAY_BLEND_LAYER_T));
        SetBlendDefaultData(&blend_layer);        
        //ת��offset_x, offset_y      
// #ifdef UILAYER_NEED_ALIGN8
//         blend_layer.interval_left = offset_x&7;
//         blend_layer.interval_right = 16-blend_layer.interval_left-(width%8);   //���Ҹ���8
//         blend_layer.layer_info.pos.x = offset_x-blend_layer.interval_left;  //8����
//         blend_layer.layer_info.width = blend_layer.interval_left+blend_layer.interval_right+width;   
// #else
        blend_layer.layer_info.pos.x = offset_x;
        blend_layer.layer_info.width = blend_layer.interval_right+width;   
//#endif        
        blend_layer.layer_info.pos.y = offset_y;
        blend_layer.layer_info.height = height;

        //set color type
        if (is_bg_layer)
        {
            blend_layer.layer_info.layer_data.type = DATA_TYPE_RGB565;
            blend_layer.layer_info.layer_data.alpha_sel = DISPLAY_BLOCK_ALPHA;
            bits_per_pixel = sizeof(GUI_COLOR_T)*8;
        }
        else
        {
            blend_layer.layer_info.layer_data.type = DATA_TYPE_ARGB888;//DATA_TYPE_RGB565;
            blend_layer.layer_info.layer_data.alpha_sel = DISPLAY_PIXEL_ALPHA;
            bits_per_pixel = sizeof(uint32)*8;
        }
        //alloc
        if (is_autofreememory && !IsFocusHandle(owner_handle))
        {
            //�Զ��ͷ�&����focus״̬��deactive
            buf_size = 0;
            buf_ptr = PNULL;
        }
        else
        {
            buf_size = UILAYER_SIZE_OF_PIXELS(
                blend_layer.layer_info.width, 
                blend_layer.layer_info.height, 
                bits_per_pixel
                );
            buf_ptr = AllocLayerBuf(func_ptr, buf_size, file_name_ptr, file_line);
        }
        lcd_dev_info.block_id = layer_handle;
        create_result = CreateHandle(owner_handle, &lcd_dev_info, &blend_layer, buf_ptr, buf_size, is_autofreememory, 
            format, func_ptr, file_name_ptr, file_line);
        if (!create_result)
        {
            //����ʧ�ܣ��ָ�is_enable
            UILAYER_ReleaseLayer(&lcd_dev_info);
            lcd_dev_info.block_id = UILAYER_NULL_HANDLE;
            result = UILAYER_RESULT_ERROR;
        }
    }
    else
    {
        //MMI_ASSERT_WARNING("ui_layer.c CreateLayer() layer num is max!");
        //SCI_TRACE_LOW:"ui_layer.c CreateLayer() layer num is max!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1033_112_2_18_3_22_51_43,(uint8*)"");
        lcd_dev_info.block_id = UILAYER_NULL_HANDLE;
        result = UILAYER_RESULT_USEDMAXNUM;
    }
    dev_info_ptr->lcd_id = lcd_dev_info.lcd_id;
    dev_info_ptr->block_id = lcd_dev_info.block_id;

    return result;
}

/*****************************************************************************/
//     Description : ���ݴ�����������layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateHandle(
                        MMI_HANDLE_T owner_handle,
                        GUI_LCD_DEV_INFO const *dev_info_ptr,
                        const DISPLAY_BLEND_LAYER_T *blend_ptr,
                        uint8 *buf_ptr,
                        uint32 buf_size,
                        BOOLEAN is_autofreememory,
                        UILAYER_MEM_FORMAT_T format,
                        UILAYER_MEM_FUNC_T* func_ptr,
                        const char*			file_name_ptr,
                        uint32				file_line
                        )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);

    if (!CheckLayerId(dev_info_ptr))
    {
        return FALSE;
    }
    if (PNULL == blend_ptr) 
    {
        //MMI_ASSERT_WARNING("CreateHandle() PNULL == blend_ptr");
        //SCI_TRACE_LOW:"CreateHandle() PNULL == blend_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1066_112_2_18_3_22_51_44,(uint8*)"");
        return FALSE;/*lint !e527*/
    }
    if (PNULL == layer)
    {
        //SCI_TRACE_LOW:"CreateHandle() PNULL == layer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1071_112_2_18_3_22_51_45,(uint8*)"");
        return FALSE;
    }

    if (PNULL != buf_ptr)
    {
//        SCI_ASSERT(0 == layer->raw_layer_buf_ptr);
        if (PNULL != layer->raw_layer_buf_ptr)
        {
            SCI_FREE(layer->raw_layer_buf_ptr);
        }
        layer->raw_layer_buf_ptr = buf_ptr;
        layer->buf_addr = (uint32)ALIGN_256BYTES_ADDR_RIGHT(layer->raw_layer_buf_ptr);
    }
    else
    {
        if (PNULL != layer->raw_layer_buf_ptr)
        {
            SCI_FREE(layer->raw_layer_buf_ptr);
        }
        layer->raw_layer_buf_ptr = PNULL;
        layer->buf_addr = PNULL;
    }

    layer->is_autofreememory = is_autofreememory;
    layer->owner_handle = MMK_ConvertIdToHandle(owner_handle);
//    SCI_MEMSET(&layer->layer_struct, 0x00, sizeof(DISPLAY_BLEND_LAYER_T));
//    layer->layer_struct = *blend_ptr;
    //set layer info
//    layer->layer_buf_size = blend_ptr->layer_buf_size;
//    layer->buf_addr = blend_ptr->layer_info.buf_addr ;
//    layer->is_enable = blend_ptr->layer_info.is_enable ;
    layer->x = blend_ptr->layer_info.pos.x;
    layer->y = blend_ptr->layer_info.pos.y;
    layer->width = blend_ptr->layer_info.width;
    layer->height = blend_ptr->layer_info.height;
    layer->type = blend_ptr->layer_info.layer_data.type;
    layer->alpha = blend_ptr->layer_info.layer_data.alpha;
    layer->alpha_sel = blend_ptr->layer_info.layer_data.alpha_sel;
    layer->colorkey = blend_ptr->layer_info.layer_data.colorkey;
    layer->colorkey_en = blend_ptr->layer_info.layer_data.colorkey_en;

    layer->layer_buf_size = buf_size;

    layer->format = format;

    if ( PNULL != func_ptr
        && PNULL != func_ptr->Malloc
        && PNULL != func_ptr->Free )
    {
        layer->mem_func = *func_ptr;
    }

#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
    layer->is_raw2 = FALSE;
    layer->is_switch_double_buffer = FALSE;
#endif
    //layer->layer_struct.layer_info.layer_data.immutable = ( layer->format & UILAYER_MEM_FORMAT_IMMUTABLE ) ? TRUE : FALSE;
#if !defined RELEASE_INFO  
    layer->file_name_ptr = file_name_ptr;
    layer->file_line     = file_line;
#endif
        
    if (PNULL != dev_info_ptr)
    {
        layer->lcd_dev_info = *dev_info_ptr;

// #ifndef UI_LAYER_SUPPORT_LOW_MEMORY
//         //for dither
//         if ( GUI_BLOCK_MAIN == dev_info_ptr->block_id
//             && PNULL == layer->raw2_layer_buf_ptr
//             && DATA_TYPE_RGB565 == layer->type )
//         {
//             layer->raw2_layer_buf_ptr = AllocLayerBuf(layer->layer_buf_size * 2);
//         }
// #endif
    }
    return TRUE;
}

/*****************************************************************************/
//     Description : �Ƚ�lcd dev
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CompareLcdDev(
                            GUI_LCD_DEV_INFO const *dev_info_ptr1,
                            GUI_LCD_DEV_INFO const *dev_info_ptr2
                            )
{
    if ((PNULL == dev_info_ptr1) || (PNULL == dev_info_ptr2))
    {
        return FALSE;
    }
    if (dev_info_ptr1->block_id == dev_info_ptr2->block_id)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//     Description : ��s_layer_blt��ȥ��main layer handle
//    Global resource dependence : 
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_RemoveMainLayer(void)
{
    GUI_LCD_DEV_INFO main_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    if (!CheckLayerId(&main_dev_info))
    {
        return;
    }
    
    if (IsMultiLayerEnable())
    {
        uint32 i = 0;

        for (i = 0; i < UILAYER_TOTAL_BLT_COUNT; i++)
        {
            if (CompareLcdDev(&main_dev_info, &s_layer_blt[i].lcd_dev_info))
            {
                break;
            }
        }
        
        if (i < UILAYER_TOTAL_BLT_COUNT)
        {
            SCI_TRACE_LOW("UILAYER_RemoveMainLayer()");

            //deactive mainlayer
//            DeactiveMainLayer();
            //free raw2
            FreeMainLayerBuffer2();

            //����
            StoreLayerUpdateRect(&main_dev_info);
            
            for (; i < (UILAYER_TOTAL_BLT_COUNT-1); i++)
            {
                s_layer_blt[i] = s_layer_blt[i+1];
            }
            s_layer_blt[UILAYER_TOTAL_BLT_COUNT-1].lcd_dev_info.block_id = UILAYER_NULL_HANDLE;
            s_layer_blt[UILAYER_TOTAL_BLT_COUNT-1].is_valid = FALSE;
        }
    }
}

/*****************************************************************************/
//     Description : restore main layer.
//    Global resource dependence : 
//  Author:   
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_RestoreMainLayer(void)
{
    uint32 i = 0;
    uint32 j = 0;
    UILAYER_BLT_T    blt_layer    = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    blt_layer.is_valid     = TRUE;
    blt_layer.layer_level  = UILAYER_LEVEL_NORMAL;
    blt_layer.lcd_dev_info = lcd_dev_info;
    

    if (!CheckLayerId(&(lcd_dev_info)))
    {
        return;
    } 
    
    if (IsMultiLayerEnable())
    {       
        if (FindBltLayer(&lcd_dev_info))
        {
            return;
        }
        
        for (i = 0; i < UILAYER_TOTAL_BLT_COUNT; i++)
        {
            if (!s_layer_blt[i].is_valid || (s_layer_blt[i].layer_level >= UILAYER_LEVEL_NORMAL))
            {
                break;
            }
        }

        //insert main layer to the first layer in normal level.
        if (i < UILAYER_TOTAL_BLT_COUNT)
        {           
            SCI_TRACE_LOW("UILAYER_RestoreMainLayer()");
            for (j = (UILAYER_TOTAL_BLT_COUNT-1); j > i; j--)
            {
                s_layer_blt[j] = s_layer_blt[j-1];
            }
            s_layer_blt[i] = blt_layer;

            //����
            StoreLayerUpdateRect(&lcd_dev_info);
            //active mainlayer
//            ActiveMainLayer();
            UILAYER_CopyLcdToMainLayer();
        }
    } 
}

/*****************************************************************************/
//     Description : ��s_layer_blt��ȥ��layer_handle
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_RemoveBltLayer(
                                   GUI_LCD_DEV_INFO const *dev_info_ptr
                                   )
{
    if (PNULL == dev_info_ptr)
    {
        return;
    }
//    if (UILAYER_IsMainLayerHandle(dev_info_ptr))
//    if (UILAYER_HANDLE_MAIN == UILAYER_GetHandleType(dev_info_ptr))
    if (IsMainLayerHandle(dev_info_ptr))
    {
        //��ֹ���㱻��ɾ��
        return;
    }
    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }
    if (IsMultiLayerEnable())
    {
        uint32 i = 0;

        for (i = 0; i < UILAYER_TOTAL_BLT_COUNT; i++)
        {
            if (CompareLcdDev(dev_info_ptr, &s_layer_blt[i].lcd_dev_info))
            {
                break;
            }
        }
        if (i < UILAYER_TOTAL_BLT_COUNT)
        {
            //����
            StoreLayerUpdateRect(dev_info_ptr);
            for (; i < (UILAYER_TOTAL_BLT_COUNT-1); i++)
            {
                s_layer_blt[i] = s_layer_blt[i+1];
            }
            s_layer_blt[UILAYER_TOTAL_BLT_COUNT-1].lcd_dev_info.block_id = UILAYER_NULL_HANDLE;
            s_layer_blt[UILAYER_TOTAL_BLT_COUNT-1].is_valid = FALSE;
        }
    }
}


/*****************************************************************************/
//     Description : ��blt�в���layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN FindBltLayer(
                           GUI_LCD_DEV_INFO const *dev_info_ptr
                           )
{
    uint32 i = 0;

    for (i = 0; i < UILAYER_TOTAL_BLT_COUNT; i++)
    {
        if (CompareLcdDev(&s_layer_blt[i].lcd_dev_info, dev_info_ptr))
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************/
//     Description : ����һ����
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_AppendBltLayer(
                                   UILAYER_APPEND_BLT_T *append_layer_ptr
                                   )
{
    uint32 i = 0;
    uint32 j = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    UILAYER_LEVEL_E level = UILAYER_LEVEL_LOW;

    if (PNULL == append_layer_ptr)
    {
        return;
    }
    if (append_layer_ptr->layer_level >= UILAYER_LEVEL_MAX)
    {
        //SCI_TRACE_LOW:"UILAYER_AppendBltLayer() level error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1335_112_2_18_3_22_52_46,(uint8*)"");
        return;
    }
    if (s_layer_blt[UILAYER_TOTAL_BLT_COUNT-1].is_valid)
    {
        //SCI_TRACE_LOW:"UILAYER_AppendBltLayer() max blt!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1340_112_2_18_3_22_52_47,(uint8*)"");
        //�������ֵ
        return;
    }
    if (UILAYER_HANDLE_MAIN == UILAYER_GetHandleType(&append_layer_ptr->lcd_dev_info))
    {
        //��ֹ���㱻���
        return;
    }    
    if (!CheckLayerId(&append_layer_ptr->lcd_dev_info))
    {
        return;
    }
	    
	if (IsMultiLayerEnable())
    {
        lcd_dev_info = append_layer_ptr->lcd_dev_info;
/*        if (!UILAYER_IsLayerActive(&lcd_dev_info))
        {
            //δactive��ֱ�ӷ���
            return;
        }
        */
        if (FindBltLayer(&lcd_dev_info))
        {
            //���У�ֱ�ӷ���
            return;
        }
        level = append_layer_ptr->layer_level;
        for (i = 0; i < UILAYER_TOTAL_BLT_COUNT; i++)
        {
            if (!s_layer_blt[i].is_valid
                || (s_layer_blt[i].layer_level > level))
            {
                break;
            }
        }
        if (i < UILAYER_TOTAL_BLT_COUNT)
        {
            //��iλ�ò���
            for (j = (UILAYER_TOTAL_BLT_COUNT-1); j > i; j--)
            {
                s_layer_blt[j] = s_layer_blt[j-1];
            }
            s_layer_blt[i].is_valid = TRUE;
            s_layer_blt[i].lcd_dev_info = append_layer_ptr->lcd_dev_info;
            s_layer_blt[i].layer_level = append_layer_ptr->layer_level;
            //����
            StoreLayerUpdateRect(&lcd_dev_info);
        }
    }
}

/*****************************************************************************/
//     Description : store layer update rect
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void StoreLayerUpdateRect(
                                GUI_LCD_DEV_INFO const *dev_info_ptr
                                )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    GUI_RECT_T full_layer_rect = {0};

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }    
    if (PNULL != layer)
    {
        full_layer_rect = GetLayerRect(layer);
        ConvertLCD2LayerRect(layer, &full_layer_rect, &full_layer_rect);
        MMITHEME_StoreUpdateRect(dev_info_ptr, full_layer_rect);    
    }
    else
    {
        //SCI_TRACE_LOW:"StoreLayerUpdateRect() PNULL == layer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1415_112_2_18_3_22_52_48,(uint8*)"");
    }
}

/*****************************************************************************/
//     Description : store layer update rect
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_StoreLayerUpdateRect(
                                GUI_LCD_DEV_INFO const *dev_info_ptr
                                )
{
    StoreLayerUpdateRect(dev_info_ptr);
}

/*****************************************************************************/
//     Description : ����8��������
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetAlign8Rect(
                               GUI_RECT_T rect
                               )
{
    GUI_RECT_T align8_rect = {0};
// #ifdef UILAYER_NEED_ALIGN8
//     align8_rect.left = ALIGN_8BYTES_POS_LEFT(rect.left);
//     align8_rect.top = rect.top;
//     align8_rect.right = ALIGN_8BYTES_POS_RIGHT(rect.right);
//     align8_rect.bottom = rect.bottom;
// #else
    align8_rect = rect;
//#endif
    return align8_rect;
}

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
	)
{    
    if (PNULL != rect_ptr)
    {
        GUILCD_MultiBlendRect(lcd_id, *rect_ptr, 0);
    }
    /*
    DISPLAY_RECT_T blt_rect = {0};
    GUI_RECT_T align_rect = {0};
    UILAYER_STRUCT_T *layer = PNULL;
    DISPLAY_BLEND_LAYER_T *layer_stuct_arr[UILAYER_TOTAL_BLT_COUNT] = {0};
    DISPLAY_BLEND_LAYER_T temp_layer_stuct_arr[UILAYER_TOTAL_BLT_COUNT] = {0};
    DISPLAY_BLEND_LAYER_T *dst_layer_handle_ptr = PNULL;
    uint32 i = 0;
    uint32 index = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (PNULL == rect_ptr)
    {
        //MMI_ASSERT_WARNING("UILAYER_InvalidateRect() PNULL == rect_ptr");
        //SCI_TRACE_LOW:"UILAYER_InvalidateRect() PNULL == rect_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1478_112_2_18_3_22_52_49,(uint8*)"");
        return;//lint !e527
    }
//    SCI_Sleep(500);
    switch (lcd_id)
    {
    case LCD_ID_0:
        align_rect = GetAlign8Rect(*rect_ptr);
        blt_rect = Convert2DispRect(align_rect);



        for (i = 0; i < UILAYER_TOTAL_BLT_COUNT; i++)
        {
            lcd_dev_info.lcd_id = lcd_id;
            lcd_dev_info.block_id = s_layer_blt[i].lcd_dev_info.block_id;
            layer = GetLayerPtr(&lcd_dev_info);
            //�ײ�Ҫ��layer_buf_ptr����Ϊ��
            if ((PNULL != layer) && (PNULL != layer->buf_addr))
            {
//                layer_stuct_arr[index++] = &(layer->layer_struct);
                CopyDisplayBlendLayer(&temp_layer_stuct_arr[index], layer);
                layer_stuct_arr[index] = &temp_layer_stuct_arr[index];
                index++;
            }
        }

        //ֻˢ��lcd_buf
        dst_layer_handle_ptr = GetMainLcdhandle();
        InvalidateRectUsingLayer(
            lcd_id,
            dst_layer_handle_ptr,
            layer_stuct_arr,
            index,
            &blt_rect
            );
        break;

    case LCD_ID_1:
        //do nothing
        break;

    default:
        //SCI_TRACE_LOW:"UILAYER_InvalidateRect() lcd_id = %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1521_112_2_18_3_22_52_50,(uint8*)"d", lcd_id);
        break;
    }
*/
}

/*****************************************************************************/
//     Description : �õ�layer��ȫ������
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetLayerRect(
                              UILAYER_STRUCT_T   *layer_ptr
                              )
{
    GUI_RECT_T rect = {0};

    if (PNULL == layer_ptr)
    {
        //MMI_ASSERT_WARNING("GetLayerRect() PNULL == layer_ptr");
        //SCI_TRACE_LOW:"GetLayerRect() PNULL == layer_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1540_112_2_18_3_22_52_51,(uint8*)"");
    }
    else
    {
//        rect = GetLayerBlendRect(&(layer_ptr->layer_struct));
        rect.left = layer_ptr->x;
        rect.top = layer_ptr->y;
        rect.right = rect.left + layer_ptr->width - 1;
        rect.bottom = rect.top + layer_ptr->height - 1;
    }
    return rect;
}

/*****************************************************************************/
//     Description : �õ�layer��ȫ������
//    Global resource dependence : 
//  Author:   hongbo.lan
//    Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T UILAYER_GetLayerRect(GUI_LCD_DEV_INFO *dev_info_ptr)
{
    GUI_RECT_T rect = {0};
    UILAYER_STRUCT_T *layer_ptr = GetLayerPtr(dev_info_ptr);

    rect = GetLayerRect(layer_ptr);

    return rect;
}

/*****************************************************************************/
//     Description : �õ�layer��ȫ������
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetLayerBlendRect(
                              DISPLAY_BLEND_LAYER_T  *layer_blend_ptr
                              )
{
    GUI_RECT_T rect = {0};
    BOOLEAN result = FALSE;

    if (PNULL == layer_blend_ptr)
    {
        //MMI_ASSERT_WARNING("GetLayerBlendRect() PNULL == layer_blend_ptr");
        //SCI_TRACE_LOW:"GetLayerBlendRect() PNULL == layer_blend_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1581_112_2_18_3_22_52_52,(uint8*)"");
    }
    else
    {
        rect.left = layer_blend_ptr->layer_info.pos.x;
        rect.top = layer_blend_ptr->layer_info.pos.y;
        rect.right = rect.left + layer_blend_ptr->layer_info.width - 1;
        rect.bottom = rect.top + layer_blend_ptr->layer_info.height - 1;
        result = TRUE;
    }
    return rect;
}

/*****************************************************************************/
//     Description : �������layer������λ��ת��Ϊ�����lcd������λ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN ConvertLayer2LCDRect(
                                 UILAYER_STRUCT_T   *layer_ptr,
                                 const GUI_RECT_T   *layer_rect_ptr,
                                 GUI_RECT_T         *lcd_rect_ptr      //out
                                 )
{
    BOOLEAN result = FALSE;

    if ((PNULL == layer_ptr) || (PNULL == lcd_rect_ptr) || (PNULL == layer_rect_ptr))
    {
        //MMI_ASSERT_WARNING("ConvertLayer2LCDRect() (PNULL == layer_ptr) || (PNULL == lcd_rect_ptr) || (PNULL == layer_rect_ptr)");
        //SCI_TRACE_LOW:"ConvertLayer2LCDRect() (PNULL == layer_ptr) || (PNULL == lcd_rect_ptr) || (PNULL == layer_rect_ptr)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1609_112_2_18_3_22_52_53,(uint8*)"");
    }
    else
    {
// #ifdef UILAYER_NEED_ALIGN8
//         lcd_rect_ptr->left = layer_rect_ptr->left + layer_ptr->x+layer_ptr->interval_left;
//         lcd_rect_ptr->right = layer_rect_ptr->right + layer_ptr->x+layer_ptr->interval_left;
// #else
        lcd_rect_ptr->left = layer_rect_ptr->left;
        lcd_rect_ptr->right = layer_rect_ptr->right;
//#endif
        lcd_rect_ptr->top = layer_rect_ptr->top + layer_ptr->y;
        lcd_rect_ptr->bottom = layer_rect_ptr->bottom + layer_ptr->y;    
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
//     Description : �������lcd������λ��ת��Ϊ�����layer������λ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN ConvertLCD2LayerRect(
                                 UILAYER_STRUCT_T   *layer_ptr,
                                 const GUI_RECT_T   *lcd_rect_ptr,
                                 GUI_RECT_T         *layer_rect_ptr    //out
                                 )
{
    BOOLEAN result = FALSE;

    if ((PNULL == layer_ptr) || (PNULL == layer_rect_ptr) || (PNULL == lcd_rect_ptr))
    {
        //MMI_ASSERT_WARNING("ConvertLCD2LayerRect() (PNULL == layer_ptr) || (PNULL == layer_rect_ptr) || (PNULL == lcd_rect_ptr)");
        //SCI_TRACE_LOW:"ConvertLCD2LayerRect() (PNULL == layer_ptr) || (PNULL == layer_rect_ptr) || (PNULL == lcd_rect_ptr)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1642_112_2_18_3_22_52_54,(uint8*)"");
    }
    else
    {
        layer_rect_ptr->left = lcd_rect_ptr->left - layer_ptr->x;
        layer_rect_ptr->top = lcd_rect_ptr->top - layer_ptr->y;
        layer_rect_ptr->right = lcd_rect_ptr->right - layer_ptr->x;
        layer_rect_ptr->bottom = lcd_rect_ptr->bottom - layer_ptr->y;    
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
//     Description : ��Ŀ�����²������ˢ����Ŀ�����ȥ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_BltLowerLayer(
                                  GUI_LCD_DEV_INFO const *dev_info_ptr,
                                  const GUI_RECT_T *rect_ptr
                                  )
{
    UILAYER_STRUCT_T *dst_layer_ptr = PNULL;
    UILAYER_STRUCT_T *layer = PNULL;
    DISPLAY_RECT_T blt_rect = {0};
    GUI_RECT_T lcd_rect = {0};
    GUI_RECT_T align8_rect = {0};
    GUI_RECT_T full_layer_rect = {0};
    GUI_RECT_T full_lcd_rect = {0};    
    GUI_RECT_T layer_rect = {0};
    DISPLAY_BLEND_LAYER_T *layer_stuct_arr[UILAYER_TOTAL_BLT_COUNT] = {0};
    DISPLAY_BLEND_LAYER_T temp_layer_stuct_arr[UILAYER_TOTAL_BLT_COUNT] = {0};
    DISPLAY_BLEND_LAYER_T dst_layer_struct = {0};
    uint32 i = 0;
    uint32 index = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    uint32 blt_num = 0;
// #ifdef UILAYER_NEED_ALIGN8      
//     void *save_data_ptr = PNULL;
// #endif
       
    if (PNULL == rect_ptr)
    {
        //MMI_ASSERT_WARNING("UILAYER_BltLowerLayer() PNULL == rect_ptr");
        //SCI_TRACE_LOW:"UILAYER_BltLowerLayer() PNULL == rect_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1686_112_2_18_3_22_53_55,(uint8*)"");
        return;/*lint !e527*/
    }
    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }   
    dst_layer_ptr = GetLayerPtrEx(dev_info_ptr, TRUE);

    if (PNULL == dst_layer_ptr)
    {
        return;
    }
    
    //rect�ü�
    lcd_rect = *rect_ptr;
    full_layer_rect = GetLayerRect(dst_layer_ptr);
    if (!GUI_IntersectRect(&lcd_rect, lcd_rect, full_layer_rect))
    {
        return;
    }
    full_lcd_rect = GetLCDRect(dev_info_ptr->lcd_id);
    if (!GUI_IntersectRect(&lcd_rect, lcd_rect, full_lcd_rect))
    {
        return;
    }

    lcd_dev_info.lcd_id = dev_info_ptr->lcd_id;
    for (i = 0; i < UILAYER_TOTAL_BLT_COUNT; i++)
    {
        lcd_dev_info.block_id = s_layer_blt[i].lcd_dev_info.block_id;
        layer = GetLayerPtr(&lcd_dev_info);
        if (dst_layer_ptr == layer)
        {
            //��dst_layer_ptr֮ǰ�������ˢ����dst_layer_ptr����ȥ
            break;
        }
        else if ((PNULL != layer) && (PNULL != layer->buf_addr))
        {            
            CopyDisplayBlendLayer(&temp_layer_stuct_arr[index], layer);
            layer_stuct_arr[index] = &temp_layer_stuct_arr[index];
//            layer_stuct_arr[index++] = &(layer->layer_struct);
            index++;
            blt_num++;
        }
    }
    if (blt_num > 0)
    {
        ConvertLCD2LayerRect(dst_layer_ptr, &lcd_rect, &layer_rect);
// #ifdef UILAYER_NEED_ALIGN8        
//         Save8AlignData(dst_layer_ptr, &layer_rect, &save_data_ptr);
// #endif
        align8_rect = GetAlign8Rect(lcd_rect);
        blt_rect = Convert2DispRect(align8_rect);
        CopyDisplayBlendLayer(&dst_layer_struct, dst_layer_ptr);
        InvalidateRectUsingLayer(
            0,
            &dst_layer_struct,
            layer_stuct_arr,
            blt_num,
            &blt_rect
            );
// #ifdef UILAYER_NEED_ALIGN8
//         Load8AlignData(dst_layer_ptr, &layer_rect, &save_data_ptr);
// #endif
    }
}

/*****************************************************************************/
//     Description : set mainlayer type
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetMainLayerType(
                                     DISPLAY_DATA_TYPE_E data_type
                                     )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    UILAYER_STRUCT_T *layer = GetLayerPtr(&lcd_dev_info);

    //SCI_TRACE_LOW:"UILAYER_SetMainLayerType() data_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1759_112_2_18_3_22_53_56,(uint8*)"d", data_type);

    if (MMITHEME_IsMainLayerTypeChangable())
    {
        if (PNULL != layer)
        {
            if ((DATA_TYPE_ARGB888 == data_type) || (DATA_TYPE_RGB565 == data_type))
            {
                layer->type = data_type;
            }
        }
    }
}

/*****************************************************************************/
//     Description : ��ʼ��layerϵͳ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_InitSystem(void)
{   
    uint16      lcd_width = 0;
    uint16      lcd_height = 0;
    DISPLAY_BLEND_LAYER_T blend_layer = {0};
    uint32 i = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    uint8 *buf_ptr = PNULL;
    uint32 mainlcd_buf_size = 0;

    //mutex
    if(PNULL == s_uilayer_mutex)
    {
        s_uilayer_mutex=SCI_CreateMutex("uilayer_mutex",SCI_NO_INHERIT);
        if(PNULL == s_uilayer_mutex)
        {
            SCI_TRACE_LOW("UILAYER_InitSystem() PNULL == s_uilayer_mutex");
        }
    }

    for (i = 0; i < UILAYER_TOTAL_BLT_COUNT; i++)
    {
        s_layer_blt[i].is_valid = FALSE;
        s_layer_blt[i].lcd_dev_info.block_id = UILAYER_NULL_HANDLE;
    }

    for (i = 0; i < UILAYER_TOTAL_LAYER_COUNT; i++)
    {
        s_layer_arr[i].is_enable = FALSE;
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    SCI_MEMSET(&blend_layer, 0x00, sizeof(DISPLAY_BLEND_LAYER_T));
    SetBlendDefaultData(&blend_layer);

    blend_layer.layer_info.layer_data.type = DATA_TYPE_RGB565;
    blend_layer.layer_info.layer_data.alpha_sel = DISPLAY_BLOCK_ALPHA;    
    blend_layer.layer_info.layer_data.colorkey_en = FALSE;
    //��̬�����ڴ�
    mainlcd_buf_size = (lcd_width * lcd_height * sizeof(GUI_COLOR_T));
//#ifdef UI_MULTILAYER_SUPPORT
    buf_ptr = AllocLayerBuf(PNULL, mainlcd_buf_size, _D_FILE_NAME, _D_FILE_LINE);
//#else
//    buf_ptr = PNULL;
//#endif

    blend_layer.layer_info.width = lcd_width;
    blend_layer.layer_info.height = lcd_height;
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;//UILAYER_MAIN_HANDLE;
    
    s_layer_arr[0].is_enable = TRUE;    

    CreateHandle(
        VIRTUAL_WIN_ID,
        &lcd_dev_info, 
        &blend_layer,
        buf_ptr,//(uint8*)mainlcd_buf_ptr,
        mainlcd_buf_size,
        FALSE,   //���Զ��ͷ��ڴ�
        UILAYER_MEM_DOUBLE_COPY,
        PNULL,
        _D_FILE_NAME,
        _D_FILE_LINE
        );
    
    UILAYER_RestoreMainLayer();
    InitMainLcdHandle();

/* rtos do not support BLKMEM API, so invalid this part*/
	if ( PNULL == s_layer_doublebuffer_flag_ptr)
	{		
		BLKMEM_CONFIG_T config = {0};
		
		//ע��˫buffer��������
		config.func_callback = (blkmem_callback)UILAYER_FreeAllLayerDoubleBuffer;
		
		BLKMEM_Register( (uint32)UILAYER_FreeAllLayerDoubleBuffer, &config );
		
		s_layer_doublebuffer_flag_ptr = BLKMEM_Malloc( (uint32)UILAYER_FreeAllLayerDoubleBuffer, sizeof(uint32));
	}	
}

//PUBLIC void UILAYER_InitSystem(
//                               GUI_COLOR_T *mainlcd_buf_ptr,
//                               uint32 mainlcd_buf_size,
//                               DISPLAY_DATA_TYPE_E data_type
//                               )
//{   
//    uint16      lcd_width = 0;
//    uint16      lcd_height = 0;
//    DISPLAY_BLEND_LAYER_T blend_layer = {0};
//    uint32 i = 0;
//    GUI_LCD_DEV_INFO lcd_dev_info = {0};
//    uint8 *buf_ptr = PNULL;
//
////    if (PNULL == mainlcd_buf_ptr)
////    {
////        //MMI_ASSERT_WARNING("UILAYER_InitSystem() PNULL == mainlcd_buf_ptr");
////        //SCI_TRACE_LOW:"UILAYER_InitSystem() PNULL == mainlcd_buf_ptr"
////        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1792_112_2_18_3_22_53_57,(uint8*)"");
////        return;   /*lint !e527*/
////    }
//    //mutex
//    if(PNULL == s_uilayer_mutex)
//    {
//        s_uilayer_mutex=SCI_CreateMutex("uilayer_mutex",SCI_NO_INHERIT);
//        if(PNULL == s_uilayer_mutex)
//        {
//            SCI_TRACE_LOW("UILAYER_InitSystem() PNULL == s_uilayer_mutex");
//        }
//    }
//
//    for (i = 0; i < UILAYER_TOTAL_BLT_COUNT; i++)
//    {
//        s_layer_blt[i].is_valid = FALSE;
//        s_layer_blt[i].lcd_dev_info.block_id = UILAYER_NULL_HANDLE;
//    }
//
//    for (i = 0; i < UILAYER_TOTAL_LAYER_COUNT; i++)
//    {
////        s_layer_arr[i].layer_struct.layer_info.is_enable = FALSE;
//        s_layer_arr[i].is_enable = FALSE;
////        s_layer_arr[i].index = i;
//    }
//
//    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
//    
//    SCI_MEMSET(&blend_layer, 0x00, sizeof(DISPLAY_BLEND_LAYER_T));
//    SetBlendDefaultData(&blend_layer);
//
//    blend_layer.layer_info.layer_data.type = data_type;//DATA_TYPE_RGB565;
//    if (DATA_TYPE_ARGB888 == data_type)
//    {
//        blend_layer.layer_info.layer_data.alpha_sel = DISPLAY_PIXEL_ALPHA;    
//        blend_layer.layer_info.layer_data.colorkey_en = FALSE;
//    }
//    else
//    {
//        blend_layer.layer_info.layer_data.alpha_sel = DISPLAY_BLOCK_ALPHA;    
//        blend_layer.layer_info.layer_data.colorkey_en = FALSE;
//    }
//    if (PNULL == mainlcd_buf_ptr)
//    {
//        //��̬�����ڴ�
//        buf_ptr = AllocLayerBuf(PNULL, mainlcd_buf_size, _D_FILE_NAME, _D_FILE_LINE);
//    }
//    else
//    {
//        buf_ptr = mainlcd_buf_ptr;
//    }
//
//    blend_layer.layer_info.width = lcd_width;
//    blend_layer.layer_info.height = lcd_height;
//    
//    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
//    lcd_dev_info.block_id = GUI_BLOCK_MAIN;//UILAYER_MAIN_HANDLE;
//    
//    s_layer_arr[0].is_enable = TRUE;
//    
//
//    CreateHandle(
//        VIRTUAL_WIN_ID,
//        &lcd_dev_info, 
//        &blend_layer,
//        buf_ptr,//(uint8*)mainlcd_buf_ptr,
//        mainlcd_buf_size,
//        FALSE,   //���Զ��ͷ��ڴ�
//        UILAYER_MEM_DOUBLE_COPY,
//        PNULL,
//        _D_FILE_NAME,
//        _D_FILE_LINE
//        );
//    
//    UILAYER_RestoreMainLayer();
//    InitMainLcdHandle();
//}

/*****************************************************************************/
//     Description : ����blend�ṹĬ������
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void SetBlendDefaultData(
                               DISPLAY_BLEND_LAYER_T *blend_info_ptr
                               )
{
    if (PNULL == blend_info_ptr)
    {
        return;
    }
    blend_info_ptr->layer_buf_size = 0;//lcd_width*lcd_height*sizeof(GUI_COLOR_T);
    blend_info_ptr->alpha_buf_size = 0;
// #ifdef UILAYER_NEED_ALIGN8    
//     blend_info_ptr->interval_left = 0;
//     blend_info_ptr->interval_right = 0;
// #endif

    blend_info_ptr->layer_info.alpha_buf_addr = PNULL;
    blend_info_ptr->layer_info.buf_addr = PNULL;//(uint32)lcd_buf_ptr;
    blend_info_ptr->layer_info.pos.x = 0;
    blend_info_ptr->layer_info.pos.y = 0;
    blend_info_ptr->layer_info.width = 0;//lcd_width;
    blend_info_ptr->layer_info.height = 0;//lcd_height;
    blend_info_ptr->layer_info.is_enable = TRUE;
    blend_info_ptr->layer_info.layer_id = 0;
    blend_info_ptr->layer_info.uv_buf_addr = PNULL;
    //layer_data
//    blend_info_ptr->layer_info.layer_data.alpha_sel = DISPLAY_BLOCK_ALPHA;
    blend_info_ptr->layer_info.layer_data.type = DATA_TYPE_ARGB888;//DATA_TYPE_RGB565;
    blend_info_ptr->layer_info.layer_data.alpha_sel = DISPLAY_PIXEL_ALPHA;    
    blend_info_ptr->layer_info.layer_data.alpha = MAX_ALPHA_VALUE;   //Ĭ��Ϊ��͸
    blend_info_ptr->layer_info.layer_data.colorkey_en = FALSE;
    blend_info_ptr->layer_info.layer_data.grey_value = 0;
}

/*****************************************************************************/
//     Description : ��ʼ��s_mainlcd_handle�ṹ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void InitMainLcdHandle(void)
{
    GUI_COLOR_T *lcd_buf_ptr = PNULL;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    lcd_buf_ptr = GUILCD_GetMainLcdBufPtr();

    //SCI_ASSERT(PNULL != lcd_buf_ptr);

    if (PNULL == lcd_buf_ptr)
    {
        return;
    }
    
    SCI_MEMSET(&s_mainlcd_handle, 0x00, sizeof(DISPLAY_BLEND_LAYER_T));
    SetBlendDefaultData(&s_mainlcd_handle);
    s_mainlcd_handle.layer_info.layer_data.type = DATA_TYPE_RGB565;
    s_mainlcd_handle.layer_info.layer_data.alpha_sel = DISPLAY_BLOCK_ALPHA;
    s_mainlcd_handle.layer_buf_size = lcd_width*lcd_height*sizeof(GUI_COLOR_T);
    s_mainlcd_handle.layer_info.buf_addr = (uint32)lcd_buf_ptr;
    s_mainlcd_handle.layer_info.width = lcd_width;
    s_mainlcd_handle.layer_info.height = lcd_height;
}

/*****************************************************************************/
//     Description : ����ڴ����ַ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC uint32 *UILAYER_GetMemCheckAddr(
                                       GUI_LCD_DEV_INFO const *dev_info_ptr
                                       )
{
    UILAYER_STRUCT_T *layer = PNULL;
    uint32 mem_addr = 0;

    if (!CheckLayerId(dev_info_ptr))
    {
        return PNULL;
    }
  
//    if (UILAYER_IsMultiLayerHandle(dev_info_ptr))
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(dev_info_ptr))
    {
        layer = GetLayerPtr(dev_info_ptr);
        if (PNULL != layer)
        {
            if (PNULL != layer->buf_addr)
            {
                mem_addr = (uint32)layer->buf_addr;
                mem_addr += layer->layer_buf_size;
            }    
            return (uint32*)mem_addr;
        }
        else
        {
            //SCI_TRACE_LOW:"UILAYER_GetMemCheckAddr() PNULL == layer"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1944_112_2_18_3_22_53_58,(uint8*)"");
            return PNULL;
        }
    }
    return PNULL;
}

/*****************************************************************************/
//     Description : ���layer��bufferָ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC uint8 *UILAYER_GetLayerBufferPtr(
                                   GUI_LCD_DEV_INFO const *dev_info_ptr
                                   )
{
    UILAYER_STRUCT_T *layer = PNULL;
    uint32 mem_addr = 0;

    if (!CheckLayerId(dev_info_ptr))
    {
        return PNULL;
    }
  
    layer = GetLayerPtrEx(dev_info_ptr, TRUE);
    if (PNULL != layer)
    {
        if (PNULL != layer->buf_addr)
        {
            mem_addr = (uint32)layer->buf_addr;
// #ifdef UILAYER_NEED_ALIGN8
//             mem_addr += ((uint16)layer->interval_left * GetLayerBytesPerPixel(layer));
// #endif
        }    
        return (uint8*)mem_addr;
    }
    else
    {
        //SCI_TRACE_LOW:"UILAYER_GetLayerBufferPtr() PNULL == layer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_1981_112_2_18_3_22_53_59,(uint8*)"");
        return PNULL;
    }
}

/*****************************************************************************/
//     Description : ���layer����Ϣ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_GetLayerInfo(
                                 GUI_LCD_DEV_INFO const *dev_info_ptr,
                                 UILAYER_INFO_T *layer_info_ptr     //output
                                 )
{
    UILAYER_STRUCT_T *layer = GetLayerPtrEx(dev_info_ptr, TRUE);
    
    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }
    GetLayerInfo(layer, layer_info_ptr);
}

/*****************************************************************************/
//     Description : ���layer����Ϣ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void GetLayerInfo(
                        UILAYER_STRUCT_T *layer,
                        UILAYER_INFO_T *layer_info_ptr     //output
                        )
{
//    uint32 mem_addr = 0;
//    uint32 bytes_per_pixel = 0;
   
    if (PNULL == layer_info_ptr)
    {
        return;
    }
    if (PNULL != layer)
    {
        layer_info_ptr->mem_width = (uint16)layer->width;
        
        //SCI_ASSERT(layer->layer_struct.layer_info.width >= (layer->layer_struct.interval_left+layer->layer_struct.interval_right));

// #ifdef UILAYER_NEED_ALIGN8
//         if (layer->width < (layer->interval_left+layer->interval_right))
//         {
//             return;
//         }
//         layer_info_ptr->layer_width = (uint16)layer->width-layer->interval_left-layer->interval_right;
// #else
        layer_info_ptr->layer_width = (uint16)layer->width;
//#endif        
        layer_info_ptr->layer_height = (uint16)layer->height;
//        if (PNULL != layer->layer_struct.layer_info.buf_addr)
//        {
//            mem_addr = (uint32)layer->layer_struct.layer_info.buf_addr;
//            bytes_per_pixel = GetLayerBytesPerPixel(layer);
//            mem_addr += ((uint16)layer->layer_struct.interval_left * bytes_per_pixel);
//            layer_info_ptr->layer_buf_ptr = (uint8*)mem_addr;
//        }
//        else
//        {
//            layer_info_ptr->layer_buf_ptr = PNULL;
//        }
        layer_info_ptr->data_type = layer->type;
    }
    else
    {
//        layer_info_ptr->layer_buf_ptr = PNULL;
    }
}

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
                                )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);

    if (PNULL == dev_info_ptr)
    {
        //MMI_ASSERT_WARNING("UILAYER_GetLayerPosition() UILAYER_GetLayerInfo() PNULL == dev_info_ptr");
        //SCI_TRACE_LOW:"UILAYER_GetLayerPosition() UILAYER_GetLayerInfo() PNULL == dev_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2070_112_2_18_3_22_53_60,(uint8*)"");
        return;/*lint !e527*/
    }
    if ((PNULL == offset_x_ptr) || (PNULL == offset_y_ptr))
    {
        //MMI_ASSERT_WARNING("UILAYER_GetLayerInfo() (PNULL == offset_x_ptr) || (PNULL == offset_y_ptr)");
        //SCI_TRACE_LOW:"UILAYER_GetLayerInfo() (PNULL == offset_x_ptr) || (PNULL == offset_y_ptr)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2076_112_2_18_3_22_53_61,(uint8*)"");
        return;/*lint !e527*/
    }        
    if (PNULL != layer)
    {
        //����ʵ�ʵ������buffer�Ŀ�ȣ�bufferʵ����ʾ����ʼ��ַ
// #ifdef UILAYER_NEED_ALIGN8
//         *offset_x_ptr = (layer->x + layer->interval_left);
// #else
        *offset_x_ptr = layer->x;
//#endif
        *offset_y_ptr = layer->y;
    }
    else
    {
        //SCI_TRACE_LOW:"UILAYER_GetLayerInfo() PNULL == layer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2091_112_2_18_3_22_53_62,(uint8*)"");
    }
}

/*****************************************************************************/
//     Description : ����layer��λ�ã������lcd���Ͻǵ����꣩
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN SetLayerPosition(
                                UILAYER_STRUCT_T *layer,
                                int16 offset_x,
                                int16 offset_y
                                )
{
//    DISPLAY_BLEND_LAYER_T *blend_ptr = PNULL;
// #ifdef UILAYER_NEED_ALIGN8
//     int16 old_interval_left = 0;
// #endif
    
    /*
    blend_ptr = &layer->layer_struct;
    old_interval_left = blend_ptr->interval_left;
    blend_ptr->layer_info.pos.x = ALIGN_8BYTES_POS_LEFT(offset_x);
    blend_ptr->interval_left = (offset_x - blend_ptr->layer_info.pos.x);
    blend_ptr->interval_right += (old_interval_left - blend_ptr->interval_left);
    blend_ptr->layer_info.pos.y = offset_y;
    */
// #ifdef UILAYER_NEED_ALIGN8
//     old_interval_left = layer->interval_left;
//     layer->x = ALIGN_8BYTES_POS_LEFT(offset_x);
//     layer->interval_left = (offset_x - layer->x);
//     layer->interval_right += (old_interval_left - layer->interval_left);
//     layer->y = offset_y;
// #else
    layer->x = offset_x;
    layer->y = offset_y;
//#endif

    return TRUE;
}

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
                                )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);

    if (IsMainLayerHandle(dev_info_ptr))
    {
        //��ֹ���㱻�����
        return;
    }
    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }    
    if ( PNULL != layer )
    {
        if ( offset_x != layer->x || offset_y != layer->y )
        {
            //���ڸı���layer��λ�ã�lcd��Ҫ����ˢ��
            if (UILAYER_IsBltLayer(dev_info_ptr))
            {
                StoreLayerUpdateRect(dev_info_ptr);   
                //��������λ��
                SetLayerPosition(layer, offset_x, offset_y);
                StoreLayerUpdateRect(dev_info_ptr);
            }
            else
            {
                //��������λ��
                SetLayerPosition(layer, offset_x, offset_y);                
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"UILAYER_SetLayerPosition() failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2164_112_2_18_3_22_53_63,(uint8*)"");
    }
}

/*****************************************************************************/
//     Description : ��㹦���Ƿ���
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:  FALSE һ�㣨���㣩 TRUE ���
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsMultiLayerEnable(void)
{
    return IsMultiLayerEnable();
}

/*****************************************************************************/
//     Description : ��㹦���Ƿ���
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:  FALSE һ�㣨���㣩 TRUE ���
/*****************************************************************************/
LOCAL BOOLEAN IsMultiLayerEnable(void)
{
    //���Ը��ݲ�ͬƽ̨���ã�����1�㣨���㣩��multilayer disable
    //SCI_TRACE_LOW("IsMultiLayerEnable = yes");
#if (defined PLATFORM_SC6600L) || (defined PLATFORM_SC6600R)
    return FALSE;
#else
    return TRUE;
#endif
}


/*****************************************************************************/
//     Description : �����̶���layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void ProtectLayerBuf(
                           UILAYER_STRUCT_T *layer
                           )
{
    if ( layer->is_immutable_drawing
        || ( layer->format & UILAYER_MEM_FORMAT_DOUBLE )
        || ( layer->format & UILAYER_MEM_IMMUTABLE )
		|| PNULL != layer->callback )
    {
        //ʹapp task����ֱ��lcd taskˢ�����
        GUILCD_GetMainLcdBufPtr();
    }
}

/*****************************************************************************/
//     Description : �ͷ�layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void ReleaseLayer(
                        UILAYER_STRUCT_T *layer
                        )
{
    if (PNULL != layer)
    {
        DeactiveLayer( layer );
//        SCI_MEMSET(layer, 0x00, sizeof(UILAYER_STRUCT_T));
//        layer->is_clip_enable = FALSE;
        layer->is_enable = FALSE;
        layer->owner_handle = 0;
    }

}

/*****************************************************************************/
//     Description : ��ͼ�����͸��ɫ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_Clear(
                          GUI_LCD_DEV_INFO const *dev_info_ptr
                          )
{
    UILAYER_STRUCT_T *layer = GetLayerPtrEx(dev_info_ptr, TRUE);
    GUI_COLOR_T *p = PNULL;
    uint32 *u32_p = PNULL;

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }
    if ((PNULL != layer) && (PNULL != layer->buf_addr))
    {
        p = (GUI_COLOR_T*)layer->buf_addr;
        u32_p = (uint32*)layer->buf_addr;
        switch (layer->type)
        {
        case DATA_TYPE_ARGB888:
            SCI_MEMSET(u32_p, NO_ALPHA_COLOR, layer->layer_buf_size);
            break;
            
        case DATA_TYPE_PMARGB888:
            MMI_MEM32SET( u32_p, PM_NO_ALPHA_COLOR, layer->layer_buf_size );
            break;

        case DATA_TYPE_RGB565:
        default:
            if (layer->colorkey_en)
            {
                GUI_COLOR_T color = 0;
                
                color = layer->colorkey;
                SCI_MEM16SET(p, color, layer->layer_buf_size);
            }
            break;
        }
        StoreLayerUpdateRect(dev_info_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"UILAYER_Clear() PNULL == layer || PNULL == layer->buf_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2275_112_2_18_3_22_54_64,(uint8*)"");
    }
}

/*****************************************************************************/
//     Description : ��ͼ�������ɫ
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_FillColor(
                              GUI_LCD_DEV_INFO const *dev_info_ptr,
                              uint32 color
                              )
{
    UILAYER_STRUCT_T *layer = GetLayerPtrEx(dev_info_ptr, TRUE);
    GUI_COLOR_T *p = PNULL;
    uint32 *u32_p = PNULL;

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }
    if ((PNULL != layer) && (PNULL != layer->buf_addr))
    {
        p = (GUI_COLOR_T*)layer->buf_addr;
        u32_p = (uint32*)layer->buf_addr;
        switch (layer->type)
        {
        case DATA_TYPE_ARGB888:
        case DATA_TYPE_PMARGB888:
            MMI_MEM32SET( u32_p, color, layer->layer_buf_size );
            break;

        case DATA_TYPE_RGB565:
        default:
            SCI_MEM16SET(p, (GUI_COLOR_T)color, layer->layer_buf_size);
            break;
        }
        StoreLayerUpdateRect(dev_info_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"UILAYER_FillColor() PNULL == layer || PNULL == layer->buf_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2319_112_2_18_3_22_54_65,(uint8*)"");
    }
}

/*****************************************************************************/
//     Description : �����ڴ�
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void *AllocLayerBuf(
                          UILAYER_MEM_FUNC_T* func_ptr,
                          uint32 buf_size,
                          const char* file_name,
                          uint32 file_line
                          )
{
    void *buf_ptr = PNULL;
    
    if ( 0 == buf_size )
    {
        //SCI_TRACE_LOW:"AllocLayerBuf() 0 == buf_size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2335_112_2_18_3_22_54_66,(uint8*)"");
        return buf_ptr;/*lint !e527*/
    }

    if ( PNULL != func_ptr
        && PNULL != func_ptr->Malloc )
    {
        buf_ptr = func_ptr->Malloc(buf_size+LAYER_BUF_TAIL_SIZE);  /*lint !e26 !e64*/    //uint32��memcheck��־
    }
    else
    {
        buf_ptr = SCI_MallocApp(buf_size+LAYER_BUF_TAIL_SIZE, file_name, file_line);  /*lint !e26 !e64*/    //uint32��memcheck��־
    }


    if (PNULL == buf_ptr)
    {
        SCI_TRACE_LOW("AllocLayerBuf no memory, size = %d, name = %s, line = %d", buf_size, file_name, file_line );
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2343_112_2_18_3_22_54_67,(uint8*)"");
        return buf_ptr;/*lint !e527*/
    }
    SCI_MEMSET(buf_ptr, 0x00, buf_size+LAYER_BUF_TAIL_SIZE);
    
    return buf_ptr;
}

/*****************************************************************************/
//     Description : �ͷ��ڴ�
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void FreeLayerBuf(
                        UILAYER_MEM_FUNC_T* func_ptr,
                        void *buf_ptr
                        )
{
    if ( PNULL != buf_ptr )
    {
        if ( PNULL != func_ptr
            && PNULL != func_ptr->Free )
        {
            func_ptr->Free(buf_ptr);
        }
        else
        {
            SCI_FREE(buf_ptr);
        }
    }
}

/*****************************************************************************/
//     Description : ����Handle����������layer��Handle get focus��ʱ�����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ActiveAllLayerOfHandle(
                                           MMI_HANDLE_T mmi_handle
                                           )
{    
    uint32 i = 0;

    //SCI_TRACE_LOW:"UILAYER_HandleWinGetFocus mmi_handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2375_112_2_18_3_22_54_68,(uint8*)"d", mmi_handle);

    mmi_handle = MMK_ConvertIdToHandle(mmi_handle);
    
    for (i = 0; i < UILAYER_TOTAL_LAYER_COUNT; i++)
    {
        if (s_layer_arr[i].is_enable
            && (s_layer_arr[i].owner_handle == mmi_handle)
            && s_layer_arr[i].is_autofreememory)
        {        
            ActiveLayer(&(s_layer_arr[i]));
        }            
    }
}

/*****************************************************************************/
//     Description : ȥ����win_handle����win_handle�Ͽؼ�����������layer��Ϊ����
// camera�ȴ��ڴ�Ӧ��ʱ����ǰ�ͷ��ڴ����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
//PUBLIC void UILAYER_DeactiveAllLayerOfWinHandle(
//												MMI_HANDLE_T mmi_handle
//												)
//{
//    uint32 i = 0;
//
//    SCI_TRACE_LOW("UILAYER_DeactiveAllLayerOfWinHandle mmi_handle = %x", mmi_handle);
//
//    mmi_handle = MMK_ConvertIdToHandle(mmi_handle);
//
//    for (i = 1; i < UILAYER_TOTAL_LAYER_COUNT; i++)
//    {
//        if (s_layer_arr[i].is_enable
////            && (s_layer_arr[i].owner_handle == mmi_handle)
//            && s_layer_arr[i].is_autofreememory)
//        {        
//			if ((s_layer_arr[i].owner_handle == mmi_handle)
//				|| (PNULL != MMK_GetCtrlPtrByWin(mmi_handle, MMK_GetCtrlId(s_layer_arr[i].owner_handle))))
//			{
//				DeactiveLayer(&(s_layer_arr[i]));
//				//remove blt layer
//				UILAYER_RemoveBltLayer(&(s_layer_arr[i].lcd_dev_info));
//			}
//        }            
//    }   
//}


/*****************************************************************************/
//     Description : ȥ����Handle����������layer��Handle lose focus��ʱ�����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_DeactiveAllLayerOfHandle(
                                             MMI_HANDLE_T mmi_handle
                                             )
{
    uint32 i = 0;

    //SCI_TRACE_LOW:"UILAYER_HandleWinLoseFocus mmi_handle = %x"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2433_112_2_18_3_22_54_69,(uint8*)"d", mmi_handle);

    mmi_handle = MMK_ConvertIdToHandle(mmi_handle);

    for (i = 1; i < UILAYER_TOTAL_LAYER_COUNT; i++)
    {
        if (s_layer_arr[i].is_enable
            && (s_layer_arr[i].owner_handle == mmi_handle)
            && s_layer_arr[i].is_autofreememory)
        {        
            DeactiveLayer(&(s_layer_arr[i]));
            //remove blt layer
            UILAYER_RemoveBltLayer(&(s_layer_arr[i].lcd_dev_info));
        }            
    }   
}

/*****************************************************************************/
//     Description : ��lcd���ݱ��ݵ�mainlayer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_CopyLcdToMainLayer(void)
{
    UILAYER_INFO_T layer_info = {0};
    GUI_LCD_DEV_INFO main_layer_dev = {0, 0};
    uint16 lcd_color = 0;
    uint32 *mem_buf_ptr = PNULL;
   
    MMI_CheckAllocatedMemInfo();
    UILAYER_GetLayerInfo(&main_layer_dev, &layer_info);     /*lint !e605*/
    mem_buf_ptr = (uint32*)UILAYER_GetLayerBufferPtr(&main_layer_dev);
    //if (PNULL != layer_info.layer_buf_ptr)
    if (PNULL != mem_buf_ptr)
    {
        if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(&main_layer_dev))
        {
            GUI_COLOR_T *lcdbuf_ptr = PNULL;
//            uint32 *mem_buf_ptr = PNULL;
            uint32 i = 0;
            uint32 j = 0;
        
            lcdbuf_ptr = GUILCD_GetMainLcdBufPtr();
            if (PNULL == lcdbuf_ptr)
            {
                SCI_TRACE_LOW("ui_layer.c UILAYER_CopyLcdToMainLayer() lcdbuf_ptr == PNULL!");
                return;
            }
//        GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id,&buf_width,PNULL);
//            mem_buf_ptr = (uint32*)layer_info.layer_buf_ptr;
            for (i = 0; i < layer_info.layer_height; i++)
            {
                for (j = 0; j < layer_info.mem_width; j++)
                {
                    lcd_color = *lcdbuf_ptr;
                    *mem_buf_ptr = RGB5652ARGB8888(lcd_color, 0xff);     /*lint !e648*/           
                    lcdbuf_ptr++;
                    mem_buf_ptr++;
                }
            }      
        }
        else
        {
            LCD_CopyMainLcdBufToMem((GUI_COLOR_T*)mem_buf_ptr/*layer_info.layer_buf_ptr*/, layer_info.mem_width, layer_info.layer_height, 0, 0, TRUE);
        }
    }
    MMI_CheckAllocatedMemInfo();
}

/*****************************************************************************/
//     Description : �ͷ�Handle����������layer��Handle close��ʱ�����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ReleaseAllLayerOfHandle(
                                            MMI_HANDLE_T mmi_handle
                                            )
{
    uint32 i = 0;

    //SCI_TRACE_LOW:"UILAYER_HandleWinClose mmi_handle = %x"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2504_112_2_18_3_22_54_70,(uint8*)"d", mmi_handle);

    mmi_handle = MMK_ConvertIdToHandle(mmi_handle);

    for (i = 1; i < UILAYER_TOTAL_LAYER_COUNT; i++)
    {
        if ((s_layer_arr[i].is_enable)
            && (s_layer_arr[i].owner_handle == mmi_handle))
        {        
            UILAYER_RemoveBltLayer(&(s_layer_arr[i].lcd_dev_info));            
            ReleaseLayer(&(s_layer_arr[i]));
        }            
    }       
}

/*****************************************************************************/
//     Description : �ͷ�layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ReleaseLayer(
                                 GUI_LCD_DEV_INFO const *dev_info_ptr
                                 )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    
    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }
    if (IsMainLayerHandle(dev_info_ptr))
    {
        //���㲻���ͷ�
        return;
    }
    UILAYER_RemoveBltLayer(dev_info_ptr);
    if (PNULL != layer)
    {
        ReleaseLayer(layer);
    }
}
//#if  defined(LIVE_WALLPAPER_FRAMEWORK_SUPPORT)
/*****************************************************************************/
//     Description : �ͷ�layerEX
//    Global resource dependence : 
//  Author:   Arvin.wu
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ReleaseLayerExt(
                                 GUI_LCD_DEV_INFO  *dev_info_ptr
                                 )
{
    if(PNULL != dev_info_ptr)
    {
        UILAYER_ReleaseLayer((GUI_LCD_DEV_INFO const *)dev_info_ptr);
        dev_info_ptr->block_id = UILAYER_NULL_HANDLE;
        dev_info_ptr->lcd_id = 0;
    }
}
//#endif
/*****************************************************************************/
//     Description : �ж�layer�Ƿ񼤻�״̬
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsLayerActive(
                                     GUI_LCD_DEV_INFO const *dev_info_ptr
                                     )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    BOOLEAN result = FALSE;
  
    if (!CheckLayerId(dev_info_ptr))
    {
        result = FALSE;
    }
    else if ((PNULL != layer) && layer->is_enable && (PNULL != layer->buf_addr))
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    return result;
}


/*****************************************************************************/
//     Description : ����layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void ActiveLayer(
                       UILAYER_STRUCT_T *layer
                       )
{
    uint32 bits_per_pixel = 0;

    if (PNULL == layer)
    {
        //MMI_ASSERT_WARNING("ActiveLayer() PNULL == layer");
        //SCI_TRACE_LOW:"ActiveLayer() PNULL == layer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2601_112_2_18_3_22_54_71,(uint8*)"");
        return;/*lint !e527*/
    }
    if (PNULL == layer->buf_addr)
    {
        bits_per_pixel = GetLayerBytesPerPixel(layer)*8;
        layer->layer_buf_size = UILAYER_SIZE_OF_PIXELS(
            layer->width, 
            layer->height, 
            bits_per_pixel
            );
//        SCI_ASSERT(0 == layer->raw_layer_buf_ptr);
        if (PNULL != layer->raw_layer_buf_ptr)
        {
            SCI_FREE(layer->raw_layer_buf_ptr);
        }
        layer->raw_layer_buf_ptr = AllocLayerBuf( &layer->mem_func, layer->layer_buf_size, layer->file_name_ptr, layer->file_line);
        layer->buf_addr = (uint32)ALIGN_256BYTES_ADDR_RIGHT(layer->raw_layer_buf_ptr);
    }
    else
    {
        //�п�����������ActiveLayer���Σ��м�û��DeactiveLayer
        //SCI_TRACE_LOW:"ActiveLayer again!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2619_112_2_18_3_22_54_72,(uint8*)"");
    }
}

/*****************************************************************************/
//     Description : ȥ����layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void DeactiveLayer(
                         UILAYER_STRUCT_T *layer
                         )
{
    if (PNULL == layer)
    {
        //MMI_ASSERT_WARNING("DeactiveLayer() PNULL == layer");
        //SCI_TRACE_LOW:"DeactiveLayer() PNULL == layer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2634_112_2_18_3_22_54_73,(uint8*)"");
        return;/*lint !e527*/
    }

    ProtectLayerBuf(layer);
    FreeLayerBuf( &layer->mem_func, layer->raw_layer_buf_ptr);
    layer->raw_layer_buf_ptr = PNULL;
#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
    FreeLayerBuf( &layer->mem_func, layer->raw2_layer_buf_ptr);
    layer->raw2_layer_buf_ptr = PNULL;
    layer->is_raw2 = FALSE;
    layer->is_switch_double_buffer = FALSE;
#endif
    layer->buf_addr = PNULL;
    layer->callback = PNULL;
    layer->param = 0;
    //ʧ��ʱȥ��˫buffer�л�����
    if ( layer->format & UILAYER_MEM_FORMAT_DOUBLE )
    {
        layer->format &= ~UILAYER_MEM_FORMAT_IMMUTABLE;
    }
}

/*****************************************************************************/
//  Description : deactive layer handle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void UILAYER_DeactiveLayer(GUI_LCD_DEV_INFO const *dev_info_ptr)
{
    UILAYER_STRUCT_T *layer_ptr = PNULL;

    if (PNULL == dev_info_ptr)
    {
        return;
    }

    if (IsMainLayerHandle(dev_info_ptr))
    {
        return;
    }

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }
    
    if (IsMultiLayerEnable())
    {
        layer_ptr = GetLayerPtr(dev_info_ptr);
        DeactiveLayer(layer_ptr);
    }
}

/*****************************************************************************/
//  Description : active layer handle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void UILAYER_ActiveLayer(GUI_LCD_DEV_INFO const *dev_info_ptr)
{
    UILAYER_STRUCT_T *layer_ptr = PNULL;

    if (PNULL == dev_info_ptr)
    {
        return;
    }

    if (IsMainLayerHandle(dev_info_ptr))
    {
        return;
    }

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }
    
    if (IsMultiLayerEnable())
    {
        layer_ptr = GetLayerPtr(dev_info_ptr);
        ActiveLayer(layer_ptr);
    }
}

/*****************************************************************************/
//     Description : ��block_idתΪlayerָ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL UILAYER_STRUCT_T *GetLayerPtrEx(
                                      GUI_LCD_DEV_INFO const *dev_info_ptr,
                                      BOOLEAN is_protect
                                      )
{
    UILAYER_STRUCT_T *layer = PNULL;

    if (!CheckLayerId(dev_info_ptr))
    {
        return layer;
    }
    if (UILAYER_NULL_HANDLE == dev_info_ptr->block_id)
    {
        layer = PNULL;
    }
    else if (GUI_BLOCK_MAIN == dev_info_ptr->block_id)
    {
        layer = &(s_layer_arr[0]);
    }
    else if (dev_info_ptr->block_id > GUI_BLOCK_MAX)
    {
        if (dev_info_ptr->block_id < (GUI_BLOCK_MAX+UILAYER_TOTAL_LAYER_COUNT))
        {
            layer = &s_layer_arr[dev_info_ptr->block_id-GUI_BLOCK_MAX];
        }
        else
        {
            //SCI_TRACE_LOW:"GetLayerPtr() dev_info_ptr->block_id = %d > (GUI_BLOCK_MAX+UILAYER_TOTAL_LAYER_COUNT)"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2687_112_2_18_3_22_55_74,(uint8*)"d", dev_info_ptr->block_id);
        }
    }
    //else if (GUI_BLOCK_MAX == dev_info_ptr->block_id)
    //{
    //    SCI_ASSERT(0);
    //}
    
    
    if ( is_protect 
        && PNULL != layer
        && layer->is_immutable_drawing )
    {
        //protect double buffer
        GUILCD_GetMainLcdBufPtr();

        layer->is_immutable_drawing = FALSE;
    }

    return layer;
}

/*****************************************************************************/
//     Description : ��block_idתΪlayerָ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL UILAYER_STRUCT_T *GetLayerPtr(
                                    GUI_LCD_DEV_INFO const *dev_info_ptr
                                    )
{
    return GetLayerPtrEx( dev_info_ptr, FALSE );
}

/*****************************************************************************/
//     Description : �ж��Ƿ�������
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMainLayerHandle(
                                GUI_LCD_DEV_INFO const *dev_info_ptr
                                )
{
    if (UILAYER_HANDLE_MAIN == UILAYER_GetHandleType(dev_info_ptr))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//     Description : ���handle����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC UILAYER_HANDLE_TYPE_E UILAYER_GetHandleType(
                                                   GUI_LCD_DEV_INFO const *dev_info_ptr
                                                   )
{
    UILAYER_HANDLE_TYPE_E handle_type = UILAYER_HANDLE_INVALID;

    if (PNULL != dev_info_ptr)
    {
        if (GUI_MAIN_LCD_ID == dev_info_ptr->lcd_id)
        {
            if ((0 == dev_info_ptr->block_id) 
                || (GUI_BLOCK_MAIN == dev_info_ptr->block_id))
            {
                handle_type = UILAYER_HANDLE_MAIN;
            }
            else if ((dev_info_ptr->block_id > 0)
                && (dev_info_ptr->block_id < GUI_BLOCK_MAX))
            {
                //osd��block_handle
                handle_type = UILAYER_HANDLE_BLOCK;
            }
            else if ((dev_info_ptr->block_id > GUI_BLOCK_MAX)
                && (dev_info_ptr->block_id <= GUI_UILAYER_MAX))
            {
                handle_type = UILAYER_HANDLE_MULTI;
            }
            else
            {
                handle_type = UILAYER_HANDLE_INVALID;
            }
        }
        else
        {
            //sub_lcdת��block
            handle_type = UILAYER_HANDLE_BLOCK;
//            //sub_lcd�Ȳ���Ҫ����
//            //SCI_TRACE_LOW:"UILAYER_GetHandleType() GUI_MAIN_LCD_ID != dev_info_ptr->lcd_id"
//            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2776_112_2_18_3_22_55_75,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"UILAYER_GetHandleType() PNULL == dev_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2781_112_2_18_3_22_55_76,(uint8*)"");
    }
    return handle_type;
}

/*****************************************************************************/
//     Description : �ж��Ƿ���handleΪ��
//    Global resource dependence : 
//  Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsEmptyLayerHandle(
                                        GUI_LCD_DEV_INFO const *dev_info_ptr
                                        )
{
    BOOLEAN result = FALSE;
     
    if (PNULL == dev_info_ptr)
    {
        MMI_ASSERT_WARNING("UILAYER_IsEmptyLayerHandle() PNULL == dev_info_ptr");
        return result;/*lint !e527*/
    }
    
    if (GUI_MAIN_LCD_ID == dev_info_ptr->lcd_id)
    {
        if (UILAYER_NULL_HANDLE == dev_info_ptr->block_id)
        {
            result = TRUE;
        }
    }
    return result;
}

/*****************************************************************************/
//     Description : check layer id
//    Global resource dependence : 
//  Author:   bin.ji
//    Note: TRUE ͼ��id    FALSE��ͼ�� id
/*****************************************************************************/
LOCAL BOOLEAN CheckLayerId(
                           GUI_LCD_DEV_INFO const *dev_info_ptr
                           )
{
    BOOLEAN result = FALSE;

    switch (UILAYER_GetHandleType(dev_info_ptr))
    {
    case UILAYER_HANDLE_MAIN:        //����handle
    case UILAYER_HANDLE_MULTI:        //multi handle
        result = TRUE;
        break;

    default:
        break;
    }
    return result;
}

/*****************************************************************************/
//     Description : ���rect�������Ƿ�͸�������͸���򽫵��²������ˢ����Ŀ�����ȥ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_BltLowerLayerWhenTransparent(
                                                 GUI_LCD_DEV_INFO const *dev_info_ptr,
                                                 const GUI_RECT_T *rect_ptr
                                                 )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    uint32 i = 0;

    if ((PNULL == dev_info_ptr) || (PNULL == rect_ptr))
    {
        //MMI_ASSERT_WARNING("UILAYER_BltLowerLayerWhenTransparent() (PNULL == dev_info_ptr) || (PNULL == rect_ptr)");
        //SCI_TRACE_LOW:"UILAYER_BltLowerLayerWhenTransparent() (PNULL == dev_info_ptr) || (PNULL == rect_ptr)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2850_112_2_18_3_22_55_77,(uint8*)"");
        return;/*lint !e527*/
    }
    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }
    //���Ŀ����������Ĳ���argb888�Ĳ㣬ֱ�ӷ���
    for (i = 0; i < UILAYER_TOTAL_BLT_COUNT; i++)
    {
//        lcd_dev_info.block_id = s_layer_blt[i];
        lcd_dev_info = s_layer_blt[i].lcd_dev_info;
        if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(&lcd_dev_info))
        {
            return;
        }
        if (CompareLcdDev(&lcd_dev_info, dev_info_ptr))
        {
            break;
        }
    }
    //���ж�ͼ���ʱ�����Ŀ���Ϊ͸��ɫ��ˢ��Ŀ���֮ǰ���Ȱѵ��²������copy��Ŀ���
    if (IsRectTransparent(dev_info_ptr, rect_ptr))
    {
        UILAYER_BltLowerLayer(dev_info_ptr, rect_ptr);
    }
}


/*****************************************************************************/
//     Description : ���rect�������Ƿ�͸��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsRectTransparent(
                                GUI_LCD_DEV_INFO const *dev_info_ptr,
                                const GUI_RECT_T *rect_ptr
                                )
{
    UILAYER_STRUCT_T *layer = PNULL;
    GUI_COLOR_T *buf_ptr = PNULL;
    GUI_COLOR_T color = 0;
    GUI_RECT_T clip_rect = {0};
    UILAYER_INFO_T layer_info = {0};
    BOOLEAN result = FALSE;

    layer = GetLayerPtr(dev_info_ptr);
    
    if (PNULL != layer)
    {
        if (layer->colorkey_en)
        {
            if (PNULL != rect_ptr)
            {
                GetLayerInfo(layer, &layer_info);
                buf_ptr = (GUI_COLOR_T*)UILAYER_GetLayerBufferPtr(dev_info_ptr);
//                buf_ptr = (GUI_COLOR_T*)layer_info.layer_buf_ptr;
                if (GetValidRect(layer, rect_ptr, &clip_rect))
                {
                    color = GET_COLOR_BY_POS(buf_ptr, layer_info.mem_width, clip_rect.left, clip_rect.top);
                    if (layer->colorkey == color)
                    {
                        result = TRUE;
                    }
                }
            }
            else
            {
                result = FALSE;
                //MMI_ASSERT_WARNING("IsRectTransparent() PNULL == rect_ptr");
                //SCI_TRACE_LOW:"IsRectTransparent() PNULL == rect_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_2919_112_2_18_3_22_55_78,(uint8*)"");
            }
        }
    }
    else
    {
        result = FALSE;
        // MMI_ASSERT_WARNING("IsRectTransparent() PNULL == layer");
    }
   
    return result;
}

/*****************************************************************************/
//     Description : ��softblt����refblt
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN GetIsSoftBlt(void)
{
#ifdef MMI_SOFTBLTLAYER_ENABLE
    if (0 == MMIAPISET_GetCurrentThemeType())
    {
        return TRUE;
    }
#endif
    return FALSE;
}

/*****************************************************************************/
//     Description : �õ�mainlcd layer�ľ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL DISPLAY_BLEND_LAYER_T *GetMainLcdhandle(void)
{
    //ÿ��get mainlcd handle�����¶�ȡmainlcdbuf ptr����ֹӰ���첽ˢ�¹���
    GUI_COLOR_T *lcd_buf_ptr = PNULL;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    BOOLEAN is_enable = s_mainlcd_handle.layer_info.is_enable;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    lcd_buf_ptr = GUILCD_GetMainLcdBufPtr();

    SCI_ASSERT(PNULL != lcd_buf_ptr); /*assert verified*/
    SCI_MEMSET(&s_mainlcd_handle, 0x00, sizeof(DISPLAY_BLEND_LAYER_T));
    SetBlendDefaultData(&s_mainlcd_handle);
#if defined(MOCOR_SUPPORT_ARGB888_MODE) //&& !defined(WIN32)
    if(LCD_GetARGB888Mode())
    {
        s_mainlcd_handle.layer_info.layer_data.type = DATA_TYPE_ARGB888;
        s_mainlcd_handle.layer_info.layer_data.alpha_sel = DISPLAY_PIXEL_ALPHA;
        s_mainlcd_handle.layer_buf_size = lcd_width*lcd_height*sizeof(uint32);
    }
    else
#endif
    {
        s_mainlcd_handle.layer_info.layer_data.type = DATA_TYPE_RGB565;
        s_mainlcd_handle.layer_info.layer_data.alpha_sel = DISPLAY_BLOCK_ALPHA;
        s_mainlcd_handle.layer_buf_size = lcd_width*lcd_height*sizeof(GUI_COLOR_T);
    }
    SCI_TRACE_LOW("[SFL]GetMainLcdhandle: layer_data.type = %d, buf_size=%d",
                s_mainlcd_handle.layer_info.layer_data.type,
                s_mainlcd_handle.layer_buf_size);

    s_mainlcd_handle.layer_info.buf_addr = (uint32)lcd_buf_ptr;
    s_mainlcd_handle.layer_info.width = lcd_width;
    s_mainlcd_handle.layer_info.height = lcd_height;
    s_mainlcd_handle.layer_info.is_enable = is_enable;
    
    return &s_mainlcd_handle;
}

/*****************************************************************************/
//     Description : �õ�mainlcd layer�ľ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL DISPLAY_BLEND_LAYER_T GetSubLcdhandle(void)
{
    DISPLAY_BLEND_LAYER_T sub_lcd_layer = {0};
#ifdef SUBLCD_SIZE
    GUI_LCD_DEV_INFO sub_lcd_dev = {GUI_SUB_LCD_ID, 0};
    GUI_COLOR_T *lcd_buf_ptr = PNULL;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;

    GUILCD_GetLogicWidthHeight(GUI_SUB_LCD_ID,&lcd_width,&lcd_height);
    lcd_buf_ptr = GUIBLOCK_GetBuf(&sub_lcd_dev);

    SCI_ASSERT(PNULL != lcd_buf_ptr); /*assert verified*/
    SetBlendDefaultData(&sub_lcd_layer);
    sub_lcd_layer.layer_info.layer_data.type = DATA_TYPE_RGB565;
    sub_lcd_layer.layer_info.layer_data.alpha_sel = DISPLAY_BLOCK_ALPHA;
    sub_lcd_layer.layer_buf_size = lcd_width*lcd_height*sizeof(GUI_COLOR_T);
    sub_lcd_layer.layer_info.buf_addr = (uint32)lcd_buf_ptr;
    sub_lcd_layer.layer_info.width = lcd_width;
    sub_lcd_layer.layer_info.height = lcd_height;
#endif
    return sub_lcd_layer;
}

/*****************************************************************************/
//     Description : multi layer blend interface for the APP
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL ERR_LCD_E InvalidateRectUsingLayer(
                    LCD_ID_E lcd_id,
				    DISPLAY_BLEND_LAYER_T *dst_layer_handle_ptr,
    				DISPLAY_BLEND_LAYER_T *layer_arr[],
    				uint32 array_size,
    				DISPLAY_RECT_T *rect_ptr
                    )
{
    if (PNULL == rect_ptr)
    {
        return ERR_LCD_NONE;
    }
    if (GetIsSoftBlt())
    {
#ifdef MMI_SOFTBLTLAYER_ENABLE
        SOFT_InvalidateRectUsingLayer(
            lcd_id,
            dst_layer_handle_ptr,
            layer0_ptr,
            layer1_ptr,
            layer2_ptr,
            layer3_ptr,                
            rect_ptr
            );
#endif
    }
    else
    {
//#ifndef WIN32
#if 1
        if (PNULL != dst_layer_handle_ptr)
        {
            //SCI_TRACE_LOW:"dst_buf: 0x%x, (%d,%d,%d,%d)"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_3018_112_2_18_3_22_55_79,(uint8*)"ddddd", dst_layer_handle_ptr->layer_info.buf_addr,dst_layer_handle_ptr->layer_info.pos.x,dst_layer_handle_ptr->layer_info.pos.y,dst_layer_handle_ptr->layer_info.width,dst_layer_handle_ptr->layer_info.height);
        }
        if (0 != array_size)
        {
            BlendLayer(
                lcd_id,
                dst_layer_handle_ptr,
                layer_arr,
                array_size,
                rect_ptr
                );
        }
        else
        {
            //SCI_TRACE_LOW:"InvalidateRectUsingLayer() array_size==0!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_3032_112_2_18_3_22_55_80,(uint8*)"");
        }
#endif
    }

    return ERR_LCD_NONE;
}

/*****************************************************************************/
//     Description : �õ�lcd��ȫ������
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetLCDRect(
                            GUI_LCD_ID_E lcd_id
                            )
{
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    GUI_RECT_T lcd_rect = {0};

    GUILCD_GetLogicWidthHeight(lcd_id, &lcd_width, &lcd_height);
    lcd_rect.left = 0;
    lcd_rect.top = 0;
    lcd_rect.right = (lcd_width - 1);
    lcd_rect.bottom = (lcd_height - 1);

    return lcd_rect;
}

/*****************************************************************************/
//     Description : ����ü�lcd/layer��Χ�ڵ���Ч����
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN GetValidRect(
                            UILAYER_STRUCT_T *layer,
                            const GUI_RECT_T *src_rect_ptr,
                            GUI_RECT_T *dst_rect_ptr    //out
                            )
{
    GUI_RECT_T full_lcd_rect = {0};
    GUI_RECT_T full_layer_rect = {0};
    GUI_RECT_T clip_rect = {0};
    BOOLEAN result = FALSE;

    if (PNULL == layer)
    {
        return FALSE;
    }    
    full_layer_rect = GetLayerRect(layer);
    full_lcd_rect = GetLCDRect(GUI_MAIN_LCD_ID);
    clip_rect = *src_rect_ptr;
//    ConvertLayer2LCDRect(layer, src_rect_ptr, &clip_rect);

    if (GUI_IntersectRect(&clip_rect, full_lcd_rect, clip_rect))
    {
        if (GUI_IntersectRect(&clip_rect, full_layer_rect, clip_rect))
        {
            ConvertLCD2LayerRect(layer, &clip_rect, dst_rect_ptr);
            result = TRUE;
        }
    }
    return result;
}

// #ifdef UILAYER_NEED_ALIGN8
// /*****************************************************************************/
// //     Description : ����8�����Ե���ݣ�Ϊ������refҪ���8����ˢ�£�������ˢ������
// //    Global resource dependence : 
// //  Author:   bin.ji
// //    Note:
// /*****************************************************************************/
// LOCAL void Save8AlignData(
//                           UILAYER_STRUCT_T *layer,
//                           const GUI_RECT_T *rect_ptr,
//                           void **data_ptr
//                           )
// {
//     GUI_COLOR_T *buf_ptr = PNULL;
//     uint16 height = 0;
//     uint16 buf_width = 0;
//     UILAYER_INFO_T layer_info = {0};
//     GUI_COLOR_T *layer_buf_ptr = PNULL;
//     int16 left = 0;
//     int16 right = 0;    
//     int16 aleft = 0;
//     int16 aright = 0;
//     int16 x = 0;
//     int16 y = 0;
//     uint32 bytes_per_pixel = 0;
// 
//     if ((PNULL == layer) || (PNULL == data_ptr) || (PNULL == rect_ptr))
//     {
//         //MMI_ASSERT_WARNING("Save8AlignData() input param error!");
//         SCI_TRACE_LOW("Save8AlignData() input param error!");
//         return;/*lint !e527*/
//     }
// 
//     if (!CheckParamRect(rect_ptr))
//     {
//         return;
//     }
//     GetLayerInfo(layer, &layer_info);
//     buf_width = layer_info.mem_width;
//     //ȡ8�����ַ
//     layer_buf_ptr = (GUI_COLOR_T *)layer->buf_addr;    
//     height = (rect_ptr->bottom+1-rect_ptr->top);
//     bytes_per_pixel = GetLayerBytesPerPixel(layer);
//     buf_ptr = (GUI_COLOR_T*)SCI_ALLOCA(MAX_8ALIGN_EDGE_PIXEL*height*bytes_per_pixel);/*lint !e26 !e64*/
//     if (PNULL == buf_ptr)
//     {
//         *data_ptr = PNULL;
//         return;
//     }
//     *data_ptr = (void*)buf_ptr;
//     left = rect_ptr->left;
//     right = rect_ptr->right;
//     aleft = ALIGN_8BYTES_POS_LEFT(left);
//     aright = ALIGN_8BYTES_POS_RIGHT(right);
//     y = rect_ptr->top;
//     while (height--)
//     {
//         for (x= aleft; x <left; x++)
//         {
//             *buf_ptr++ = GET_COLOR_BY_POS(layer_buf_ptr, buf_width, x, y);
//         }
//         for (x= (right+1); x <=aright; x++)
//         {
//             *buf_ptr++ = GET_COLOR_BY_POS(layer_buf_ptr, buf_width, x, y);
//         }
//         y++;
//     }
// }
// 
// /*****************************************************************************/
// //     Description : ����8�����Ե���ݣ�Ϊ������refҪ���8����ˢ�£�������ˢ������
// //    Global resource dependence : 
// //  Author:   bin.ji
// //    Note:
// /*****************************************************************************/
// LOCAL void Load8AlignData(
//                           UILAYER_STRUCT_T *layer,
//                           const GUI_RECT_T *rect_ptr,
//                           void **data_ptr
//                           )
// {
//     GUI_COLOR_T *buf_ptr = PNULL;
//     uint16 height = 0;
//     uint16 buf_width = 0;
//     UILAYER_INFO_T layer_info = {0};
//     GUI_COLOR_T *layer_buf_ptr = PNULL;
//     int16 left = 0;
//     int16 right = 0;   
//     int16 aleft = 0;
//     int16 aright = 0;
//     int16 x = 0;
//     int16 y = 0;
//     uint32 bytes_per_pixel = 0;
// 
//     if ((PNULL == layer) || (PNULL == data_ptr) || (PNULL == rect_ptr))
//     {
//         //MMI_ASSERT_WARNING("Load8AlignData() input param error!");
//         SCI_TRACE_LOW("Load8AlignData() input param error!");
//         return;/*lint !e527*/
//     }
//     if (PNULL == *data_ptr)
//     {
//         return;
//     }
//     if (!CheckParamRect(rect_ptr))
//     {
//         return;
//     }
//     GetLayerInfo(layer, &layer_info);
//     buf_width = layer_info.mem_width;
//     //ȡ8�����ַ
//     layer_buf_ptr = (GUI_COLOR_T *)layer->buf_addr;
//     height = (rect_ptr->bottom+1-rect_ptr->top);
//     buf_ptr = (GUI_COLOR_T*)*data_ptr;
//     left = rect_ptr->left;
//     right = rect_ptr->right;
//     aleft = ALIGN_8BYTES_POS_LEFT(rect_ptr->left);
//     aright = ALIGN_8BYTES_POS_RIGHT(rect_ptr->right);
//     y = rect_ptr->top;
//     while (height--)
//     {
//         for (x= aleft; x <left; x++)
//         {
//             SET_COLOR_BY_POS(layer_buf_ptr, buf_width, x, y, *buf_ptr);
//             buf_ptr++;
//         }
//         for (x= (right+1); x <=aright; x++)
//         {
//             SET_COLOR_BY_POS(layer_buf_ptr, buf_width, x, y, *buf_ptr);
//             buf_ptr++;
//         }
//         y++;
//     }
//     SCI_FREE(*data_ptr);
//     *data_ptr = PNULL;
// }
// #endif

/*****************************************************************************/
//     Description : check param rect
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckParamRect(
                             const GUI_RECT_T *rect_ptr
                             )
{
    BOOLEAN result = TRUE;

    if (PNULL == rect_ptr)
    {
        result = FALSE;
    }
    else if ((rect_ptr->right < rect_ptr->left) || (rect_ptr->bottom < rect_ptr->top))
    {
        result = FALSE;
    }
    return result;
}

/*****************************************************************************/
// Description : rotation osd layer
// Global resource dependence : 
// Author: bin.ji
// Note:
/*****************************************************************************/
LOCAL void RotationOsdLayer(void)
{
    GUI_LCD_DEV_INFO *dev_info_ptr = &s_osd_layer;
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;
    
    if (IsMainLayerHandle(dev_info_ptr))
    {
        //��ֹ���㱻�����
        return;
    }
    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }      
    if ((PNULL != layer) && layer->is_enable)
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

        layer->width = lcd_width;
        layer->height = lcd_height;
    }    
}

/*****************************************************************************/
// Description : modify block width and rect info for rotation
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC void UILAYER_Rotation(
                             GUI_LCD_DEV_INFO   *lcd_dev_ptr
                             )
{
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;
    UILAYER_STRUCT_T    *layer_ptr = PNULL;

    //modify main handle
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    s_mainlcd_handle.layer_info.width  = lcd_width;
    s_mainlcd_handle.layer_info.height = lcd_height;

    //get main layer
    layer_ptr = GetLayerPtr(lcd_dev_ptr);
    if (PNULL != layer_ptr)
    {
        layer_ptr->width  = lcd_width;
        layer_ptr->height = lcd_height;
    }
    else
    {
        //SCI_TRACE_LOW:"ui_layer.c UILAYER_Rotation() layer_ptr == PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_3310_112_2_18_3_22_56_81,(uint8*)"");
    }
    //rotate osd layer
    RotationOsdLayer();

    //modify bg
    MMITHEME_SetBgLayerChanged();
}

/*****************************************************************************/
//     Description : �ڴ��⣺���һ��ʱ����ĳ��ַ�����Ƿ�ı�
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void MMI_MemMoniterStart(
                                uint32 *mem_ptr
                                )
{
    if (PNULL != mem_ptr)
    {
        s_mmi_memmoniter_info.mem_ptr = mem_ptr;
        s_mmi_memmoniter_info.mem_value = *mem_ptr;
    }
}

/*****************************************************************************/
//     Description : �ڴ��⣺���һ��ʱ����ĳ��ַ�����Ƿ�ı�
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void MMI_MemMoniterEnd(void)
{
    if (PNULL != s_mmi_memmoniter_info.mem_ptr)
    {
        if (s_mmi_memmoniter_info.mem_value != *(s_mmi_memmoniter_info.mem_ptr))
        {
            //MMI_ASSERT_WARNING("MMI_MemMoniterEnd fail!");
            //SCI_TRACE_LOW:"MMI_MemMoniterEnd fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_3345_112_2_18_3_22_56_82,(uint8*)"");
        }
    }
    s_mmi_memmoniter_info.mem_ptr = PNULL;
    s_mmi_memmoniter_info.mem_value = 0;
}

/*****************************************************************************/
//     Description : �ж�handle�Ƿ�focus
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsFocusHandle(
                            MMI_HANDLE_T owner_handle
                            )
{
    BOOLEAN is_focus = FALSE;

    owner_handle = MMK_ConvertIdToHandle(owner_handle);
    switch (MMK_GetHandleType(owner_handle))
    {
    case MMI_HANDLE_WINDOW:
        is_focus = MMK_IsFocusWin(owner_handle);
        break;

    case MMI_HANDLE_CONTROL:
        is_focus = MMK_IsFocusWin(MMK_GetWinHandleByCtrl(owner_handle));
        break;

    default:
        break;
    }

    return is_focus;
}

/*****************************************************************************/
//     Description : ��ͼ�����ĳ����ɫ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ClearRect(
                          GUI_LCD_DEV_INFO const *dev_info_ptr,
                          GUI_RECT_T rect
                          )
{
    UILAYER_STRUCT_T *layer = GetLayerPtrEx(dev_info_ptr, TRUE);

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }
    if ((PNULL != layer) && (PNULL != layer->buf_addr))
    {
        GUI_RECT_T layer_rect = GetLayerRect( layer );

        if ( !GUI_IsInvalidRect( rect ) 
            //layer rect����ȫ��rect�����ڣ���clear all
            && GUI_RectIsCovered( rect, layer_rect )  )
        {
            UILAYER_Clear( dev_info_ptr );

            return;
        }

        switch (layer->type)
        {
        case DATA_TYPE_ARGB888:
        case DATA_TYPE_PMARGB888:
            LCD_FillRectU32(dev_info_ptr, rect, 
                ( DATA_TYPE_PMARGB888 == layer->type ) ? PM_NO_ALPHA_COLOR : NO_ALPHA_COLOR);
            break;
        
        case DATA_TYPE_RGB565:
        default:
            if (layer->colorkey_en)
            {
                GUI_COLOR_T *p = PNULL;

                p = (GUI_COLOR_T*)layer->buf_addr;
                LCD_FillRect(dev_info_ptr, rect, layer->colorkey);
            }
            break;
        }    
    }
    else
    {
        //SCI_TRACE_LOW:"UILAYER_Clear() PNULL == layer || PNULL == layer->buf_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_3419_112_2_18_3_22_56_83,(uint8*)"");
    }
}

/*****************************************************************************/
//     Description : �ж�layer�Ƿ񼤻�״̬
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC DISPLAY_DATA_TYPE_E UILAYER_GetLayerColorType(
                                     GUI_LCD_DEV_INFO const *dev_info_ptr
                                     )
{
    DISPLAY_DATA_TYPE_E data_type = DATA_TYPE_RGB565;
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
  
    if (!CheckLayerId(dev_info_ptr))
    {
        return data_type;
    }
//    if ((PNULL != layer) && layer->is_enable && (PNULL != layer->buf_addr))
    if ((PNULL != layer) && layer->is_enable)
    {
        data_type = layer->type;
    }
    return data_type;
}

/*****************************************************************************/
//     Description : ����layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL uint32 GetLayerBytesPerPixel(
                       UILAYER_STRUCT_T *layer
                       )
{
    return GetBytesPerPixel( layer->type );
//     uint32 bytes_per_pixel = 0;
// 
//     switch (layer->type)
//     {
//     case DATA_TYPE_ARGB888:
//         bytes_per_pixel = sizeof(uint32);
//         break;
//         
//     case DATA_TYPE_RGB565:
//     default:
//         bytes_per_pixel = sizeof(GUI_COLOR_T);
//         break;
//     }
//     return bytes_per_pixel;
}

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
                                )
{
    UILAYER_STRUCT_T *dst_layer_ptr = GetLayerPtrEx(dst_dev_ptr, TRUE);
    UILAYER_STRUCT_T *src_layer_ptr = GetLayerPtr(src_dev_ptr);
    uint8 *src_buf = PNULL;
    uint8 *dst_buf = PNULL;
    uint32 blt_width = 0;
    uint32 blt_height = 0;
    uint32 src_width = 0;
    uint32 dst_width = 0;
    GUI_RECT_T src_layer_rect = {0};
    GUI_RECT_T dst_layer_rect = {0};
    GUI_RECT_T rect = {0};
    DISPLAY_DATA_TYPE_E src_data_type = 0;
    DISPLAY_DATA_TYPE_E dst_data_type = 0;
    uint32 src_bytes_per_pixel = 0;
    uint32 dst_bytes_per_pixel = 0;
    DISPLAY_BLEND_LAYER_T *src_layer_arr[2] = {0};
    DISPLAY_BLEND_LAYER_T temp_layer_arr[2] = {0};
    DISPLAY_RECT_T display_rect = {0};

    if (PNULL == rect_ptr)
    {
        return;
    }
    if (!CheckLayerId(src_dev_ptr)||!CheckLayerId(dst_dev_ptr))
    {
        return;
    }
    if ((PNULL == src_layer_ptr) || (PNULL == src_layer_ptr->buf_addr)
        ||(PNULL == dst_layer_ptr) || (PNULL == dst_layer_ptr->buf_addr))
    {
        return;
    }
    src_layer_rect = GetLayerRect(src_layer_ptr);
    dst_layer_rect = GetLayerRect(dst_layer_ptr);

    if (!GUI_IntersectRect(&rect, *rect_ptr, src_layer_rect))
    {
        return;
    }
    if (!GUI_IntersectRect(&rect, rect, dst_layer_rect))
    {
        return;
    }

    if (is_compound)
    {       
//        src_layer_arr[0] = &(dst_layer_ptr->layer_struct);
//        src_layer_arr[1] = &(src_layer_ptr->layer_struct);
        CopyDisplayBlendLayer(&temp_layer_arr[0], dst_layer_ptr);
        CopyDisplayBlendLayer(&temp_layer_arr[1], src_layer_ptr);
        src_layer_arr[0] = &temp_layer_arr[0];
        src_layer_arr[1] = &temp_layer_arr[1];

        display_rect = Convert2DispRect(rect);

        if(display_rect.left < 0)
        {
           display_rect.left = 0;
        }

        BlendLayer(
            dst_dev_ptr->lcd_id,
//            &dst_layer_ptr->layer_struct,
            src_layer_arr[0],
            src_layer_arr,
            2,
            &display_rect
            );
    }
    else
    {
        src_buf = (uint8*)src_layer_ptr->buf_addr;
        src_width = src_layer_ptr->width;
        src_data_type = src_layer_ptr->type;
    
        dst_buf = (uint8*)dst_layer_ptr->buf_addr;
        dst_width = dst_layer_ptr->width;
        dst_data_type = dst_layer_ptr->type;
        blt_width  = rect.right - rect.left + 1;
        blt_height = rect.bottom - rect.top + 1;   
    
        src_bytes_per_pixel = GetBytesPerPixel(src_data_type);
        dst_bytes_per_pixel = GetBytesPerPixel(dst_data_type);
        if ((rect.left < src_layer_rect.left) || (rect.left < dst_layer_rect.left) 
            || (rect.top < src_layer_rect.top)|| (rect.top < dst_layer_rect.top))
        {
            return;
        }
        src_buf += (((rect.left-src_layer_rect.left) + (rect.top-src_layer_rect.top)*src_width)*src_bytes_per_pixel);   /*lint !e737*/
        dst_buf += (((rect.left-dst_layer_rect.left) + (rect.top-dst_layer_rect.top)*dst_width)*dst_bytes_per_pixel);   /*lint !e737*/
    
        BltBuffer(src_buf, src_width, src_data_type, dst_buf, dst_width, dst_data_type, blt_width, blt_height);
    }
    //store update rect
    LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dst_dev_ptr, &rect, &rect);
    MMITHEME_StoreUpdateRect(dst_dev_ptr, rect);   
}

/*****************************************************************************/
//     Description : �����������͵õ�pixel���ֽ���
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL uint32 GetBytesPerPixel(
                              DISPLAY_DATA_TYPE_E data_type
                              )
{ 
    uint32 bytes_per_pixel = 0;

    switch(data_type)
    {
    case DATA_TYPE_ARGB888:
    case DATA_TYPE_PMARGB888:
        bytes_per_pixel = sizeof(uint32);
        break;
    
    case DATA_TYPE_RGB565:
    default:
        bytes_per_pixel = sizeof(GUI_COLOR_T);
        break;
    }
    return bytes_per_pixel;
}

/*****************************************************************************/
//     Description : blt pixel
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL __inline void BltPixel_ARGB888ToARGB888(
                    uint8 *src_buf_ptr,
                    uint8 *dst_buf_ptr
                    )
{
    *((uint32*)dst_buf_ptr) = *((uint32*)src_buf_ptr);
}

/*****************************************************************************/
//     Description : blt pixel
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL __inline void BltPixel_RGB565ToRGB565(
                    uint8 *src_buf_ptr,
                    uint8 *dst_buf_ptr
                    )
{
    *((uint16*)dst_buf_ptr) = *((uint16*)src_buf_ptr);
}

/*****************************************************************************/
//     Description : blt pixel
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL __inline void BltPixel_RGB565ToARGB888(
                    uint8 *src_buf_ptr,
                    uint8 *dst_buf_ptr
                    )
{
    uint16 *src_buf = (uint16*)src_buf_ptr;
    uint32 *dst_buf = (uint32*)dst_buf_ptr;
    uint16 src_color = 0;
    
    src_color = *src_buf;
    *dst_buf = RGB5652ARGB8888(src_color, 0xff);    /*lint !e648*/
}

/*****************************************************************************/
//     Description : blt pixel
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL __inline void BltPixel_ARGB888ToRGB565(
                    uint8 *src_buf_ptr,
                    uint8 *dst_buf_ptr
                    )
{
    uint32 *src_buf = (uint32*)src_buf_ptr;
    uint16 *dst_buf = (uint16*)dst_buf_ptr;

    *dst_buf = RGB8882RGB565(*src_buf);
}

/*****************************************************************************/
//     Description : �����������͵õ�blt pixel�ĺ���
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BLTPIXEL_FUNC_T GetBltPixelFunc(
                              DISPLAY_DATA_TYPE_E src_data_type,
                              DISPLAY_DATA_TYPE_E dst_data_type
                              )
{ 
    BLTPIXEL_FUNC_T blt_pixel_func = PNULL;

    if (DATA_TYPE_ARGB888 == dst_data_type)
    {
        switch(src_data_type)
        {
        case DATA_TYPE_ARGB888:
            blt_pixel_func = BltPixel_ARGB888ToARGB888;
            break;
    
        case DATA_TYPE_RGB565:
        default:
            blt_pixel_func = BltPixel_RGB565ToARGB888;
            break;
        }
    }
    else
    {
        switch(src_data_type)
        {
        case DATA_TYPE_ARGB888:
            blt_pixel_func = BltPixel_ARGB888ToRGB565;
            break;
    
        case DATA_TYPE_RGB565:
        default:
            blt_pixel_func = BltPixel_RGB565ToRGB565;
            break;
        }
    }
    return blt_pixel_func;
}

/*****************************************************************************/
//     Description : ��ĳ������ˢ����һ����ȥ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void BltBuffer(
                     uint8 *src_buf_ptr,
                     uint32 src_buf_width,
                     DISPLAY_DATA_TYPE_E src_data_type,
                     uint8 *dst_buf_ptr,
                     uint32 dst_buf_width,
                     DISPLAY_DATA_TYPE_E dst_data_type,
                     uint32 blt_width,
                     uint32 blt_height
                     )
{
    uint8 *src_buf = PNULL;
    uint8 *dst_buf = PNULL;
    uint32 xcount = 0;
    uint32 ycount = 0;
    uint32 src_bytes_per_pixel = 0;
    uint32 dst_bytes_per_pixel = 0;
    uint32 srcjumpcount = 0;
    uint32 dstjumpcount = 0;
    uint32 srcbltlinecount = 0;
    uint32 dstbltlinecount = 0;
    BLTPIXEL_FUNC_T blt_pixel_func = PNULL;
    BOOLEAN is_same_type = FALSE;

    if ((src_buf_width < blt_width) || (dst_buf_width < blt_width))
    {
        return;
    }
    if ((PNULL == src_buf_ptr) || (PNULL == dst_buf_ptr))
    {
        return;
    }    
    src_buf = src_buf_ptr;
    dst_buf = dst_buf_ptr;
    ycount = blt_height;
    src_bytes_per_pixel = GetBytesPerPixel(src_data_type);
    dst_bytes_per_pixel = GetBytesPerPixel(dst_data_type);
    blt_pixel_func = GetBltPixelFunc(src_data_type, dst_data_type);
    if (PNULL == blt_pixel_func
        || 0 == ycount )
    {
        return;
    }
    is_same_type = (src_data_type == dst_data_type);
    srcjumpcount = (src_buf_width-blt_width)*src_bytes_per_pixel;
    dstjumpcount = (dst_buf_width-blt_width)*dst_bytes_per_pixel;
    srcbltlinecount = blt_width*src_bytes_per_pixel;
    dstbltlinecount = blt_width*dst_bytes_per_pixel;

    if ( src_buf_width == blt_width
        && dst_buf_width == blt_width
        && is_same_type )
    {
        SCI_MEMCPY(dst_buf, src_buf, srcbltlinecount * ycount );
    }
    else
    {
        while(ycount)
        {
            if (is_same_type)
            {
                SCI_MEMCPY(dst_buf, src_buf, srcbltlinecount);
                src_buf+=srcbltlinecount;
                dst_buf+=dstbltlinecount;
            }
            else
            {
                xcount = blt_width;
                while(xcount)
                {
                    blt_pixel_func(src_buf, dst_buf);
                    
                    src_buf+=src_bytes_per_pixel;
                    dst_buf+=dst_bytes_per_pixel;
                    xcount--;
                }
            }
            src_buf += srcjumpcount;
            dst_buf += dstjumpcount;
            ycount--;
        }
    }
}

/*****************************************************************************/
//     Description : �ж�layer�Ƿ�blt layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsBltLayer(
                                  GUI_LCD_DEV_INFO const *dev_info_ptr
                                  )
{
    if (PNULL == dev_info_ptr)
    {
        return FALSE;
    }
    if (!CheckLayerId(dev_info_ptr))
    {
        return FALSE;
    }  
    if (FindBltLayer(dev_info_ptr))
    {
        //���У�ֱ�ӷ���
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//     Description : blend layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL ERR_DISPLAY_E BlendLayer(  LCD_ID_E lcd_id,
				                                 DISPLAY_BLEND_LAYER_T *dst_layer_ptr,
    				                             DISPLAY_BLEND_LAYER_T *layer_arr[],
				                                 uint32 array_size,
    				                             DISPLAY_RECT_T *rect_ptr)
{
    ERR_DISPLAY_E error = ERR_DISPLAY_NONE;

#if defined(PLATFORM_SC6800H) || defined(PLATFORM_SC8800G)  //Ӳ���ϳ�ͼ��������<1024
    uint32 bytes_per_pixel = 0;
    uint32 buf_size = 0;
    void *buf_ptr = PNULL;
    DISPLAY_DATA_TYPE_E data_type = DATA_TYPE_ARGB888;
    uint8 *src_buf = PNULL;
    uint16 src_width = 0;
    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T layer_rect = {0};
    GUI_RECT_T cross_rect = {0};
    uint16 cross_rect_width = 0;
    uint16 cross_rect_height = 0;
    DISPLAY_BLEND_LAYER_T copy_blend = {0};
    uint32 tick_count = 0;
    uint32 i = 0;
    
    if ((PNULL == rect_ptr) || (PNULL == layer_arr) || (PNULL == layer_arr[0]))
    {
        return error;
    }
    for (i = 1; i < array_size; i++)
    {
        if ((PNULL != layer_arr[i]) && (layer_arr[i]->layer_info.width >= HARDWARE_LAYER_WIDTH_LIMIIT))
        {
            //��֧�ַ�0ͼ���ȴ���Ӳ������
            return error;
        }
    }
    if (layer_arr[0]->layer_info.width >= HARDWARE_LAYER_WIDTH_LIMIIT)
    {
        //��ȴ���Ӳ�����ƣ���layer_arr[0]����copy�����������ײ�
        layer_rect = GetLayerBlendRect(layer_arr[0]);
        copy_blend = *layer_arr[0];
        copy_blend.layer_info.alpha_buf_addr = PNULL;

        disp_rect = Convert2GuiRect(*rect_ptr);
        if (!GUI_IntersectRect(&cross_rect, disp_rect, layer_rect))
        {
            return error;
        }

        copy_blend.layer_info.width = cross_rect_width = (cross_rect.right+1-cross_rect.left);
        copy_blend.layer_info.height = cross_rect_height = (cross_rect.bottom+1-cross_rect.top);
        
        if (cross_rect_width >= HARDWARE_LAYER_WIDTH_LIMIIT)
        {
            //�����Ȼ����Ӳ������
            return error;
        }
        data_type = copy_blend.layer_info.layer_data.type;
        bytes_per_pixel = (DATA_TYPE_ARGB888 == data_type) ? sizeof(uint32):sizeof(GUI_COLOR_T);
        buf_size = UILAYER_SIZE_OF_PIXELS(
            cross_rect_width, 
            cross_rect_height, 
            bytes_per_pixel*8
            );
        buf_ptr = AllocLayerBuf( PNULL, buf_size, _D_FILE_NAME, _D_FILE_LINE);
//        copy_blend.layer_info.buf_addr = (uint32)buf_ptr;
        copy_blend.layer_info.buf_addr = ALIGN_256BYTES_ADDR_RIGHT(buf_ptr);        
               
        copy_blend.layer_info.pos.x = cross_rect.left;
        copy_blend.layer_info.pos.y = cross_rect.top;

        src_width = layer_arr[0]->layer_info.width;
        src_buf = (uint8*)layer_arr[0]->layer_info.buf_addr;
        if ((PNULL != src_buf) && (PNULL != buf_ptr))
        {
            src_buf += (((cross_rect.left-layer_rect.left) + (cross_rect.top-layer_rect.top)*src_width)*bytes_per_pixel);   /*lint !e737*/
            tick_count = SCI_GetTickCount();
            BltBuffer(src_buf, src_width, data_type, (uint8 *)copy_blend.layer_info.buf_addr, cross_rect_width, data_type, cross_rect_width, cross_rect_height);
            //SCI_TRACE_LOW:"ui_layer.c BltBuffer() buf_size = %d, tick = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_3896_112_2_18_3_22_57_84,(uint8*)"dd", buf_size, (SCI_GetTickCount()-tick_count));
            
            layer_arr[0] = &copy_blend;     /*lint !e684*/
//#ifdef WIN32
//            MSDev_DISPLAY_BlendLayer(lcd_id,layer_arr,array_size, rect_ptr);
//#endif
            error = DISPLAY_BlendLayer(lcd_id, dst_layer_ptr, layer_arr, array_size, rect_ptr);
        }
        else
        {
            //SCI_TRACE_LOW:"BlendLayer() src_buf = 0x%x, buf_ptr = 0x%x!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_3906_112_2_18_3_22_57_85,(uint8*)"dd", src_buf, buf_ptr);
        }
        FreeLayerBuf( PNULL, buf_ptr );
    }
    else
#endif
    {
//#ifdef WIN32
//		MSDev_DISPLAY_BlendLayer(lcd_id,layer_arr,array_size, rect_ptr);
//#endif
        error = DISPLAY_BlendLayer(lcd_id, dst_layer_ptr, layer_arr, array_size, rect_ptr);
    }

    return error;
}

/*****************************************************************************/
//     Description : ת��rect��ʽ��GUI_RECT_T->DISPLAY_RECT_T
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL DISPLAY_RECT_T Convert2DispRect(
                                     GUI_RECT_T rect
                                     )
{
    DISPLAY_RECT_T disp_rect = {0};

    disp_rect.left = rect.left;
    disp_rect.top = rect.top;
    disp_rect.right = rect.right;
    disp_rect.bottom = rect.bottom;

    return disp_rect;
}

/*****************************************************************************/
//     Description : ת��rect��ʽ��DISPLAY_RECT_T->GUI_RECT_T
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T Convert2GuiRect(
                                DISPLAY_RECT_T disp_rect
                                )
{
    GUI_RECT_T rect = {0};

    rect.left = disp_rect.left;
    rect.top = disp_rect.top;
    rect.right = disp_rect.right;
    rect.bottom = disp_rect.bottom;

    return rect;
}

/*****************************************************************************/
//     Description : ��Ϊ��Ļץͼ���ܣ����lcd_buffer�Լ�ͼ������
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_GetBltInfoForPrintScreen(
                                                UILAYER_PRINTSCREEN_INFO_T *print_info_ptr
                                                )
{
    UILAYER_STRUCT_T *layer = PNULL;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    uint32 i = 0;

    if (PNULL == print_info_ptr)
    {
        //SCI_TRACE_LOW:"UILAYER_GetBltInfoForPrintScreen param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_3972_112_2_18_3_22_57_86,(uint8*)"");
        return FALSE;
    }
    for (i = 0; i < UILAYER_TOTAL_BLT_COUNT; i++)
    {
        lcd_dev_info.lcd_id = 0;
        lcd_dev_info.block_id = s_layer_blt[i].lcd_dev_info.block_id;
        layer = GetLayerPtr(&lcd_dev_info);
        if (PNULL != layer)
        {
//            print_info_ptr->layer_stuct_arr[i] = &(layer->layer_struct);
            CopyDisplayBlendLayer((DISPLAY_BLEND_LAYER_T*)&print_info_ptr->layer_stuct_arr[i], layer);
        }
    }
    print_info_ptr->lcd_layer_blend = *GetMainLcdhandle();
    print_info_ptr->sublcd_layer_blend = GetSubLcdhandle();
    print_info_ptr->arr_size = UILAYER_TOTAL_BLT_COUNT;
    return TRUE;
}
#ifdef WRE_SUPPORT
/*****************************************************************************/
//����Description : ��ĳ���ƶ���ָ���ĺ���
//����Global resource dependence : 
//����Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_MoveLayer(
									GUI_LCD_DEV_INFO const *dst_dev_ptr,
									GUI_LCD_DEV_INFO const *after_dev_ptr)
{
    UILAYER_BLT_T dl = {0};
    uint32 i = 0;
    uint32 after = 0xffff;
    uint32 dst = 0xffff;

    if (!CheckLayerId(dst_dev_ptr) || !CheckLayerId(after_dev_ptr))
    {
        return;
    }
	for (i=0; i < UILAYER_TOTAL_BLT_COUNT; i++)
	{
		if(CompareLcdDev(&s_layer_blt[i].lcd_dev_info, after_dev_ptr))
		{
			after = i;
            break;
		}
	}  
	for (i=0; i < UILAYER_TOTAL_BLT_COUNT; i++)
	{
		if(CompareLcdDev(&s_layer_blt[i].lcd_dev_info, dst_dev_ptr))
		{
			dst = i;
            break;
		}
	}
    if ((0xffff == after) || (0xffff == dst))
    {
        return;
    }
    if ((dst == (after+1)) || (after == dst))
    {
        return;
    }
    dl = s_layer_blt[dst];
    if (dst > after)
    {
	    for(i=dst; i>after+1; i--)
	    {
		    s_layer_blt[i] = s_layer_blt[i-1];
	    }    
        s_layer_blt[after+1] = dl;
    }
    else
    {
	    for(i=dst; i<after; i++)
	    {
		    s_layer_blt[i] = s_layer_blt[i+1];
	    }
        s_layer_blt[after] = dl;
    }
    /*
	int j,ak=-1,si=-1;
	UILAYER_BLT_T dl={0x00};
	//��λ������
	for (j=0;j <= (UILAYER_TOTAL_BLT_COUNT-1);j++)
	{
		if(CompareLcdDev(&s_layer_blt[j].lcd_dev_info,after_dev_ptr))
		{
			ak=j;
		}
		else if(CompareLcdDev(&s_layer_blt[j].lcd_dev_info,dst_dev_ptr))
		{
			si=j;
		}
		if(ak != -1 && si != -1) break;
		
	}
	if(ak == -1 || si == -1) return;
	if(ak == si+1) return;
	//�ƶ�
	dl=s_layer_blt[si];
	for(j=si;j>ak+1;j--)
	{
		s_layer_blt[j] = s_layer_blt[j-1];
	}
	s_layer_blt[ak+1] = dl;
    */
}
#endif


/*****************************************************************************/
//     Description : test all blend info
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void TestAllBlendInfo(
                            LCD_ID_E lcd_id,         //in
                            GUI_RECT_T align_rect,   //in
                            BOOLEAN* is_all_immutable_ptr,
                            BOOLEAN* is_all_valid_width_ptr
                            )
{
    UILAYER_STRUCT_T *layer = PNULL;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    uint32 i = 0;
    uint32 index = 0;
    uint32 test_size = 0;
    BOOLEAN is_all_immutable   = TRUE;
    BOOLEAN is_all_valid_width = TRUE;
    BOOLEAN is_exist_mainlayer = FALSE;

    for (i = 0; i < UILAYER_TOTAL_BLT_COUNT; i++)
    {
        GUI_RECT_T rect = {0};
        GUI_RECT_T layer_rect = {0};
        
        lcd_dev_info.lcd_id = lcd_id;
        lcd_dev_info.block_id = s_layer_blt[i].lcd_dev_info.block_id;
        
        layer = GetLayerPtr(&lcd_dev_info);
        if (PNULL == layer)
        {
            continue;
        }

        if (IsMainLayerHandle( &s_layer_blt[i].lcd_dev_info ))
        {
            is_exist_mainlayer = TRUE;
        }

        layer_rect = GetLayerRect(layer);
        
        if (!GUI_IntersectRect(&rect, align_rect, layer_rect))
        {
            continue;
        }
        
        //�ײ�Ҫ��layer_buf_ptr����Ϊ�գ�rect��ЧҲ�������ײ�
        if (PNULL != layer->buf_addr)
        {
            index++;

            if ( index > UILAYER_BLEND_MAX_NUM
                || 0 == layer->format )
            {
                is_all_immutable = FALSE;
            }


#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
            if ( UILAYER_MEM_FORMAT_DOUBLE == layer->format
                && PNULL == layer->raw2_layer_buf_ptr )
            {
                test_size += layer->layer_buf_size;
            }
 #endif           
            //����Ӳ������, ������ֱ��return
            if ( layer->width >= HARDWARE_LAYER_WIDTH_LIMIIT )
            {
                is_all_valid_width = is_all_immutable = FALSE;
                break;
            }
        }
    }

    if ( 0 != test_size//��ֹȫimutableҲ��ͬ��ˢ��
        && MMK_TestMemory( MMK_TEST_LOW_MEM, test_size ) )
    {
        is_all_immutable = FALSE;
    }

    if ( PNULL != is_all_immutable_ptr )
    {
        *is_all_immutable_ptr = is_all_immutable;
    }
    
    if ( PNULL != is_all_valid_width_ptr )
    {
        *is_all_valid_width_ptr = is_all_valid_width;
    }

    //�޷�����ˢ�����٣�����ʹ�õ�buffer
    if ( !is_all_immutable
        && is_exist_mainlayer)
    {
        FreeMainLayerBuffer2();
    }
}

// #ifndef UI_LAYER_SUPPORT_LOW_MEMORY
// extern void DISPLAY_RGB565TOARGB888(
//                                     const uint16         *src_img_ptr, 
//                                     const DISPLAY_RECT_T *src_rect_ptr, 
//                                     uint16               width, 
//                                     uint32               *dst_img_ptr
//                                     );
// #endif

// PUBLIC void DISPLAY_RGB565TOARGB888(uint16 * src_img_ptr, DISPLAY_RECT_T *src_rect_ptr, uint16 width, uint32 * dst_img_ptr)
// {
//     uint32 i, j;
//     uint32 src_clip_width = src_rect_ptr->right - src_rect_ptr->left + 1;
//     uint32 src_clip_height = src_rect_ptr->bottom -  src_rect_ptr->top + 1 ;
//     uint32 offset   = src_rect_ptr->top*width+src_rect_ptr->left;
//     uint16 *src_ptr = src_img_ptr + offset;
//     uint32 *dst_ptr = dst_img_ptr + offset;
//     uint32 data = 0;
//     uint16* temp_src = PNULL;
//     uint32* temp_dst = PNULL;
//     
//     for(i=0;i<src_clip_height; i++)
//     {
//         temp_src = src_ptr;
//         temp_dst = dst_ptr;
// 
//         for(j=0; j<src_clip_width; j++)
//         {
//             data = *temp_src++;
//             data =( (0xff&(data<<3))|(0xff00&(data<<5))|(0xff0000&(data<<8))|0xff000000);
//             *temp_dst++ = data;
//         }
// 
//         src_ptr += width;
//         dst_ptr += width;
//     }
// 
//     return; 
// }

/*****************************************************************************/
// Description : ������ʾʱ���Ƿ�565����תΪ888���ݵ�״̬
// Global resource dependence : 
// Author: Xiaoqing.Lu
// Note:
/*****************************************************************************/
//PUBLIC void UILAYER_SetDisplayRGB565TOARGB888State(BOOLEAN is_true)
//{
//    s_IsDisplayRGB565TOARGB888 = is_true;
//}

/*****************************************************************************/
// Description : �����ʾʱ���Ƿ�565����תΪ888���ݵ�״̬
// Global resource dependence : 
// Author: Xiaoqing.Lu
// Note:
/*****************************************************************************/
//PUBLIC BOOLEAN UILAYER_GetDisplayRGB565TOARGB888State(void)
//{
//    return s_IsDisplayRGB565TOARGB888;
//}

/*****************************************************************************/
//     Description : copy display blend layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CopyDisplayBlendLayer(
                                    DISPLAY_BLEND_LAYER_T *dst_layer,   //dst
                                    UILAYER_STRUCT_T *layer
                                    )
{
    if ((PNULL != dst_layer) && (PNULL != layer))
    {
        dst_layer->alpha_buf_size = 0;
// #ifdef UILAYER_NEED_ALIGN8
//         dst_layer->interval_left = layer->interval_left;
//         dst_layer->interval_right = layer->interval_right;
// #else
        dst_layer->interval_left = 0;
        dst_layer->interval_right = 0;
//#endif
        dst_layer->layer_info.layer_data.alpha = layer->alpha;
        dst_layer->layer_info.layer_data.alpha_sel = layer->alpha_sel;
        dst_layer->layer_info.layer_data.colorkey = layer->colorkey;
        dst_layer->layer_info.layer_data.colorkey_en = layer->colorkey_en;
//        if (DATA_TYPE_ARGB888 == layer->type)
//        {
//            dst_layer->layer_info.layer_data.alpha_sel = DISPLAY_PIXEL_ALPHA;    
//            dst_layer->layer_info.layer_data.alpha = MAX_ALPHA_VALUE;   //Ĭ��Ϊ��͸
//            dst_layer->layer_info.layer_data.colorkey_en = FALSE;
//        }
//        else
//        {
//            dst_layer->layer_info.layer_data.alpha_sel = DISPLAY_BLOCK_ALPHA;    
//            dst_layer->layer_info.layer_data.alpha = MAX_ALPHA_VALUE;   //Ĭ��Ϊ��͸
//            dst_layer->layer_info.layer_data.colorkey_en = FALSE;
//        }
        dst_layer->layer_buf_size = layer->layer_buf_size;
        dst_layer->layer_info.alpha_buf_addr = PNULL;
        dst_layer->layer_info.buf_addr = layer->buf_addr;
        dst_layer->layer_info.is_enable = layer->is_enable;
        dst_layer->layer_info.layer_id = 0;
        dst_layer->layer_info.uv_buf_addr = PNULL;
        dst_layer->layer_info.pos.x = layer->x;
        dst_layer->layer_info.pos.y = layer->y;
        dst_layer->layer_info.width = layer->width;
        dst_layer->layer_info.height = layer->height;
        dst_layer->layer_info.layer_data.grey_value = 0;
        dst_layer->layer_info.layer_data.immutable = layer->immutable;
        dst_layer->layer_info.layer_data.type = layer->type;
        dst_layer->layer_info.clip_rect = layer->clip_rect;

        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//     Description : active mainlayer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void ActiveMainLayer( void )
{
    GUI_LCD_DEV_INFO main_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    ActiveLayer(GetLayerPtrEx(&main_dev_info, TRUE));
}

/*****************************************************************************/
//     Description : deactive main layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void DeactiveMainLayer( void )
{
    GUI_LCD_DEV_INFO main_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
//    FreeMainLayerBuffer2();
    DeactiveLayer(GetLayerPtrEx(&main_dev_info, TRUE));
}

/*****************************************************************************/
//     Description : free main layer buffer2
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void FreeMainLayerBuffer2( void )
{
    GUI_LCD_DEV_INFO main_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    UILAYER_STRUCT_T *layer = GetLayerPtrEx(&main_dev_info, TRUE);
    
    if ( PNULL != layer
#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
        && PNULL != layer->raw2_layer_buf_ptr
#endif
       )
    {
#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
        //set layer->buf_addr
        if (layer->buf_addr == ALIGN_256BYTES_ADDR_RIGHT(layer->raw2_layer_buf_ptr))
        {
            if (PNULL != layer->raw_layer_buf_ptr)
            {
                layer->buf_addr = ALIGN_256BYTES_ADDR_RIGHT(layer->raw_layer_buf_ptr);
            }
            else
            {
                layer->buf_addr = PNULL;
            }
        }
#endif
        ProtectLayerBuf(layer);
#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
        FreeLayerBuf( &layer->mem_func, layer->raw2_layer_buf_ptr);
        layer->raw2_layer_buf_ptr = PNULL;
#endif
    }    
}

/*****************************************************************************/
//     Description : �������ͼ���˫����
//    Global resource dependence : 
//  Author:  James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_FreeAllLayerDoubleBuffer( void )
{
    //��ʱֻ�������
    FreeMainLayerBuffer2();
}

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
                                    )
{
    DISPLAY_RECT_T blt_rect = {0};
    GUI_RECT_T align_rect = {0};
    UILAYER_STRUCT_T *layer = PNULL;
    uint32 i = 0;
    uint32 index = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    BOOLEAN is_all_immutable   = TRUE;
    BOOLEAN is_all_valid_width = TRUE;

    s_blend_callback_num = 0;

    if ( PNULL == rect_ptr || PNULL == blend_ptr || LCD_ID_0 != lcd_id )
    {
        //SCI_TRACE_LOW:"UILAYER_GetBlendInfo() param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_4232_112_2_18_3_22_58_87,(uint8*)"");

        return FALSE;
    }
    
    align_rect = GetAlign8Rect(*rect_ptr);
    blt_rect = Convert2DispRect(align_rect);
    
    //protect double buffer
    GUILCD_GetMainLcdBufPtr();

    TestAllBlendInfo( lcd_id, align_rect, &is_all_immutable, &is_all_valid_width );

    if ( !is_all_valid_width )
    {
        return FALSE;
    }

    for (i = 0; i < UILAYER_TOTAL_BLT_COUNT; i++)
    {
        GUI_RECT_T rect = {0};
        GUI_RECT_T layer_rect = {0};
        
        lcd_dev_info.lcd_id = lcd_id;
        lcd_dev_info.block_id = s_layer_blt[i].lcd_dev_info.block_id;
        
        layer = GetLayerPtr(&lcd_dev_info);
        if (PNULL == layer)
        {
            continue;
        }
        layer_rect = GetLayerRect(layer);
        
        if (!GUI_IntersectRect(&rect, align_rect, layer_rect))
        {
            continue;
        }

        //�ײ�Ҫ��layer_buf_ptr����Ϊ�գ�rect��ЧҲ�������ײ�
        if (PNULL != layer->buf_addr)
        {
            CopyDisplayBlendLayer(&blend_ptr->src_layer[index], layer);
//            blend_ptr->src_layer[index] = layer->layer_struct;

            //��ȫ��immuable, ��������
            if ( is_all_immutable )
            {
                BOOLEAN is_immutable = FALSE;

                if ( layer->format & UILAYER_MEM_FORMAT_IMMUTABLE
					//���лص���Ҳ��ΪIMMUTABLE
					|| PNULL != layer->callback )
                {
                    is_immutable = layer->is_immutable_drawing = blend_ptr->src_layer[index].layer_info.layer_data.immutable = TRUE;
                }
                
                if ( ( layer->format & UILAYER_MEM_FORMAT_DOUBLE ) )
                {
#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
                    AllocLayerRaw2Buf( layer );

                    if ( PNULL != layer->raw2_layer_buf_ptr )
                    {
                        if ( !is_immutable )
                        {
                            uint8 *raw2_buf_ptr = PNULL;
                            uint32 blt_width  = rect.right - rect.left + 1;
                            uint32 blt_height = rect.bottom - rect.top + 1;
                            uint32 bytes_per_pixel = GetBytesPerPixel( layer->type );
                            uint32 mem_width  = layer->width * bytes_per_pixel;
                            uint32 offset_x   = (rect.left - layer_rect.left) * bytes_per_pixel; /*lint !e737*/
                            uint32 offset_y   = (rect.top - layer_rect.top) * mem_width; /*lint !e737*/
                            uint8* dst_ptr;
                            uint8* src_ptr;
                            uint32 size;
                            
                            raw2_buf_ptr = layer->raw2_layer_buf_ptr;
                            blend_ptr->src_layer[index].layer_info.buf_addr = (uint32)ALIGN_256BYTES_ADDR_RIGHT(raw2_buf_ptr);
                            
                            dst_ptr = (uint8*)ALIGN_256BYTES_ADDR_RIGHT(layer->raw2_layer_buf_ptr);
                            src_ptr = (uint8*)layer->buf_addr;
                            
                            if( layer->width > blt_width )
                            {
                                SCI_ASSERT( rect.left >= layer_rect.left && rect.top >= layer_rect.top ); /*assert verified*/
                                
                                dst_ptr += offset_x + offset_y;
                                src_ptr += offset_x + offset_y;
                                size     = blt_width * bytes_per_pixel;
                                
                                while(blt_height)
                                {
                                    SCI_MEMCPY(dst_ptr, src_ptr, size);
                                    
                                    src_ptr += mem_width;
                                    dst_ptr += mem_width;
                                    blt_height--;
                                } 
                            }
                            else
                            {
                                dst_ptr += offset_y;
                                src_ptr += offset_y;
                                size     = blt_height * mem_width;
                                
                                SCI_MEMCPY( dst_ptr, src_ptr, size );
                            }
                        }
                        
                        blend_ptr->src_layer[index].layer_info.layer_data.immutable = TRUE;
                    }
                    else
#endif
                    //���䲻���ڴ棬�����첽ˢ��
                    {
                        blend_ptr->src_layer[index].layer_info.layer_data.immutable = FALSE;
                        is_all_immutable = FALSE;
                    }
                }   
            }

            index++;
        }

        if ( PNULL != layer->callback )
        {
            s_blend_callback_array[s_blend_callback_num].lcd_dev_info = layer->lcd_dev_info;
            s_blend_callback_array[s_blend_callback_num].buf_addr = layer->buf_addr;
            s_blend_callback_num++;
        }
    }

    if ( s_blend_callback_num > 0 )
    {
        blend_ptr->callback = BlendCallback;
    }

    if (UILAYER_IsOsdLayerEnable())
    {
        layer = GetLayerPtr(&s_osd_layer);
        if (PNULL != layer)
        {
            CopyDisplayBlendLayer(&blend_ptr->dst_layer, layer);
//            blend_ptr->dst_layer = layer->layer_struct;
        }
        else
        {
            blend_ptr->dst_layer = *GetMainLcdhandle();
        }
    }
    else
    {
        blend_ptr->dst_layer = *GetMainLcdhandle();
    }
//    blend_ptr->dst_layer = *GetMainLcdhandle();
    blend_ptr->layer_num = index;
    blend_ptr->rect = blt_rect;
    
    if (0 == blend_ptr->layer_num)
    {
        return FALSE;
    }
    
//     for (i = 0; i < blend_ptr->layer_num; i++)
//     {             
//         if ( blend_ptr->src_layer[i].layer_info.width >= HARDWARE_LAYER_WIDTH_LIMIIT )
//         {
//             //��֧��ͼ���ȴ���Ӳ������
//             return FALSE;
//         }
//     }

    return TRUE;
}


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
                                    )
{
    uint32 i=0;
    UILAYER_STRUCT_T *layer=PNULL;
    uint32 index=0;
    DISPLAY_RECT_T blt_rect = {0};
    GUI_RECT_T align_rect = {0};

    align_rect = GetAlign8Rect(*rect_ptr);
    blt_rect = Convert2DispRect(align_rect);
    
    for(i=0; i<num_layers; i++)
    {
        layer = GetLayerPtr(&((blt_layers+i)->lcd_dev_info));
        if(PNULL == layer)
        {
            continue;
        }
        if(PNULL != layer->buf_addr)
        {
            CopyDisplayBlendLayer(&(blend_ptr->src_layer[index]), layer);
            index++;
        }
    }
    blend_ptr->layer_num = index;
    layer = GetLayerPtr(dst_layer);
    CopyDisplayBlendLayer(&blend_ptr->dst_layer, layer);
    blend_ptr->rect = blt_rect;
    return TRUE;
}

/*****************************************************************************/
//  Description: clip rect
//  Global resource dependence:
//  Author: 
//  Note:
/*****************************************************************************/
// LOCAL uint32 ClipDisplayRect(DISPLAY_RECT_T *dst, DISPLAY_RECT_T*src)
// {
//     if(dst->left > src->right || dst->right < src->left ||
//         dst->top > src->bottom || dst->bottom < src->top)
//             return 0;
//     /* They must overlap */
//     if(dst->left < src->left)
//             dst->left = src->left;
//     if(dst->top < src->top)
//             dst->top = src->top;
//     if(dst->right > src->right)
//             dst->right = src->right;
//     if(dst->bottom > src->bottom)
//             dst->bottom = src->bottom;
//     return 1;
// }

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
                                          )
{
#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
    UILAYER_STRUCT_T *ui_layer = GetLayerPtr( MMITHEME_GetDefaultLcdDev() );
    DISPLAY_LAYER_INFO_T *layer_info_ptr;
    uint32 size,width,height;
    uint32 offset; 
    //uint32 start_x,start_y; 
    uint32 mem_width;
    //DISPLAY_RECT_T dst_rect;
    uint32 buf_addr;
    uint8 *dst_base;
    uint8 *src_base;
    uint32 bytes_per_pixel;
    uint32 dst_bytes_per_pixel;
    uint16 logic_lcd_width = 0;
    uint16 logic_lcd_height = 0;
    GUI_RECT_T rect = {0};

    //check param
    if ( LCD_ID_0 != lcd_id
        || PNULL == param_ptr 
        || 1 != param_ptr->layer_num
        || PNULL == param_ptr->layer_arr
        || PNULL == ( layer_info_ptr = param_ptr->layer_arr[0] )
        || PNULL == ui_layer
        //|| PNULL == ui_layer->raw2_layer_buf_ptr
        || PNULL == param_ptr->param_ptr
        || PNULL == rect_ptr
        /*|| DISPLAY_ANGLE_0 != param_ptr->param_ptr->rot_angle*/ )
    {
        return FALSE;
    }
    
//     dst_rect.left   = layer_info_ptr->pos.x;
//     dst_rect.top    = layer_info_ptr->pos.y;
//     dst_rect.right  = layer_info_ptr->pos.x + layer_info_ptr->width - 1;
//     dst_rect.bottom = layer_info_ptr->pos.y + layer_info_ptr->height - 1;
//     
//     //check rect
//     if ( 0 == ClipDisplayRect( &dst_rect, &param_ptr->param_ptr->rect ) )
//     { 
//         return FALSE;
//     }  
    
    //get logic lcd width and height
    GUILCD_GetLogicWidthHeight(lcd_id,
        &logic_lcd_width,
        &logic_lcd_height);
    
    //adjust rect
    rect.right  = logic_lcd_width - 1;
    rect.bottom = logic_lcd_height - 1;

    if (!GUI_IntersectRect(&rect, *rect_ptr, rect))
    {
        return FALSE;
    }

    bytes_per_pixel = GetBytesPerPixel( layer_info_ptr->layer_data.type );
//    dst_bytes_per_pixel = GetBytesPerPixel( ui_layer->layer_struct.layer_info.layer_data.type );
    dst_bytes_per_pixel = GetBytesPerPixel( ui_layer->type );

    //check rect
    if ( layer_info_ptr->width * layer_info_ptr->height * bytes_per_pixel >
        ui_layer->width *ui_layer->height * dst_bytes_per_pixel )
    {
        return FALSE;
    }

    AllocLayerRaw2Buf( ui_layer );

    if ( PNULL == ui_layer->raw2_layer_buf_ptr )
    {
        return FALSE;
    }

    buf_addr = ALIGN_256BYTES_ADDR_RIGHT( ui_layer->raw2_layer_buf_ptr );

    //ʹ���߼���������������꣬��������첽ˢ����������
    width  = rect.right - rect.left + 1;//dst_rect.right - dst_rect.left + 1;
    height = rect.bottom - rect.top + 1;//dst_rect.bottom - dst_rect.top + 1;
    
    //start_x    = rect_ptr->left;//dst_rect.left - layer_info_ptr->pos.x;
    //start_y    = rect_ptr->top;//dst_rect.top - layer_info_ptr->pos.y;
    offset     = (uint32)( rect.top * logic_lcd_width/*layer_info_ptr->width*/ + rect.left ) * bytes_per_pixel;
    dst_base   = (uint8*)(buf_addr + offset);
    src_base   = (uint8*)(layer_info_ptr->buf_addr + offset);
    
    //protect double buffer
    GUILCD_GetMainLcdBufPtr();

    //copy invalidate param to double buffer
    if ( width == logic_lcd_width/*layer_info_ptr->width*/ )
    { 
        size = width * height * bytes_per_pixel;
        
        SCI_MEMCPY( dst_base, src_base, size );
    }
    else
    {
        size = width * bytes_per_pixel;
        mem_width = logic_lcd_width/*layer_info_ptr->width*/ * bytes_per_pixel;
        
        while ( height )
        {
            SCI_MEMCPY( dst_base, src_base, size );
            
            dst_base += mem_width;
            src_base += mem_width;
            height--;
        }  
    }
    
    layer_info_ptr->buf_addr = buf_addr;
    layer_info_ptr->layer_data.immutable = TRUE;
    
    return TRUE;
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//     Description : ��������mem format
//    Global resource dependence : 
//  Author:  bin.ji
//    Note: 
/*****************************************************************************/
PUBLIC void UILAYER_SetMainLayerMemFormat(
                                    UILAYER_MEM_FORMAT_T type
                                    )
{
    GUI_LCD_DEV_INFO main_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    SCI_TRACE_LOW("UILAYER_SetMainLayerMemFormat() type = %d", type);
    SetLayerMemFormat(&main_dev_info, type);
}

/*****************************************************************************/
//     Description : ����layer���ݲ���
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: �����Ż�ר�ã��������ݲ��䣬�ײ��첽ˢ���ɽ�ʡ���������ݵ�ʱ��
/*****************************************************************************/
PUBLIC void UILAYER_SetLayerMemFormat(
                                    GUI_LCD_DEV_INFO const *dev_info_ptr,
                                    UILAYER_MEM_FORMAT_T type
                                    )
{
    if (IsMainLayerHandle(dev_info_ptr))
    {
        //��ֹ���㱻�����
        return;
    }
    SetLayerMemFormat(dev_info_ptr, type);
}

/*****************************************************************************/
//     Description : ����layer���ݲ���
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: �����Ż�ר�ã��������ݲ��䣬�ײ��첽ˢ���ɽ�ʡ���������ݵ�ʱ��
/*****************************************************************************/
LOCAL void SetLayerMemFormat(
                             GUI_LCD_DEV_INFO const *dev_info_ptr,
                             UILAYER_MEM_FORMAT_T type
                             )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }

    if ( PNULL != layer 
        && type != layer->format )
    {
        ProtectLayerBuf(layer);

        //����old type
#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
        if ( ( layer->format & UILAYER_MEM_FORMAT_DOUBLE ) && !( type & UILAYER_MEM_FORMAT_DOUBLE ) )
        {
            //�ͷ�˫����
            FreeLayerBuf( &layer->mem_func, layer->raw2_layer_buf_ptr);
            layer->raw2_layer_buf_ptr = PNULL;
        }
        layer->is_raw2 = FALSE;
        layer->is_switch_double_buffer = FALSE;
#endif
        layer->format = type;
        //layer->layer_struct.layer_info.layer_data.immutable = ( layer->format & UILAYER_MEM_FORMAT_IMMUTABLE ) ? TRUE : FALSE;
    }
    else
    {
        //SCI_TRACE_LOW:"UILAYER_SetLayerMemFormat() do nothing"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_4594_112_2_18_3_22_59_88,(uint8*)"");
    }
}

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: 
/*****************************************************************************/
PUBLIC UILAYER_MEM_FORMAT_T UILAYER_GetLayerMemFormat(
                                                      GUI_LCD_DEV_INFO const *dev_info_ptr
                                                      )
{
    UILAYER_MEM_FORMAT_T type  = 0;
    UILAYER_STRUCT_T*    layer = GetLayerPtr(dev_info_ptr);
    
    if ( CheckLayerId(dev_info_ptr) )
    {
        if ( PNULL != layer )
        {
            type = layer->format;
#ifdef UI_LAYER_SUPPORT_LOW_MEMORY
            //lowmem�汾ȥ��˫buffer����
            layer->format &= ~UILAYER_MEM_FORMAT_DOUBLE;
#endif
        }
        else
        {
            //SCI_TRACE_LOW:"UILAYER_GetLayerMemFormat() do nothing"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_4621_112_2_18_3_22_59_89,(uint8*)"");
        }
    }

    return type;
}
#ifndef UI_LAYER_SUPPORT_LOW_MEMORY

/*****************************************************************************/
//     Description : ����˫����
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:�����ڴ�ˮ��ʱ,�򲻷���˫����
/*****************************************************************************/
LOCAL void AllocLayerRaw2Buf(
                             UILAYER_STRUCT_T *layer
                             )
{
    uint32 file_line = 0;

    if ( PNULL != layer )
    {
        if ( PNULL == layer->raw2_layer_buf_ptr )
        {
            if ( !MMK_TestMemory( MMK_TEST_LOW_MEM, layer->layer_buf_size ) )
            {
                if (_D_FILE_NAME == layer->file_name_ptr)
                {
                    //����ui_layer.c��˫buffer
                    file_line = _D_FILE_LINE;
                }
                else
                {
                    file_line = layer->file_line;
                }                
                layer->raw2_layer_buf_ptr = AllocLayerBuf( &layer->mem_func, layer->layer_buf_size, layer->file_name_ptr, file_line/*layer->file_line*/);        
            }
        }
    }
}
#endif

/*****************************************************************************/
//     Description : �л�double mem
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: �����Ż�ר�ã��л�double mem�����ٿ������Ƽ��ڻ���ʱˢ����������ʱʹ��
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_SwitchDoubleMem(
                                       GUI_LCD_DEV_INFO const *dev_info_ptr,
                                       BOOLEAN is_switch
                                       )
{
    BOOLEAN result = FALSE;

#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    
    if ( !UILAYER_IsLayerActive( dev_info_ptr )
        || IsMainLayerHandle( dev_info_ptr ) )
    {
        return FALSE;
    }
    
    if ( PNULL != layer
        && ( layer->format & UILAYER_MEM_FORMAT_DOUBLE ) )
    {
        if ( is_switch )
        {
            //��ֹ���ڴ�ʱ�������ͷź�������ڴ����Ķ������Ứ��
            //����ֻ�ڵ�һ�η����ڴ�
            if ( !layer->is_switch_double_buffer )
            {
                AllocLayerRaw2Buf( layer );
            }
            
            if ( PNULL != layer->raw2_layer_buf_ptr )
            {
                //��һ����Ҫдraw1����Ϊ˫���忽��ʱ��raw2���ڱ�ˢ��ʹ��
                if ( layer->is_raw2 )
                {
                    layer->buf_addr = (uint32)ALIGN_256BYTES_ADDR_RIGHT(layer->raw2_layer_buf_ptr);
                }
                else
                {
                    layer->buf_addr = (uint32)ALIGN_256BYTES_ADDR_RIGHT(layer->raw_layer_buf_ptr);
                }
                
                layer->format |= UILAYER_MEM_FORMAT_IMMUTABLE;
                layer->is_immutable_drawing = FALSE;
                layer->is_raw2 = !layer->is_raw2;

                result = TRUE;
            } 
            
            layer->is_switch_double_buffer = TRUE;
        }
        else
        {
            //protect double buffer
            GUILCD_GetMainLcdBufPtr();

            layer->buf_addr = (uint32)ALIGN_256BYTES_ADDR_RIGHT(layer->raw_layer_buf_ptr);
            layer->is_raw2 = FALSE;
            layer->is_switch_double_buffer = FALSE;
            
            layer->format &= ~UILAYER_MEM_FORMAT_IMMUTABLE;

            result = TRUE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"UILAYER_SwitchDoubleMem() do nothing"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_4693_112_2_18_3_22_59_90,(uint8*)"");
    }
#endif

    return result;
}

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
                                          )
{
    BOOLEAN result = FALSE;

#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    
    if ( !UILAYER_IsLayerActive( dev_info_ptr )
        //����Ҳ�����д������ٶ�
        /*|| IsMainLayerHandle( dev_info_ptr )*/ )
    {
        return FALSE;
    }
    
    if ( PNULL != src_rect_ptr
        && PNULL != dst_rect_ptr
        && PNULL != layer
        && ( layer->format & UILAYER_MEM_FORMAT_DOUBLE )
        && PNULL != layer->raw2_layer_buf_ptr )
    {
        GUI_RECT_T layer_rect = GetLayerRect(layer);
        GUI_RECT_T src_rect = {0};
        GUI_RECT_T dst_rect = {0};
        uint8 *src_buf = PNULL;
        uint8 *dst_buf = PNULL;
        uint32 src_blt_width = 0;
        uint32 src_blt_height = 0;
        uint32 dst_blt_width = 0;
        uint32 dst_blt_height = 0;
        int32  src_x = 0;
        int32  src_y = 0;
        
        if (!GUI_IntersectRect(&src_rect, *src_rect_ptr, layer_rect))
        {
            return FALSE;
        }
        
        if (!GUI_IntersectRect(&dst_rect, *dst_rect_ptr, layer_rect))
        {
            return FALSE;
        }

        if ( PNULL != clip_rect_ptr )
        {
            if (!GUI_IntersectRect(&dst_rect, *clip_rect_ptr, dst_rect))
            {
                return FALSE;
            }
        }
        
        if ( layer->format & UILAYER_MEM_FORMAT_IMMUTABLE )
        {
            if ( layer->is_raw2 )
            {
                src_buf = (uint8*)ALIGN_256BYTES_ADDR_RIGHT(layer->raw2_layer_buf_ptr);
                dst_buf = (uint8*)ALIGN_256BYTES_ADDR_RIGHT(layer->raw_layer_buf_ptr);
            }
            else
            {
                src_buf = (uint8*)ALIGN_256BYTES_ADDR_RIGHT(layer->raw_layer_buf_ptr);
                dst_buf = (uint8*)ALIGN_256BYTES_ADDR_RIGHT(layer->raw2_layer_buf_ptr);
            }
        }
        else
        {
            src_buf = (uint8*)ALIGN_256BYTES_ADDR_RIGHT(layer->raw2_layer_buf_ptr);
            dst_buf = (uint8*)ALIGN_256BYTES_ADDR_RIGHT(layer->raw_layer_buf_ptr);
        }

        src_blt_width  = src_rect.right - src_rect.left + 1;
        src_blt_height = src_rect.bottom - src_rect.top + 1;

        dst_blt_width  = dst_rect.right - dst_rect.left + 1;
        dst_blt_height = dst_rect.bottom - dst_rect.top + 1;   
        
        src_blt_width  = MIN( src_blt_width, dst_blt_width );
        src_blt_height = MIN( src_blt_height, dst_blt_height );

        src_x = src_rect.left - layer_rect.left + dst_rect.left - dst_rect_ptr->left;
        src_y = src_rect.top - layer_rect.top + dst_rect.top - dst_rect_ptr->top;

        if ( src_x >= 0
            && src_y >= 0
            && src_x < layer->width
            && src_y < layer->height )
        {
            src_buf += (( src_x + src_y * layer->width ) * GetBytesPerPixel(layer->type));   /*lint !e737*/
            dst_buf += (((dst_rect.left - layer_rect.left) + (dst_rect.top - layer_rect.top) * layer->width) * GetBytesPerPixel(layer->type));   /*lint !e737*/
            
            BltBuffer(src_buf, layer->width, layer->type, dst_buf, layer->width, layer->type, src_blt_width, src_blt_height);
            
            //store update rect
            ConvertLCD2LayerRect(layer, &layer_rect, &layer_rect);
            MMITHEME_StoreUpdateRect(dev_info_ptr, layer_rect);   
            
            result = TRUE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"UILAYER_DoubleMemWriteBack() do nothing"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_4808_112_2_18_3_22_59_91,(uint8*)"");
    }
#endif

    return result;
}

/*****************************************************************************/
//     Description : �����������ݹ̶�
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: �����Ż�ר�ã��ٶ��������ݹ̶����ٽ���˫���忽�����Ƽ��ڻ���ʱʱʹ��
/*****************************************************************************/
PUBLIC void UILAYER_SetMainLayerImmutable(
                                          BOOLEAN is_immutable
                                          )
{
    GUI_LCD_DEV_INFO lcd_dev = {0};
    UILAYER_STRUCT_T *layer = GetLayerPtr(&lcd_dev);
    
    if ( IsMainLayerHandle(&lcd_dev) && CheckLayerId(&lcd_dev) )
    {
        if ( PNULL != layer )
        {
            if ( is_immutable )
            {
                layer->format |= UILAYER_MEM_FORMAT_IMMUTABLE;
            }
            else
            {
                layer->format &= ~UILAYER_MEM_FORMAT_IMMUTABLE;
            }
        }
    }
}

/*****************************************************************************/
//     Description : direct draw
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: �����Ż�ר�ã�����Ӳ�����ԣ��ϲ���ֱ��ˢ��lcdӲ����
//          �����ںϲ��������lcd buffer���������������ʾ����ȷ������ʹ��
/*****************************************************************************/
PUBLIC void UILAYER_SetDirectDraw(
                                  BOOLEAN is_direct
                                  )
{
    //��¼��һ֡�Ƿ�direct draw
    if ( !is_direct )
    {
        s_is_pre_direct_draw = !s_mainlcd_handle.layer_info.is_enable;
    }

    s_mainlcd_handle.layer_info.is_enable = !is_direct;
}

/*****************************************************************************/
//     Description : is pre direct draw
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: �����Ż�ר�ã�����Ӳ�����ԣ��ϲ���ֱ��ˢ��lcdӲ����
//          �ж���һ֡�Ƿ�Ϊdricet draw
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsPerDirectDraw( void )
{
    return s_is_pre_direct_draw;
}

/*****************************************************************************/
//     Description : ��ò���ϳɵ�ͼ����Ŀ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC uint32 UILAYER_GetBltNum(
                                LCD_ID_E lcd_id,    //in
                                const GUI_RECT_T *rect_ptr   //in
                                )
{
    DISPLAY_RECT_T blt_rect = {0};
    GUI_RECT_T align_rect = {0};
    UILAYER_STRUCT_T *layer = PNULL;
    uint32 i = 0;
    uint32 index = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    uint32 layer_num = 0;

    if ( PNULL == rect_ptr || LCD_ID_0 != lcd_id )
    {
        //SCI_TRACE_LOW:"UILAYER_GetBltNum() param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_4889_112_2_18_3_22_59_92,(uint8*)"");
        return 0;
    }
    
    align_rect = GetAlign8Rect(*rect_ptr);
    blt_rect = Convert2DispRect(align_rect);
    
    for (i = 0; i < UILAYER_TOTAL_BLT_COUNT; i++)
    {
        GUI_RECT_T rect = {0};
        GUI_RECT_T layer_rect = {0};
        
        lcd_dev_info.lcd_id = lcd_id;
        lcd_dev_info.block_id = s_layer_blt[i].lcd_dev_info.block_id;
        
        layer = GetLayerPtr(&lcd_dev_info);
        if (PNULL == layer)
        {
            continue;
        }
        layer_rect = GetLayerRect(layer);
        
        if (!GUI_IntersectRect(&rect, align_rect, layer_rect))
        {
            continue;
        }
        
        //�ײ�Ҫ��layer_buf_ptr����Ϊ�գ�rect��ЧҲ�������ײ�
        if (PNULL != layer->buf_addr)
        {
            if ( layer->width >= HARDWARE_LAYER_WIDTH_LIMIIT )
            {
                //��֧��ͼ���ȴ���Ӳ������
                return 0;
            }
            index++;
        }
    }
    layer_num = index;
    return layer_num;
}

/*****************************************************************************/
//     Description : ת����������ָ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
//LOCAL uint8 *GetLayerClipBufPtr(
//                                uint8 *buf_ptr,
//                                UILAYER_STRUCT_T const *layer
//                                )
//{
//    uint8 *out_buf_ptr = PNULL;
//    uint32 bytes_per_pixel = PNULL;
//    int32 mem_width  = 0;
//
//    if (PNULL != layer)
//    {
//        if (layer->is_clip_enable && (layer->clip_rect.left >= 0) && (layer->clip_rect.top >= 0))
//        {
//            bytes_per_pixel = GetBytesPerPixel( layer->layer_struct.layer_info.layer_data.type );
//            mem_width  = layer->layer_struct.layer_info.width * bytes_per_pixel;        
//            out_buf_ptr = buf_ptr + mem_width * layer->clip_rect.top + layer->clip_rect.left;
//        }
//        else
//        {
//            out_buf_ptr = buf_ptr;
//        }
//    }
//    else
//    {
//        out_buf_ptr = buf_ptr;
//    }
//    return out_buf_ptr;
//}

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
//                                )
//{
//    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
//    GUI_RECT_T rect = {0};
//    GUI_RECT_T layer_rect = {0};
//
//    if (IsMainLayerHandle(dev_info_ptr))
//    {
//        //��ֹ���㱻�����
//        return;
//    }
//    if (!CheckLayerId(dev_info_ptr))
//    {
//        return;
//    }
//    if (PNULL != layer)
//    {
//        if (is_clip_enable)
//        {
//            if (PNULL != rect_ptr)
//            {
//                layer_rect.left = 0;
//                layer_rect.top = 0;
//                layer_rect.right = layer->layer_struct.layer_info.width-1;
//                layer_rect.bottom = layer->layer_struct.layer_info.height-1;
//                if (GUI_IntersectRect(&rect, *rect_ptr, layer_rect))
//                {
//                    //Ŀǰ��֧�ָı���
//                    if ((layer_rect.left == rect.left) && (layer_rect.right == rect.right))
//                    {
//                        layer->is_clip_enable = TRUE;
//                        layer->clip_rect = rect;
//                    }
//                    else
//                    {
//                        SCI_TRACE_LOW("UILAYER_SetClipRect() not support change width!");
//                    }
//                }
//                else
//                {
//                    SCI_TRACE_LOW("UILAYER_SetClipRect() not valid rect!");
//                }
//            }
//        }
//        else
//        {
//            layer->is_clip_enable = FALSE;
//        }
//    }
//}

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
                               )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    GUI_RECT_T rect = {0};
    GUI_RECT_T layer_rect = {0};
    DISPLAY_DATA_TYPE_E src_data_type = 0;
    DISPLAY_DATA_TYPE_E dst_data_type = 0;
    uint8 *src_buf = PNULL;
    uint8 *dst_buf = PNULL;
    uint32 blt_width = 0;
    uint32 blt_height = 0;
    uint32 src_bytes_per_pixel = 0;
    uint32 dst_bytes_per_pixel = 0;
    uint32 src_width = 0;
    uint32 dst_width = 0;

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }
    if ((PNULL == param_ptr) || (PNULL == copybuf_ptr))
    {
        return;
    }
    if (!UILAYER_IsLayerActive(dev_info_ptr))
    {
        //SCI_TRACE_LOW:"UILAYER_CopyLayerToBuffer() dev_info_ptr(%d,%d) not active!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_5057_112_2_18_3_23_0_93,(uint8*)"dd", dev_info_ptr->lcd_id, dev_info_ptr->block_id);
        //δactive��ֱ�ӷ���
        return;
    }
    if (PNULL != layer)
    {
        rect = param_ptr->rect;
        layer_rect = GetLayerRect(layer);
        
        if (!GUI_IntersectRect(&rect, rect, layer_rect))
        {
            return;
        }
        
        src_buf = (uint8*)layer->buf_addr;
        src_width = layer->width;
        src_data_type = layer->type;
    
        dst_buf = (uint8*)copybuf_ptr->buf_ptr;
        dst_width = copybuf_ptr->width;
        dst_data_type = copybuf_ptr->data_type;

        if ((PNULL != src_buf) && (PNULL != dst_buf))
        {
            blt_width  = rect.right - rect.left + 1;
            blt_height = rect.bottom - rect.top + 1;   
    
            src_bytes_per_pixel = GetBytesPerPixel(src_data_type);
            dst_bytes_per_pixel = GetBytesPerPixel(dst_data_type);
            if ((rect.left < layer_rect.left) || (rect.left < layer_rect.left) 
                || (rect.top < layer_rect.top)|| (rect.top < layer_rect.top))
            {
                return;
            }
            src_buf += (((rect.left-layer_rect.left) + (rect.top-layer_rect.top)*src_width)*src_bytes_per_pixel);   /*lint !e737*/

            BltBuffer(src_buf, src_width, src_data_type, dst_buf, dst_width, dst_data_type, blt_width, blt_height);          
        }
        else
        {
            //SCI_TRACE_LOW:"UILAYER_CopyLayerToBuffer() src_buf = 0x%x, dst_buf = 0x%x!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_5097_112_2_18_3_23_0_94,(uint8*)"dd", src_buf, dst_buf);
        }
    }
}

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
                               )
{
    UILAYER_STRUCT_T *layer = GetLayerPtrEx(dev_info_ptr, TRUE);
    GUI_RECT_T rect = {0};
    GUI_RECT_T layer_rect = {0};
    DISPLAY_DATA_TYPE_E src_data_type = 0;
    DISPLAY_DATA_TYPE_E dst_data_type = 0;
    uint8 *src_buf = PNULL;
    uint8 *dst_buf = PNULL;
    uint32 blt_width = 0;
    uint32 blt_height = 0;
    uint32 src_bytes_per_pixel = 0;
    uint32 dst_bytes_per_pixel = 0;
    uint32 src_width = 0;
    uint32 dst_width = 0;

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }
    if ((PNULL == param_ptr) || (PNULL == copybuf_ptr))
    {
        return;
    }
    if (!UILAYER_IsLayerActive(dev_info_ptr))
    {
        //SCI_TRACE_LOW:"UILAYER_CopyBufferToLayer() dev_info_ptr(%d,%d) not active!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_5136_112_2_18_3_23_0_95,(uint8*)"dd", dev_info_ptr->lcd_id, dev_info_ptr->block_id);
        //δactive��ֱ�ӷ���
        return;
    }
    if (PNULL != layer)
    {
        rect = param_ptr->rect;
        layer_rect = GetLayerRect(layer);
        if (!GUI_IntersectRect(&rect, rect, layer_rect))
        {
            return;
        }

        //�ü�����
        if ( PNULL != param_ptr->clip_rect_ptr )
        {
            if (!GUI_IntersectRect(&rect, rect, *param_ptr->clip_rect_ptr))
            {
                return;
            }
        }
        
        dst_buf = (uint8*)layer->buf_addr;
        dst_width = layer->width;
        dst_data_type = layer->type;
    
        src_buf = (uint8*)copybuf_ptr->buf_ptr;
        src_width = copybuf_ptr->width;
        src_data_type = copybuf_ptr->data_type;

        if ((PNULL != src_buf) && (PNULL != dst_buf))
        {
            blt_width  = rect.right - rect.left + 1;
            blt_height = rect.bottom - rect.top + 1;

            blt_width = MIN(blt_width, copybuf_ptr->width);
            blt_height = MIN(blt_height, copybuf_ptr->height);
    
            src_bytes_per_pixel = GetBytesPerPixel(src_data_type);
            dst_bytes_per_pixel = GetBytesPerPixel(dst_data_type);
            if ((rect.left < layer_rect.left) || (rect.left < layer_rect.left) 
                || (rect.top < layer_rect.top)|| (rect.top < layer_rect.top))
            {
                return;
            }
            dst_buf += (((rect.left-layer_rect.left) + (rect.top-layer_rect.top)*dst_width)*dst_bytes_per_pixel);   /*lint !e737*/
            src_buf += (((rect.left-param_ptr->rect.left) + (rect.top-param_ptr->rect.top)*src_width)*src_bytes_per_pixel);   /*lint !e737*/

            BltBuffer(src_buf, src_width, src_data_type, dst_buf, dst_width, dst_data_type, blt_width, blt_height);          

            //store update rect
            ConvertLCD2LayerRect(layer, &rect, &rect);
            MMITHEME_StoreUpdateRect(dev_info_ptr, rect);   
        }
        else
        {
            //SCI_TRACE_LOW:"UILAYER_CopyBufferToLayer() src_buf = 0x%x, dst_buf = 0x%x!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_5192_112_2_18_3_23_0_96,(uint8*)"dd", src_buf, dst_buf);
        }
    }    
}

/*****************************************************************************/
//     Description : ��������lcd_buffer��osd layer�Ƿ���
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsOsdLayerEnable(void)
{
    GUI_LCD_DEV_INFO *dev_info_ptr = &s_osd_layer;
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    
    if (IsMainLayerHandle(dev_info_ptr))
    {
        //��ֹ���㱻�����
        return FALSE;
    }
    if (!CheckLayerId(dev_info_ptr))
    {
        return FALSE;
    }      
    if ((PNULL != layer) && layer->is_enable && (PNULL != layer->buf_addr))
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//     Description : ����/�ر���������lcd_buffer��osd layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_EnableOsdLayer(
                                   BOOLEAN is_enable
                                   )
{
    if (is_enable)
    {
        CreateOsdLayer();
    }
    else
    {
        DestroyOsdLayer();
    }
}

/*****************************************************************************/
//  Description : destroy osd layer
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void DestroyOsdLayer(void)
{
    UILAYER_ReleaseLayer(&(s_osd_layer));
    SCI_MEMSET(&s_osd_layer, 0x00, sizeof(GUI_LCD_DEV_INFO));
}

/*****************************************************************************/
//  Description : create osd layer
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void CreateOsdLayer(void)
{
    GUI_LCD_DEV_INFO layer = {0};
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    UILAYER_CREATE_T create_info = {0};  

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    if ((UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_osd_layer))
        && UILAYER_IsLayerActive(&s_osd_layer))    
    {
        //�Ѿ�����
        return;
    }
    else
    {       
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = 0;//MMIDC_MAIN_WIN_ID;//win_id;
        create_info.offset_x        = 0;//main_rect.left;
        create_info.offset_y        = 0;//main_rect.top;
        create_info.width           = lcd_width;//(main_rect.right+1-main_rect.left);
        create_info.height          = lcd_height;//(main_rect.bottom+1-main_rect.top);
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = TRUE;//FALSE;   
        create_info.format = UILAYER_MEM_IMMUTABLE;
        
        UILAYER_CreateLayerEx(&create_info, &layer, _D_FILE_NAME, _D_FILE_LINE);        
        s_osd_layer = layer;
//        UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
//        widget_ctrl_ptr->softkey_layer = widget_layer_handle;
        if (UILAYER_IsLayerActive(&layer))
        {
            UILAYER_Clear(&layer);
        }
    }    
}
#if defined(MENU_CRYSTALCUBE_SUPPORT) || defined (MENU_CYCLONE_SUPPORT) || defined (MET_MEX_SUPPORT)
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
                                   )
{
    UILAYER_STRUCT_T *layer = PNULL;

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }
    if (PNULL == new_buf_addr)
    {
        return;
    }
    if (PNULL == new_raw_buf_addr)
    {
        return;
    }
    layer = GetLayerPtr(dev_info_ptr);

// #ifdef UILAYER_NEED_ALIGN8
//     SCI_PASSERT(layer->interval_left == 0, ("interval_left %d", layer->interval_left));/*assert verified*/
// #endif

//����BUFFER
    if (PNULL != layer
        &&PNULL != layer->buf_addr
        &&PNULL != layer->raw_layer_buf_ptr
        )
    {
        if (old_buf_addr != PNULL)
            *old_buf_addr = (uint8*)layer->buf_addr;
        if (old_raw_buf_addr != PNULL)
        *old_raw_buf_addr = layer->raw_layer_buf_ptr;
        //SCI_ASSERT(0 == layer->raw_layer_buf_ptr);
        layer->raw_layer_buf_ptr = new_raw_buf_addr;
        layer->buf_addr = (uint32)new_buf_addr;
    }
    else
    {
        //SCI_TRACE_LOW:"UILAYER_SetLayerBufferPtr() PNULL == layer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_5339_112_2_18_3_23_0_97,(uint8*)"");
    }
}
/*****************************************************************************/
//  Description : get raw layer buffer ptr
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/

PUBLIC uint8* UILAYER_GetLayerRawBufferPtr(GUI_LCD_DEV_INFO const *dev_info_ptr)
{
    UILAYER_STRUCT_T *layer = PNULL;
    if (!CheckLayerId(dev_info_ptr))
    {
        return PNULL;
    }
    layer = GetLayerPtr(dev_info_ptr);
// #ifdef UILAYER_NEED_ALIGN8
//     SCI_PASSERT(layer->interval_left == 0, ("interval_left %d", layer->interval_left));/*assert verified*/
// #endif
    return layer->raw_layer_buf_ptr;
}
#endif
#if defined(HERO_ENGINE_SUPPORT )
UILAYER_RESULT_E hr_layer_create_ex(
                                    const UILAYER_CREATE_T *layer_ptr, 
                                    int32 *handle_ptr,
                                    uint8 *buffer_ptr,
                                    int32 buf_size
                                    )
{	
    UILAYER_STRUCT_T *layer = NULL;
    GUI_LCD_DEV_INFO dev_info = {0};
	DISPLAY_BLEND_LAYER_T blend_layer = {0};
	UILAYER_RESULT_E result = UILAYER_RESULT_SUCCESS;
    
    if (PNULL == layer_ptr)
    {
        return UILAYER_RESULT_ERROR;
    }

    if (GUI_MAIN_LCD_ID != layer_ptr->lcd_id)
    {
        //Ŀǰֻ֧��GUI_MAIN_LCD_ID�ϴ����²�
        return UILAYER_RESULT_ERROR;
    }
    
    if (FindValidHandle((UILAYER_HANDLE_T*)handle_ptr))
    {
        SCI_MEMSET(&blend_layer, 0x00, sizeof(DISPLAY_BLEND_LAYER_T));
        SetBlendDefaultData(&blend_layer);        
        //ת��offset_x, offset_y      
#ifdef UILAYER_NEED_ALIGN8
        blend_layer.interval_left = layer_ptr->offset_x&7;
        blend_layer.interval_right = 16-blend_layer.interval_left-(layer_ptr->width%8);   //���Ҹ���8
#else
        blend_layer.interval_left = 0;
        blend_layer.interval_right = 0;
#endif        
        blend_layer.layer_info.pos.x = layer_ptr->offset_x-blend_layer.interval_left;  //8����
        blend_layer.layer_info.pos.y = layer_ptr->offset_y;
        blend_layer.layer_info.width = blend_layer.interval_left+blend_layer.interval_right+layer_ptr->width;   
        blend_layer.layer_info.height = layer_ptr->height;

        if (IsMultiLayerEnable())
        {         
            blend_layer.layer_info.layer_data.type = DATA_TYPE_RGB565;
            blend_layer.layer_info.layer_data.alpha_sel = DISPLAY_BLOCK_ALPHA;
            dev_info.lcd_id = layer_ptr->lcd_id;
            dev_info.block_id = *handle_ptr;
            layer = GetLayerPtr(&dev_info);
            layer->is_autofreememory = !layer_ptr->is_static_layer;
            layer->owner_handle = MMK_ConvertIdToHandle(layer_ptr->owner_handle);
#ifdef UILAYER_NEED_ALIGN8
			layer->interval_left = blend_layer.interval_left;
			layer->interval_right = blend_layer.interval_right;
#endif
			layer->type = blend_layer.layer_info.layer_data.type;
			layer->alpha_sel = blend_layer.layer_info.layer_data.alpha_sel;
			layer->width = blend_layer.layer_info.width;
			layer->height = blend_layer.layer_info.height;
			layer->x = blend_layer.layer_info.pos.x;
			layer->y = blend_layer.layer_info.pos.y;
            layer->raw_layer_buf_ptr = buffer_ptr;
            layer->buf_addr = (uint32)ALIGN_256BYTES_ADDR_RIGHT(layer->raw_layer_buf_ptr);
            layer->layer_buf_size = buf_size;  
            layer->lcd_dev_info = dev_info;
        }
        else
        {
            dev_info.block_id = UILAYER_NULL_HANDLE;
            result = UILAYER_RESULT_MULTILAYER_DISABLED;
        }
    }
    else
    {
        MMI_ASSERT_WARNING("ui_layer.c CreateLayer() layer num is max!");
        dev_info.block_id = UILAYER_NULL_HANDLE;
        result = UILAYER_RESULT_USEDMAXNUM;
    }

    return result;
}

void hr_layer_release_ex(GUI_LCD_DEV_INFO const *dev_info_ptr)
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    
    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }

    UILAYER_RemoveBltLayer(dev_info_ptr);
	
	if (!IsMainLayerHandle(dev_info_ptr))
    {
        if (PNULL == layer)
        {
            MMI_ASSERT_WARNING("ReleaseLayer() PNULL == layer");
            return;/*lint !e527*/
        }
        layer->is_enable = FALSE;
        layer->owner_handle = 0;

        if (PNULL != layer->buf_addr)
        {
            layer->raw_layer_buf_ptr = PNULL;
        }
    }
}

PUBLIC BOOLEAN hero_UILAYER_IsMainLayerHandle(
                                            GUI_LCD_DEV_INFO const *dev_info_ptr
                                           )
{
    return IsMainLayerHandle(dev_info_ptr);
}

/*****************************************************************************/
//     Description : ���layer��bufferָ��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC uint8 *hero_UILAYER_GetLayerBufferPtrEx(
                                             GUI_LCD_DEV_INFO const *dev_info_ptr
                                            )
{
    UILAYER_STRUCT_T *layer = PNULL;
    uint32 mem_addr = 0;
//     uint32 bytes_per_pixel = 0;

    if (!CheckLayerId(dev_info_ptr))
    {
        return PNULL;
    }
  
    layer = GetLayerPtrEx(dev_info_ptr, TRUE);
    if (PNULL != layer)
    {
        if (PNULL != layer->raw_layer_buf_ptr)
        {
            mem_addr = (uint32)layer->raw_layer_buf_ptr;
        }    
        return (uint8*)mem_addr;
    }
    else
    {
        SCI_TRACE_LOW("UILAYER_GetLayerBufferPtr() PNULL == layer"); 
        return PNULL;
    }
}
#endif
/*****************************************************************************/
//  Description : С��ϳɴ��
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
/*
PUBLIC BOOLEAN UILAYER_PreMergeLayer(
                                  UILAYER_BLEND_T *merge_struct_in_ptr,
                                  UILAYER_BLEND_T *merge_struct_out_ptr  //out
                                  )
{
    int32 i = 0;
    int32 j = 0;
    GUI_RECT_T layer_rect[UILAYER_TOTAL_BLT_COUNT] = {0};
    GUI_RECT_T update_rect = {0};    
    GUI_RECT_T rect = {0};   
    BOOLEAN is_intersect = FALSE;
//    uint32 layer_stack_link[UILAYER_TOTAL_BLT_COUNT] = {0};
    GUI_RECT_T layer_stack[UILAYER_TOTAL_BLT_COUNT] = {0};
    uint32 stack_top = 0;

    if (PNULL == merge_struct_in_ptr)
    {
        return FALSE;
    }
    for (i = 0; i < merge_struct_in_ptr->layer_num; i++)
    {        
        if (PNULL != merge_struct_in_ptr->src_layer)
        {
            layer_rect[i].left = merge_struct_in_ptr->src_layer[i].layer_info.pos.x;
            layer_rect[i].top = merge_struct_in_ptr->src_layer[i].layer_info.pos.y;
            layer_rect[i].right = layer_rect[i].left + merge_struct_in_ptr->src_layer[i].layer_info.width - 1;
            layer_rect[i].bottom = layer_rect[i].top + merge_struct_in_ptr->src_layer[i].layer_info.height - 1;
            
            update_rect.left = merge_struct_in_ptr->rect.left;
            update_rect.top = merge_struct_in_ptr->rect.top;
            update_rect.right = merge_struct_in_ptr->rect.right;
            update_rect.bottom = merge_struct_in_ptr->rect.bottom;
            
            GUI_IntersectRect(&layer_rect[i], layer_rect[i], update_rect);
        }
    }
    layer_stack[0] = layer_rect[0];
    stack_top = 0;
    for (i = 1; i < merge_struct_in_ptr->layer_num; i++)
    {
        for (j = stack_top; j >= 0; j--)
        {
            is_intersect = GUI_IntersectRect(&rect, layer_rect[i], layer_stack[j]);
            if (is_intersect)
            {
                break;
            }
        }
        if (j == stack_top)
        {
            layer_stack[stack_top+1] = layer_rect[i];
            stack_top++;
        }
        else
        {
            layer_stack[j+1].left = MIN(layer_stack[j+1].left, layer_rect[i].left);
            layer_stack[j+1].top = MIN(layer_stack[j+1].top, layer_rect[i].top);
            layer_stack[j+1].right = MAX(layer_stack[j+1].right, layer_rect[i].right);
            layer_stack[j+1].bottom = MAX(layer_stack[j+1].bottom, layer_rect[i].bottom);
        }
    }
    if ((stack_top+2) < merge_struct_in_ptr->layer_num)
    {
        stack_top = stack_top;
    }
    return TRUE;
}
*/

/*****************************************************************************/
//  Description : ����4��ֶ�κϳ�
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_MultiBlendLayer(
                                       LCD_ID_E    lcd_id,
                                       DISPLAY_BLENDLAYER_PARM_T*  blend_param_ptr
                                       )
{
    DISPLAY_BLEND_LAYER_T       *temp_src_layer[UILAYER_BLEND_MAX_NUM] = {0};
    uint32                      temp_layer_num = 0;
    uint32                      i = 0;
    uint32                      j = 0;
    
    //����4��ֶ�κϳ�
    if ((PNULL == blend_param_ptr) || (blend_param_ptr->layer_num <= UILAYER_BLEND_MAX_NUM))
    {
        return FALSE;
    }
    //SCI_TRACE_LOW:"UILAYER_MultiBlendLayer() layer_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_5455_112_2_18_3_23_0_98,(uint8*)"d", blend_param_ptr->layer_num);
    //1
    for (j = 0; j < UILAYER_BLEND_MAX_NUM; j++)
    {
        temp_src_layer[j] = blend_param_ptr->layer_arr[j];
    }
    DISPLAY_BlendLayer(lcd_id, blend_param_ptr->dst_layer_ptr, temp_src_layer, 
        UILAYER_BLEND_MAX_NUM, blend_param_ptr->rect_ptr);
    //2
    for (i = UILAYER_BLEND_MAX_NUM; (i+UILAYER_BLEND_MAX_NUM-1) <= blend_param_ptr->layer_num;)
    {
        temp_src_layer[0] = blend_param_ptr->dst_layer_ptr;
        for (j = 1; j < UILAYER_BLEND_MAX_NUM; j++, i++)
        {
            temp_src_layer[j] = blend_param_ptr->layer_arr[i];
        }        
        DISPLAY_BlendLayer(lcd_id, blend_param_ptr->dst_layer_ptr, temp_src_layer, 
            UILAYER_BLEND_MAX_NUM, blend_param_ptr->rect_ptr);
    }
    //3
    temp_src_layer[0] = blend_param_ptr->dst_layer_ptr;
    for (j = 1; j < UILAYER_BLEND_MAX_NUM; j++)
    {
        temp_src_layer[j] = PNULL;
    }
    temp_layer_num = (blend_param_ptr->layer_num-i);
    if (0 != temp_layer_num)
    {
        for (i = 0; i < temp_layer_num; i++)
        {
            temp_src_layer[i+1] = blend_param_ptr->layer_arr[blend_param_ptr->layer_num-temp_layer_num+i];                    
        }                
        DISPLAY_BlendLayer(lcd_id, blend_param_ptr->dst_layer_ptr, temp_src_layer, 
            (temp_layer_num+1), blend_param_ptr->rect_ptr);
    }
    return TRUE;
}




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
                                       )
{
    DISPLAY_BLEND_LAYER_T  *temp_src_layer[UILAYER_BLEND_MAX_NUM] = {0};
    uint32  i = 0;
    uint32  j = 0;

    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;

    if(MAIN_LCD_ID != lcd_id || PNULL == blend_param_ptr)
    {
        SCI_TRACE_LOW("[UILAYER][SFL]UILAYER_MultiBlendLayerEx invalid input param.");
        return FALSE;
    }

    SCI_TRACE_LOW("[UILAYER][SFL]UILAYER_MultiBlendLayerEx layer_num=%d", blend_param_ptr->layer_num);
//    SCI_TRACE_LOW("[UILAYER][SFL]UILAYER_MultiBlendLayerEx rect [%d, %d, %d, %d]",
//                    blend_param_ptr->rect_ptr->left,
//                    blend_param_ptr->rect_ptr->top,
//                    blend_param_ptr->rect_ptr->right,
//                    blend_param_ptr->rect_ptr->bottom);
    if(blend_param_ptr->layer_num <= UILAYER_BLEND_MAX_NUM)
    {
        for (j = 0; j<blend_param_ptr->layer_num; j++)
        {
            temp_src_layer[j] = blend_param_ptr->layer_arr[j];
        }
        ret = DISPLAY_BlendLayer(lcd_id,
                                blend_param_ptr->dst_layer_ptr, 
                                temp_src_layer, 
                                blend_param_ptr->layer_num, 
                                blend_param_ptr->rect_ptr);
    }
    else
    {
        for (j = 0; j < UILAYER_BLEND_MAX_NUM; j++)
        {
            temp_src_layer[j] = blend_param_ptr->layer_arr[j];
        }
        ret = DISPLAY_BlendLayer(lcd_id, 
                                blend_param_ptr->dst_layer_ptr, 
                                temp_src_layer, 
                                UILAYER_BLEND_MAX_NUM, 
                                blend_param_ptr->rect_ptr);
        if(ERR_DISPLAY_NONE != ret)
        {
            SCI_TRACE_LOW("[UILAYER][SFL]UILAYER_MultiBlendLayerEx call to DISPLAY_BlendLayer() error[%d]", ret);
            return FALSE;
        }
        else
        {   
            SCI_TRACE_LOW("[UILAYER][SFL]UILAYER_MultiBlendLayerEx, 4 layers processed.");
        }
        for(i=UILAYER_BLEND_MAX_NUM; i<blend_param_ptr->layer_num; )
        {
            memset(temp_src_layer, 0, sizeof(temp_src_layer));
            temp_src_layer[0] = blend_param_ptr->dst_layer_ptr;
            for(j=0; j<((blend_param_ptr->layer_num-i)>(UILAYER_BLEND_MAX_NUM-1)?(UILAYER_BLEND_MAX_NUM-1):(blend_param_ptr->layer_num-i));j++)
            {
                temp_src_layer[j+1] = blend_param_ptr->layer_arr[i+j];
            }
            //dst + 3 src layers.
            ret = DISPLAY_BlendLayer(lcd_id,
                                blend_param_ptr->dst_layer_ptr,
                                temp_src_layer,
                                ((blend_param_ptr->layer_num-i)>(UILAYER_BLEND_MAX_NUM-1)?(UILAYER_BLEND_MAX_NUM):(blend_param_ptr->layer_num-i+1)),
                                blend_param_ptr->rect_ptr);
            if(ERR_DISPLAY_NONE != ret)
            {
                SCI_TRACE_LOW("[UILAYER][SFL]UILAYER_MultiBlendLayerEx call to DISPLAY_BlendLayer() error[%d]", ret);
                return FALSE;
            }

            i += (blend_param_ptr->layer_num-i)>(UILAYER_BLEND_MAX_NUM-1)?(UILAYER_BLEND_MAX_NUM-1):(blend_param_ptr->layer_num-i);
            SCI_TRACE_LOW("[UILAYER][SFL]UILAYER_MultiBlendLayerEx, %d layers processed.", i+1);
        }

    }
    
    SCI_TRACE_LOW("[UILAYER][SFL]UILAYER_MultiBlendLayerEx Exit.");
    
    return TRUE;
}

/*****************************************************************************/
//     Description : �����ļ���handleԤ��ˢ��lcd�ϣ�������˳������˸�����ӵ�˳��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetBltLayer(
                                UILAYER_APPEND_BLT_T *layer_handle_arr,
                                uint32 arr_size
                                )
{
    uint32 i = 0;
    uint32 size = MIN(arr_size, UILAYER_TOTAL_BLT_COUNT);
    uint32 count = 0;
    BOOLEAN is_exist_mainlayer = FALSE;

    if ( PNULL != layer_handle_arr )
    {
        for ( i = 0; i < size; i++ )
        {
            if ( CheckLayerId( &layer_handle_arr[i].lcd_dev_info ) )
            {
                s_layer_blt[count].lcd_dev_info = layer_handle_arr[i].lcd_dev_info;
                s_layer_blt[count].layer_level = layer_handle_arr[i].layer_level;
                s_layer_blt[count].is_valid = TRUE;
                count++;
                
                //����
                StoreLayerUpdateRect(&layer_handle_arr[i].lcd_dev_info);

                if (IsMainLayerHandle( &layer_handle_arr[i].lcd_dev_info ))
                {
                    is_exist_mainlayer = TRUE;
                }
            }
        }
        for (i = count; i < UILAYER_TOTAL_BLT_COUNT; i++)
        {
            s_layer_blt[i].lcd_dev_info.block_id = UILAYER_NULL_HANDLE;
            s_layer_blt[i].is_valid = FALSE;
        }

        if (!is_exist_mainlayer)
        {
            //free raw2
            FreeMainLayerBuffer2();
        }
    }
}

/*****************************************************************************/
//     Description : �õ���ǰ��blt layer���ã�����blt��Ŀ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC uint32 UILAYER_GetBltLayer(
                                  UILAYER_APPEND_BLT_T *layer_handle_arr,    //in/out
                                  uint32 arr_num    //in
                                  )
{
    uint32 i = 0;
    uint32 count = 0;

    if (PNULL != layer_handle_arr)
    {
        for (i = 0; count < arr_num && i < UILAYER_TOTAL_BLT_COUNT; i++)
        {
            if ( s_layer_blt[i].is_valid 
                && UILAYER_NULL_HANDLE != s_layer_blt[i].lcd_dev_info.block_id )
            {
                layer_handle_arr[count].lcd_dev_info = s_layer_blt[i].lcd_dev_info;
                layer_handle_arr[count].layer_level = s_layer_blt[i].layer_level;

                count++;
            }
        }
    }    

    return count;
}

PUBLIC void dumpBltLayers(int inc)
{
    int32 i=0;
    char output[255] = {0};

    while(i < UILAYER_TOTAL_BLT_COUNT)
    {
        if(s_layer_blt[i].is_valid)
        {
            sprintf(output, "%s{[id:%d][lv:%d]},\n", output, s_layer_blt[i].lcd_dev_info.block_id,s_layer_blt[i].layer_level);
        }
        i+=1;
    }
    SCI_TRACE_LOW("[UILAYER][SFL]dumpBltLayers[%d] \n %s", inc, output);
}

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
                                  )
{
    uint32 i = 0;
    uint32 count = 0;
    UILAYER_STRUCT_T *layer_info;
    mmi_handle = MMK_ConvertIdToHandle(mmi_handle);

    if (PNULL != layer_handle_arr)
    {
        for (i = 0; count < arr_num && i < UILAYER_TOTAL_BLT_COUNT; i++)
        {
            if ( s_layer_blt[i].is_valid 
                && (UILAYER_NULL_HANDLE != s_layer_blt[i].lcd_dev_info.block_id) )
            {
                layer_info = GetLayerPtr(&(s_layer_blt[i].lcd_dev_info));
                if(layer_info->owner_handle == mmi_handle)
                {
                    layer_handle_arr[count].lcd_dev_info = s_layer_blt[i].lcd_dev_info;
                    layer_handle_arr[count].layer_level = s_layer_blt[i].layer_level;
                    SCI_TRACE_LOW("[SFL1]UILAYER_GetBltLayersOfHandle: idx:%d, block_id:%d", count, layer_handle_arr[count].lcd_dev_info.block_id);
                    count++;
                }
            }
        }
    }

    return count;
}


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
                                        )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    
    if (IsMainLayerHandle(dev_info_ptr))
    {
        //��ֹ���㱻�����
        return;
    }

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }

    if (PNULL != layer)
    {
        if ( is_pre_multiply_alpha )
        {
            //��ARGB888תΪԤ��
            if ( DATA_TYPE_ARGB888 == layer->type )
            {
                layer->type = DATA_TYPE_PMARGB888;

                if ( is_need_convert )
                {
                    ProtectLayerBuf(layer);

                    MMI_ARGB888TOPMARGB888( (uint32*)layer->buf_addr, layer->layer_buf_size );
                }
            }
        }
        else
        {
            //��Ԥ�˸�ΪARGB888
            if ( DATA_TYPE_PMARGB888 == layer->type )
            {
                layer->type = DATA_TYPE_ARGB888;
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"UILAYER_SetPreMultiplyAlpha() failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_5609_112_2_18_3_23_1_99,(uint8*)"");
    }
}

/*****************************************************************************/
//     Description : ��ARGB888��ʽתΪԤ��
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMI_RGB565TOARGB555(
                                   uint16 *src_ptr, //in,out
                                   uint32 size,
                                   BOOLEAN Alpha_Bit
                                   )
{
    uint32 count;

    uint16 dst16;
	uint16 alpha_calc = 0;
	
    if ( PNULL == src_ptr
        || 0 == size )
    {
        return;
    }
	//SCI_ASSERT(Alpha_Bit);

	if(Alpha_Bit)
	{
		alpha_calc = (1<<15) ;
	}
//alpha_calc = (1<<15) ;
    count = size / sizeof(uint16);

    while ( count )
    {
        count--;
		
        dst16 = *src_ptr;
		
        //*src_ptr++ = (dst16&0x001f)|(((dst16&0xffc0)>>1)&0x7fff)|alpha_calc;
        *src_ptr++ = (dst16&0x001f)|((dst16>>1)&0x7fe0)|alpha_calc;
    }
	src_ptr--;
		
	SCI_TRACE_LOW("[NEST]  Alpha_Bit = %d %x",Alpha_Bit,*src_ptr);
}

/*****************************************************************************/
//     Description : ��RGB565��ʽתΪ5515
//    Global resource dependence : 
//  Author:   Haipeng.LIU
//    Note:
/*****************************************************************************/
PUBLIC void MMI_RGB565TOARGB5515(
                                   uint16 *src_ptr, //in,out
                                   uint32 size,
                                   BOOLEAN Alpha_Bit
                                   )
{
    uint32 count;

    uint16 dst16;
//	uint16 alpha_calc = 0;
	
    if ( PNULL == src_ptr
        || 0 == size )
    {
        return;
    }

    count = size / sizeof(uint16);

    while ( count )
    {
        count--;
		
        dst16 = *src_ptr;
		
        //*src_ptr++ = (dst16&0x001f)|((dst16>>1)&0x7fe0)|alpha_calc;
        if(Alpha_Bit)
        {
            *src_ptr++ = dst16|0x0020;
        }
        else
        {
            *src_ptr++ = dst16&0xffdf;
        }
    }
}


/*****************************************************************************/
//     Description : ����RGB565 ��ͼ����ΪARGB555
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ChangeLayerRGB565ToARGB555(
									                                        GUI_LCD_DEV_INFO const *dev_info_ptr,
									                                        BOOLEAN                 is_need_convert ,
									                                        BOOLEAN       		Alpha_Bit
									                                        )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    
    if (IsMainLayerHandle(dev_info_ptr))
    {
        //��ֹ��	�㱻�����
        return;
    }

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }

    if (PNULL != layer)
    {
            if ( DATA_TYPE_RGB565 == layer->type )
            {
                layer->type = DATA_TYPE_ARGB555;
				
                if ( is_need_convert )
                {
                    ProtectLayerBuf(layer);

                    MMI_RGB565TOARGB555( (uint16*)layer->buf_addr, layer->layer_buf_size ,Alpha_Bit);
                }
            }

        
    }
    else
    {
        SCI_TRACE_LOW("UILAYER_ChangeLayerRGB565ToARGB555     FAILE");
    }
}

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
									                                        )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    
    if (IsMainLayerHandle(dev_info_ptr))
    {
        //��ֹ��	�㱻�����
        return;
    }

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }

    if (PNULL != layer)
    {
            if ( DATA_TYPE_RGB565 == layer->type )
            {
                layer->type = DATA_TYPE_ARGB555;
				
                if ( is_need_convert )
                {
                    ProtectLayerBuf(layer);

                    MMI_RGB565TOARGB5515( (uint16*)layer->buf_addr, layer->layer_buf_size ,Alpha_Bit);
                }
            }

        
    }
    else
    {
        SCI_TRACE_LOW("UILAYER_ChangeLayerRGB565ToARGB5515     FAILE");
    }
}

/*****************************************************************************/
//     Description : ����ARGB555 ��ͼ����ΪRGB565
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ChangeLayerARGB555ToRGB565(
									                                        GUI_LCD_DEV_INFO const *dev_info_ptr,
									                                        BOOLEAN                 is_need_convert 
									                                        )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    
    if (IsMainLayerHandle(dev_info_ptr))
    {
        //��ֹ���㱻�����
        return;
    }

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }

    if (PNULL != layer)
    {
            if (DATA_TYPE_ARGB555  == layer->type )
            {
                layer->type = DATA_TYPE_RGB565;
                if ( is_need_convert )
                {
                    ProtectLayerBuf(layer);

                    //MMI_ARGB888TOPMARGB888( (uint32*)layer->buf_addr, layer->layer_buf_size );
                }
            }

        
    }
    else
    {
        SCI_TRACE_LOW("UILAYER_ChangeLayerARGB555ToRGB565     FAILE");
    }
}


/*****************************************************************************/
//     Description : ����ͼ��Ļص�����,��ˢ���󱻵���
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void BlendCallback(
                         uint32 param
                         )
{
    UILAYER_STRUCT_T *layer = PNULL;
    uint32 mem_addr = 0;
    uint32 i = 0;
    UILAYER_INFO_T layer_info = {0};

    for ( ; i < s_blend_callback_num; i++ )
    {
        layer = GetLayerPtr( &s_blend_callback_array[i].lcd_dev_info );
        
        if ( PNULL != layer 
            && PNULL != layer->buf_addr
            && PNULL != layer->callback
            //��ַ������Ч
            && ( s_blend_callback_array[i].buf_addr == ALIGN_256BYTES_ADDR_RIGHT(layer->raw_layer_buf_ptr) 
#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
            || s_blend_callback_array[i].buf_addr == ALIGN_256BYTES_ADDR_RIGHT(layer->raw2_layer_buf_ptr)
#endif
			) )
        {
            mem_addr = (uint32)s_blend_callback_array[i].buf_addr;

// #ifdef UILAYER_NEED_ALIGN8
//             mem_addr += ((uint16)layer->interval_left * GetLayerBytesPerPixel(layer));
// #endif
            GetLayerInfo( layer, &layer_info );
            
            layer->callback( (uint8*)mem_addr, &layer_info, layer->param );

            //������ÿ�, ��������ˢ��Ӱ��
            layer->callback = PNULL;
			layer->param = 0;
        } 
    }
}

/*****************************************************************************/
//     Description : ����ͼ��Ļص�����,��ˢ���󱻵���
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:ȷ��������IMMUTABLE��ͼ�����ʹ�ô˹���
/*****************************************************************************/
PUBLIC void UILAYER_SetBlendCallback(
                                     GUI_LCD_DEV_INFO const *dev_info_ptr,
                                     UILAYER_BLEND_CALLBACK callback,
                                     uint32                 param
                                     )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    
    if (IsMainLayerHandle(dev_info_ptr))
    {
        //��ֹ���㱻�����
        return;
    }

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }

    if (PNULL != layer)
    {
		ProtectLayerBuf( layer );

        layer->callback = callback;
        layer->param    = param;
    }
    else
    {
        //SCI_TRACE_LOW:"UILAYER_SetBlendCallback() failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_5688_112_2_18_3_23_1_100,(uint8*)"");
    }
}
#ifdef MET_MEX_SUPPORT
PUBLIC BOOLEAN UILAYER_SetMainLcdLayer(
    uint16 uiWidth, 
    uint16 uiHeight)
{   
   UILAYER_STRUCT_T *layer = PNULL;
   DISPLAY_BLEND_LAYER_T *main_layer_ptr = NULL;
   GUI_LCD_DEV_INFO lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };        
   if (!CheckLayerId(&lcd_dev_info))
   {
       return FALSE;
   }
   
   layer = GetLayerPtr(&lcd_dev_info);
   if (PNULL != layer)
    {
      layer->width= uiWidth;
      layer->height = uiHeight;
    }


   main_layer_ptr = GetMainLcdhandle();

   main_layer_ptr->layer_info.width = uiWidth;
   main_layer_ptr->layer_info.height = uiHeight;  
   return TRUE;
}
#endif

/*****************************************************************************/
//     Description : set clip rect
//    Global resource dependence : 
//  Author:   bin.ji
//    Note: clip_rect�������ͼ������
/*****************************************************************************/
PUBLIC void UILAYER_SetClipRect(
                                GUI_LCD_DEV_INFO const *dev_info_ptr,
                                GUI_RECT_T clip_rect
                                )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    
    if (IsMainLayerHandle(dev_info_ptr))
    {
        //��ֹ���㱻�����
        return;
    }

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }

    if (PNULL != layer)
    {
        layer->clip_rect.left = clip_rect.left;
        layer->clip_rect.top = clip_rect.top;
        layer->clip_rect.right = clip_rect.right;
        layer->clip_rect.bottom = clip_rect.bottom;
    }
    else
    {
        SCI_TRACE_LOW("UILAYER_SetClipRect() failed");
    }
}


#define PRINT_NODE_LEN 0xFF
#define PRINT_565_STR  "RGB565LE"
#define PRINT_888_STR  "RGBX888LE"
/*****************************************************************************/
// 	Description : print info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void UILAYER_PrintInfo(void)
{
    char              str_arr[PRINT_NODE_LEN+1] = {0};
    uint32            str_len = 0;
    const char*       print_str = PNULL;
    UILAYER_STRUCT_T* layer = PNULL;
    uint32            i = 0;
    BOOLEAN           is_bltlayer = FALSE;
    
    print_str = "s_layer_arr";
    OS_DumpPrint( (char*)print_str, strlen(print_str) );
    
    for (i = 0; i < UILAYER_TOTAL_LAYER_COUNT; i++)
    {
        if ( s_layer_arr[i].is_enable )
        {
            layer = &s_layer_arr[i];
            
            is_bltlayer = FindBltLayer(&layer->lcd_dev_info);
            
            str_len = _snprintf( str_arr, PRINT_NODE_LEN, "block_id=%d,is_bltlayer=%d,size=%d,name=%s,line=%d",
                layer->lcd_dev_info.block_id, is_bltlayer, layer->layer_buf_size, layer->file_name_ptr, layer->file_line );
            
            OS_DumpPrint( str_arr, str_len );
            
            if ( 0 != layer->buf_addr )
            {
                str_len = _snprintf( str_arr, PRINT_NODE_LEN, "data.image 0x%x %d. %d. /%s", 
                    layer->buf_addr, layer->width, layer->height, (DATA_TYPE_RGB565 == layer->type) ? PRINT_565_STR : PRINT_888_STR );
                
                OS_DumpPrint( str_arr, str_len );
            }
        }
    }
}

/*****************************************************************************/
//     Description : switch mainlayer������cameraʹ�ã����Խ�ʡһ��buffer��
//    Global resource dependence : 
//  Author:   bin.ji
//    Note: is_setto_mainlcd_buffer=TRUE������Ϊlcd_buffer��FALSE����lcd_buffer���û�������̬���䣩
/*****************************************************************************/
PUBLIC void UILAYER_SwitchMainLayer(
                                    BOOLEAN is_setto_mainlcd_buffer
                                    )
{
    GUI_LCD_DEV_INFO main_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    UILAYER_STRUCT_T *layer = GetLayerPtr(&main_dev_info);

    if (!CheckLayerId(&main_dev_info) || PNULL == layer)
    {
        return;
    }

    SCI_TRACE_LOW("ui_layer.c UILAYER_SwitchMainLayer() is_setto_mainlcd_buffer = %d", is_setto_mainlcd_buffer);

    if (is_setto_mainlcd_buffer != s_is_setto_mainlcd_buffer)
    {
        if (is_setto_mainlcd_buffer)
        {
            DeactiveMainLayer();
            layer->raw_layer_buf_ptr = (uint8*)GUILCD_GetMainLcdBufPtr();
            layer->buf_addr= (uint32)GUILCD_GetMainLcdBufPtr();
        }
        else
        {
            layer->raw_layer_buf_ptr = PNULL;
            layer->buf_addr= PNULL;
            ActiveMainLayer();
        }
        s_is_setto_mainlcd_buffer = is_setto_mainlcd_buffer;
    }
}
#if 0

/*****************************************************************************/
//     Description : �ڴ渲������
//    Global resource dependence : 
//  Author:   bin.ji
//    Note: 
/*****************************************************************************/
PUBLIC void UILAYER_TestMemcpy(
                               GUI_LCD_DEV_INFO const *dev_info_ptr
                               )
{
    UILAYER_STRUCT_T *layer = GetLayerPtr(dev_info_ptr);
    
    if (IsMainLayerHandle(dev_info_ptr))
    {
        //��ֹ���㱻�����
        return;
    }

    if (!CheckLayerId(dev_info_ptr))
    {
        return;
    }

    if (PNULL != layer)
    {
        if (PNULL != layer->buf_addr)
    	 {
    	     SCI_MEMCPY((void*)layer->buf_addr, (void*)s_mainlcd_handle.layer_info.buf_addr, layer->layer_buf_size+200);             
    	 }
    }
    else
    {
        SCI_TRACE_LOW("ui_layer.c UILAYER_TestMem() failed");
    }
}

#endif
#endif	//UI_MULTILAYER_SUPPORT
