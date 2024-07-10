#include "dal_display.h"



typedef struct _img_layer_tag
{
    DISPLAY_DATA_TYPE_E    type;        //data type
    uint32                 buf_addr;    //it is buffer address of display data for ARGB888/RGB565
                                      
    uint16                 width;       //widht of block ,uinit is pixel
    uint16                 height;                 //height of block,unit is pixel
    DISLPAY_POS_T          pos;         //start position of display block
    uint32                 alpha;       //alpha value 
    DISPLAY_ALPHA_TYPE_E   alpha_sel;   //pixel alpha or block alpha
    uint32                 colorkey;    // colorkey value
    BOOLEAN                colorkey_en; // 1:use colorkey;0: don't use colorkey
    DISPLAY_RECT_T         clip_rect;
    
}IMG_LAYER_T;


PUBLIC ERR_DISPLAY_E OSD_Blend(IMG_LAYER_T *dst_layer_ptr,
                               IMG_LAYER_T *layer_arr,
                               uint32 layer_num,
                               DISPLAY_RECT_T *rect_ptr);

