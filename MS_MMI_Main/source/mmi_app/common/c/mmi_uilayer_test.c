#ifdef WIN32

#include "mmi_app_common_trc.h"
/*****************************************************************************
** File Name:      mmi_uilayer_test.c                                          *
** Author:                                                                   *
** Date:           06/17/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2011      bin.ji           Creat                                  *
******************************************************************************/
#define _MMI_UILAYER_TEST_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmi_id.h"
#include "window_parse.h"
#include "mmidisplay_data.h"

LOCAL GUI_LCD_DEV_INFO s_layer_test[3] = {0};

/*****************************************************************************/
//  Discription: create layer test
//  Global resource dependence: None
//  Author: bin.ji
//  Note : 
//  Return:
/*****************************************************************************/
LOCAL void CreateLayerTest( 
                           MMI_WIN_ID_T      win_id
                           );

/*****************************************************************************/
//  Discription: draw layer test
//  
//  Global resource dependence: None
//  Author: bin.ji
//  Note : 
//  Return:
/*****************************************************************************/
LOCAL void DrawLayerTest( void );

/*****************************************************************************/
//  Description : handle test window message
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestUILayerWinMsg(
                                    MMI_WIN_ID_T      win_id, 
                                    MMI_MESSAGE_ID_E  msg_id,
                                    DPARAM            param
                                    );

//uilayerµÄ²âÊÔ´°¿Ú                                                
LOCAL WINDOW_TABLE( MMIUILAYER_TEST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleTestUILayerWinMsg),    
    WIN_ID(MMIUILAYER_TEST_WIN_ID),
    END_WIN
};

/*****************************************************************************/
//  Description : handle test window message
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestUILayerWinMsg(
                                    MMI_WIN_ID_T      win_id, 
                                    MMI_MESSAGE_ID_E  msg_id,
                                    DPARAM            param
                                    )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;    
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        CreateLayerTest(win_id);
        break;

    case MSG_FULL_PAINT:
        DrawLayerTest();
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Discription: create layer test
//  
//  Global resource dependence: None
//  Author: bin.ji
//  Note : 
//  Return:
/*****************************************************************************/
LOCAL void CreateLayerTest( 
                           MMI_WIN_ID_T      win_id
                           )
{
    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T    create_info = {0};        
        
        //s_layer_test[0]
        create_info.lcd_id = MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 50;
        create_info.offset_y = 50;
        create_info.width = 200;
        create_info.height = 100;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;   
        UILAYER_CreateLayer(&create_info, &s_layer_test[0]);           
        
        //s_layer_test[1]
        create_info.lcd_id = MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 50+20;
        create_info.offset_y = 50+20;
        create_info.width = 200;
        create_info.height = 100;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;   
        UILAYER_CreateLayer(&create_info, &s_layer_test[1]);  

        //s_layer_test[2]
        create_info.lcd_id = MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 50+40;
        create_info.offset_y = 50+40;
        create_info.width = 200;
        create_info.height = 100;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;   
        UILAYER_CreateLayer(&create_info, &s_layer_test[2]);  
        UILAYER_SetLayerAlpha(&s_layer_test[2], 100);
//        UILAYER_SetLayerColorKey(&s_layer_test[2], TRUE, UILAYER_TRANSPARENT_COLOR);
    }
}

/*****************************************************************************/
//  Discription: draw layer test
//  
//  Global resource dependence: None
//  Author: bin.ji
//  Note : 
//  Return:
/*****************************************************************************/
LOCAL void DrawLayerTest( void )
{
    GUI_LCD_DEV_INFO main_dev_info = {0, 0};
    GUI_RECT_T rect0 = {0};
    UILAYER_APPEND_BLT_T append_layer = {0};
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;

    append_layer.lcd_dev_info = s_layer_test[0];
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);
    append_layer.lcd_dev_info = s_layer_test[1];
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);
    append_layer.lcd_dev_info = s_layer_test[2];
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    rect0.left = 0;
    rect0.top = 0;
    rect0.right = (lcd_width-1);
    rect0.bottom = (lcd_height-1);
    LCD_FillRect(&main_dev_info, rect0, MMI_YELLOW_COLOR);
    LCD_FillRect(&s_layer_test[0], UILAYER_GetLayerRect(&s_layer_test[0]), MMI_RED_COLOR);
    LCD_FillRect(&s_layer_test[1], UILAYER_GetLayerRect(&s_layer_test[1]), MMI_BLUE_COLOR);
    LCD_FillRect(&s_layer_test[2], UILAYER_GetLayerRect(&s_layer_test[2]), MMI_GREEN_COLOR);
//    LCD_FillRect(&s_layer_test[2], UILAYER_GetLayerRect(&s_layer_test[2]), UILAYER_TRANSPARENT_COLOR);
}

/*****************************************************************************/
//  Discription: test uilayer
//  Global resource dependence: None
//  Author: bin.ji
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIUILAYER_Test( void )
{
    MMK_CreateWin((uint32*)MMIUILAYER_TEST_WIN_TAB, PNULL);  
}

#endif