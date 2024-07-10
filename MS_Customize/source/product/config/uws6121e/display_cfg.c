#include "display_cfg.h"
#include "dal_img.h"
/*lint -save -e64*/
#ifdef DISPLAY_PIXELDEPTH_MAX_32
#define DISPLAY_BYTE_NUM_IN_PIXEL  4
#else
#define DISPLAY_BYTE_NUM_IN_PIXEL  2
#endif

#ifndef DISPLAY_LOW_MEMORY
#if defined MAIN_LCD_DISPLAY_LAYER_NUM_1
#define MAIN_LCD_DISPLAY_MEM_NUM        1
#define MAIN_LCD_DISPLAY_MEM_SIZE    (LCD_WIDTH*LCD_HEIGHT*DISPLAY_BYTE_NUM_IN_PIXEL)
__align(32) LOCAL uint8 s_display_mem[MAIN_LCD_DISPLAY_MEM_SIZE] = {0};/*lint -esym(551, s_display_mem)*/
#elif defined MAIN_LCD_DISPLAY_LAYER_NUM_2
#define MAIN_LCD_DISPLAY_MEM_NUM        2
#define MAIN_LCD_DISPLAY_MEM_SIZE    (LCD_WIDTH*LCD_HEIGHT*DISPLAY_BYTE_NUM_IN_PIXEL*2)
LOCAL __align(32) uint8 s_display_mem[MAIN_LCD_DISPLAY_MEM_SIZE] = {0};/*lint -esym(552, s_display_mem)*/
#elif defined MAIN_LCD_DISPLAY_LAYER_NUM_3
#define MAIN_LCD_DISPLAY_MEM_NUM        3
#define MAIN_LCD_DISPLAY_MEM_SIZE    (LCD_WIDTH*LCD_HEIGHT*DISPLAY_BYTE_NUM_IN_PIXEL*3)
LOCAL __align(32) uint8 s_display_mem[MAIN_LCD_DISPLAY_MEM_SIZE] = {0};/*lint -esym(552, s_display_mem)*/
#else
#ifdef PLATFORM_SC6600L
#define MAIN_LCD_DISPLAY_MEM_NUM        0
#define MAIN_LCD_DISPLAY_MEM_SIZE       2
#else
#define MAIN_LCD_DISPLAY_MEM_NUM        1
#define MAIN_LCD_DISPLAY_MEM_SIZE   (LCD_WIDTH*LCD_HEIGHT*DISPLAY_BYTE_NUM_IN_PIXEL)
#endif//PLATFORM_SC6600L
LOCAL __align(32) uint8 s_display_mem[MAIN_LCD_DISPLAY_MEM_SIZE] = {0};
#endif


#if defined SUB_LCD_DISPLAY_LAYER_NUM_1
#define SUB_LCD_DISPLAY_MEM_NUM        1
#define SUB_LCD_DISPLAY_MEM_SIZE    (SUBLCD_WIDTH*SUBLCD_HEIGHT*DISPLAY_BYTE_NUM_IN_PIXEL)
__align(32) LOCAL uint8 s_sub_lcd_display_mem[SUB_LCD_DISPLAY_MEM_SIZE] = {0};/*lint -esym(551, s_sub_lcd_display_mem)*/
#else
#define SUB_LCD_DISPLAY_MEM_NUM        0
#define SUB_LCD_DISPLAY_MEM_SIZE    2
__align(32) LOCAL uint8 s_sub_lcd_display_mem[SUB_LCD_DISPLAY_MEM_SIZE] = {0};/*lint -esym(551, s_sub_lcd_display_mem)*/
#endif
#elif LCD_INSTALL_ANGLE_90
#if defined MAIN_LCD_DISPLAY_LAYER_NUM_1
#define MAIN_LCD_DISPLAY_MEM_NUM        1
#define MAIN_LCD_DISPLAY_MEM_SIZE    (LCD_WIDTH*LCD_HEIGHT*DISPLAY_BYTE_NUM_IN_PIXEL)
__align(32) LOCAL uint8 s_display_mem[MAIN_LCD_DISPLAY_MEM_SIZE] = {0};/*lint -esym(551, s_display_mem)*/
#elif defined MAIN_LCD_DISPLAY_LAYER_NUM_2
#define MAIN_LCD_DISPLAY_MEM_NUM        2
#define MAIN_LCD_DISPLAY_MEM_SIZE    (LCD_WIDTH*LCD_HEIGHT*DISPLAY_BYTE_NUM_IN_PIXEL*2)
LOCAL __align(32) uint8 s_display_mem[MAIN_LCD_DISPLAY_MEM_SIZE] = {0};/*lint -esym(552, s_display_mem)*/
#else defined MAIN_LCD_DISPLAY_LAYER_NUM_3
#define MAIN_LCD_DISPLAY_MEM_NUM        3
#define MAIN_LCD_DISPLAY_MEM_SIZE    (LCD_WIDTH*LCD_HEIGHT*DISPLAY_BYTE_NUM_IN_PIXEL*3)
LOCAL __align(32) uint8 s_display_mem[MAIN_LCD_DISPLAY_MEM_SIZE] = {0};/*lint -esym(552, s_display_mem)*/

#endif
#if defined SUB_LCD_DISPLAY_LAYER_NUM_1
#define SUB_LCD_DISPLAY_MEM_NUM        1
#define SUB_LCD_DISPLAY_MEM_SIZE    (SUBLCD_WIDTH*SUBLCD_HEIGHT*DISPLAY_BYTE_NUM_IN_PIXEL)
__align(32) LOCAL uint8 s_sub_lcd_display_mem[SUB_LCD_DISPLAY_MEM_SIZE] = {0};/*lint -esym(551, s_sub_lcd_display_mem)*/
#else
#define SUB_LCD_DISPLAY_MEM_NUM        0
#define SUB_LCD_DISPLAY_MEM_SIZE    2
__align(32) LOCAL uint8 s_sub_lcd_display_mem[SUB_LCD_DISPLAY_MEM_SIZE] = {0};/*lint -esym(551, s_sub_lcd_display_mem)*/
#endif

#else

	#define MAIN_LCD_DISPLAY_MEM_NUM        0
	#define MAIN_LCD_DISPLAY_MEM_SIZE       0

	LOCAL __align(32) uint8 s_display_mem[2] = {0};/*lint -esym(552, s_display_mem)*/

	#define SUB_LCD_DISPLAY_MEM_NUM        0
	#define SUB_LCD_DISPLAY_MEM_SIZE    0

	LOCAL __align(32) uint8 s_sub_lcd_display_mem[2] = {0};/*lint -esym(551, s_sub_lcd_display_mem)*//*lint -esym(552, s_sub_lcd_display_mem)*/
#endif
/*****************************************************************************/
//  Description:  get display number of layer on specified lcd
//    Global resource dependence:
//  Author: Jianping.wang
//    Note:
/*****************************************************************************/
uint32 DISPLAYCFG_GetLayerNum(LCD_ID_E lcd_id)
{
    uint32 layer_num = 0;

    if(lcd_id == LCD_ID_0)
    {
        layer_num = MAIN_LCD_DISPLAY_MEM_NUM;
    }
    else
    {
        layer_num = SUB_LCD_DISPLAY_MEM_NUM;
    }

    return layer_num;
}
/*****************************************************************************/
//  Description:  get display memory  address of layer on specified lcd
//    Global resource dependence:
//  Author: Jianping.wang
//    Note:
/*****************************************************************************/
uint32 DISPLAYCFG_GetMemAddr(LCD_ID_E lcd_id)
{
    uint32 mem_addr = 0;
    uint32 layer_num = 0;

    if(lcd_id == LCD_ID_0)
    {
        layer_num = MAIN_LCD_DISPLAY_MEM_NUM;
        if(layer_num == 0)/*lint !e774*/
        {
            mem_addr = 0;
        }
        else
        {
            mem_addr = (uint32)&s_display_mem[0];
        }
    }
    else
    {
        layer_num = SUB_LCD_DISPLAY_MEM_NUM;
        if(layer_num == 0)/*lint !e774*/
        {
            mem_addr = 0;
        }
        else
        {
            mem_addr = (uint32)&s_sub_lcd_display_mem[0];
        }
    }

    return mem_addr;
}
/*****************************************************************************/
//  Description:  get display memory size of layer on specified lcd
//    Global resource dependence:
//  Author: Jianping.wang
//    Note:
/*****************************************************************************/
uint32 DISPLAYCFG_GetMemSize(LCD_ID_E lcd_id,uint32 layer_id)
{
    uint32 mem_size = 0;

    if(lcd_id == LCD_ID_0)
    {
        mem_size = MAIN_LCD_DISPLAY_MEM_SIZE;
    }
    else
    {
        mem_size = SUB_LCD_DISPLAY_MEM_SIZE;
    }

    return mem_size;
}


//bmp_decode

PUBLIC const BMP_DECODER_TAB_T res_dec_tab1[] =/*lint -esym(552, res_dec_tab1)*/
{
	{RES_DEC_TYPE1, PNULL, PNULL},
	{RES_DEC_TYPE2, PNULL, PNULL},
	{RES_DEC_TYPE3, PNULL, PNULL},
	{RES_DEC_TYPE4, RES_Dec_Bmp_Fun7, RES_Dec_Bmp_Fun8},
	{RES_DEC_TYPE5, PNULL, PNULL},
	{RES_DEC_TYPE6, RES_Dec_Bmp_Fun11, RES_Dec_Bmp_Fun12},
       {RES_DEC_TYPE7, PNULL, PNULL},
	{RES_DEC_TYPE8, PNULL, PNULL},
       {RES_DEC_TYPE9, PNULL, PNULL}

};

PUBLIC uint32 Res_Dec_Func1(uint32 *res_dec_ptr)
{
    uint32 size = 0;
    size = sizeof(res_dec_tab1)/sizeof(BMP_DECODER_TAB_T);
    *res_dec_ptr = res_dec_tab1;

    return size;
}

//argb565 , argb888
LOCAL const ALPHA_RES_DEC_TAB_T res_dec_tab2[] =
{
	{FLAG_RGB565, RES_IMG_TYPE1, RES_BG_TYPE1, RES_Dec_Argb_Fun1},
	{FLAG_RGB565, RES_IMG_TYPE2, RES_BG_TYPE1, RES_Dec_Argb_Fun2},
	{FLAG_RGB565, RES_IMG_TYPE2, RES_BG_TYPE2, RES_Dec_Argb_Fun3},

	{FLAG_ARGB888, RES_IMG_TYPE1, RES_BG_TYPE1, RES_Dec_Argb_Fun4},
	{FLAG_ARGB888, RES_IMG_TYPE2, RES_BG_TYPE1, RES_Dec_Argb_Fun5},
	{FLAG_ARGB888, RES_IMG_TYPE2, RES_BG_TYPE2, RES_Dec_Argb_Fun6}

};

PUBLIC uint32 Res_Dec_Func2(uint32 *res_dec_ptr)
{
    uint32 size = 0;
    size = sizeof(res_dec_tab2)/sizeof(ALPHA_RES_DEC_TAB_T);
    *res_dec_ptr = (uint32 *)res_dec_tab2;

    return size;
}


//argb_rle
LOCAL const ARGB_RLE_DEC_TAB_T res_dec_tab3[] =
{
	{RES_IMG_TYPE1, RES_BG_TYPE1, RES_Dec_Argb_Fun7},
	{RES_IMG_TYPE2, RES_BG_TYPE1, RES_Dec_Argb_Fun8},
	{RES_IMG_TYPE2, RES_BG_TYPE2, RES_Dec_Argb_Fun9}
};


PUBLIC uint32 Res_Dec_Func3(uint32 *res_dec_ptr)
{
    uint32 size = 0;
    size = sizeof(res_dec_tab3)/sizeof(ARGB_RLE_DEC_TAB_T);
    *res_dec_ptr = (uint32 *)res_dec_tab3;

    return size;
}


LOCAL const ARGB_RLE_DEC_TAB_T res_dec_tab4[] =
{
       {RES_IMG_TYPE1, RES_BG_TYPE1, RES_Dec_Argb_Fun10},
	{RES_IMG_TYPE2, RES_BG_TYPE1, RES_Dec_Argb_Fun8},
	{RES_IMG_TYPE2, RES_BG_TYPE2, RES_Dec_Argb_Fun9}

};

PUBLIC uint32 Res_Dec_Func4(uint32 *res_dec_ptr)
{
uint32 size = 0;
size = sizeof(res_dec_tab4)/sizeof(ARGB_RLE_DEC_TAB_T);
*res_dec_ptr = (uint32 *)res_dec_tab4;

  return size;
}
/*lint -restore*/



