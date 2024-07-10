/*****************************************************************************
** File Name:      mmiwidget_dcd.c                                        *
** Author:                                                                   *
** Date:           10/23/2009                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**  Creat
******************************************************************************/

#define _MMIWIDGET_DCD_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"

#if ((defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)) && defined(MMI_ENABLE_DCD)
#include "mmi_app_widget_trc.h"
#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "guires.h"
#include "guitext.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiwidget_position.h"
#include "mmipub.h"
#include "mmidcd_export.h"
#include "mmidcd_main.h" 
#include "mmidcd_text.h"
#include "mmisms_app.h"
#include "mmitheme_text.h" 

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define MMIWIDGET_DCD_INTERVAL		10

#define MMIWIDGET_DCD_FONT_SIZE		SONG_FONT_12

#define MMIWIDGET_DCD_TEXT_LINE_SPACE        2			

#define  MMIWIDGET_SECOND_PER_MICROSECOND_POWER              1000

#define MMIWIDGET_DCD_TAB_ANIM_TOTAL		4


typedef struct widget_dcd_gui_struct_tag
{
    uint16 total_page_num;

    uint16 current_page_index;          /*dcd page num 绝对量*/
    uint16 current_index_in_page;       /*dcd main win 相对值 */
    uint16 current_item_num;            /*dcd main win 相对值 */
    uint16 current_real_index_in_page;	/*dcd item index in 1 page ,绝对值*/
	
    uint16 cur_idle_page_index;         
    uint16 cur_idle_realitem_index;

} WIDGET_DCD_GUI_STRUCT_T;

//点中区域的标识
typedef enum
{
	MMIWIDGET_DCD_HIT_NONE,
	MMIWIDGET_DCD_HIT_RIGHT_IDC,		// right indicator 
	MMIWIDGET_DCD_HIT_LEFT_IDC,		// left indicator 
	MMIWIDGET_DCD_HIT_MAX
}MMIWIDGET_DCD_HIT_TYPE_E;

// switch type
typedef enum
{
    MMIWIDGET_DCD_SWITCH_NONE,
    MMIWIDGET_DCD_SWITCH_PRE,
    MMIWIDGET_DCD_SWITCH_NEXT,
    MMIWIDGET_DCD_SWITCH_MAX
}MMIWIDGET_DCD_SWITCH_TYPE_E;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : display DCD Panel
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayDcdPanel( MMI_WIN_ID_T     win_id);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void  MMIWIDGETDCD_UpdateItem(MMI_WIN_ID_T   win_id,
                               uint16         item_index,
                               BOOLEAN        is_swtich,
                               BOOLEAN        is_need_update
                               );

/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIWIDGETDCD_SetAnimParam(
                               MMI_WIN_ID_T   win_id,
                               uint16           index,
                               BOOLEAN          is_update  //是否立即刷新             
                               );

/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETDCD_SetTextParam(     
				MMI_WIN_ID_T   win_id,
                            uint16           index,
                            BOOLEAN          is_update  //是否立即刷新             
                            );

/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETDCD_SetTabAnimParam(MMI_WIN_ID_T   win_id );

/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETDCD_AppendOneTabAnim(
                                       MMI_CTRL_ID_T    ctrl_id,         //in:control id
                                       uint16           item_index     //in:item索引
                                       );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreatText(
                             MMI_WIN_ID_T             win_id,  //win id
                             MMI_CTRL_ID_T                  ctrl_id    //control id
                             );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreatAnim(
                             MMI_WIN_ID_T             win_id,    //win id
                             MMI_CTRL_ID_T                  ctrl_id    //control id
                             );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreatTabAnim(
                            MMI_WIN_ID_T             win_id    //win id
                             );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWIDGETDCD_HandleTpPressDown(
				MMI_WIN_ID_T      win_id,
				DPARAM            param  
				);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWIDGETDCD_HandleTpPressUp(
				MMI_WIN_ID_T      win_id,
				DPARAM            param  
				);


/*****************************************************************************/
//  Description : get auto switch
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  DisplayDcdValid(      );


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get  DCD text rect
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIWIDGETDCD_GetDcdTextRect();
/*****************************************************************************/
//  Description : get  DCD anim rect
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIWIDGETDCD_GetAnimRect();

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:violent.wei
//  Note: jun.hu 09/8/27 把它从mmidcd_main.c中移动到这里
/*****************************************************************************/
PUBLIC void MMIWIDGETDCD_SetData(void);

/*****************************************************************************/
//  Description : get auto switch
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGETDCD_SwitchToNextItem(
                                       MMIWIDGET_DCD_SWITCH_TYPE_E type
                                       );


/*****************************************************************************/
//     Description : Handle Widget DCD Win Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 

PUBLIC MMI_RESULT_E HandleWidgetDcdWinMsg(
                                      MMI_WIN_ID_T        win_id,        	//IN:
                                      MMI_MESSAGE_ID_E    msg_id,      //IN:
                                      DPARAM            param        		//IN:
                                      );

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_dcd_widget =
{
    MMIWIDGET_DCD_ID, 
    HandleWidgetDcdWinMsg, 
    DisplayDcdValid, 
    PNULL,
    PNULL,
    WIDGET_DCD_WIN_ID, 
    IMAGE_WIDGET_DCD_SHORTCUT, 
    IMAGE_WIDGET_DCD_BG,
    TXT_WIDGET_DCD,
    0,//    75, 
    258,//    75
    TRUE
};

LOCAL WIDGET_DCD_GUI_STRUCT_T s_widget_dcd_gui_struct = {0};

//点中区域的标识
LOCAL uint32 s_widget_dcd_hit_type = MMIWIDGET_DCD_HIT_NONE;


//tab 图片显示区域

LOCAL const GUI_RECT_T s_dcd_widget_arr[] = 
{
    MMIWIDGET_DCD_TAB_ANIM_0_RECT,
    MMIWIDGET_DCD_TAB_ANIM_1_RECT,
    MMIWIDGET_DCD_TAB_ANIM_2_RECT,
    MMIWIDGET_DCD_TAB_ANIM_3_RECT
};

//当前高亮的title
LOCAL uint32 s_widget_dcd_focus_index = 0;

/*****************************************************************************/
//  Description : get auto switch
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  DisplayDcdValid( )
{
	if(MMIAPIDCD_DcdIsOpen())
	{
		return MMI_RESULT_TRUE;
	}
	else
	{
		return MMI_RESULT_FALSE;
	}

}
/*****************************************************************************/
//     Description : Handle Widget DCD Win Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
PUBLIC MMI_RESULT_E  HandleWidgetDcdWinMsg(
                                          MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM       param
                                          )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;    
    static BOOLEAN          s_is_need_update = FALSE;

    switch (msg_id)
    {

	case MSG_OPEN_WINDOW:
		CreatText(win_id,MMIWIDGET_DCD_TEXT_CTRL_ID);
		CreatAnim(win_id,MMIWIDGET_DCD_ANIM_CTRL_ID);
		CreatTabAnim(win_id);
		MMIWIDGETDCD_SetData(); 
      		s_is_need_update = TRUE;		
       	break;
        
    	case MSG_FULL_PAINT:
		//open win 时刷新显示
		
	


		if(s_is_need_update)
		{
        		MMIWIDGETDCD_UpdateItem(win_id, s_widget_dcd_gui_struct.current_real_index_in_page,FALSE,FALSE);
			s_is_need_update = FALSE;
		}	
		DisplayDcdPanel(win_id);
		//s_is_update_screen = FALSE;
			
        	break;
       case MSG_WIDGET_TP_PRESS_DOWN:
		recode = MMIWIDGETDCD_HandleTpPressDown(win_id,param);
		break;

       case MSG_WIDGET_TP_PRESS_UP:
		recode = MMIWIDGETDCD_HandleTpPressUp(win_id,param);

		break;
		
    	case MSG_NOTIFY_TEXT_END_LINE:
        	{               
            		s_widget_dcd_gui_struct.cur_idle_realitem_index++;
            		if(MMIDCD_MAX_ITEM_NUM_PER_PAGE < s_widget_dcd_gui_struct.cur_idle_realitem_index)
            		{
                		s_widget_dcd_gui_struct.cur_idle_realitem_index = 0;
            		}
            		MMIWIDGETDCD_UpdateItem(win_id, s_widget_dcd_gui_struct.cur_idle_realitem_index, TRUE, TRUE);
        	}
        	break;
        
    	case MSG_DCD_LIST_UPDATE:
        	{
            		MMIWIDGETDCD_UpdateItem(win_id, s_widget_dcd_gui_struct.cur_idle_realitem_index, FALSE, TRUE);  
        	}
        	break;

    	case MSG_DCD_SYNCHRONIZE_DISPLAY:    
        	{	
			s_widget_dcd_gui_struct.cur_idle_page_index = MMIAPIDCD_GetDcdCurrentPageIndex();	
            		MMIWIDGETDCD_UpdateItem(win_id, s_widget_dcd_gui_struct.cur_idle_realitem_index, FALSE, FALSE);       
        	}
        	break;   

	case MSG_NOTIFY_TEXT_SCROLL:
				 MMK_UpdateScreen();	
		break;

			
    	case MSG_APP_CANCEL:
        	MMK_CloseWin(win_id);
        	break;
        
    	default:
        	recode = MMI_RESULT_FALSE;
        	break;
    }
    
    return recode;
    
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:violent.wei
//  Note: jun.hu 09/8/27 把它从mmidcd_main.c中移动到这里
/*****************************************************************************/
PUBLIC void MMIWIDGETDCD_SetData(void)
{
    //SCI_TRACE_LOW:"[MMIDCD]:MMIWIDGETDCD_SetData() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_DCD_389_112_2_18_3_8_4_191,(uint8*)"");
    s_widget_dcd_gui_struct.current_page_index = 0 ;
    s_widget_dcd_gui_struct.current_index_in_page = 0;
    s_widget_dcd_gui_struct.current_real_index_in_page =0; 

    s_widget_dcd_gui_struct.cur_idle_page_index = MMIAPIDCD_GetDcdCurrentPageIndex();	
     // dcd idle窗口用到的数据
    s_widget_dcd_gui_struct.cur_idle_realitem_index = 0;   // dcd idle窗口用到的数据
    
    if(PNULL != MMIAPIDCD_GetXmlDoc_Ptr())
    {
        s_widget_dcd_gui_struct.total_page_num   = MMIAPIDCD_GetFormalFeedNum();
        s_widget_dcd_gui_struct.current_item_num = MMIAPIDCDFILE_GetFeedUseableEntryCount(0, TRUE);
    }
    else
    {
        s_widget_dcd_gui_struct.total_page_num   = 0;
        s_widget_dcd_gui_struct.current_item_num = 0;
    }

    //init focus index
    s_widget_dcd_focus_index = 0;
	
    //SCI_TRACE_LOW:"[MMIDCD]:MMIWIDGETDCD_SetData() exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_DCD_412_112_2_18_3_8_4_192,(uint8*)"");
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreatText(
                             MMI_WIN_ID_T             win_id,   //win id
                             MMI_CTRL_ID_T                  ctrl_id    //control id
                             )
{
    MMI_CONTROL_CREATE_T create = {0};
    GUITEXT_INIT_DATA_T  init_data = {0};
    GUI_RECT_T		text_rect = {0};

    text_rect = MMIWIDGETDCD_GetDcdTextRect();
     init_data.both_rect.v_rect = init_data.both_rect.h_rect = text_rect;    
    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_TEXTBOX_ID;
    create.parent_win_handle = win_id; 
    create.parent_ctrl_handle = 0;
    create.init_data_ptr = &init_data;
    MMK_CreateControl( &create );    
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreatAnim(
                            MMI_WIN_ID_T             win_id,     //win id
                             MMI_CTRL_ID_T                  ctrl_id    //control id
                             )
{
	
    GUIANIM_INIT_DATA_T init_data = {0};
    MMI_CONTROL_CREATE_T create   = {0};           
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_FILE_INFO_T file_info  = {0};
    GUIANIM_RESULT_E retcode = 0;
    GUIANIM_DISPLAY_INFO_T display_info = {0};
    GUI_RECT_T disp_rect = {0};
    GUIANIM_DATA_INFO_T             data_info           = {0};

    disp_rect  = MMIWIDGETDCD_GetAnimRect();

    init_data.both_rect.h_rect = init_data.both_rect.v_rect = disp_rect;//full_disp_rect;
    
    create.ctrl_id = ctrl_id;//0;
    create.guid = SPRD_GUI_ANIM_ID;
    create.parent_win_handle = win_id;
    create.parent_ctrl_handle = 0;//ctrl_id;
    create.init_data_ptr = &init_data;
    
    MMK_CreateControl(&create);    
        
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreatTabAnim(
                            MMI_WIN_ID_T             win_id    //win id
                             )
{
    uint32                  i = 0;
    GUIANIM_INIT_DATA_T init_data = {0};
    MMI_CONTROL_CREATE_T create   = {0};           
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_FILE_INFO_T file_info  = {0};
    GUIANIM_RESULT_E retcode = 0;
    GUIANIM_DISPLAY_INFO_T display_info = {0};
    GUI_RECT_T disp_rect = {0};
    GUIANIM_DATA_INFO_T             data_info           = {0};

    for(i= 0;i<MMIWIDGET_DCD_TAB_ANIM_TOTAL;i++)
    {

		disp_rect  = s_dcd_widget_arr[i];
		disp_rect  = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, disp_rect);		

    		init_data.both_rect.h_rect = init_data.both_rect.v_rect = disp_rect;//full_disp_rect;
    
   	 	create.ctrl_id = (MMIWIDGET_DCD_TAB_ANIM_CTRL_ID+i);
    		create.guid = SPRD_GUI_ANIM_ID;
    		create.parent_win_handle = win_id;
    		create.parent_ctrl_handle = 0;
    		create.init_data_ptr = &init_data;
    
        MMK_CreateControl(&create);
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWIDGETDCD_HandleTpPressDown(
				MMI_WIN_ID_T      win_id,
				DPARAM            param  
				)
{
	MMI_RESULT_E  recode 		= MMI_RESULT_TRUE;
	GUI_RECT_T	   right_idc_t  	= MMIWIDGET_DCD_WIN_RIGHT_IDC_RECT;
	GUI_RECT_T	   left_idc_t 	 	= MMIWIDGET_DCD_WIN_LEFT_IDC_RECT;
    	GUI_POINT_T 	   point 			= {0};

	point.x = MMK_GET_TP_X(param);
	point.y = MMK_GET_TP_Y(param);
    	point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);

	if (GUI_PointIsInRect(point, right_idc_t) && (s_widget_dcd_gui_struct.cur_idle_page_index<s_widget_dcd_gui_struct.total_page_num-1))
	{
	        s_widget_dcd_hit_type = MMIWIDGET_DCD_HIT_RIGHT_IDC;
	}
	else if(GUI_PointIsInRect(point, left_idc_t) && (0 < s_widget_dcd_gui_struct.cur_idle_page_index))
	{
	        s_widget_dcd_hit_type = MMIWIDGET_DCD_HIT_LEFT_IDC;
	}
	else
	{
	        s_widget_dcd_hit_type = MMIWIDGET_DCD_HIT_NONE;
	        recode = MMI_RESULT_FALSE;
	}

	return recode;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWIDGETDCD_HandleTpPressUp(
				MMI_WIN_ID_T      win_id,
				DPARAM            param  
				)
{
	MMI_RESULT_E  recode 		= MMI_RESULT_TRUE;
	GUI_RECT_T	   right_idc_t  	= MMIWIDGET_DCD_WIN_RIGHT_IDC_RECT;
	GUI_RECT_T	   left_idc_t 	 	= MMIWIDGET_DCD_WIN_LEFT_IDC_RECT;
	GUI_RECT_T	   open_dcd_main_win_t =MMIWIDGET_DCD_OPENMAINWIN_RECT;
    	GUI_POINT_T 	   point 			= {0};

	s_widget_dcd_hit_type = MMIWIDGET_DCD_HIT_NONE;

	point.x = MMK_GET_TP_X(param);
	point.y = MMK_GET_TP_Y(param);
    	point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);

	if (GUI_PointIsInRect(point, right_idc_t)&& (s_widget_dcd_gui_struct.cur_idle_page_index<s_widget_dcd_gui_struct.total_page_num))
	{
		s_widget_dcd_gui_struct.cur_idle_page_index++;
		if(s_widget_dcd_focus_index < 3)
		{
			s_widget_dcd_focus_index++;
		}
	}
	else if(GUI_PointIsInRect(point, left_idc_t)&& (0 < s_widget_dcd_gui_struct.cur_idle_page_index))
	{
		s_widget_dcd_gui_struct.cur_idle_page_index--;
		if(s_widget_dcd_focus_index > 0)
		{
			s_widget_dcd_focus_index--;
		}
		
	}
	else if(GUI_PointIsInRect(point, open_dcd_main_win_t))
	{
		MMIAPIDCD_SetDcdCurrentPageIndex(s_widget_dcd_gui_struct.cur_idle_page_index,s_widget_dcd_gui_struct.cur_idle_realitem_index);	
		MMIAPIDCD_ExternalOpenWin(FALSE);
		return recode;
	}
	MMIAPIDCD_SetDcdCurrentPageIndex(s_widget_dcd_gui_struct.cur_idle_page_index,s_widget_dcd_gui_struct.cur_idle_realitem_index);	
	MMIWIDGETDCD_UpdateItem(win_id, 0, FALSE, FALSE);	
	return recode;

}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void  MMIWIDGETDCD_UpdateItem(MMI_WIN_ID_T   win_id,
                               uint16         item_index,
                               BOOLEAN        is_swtich,
                               BOOLEAN        is_need_update
                               )
{      
    BOOLEAN is_update = FALSE;
    uint16  pageindex = s_widget_dcd_gui_struct.cur_idle_page_index;
    uint16  show_item_index = 0;
    BOOLEAN show_result = FALSE;
    
    if(is_need_update)
    {
        is_update = MMK_IsFocusWin(win_id);
    }   
    
    if(MMIAPIDCD_GetUseableItemFromIndex(pageindex, item_index, &show_item_index))
    {
        s_widget_dcd_gui_struct.cur_idle_realitem_index = show_item_index;
	 MMIWIDGETDCD_SetTabAnimParam(win_id);
        MMIWIDGETDCD_SetAnimParam( win_id,show_item_index, is_update);
        MMIWIDGETDCD_SetTextParam(win_id, show_item_index, is_update);
        show_result = TRUE;
    }
    
    if(!show_result)
    {    
        if(is_swtich)                   
        {
           MMIWIDGETDCD_SwitchToNextItem( MMIWIDGET_DCD_SWITCH_NEXT);          
        }
        else
        {
            if(MMIAPIDCD_GetUseableItemFromIndex(pageindex, 0, &show_item_index))
            {
                s_widget_dcd_gui_struct.cur_idle_realitem_index = show_item_index;
            }
            
            // 不管有没有找到可用的item ,都要调用下面的这个函数来显示，如果没找到，则显示内容为空....
	     MMIWIDGETDCD_SetTabAnimParam(win_id);
	     MMIWIDGETDCD_SetAnimParam( win_id,show_item_index, is_update);
            MMIWIDGETDCD_SetTextParam( win_id,show_item_index, is_update);
        }
    }

}



/*****************************************************************************/
//  Description : get auto switch
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGETDCD_SwitchToNextItem(
                                       MMIWIDGET_DCD_SWITCH_TYPE_E type
                                       )
{
    uint32          i           = 0;
    uint32          move_num    = 0;
    BOOLEAN	        result      = TRUE;
    uint32          cur_item_index = 0;

    if (s_widget_dcd_gui_struct.total_page_num == 0)
    {
        //SCI_TRACE_LOW:"GUITAB_SwitchToNextItem FALSE!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_DCD_657_112_2_18_3_8_4_193,(uint8*)"");
        return FALSE;
    }
    
    switch(type) 
    {
    case MMIWIDGET_DCD_SWITCH_NEXT:
	if ((s_widget_dcd_gui_struct.cur_idle_page_index<s_widget_dcd_gui_struct.total_page_num))
	{
		s_widget_dcd_gui_struct.cur_idle_page_index++;
		if(s_widget_dcd_focus_index < 3)
		{
			s_widget_dcd_focus_index++;
		}
	}
	else
	{
		s_widget_dcd_gui_struct.cur_idle_page_index=0;
		s_widget_dcd_focus_index = 0;
	}
	MMIWIDGETDCD_UpdateItem(WIDGET_DCD_WIN_ID,0,FALSE,TRUE);	
    	break;

    case MMIWIDGET_DCD_SWITCH_PRE:
        break;
        
    default:
        //SCI_TRACE_LOW:"MMIWIDGETDCD_SwitchToNextItem type error! %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_DCD_684_112_2_18_3_8_4_194,(uint8*)"d", type);
        result = FALSE;
        break;
    }

    return(result);
}

/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETDCD_SetTabAnimParam(MMI_WIN_ID_T   win_id )
{
    MMI_CTRL_ID_T       ctrl_id = MMIWIDGET_DCD_TAB_ANIM_CTRL_ID;
    uint32 i =0;   
    uint16 			    current_page_index = s_widget_dcd_gui_struct.cur_idle_page_index;

    //用于确认当前显示的四个项目的index  
    current_page_index = current_page_index-s_widget_dcd_focus_index; 
		
    for(i=0;i<MMIWIDGET_DCD_TAB_ANIM_TOTAL;i++)
    {
		ctrl_id =( MMIWIDGET_DCD_TAB_ANIM_CTRL_ID+i);	
		MMIWIDGETDCD_AppendOneTabAnim(ctrl_id,current_page_index);
		current_page_index++;

  	}
}

/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETDCD_AppendOneTabAnim(
                                       MMI_CTRL_ID_T    ctrl_id,         //in:control id
                                       uint16           item_index     //in:item索引
                                       )
{
    	wchar  picture_full_path[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0}; 
    	uint16 page_index=item_index;
    	uint16  path_len = 0;
    	BOOLEAN  show_file = FALSE;
    	GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
    	GUIANIM_CTRL_INFO_T     control_info = {0};
    	GUIANIM_DATA_INFO_T     data_info = {0};
    	GUIANIM_FILE_INFO_T     anim_info = {0};
    	GUIANIM_DISPLAY_INFO_T  display_info = {0};
		
    MMIDCDFILE_GetOnePictureFullPath(picture_full_path, 
                                    MMIAPIDCD_GetFeedContentInfo(page_index,  0, (char*)"ctxt-id"),
                                    MMIAPIDCD_GetFeedContentInfo(page_index,  0, (char*)"type"),
                                    MMIDCD_PICTURE_DIR_PATH
                                    );
    path_len = MMIAPICOM_Wstrlen(picture_full_path);

    //set anim param
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = ctrl_id;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_syn_decode = TRUE;

	
    if(MMIAPIFMM_IsFileExist(picture_full_path, path_len))
    {
            anim_info.full_path_wstr_ptr = picture_full_path;
            anim_info.full_path_wstr_len = path_len;
        
            anim_result = GUIANIM_SetParam(&control_info,PNULL,&anim_info,&display_info);
            show_file = TRUE;
            //SCI_TRACE_LOW:"MMICD will show file"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_DCD_754_112_2_18_3_8_4_195,(uint8*)"");
    }

    if(!show_file)
    {
        data_info.img_id = IMAGE_WIDGET_DCD_TITLE_DEFAULT;         
        anim_result = GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
	
    }
    
}
/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIWIDGETDCD_SetAnimParam(
                               MMI_WIN_ID_T   win_id,
                               uint16           index,
                               BOOLEAN          is_update  //是否立即刷新             
                               )
{
    BOOLEAN                 result = FALSE;
    GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_FILE_INFO_T     anim_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    wchar  picturn_full_path[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0}; 
    
    uint16  page_index    = s_widget_dcd_gui_struct.cur_idle_page_index;  
    uint16  full_path_len = 0;
    uint16  useable_item_num = 0;
    
    MMIDCDFILE_GetOnePictureFullPath(picturn_full_path,
                                    MMIAPIDCD_GetEntryContentInfo(page_index, index, 0, (char*)"ctxt-id"),
                                    MMIAPIDCD_GetEntryContentInfo(page_index, index, 0, (char*)"type"),
                                    MMIDCD_PICTURE_DIR_PATH
                                    );
    full_path_len = MMIAPICOM_Wstrlen(picturn_full_path);
    
    GUIANIM_SetDefaultIcon(MMIWIDGET_DCD_ANIM_CTRL_ID,IMAGE_WIDGET_DCD_DEFAULT,IMAGE_WIDGET_DCD_DEFAULT);
    
    //set anim param
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMIWIDGET_DCD_ANIM_CTRL_ID;
    display_info.align_style = GUIANIM_ALIGN_HMIDDLE_TOP;
    display_info.is_syn_decode = TRUE;
    display_info.is_update = is_update;



    useable_item_num = MMIAPIDCDFILE_GetFeedUseableEntryCount(page_index, MMIAPIDCD_GeSettingInfo()->is_show_readed);      
    if((0 < useable_item_num ) && MMIAPIFMM_IsFileExist(picturn_full_path,full_path_len))
    {
        anim_info.full_path_wstr_ptr = picturn_full_path;
        anim_info.full_path_wstr_len = full_path_len;
        
        anim_result = GUIANIM_SetParam(&control_info,PNULL,&anim_info,&display_info);
    }
    else
    {
        data_info.img_id = IMAGE_WIDGET_DCD_DEFAULT;         
        
        anim_result = GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    }
    
    if ((GUIANIM_RESULT_SUCC == anim_result) ||
        (GUIANIM_RESULT_WAIT == anim_result))
    {
        result = TRUE;
    }
    
    return (result);
    
    
}

/*****************************************************************************/
//  Description : set text parameter
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETDCD_SetTextParam(     
				MMI_WIN_ID_T   win_id,
                            uint16           index,
                            BOOLEAN          is_update  //是否立即刷新             
                            )
{
    MMI_STRING_T    content_str = {0};
    const wchar     test_str[] = {L"\n"};
    uint16          test_len=0;
    uint16          title_len=0;
    uint16          wlen=0;
    uint16          page_index = s_widget_dcd_gui_struct.cur_idle_page_index;  
    uint32          timer = MMIAPIDCD_GeSettingInfo()->scroll_time * MMIWIDGET_SECOND_PER_MICROSECOND_POWER;
    GUI_COLOR_T     font_color = 0;
    GUI_FONT_T      font=MMIWIDGET_DCD_FONT_SIZE;
    GUI_BG_T        dcd_bg = {0};
    wchar           *full_content_ptr = PNULL;

    full_content_ptr = SCI_ALLOCAZ(MMISMS_MAX_MESSAGE_LEN * sizeof(wchar));

    if (PNULL == full_content_ptr)
    {
        return;
    }

    //set text to content_str
    if (0 == MMIAPIDCDFILE_GetFeedUseableEntryCount(page_index, TRUE))   
    {
        //快讯内容为空
        MMI_GetLabelTextByLang(TXT_DCD_CONTENT_IS_EMPTY, &content_str);
        font_color = MMITHEME_GetDefaultWidgetThemeColor();
    }
    else if(0 == MMIAPIDCDFILE_GetFeedUseableEntryCount(page_index, MMIAPIDCD_GeSettingInfo()->is_show_readed))   
    {            
        //没有未读条目
        MMI_GetLabelTextByLang(TXT_DCD_BLANK, &content_str);
        font_color = MMITHEME_GetDefaultWidgetThemeColor();
    }
    else    
    {
        title_len =MMIAPIDCD_GetEntryTitle(full_content_ptr, MMISMS_MAX_MESSAGE_LEN, page_index, index);
        
        //SCI_ASSERT(MMISMS_MAX_MESSAGE_LEN >= title_len);

        if (title_len > MMISMS_MAX_MESSAGE_LEN)
        {
            title_len = MMISMS_MAX_MESSAGE_LEN - 1;
        }
        
        test_len= MMIAPICOM_Wstrlen(test_str);
        MMI_WSTRNCPY(full_content_ptr+title_len, MMISMS_MAX_MESSAGE_LEN,test_str,test_len,test_len);
        
        if((title_len+test_len) < MMISMS_MAX_MESSAGE_LEN )
        {
            wlen = (uint16)(MMISMS_MAX_MESSAGE_LEN - title_len-test_len);   
            MMIAPIDCD_GetEntrySummary(full_content_ptr + title_len+test_len, wlen, page_index, index);
        }
        if(!MMIAPIDCD_GetItemAlreadyRead(page_index, index))
        {   
            font_color = MMIDCD_NO_READ_COLOR;
        }       
        else
        {
            font_color = MMITHEME_GetDefaultWidgetThemeColor();
        }
        
        //set text to content_str
        content_str.wstr_ptr = full_content_ptr;
        content_str.wstr_len= (uint16)MMIAPICOM_Wstrlen(full_content_ptr);
    }

	
    //set text color and font
    GUITEXT_SetFont(MMIWIDGET_DCD_TEXT_CTRL_ID,&font,&font_color);
    
    //set bg
    dcd_bg.bg_type = GUI_BG_NONE;
    //dcd_bg.img_id  = IMAGE_WIDGET_DCD_BG;//MMI_BLUE_COLOR;
    GUITEXT_SetBg(MMIWIDGET_DCD_TEXT_CTRL_ID,&dcd_bg);
    
    //text not handle tp msg
    GUITEXT_SetHandleTpMsg(FALSE,MMIWIDGET_DCD_TEXT_CTRL_ID);
    //set text no progress
    GUITEXT_IsDisplayPrg(FALSE,MMIWIDGET_DCD_TEXT_CTRL_ID);
    
    //set text auto scroll 
    GUITEXT_SetAutoScroll(TRUE,&timer,MMIWIDGET_DCD_TEXT_CTRL_ID);
    GUITEXT_SetCircularHandle(FALSE,MMIWIDGET_DCD_TEXT_CTRL_ID);
    
    //set text
    GUITEXT_SetString(MMIWIDGET_DCD_TEXT_CTRL_ID,content_str.wstr_ptr,content_str.wstr_len,FALSE);     

    if(is_update)
    {
    	MMK_UpdateScreen();
    }

    SCI_FREE(full_content_ptr);
}





/*****************************************************************************/
//  Description : get  DCD tab rect
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIWIDGETDCD_GetDcdTextRect()
{
	GUI_RECT_T text_rect = {MMIWIDGET_DCD_WIN_TEXT_LEFT, MMIWIDGET_DCD_WIN_TEXT_TOP, MMIWIDGET_DCD_WIN_TEXT_RIGHT, MMIWIDGET_DCD_WIN_TEXT_BOTTOM};
	text_rect  = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, WIDGET_DCD_WIN_ID, text_rect);
	return  text_rect;
}
/*****************************************************************************/
//  Description : get  DCD tab rect
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIWIDGETDCD_GetAnimRect()
{
	GUI_RECT_T anim_rect = {MMIWIDGET_DCD_WIN_ANIM_LEFT,MMIWIDGET_DCD_WIN_ANIM_TOP,MMIWIDGET_DCD_WIN_ANIM_RIGHT,MMIWIDGET_DCD_WIN_ANIM_BOTTOM};
	anim_rect  = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, WIDGET_DCD_WIN_ID, anim_rect);
	return anim_rect;
}
/*****************************************************************************/
//  Description : get  DCD tab focus rect
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIWIDGETDCD_GetFocusRect(uint32 focus_index)
{
	GUI_RECT_T focus_rect = s_dcd_widget_arr[focus_index];

	focus_rect.top =  s_dcd_widget_arr[focus_index].top -2;  
	focus_rect.left =    s_dcd_widget_arr[focus_index].left -2;
	focus_rect.right = s_dcd_widget_arr[focus_index].right +2;
	focus_rect.bottom = s_dcd_widget_arr[focus_index].bottom +2;
	focus_rect =  MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, WIDGET_DCD_WIN_ID, focus_rect);  

	return focus_rect;
}
/*****************************************************************************/
//  Description : display DCD Panel
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayDcdPanel( MMI_WIN_ID_T win_id)
{
    	GUI_RECT_T win_rect = {0};
    	GUI_LCD_DEV_INFO lcd_dev_info = {0};
    	uint32 i = 0;
    	GUI_RECT_T img_rect = {0};
	GUI_RECT_T	   left_idc_t 	 	= MMIWIDGET_DCD_WIN_LEFT_IDC_RECT;
	GUI_RECT_T	   right_idc_t  	= MMIWIDGET_DCD_WIN_RIGHT_IDC_RECT;
	GUI_RECT_T	   focus_image_t  	= {0};

	MMI_IMAGE_ID_T   left_image_id = IMAGE_WIDGET_DCD_LEFT;
	MMI_IMAGE_ID_T   right_image_id = IMAGE_WIDGET_DCD_RIGHT;
	MMI_IMAGE_ID_T   focus_image_id = IMAGE_WIDGET_DCD_TITLE_FOCUS;


	
    	MMK_GetWinRect(win_id, &win_rect);
    	MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
	    //set text theme lcd dev
	    //MMK_SetCtrlLcdDevInfo(MMIWIDGET_DCD_TEXT_CTRL_ID,&lcd_dev_info);


    	if (UILAYER_IsLayerActive(&lcd_dev_info))
    	{

       	GUIAPICTRL_SetLcdDevInfo(MMIWIDGET_DCD_TEXT_CTRL_ID,&lcd_dev_info);
		//背景图
		GUIRES_DisplayImg(
            			PNULL,
            			&win_rect,
            			PNULL,
            			win_id,
            			IMAGE_WIDGET_DCD_BG,
            			&lcd_dev_info
            			);

	       left_idc_t 	= MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, left_idc_t);
	       right_idc_t 	= MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, right_idc_t);
		focus_image_t= MMIWIDGETDCD_GetFocusRect(s_widget_dcd_focus_index);
			
		switch(s_widget_dcd_hit_type)
		{
			case MMIWIDGET_DCD_HIT_RIGHT_IDC:

				left_image_id   = IMAGE_WIDGET_DCD_LEFT;
				right_image_id = IMAGE_WIDGET_DCD_RIGHT_SELECT;
				
				break;

			case MMIWIDGET_DCD_HIT_LEFT_IDC:

				left_image_id   = IMAGE_WIDGET_DCD_LEFT_SELECT;
				right_image_id = IMAGE_WIDGET_DCD_RIGHT;
				break;			
			
			default:
				if(0 == s_widget_dcd_gui_struct.cur_idle_page_index)
				{
					left_image_id = IMAGE_WIDGET_DCD_LEFT_GRAY;
				}
				else
				{
					left_image_id = IMAGE_WIDGET_DCD_LEFT;
				}

				if(s_widget_dcd_gui_struct.cur_idle_page_index == s_widget_dcd_gui_struct.total_page_num)
				{
	 				right_image_id = IMAGE_WIDGET_DCD_RIGHT_GRAY;
				}
				else
				{
	 				right_image_id = IMAGE_WIDGET_DCD_RIGHT;
				}
				break;
		
		}

		//left idc
		GUIRES_DisplayImg(
            					PNULL,
            					&left_idc_t,
            					PNULL,
            					win_id,
            					left_image_id,
            					&lcd_dev_info
            					);
		//right idc
		GUIRES_DisplayImg(
            					PNULL,
            					&right_idc_t,
            					PNULL,
            					win_id,
            					right_image_id,
            					&lcd_dev_info
            					);
		//focus image
		GUIRES_DisplayImg(
            					PNULL,
            					&focus_image_t,
            					PNULL,
            					win_id,
            					focus_image_id,
            					&lcd_dev_info
            					);



    	}

       
}



#endif
