/*****************************************************************************
** File Name:      mmiwidget_special_effect.h                                        *
** Author:                                                                   *
** Date:           06/26/2012                                                 *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe widget  special effect interface                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                  NAME             DESCRIPTION                        *
** 06/26/2012        ying.xu              Creat
******************************************************************************/
#ifndef  _MMI_WIDGET_SPECIAL_EFFECT_H 
#define  _MMI_WIDGET_SPECIAL_EFFECT_H  

//#define MMIWIDGET_SPECIAL_EFFECT
//#define MMIWIDGET_WEATHER_SE 
//#define MMIWIDGET_ACLOCK_SE 
//#define MMI_WEATHER_TEST    


#define MMIWIDGET_SPECIAL_EFFECT_FRAME_INTERVAL                 10
#define MMIWIDGET_SPECIAL_EFFECT_WAITING_TIME_BEFORE_PLAYING    1000

typedef int32 (*FUNCTION)( MMI_WIN_ID_T  win_id, void *p_widget_se);

typedef struct Bmp_obj {
    IMGREF_SIZE_T image_size;	
    uint32 * image_buffer;
}BMP_OBJ;


//widget special effect control struct
typedef struct mmiwidget_special_effect {      
     int32  current_frame   ;     //-1,0, max_frame
     int32 focus_flag      ;    // 1:focused; 0 not focused
     int32 resource_state      ;     //-1:not inited; 1 ready
     uint8  display_se_timer_id ;   //animation timer
     FUNCTION CreateResourceBuffer  ;
     FUNCTION DestroyResourceBuffer  ;
     FUNCTION Display  ;
     void*    user_data;
     //FUNCTION DisplayForTimer  ;     
}MMIWIDGET_SPECIAL_EFFECT_T;





//widget 特效的窗口处理函数
PUBLIC MMI_RESULT_E  MMIWIDGET_HandleWidgetSpecialEffectWinMsg(
    MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se,
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM            param        //IN:
);

//widget 特效显示定时器启动
PUBLIC int32 MMIWIDGET_StartWidgetSpecialEffectDisplayTimer(MMI_WIN_ID_T  win_id, uint32 time_interval,
                                                     MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se);


//widget 特效显示定时器停止
PUBLIC int32 MMIWIDGET_StopWidgetSpecialEffectDisplayTimer(MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se);


//widget reset特效的关键状态，定制定时器
PUBLIC int32 MMIWIDGET_ResetWidgetSpecialEffectState( MMI_WIN_ID_T  win_id,  MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se);


PUBLIC int32  MMIWIDGET_DisplaySEForTimer( 
                                    MMI_WIN_ID_T  win_id,  
                                    MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se
                                    );

//create picture resource
PUBLIC uint32* MMIWIDGET_CreatePictureResource( MMI_WIN_ID_T  win_id,  MMI_IMAGE_ID_T image_id);

#endif
