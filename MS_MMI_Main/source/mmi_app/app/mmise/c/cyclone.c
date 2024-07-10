/****************************************************************************
** File Name:      cyclone.c                                                	                    *
** Author:           yangdingsheng                                                      		            *
** Date:           2011.05.15                                             	                            *
** Description:    This file is used to describe crystal cube effect.
****************************************************************************
**                         Important Edit History                         	 	*
** ------------------------------------------------------------------------*
** DATE           	        NAME             DESCRIPTION                       	*
** 2011.03.15        	yangdingsheng	
** 
****************************************************************************/
#include "mmi_app_mmise_trc.h"
#include "std_header.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_position.h"
#include "mmk_app.h"
#include "mmise.h"
#include "cyclone.h"
#include "block_mem_def.h"
#include "block_mem.h"
#include "graphics_3d_rotate.h"
/**---------------------------------------------------------------------------*
 **                         	Macro	Definition                                  *
 **---------------------------------------------------------------------------*/


#ifdef CYCLONE_DEBUGE
#ifdef SCI_TRACE_LOW
#undef SCI_TRACE_LOW
#define SCI_TRACE_LOW    SCI_TraceLow
#endif
#endif

#define SendNotify(id)  MMISE_SendMsgToMMI(id,PNULL,0)
#define SendNotifyWithParam(id,param,size)  MMISE_SendMsgToMMI(id,param,size)




#define CYCLONE_EYE_X	(WIN_WIDTH /2)
#define CYCLONE_EYE_Y	(WIN_HEIGHT * -12 / 64)
#define CYCLONE_EYE_Z	(-WIN_HEIGHT*3)
#define CYCLONE_NUM_ICON_PER_CIRCLE	(8)
#define CYCLONE_SLOPE_PARA	(6)					//龙卷风斜率
#define CYCLONE_SMALL_CIRCLE_WIDTH	(52)		//龙卷风小开口宽度
#define CYCLONE_LAYOUT_PARAM  30          //可以调整布局
#define S2D_SIN_BASE	(65536)


/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
typedef struct
{
    BOOLEAN is_forever;
    BOOLEAN is_init;
    uint16 width;
    uint16 height;
}CYCLONE_PARAM_T;




/**---------------------------------------------------------------------------*
 **                         		Constant Variables                                	*
 **---------------------------------------------------------------------------*/



/**---------------------------------------------------------------------------*
**                    		Local Functions Declare                                         	*
**---------------------------------------------------------------------------*/


LOCAL MMI_RESULT_E CycloneHandleProcessMsg(
        PWND app_ptr,
        uint16 msg_id,
        DPARAM param
        );

        

LOCAL void cyclone_do_operation(uint32 oper_type,void* param_ptr,uint32 param_size);

LOCAL void DualBuff_Exit(void);

/**---------------------------------------------------------------------------*
**                 			Extern Functions                                      	*
**---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/




LOCAL  uint8* g_highlight_layer_buf_ptr = PNULL;
LOCAL CYCLONE_PARAM_T g_cyclone_param = {0};
LOCAL CYCLONE_ANIM_PARAM_T g_cyclone_anim = {0};
LOCAL CYCLONE_SCROLL_PARAM_T g_cyclone_scroll = {0};
LOCAL BOOLEAN s_release_is_done = FALSE;

CYCLONE_MEM_BLOCK *cyclone_mem_ptr = PNULL;
LOCAL uint16 g_cyclone_icon_num = 0;
LOCAL int32 g_front_angle = 0;
LOCAL BOOLEAN s_dualbuff_exit = FALSE;
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

 
MMI_APPLICATION_T g_mmicyclone_app = {CycloneHandleProcessMsg, CT_CONTROL, PNULL};




/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**********************************************************************
*   CycloneHandleProcessMsg
*
*
**********************************************************************/
LOCAL MMI_RESULT_E CycloneHandleProcessMsg(
        PWND app_ptr,
        uint16 msg_id,
        DPARAM param
        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MAINMENUWIN_MENU_CTRL_ID;
    MmiOtherTaskMsgS* sig_ptr = (MmiOtherTaskMsgS*)param;
    
    if (msg_id > MMISE_SIG_BEGIN && msg_id < MMISE_SIG_END)
    {
        MMK_PostMsg(ctrl_id, sig_ptr->msg_id, sig_ptr->param_ptr, sig_ptr->size_of_param);

        if( sig_ptr->size_of_param )
        {
            SCI_FREE( sig_ptr->param_ptr );
        }
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }

    return (result);
}

/**********************************************************************
*   CYCLONE_IsMyMsg
*
*
**********************************************************************/
PUBLIC BOOLEAN CYCLONE_IsMyMsg(uint16 msg_id)
{
    if (msg_id > MMICYCLONE_MSG_BEGIN && msg_id < MMICYCLONE_MSG_END) return TRUE;

    return FALSE;
}
/**********************************************************************
*   MMIAPICYCLONE_Init
*
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_Init(MMICYCLONE_Init_PARAM_T* init_param_ptr)
{
    MMISE_CalculateTaskCreate();
    cyclone_do_operation(MMICYCLONE_MSG_PARAMINIT_REQ,(void*)init_param_ptr,sizeof(MMICYCLONE_Init_PARAM_T));
}
/**********************************************************************
*   MMIAPICYCLONE_Reset
*
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_Reset(MMICYCLONE_Init_PARAM_T* init_param_ptr)
{
    MMISE_CalculateTaskCreate();
    cyclone_do_operation(MMICYCLONE_MSG_PARAM_RESET_REQ,(void*)init_param_ptr,sizeof(MMICYCLONE_Init_PARAM_T));
    SCI_Sleep(200);
}

/**********************************************************************
*   MMIAPICYCLONE_ReleaseParam
*
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_ReleaseParam()
{
    int32 i = 0;
    s_release_is_done = FALSE;
    DualBuff_Exit();
    cyclone_do_operation(MMICYCLONE_MSG_PARAMRELEASE_REQ,PNULL,0);    

    /*要等待calc task处理完PARAMRELEASE_REQ之后才返回*/

    while ((!s_release_is_done)&&(i<50))
    {
        SCI_Sleep(10);
        i++;
    }
    s_release_is_done = TRUE;
    MMISE_CalculateTaskDestroy();
}
/**********************************************************************
*   MMIAPICYCLONE_Destroy
*
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_Destroy()
{
    DualBuff_Exit();
    cyclone_do_operation(MMICYCLONE_MSG_EXIT_REQ,PNULL,0);
    MMISE_CalculateTaskDestroy();
}
/**********************************************************************
*   MMIAPICYCLONE_StartEnterAnim
*
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_StartEnterAnim()
{
    cyclone_do_operation(MMICYCLONE_MSG_ANIM_START_REQ,PNULL,0);
}

/**********************************************************************
*   MMIAPICYCLONE_GetNextEnterAnimFrame
*
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_GetNextEnterAnimFrame()
{
    cyclone_do_operation(MMICYCLONE_MSG_ANIM_FRAME_CNF,PNULL,0);
}
/**********************************************************************
*   MMIAPICYCLONE_Rotate
*
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_Rotate(MMICYCLONE_ROTATE_PARAM_T *param)
{
    cyclone_do_operation(MMICYCLONE_MSG_ROTATE_REQ,(void*)param,sizeof(MMICYCLONE_ROTATE_PARAM_T));
}
/**********************************************************************
*   MMIAPICYCLONE_GetNextRotateFrame
*
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_GetNextRotateFrame()
{
    cyclone_do_operation(MMICYCLONE_MSG_ROTATE_FRAME_CNF,PNULL,0);
}
/**********************************************************************
*   MMIAPICYCLONE_TP_Down
*
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_TP_Down(MMICYCLONE_SCROLL_PARAM_T *param)
{
    cyclone_do_operation(MMICYCLONE_MSG_TP_DOWN_REQ,(void*)param,sizeof(MMICYCLONE_SCROLL_PARAM_T));
}
/**********************************************************************
*   MMIAPICYCLONE_TP_Move
*
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_TP_Move(MMICYCLONE_SCROLL_PARAM_T *param)
{
    cyclone_do_operation(MMICYCLONE_MSG_TP_SCROOL_REQ,(void*)param,sizeof(MMICYCLONE_SCROLL_PARAM_T));
}
/**********************************************************************
*   MMIAPICYCLONE_TP_Up_Anim
*
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_TP_Up_Anim(MMICYCLONE_SCROLL_PARAM_T *param)
{
    cyclone_do_operation(MMICYCLONE_MSG_SRCOLLANIM_REQ,(void*)param,sizeof(MMICYCLONE_SCROLL_PARAM_T));
}
/**********************************************************************
*   MMIAPICYCLONE_GetNextTpUpAnimFrame
*
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_GetNextTpUpAnimFrame()
{
    cyclone_do_operation(MMICYCLONE_MSG_SRCOLLANIM_CNF,PNULL,0);
}
/**********************************************************************
*   MMIAPICYCLONE_GetNextScrollFrame
*
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_GetNextScrollFrame()
{
    cyclone_do_operation(MMICYCLONE_MSG_TP_SCROOL_CNF,PNULL,0);
}
/**********************************************************************
*   MMIAPICYCLONE_GetHighLightFrame
*
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_GetHighLightFrame()
{
    cyclone_do_operation(MMICYCLONE_MSG_HIGHLIGHT_FRAME_REQ,PNULL,0);
}

/**********************************************************************
*MMIAPICYCLONE_CreatMemBlock
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_CreatMemBlock(uint16 icon_num)
{
    if (PNULL == cyclone_mem_ptr)
    {
        cyclone_mem_ptr = (CYCLONE_MEM_BLOCK*)SCI_ALLOC_APP(sizeof(CYCLONE_MEM_BLOCK));
    }
    SCI_MEMSET(cyclone_mem_ptr, 0, sizeof(CYCLONE_MEM_BLOCK));
    
    if (PNULL == cyclone_mem_ptr->icon_buff)
    {
        cyclone_mem_ptr->icon_buff = (uint32**)SCI_ALLOC_APP((icon_num+1)*MENUICON_WIDTH*MENUICON_HEIGHT*sizeof(uint32));
    }
    SCI_MEMSET(cyclone_mem_ptr->icon_buff, 0, (icon_num+1)*MENUICON_WIDTH*MENUICON_HEIGHT*sizeof(uint32));
    
    if (PNULL == cyclone_mem_ptr->icon_buff_adress)
    {
        cyclone_mem_ptr->icon_buff_adress = (uint32**)SCI_ALLOC_APP(icon_num*sizeof(uint32));
    }
    SCI_MEMSET(cyclone_mem_ptr->icon_buff_adress, 0, icon_num*sizeof(uint32));
    
    if (PNULL == cyclone_mem_ptr->icon_adress_backup)
    {
        cyclone_mem_ptr->icon_adress_backup = (uint32**)SCI_ALLOC_APP(icon_num*sizeof(uint32));
    }
    SCI_MEMSET(cyclone_mem_ptr->icon_adress_backup, 0, icon_num*sizeof(uint32));
    
    if (PNULL == cyclone_mem_ptr->sort_axisz)
    {
        cyclone_mem_ptr->sort_axisz = (uint16*)SCI_ALLOC_APP((icon_num+1)*sizeof(uint16));
    }
    SCI_MEMSET(cyclone_mem_ptr->sort_axisz, 0, (icon_num+1)*sizeof(uint16));
    
    if (PNULL == cyclone_mem_ptr->s3d_geometry)
    {
        cyclone_mem_ptr->s3d_geometry = (S3D_ROTATION_GEOMETRY_T*)SCI_ALLOC_APP((icon_num+1)*sizeof(S3D_ROTATION_GEOMETRY_T));
    }
    SCI_MEMSET(cyclone_mem_ptr->s3d_geometry, 0, (icon_num+1)*sizeof(S3D_ROTATION_GEOMETRY_T));
    
}

/**********************************************************************
*   MMIAPICYCLONE_ReleaseMemBlock
*
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_ReleaseMemBlock()
{
    if(PNULL != cyclone_mem_ptr)
    {
        if (PNULL != cyclone_mem_ptr->icon_buff)
        {
            SCI_FREE(cyclone_mem_ptr->icon_buff);
            cyclone_mem_ptr->icon_buff = PNULL;
        }
        
        if (PNULL != cyclone_mem_ptr->icon_buff_adress)
        {
            SCI_FREE(cyclone_mem_ptr->icon_buff_adress);
            cyclone_mem_ptr->icon_buff_adress = PNULL;
        }
        
        if (PNULL != cyclone_mem_ptr->icon_adress_backup)
        {
            SCI_FREE(cyclone_mem_ptr->icon_adress_backup);
            cyclone_mem_ptr->icon_adress_backup = PNULL;
        }
        
        if (PNULL != cyclone_mem_ptr->sort_axisz)
        {
            SCI_FREE(cyclone_mem_ptr->sort_axisz);
            cyclone_mem_ptr->sort_axisz = PNULL;
        }
        
        if (PNULL != cyclone_mem_ptr->s3d_geometry)
        {
            SCI_FREE(cyclone_mem_ptr->s3d_geometry);
            cyclone_mem_ptr->s3d_geometry = PNULL;
        }
        
        SCI_FREE(cyclone_mem_ptr);
        cyclone_mem_ptr = PNULL;
    }
}


/**********************************************************************
*   MMIAPICYCLONE_DualBuff_init
*
*
**********************************************************************/
PUBLIC void MMIAPICYCLONE_DualBuff_init(uint8 * buffer1,uint8 *buffer2)
{
    if((PNULL == buffer1)||(PNULL == buffer2))
    {
        return;
    }
	cyclone_mem_ptr->dbuff_inf.readidx = 1;
	cyclone_mem_ptr->dbuff_inf.writeidx= 1;
	cyclone_mem_ptr->dbuff_inf.buffer[0]= buffer1;
	cyclone_mem_ptr->dbuff_inf.buffer[1]= buffer2;

	s_dualbuff_exit = FALSE;
}
/**********************************************************************
*   MMIAPICYCLONE_DualBuff_GetReadBuff
*
*
**********************************************************************/
PUBLIC uint8* MMIAPICYCLONE_DualBuff_GetReadBuff(void)
{
	uint8* read_buff = PNULL;
	while((cyclone_mem_ptr->dbuff_inf.readidx == cyclone_mem_ptr->dbuff_inf.writeidx) && (s_dualbuff_exit))
	{
		SCI_Sleep(1);
	}

	if (s_dualbuff_exit) return PNULL;
	
	read_buff = cyclone_mem_ptr->dbuff_inf.buffer[cyclone_mem_ptr->dbuff_inf.readidx];
	cyclone_mem_ptr->dbuff_inf.readidx = ((cyclone_mem_ptr->dbuff_inf.readidx)?0:1);
	return read_buff;
}
/**********************************************************************
*   MMICYCLONE_DualBuff_GetWriteBuff
*
*
**********************************************************************/
PUBLIC uint8* MMICYCLONE_DualBuff_GetWriteBuff(void)
{
	uint8* write_buff = PNULL;
    if (s_dualbuff_exit) return PNULL;
	while((cyclone_mem_ptr->dbuff_inf.readidx != cyclone_mem_ptr->dbuff_inf.writeidx) && (s_dualbuff_exit))
	{
		SCI_Sleep(1);
	}
	
	write_buff = cyclone_mem_ptr->dbuff_inf.buffer[cyclone_mem_ptr->dbuff_inf.writeidx];
	cyclone_mem_ptr->dbuff_inf.writeidx = ((cyclone_mem_ptr->dbuff_inf.writeidx)?0:1);
	
	return write_buff;
}
/**********************************************************************
*   DualBuff_Exit
*
*
**********************************************************************/
LOCAL void DualBuff_Exit(void)
{
    s_dualbuff_exit = TRUE;
    SCI_Sleep(10);
}
/**********************************************************************
*   cyclone_axisz_sort
*
*
**********************************************************************/
LOCAL BOOLEAN cyclone_axisz_sort(S3D_ROTATION_GEOMETRY_T *Geomrtry_Ptr,					//数据首地址
					   int32 iLen)			//判断大小依据
{
	uint16 i = 0;
	uint16 j = 0;
	BOOLEAN isChanged = SCI_TRUE;								//链表排序标志，当没有变化是退出冒泡
	uint16 Geomrtry_Temp;

	for( i = 0 ; i < iLen-1 ; i ++)
	{
		if(isChanged)
		{
			isChanged = SCI_FALSE;
			for( j = 1 ; j < iLen - i ; j ++)
			{
				if(Geomrtry_Ptr[cyclone_mem_ptr->sort_axisz[j-1]].axis.z>Geomrtry_Ptr[cyclone_mem_ptr->sort_axisz[j]].axis.z)
				{
				}
				else
				{
					isChanged = SCI_TRUE;
					Geomrtry_Temp = cyclone_mem_ptr->sort_axisz[j-1];
					cyclone_mem_ptr->sort_axisz[j-1] = cyclone_mem_ptr->sort_axisz[j];
					cyclone_mem_ptr->sort_axisz[j] = Geomrtry_Temp;
				}
			}
		}
	}
	return SCI_TRUE;
}

/**********************************************************************
*   cyclone_init_param
*
*
**********************************************************************/
LOCAL void cyclone_init_param(MMICYCLONE_Init_PARAM_T* init_param_ptr,BOOLEAN is_reset)
{
    int i=0;
    int x,y=0;
    uint16 item_temp = 0;
    int temp_i = 0;
    int temp_y = 0;
    
    g_cyclone_icon_num = MIN(init_param_ptr->cyclone_max_item, CYCLONE_MENU_ITEM_MAX);//icon_num
    g_front_angle = ((3600*1*(g_cyclone_icon_num-1))/8);
    
   SCI_MEMSET(cyclone_mem_ptr->s3d_geometry,0x00,(sizeof(S3D_ROTATION_GEOMETRY_T)*(g_cyclone_icon_num-1)));

	if((init_param_ptr->cur_item_index-(init_param_ptr->cur_select_pos_index*ITEM_NUM_ONE_CIRCLE))>=0)
		{
			item_temp= init_param_ptr->cur_item_index-(init_param_ptr->cur_select_pos_index*ITEM_NUM_ONE_CIRCLE);
		}
	else
		{
			item_temp=g_cyclone_icon_num + init_param_ptr->cur_item_index-(init_param_ptr->cur_select_pos_index*ITEM_NUM_ONE_CIRCLE);
		}
	
    for( i = 0 ; i < g_cyclone_icon_num; i++)
    {
        temp_i = i/4*MENUICON_WIDTH*4*MENUICON_HEIGHT + (i%4)*MENUICON_WIDTH;
            
        for( y = 0 ; y < MENUICON_HEIGHT ; y++)
        {
            temp_y = y*MENUICON_WIDTH*4 + temp_i;
            
            for( x = 0 ; x < MENUICON_WIDTH ; x++)
            {
                //cyclone_mem_ptr->icon_buff[i][ x*MENUICON_HEIGHT+y] = ((uint32 *)(init_param_ptr->all_icon_buffer_ptr))[temp_y+x];
                SCI_MEMCPY(&cyclone_mem_ptr->icon_buff[i][ x*MENUICON_HEIGHT+y],&((uint32 *)(init_param_ptr->all_icon_buffer_ptr))[temp_y+x],sizeof(uint32));
            }
        }

	    cyclone_mem_ptr->s3d_geometry[i].eye.x = CYCLONE_EYE_X;
	    cyclone_mem_ptr->s3d_geometry[i].eye.y = CYCLONE_EYE_Y;
	    cyclone_mem_ptr->s3d_geometry[i].eye.z = CYCLONE_EYE_Z;
		





	    cyclone_mem_ptr->s3d_geometry[i].axis.axis = 1;

	    cyclone_mem_ptr->s3d_geometry[i].is_axis_use_default = SCI_FALSE;
	    cyclone_mem_ptr->s3d_geometry[i].is_eye_use_default = SCI_FALSE;

	    cyclone_mem_ptr->s3d_geometry[i].img_size.x = 0;
	    cyclone_mem_ptr->s3d_geometry[i].img_size.y = 0;

		if(i+item_temp<g_cyclone_icon_num)
			{
			       cyclone_mem_ptr->icon_buff_adress[i+item_temp] = &cyclone_mem_ptr->icon_buff[i][0];
			}
		else
			{
			       cyclone_mem_ptr->icon_buff_adress[i+item_temp-g_cyclone_icon_num]= &cyclone_mem_ptr->icon_buff[i][0];
			}
    }
	
    g_highlight_layer_buf_ptr = init_param_ptr->highlight_buffer_ptr;

   SCI_MEMSET(&g_cyclone_scroll,0x00,sizeof(CYCLONE_SCROLL_PARAM_T));
   SCI_MEMSET(&g_cyclone_anim,0x00,sizeof(CYCLONE_ANIM_PARAM_T));
   SCI_MEMSET(&g_cyclone_param,0x0,sizeof(CYCLONE_PARAM_T));

   g_cyclone_param.is_init = TRUE;

    if (is_reset)
    {
	   SendNotify(MMICYCLONE_MSG_PARAM_RESET_CNF);
    }
    else
    {
	   SendNotify(MMICYCLONE_MSG_PARAMINIT_CNF);
    }
   
    
}
/**********************************************************************
*   cyclone_deinit_param
*
*
**********************************************************************/
LOCAL void cyclone_deinit_param()
{
    g_cyclone_scroll.tp_state = STATE_SCROLL_NONE;
    g_cyclone_param.is_init = FALSE;
    //SendNotify(MMICYCLONE_MSG_PARAMRELEASE_CNF);    
    g_cyclone_icon_num = 0;
}
/**********************************************************************
*   cyclone_calc_draw_win_by_angle
*
*
**********************************************************************/
void cyclone_calc_draw_win_by_angle(int cyclone_angle,BOOLEAN is_highlight)
{

    int i =0;
    S3D_ROTATION_INOUT_IMG_INFO_T s3d_window;
    S3D_ROTATION_IMG_T s3d_img;
    IMGREF_SIZE_T size;
    uint8 *write_buff_ptr = PNULL;
    int row_num = 0;
	write_buff_ptr = MMICYCLONE_DualBuff_GetWriteBuff();

	if (PNULL == write_buff_ptr) return;
	
	SCI_MEMSET(write_buff_ptr,0,WIN_WIDTH * WIN_HEIGHT*sizeof(COLOR32));

	
    s3d_window.color_ptr = write_buff_ptr;
    s3d_window.size.w = WIN_WIDTH-0;
    s3d_window.size.h = WIN_HEIGHT;
    s3d_img.src_type = 0;//S3D_DATA_ARGB888; 
    size.h = MENUICON_HEIGHT;
    size.w = MENUICON_WIDTH;

    for( i = 0 ; i < g_cyclone_icon_num+1 ; i++)
    {
        int round = 0;
        int32 angle = 0;
        cyclone_mem_ptr->sort_axisz[i] = (uint16)i;

        angle = (int32)(cyclone_angle - i*(CIRCLE_ANGLE/CYCLONE_NUM_ICON_PER_CIRCLE));

        if(angle>=0)
        {
        	cyclone_mem_ptr->s3d_geometry[i].angle = CIRCLE_ANGLE/2-angle;
        	cyclone_mem_ptr->s3d_geometry[i].axis.y = FENG_HEIGHT-(FENG_HEIGHT*angle/(CIRCLE_ANGLE*CYCLONE_LAYOUT_PARAM/CYCLONE_NUM_ICON_PER_CIRCLE));//+(HEIGHT-FENG_HEIGHT)/3;
        	round = (FENG_HEIGHT*angle/(CIRCLE_ANGLE*CYCLONE_LAYOUT_PARAM/CYCLONE_NUM_ICON_PER_CIRCLE))/CYCLONE_SLOPE_PARA+CYCLONE_SMALL_CIRCLE_WIDTH;

        	cyclone_mem_ptr->s3d_geometry[i].axis.x = WIN_WIDTH/2-round*s2d_sin_new(angle)/S2D_SIN_BASE;
        	cyclone_mem_ptr->s3d_geometry[i].axis.z = -round*s2d_cos_new(angle)/S2D_SIN_BASE;
        }
        else
        {
        	cyclone_mem_ptr->s3d_geometry[i].angle = CIRCLE_ANGLE/2;
        	cyclone_mem_ptr->s3d_geometry[i].axis.y = FENG_HEIGHT-(FENG_HEIGHT*angle*10/(CIRCLE_ANGLE*CYCLONE_LAYOUT_PARAM/CYCLONE_NUM_ICON_PER_CIRCLE));//+(HEIGHT-FENG_HEIGHT)/3;
        	angle = 0;
        	round = (FENG_HEIGHT*angle/(CIRCLE_ANGLE*CYCLONE_LAYOUT_PARAM/CYCLONE_NUM_ICON_PER_CIRCLE))/CYCLONE_SLOPE_PARA+CYCLONE_SMALL_CIRCLE_WIDTH;
        	cyclone_mem_ptr->s3d_geometry[i].axis.x = WIN_WIDTH/2-round*s2d_sin_new(angle)/S2D_SIN_BASE;
        	cyclone_mem_ptr->s3d_geometry[i].axis.z = -round*s2d_cos_new(angle)/S2D_SIN_BASE;

        }
    }

    cyclone_mem_ptr->s3d_geometry[g_cyclone_icon_num].eye.x = CYCLONE_EYE_X;
    cyclone_mem_ptr->s3d_geometry[g_cyclone_icon_num].eye.y = CYCLONE_EYE_Y;
    cyclone_mem_ptr->s3d_geometry[g_cyclone_icon_num].eye.z = CYCLONE_EYE_Z;


    cyclone_mem_ptr->s3d_geometry[g_cyclone_icon_num].axis.axis = 1;

    cyclone_mem_ptr->s3d_geometry[g_cyclone_icon_num].is_axis_use_default = SCI_FALSE;
    cyclone_mem_ptr->s3d_geometry[g_cyclone_icon_num].is_eye_use_default = SCI_FALSE;

    cyclone_mem_ptr->s3d_geometry[g_cyclone_icon_num].img_size.x = 0;
    cyclone_mem_ptr->s3d_geometry[g_cyclone_icon_num].img_size.y = 0;

    cyclone_axisz_sort(&cyclone_mem_ptr->s3d_geometry[0],g_cyclone_icon_num+1);

    for( i = g_cyclone_icon_num ; i >=0 ; i--)
    {
        int z = 0;
        uint16 sort_i = cyclone_mem_ptr->sort_axisz[i];
        int temp_cos = s2d_cos_new(cyclone_mem_ptr->s3d_geometry[sort_i].angle);
        uint32* icon_res = ((sort_i>=g_cyclone_icon_num)?(cyclone_mem_ptr->icon_buff_adress[0]):(cyclone_mem_ptr->icon_buff_adress[sort_i]));
        if((temp_cos) > 1000 || (temp_cos) <- 1000)//不显示龙卷风两边的竖线，也就是90度附近的icon
        {		
            if( temp_cos < -CYCLONE_SIN_BASE)   //正对面的图片
            {
        	    if(is_highlight)
        	    {
        			uint16 index= sort_i/ITEM_NUM_ONE_CIRCLE;

        			if(sort_i>(g_cyclone_icon_num-1))
        			{
        				continue;
        			}
                    //先判断需要几圈
                    row_num = (g_cyclone_icon_num/ITEM_NUM_ONE_CIRCLE);
                    row_num = (((g_cyclone_icon_num%ITEM_NUM_ONE_CIRCLE)>0)?(row_num+1):row_num);   
                    //保证只能显示四个可选ICON
                    if(index <(row_num-SELECT_ICON_NUM_IN_WIN ))
                    {
                        continue;
                    }
                    else
                    {
                        index = index - (row_num-SELECT_ICON_NUM_IN_WIN );
                    }
        			s3d_img.src_ptr = &cyclone_mem_ptr->highlightIcon[index][0];
        			cyclone_mem_ptr->s3d_geometry[sort_i].img_size.w = HIGHLIGHT_ICON_WIDTH;
        			cyclone_mem_ptr->s3d_geometry[sort_i].img_size.h= (HIGHLIGHT_ICON_HEIGHT);
            	}
            	else
            	{
        			s3d_img.src_ptr = icon_res;
        			cyclone_mem_ptr->s3d_geometry[sort_i].img_size.w = MENUICON_WIDTH;
        			cyclone_mem_ptr->s3d_geometry[sort_i].img_size.h = (MENUICON_HEIGHT);
            	}
            }
            else
            {
        		cyclone_mem_ptr->s3d_geometry[sort_i].img_size.w = MENUICON_WIDTH;
        		cyclone_mem_ptr->s3d_geometry[sort_i].img_size.h = (MENUICON_HEIGHT);
        		z =   255 - 80 -((80 * temp_cos)>>16);
        		S3D_ImageModifyBrightness(size,
        								z,
        								icon_res,
        								&cyclone_mem_ptr->icon_buff[g_cyclone_icon_num][0]);

        		s3d_img.src_ptr = &cyclone_mem_ptr->icon_buff[g_cyclone_icon_num][0];
        	}

            if(sort_i>g_cyclone_icon_num)
            {
                return;
            }
            if(PNULL == cyclone_mem_ptr)
            {
                return;
            }
            if(cyclone_mem_ptr->s3d_geometry[sort_i].axis.y > (MENUICON_HEIGHT/2) && cyclone_mem_ptr->s3d_geometry[sort_i].axis.y < (WIN_HEIGHT-(MENUICON_HEIGHT/2)) )
            {
                S3D_ImageMidAxisRotation(&cyclone_mem_ptr->s3d_geometry[sort_i],
                				&s3d_img,
                				&s3d_window);
            }
        }
    }
}

/**********************************************************************
*   cyclone_calc_highlight_frame
*
*
**********************************************************************/
LOCAL void cyclone_calc_highlight_frame(void)
{
	int i,x,y;
	int temp_angle = (g_front_angle);
    int temp_i = 0;
    int temp_y = 0;
	if (!g_cyclone_param.is_init) return;
	
	//SCI_TRACE_LOW:"cyclone_calc_highlight_frame begin %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CYCLONE_723_112_2_18_2_37_27_23,(uint8*)"d",SCI_GetTickCount());

	for( i = 0 ; i < SELECT_ICON_NUM_IN_WIN; i++)
	{
	    temp_i = i*HIGHLIGHT_ICON_WIDTH*HIGHLIGHT_ICON_HEIGHT;
        
	    for( y = 0 ; y < HIGHLIGHT_ICON_HEIGHT ; y++)
	    {
	        temp_y = y*HIGHLIGHT_ICON_WIDTH + temp_i;
            
	        for( x = 0 ; x < HIGHLIGHT_ICON_WIDTH ; x++)
	        {
	            //cyclone_mem_ptr->highlightIcon[i][x*HIGHLIGHT_ICON_HEIGHT+y]= ((uint32 *)g_highlight_layer_buf_ptr)[i*HIGHLIGHT_ICON_WIDTH*HIGHLIGHT_ICON_HEIGHT+y*HIGHLIGHT_ICON_WIDTH+x];
                    SCI_MEMCPY(& cyclone_mem_ptr->highlightIcon[i][x*HIGHLIGHT_ICON_HEIGHT+y],&((uint32 *)g_highlight_layer_buf_ptr)[temp_y+x],(sizeof(uint32)));
            }
	    }

	}

	cyclone_calc_draw_win_by_angle(temp_angle,TRUE);

	//SCI_TRACE_LOW:"cyclone_calc_highlight_frame end %d , angle=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CYCLONE_744_112_2_18_2_37_27_24,(uint8*)"dd",SCI_GetTickCount(),temp_angle);
       SendNotify(MMICYCLONE_MSG_HIGHLIGHT_FRAME_IND);
}
/**********************************************************************
*   cyclone_calc_anim_frame
*
*
**********************************************************************/
LOCAL void cyclone_calc_anim_frame()
{
    if (!g_cyclone_param.is_init) return;
    
    g_cyclone_anim.calc_angle+=ANIM_ROLL_OFF;
	//SCI_TRACE_LOW:"calc_anim_frame begin %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CYCLONE_756_112_2_18_2_37_27_25,(uint8*)"d",SCI_GetTickCount());
    if(g_cyclone_anim.calc_angle>=g_front_angle)
    {
	    cyclone_calc_highlight_frame();
		//SendNotify(MMICYCLONE_MSG_HIGHLIGHT_FRAME_IND);
    }
	else
	{
	
		cyclone_calc_draw_win_by_angle(g_cyclone_anim.calc_angle,FALSE);
		SendNotify(MMICYCLONE_MSG_ANIM_FRAME_IND);
	}
    //SCI_TRACE_LOW:"calc_anim_frame end %d , angle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CYCLONE_768_112_2_18_2_37_27_26,(uint8*)"dd",SCI_GetTickCount(),g_cyclone_anim.calc_angle);
}
/**********************************************************************
*   chang_res_by_angle
*
*
**********************************************************************/
LOCAL void chang_res_by_angle(int *angle,BOOLEAN is_use_backup)
{
    int32 i = 0;
    int32 angle_temp;

    //SCI_TRACE_LOW:"chang_res_by_angle start angle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CYCLONE_779_112_2_18_2_37_27_27,(uint8*)"d",*angle);

    if(!is_use_backup)
    {
    	SCI_MEMCPY(&cyclone_mem_ptr->icon_adress_backup[0], &cyclone_mem_ptr->icon_buff_adress[0], (sizeof(uint32*)*g_cyclone_icon_num));
    }

    angle_temp = *angle;
    i=angle_temp/ANGLE_BETWEEN_ICON;
    i=i%g_cyclone_icon_num;

    if(i>0)
    {
    	SCI_MEMCPY(&cyclone_mem_ptr->icon_buff_adress[0],&cyclone_mem_ptr->icon_adress_backup[(uint32)i], sizeof(uint32*)*(g_cyclone_icon_num-(uint32)i));
    	SCI_MEMCPY(&cyclone_mem_ptr->icon_buff_adress[g_cyclone_icon_num- (uint32)i],&cyclone_mem_ptr->icon_adress_backup[0], sizeof(uint32*)*((uint32)i));
    }
    else if(i<0)
    {
    	i=-i;
    	SCI_MEMCPY(&cyclone_mem_ptr->icon_buff_adress[(uint32)i], &cyclone_mem_ptr->icon_adress_backup[0], sizeof(uint32*)*(g_cyclone_icon_num-(uint32)i));
    	SCI_MEMCPY(&cyclone_mem_ptr->icon_buff_adress[0], &cyclone_mem_ptr->icon_adress_backup[g_cyclone_icon_num - (uint32)i], sizeof(uint32*)*((uint32)i));
    }
    else
    {
    	SCI_MEMCPY(&cyclone_mem_ptr->icon_buff_adress[0],&cyclone_mem_ptr->icon_adress_backup[0],(sizeof(uint32*)*g_cyclone_icon_num));
    }


    *angle = angle_temp%ANGLE_BETWEEN_ICON;

    //SCI_TRACE_LOW:"chang_res_by_angle end i=%d , angle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CYCLONE_809_112_2_18_2_37_27_28,(uint8*)"dd",i,*angle);
}
/**********************************************************************
*   backup_icon_res_link
*
*
**********************************************************************/
LOCAL void backup_icon_res_link(void)
{
    SCI_MEMCPY(cyclone_mem_ptr->icon_adress_backup, cyclone_mem_ptr->icon_buff_adress, (sizeof(uint32*)*g_cyclone_icon_num));
}

/**********************************************************************
*   sync_icon_res_link
*
*
**********************************************************************/
LOCAL void sync_icon_res_link(void)
{
    uint16 i=0;
    uint16 item_temp = 0;
	if((g_cyclone_scroll.cur_item_index-(g_cyclone_scroll.cur_select_pos_index*ITEM_NUM_ONE_CIRCLE))>=0)
		{
			item_temp= g_cyclone_scroll.cur_item_index-(g_cyclone_scroll.cur_select_pos_index*ITEM_NUM_ONE_CIRCLE);
		}
	else
		{
			item_temp=g_cyclone_icon_num + g_cyclone_scroll.cur_item_index-(g_cyclone_scroll.cur_select_pos_index*ITEM_NUM_ONE_CIRCLE);
		}

    for( i = 0 ; i < g_cyclone_icon_num; i++)
    {

		if(i+item_temp<g_cyclone_icon_num)
			{
			       cyclone_mem_ptr->icon_buff_adress[i+item_temp] = &cyclone_mem_ptr->icon_buff[i][0];
			}
		else
			{
			       cyclone_mem_ptr->icon_buff_adress[i+item_temp-g_cyclone_icon_num]= &cyclone_mem_ptr->icon_buff[i][0];
			}
    }
}

/**********************************************************************
*   cyclone_calc_rotate_frame
*
*   
**********************************************************************/
LOCAL void cyclone_calc_rotate_frame()
{
	BOOLEAN is_rotate_end=FALSE;
	int angle_this_frame = 0;
	int angle_pre_frame = 0;

	if (!g_cyclone_param.is_init) return;

	//SCI_ASSERT((g_cyclone_anim.direction==ROTATE_DIRECTION_LEFT)||(g_cyclone_anim.direction==ROTATE_DIRECTION_RIGHT));  /*assert verified*/
	if (
        (g_cyclone_anim.direction != ROTATE_DIRECTION_LEFT)&&
        (g_cyclone_anim.direction != ROTATE_DIRECTION_RIGHT)
        )
	{
        return;
	}

	//SCI_TRACE_LOW:"cyclone_calc_rotate_frame begin %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CYCLONE_872_112_2_18_2_37_27_29,(uint8*)"d",SCI_GetTickCount());

	if(g_cyclone_anim.direction == ROTATE_DIRECTION_RIGHT)
	{
		angle_pre_frame = g_cyclone_anim.start_angle - g_cyclone_anim.cur_angle;
		
		angle_this_frame = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2,
								        (g_cyclone_anim.start_angle - g_cyclone_anim.stop_angle) ,
								        g_cyclone_anim.total_frame, 
								        g_cyclone_anim.cur_frame);
								        
		g_cyclone_anim.cur_angle=g_cyclone_anim.start_angle - angle_this_frame;
	    g_cyclone_anim.cur_frame_offset_angle =g_cyclone_anim.cur_frame_offset_angle -(angle_this_frame-angle_pre_frame);
		chang_res_by_angle(&g_cyclone_anim.cur_frame_offset_angle,FALSE);
		if(g_cyclone_anim.cur_angle <= g_cyclone_anim.stop_angle)
		{
			is_rotate_end = TRUE;
		}
	}
	else   //if(g_cyclone_anim.direction == ROTATE_DIRECTION_LEFT)
	{
		angle_pre_frame = g_cyclone_anim.cur_angle - g_cyclone_anim.start_angle;
		angle_this_frame = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2,
								        (g_cyclone_anim.stop_angle - g_cyclone_anim.start_angle) ,
								        g_cyclone_anim.total_frame, 
								        g_cyclone_anim.cur_frame);
		g_cyclone_anim.cur_angle=g_cyclone_anim.start_angle + angle_this_frame;
	    g_cyclone_anim.cur_frame_offset_angle =g_cyclone_anim.cur_frame_offset_angle +(angle_this_frame-angle_pre_frame);
		chang_res_by_angle(&g_cyclone_anim.cur_frame_offset_angle,FALSE);
		if(g_cyclone_anim.cur_angle >= g_cyclone_anim.stop_angle)
		{
			is_rotate_end = TRUE;
		}
	}

	if(is_rotate_end)
	{
		cyclone_calc_highlight_frame();
		#ifdef WIN32
		SCI_Sleep(10);
		#endif
		//SendNotify(MMICYCLONE_MSG_HIGHLIGHT_FRAME_IND);
	}
	else
	{
		g_cyclone_anim.calc_angle = g_front_angle + g_cyclone_anim.cur_frame_offset_angle;
		cyclone_calc_draw_win_by_angle(g_cyclone_anim.calc_angle,FALSE);
		SendNotify(MMICYCLONE_MSG_ROTATE_FRAME_IND);
	}
	//SCI_TRACE_LOW:"cyclone_calc_rotate_frame end %d , angle=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CYCLONE_921_112_2_18_2_37_27_30,(uint8*)"dd",SCI_GetTickCount(),g_cyclone_anim.calc_angle);
	g_cyclone_anim.cur_frame ++;
}

/**********************************************************************
*   cyclone_calc_scroll_frame
*
*   
**********************************************************************/
LOCAL void cyclone_calc_scroll_frame()
{
	int angle_this_frame = 0;

	if (!g_cyclone_param.is_init) return;
	
	//SCI_TRACE_LOW:"cyclone_calc_scroll_frame begin %d, angle=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CYCLONE_935_112_2_18_2_37_27_31,(uint8*)"dd",SCI_GetTickCount(),g_cyclone_scroll.scroll_angle);

	angle_this_frame = g_cyclone_scroll.scroll_angle;
	chang_res_by_angle(&angle_this_frame,TRUE);

	angle_this_frame = g_front_angle + angle_this_frame;
	cyclone_calc_draw_win_by_angle(angle_this_frame,FALSE);
	SendNotify(MMICYCLONE_MSG_TP_SCROOL_IND);
	//SCI_TRACE_LOW:"cyclone_calc_scroll_frame end %d , angle=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CYCLONE_943_112_2_18_2_37_28_32,(uint8*)"dd",SCI_GetTickCount(),g_cyclone_scroll.scroll_angle);
}

/**********************************************************************
*   cyclone_calc_scroll_anim_frame
*
*   计算scroll anim frame
**********************************************************************/
LOCAL void cyclone_calc_scroll_anim_frame()
{
    int angle_this_frame = 0;

    if (!g_cyclone_param.is_init) return;
    
    //SCI_TRACE_LOW:"cyclone_calc_scroll_anim_frame begin %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CYCLONE_956_112_2_18_2_37_28_33,(uint8*)"d",SCI_GetTickCount());
    
    if(g_cyclone_scroll.inertia_angle == 0)
    {	
	    	if(g_cyclone_scroll.anim_angle%ANGLE_BETWEEN_ICON == 0)
	    	{
			SCI_ASSERT((g_cyclone_scroll.catch_stop_angle - g_cyclone_scroll.anim_angle) == 0);/*assert verified*/
			//angle_this_frame = g_cyclone_scroll.anim_angle;
			//chang_res_by_angle(&angle_this_frame,TRUE);
			sync_icon_res_link();
			cyclone_calc_highlight_frame();
	        	//SendNotify(MMICYCLONE_MSG_HIGHLIGHT_FRAME_IND);
	    	}
	    	else
	    	{
                if (g_cyclone_scroll.total_catch_frame == 0)
		    		{
					g_cyclone_scroll.anim_angle += g_cyclone_scroll.catch_angle;
					SCI_ASSERT((g_cyclone_scroll.catch_stop_angle - g_cyclone_scroll.anim_angle) == 0);/*assert verified*/
					//angle_this_frame = g_cyclone_scroll.anim_angle;
					//chang_res_by_angle(&angle_this_frame,TRUE);
					sync_icon_res_link();
					cyclone_calc_highlight_frame();
					//SendNotify(MMICYCLONE_MSG_HIGHLIGHT_FRAME_IND);
		    		}
               else  if (g_cyclone_scroll.cur_catch_frame > (g_cyclone_scroll.total_catch_frame))
	                {
					SCI_ASSERT(0 == 0);/*assert verified*/
					SCI_ASSERT((g_cyclone_scroll.catch_stop_angle - g_cyclone_scroll.anim_angle) == 0);/*assert verified*/
					SCI_ASSERT((g_cyclone_scroll.total_catch_frame) < 10);/*assert verified*/
					//angle_this_frame = g_cyclone_scroll.anim_angle;
					//chang_res_by_angle(&angle_this_frame,TRUE);
					sync_icon_res_link();
					cyclone_calc_highlight_frame();
			            	//SendNotify(MMICYCLONE_MSG_HIGHLIGHT_FRAME_IND);
	                }
                else
	                {
					angle_this_frame = (int)MMK_ClcltDampPlayOffset(
										DAMP_PLAY_TYPE_2,
										g_cyclone_scroll.catch_angle,
										g_cyclone_scroll.total_catch_frame,
										g_cyclone_scroll.cur_catch_frame);

					g_cyclone_scroll.anim_angle = g_cyclone_scroll.catch_start_angle + angle_this_frame;
					angle_this_frame = g_cyclone_scroll.anim_angle;
					chang_res_by_angle(&angle_this_frame,TRUE);
					angle_this_frame = g_front_angle + angle_this_frame;
					cyclone_calc_draw_win_by_angle(angle_this_frame,FALSE);
					SendNotifyWithParam(MMICYCLONE_MSG_SRCOLLANIM_IND,(void *)((&g_cyclone_scroll.anim_angle)),sizeof(int));
					//SCI_TRACE_LOW:"cyclone_calc_scroll_anim_frame cur_catch_frame=%d catch_start_angle=%d anim_angle=%d"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,CYCLONE_1006_112_2_18_2_37_28_34,(uint8*)"ddd",g_cyclone_scroll.cur_catch_frame,g_cyclone_scroll.catch_start_angle,g_cyclone_scroll.anim_angle);
					g_cyclone_scroll.cur_catch_frame ++;
	                }
    		}
    }
    else
    {
	g_cyclone_scroll.anim_angle -= g_cyclone_scroll.inertia_angle;
	//SCI_TRACE_LOW:"g_cyclone_scroll.inertia_angle=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CYCLONE_1014_112_2_18_2_37_28_35,(uint8*)"d",g_cyclone_scroll.inertia_angle);

	angle_this_frame = g_cyclone_scroll.anim_angle;

	chang_res_by_angle(&angle_this_frame,TRUE);

	angle_this_frame = g_front_angle + angle_this_frame;
	cyclone_calc_draw_win_by_angle(angle_this_frame,FALSE);
	SendNotifyWithParam(MMICYCLONE_MSG_SRCOLLANIM_IND,(void *)((&g_cyclone_scroll.anim_angle)),sizeof(int));
        if (g_cyclone_scroll.inertia_angle >=CYCLONE_TP_UP_ACCELERATION)
        {
            g_cyclone_scroll.inertia_angle -= CYCLONE_TP_UP_ACCELERATION;
        }
        else if (g_cyclone_scroll.inertia_angle+CYCLONE_TP_UP_ACCELERATION <0)
        {
            g_cyclone_scroll.inertia_angle += CYCLONE_TP_UP_ACCELERATION;
        }
        else
        {
            g_cyclone_scroll.inertia_angle = 0;
        }
    }

    //SCI_TRACE_LOW:"cyclone_calc_scroll_anim_frame end %d , angle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CYCLONE_1037_112_2_18_2_37_28_36,(uint8*)"dd",SCI_GetTickCount(),g_cyclone_scroll.anim_angle);
}

/**********************************************************************
*   cyclone_calc_func
*
*   算法处理函数
**********************************************************************/
LOCAL void cyclone_calc_func(
                           uint32 calc_type,
                           void* param_ptr, 
                           uint32 param_size
                          )
{
    //SCI_TRACE_LOW:"CYCLONE: crystalcube_calc_func type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CYCLONE_1050_112_2_18_2_37_28_37,(uint8*)"d",calc_type);
    switch (calc_type)
    {
        case MMICYCLONE_MSG_PARAMINIT_REQ:
            {
                cyclone_init_param((MMICYCLONE_Init_PARAM_T*)param_ptr,FALSE);
                break;
            }
        case MMICYCLONE_MSG_PARAM_RESET_REQ:
        	{
                cyclone_init_param((MMICYCLONE_Init_PARAM_T*)param_ptr,TRUE);
        	}
        	break;
        case MMICYCLONE_MSG_ANIM_START_REQ:
            {
                g_cyclone_anim.calc_angle = START_ANGLE;
                cyclone_calc_anim_frame();
            }
            break;
            
         case MMICYCLONE_MSG_ANIM_FRAME_CNF:
            {
                cyclone_calc_anim_frame();
            }    
                break;
        case MMICYCLONE_MSG_HIGHLIGHT_FRAME_REQ:
            {
			cyclone_calc_highlight_frame();
			//SendNotify(MMICYCLONE_MSG_HIGHLIGHT_FRAME_IND);
            }
            break;
        case MMICYCLONE_MSG_ROTATE_REQ:
	        {
			g_cyclone_anim.direction = ((MMICYCLONE_ROTATE_PARAM_T*)param_ptr)->direction;
			g_cyclone_anim.cur_angle = g_cyclone_anim.start_angle = ((MMICYCLONE_ROTATE_PARAM_T*)param_ptr)->start_angle;
			g_cyclone_anim.calc_angle = ((g_cyclone_anim.direction==ROTATE_DIRECTION_RIGHT)?(g_front_angle):(g_front_angle+ANGLE_BETWEEN_ICON));
			g_cyclone_anim.stop_angle = ((MMICYCLONE_ROTATE_PARAM_T*)param_ptr)->stop_angle;
			g_cyclone_anim.total_frame = ((MMICYCLONE_ROTATE_PARAM_T*)param_ptr)->total_frame;
			g_cyclone_anim.cur_frame_offset_angle = 0;
			g_cyclone_anim.cur_frame = 0;
			cyclone_calc_rotate_frame();
	        }
	        break;
         case MMICYCLONE_MSG_ROTATE_FRAME_CNF:
            {
                cyclone_calc_rotate_frame();
            }     
            break;
        case MMICYCLONE_MSG_TP_DOWN_REQ:
		{
			BOOLEAN is_down_in_anim = ((MMICYCLONE_SCROLL_PARAM_T*)param_ptr)->is_down_anim;
			g_cyclone_scroll.tp_state = STATE_SCROLL_READY;
			if (!g_cyclone_param.is_init) break;
			if(!is_down_in_anim)
				{
					backup_icon_res_link();
				}
		}
		break;
        case MMICYCLONE_MSG_TP_SCROOL_REQ:
            {
			BOOLEAN is_first_scroll;
			g_cyclone_scroll.tp_state = STATE_SCROLLING;
			is_first_scroll = (BOOLEAN)(((MMICYCLONE_SCROLL_PARAM_T*)param_ptr)->is_first_scroll);
			g_cyclone_scroll.scroll_angle = (int)(((MMICYCLONE_SCROLL_PARAM_T*)param_ptr)->scroll_angle);
			if(is_first_scroll)
				{
					cyclone_calc_scroll_frame();
				}
            }
            break;
	case MMICYCLONE_MSG_TP_SCROOL_CNF:
		{
			if(g_cyclone_scroll.tp_state == STATE_SCROLLING)
			{
				cyclone_calc_scroll_frame();
			}
		}
		break;
			
        case MMICYCLONE_MSG_SRCOLLANIM_REQ:
	        {
			int abs_catch_angle;
			g_cyclone_scroll.tp_state = STATE_SCROLL_ANIM;
			g_cyclone_scroll.inertia_angle = ((MMICYCLONE_SCROLL_PARAM_T*)param_ptr)->inertia_angle;
			g_cyclone_scroll.catch_angle = ((MMICYCLONE_SCROLL_PARAM_T*)param_ptr)->catch_angle;
			g_cyclone_scroll.catch_stop_angle =((MMICYCLONE_SCROLL_PARAM_T*)param_ptr)->catch_stop_angle;
			g_cyclone_scroll.catch_start_angle = g_cyclone_scroll.catch_stop_angle - g_cyclone_scroll.catch_angle;
			g_cyclone_scroll.anim_angle = ((MMICYCLONE_SCROLL_PARAM_T*)param_ptr)->scroll_anim_start_angle;
			g_cyclone_scroll.cur_catch_frame = 0;
			abs_catch_angle = (abs(g_cyclone_scroll.catch_angle));
			g_cyclone_scroll.total_catch_frame = (abs_catch_angle/CATCH_FRAME_NUM_FACTOR);
			g_cyclone_scroll.cur_select_pos_index = ((MMICYCLONE_SCROLL_PARAM_T*)param_ptr)->cur_select_pos_index;
			g_cyclone_scroll.cur_item_index = ((MMICYCLONE_SCROLL_PARAM_T*)param_ptr)->cur_item_index;
			cyclone_calc_scroll_anim_frame();
	        }
                break;

         case MMICYCLONE_MSG_SRCOLLANIM_CNF:
         	{
			if(g_cyclone_scroll.tp_state == STATE_SCROLL_ANIM)
				{
				    cyclone_calc_scroll_anim_frame();
				}
         	}
         	break;
         case MMICYCLONE_MSG_PARAMRELEASE_REQ:
         	{
                cyclone_deinit_param();
                s_release_is_done = TRUE;
         	}
         	break;
         case MMICYCLONE_MSG_EXIT_REQ:
            {
                g_cyclone_param.is_init = FALSE;
                g_cyclone_scroll.tp_state = STATE_SCROLL_NONE;
			    //cyclone_deinit_param();
			    break;
            }
         default:
            {
                break;
            }
    }
}

/**********************************************************************
*   cyclone_do_operation
*
*   计算接口
**********************************************************************/
LOCAL void cyclone_do_operation(uint32 oper_type,void* param_ptr,uint32 param_size)
{
    MMISE_CALC_PARAM_T calc_param;

    calc_param.calc_type = oper_type;
    calc_param.data_ptr = param_ptr;
    calc_param.data_size = param_size;
    calc_param.calc_func = cyclone_calc_func;
    
    MMISE_StartCalculate(&calc_param);
}
