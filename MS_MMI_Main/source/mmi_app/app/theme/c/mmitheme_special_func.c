/****************************************************************************
** File Name:      mmitheme_special_func.c                                             *
** Author:         bin.ji                                              *
** Date:           04/29/2008                                             *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    ������Ч            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/29/2008     bin.ji       Create
** 
****************************************************************************/
#define _MMITHEME_SPECIAL_FUNC_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
//#include "sci_types.h"
#include "mmidisplay_data.h"
#include "mmitheme_special_func.h"
#include "mmipicview_export.h"
#include "mmitheme_special.h"
#include "mmitheme_pos.h"
#include "guilcd.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#if defined (MAINLCD_SIZE_128X160)
    #define MMI_EFFECT_BOX_ENLARGE_STEP_X   10
    #define MMI_EFFECT_BOX_ENLARGE_STEP_Y   20

    #define MMI_EFFECT_HORIZONTAL_GRID_STEP_X   44
    #define MMI_EFFECT_HORIZONTAL_GRID_STEP_Y   22
    /*fixed by langhua.yang for CR:NEWMS00207729 */
    #define MMI_EFFECT_LEFT_RIGHTWARD_FRAME_NUM 16
    #define MMI_EFFECT_UP_DOWNWARD_FRAME_NUM    16
#elif defined(MAINLCD_SIZE_176X220)
    #define MMI_EFFECT_BOX_ENLARGE_STEP_X   10
    #define MMI_EFFECT_BOX_ENLARGE_STEP_Y   20

    #define MMI_EFFECT_HORIZONTAL_GRID_STEP_X   44
    #define MMI_EFFECT_HORIZONTAL_GRID_STEP_Y   22
    
    #define MMI_EFFECT_LEFT_RIGHTWARD_FRAME_NUM 22
    #define MMI_EFFECT_UP_DOWNWARD_FRAME_NUM    22
#elif defined(MAINLCD_SIZE_320X480)
    #define MMI_EFFECT_BOX_ENLARGE_STEP_X   40
    #define MMI_EFFECT_BOX_ENLARGE_STEP_Y   60
    
    #define MMI_EFFECT_HORIZONTAL_GRID_STEP_X   40
    #define MMI_EFFECT_HORIZONTAL_GRID_STEP_Y   60

    #define MMI_EFFECT_LEFT_RIGHTWARD_FRAME_NUM 40
    #define MMI_EFFECT_UP_DOWNWARD_FRAME_NUM    30    
#else
    #define MMI_EFFECT_BOX_ENLARGE_STEP_X   20
    #define MMI_EFFECT_BOX_ENLARGE_STEP_Y   30
    
    #define MMI_EFFECT_HORIZONTAL_GRID_STEP_X   60
    #define MMI_EFFECT_HORIZONTAL_GRID_STEP_Y   40

    #define MMI_EFFECT_LEFT_RIGHTWARD_FRAME_NUM 30
    #define MMI_EFFECT_UP_DOWNWARD_FRAME_NUM    40
#endif

typedef enum
{
    MMI_TRANSITION_EFFECT_NONE,         //��Ч��
    MMI_TRANSITION_EFFECT_BOX_ENLARGE,  //��״չ��
//    MMI_TRANSITION_EFFECT_BOX_RETRENCH, //��״����
    MMI_TRANSITION_EFFECT_HORIZONTAL_GRID, //��������ʽ
    MMI_TRANSITION_EFFECT_LEFTWARD_ERASE, //�������
    MMI_TRANSITION_EFFECT_RIGHTWARD_ERASE, //���Ҳ���
    MMI_TRANSITION_EFFECT_UPWARD_ERASE, //���ϲ���
    MMI_TRANSITION_EFFECT_DOWNWARD_ERASE, //���²���
#ifdef EFFECT_RIPPLE_SUPPORT      
    MMI_TRANSITION_EFFECT_RIPPLE_FADE, //ripple fade in out.    
#endif    
    MMI_TRANSITION_EFFECT_FADE_IN, //����
    MMI_TRANSITION_EFFECT_MAX           
}MMI_TRANSITION_EFFECT_E;   //�л�Ч��

typedef struct 
{
    uint32 frame_num;   //ˢ�µ�֡��
    uint32 delay_time;  //ÿ֡ˢ����ʱʱ��
    uint32 step_x;  //ÿ֡ˢ����ʱʱ��
    uint32 step_y;  //ÿ֡ˢ����ʱʱ��
}MMI_TRANSITION_BOX_PARAM_T;    //��״չ��/��������

typedef struct 
{
    uint32 frame_num;   //ˢ�µ�֡���������ܱ�grid_width������
    uint32 delay_time;  //ÿ֡ˢ����ʱʱ��
    uint32 grid_width;  //�����ȣ������ܱ�MMI_MAINSCREEN_WIDTH������
    uint32 grid_height; //����߶ȣ������ܱ�MMI_MAINSCREEN_HEIGHT������
}MMI_TRANSITION_GRID_PARAM_T;    //����ʽ����

typedef struct 
{
    uint32 frame_num;   //ˢ�µ�֡��
    uint32 delay_time;  //ÿ֡ˢ����ʱʱ��
}MMI_TRANSITION_ERASE_PARAM_T;

typedef union
{
    MMI_TRANSITION_BOX_PARAM_T box_param;   //��״��Ч����
    MMI_TRANSITION_GRID_PARAM_T grid_param; //����ʽ��Ч����
    MMI_TRANSITION_ERASE_PARAM_T erase_param; //����ʽ
}MMI_TRANSITION_PARAM_U;    //�л�Ч������

typedef struct
{
    MMI_TRANSITION_EFFECT_E type;   //�л�Ч������
    MMI_TRANSITION_PARAM_U param;   //�л�Ч������
}MMI_TRANSITION_EFFECT_T;   //�л�Ч��

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

//����ָʾ����һ��Ӧ�ã�mainmenu��picslide, picfullscreen���õ���ˢ����Ч����ͬ��Ӧ����Ҫ����Ч��ͬ
MMITHEME_SPECIAL_TYPE_E g_special_effect_dirty = MMITHEME_SPECIAL_NONE;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : ��״չ����ʽˢ��ȫ��
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void UpdateFullScreenByBoxEnlarge(
                                        MMI_TRANSITION_BOX_PARAM_T *param_ptr
                                        );

/*****************************************************************************/
// 	Description : �������̷�ʽˢ��ȫ��
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void UpdateFullScreenByHorizontalGrid(
                                            MMI_TRANSITION_GRID_PARAM_T *param_ptr
                                            );

/*****************************************************************************/
// 	Description : �������Ч��
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
void MMITHEME_GetTransitionEffect(
                                  MMI_TRANSITION_EFFECT_T *effect_ptr,
                                  MMITHEME_SPECIAL_TYPE_E type
                                  );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : ��״չ����ʽˢ��ȫ��
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void UpdateFullScreenByBoxEnlarge(
                                        MMI_TRANSITION_BOX_PARAM_T *param_ptr
                                        )
{
    GUI_RECT_T rect = {0};
    int16 i = 0;
    int16 step_x = 0;
    int16 step_y = 0;
    int16 frame_num = 0;
    uint32 delay_time = 0;
    uint16     lcd_width   = 0;
    uint16     lcd_height  = 0;

    if (PNULL == param_ptr)
    {
        return;
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);    

    frame_num = param_ptr->frame_num;
    step_x = param_ptr->step_x;
    step_y = param_ptr->step_y;
    delay_time = param_ptr->delay_time;

    rect.left = step_x*frame_num;
    rect.top = step_y*frame_num;
    rect.right = (lcd_width-rect.left);
    rect.bottom = (lcd_height-rect.top);

    for (i = frame_num; i > 0; i--)
    {    
        rect.left -= step_x;
        rect.top -= step_y;
        rect.right += step_x;
        rect.bottom += step_y;

        GUILCD_InvalidateRect(
            GUI_MAIN_LCD_ID, 
            rect,
            0);  
        SCI_Sleep(delay_time);
    }
}

/*****************************************************************************/
// 	Description : �������̷�ʽˢ��ȫ��
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void UpdateFullScreenByHorizontalGrid(
                                            MMI_TRANSITION_GRID_PARAM_T *param_ptr
                                            )
{
    GUI_RECT_T rect = {0};
    int32 i = 0;
    int32 frame_num = 0;
    int32 delay_time = 0;
    int32 grid_width = 0;
    int32 grid_height = 0;
    int32 grid_x_num = 0;
    int32 grid_y_num = 0;
    int32 frame_width = 0;
    int32 x = 0;
    int32 y = 0;
    int32 offset = 0;
    uint16 lcd_width   = 0;
    uint16 lcd_height  = 0;

    if (PNULL == param_ptr)
    {
        return;
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);  
    
    frame_num = param_ptr->frame_num;
    delay_time = param_ptr->delay_time;
    grid_width = param_ptr->grid_width;
    grid_height = param_ptr->grid_height;
    grid_x_num = (lcd_width / grid_width)>>1;
    grid_y_num = (lcd_height / grid_height);
    SCI_ASSERT(0 != frame_num); /*assert verified*/
    frame_width = (grid_width/frame_num);

    offset = 0;
    for (i = 0; i < (frame_num<<1); i++)
    {
        rect.top = 0;
        rect.bottom = (grid_height-1);       
        
        for (y = 0; y < grid_y_num; y++)
        {
            if (y&0x01)
            {
                if (offset >= grid_width)
                {
                    rect.left = (offset-grid_width);
                }
                else
                {
                    rect.left = (offset+grid_width);
                }
            }
            else
            {
                rect.left = offset;
            }
            rect.right = (rect.left+frame_width-1);
            for (x = 0; x < grid_x_num; x++)
            {
                GUILCD_InvalidateRect(
                    GUI_MAIN_LCD_ID, 
                    rect,
                    0); 
                
                rect.left += (grid_width<<1);
                rect.right += (grid_width<<1);
            }
            rect.top += grid_height;
            rect.bottom += grid_height;
        }
        offset += frame_width;
        SCI_Sleep(delay_time);
    }

    rect.top  = 0;
    rect.left = 0;
    rect.right = lcd_width - 1;
    rect.bottom = lcd_height - 1;

    GUILCD_InvalidateRect(
        GUI_MAIN_LCD_ID, 
        rect,
        0);     
}

/*****************************************************************************/
// 	Description : �������Ч��
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
void MMITHEME_GetTransitionEffect(
                                  MMI_TRANSITION_EFFECT_T   *effect_ptr,
                                  MMITHEME_SPECIAL_TYPE_E   type
                                  )
{
    static uint32 s_effect_type = 0;
    
    if (PNULL == effect_ptr)
    {
        return;
    }

    switch (type)
    {
    case MMITHEME_SPECIAL_MAINMENU:
        effect_ptr->type = MMI_TRANSITION_EFFECT_NONE;//MMI_TRANSITION_EFFECT_BOX_ENLARGE;
        break;

    case MMITHEME_SPECIAL_PIC_SLIDE:
        //get picture slide type
        {
            MMIPICVIEW_EFFECT_TYPE_E    slide_effect = MMIAPIPICVIEW_GetSlideEffect();

            switch (slide_effect)
            {
            case MMIPICVIEW_EFFECT_NONE:
                s_effect_type = MMI_TRANSITION_EFFECT_NONE;
                break;

            case MMIPICVIEW_EFFECT_ENLARGE:
                s_effect_type = MMI_TRANSITION_EFFECT_BOX_ENLARGE;
                break;

            case MMIPICVIEW_EFFECT_GRID:
                s_effect_type = MMI_TRANSITION_EFFECT_HORIZONTAL_GRID;
                break;

            case MMIPICVIEW_EFFECT_RIGHTWARD:
                s_effect_type = MMI_TRANSITION_EFFECT_RIGHTWARD_ERASE;
                break;

            case MMIPICVIEW_EFFECT_LEFTWARD:
                s_effect_type = MMI_TRANSITION_EFFECT_LEFTWARD_ERASE;
                break;

            case MMIPICVIEW_EFFECT_UPWARD:
                s_effect_type = MMI_TRANSITION_EFFECT_UPWARD_ERASE;
                break;

            case MMIPICVIEW_EFFECT_DOWNWARD:
                s_effect_type = MMI_TRANSITION_EFFECT_DOWNWARD_ERASE;
                break;
#ifdef EFFECT_RIPPLE_SUPPORT  
            case MMIPICVIEW_EFFECT_RIPPLE:
                s_effect_type = MMI_TRANSITION_EFFECT_RIPPLE_FADE;
                break;
#endif
            case MMIPICVIEW_EFFECT_ORDER:
                while (MMI_TRANSITION_EFFECT_NONE == s_effect_type
                    || MMI_TRANSITION_EFFECT_FADE_IN == s_effect_type)
                {
                    s_effect_type = (s_effect_type+1)%MMI_TRANSITION_EFFECT_MAX;
                }
                break;

            default:
                SCI_PASSERT(FALSE,("MMITHEME_GetTransitionEffect:slide_effect %d is error!",slide_effect)); /*assert verified*/
                break;
            }
        }
        effect_ptr->type = s_effect_type;
        s_effect_type = (s_effect_type+1)%MMI_TRANSITION_EFFECT_MAX;
        break;

    case MMITHEME_SPECIAL_PIC_FULLSCREEN:
        while (MMI_TRANSITION_EFFECT_NONE == s_effect_type
            || MMI_TRANSITION_EFFECT_FADE_IN == s_effect_type)
        {
            s_effect_type = (s_effect_type+1)%MMI_TRANSITION_EFFECT_MAX;
        }
        effect_ptr->type = s_effect_type;
        s_effect_type = (s_effect_type+1)%MMI_TRANSITION_EFFECT_MAX;
        break;

    default:
        SCI_PASSERT(FALSE,("MMITHEME_GetTransitionEffect: type %d is error!",type));    /*assert verified*/
        break;
    }

    if(MMITHEME_IsMainScreenLandscape())
    {
        if (MMI_TRANSITION_EFFECT_BOX_ENLARGE == effect_ptr->type)
        {
            effect_ptr->param.box_param.delay_time  = 100;
            effect_ptr->param.box_param.frame_num   = 2;
            effect_ptr->param.box_param.step_x      = MMI_EFFECT_HORIZONTAL_GRID_STEP_X;
            effect_ptr->param.box_param.step_y      = MMI_EFFECT_HORIZONTAL_GRID_STEP_Y;      
  
        }
        else if (MMI_TRANSITION_EFFECT_HORIZONTAL_GRID == effect_ptr->type)
        {
            effect_ptr->param.grid_param.delay_time  = 5;
            effect_ptr->param.grid_param.frame_num   = 5;
            effect_ptr->param.grid_param.grid_width  = MMI_EFFECT_HORIZONTAL_GRID_STEP_Y;
            effect_ptr->param.grid_param.grid_height = MMI_EFFECT_HORIZONTAL_GRID_STEP_X;             
        }
        //����ˢ���Ⱥ�����, ��Ҫ������ͬ��֡����ʱ����
        else if( MMI_TRANSITION_EFFECT_RIGHTWARD_ERASE == effect_ptr->type
            || MMI_TRANSITION_EFFECT_LEFTWARD_ERASE == effect_ptr->type )
        {
            effect_ptr->param.erase_param.delay_time = 5;
            effect_ptr->param.erase_param.frame_num = MMI_EFFECT_UP_DOWNWARD_FRAME_NUM;        
        }
        else if( MMI_TRANSITION_EFFECT_UPWARD_ERASE == effect_ptr->type
            || MMI_TRANSITION_EFFECT_DOWNWARD_ERASE == effect_ptr->type )
        {
            effect_ptr->param.erase_param.delay_time = 1;
            effect_ptr->param.erase_param.frame_num = MMI_EFFECT_LEFT_RIGHTWARD_FRAME_NUM;            
        }    
    }
    else
    {
        if (MMI_TRANSITION_EFFECT_BOX_ENLARGE == effect_ptr->type)
        {
            effect_ptr->param.box_param.delay_time = 50;
            effect_ptr->param.box_param.frame_num = 5;
            effect_ptr->param.box_param.step_x = MMI_EFFECT_BOX_ENLARGE_STEP_X;
            effect_ptr->param.box_param.step_y = MMI_EFFECT_BOX_ENLARGE_STEP_Y;
        }
        else if (MMI_TRANSITION_EFFECT_HORIZONTAL_GRID == effect_ptr->type)
        {
            effect_ptr->param.grid_param.delay_time = 100;
            effect_ptr->param.grid_param.frame_num = 2;
            effect_ptr->param.grid_param.grid_width = MMI_EFFECT_HORIZONTAL_GRID_STEP_X;
            effect_ptr->param.grid_param.grid_height = MMI_EFFECT_HORIZONTAL_GRID_STEP_Y;  
        }
        //����ˢ���Ⱥ�����, ��Ҫ������ͬ��֡����ʱ����
        else if( MMI_TRANSITION_EFFECT_RIGHTWARD_ERASE == effect_ptr->type
            || MMI_TRANSITION_EFFECT_LEFTWARD_ERASE == effect_ptr->type )
        {
            effect_ptr->param.erase_param.delay_time = 1;
            effect_ptr->param.erase_param.frame_num = MMI_EFFECT_LEFT_RIGHTWARD_FRAME_NUM;
        }
        else if( MMI_TRANSITION_EFFECT_UPWARD_ERASE == effect_ptr->type
            || MMI_TRANSITION_EFFECT_DOWNWARD_ERASE == effect_ptr->type )
        {
            effect_ptr->param.erase_param.delay_time = 5;
            effect_ptr->param.erase_param.frame_num = MMI_EFFECT_UP_DOWNWARD_FRAME_NUM;        
        }    
    }
}

/*****************************************************************************/
// 	Description : �������ҷ���Ĳ���
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:�滻bin.ji���㷨, �Ż��ٶ�, ��ֹapp task����
/*****************************************************************************/
LOCAL void UpdateFullScreenByDirectionErase(
                                            MMI_TRANSITION_EFFECT_E type,
                                            MMI_TRANSITION_ERASE_PARAM_T *param_ptr
                                            )
{
    GUI_RECT_T main_rect  = MMITHEME_GetFullScreenRect();
    GUI_RECT_T rect       = main_rect;
    int16      i          = 0;
    int16      step       = 0;
    int16      frame_num  = 0;
    uint32     delay_time = 0;
    int16*     pos1_ptr    = PNULL;
    int16*     pos2_ptr    = PNULL;
    uint16     lcd_width   = 0;
    uint16     lcd_height  = 0;

    if (PNULL == param_ptr)
    {
        return;
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);      

    frame_num = param_ptr->frame_num;
    delay_time = param_ptr->delay_time;

    switch( type )
    {
    case MMI_TRANSITION_EFFECT_UPWARD_ERASE:
        step     = -lcd_height/frame_num;        
        rect.top = lcd_height;
        pos1_ptr = &rect.top;
        pos2_ptr = &rect.bottom;
        break;
    case MMI_TRANSITION_EFFECT_DOWNWARD_ERASE:
        step        = lcd_height/frame_num;        
        rect.bottom = 0;
        pos1_ptr    = &rect.bottom;
        pos2_ptr    = &rect.top;
        break;
    case MMI_TRANSITION_EFFECT_LEFTWARD_ERASE:
        step      = -lcd_width/frame_num;        
        rect.left = lcd_width;
        pos1_ptr  = &rect.left;
        pos2_ptr  = &rect.right;
        break;
    case MMI_TRANSITION_EFFECT_RIGHTWARD_ERASE:
        step      = lcd_width/frame_num;        
        rect.right = 0;
        pos1_ptr   = &rect.right;
        pos2_ptr   = &rect.left;
        break;
    default:
        SCI_ASSERT( FALSE );    /*assert verified*/
        break;
    }

    for ( i = frame_num; i > 0; i-- )
    {    
        //ÿ��ˢ��һ��rect
        (*pos1_ptr) += step;

        //ȷ����������
        GUI_IntersectRect( &rect, rect, main_rect );

        GUILCD_InvalidateRect(
            GUI_MAIN_LCD_ID, 
            rect,
            0);  

        (*pos2_ptr) += step;

        SCI_Sleep(delay_time);
    }
}

/*****************************************************************************/
// 	Description : ������Чˢ��
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_UpdateRectBySpecialEffect(
                                               GUI_RECT_T *rect_ptr
                                               )
{
    BOOLEAN result = TRUE;
    MMI_TRANSITION_EFFECT_T transition_effect;

    if (MMITHEME_SPECIAL_NONE == g_special_effect_dirty || PNULL == rect_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetTransitionEffect(&transition_effect, g_special_effect_dirty);

    switch (transition_effect.type)
    {
    case MMI_TRANSITION_EFFECT_BOX_ENLARGE:
        UpdateFullScreenByBoxEnlarge(&transition_effect.param.box_param);
        break;

    case MMI_TRANSITION_EFFECT_HORIZONTAL_GRID:
        UpdateFullScreenByHorizontalGrid(&transition_effect.param.grid_param);
        break;   

    case MMI_TRANSITION_EFFECT_UPWARD_ERASE:
    case MMI_TRANSITION_EFFECT_DOWNWARD_ERASE:
    case MMI_TRANSITION_EFFECT_LEFTWARD_ERASE:
    case MMI_TRANSITION_EFFECT_RIGHTWARD_ERASE:
        //�滻bin.ji���㷨, ��ֹapp task����
        UpdateFullScreenByDirectionErase( transition_effect.type, &transition_effect.param.erase_param );
        break;

#ifdef UI_P3D_SUPPORT
    case MMI_TRANSITION_EFFECT_FADE_IN:
        MMITHEME_InvalidateRect_FadeIn(LCD_ID_0, rect_ptr->left, rect_ptr->top,
            rect_ptr->right, rect_ptr->bottom);
        break;

#ifdef EFFECT_RIPPLE_SUPPORT
    case MMI_TRANSITION_EFFECT_RIPPLE_FADE:
        MMITHEME_PlayRippleFadeAnim(LCD_ID_0);      
        break;
#endif        
#endif
        
    default:
        result = FALSE;
        break;
    }    

    g_special_effect_dirty = MMITHEME_SPECIAL_NONE;
    return result;
}

/*****************************************************************************/
// 	Description : �����ĸ�Ӧ���õ���ˢ����Ч����ͬ��Ӧ����Ҫ����Ч��ͬ
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
void MMITHEME_SetSpecialEffectDirty(
                                    MMITHEME_SPECIAL_TYPE_E dirty_flag
                                    )
{
    g_special_effect_dirty = dirty_flag;
}

