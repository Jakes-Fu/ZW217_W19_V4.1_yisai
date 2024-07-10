#include "window_parse.h"
#include "mmk_app.h"
#include "guibutton.h"

#include "guitext.h"
#include "guimsgbox.h"
#include "guilcd.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "tb_dal.h"
#include "cafcontrol.h"

#include "mmi_textfun.h"
#include "mmi_appmsg.h"
#include "window_parse.h"
#include "mmi_common.h"
#include "mmipub.h"
#include "mmi_default.h"
#include "mmk_timer.h"

#include "os_api.h"
#include "mmiset_export.h"
#include "gui_ucs2b_converter.h"
#include "mmifm_export.h"

#include "od_game_res_id_def.h"
#include "od_game_nv.h"

#ifdef OD_GAME_SUPPORT
typedef struct
{
    OD_U32   game_menu_id;
    OD_U32 game_title_id;
    OD_U32 help_text_id;
    OD_U8 level_count;                                 /* how many levels */
    OD_U32 level_str_id_list[OD_GAME_MAX_LEVEL_COUNT];     /* level string ID */
    OD_U32 nvram_id;                           
    OD_S32 hilight_index;
    OD_BOOL is_pause;
    OD_BOOL is_new_game;
    void (*enter_game_func_ptr) (void);     /* function to enter game */
}OD_GAME_MENU_T;

OD_GAME_MENU_T g_od_game_menu_data;

OD_U16 od_game_get_screen_width()
{
    return OD_LCD_WIDTH;
}


OD_U16 od_game_get_screen_height()
{
    return OD_LCD_HEIGHT;
}


void *od_game_malloc(OD_U32 size)
{
    void * p;

    p = SCI_ALLOC_APP(size);

    if(p)
    {
        SCI_MEMSET(p,0,size);
    }
    return p;
}


void od_game_free(void *ptr)
{
    if(ptr)
    SCI_FREE(ptr);
}

OD_U16* od_game_wstrcat(OD_U16* dst,const OD_U16* src)
{
   return  MMIAPICOM_Wstrcat( dst, src);
}

OD_U32 od_game_wstrlen(const OD_U16* str)
{
   return  MMIAPICOM_Wstrlen(str);
                               
}

OD_U16* od_game_wstrcpy(OD_U16* dst,const OD_U16* src)
{
	return MMIAPICOM_Wstrcpy(dst,src);
                               
}

OD_U32 od_game_utf8_to_wstr(OD_U16*wstr_ptr,OD_U32 wstr_len,const OD_U8 *utf8_ptr)
{
	return GUI_UTF8ToWstr(wstr_ptr,wstr_len,utf8_ptr,strlen(utf8_ptr));
}


OD_U32 od_game_wstr_to_utf8(OD_U8*utf8_ptr,OD_U32 utf8_buf_len,const OD_U16* wstr_ptr,OD_U32 wstr_len)
{
	return GUI_WstrToUTF8(utf8_ptr,utf8_buf_len,wstr_ptr,wstr_len);
}


OD_U8* od_game_get_format_string(OD_U16* wstr,OD_U16 wstr_len,OD_S32 d,OD_U8* s)
{
    OD_U8* asc_string = NULL;
    OD_U8* temp_string = NULL;
    OD_U8* ucs_string = NULL;
    OD_U32 utf8_buf_len;

    if(wstr == NULL)
    {
        return NULL;
    }
    utf8_buf_len = wstr_len*3; 
    asc_string = (OD_U8*)od_game_malloc(utf8_buf_len +1);
    od_game_wstr_to_utf8(asc_string,utf8_buf_len, (const OD_U16 *)wstr,wstr_len);
    if(s)
    {
        utf8_buf_len+= strlen(s);
        temp_string = (OD_U8*)od_game_malloc(utf8_buf_len);
        sprintf(temp_string,asc_string,s);
    }
    else
    {
        utf8_buf_len+=20;
        temp_string = (OD_U8*)od_game_malloc(utf8_buf_len);
        sprintf(temp_string,asc_string,d);
    }
    od_game_free(asc_string);
    ucs_string =  (OD_U8*)od_game_malloc((utf8_buf_len+1)*2);
    od_game_utf8_to_wstr((OD_U16 *)ucs_string,utf8_buf_len,temp_string);
    od_game_free(temp_string);
    return ucs_string;
}


#define gdi_layer_lock_frame_buffer() MMITHEME_SetUpdateDelayCount(100);
#define gdi_layer_unlock_frame_buffer() MMITHEME_SetUpdateDelayCount(0);

//公共UI部分
void od_game_gdi_layer_lock_frame_buffer()
{
    gdi_layer_lock_frame_buffer();
}


void od_game_gdi_layer_unlock_frame_buffer()
{
    gdi_layer_unlock_frame_buffer();
}


void od_game_image_get_dimension(OD_U32 image_id, OD_S16 * w, OD_S16 * h)
{
    OD_U16 width,height;
    GUIRES_GetImgWidthHeight(&width,&height,image_id,MMK_GetFocusWinId());
    *w = width;
    *h = height;
}

void od_game_gui_push_and_set_clip(od_game_rect_t* clip_rect,OD_S32 x1, OD_S32 y1,OD_S32 x2,OD_S32 y2)
{
    clip_rect->left= x1;
    clip_rect->right = x2;
    clip_rect->top = y1;
    clip_rect->bottom = y2;
}

OD_BOOL  od_game_guilist_intersect_rect(
                                 od_game_rect_t    *rect_dest_ptr,
                                 od_game_rect_t    rect1,
                                 od_game_rect_t    rect2
                                 )
{
    OD_BOOL     result = OD_TRUE;

    if (OD_NULL == rect_dest_ptr)
    {
        return OD_FALSE;
    }
    
    rect_dest_ptr->left   = (OD_S16)(MAX(rect1.left, rect2.left));
    rect_dest_ptr->top    = (OD_S16)(MAX(rect1.top, rect2.top));
    rect_dest_ptr->right  = (OD_S16)(MIN(rect1.right, rect2.right));
    rect_dest_ptr->bottom = (OD_S16)(MIN(rect1.bottom, rect2.bottom));
    
    if ((rect_dest_ptr->left > rect_dest_ptr->right) ||
        (rect_dest_ptr->top > rect_dest_ptr->bottom))
    {
        result = OD_FALSE;
    }
    else if ((0 == rect_dest_ptr->left) &&
             (0 == rect_dest_ptr->top) &&
             (0 == rect_dest_ptr->right) &&
             (0 == rect_dest_ptr->bottom))
    {
        result = OD_FALSE;
    }
    
    return (result);
}


void od_game_draw_image_id(OD_S16 x,OD_S16 y,OD_U32 image_id,od_game_rect_t clip_rect)
{
	GUI_LCD_DEV_INFO const* lcd_dev_info_ptr = (const GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev();
	OD_U16 image_w,image_h;
	od_game_rect_t rect,image_rect;

	od_game_image_get_dimension(image_id,&image_w, &image_h); 
	rect.left = x;
	rect.top  = y;
	rect.right = x + image_w-1;
	rect.bottom = y + image_h -1;
	od_game_guilist_intersect_rect(&rect, rect, clip_rect);
	image_rect.left = rect.left - x ;
	image_rect.top  = rect.top - y ;
	image_rect.right = image_rect.left + rect.right - rect.left;
	image_rect.bottom = image_rect.top + rect.bottom - rect.top;
	IMG_EnableTransparentColor(OD_TRUE);
	GUIRES_DisplayImg(OD_NULL,
					&rect, 
					&image_rect, 
					MMK_GetFocusWinId(), 
					image_id, 
					lcd_dev_info_ptr);
	IMG_EnableTransparentColor(OD_FALSE);
}


OD_U8 od_game_start_timer(OD_U32 count,  void (*callback) (void))
{
	return Od_CreateTimer(count,callback);
}


void od_game_cancel_timer(OD_U8 timer_id)
{
	Od_StopTimer(timer_id);
}

void od_game_layer_blt_previous()
{
    MMITHEME_UpdateRect();
}

#define rgb_to_c565(r,g,b)  ((((r)&0xf8)<<8) | (((g)&0xfc)<<3) | (((b)&0xf8)>>3))

void od_game_draw_line(OD_S32 x1,OD_S32 y1,OD_S32 x2,OD_S32 y2,od_game_color c)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_COLOR_T  col= rgb_to_c565(c.r, c.g,c.b);
    LCD_DrawLine(&lcd_dev_info,x1,y1,x2,y2,col);
}


void od_game_fill_rectangle(OD_S32 x1,OD_S32 y1,OD_S32 x2,OD_S32 y2,od_game_color c)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T rect;
    GUI_COLOR_T  col= rgb_to_c565(c.r,c.g,c.b);
    rect.left= x1;
    rect.right = x2;
    rect.top = y1;
    rect.bottom = y2;
    LCD_FillRect(&lcd_dev_info, rect, col);
}

OD_S32 od_game_rand()
{
    return rand();
}




void od_game_print_text(MMI_STRING_T *_text, od_game_rect_t *rect, GUI_COLOR_T color,GUI_ALIGN_E align,GUI_FONT_T font,GUI_LCD_DEV_INFO* lcd_dev_info)
{
    GUISTR_STYLE_T str_style = {0};

    str_style.font = font;
	
    str_style.font_color = color;
    str_style.align = align;

    GUISTR_DrawTextToLCDInRect( 
							lcd_dev_info, 
							(const GUI_RECT_T*)rect, 
							(const GUI_RECT_T*)rect,
							_text,
							&str_style,
							GUISTR_STATE_SINGLE_LINE,
							0); 
}


GUI_FONT_T	od_game_get_font_type(OD_U8 font_size)
{
    GUI_FONT_T	font_type = MMI_DEFAULT_NORMAL_FONT;
    switch(font_size)
    {
        case 0:
            font_type = MMI_DEFAULT_SMALL_FONT;
            break;
        case 1:
            font_type = MMI_DEFAULT_NORMAL_FONT;
            break;
        case 2:
            font_type = MMI_DEFAULT_BIG_FONT;
            break;
    }
    return font_type;
}


OD_S16 od_game_draw_text(OD_U8* str,OD_U16 str_len,OD_BOOL ucs2,od_game_rect_t *rect, od_game_color c,OD_U8 align,OD_U8 font_size)
{
    GUI_FONT_T font;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_COLOR_T  col= rgb_to_c565(c.r, c.g,c.b);
    OD_U16* wstr = NULL;
    MMI_STRING_T _text = {0};
    GUI_ALIGN_E sys_align = ALIGN_LVMIDDLE;
    OD_S32 w,h;
    font = od_game_get_font_type(font_size);
    switch(align)
    {
        case 0:
            sys_align = ALIGN_LVMIDDLE;
            break;
        case 1:
            sys_align = ALIGN_HVMIDDLE;
            break;
        case 2:
            sys_align = ALIGN_RVMIDDLE;
            break;
    }
    if(!ucs2)
    {
        wstr = od_game_malloc((str_len+1)*3);
        od_game_utf8_to_wstr(wstr, str_len, str);
    }
    else
    {
        wstr = (OD_U16*)str;
    }
    _text.wstr_ptr = wstr;
    _text.wstr_len = str_len;
    od_game_string_get_dimension(_text.wstr_ptr,_text.wstr_len,font_size,&w, &h);

    od_game_print_text(&_text,rect,col,sys_align,font,&lcd_dev_info);
    if(!ucs2)
    od_game_free(wstr);
    return w;
}



void od_game_show_popup_bg(OD_S32 x1, OD_S32 y1,OD_S32 x2,OD_S32 y2,OD_U32 image_id,OD_BOOL need_line,od_game_color line_color)
{
    OD_S32 h=0;
    OD_S32 w=0;
    OD_U32 bg_id = image_id;
    OD_S16 bg_w,bg_h;
    od_game_rect_t clip_rect;
    if(bg_id == 0)
    {
        bg_id = OD_GAME_POPUP_BG_IMAGE;
    }

    od_game_gui_push_and_set_clip(&clip_rect,x1,y1,x2,y2);    
    od_game_image_get_dimension(bg_id,&bg_w,&bg_h);
    while(h<(y2-y1 +1))
    {
        w = 0;
        while(w<(x2-x1+1))
        {
            od_game_draw_image_id(x1+w,y1+h,bg_id,clip_rect);
            w+= bg_w;
        }
        h+= bg_h;
    }
    if(need_line)
    {
        od_game_draw_line(x1,y1,x2,y1, line_color);
        od_game_draw_line(x1,y2,x2,y2, line_color);
        od_game_draw_line(x1,y1,x1,y2, line_color);
        od_game_draw_line(x2,y1,x2,y2, line_color);
    }
}

void od_game_string_get_dimension(OD_U16* wstr,OD_U16 wstr_len,OD_U8 font_size,OD_S32* text_width,OD_S32* text_height)
{
    MMI_STRING_T _text = {0};
     GUI_FONT_T	font_type = od_game_get_font_type(font_size);

    _text.wstr_ptr = wstr;
    _text.wstr_len = wstr_len;;
    *text_width=GUI_GetStringWidth(font_type, _text.wstr_ptr, _text.wstr_len);
    *text_height=GUI_GetStringHeight(font_type, _text.wstr_ptr, _text.wstr_len);
}


void od_game_show_popup_text_with_bg_id(OD_U32* list_of_strings,OD_U8** list_of_items,OD_U16 count,OD_U32 bg_id)
{
    od_game_rect_t rect;
    od_game_color line_color = {50,50,50};
    od_game_color text_color = {0,0,0};
    OD_S32 width,height;
    OD_S32 x1,x2,y1,y2,h;
    OD_U8 gap;
    OD_U16 total_height,line_height,i;

    if((list_of_strings == NULL && list_of_items == NULL)|| count == 0)
    {
        return;
    }

    gap = od_game_get_screen_height()/20;
    //计算popup的区域
    od_game_string_get_dimension(L"A",1,0,&width, &height);
    
    line_height = height+gap;
    total_height = 0;
    for(i=0;i<count;i++)
    {
        if((list_of_strings && list_of_strings[i] > 0) || (list_of_items && list_of_items[i]))
        {
            total_height+=line_height;
        }
        else
        {
            total_height += height;
        }
    }
    //画popup的背景
    x1 = od_game_get_screen_width()/20;
    x2 = od_game_get_screen_width() -x1 -1;
    y1 = (od_game_get_screen_height() -total_height)/2;
    y2 = y1+total_height -1;
    od_game_show_popup_bg(x1,y1,x2,y2,bg_id,OD_TRUE,line_color);

    rect.left = x1;
    rect.right = x2;
    rect.top = y1;
    rect.bottom = y1;
    //画字符串
    for(i=0;i<count;i++)
    {
        if((list_of_strings && list_of_strings[i] > 0) || (list_of_items && list_of_items[i]))
        {
            rect.bottom += line_height;
            if(list_of_strings)
            {
                MMI_STRING_T text_str ={0};
                MMI_GetLabelTextByLang(list_of_strings[i],&text_str);
                od_game_draw_text((OD_U8*)text_str.wstr_ptr,text_str.wstr_len,1,&rect,text_color,1, 0);
            }
            else
            od_game_draw_text(list_of_items[i],od_game_wstrlen(list_of_items[i]),1,&rect,text_color,1, 0);                
            rect.top+= line_height;
        }
        else
        {
            rect.bottom += height;
            rect.top += height;
        }
    }
}


void od_game_show_popup_text(OD_U32* list_of_strings,OD_U8** list_of_items,OD_U16 count)
{
	od_game_show_popup_text_with_bg_id(list_of_strings,list_of_items,count,0);
}


void od_game_draw_game_back_button()
{
    od_game_rect_t rect;
    od_game_color text_color = {0,0,0};
    OD_S32 x1,x2,y1,y2;
    OD_S32 width,height;
    MMI_STRING_T text_str ={0};

    MMI_GetLabelTextByLang(OD_GAME_BACK_TEXT,&text_str);
    od_game_string_get_dimension(text_str.wstr_ptr,text_str.wstr_len,0,&width, &height);
    
    height += (od_game_get_screen_height()/40);

    x1 = 0;
    x2 = od_game_get_screen_width() -1;
    y2 = od_game_get_screen_height() -1;
    y1 = y2 - height -1;
    od_game_show_popup_bg(x1,y1,x2,y2,0,OD_FALSE,text_color);

    rect.left = x1;
    rect.right=  x2-2;
    rect.top = y1;
    rect.bottom = y1+height;
    od_game_draw_text(text_str.wstr_ptr,text_str.wstr_len,1,&rect,text_color,2,0);
}

void od_game_draw_game_over_text()
{
    OD_U32 list_of_strings[3] = {0,OD_GAME_OVER_TEXT,0};
    od_game_show_popup_text(list_of_strings,NULL,3);
    od_game_draw_game_back_button();
}


// 碰撞临时结构
typedef struct
{
    OD_S16    Value;
    OD_U8    Flag;
}OD_GAME_HIT_T;

// 交换
#define OD_GAME_SWAP(X, Y) { t = List[X]; List[X] = List[Y]; List[Y] = t; }
// 对4个元素排序
void od_game_sort4(OD_GAME_HIT_T *List)
{
    OD_GAME_HIT_T t;
    if(List[0].Value > List[2].Value) OD_GAME_SWAP(0, 2);
    if(List[1].Value > List[3].Value) OD_GAME_SWAP(1, 3);
    if(List[0].Value > List[1].Value) OD_GAME_SWAP(0, 1);
    if(List[2].Value > List[3].Value) OD_GAME_SWAP(2, 3);
    if(List[1].Value > List[2].Value) OD_GAME_SWAP(1, 2);         
} 
#undef OD_GAME_SWAP

// 碰撞检测
BOOL od_game_hit_check(od_game_rect_t *rc1, od_game_rect_t *rc2, od_game_rect_t *pOutRC)
{
    od_game_rect_t tmpRc;
    OD_GAME_HIT_T hsH[4] = {0};
    OD_GAME_HIT_T hsV[4] = {0};
    hsH[0].Value = rc1->left;
    hsH[1].Value = rc1->right;
    hsH[2].Value = rc2->left;
    hsH[3].Value = rc2->right;
    hsH[0].Flag = 1;
    hsH[1].Flag = 1;
    hsH[2].Flag = 2;
    hsH[3].Flag = 2;

    hsV[0].Value = rc1->top;
    hsV[1].Value = rc1->bottom;
    hsV[2].Value = rc2->top;
    hsV[3].Value = rc2->bottom;
    hsV[0].Flag = 1;
    hsV[1].Flag = 1;
    hsV[2].Flag = 2;
    hsV[3].Flag = 2;
    
    // 横向检测
    od_game_sort4(hsH);
    if(hsH[0].Flag == hsH[1].Flag) return FALSE;
    
    // 纵向检测
    od_game_sort4(hsV);
    if(hsV[0].Flag == hsV[1].Flag) return FALSE;
    
    // 保存碰撞的重合区域
    tmpRc.left = hsH[1].Value;
    tmpRc.right = hsH[2].Value;
    tmpRc.top = hsV[1].Value;
    tmpRc.bottom = hsV[2].Value;
    
    if(pOutRC) *pOutRC = tmpRc;
    return TRUE;
}
//end 公共UI部分


OD_GAME_MENU_T* od_game_get_menu_data()
{
      return &g_od_game_menu_data;
}

void od_game_read_nv(void* buffer_ptr,OD_U32 len)
{
    OD_U16 nv_return = 0;
    memset(buffer_ptr,0,len);
    OD_MMINV_READ(MMINV_OD_GAME_START, buffer_ptr, nv_return);  
    if (MN_RETURN_SUCCESS != nv_return)
    {
    	OD_MMINV_WRITE(MMINV_OD_GAME_START, buffer_ptr);
    }  
}


void od_game_write_nv(void* buffer_ptr,OD_U32 len)
{
    OD_MMINV_WRITE(MMINV_OD_GAME_START, buffer_ptr);
}


void od_game_read_nvram_by_id(OD_U32 nvram_id,    od_game_nv_t* data_ptr)
{
    OD_U16 nv_return = 0;	

    OD_MMINV_READ(nvram_id,data_ptr, nv_return);
    if (MN_RETURN_SUCCESS != nv_return || data_ptr->init == 0)
    {
        memset(data_ptr,0,sizeof(od_game_nv_t));
		data_ptr->init = 1;
        data_ptr->sound = OD_TRUE;
#ifdef OD_GAME_BILLING_SUPPORT
        data_ptr->free_trials = 5;
#endif
        OD_MMINV_WRITE(nvram_id,data_ptr);
    }
}


void od_game_read_nvram(OD_U32 nvram_id,od_game_nv_t* nv_data)
{
    od_game_read_nvram_by_id(nvram_id,nv_data);
}


void od_game_save_nvram(OD_U32 nvram_id,od_game_nv_t* nv_data)
{
    OD_MMINV_WRITE(nvram_id,nv_data);
}


OD_U8 od_game_get_nvram_level()
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    od_game_nv_t nv_data;
    od_game_read_nvram(menuDataPtr->nvram_id,&nv_data);
    return nv_data.level;
}


void od_game_set_nvram_level(OD_U8 level)
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    od_game_nv_t nv_data;
    od_game_read_nvram(menuDataPtr->nvram_id,&nv_data);
    nv_data.level = level;  
    od_game_save_nvram(menuDataPtr->nvram_id,&nv_data);
}

OD_U8 od_game_get_nvram_level_2()
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    od_game_nv_t nv_data;
    od_game_read_nvram(menuDataPtr->nvram_id,&nv_data);
    return nv_data.level_2;
}


void od_game_set_nvram_level_2(OD_U8 level)
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    od_game_nv_t nv_data;
    od_game_read_nvram(menuDataPtr->nvram_id,&nv_data);
    nv_data.level_2 = level;  
    od_game_save_nvram(menuDataPtr->nvram_id,&nv_data);
}


BOOL od_game_is_sound_on()
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    od_game_nv_t nv_data;
    od_game_read_nvram(menuDataPtr->nvram_id,&nv_data);
    return nv_data.sound;
}


OD_BOOL od_game_is_new_game()
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    return menuDataPtr->is_new_game;
}


OD_BOOL od_game_is_pause_game()
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    return menuDataPtr->is_pause;
}


void od_game_set_game_pause(OD_BOOL is_pause)
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    menuDataPtr->is_pause = is_pause;
}


OD_U8 od_game_get_current_level()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return od_game_get_nvram_level();
}


OD_U32 od_game_get_scores_by_level(OD_U8 level)
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    od_game_nv_t nv_data;
    od_game_read_nvram(menuDataPtr->nvram_id,&nv_data);
    return nv_data.scores_list[level];
}

void od_game_save_high_scores(OD_U32 scores,OD_U8 level_index)
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    od_game_nv_t nv_data;
    od_game_read_nvram(menuDataPtr->nvram_id,&nv_data);
   
    if(scores > nv_data.scores_list[level_index])
    {
        nv_data.scores_list[level_index] = scores;
        od_game_save_nvram(menuDataPtr->nvram_id,&nv_data);
    }
}

void od_game_stop_ring()
{
    MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
}


#include "mmi_ring.h"
void od_game_play_tone(OD_U32 ring_id,OD_U8 type)
{
    if(od_game_is_sound_on())
    {
        OD_U32 play_times = 0xffffffff;
    	MMIAUD_RING_DATA_INFO_T     ring_data = {MMISRVAUD_RING_FMT_MIDI, 0, NULL};/*lint !e64*/

        if(MMIAPIMP3_IsPlayerPlaying()||(FM_PLAYING == MMIAPIFM_GetStatus()))
        {
            return;
        }


		MMI_GetRingInfo(ring_id ,
			&ring_data);

		if(ring_data.data_len == 0 || ring_data.data_ptr == NULL)
		{
			return;
		}
        if(type == 0)
        {
            play_times = 1;
        }
		MMIAPISET_PlayRingByPtr(MMISET_RING_TYPE_OTHER,MMIAPISET_GetMultimVolume(),(uint8 *)ring_data.data_ptr,ring_data.data_len,play_times,MMISRVAUD_RING_FMT_MIDI,NULL);
    }
}
 
void od_game_menu_close_menu_list()
{
    MMK_CloseWin(OD_GAME_MENU_WIN_ID);
}

void od_game_menu_enter(uint32*win_table_ptr)
{
    MMK_CreateWin((uint32 *) win_table_ptr, PNULL);
}


void od_game_enter_new()
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    menuDataPtr->is_new_game = TRUE;
    menuDataPtr->is_pause = FALSE;
    if(menuDataPtr->enter_game_func_ptr)
    {
        menuDataPtr->enter_game_func_ptr();
    }
}


void od_game_enter_resume()
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();

	if(menuDataPtr->is_pause)
	{
    menuDataPtr->is_new_game = FALSE;
	}
	else
	{
		menuDataPtr->is_new_game = TRUE;
	}
    if(menuDataPtr->enter_game_func_ptr)
    {
        menuDataPtr->enter_game_func_ptr();
    }
}


void od_game_reset_high_scores(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    OD_U16 i;
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    od_game_nv_t nv_data;
    od_game_read_nvram(menuDataPtr->nvram_id,&nv_data);
    
    nv_data.level = 0;
    for (i = 0; i < OD_GAME_MAX_LEVEL_COUNT; i++)
    {
        nv_data.scores_list[i] = 0;
    }
    od_game_save_nvram(menuDataPtr->nvram_id,&nv_data);
    MMIPUB_OpenAlertSuccessWin(OD_GAME_DONE_TEXT);
    MMK_CloseWin(OD_GAME_HIGH_SCORES_WIN_ID);
}


LOCAL MMI_RESULT_E HandleRestHighScoresQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM              param      //IN:
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = OD_GAME_RESET_HIGH_SCORES_WIN_ID;    
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
        od_game_reset_high_scores();
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}


PUBLIC void od_game_reset_high_scores_confirm(void)
{
    MMI_STRING_T prompt_str = {0};
    MMI_WIN_ID_T query_win_id = OD_GAME_RESET_HIGH_SCORES_WIN_ID;
    MMI_GetLabelTextByLang(OD_GAME_RESET_HIGHT_SCORES_TEXT, &prompt_str);
    MMIPUB_OpenQueryWinByTextPtr(&prompt_str, PNULL, IMAGE_PUBWIN_QUERY, &query_win_id, HandleRestHighScoresQueryWinMsg);
}


LOCAL MMI_RESULT_E  Handle_od_game_high_scores_window(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
	uint32          group_id = 0; 
	uint32          menu_id  = 0;
	MMI_RESULT_E    recode   = MMI_RESULT_TRUE;
	MMI_STRING_T 	string = {0};
	OD_U8 i;
	OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
	OD_U8 buffer[64];
	OD_U16 buffer_UCS2[64];
	OD_U16* scores_text;
	od_game_nv_t nv_data;
    
	switch(msg_id)
	{
		case MSG_OPEN_WINDOW:			
                        GUIWIN_SetSoftkeyTextId(win_id,  OD_GAME_RESET_TEXT, TXT_NULL, OD_GAME_BACK_TEXT, FALSE);
                        MMK_SetAtvCtrl(win_id, OD_GAME_HIGH_SCORES_CTRL_ID);
                        SCI_MEMSET(&string, 0, sizeof(string));
                        MMI_GetLabelTextByLang(menuDataPtr->help_text_id, &string);
                        scores_text = od_game_malloc(512);
                        od_game_read_nvram(menuDataPtr->nvram_id,&nv_data);

                        for (i = 0; i < menuDataPtr->level_count; i++)
                        {
                            SCI_MEMSET(buffer,0,64);
                            SCI_MEMSET(buffer_UCS2,0,128);
                            SCI_MEMSET(&string, 0, sizeof(string));
                            MMI_GetLabelTextByLang(menuDataPtr->level_str_id_list[i], &string);
                            memcpy(buffer_UCS2,string.wstr_ptr,string.wstr_len*2);
                            MMIAPICOM_Wstrcat(scores_text,buffer_UCS2);
                            SCI_MEMSET(buffer_UCS2,0,128);
                    #ifdef OD_BUBBLE_PARTY_MIX_SUPPORT
                            if(menuDataPtr->game_menu_id == MENU_ID_OD_GAME_BUBBLE_PARTY_MIX &&i ==0)
                            sprintf((OD_S8*) buffer, "\n%d\n",nv_data.scores_list[i]);
                            else    
                    #endif
                    #ifdef OD_SUPER_PET_SUPPORT
                            if(menuDataPtr->game_menu_id == MENU_ID_OD_GAME_SUPER_PET &&i ==0)
                            sprintf((OD_S8*) buffer, "\n%d\n",nv_data.scores_list[i]);
                            else    
                    #endif
                            if(i == menuDataPtr->level_count-1)
                            sprintf((OD_S8*) buffer, ":\n%d", nv_data.scores_list[i]);
                            else
                            sprintf((OD_S8*) buffer, ":\n%d\n", nv_data.scores_list[i]);
                            od_game_utf8_to_wstr(buffer_UCS2,64,buffer);

                    #ifdef OD_BUBBLE_PARTY_MIX_SUPPORT
                            if(menuDataPtr->game_menu_id == MENU_ID_OD_GAME_BUBBLE_PARTY_MIX &&i ==0)
                            {
                                OD_U8* level_str;     
                                SCI_MEMSET(&string, 0, sizeof(string));
                                MMI_GetLabelTextByLang(OD_GAME_BUBBLE_PARTY_MIX_LEVEL_TEXT, &string);
                                level_str= od_game_get_format_string(string.wstr_ptr,string.wstr_len,od_game_get_nvram_level_2()+1,0);     

                                MMIAPICOM_Wstrcat(scores_text,L":\n");
                                MMIAPICOM_Wstrcat(scores_text,(OD_U16*)level_str);
                                od_game_free(level_str);
                            }
                    #endif                
                    #ifdef OD_SUPER_PET_SUPPORT
                            if(menuDataPtr->game_menu_id == MENU_ID_OD_GAME_SUPER_PET &&i ==0)
                            {
                                OD_U8* level_str;     
                                SCI_MEMSET(&string, 0, sizeof(string));
                                MMI_GetLabelTextByLang(OD_GAME_SUPER_PET_LEVEL_TEXT, &string);
                                level_str= od_game_get_format_string(string.wstr_ptr,string.wstr_len,od_game_get_nvram_level_2()+1,0);     

                                MMIAPICOM_Wstrcat(scores_text,L":\n");
                                MMIAPICOM_Wstrcat(scores_text,(OD_U16*)level_str);
                                od_game_free(level_str);
                            }
                    #endif                

                            MMIAPICOM_Wstrcat(scores_text,buffer_UCS2);
                        }
			 GUITEXT_SetString(OD_GAME_HIGH_SCORES_CTRL_ID,scores_text,od_game_wstrlen(scores_text), FALSE);
                      od_game_free(scores_text);  
     			 break;
    		case MSG_CTL_PENOK:
		case MSG_CTL_MIDSK:
		case MSG_APP_WEB:
		case MSG_CTL_OK:
		case MSG_APP_OK:
                    od_game_reset_high_scores_confirm();
                    break;
		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(win_id);
			break;

		default:
			recode = MMI_RESULT_FALSE;
			break;
	}
	return recode;
}


WINDOW_TABLE( MMI_OD_GAME_HIGH_SCORES_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)Handle_od_game_high_scores_window ),    
	WIN_ID(OD_GAME_HIGH_SCORES_WIN_ID ),
	WIN_TITLE(OD_GAME_HIGH_SCORE_TEXT),
	WIN_SOFTKEY(OD_GAME_RESET_TEXT, TXT_NULL, OD_GAME_BACK_TEXT),
	CREATE_TEXT_CTRL(OD_GAME_HIGH_SCORES_CTRL_ID),
	END_WIN
};


void od_game_enter_high_scores()
{
    MMK_CreateWin((uint32 *) MMI_OD_GAME_HIGH_SCORES_WIN_TAB, PNULL);
}


void od_game_set_game_sound(OD_U8 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    od_game_nv_t nv_data;
    od_game_read_nvram(menuDataPtr->nvram_id,&nv_data);
    nv_data.sound = index;
    od_game_save_nvram(menuDataPtr->nvram_id,&nv_data);
    MMIPUB_OpenAlertSuccessWin(OD_GAME_DONE_TEXT);
}


LOCAL MMI_RESULT_E Handle_od_game_sound_window(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();

    switch ( msg_id )
    {
        case MSG_OPEN_WINDOW:
            {
                GUILIST_ITEM_T      item_t = {0};
                GUILIST_ITEM_DATA_T item_data = {0};
                od_game_nv_t nv_data;
                od_game_read_nvram(menuDataPtr->nvram_id,&nv_data);

                MMK_SetAtvCtrl(win_id,OD_GAME_SOUND_CTRL_ID);
   
                GUILIST_SetMaxItem(OD_GAME_SOUND_CTRL_ID, 2, FALSE );//max item 33

                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
                item_t.item_data_ptr = &item_data;
                item_t.user_data = 0;
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = OD_GAME_OFF_TEXT;
                GUILIST_AppendItem(OD_GAME_SOUND_CTRL_ID, &item_t);
                item_data.item_content[0].item_data.text_id = OD_GAME_ON_TEXT;
                item_t.user_data++;
                GUILIST_AppendItem(OD_GAME_SOUND_CTRL_ID, &item_t);
                GUILIST_SetCurItemIndex(OD_GAME_SOUND_CTRL_ID,nv_data.sound);
                GUILIST_SetSelectedItem(OD_GAME_SOUND_CTRL_ID, nv_data.sound,TRUE);
                //GUIWIN_SetSoftkeyTextId(win_id,OD_GAME_OK_TEXT,TXT_NULL,OD_GAME_BACK_TEXT,FALSE);
            }
            break;
        case MSG_CTL_PENOK:
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
        case MSG_APP_OK:
            {
                OD_U16  cur_selection   =   0;
                GUILIST_GetSelectedItemIndex(OD_GAME_SOUND_CTRL_ID,&cur_selection,1);
                od_game_set_game_sound(cur_selection);
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        case MSG_APP_RED:
            MMK_CloseWin(win_id);
            break;
        case MSG_FULL_PAINT:
            break;
    }

    return recode;
}



WINDOW_TABLE(MMI_OD_GAME_SOUND_WIN_TAB) = 
{
    WIN_FUNC((uint32)Handle_od_game_sound_window),
    WIN_ID(OD_GAME_SOUND_WIN_ID),
    WIN_TITLE(OD_GAME_SOUND_TEXT),
    WIN_SOFTKEY(OD_GAME_OK_TEXT, TXT_NULL, OD_GAME_BACK_TEXT),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, OD_GAME_SOUND_CTRL_ID),
    END_WIN
};

void od_game_enter_sound()
{
    MMK_CreateWin((uint32 *) MMI_OD_GAME_SOUND_WIN_TAB, PNULL);
}


LOCAL MMI_RESULT_E  Handle_od_game_help_window(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
	uint32          group_id = 0; 
	uint32          menu_id  = 0;
	MMI_RESULT_E    recode   = MMI_RESULT_TRUE;
	MMI_STRING_T 	string = {0};
       OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();

	switch(msg_id)
	{
		case MSG_OPEN_WINDOW:			
			GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, OD_GAME_BACK_TEXT, FALSE);
			MMK_SetAtvCtrl(win_id, OD_GAME_HELP_CTRL_ID);
			SCI_MEMSET(&string, 0, sizeof(string));
			MMI_GetLabelTextByLang(menuDataPtr->help_text_id, &string);
			 GUITEXT_SetString(OD_GAME_HELP_CTRL_ID, string.wstr_ptr, string.wstr_len, FALSE);
			break;

		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(win_id);
			break;

		default:
			recode = MMI_RESULT_FALSE;
			break;
	}
	return recode;
}


WINDOW_TABLE( MMI_OD_GAME_HELP_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)Handle_od_game_help_window ),    
	WIN_ID( OD_GAME_HELP_WIN_ID ),
	WIN_TITLE(OD_GAME_HELP_TEXT),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, OD_GAME_BACK_TEXT),
	CREATE_TEXT_CTRL(OD_GAME_HELP_CTRL_ID),
	END_WIN
};


void od_game_enter_help()
{
    MMK_CreateWin((uint32 *) MMI_OD_GAME_HELP_WIN_TAB, PNULL);
}


void od_game_enter_new_by_level(OD_U8 level)
{

	od_game_set_nvram_level(level);
    od_game_enter_new();
}


#ifndef OD_GAME_BILLING_SUPPORT
OD_U32 od_game_free_play_once(OD_U16 code)
{
    return code;
}

OD_U16 od_game_get_project_id(OD_GAME_TYPE game_type)
{
    return game_type;
}
OD_BOOL od_game_auto_unlock_code_by_sms(OD_GAME_SIM_TYPE sim_type,OD_U8* number,OD_U8* content)
{
	return OD_FALSE;
}
OD_BOOL od_game_debug_ProcessEntrStr(OD_U8* str_ptr, OD_U16 str_len)
{
	return OD_FALSE;	
}
#endif


LOCAL MMI_RESULT_E  Handle_od_game_menu_window(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
	uint32          group_id = 0; 
	uint32          menu_id  = 0;
	MMI_RESULT_E    recode   = MMI_RESULT_TRUE;
	MMI_STRING_T menu_title = {0};
	MMI_STRING_T title_string = {0};
	MMI_STRING_T content_string = {0};
       OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();

	GUIMENU_GetId(OD_GAME_MENU_CTRL_ID,&group_id,&menu_id);	
	switch(msg_id)
	{
		case MSG_OPEN_WINDOW:
			MMK_SetAtvCtrl(win_id, OD_GAME_MENU_CTRL_ID);
  			MMI_GetLabelTextByLang(menuDataPtr->game_title_id, &menu_title);	
			GUIMENU_SetMenuTitle(&menu_title, OD_GAME_MENU_CTRL_ID);
			break;
		case MSG_FULL_PAINT:
		#if 0//不隐藏Resume菜单	
             if(menuDataPtr->is_pause)
             {
				GUIMENU_SetItemVisible(OD_GAME_MENU_CTRL_ID,group_id,MENU_ID_OD_GAME_RESUME,TRUE);
             }
             else
             {
				GUIMENU_SetItemVisible(OD_GAME_MENU_CTRL_ID,group_id,MENU_ID_OD_GAME_RESUME,FALSE);
             }
		#endif	 
              break;
		case MSG_CTL_PENOK:
		case MSG_CTL_MIDSK:
		case MSG_APP_WEB:
		case MSG_CTL_OK:
		case MSG_APP_OK:
			switch(menu_id) 
			{
                case MENU_ID_OD_GAME_RESUME:
                    od_game_enter_resume();
                    break;
                case MENU_ID_OD_GAME_NEW:
                    od_game_enter_new_by_level(0);
                    break;
                case MENU_ID_OD_GAME_NEW_2:
                    od_game_enter_new_by_level(1);
                    break;
                case MENU_ID_OD_GAME_NEW_3:
                    od_game_enter_new_by_level(2);
                    break;
                case MENU_ID_OD_GAME_HIGH_SCORES:
                    od_game_enter_high_scores();
                    break;
                case MENU_ID_OD_GAME_SOUND:
                    od_game_enter_sound();
                    break;
                case MENU_ID_OD_GAME_HELP:
                    od_game_enter_help();
                    break;	
				default:
				break;
			}
			break;    

		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(win_id);
			break;
		default:
			recode = MMI_RESULT_FALSE;
			break;
	}

	return recode;
}



#ifdef OD_DOODLE_JUMP_SUPPORT
extern BOOL od_doodle_jump_is_pause();
extern void od_doodle_jump_enter(void);

WINDOW_TABLE(MMI_OD_GAME_DOODLE_JUMP_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)Handle_od_game_menu_window ),    
	WIN_ID( OD_GAME_MENU_WIN_ID ),
	WIN_TITLE(OD_GAME_DOODLE_JUMP_TEXT),
	WIN_SOFTKEY(OD_GAME_OK_TEXT, TXT_NULL, OD_GAME_BACK_TEXT),
	CREATE_MENU_CTRL(MENU_ID_OD_GAME_DOODLE_JUMP, OD_GAME_MENU_CTRL_ID),
	END_WIN
};

void od_game_entry_doodle_jump()
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    memset(menuDataPtr,0,sizeof(OD_GAME_MENU_T));

    menuDataPtr->game_menu_id = MENU_ID_OD_GAME_DOODLE_JUMP;
    menuDataPtr->game_title_id = OD_GAME_DOODLE_JUMP_TEXT;
    menuDataPtr->help_text_id = OD_GAME_DOODLE_JUMP_HELP_TEXT;
    menuDataPtr->level_count = 3;
    menuDataPtr->level_str_id_list[0] = OD_GAME_CLASSIC_NEW_TEXT;
    menuDataPtr->level_str_id_list[1] = OD_GAME_JUNGLE_NEW_TEXT;
    menuDataPtr->level_str_id_list[2] = OD_GAME_SPACE_NEW_TEXT;
    menuDataPtr->is_pause = od_doodle_jump_is_pause();
    menuDataPtr->is_new_game = TRUE;
    menuDataPtr->enter_game_func_ptr = od_doodle_jump_enter;
    menuDataPtr->nvram_id = MMINV_OD_DOODLE_JUMP_NV_ID;
    od_game_menu_enter(MMI_OD_GAME_DOODLE_JUMP_WIN_TAB);
}


void od_game_entry_doodle_jump_with_billing()
{
#ifdef OD_GAME_BILLING_SUPPORT
    od_game_entry_billing(OD_GAME_DOODLE_JUMP,od_game_entry_doodle_jump);
#else
    od_game_entry_doodle_jump();
#endif
}
#endif


#ifdef OD_CROSSY_ROAD_SUPPORT
extern OD_BOOL od_crossy_road_is_pause();
extern void od_crossy_road_enter(void);

WINDOW_TABLE(MMI_OD_GAME_CROSSY_ROAD_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)Handle_od_game_menu_window ),    
	WIN_ID( OD_GAME_MENU_WIN_ID ),
	WIN_TITLE(OD_GAME_CROSSY_ROAD_TEXT),
	WIN_SOFTKEY(OD_GAME_OK_TEXT, TXT_NULL, OD_GAME_BACK_TEXT),
	CREATE_MENU_CTRL(MENU_ID_OD_GAME_CROSSY_ROAD, OD_GAME_MENU_CTRL_ID),
	END_WIN
};

void od_game_entry_crossy_road()
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    memset(menuDataPtr,0,sizeof(OD_GAME_MENU_T));

    menuDataPtr->game_menu_id = MENU_ID_OD_GAME_CROSSY_ROAD;
    menuDataPtr->game_title_id = OD_GAME_CROSSY_ROAD_TEXT;
    menuDataPtr->help_text_id = OD_GAME_CROSSY_ROAD_HELP_TEXT;
    menuDataPtr->level_count = 3;
    menuDataPtr->level_str_id_list[0] = OD_GAME_COCK_NEW_TEXT;
    menuDataPtr->level_str_id_list[1] = OD_GAME_CAT_NEW_TEXT;
    menuDataPtr->level_str_id_list[2] = OD_GAME_ROBOT_NEW_TEXT;   
    menuDataPtr->is_pause = od_crossy_road_is_pause();
    menuDataPtr->is_new_game = TRUE;
    menuDataPtr->enter_game_func_ptr = od_crossy_road_enter;
    menuDataPtr->nvram_id = MMINV_OD_CROSSY_ROAD_NV_ID;
    od_game_menu_enter(MMI_OD_GAME_CROSSY_ROAD_WIN_TAB);
}


void od_game_entry_crossy_road_with_billing()
{
#ifdef OD_GAME_BILLING_SUPPORT
    od_game_entry_billing(OD_GAME_CROSSY_ROAD,od_game_entry_crossy_road);
#else
    od_game_entry_crossy_road();
#endif
}
#endif

#ifdef OD_STICK_HERO_SUPPORT
extern OD_BOOL od_stick_hero_is_pause();
extern void od_stick_hero_enter(void);

WINDOW_TABLE(MMI_OD_GAME_STICK_HERO_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)Handle_od_game_menu_window ),    
	WIN_ID( OD_GAME_MENU_WIN_ID ),
	WIN_TITLE(OD_GAME_STICK_HERO_TEXT),
	WIN_SOFTKEY(OD_GAME_OK_TEXT, TXT_NULL, OD_GAME_BACK_TEXT),
	CREATE_MENU_CTRL(MENU_ID_OD_GAME_STICK_HERO, OD_GAME_MENU_CTRL_ID),
	END_WIN
};

void od_game_entry_stick_hero()
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    memset(menuDataPtr,0,sizeof(OD_GAME_MENU_T));

    menuDataPtr->game_menu_id = MENU_ID_OD_GAME_STICK_HERO;
    menuDataPtr->game_title_id = OD_GAME_STICK_HERO_TEXT;
    menuDataPtr->help_text_id = OD_GAME_STICK_HERO_HELP_TEXT;
    menuDataPtr->level_count = 3;
    menuDataPtr->level_str_id_list[0] = OD_GAME_STICK_HERO_CLASSIC_NEW_TEXT;
    menuDataPtr->level_str_id_list[1] = OD_GAME_STICK_HERO_ATHLETIC_NEW_TEXT;
    menuDataPtr->level_str_id_list[2] = OD_GAME_STICK_HERO_FUNKY_NEW_TEXT;
    menuDataPtr->is_pause = od_stick_hero_is_pause();
    menuDataPtr->is_new_game = OD_TRUE;
    menuDataPtr->enter_game_func_ptr = od_stick_hero_enter;
    menuDataPtr->nvram_id = MMINV_STICK_HERO_NV_ID;
    od_game_menu_enter(MMI_OD_GAME_STICK_HERO_WIN_TAB);
}


void od_game_entry_stick_hero_with_billing()
{
#ifdef OD_GAME_BILLING_SUPPORT
    od_game_entry_billing(OD_GAME_STICK_HERO,od_game_entry_stick_hero);
#else
    od_game_entry_stick_hero();
#endif
}
#endif


#ifdef OD_BUBBLE_PARTY_MIX_SUPPORT
extern OD_BOOL od_bubble_party_mix_is_pause();
extern void od_bubble_party_mix_enter(void);

WINDOW_TABLE(MMI_OD_GAME_BUBBLE_PARTY_MIX_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)Handle_od_game_menu_window ),    
	WIN_ID( OD_GAME_MENU_WIN_ID ),
	WIN_TITLE(OD_GAME_BUBBLE_PARTY_MIX_TEXT),
	WIN_SOFTKEY(OD_GAME_OK_TEXT, TXT_NULL, OD_GAME_BACK_TEXT),
	CREATE_MENU_CTRL(MENU_ID_OD_GAME_BUBBLE_PARTY_MIX, OD_GAME_MENU_CTRL_ID),
	END_WIN
};

void od_game_entry_bubble_party_mix()
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    memset(menuDataPtr,0,sizeof(OD_GAME_MENU_T));

    menuDataPtr->game_menu_id = MENU_ID_OD_GAME_BUBBLE_PARTY_MIX;
    menuDataPtr->game_title_id = OD_GAME_BUBBLE_PARTY_MIX_TEXT;
    menuDataPtr->help_text_id = OD_GAME_BUBBLE_PARTY_MIX_HELP_TEXT;
    menuDataPtr->level_count = 2;
    menuDataPtr->level_str_id_list[0] = OD_GAME_BUBBLE_PARTY_MIX_NEW_TEXT;
    menuDataPtr->level_str_id_list[1] = OD_GAME_BUBBLE_PARTY_MIX_NEW2_TEXT;
    menuDataPtr->is_pause = od_bubble_party_mix_is_pause();
    menuDataPtr->is_new_game = TRUE;
    menuDataPtr->enter_game_func_ptr = od_bubble_party_mix_enter;
    menuDataPtr->nvram_id = MMINV_BUBBLE_PARTY_MIX_NV_ID;
    od_game_menu_enter(MMI_OD_GAME_BUBBLE_PARTY_MIX_WIN_TAB);
}

void od_game_entry_bubble_party_mix_with_billing()
{
#ifdef OD_BUBBLE_PARTY_MIX_FREE_SUPPORT
    od_game_entry_bubble_party_mix();
#else
    #ifdef OD_GAME_BILLING_SUPPORT
        od_game_entry_billing(OD_GAME_BUBBLE_PARTY_MIX,od_game_entry_bubble_party_mix);
    #else
        od_game_entry_bubble_party_mix();
    #endif
#endif
}
#endif



#ifdef OD_SUPER_PET_SUPPORT
extern OD_BOOL od_super_pet_is_pause();
extern void od_super_pet_enter(void);

WINDOW_TABLE(MMI_OD_GAME_SUPER_PET_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)Handle_od_game_menu_window ),    
	WIN_ID( OD_GAME_MENU_WIN_ID ),
	WIN_TITLE(OD_GAME_SUPER_PET_TEXT),
	WIN_SOFTKEY(OD_GAME_OK_TEXT, TXT_NULL, OD_GAME_BACK_TEXT),
	CREATE_MENU_CTRL(MENU_ID_OD_GAME_SUPER_PET, OD_GAME_MENU_CTRL_ID),
	END_WIN
};

void od_game_entry_super_pet()
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    memset(menuDataPtr,0,sizeof(OD_GAME_MENU_T));

    menuDataPtr->game_menu_id = MENU_ID_OD_GAME_SUPER_PET;
    menuDataPtr->game_title_id = OD_GAME_SUPER_PET_TEXT;
    menuDataPtr->help_text_id = OD_GAME_SUPER_PET_HELP_TEXT;
    menuDataPtr->level_count = 2;
    menuDataPtr->level_str_id_list[0] = OD_GAME_SUPER_PET_NEW_TEXT;
    menuDataPtr->level_str_id_list[1] = OD_GAME_SUPER_PET_NEW2_TEXT;
    menuDataPtr->is_pause = od_super_pet_is_pause();
    menuDataPtr->is_new_game = TRUE;
    menuDataPtr->enter_game_func_ptr = od_super_pet_enter;
    menuDataPtr->nvram_id = MMINV_SUPER_PET_NV_ID;
    od_game_menu_enter(MMI_OD_GAME_SUPER_PET_WIN_TAB);
}

void od_game_entry_super_pet_with_billing()
{
    od_game_entry_super_pet();
}
#endif

#ifdef OD_RUNNER_SUPPORT
extern BOOL od_runner_is_pause();
extern void od_runner_enter(void);

WINDOW_TABLE(MMI_OD_GAME_RUNNER_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)Handle_od_game_menu_window ),    
	WIN_ID( OD_GAME_MENU_WIN_ID ),
	WIN_TITLE(OD_GAME_RUNNER_TEXT),
	WIN_SOFTKEY(OD_GAME_OK_TEXT, TXT_NULL, OD_GAME_BACK_TEXT),
	CREATE_MENU_CTRL(MENU_ID_OD_GAME_RUNNER, OD_GAME_MENU_CTRL_ID),
	END_WIN
};

void od_game_entry_runner()
{
    OD_GAME_MENU_T* menuDataPtr = od_game_get_menu_data();
    memset(menuDataPtr,0,sizeof(OD_GAME_MENU_T));

    menuDataPtr->game_menu_id = MENU_ID_OD_GAME_RUNNER;
    menuDataPtr->game_title_id = OD_GAME_RUNNER_TEXT;
    menuDataPtr->help_text_id = OD_GAME_RUNNER_HELP_TEXT;
    menuDataPtr->level_count = 1;
    menuDataPtr->level_str_id_list[0] = OD_GAME_RUNNER_NEW_TEXT;
    menuDataPtr->is_pause = od_runner_is_pause();
    menuDataPtr->is_new_game = TRUE;
    menuDataPtr->enter_game_func_ptr = od_runner_enter;
    menuDataPtr->nvram_id = MMINV_RUNNER_NV_ID;
    od_game_menu_enter(MMI_OD_GAME_RUNNER_WIN_TAB);
}


void od_game_entry_runner_with_billing()
{
    od_game_entry_runner();
}
#endif


BOOLEAN od_game_handle_menu(MMI_MENU_ID_T menu_id)
{
	BOOLEAN is_od_game_menu=TRUE;
	switch(menu_id)
  	{
#ifdef OD_DOODLE_JUMP_SUPPORT
              case MENU_ID_OD_GAME_DOODLE_JUMP:						
					 od_game_entry_doodle_jump_with_billing();
                     break;
#endif                     
#ifdef OD_CROSSY_ROAD_SUPPORT
              case MENU_ID_OD_GAME_CROSSY_ROAD:
                     od_game_entry_crossy_road_with_billing();
                     break;
#endif                     
#ifdef OD_STICK_HERO_SUPPORT
              case MENU_ID_OD_GAME_STICK_HERO:
                     od_game_entry_stick_hero_with_billing();
                     break;
#endif                     
#ifdef OD_BUBBLE_PARTY_MIX_SUPPORT
              case MENU_ID_OD_GAME_BUBBLE_PARTY_MIX:
                     od_game_entry_bubble_party_mix_with_billing();
                     break;
#endif                     
#ifdef OD_SUPER_PET_SUPPORT
              case MENU_ID_OD_GAME_SUPER_PET:
                     od_game_entry_super_pet_with_billing();
                     break;
#endif              
#ifdef OD_RUNNER_SUPPORT
              case MENU_ID_OD_GAME_RUNNER:
                     od_game_entry_runner_with_billing();
                     break;
#endif           
		default:
			is_od_game_menu=OD_FALSE;
			break;
	}
	return is_od_game_menu;
}
#endif
