/*****************************************************************************
 ** File Name:      tp_gesture_drv.c                                         *
 ** Date:           2012/02/02                                               *
 ** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
 ** Description:     This file defines the function about tp gesture         *
 **                                                                          * 
 *****************************************************************************

******************************************************************************
**                               Edit History                                *
** ------------------------------------------------------------------------- *
**     DATE          NAME             DESCRIPTION                            *
**  2012.02.02                         Create.                               *
******************************************************************************/

/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/
#include "tp_gesture_drv.h"
#include "tp_api.h"
#include "tasks_id.h" 
#include "threadx_os.h"
#include <math.h>

//#define TP_GESTURE_DBG
#ifdef TP_GESTURE_DBG
#define TP_GESTURE_LOG  SCI_TRACE_LOW
#else
#ifdef WIN32
	#define TP_GESTURE_LOG
#else
	#define TP_GESTURE_LOG(...)
#endif	
#endif
#define GESTURE_CALI

/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif
/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/
#define TPC_GESTURE_ROTATE_INTV     0x48 
#define TP_SPEED_ZOOM_FAST          0x19//0x20
#define TP_SPEED_ZOOM_NORMAL        0x30
#define TP_SPEED_ZOOM_SLOW          0x40

/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/
typedef struct TP_GESTURE_ZOOM_TAG
{
    int32  minus_interval;
    uint32 interval;
    int32 delta_hori[2];
    int32 delta_vert[2];
    uint32 pythago_squval[2];
}TP_GESTURE_ZOOM_T;

typedef struct TP_GESTURE_TAG
{
    uint16 num_of_point;
    uint16 gesture_type;
    TP_GESTURE_ZOOM_T zoom_ges;
    BOOLEAN enable_zoom;
    BOOLEAN enable_rotate;
}TP_GESTURE_T;

/**---------------------------------------------------------------------------*
 **                        Global/Local Variables                             *
 **---------------------------------------------------------------------------*/
LOCAL TP_GESTURE_T tp_ges;
LOCAL TPDSVR_SIG_T sig_sigle_save;
LOCAL TPDSVR_SIG_T sig_send_msg;
LOCAL XY_DATA_T position[2];
LOCAL uint16 ges_rotate_mark;//fingernum, gesturetype,orbit direction, steps
LOCAL uint32 pythago_val[2];
LOCAL BOOLEAN move_msg = SCI_TRUE;
/**---------------------------------------------------------------------------*
 **                      Global/Local Function Prototypes                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                        Function Definitions                               *
 **---------------------------------------------------------------------------*/
 /*lint -save -e18*/
/*****************************************************************************/
//  FUNCTION:     TP_Init_Coordinate
//  Description:  Reset data after pen up or gesture verified
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN TP_Init_Coordinate(TPDSVR_SIG_T *sig)
{
    if( tp_ges.num_of_point == 1)
    {
        position[0].x_position = sig->x_key;
        position[0].y_position = sig->y_key;
        position[0].finger_id = sig->cur_index;
    }
    else
    {
        position[0].x_position = sig->x_key;
        position[0].y_position = sig->y_key;
        position[0].finger_id = sig->cur_index;
        position[1].x_position = sig->xy_position[0].x_position;
        position[1].y_position = sig->xy_position[0].y_position;
        position[1].finger_id = sig->xy_position[0].finger_id;

        tp_ges.zoom_ges.delta_hori[0] = position[0].x_position - position[1].x_position;
        tp_ges.zoom_ges.delta_vert[0] = position[0].y_position - position[1].y_position;
    }
    return SCI_TRUE;
}

/*****************************************************************************/
//  FUNCTION:     TP_Zoom
//  Description:  How to veriy zoom
//	Note:
/*****************************************************************************/
    
LOCAL BOOLEAN TP_Zoom(TPDSVR_SIG_T *sig)
{
    //zoom in/out
    tp_ges.zoom_ges.pythago_squval[1] = tp_ges.zoom_ges.delta_hori[1]*tp_ges.zoom_ges.delta_hori[1] + tp_ges.zoom_ges.delta_vert[1]*tp_ges.zoom_ges.delta_vert[1];
    pythago_val[1] = (uint32)(sqrt((double)tp_ges.zoom_ges.pythago_squval[1]));
    TP_GESTURE_LOG("INTERVAL after %d, before%d\n", (uint32)pythago_val[1] , (uint32)pythago_val[0]);
    tp_ges.zoom_ges.minus_interval = (int32)((uint32)pythago_val[1] - (uint32)pythago_val[0]);
    if((uint32)(abs(tp_ges.zoom_ges.minus_interval) )> tp_ges.zoom_ges.interval)
    {
        if(tp_ges.zoom_ges.minus_interval > 0)
        {
            sig->gesture_type = tp_ges.gesture_type = TG_ZOOM_IN;
            sig->SignalCode = TP_GESTURE_MSG;
            TP_GESTURE_LOG("ZOOM IN\n");
            move_msg = SCI_FALSE;
        }
        else
        {
            sig->gesture_type = tp_ges.gesture_type = TG_ZOOM_OUT;//zoom small
            sig->SignalCode = TP_GESTURE_MSG;
            TP_GESTURE_LOG("ZOOM OUT\n");
            move_msg = SCI_FALSE;
        }
    } 
    return SCI_TRUE;
}

/*****************************************************************************/
//  FUNCTION:     TP_Gesture_Enable
//  Description:  How to veriy zoom
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN TP_Gesture_Enable(void)
{
    BOOLEAN gest_enable = SCI_FALSE;
    if(tp_ges.enable_rotate)
    {
        return SCI_TRUE;
    }
    if(tp_ges.enable_zoom)
    {
        return SCI_TRUE;
    }

    return gest_enable;
    
}
/*****************************************************************************/
//  FUNCTION:     TP_Single_Rotate
//  Description:  How to veriy single rotate
//	Note:
/*****************************************************************************/

LOCAL BOOLEAN TP_Single_Rotate(TPDSVR_SIG_T *sig)
{
    //rotate start
    //rotate first step
    tp_ges.zoom_ges.delta_hori[0] = sig->x_key - position[0].x_position;
    //TP_GESTURE_LOG("gesture_type result_xy = %d\n",delta_hori[0]);
    if((tp_ges.zoom_ges.delta_hori[0] > TPC_GESTURE_ROTATE_INTV) && (ges_rotate_mark == 0))
    {
        position[0].x_position = sig->x_key;
        ges_rotate_mark = 0x1101;
    }
    else if((tp_ges.zoom_ges.delta_hori[0] < -TPC_GESTURE_ROTATE_INTV) && (ges_rotate_mark == 0))
    {
        //TP_GESTURE_LOG("gesture_type result_xy < -0x50\n");
        position[0].x_position = sig->x_key;
        ges_rotate_mark = 0x1111;
    }

    //rotate second step
    tp_ges.zoom_ges.delta_hori[0] = sig->x_key - position[0].x_position;
    if((tp_ges.zoom_ges.delta_hori[0]  < -TPC_GESTURE_ROTATE_INTV) && (ges_rotate_mark == 0x1101))
    {
        //rotate third step
        if(sig->y_key > position[0].y_position)
        {
            TP_GESTURE_LOG("gesture_type: TG_ROTATE_RIGHT\n");
            sig->gesture_type = TG_ROTATE_RIGHT;
            tp_ges.gesture_type = sig->gesture_type;
            sig->SignalCode = TP_GESTURE_MSG;
            ges_rotate_mark = 0x1109;
        }
        else if( position[0].y_position > sig->y_key )
        {
            TP_GESTURE_LOG("gesture_type: TG_ROTATE_LEFT\n");
            sig->gesture_type = TG_ROTATE_LEFT;  
            tp_ges.gesture_type = sig->gesture_type;
            sig->SignalCode = TP_GESTURE_MSG;
            ges_rotate_mark = 0x1109;
        }
    }
    else if((tp_ges.zoom_ges.delta_hori[0] > TPC_GESTURE_ROTATE_INTV) && (ges_rotate_mark == 0x1111))
    {
        //rotate third step
        if(sig->y_key > position[0].y_position)//? CHECK IT SHOULD BE RIGHT
        {
            TP_GESTURE_LOG("gesture_type: TG_ROTATE_RIGHT 1\n");
            sig->gesture_type = TG_ROTATE_RIGHT;
            tp_ges.gesture_type = sig->gesture_type;
            sig->SignalCode = TP_GESTURE_MSG;
            ges_rotate_mark = 0x1119;
        }
        else if( position[0].y_position > sig->y_key )
        {
            TP_GESTURE_LOG("gesture_type: TG_ROTATE_LEFT 1 \n");
            sig->gesture_type = TG_ROTATE_LEFT;  
            tp_ges.gesture_type = sig->gesture_type;
            sig->SignalCode = TP_GESTURE_MSG;
            ges_rotate_mark = 0x1119;
        }   
    }
    //rotate end 
    return SCI_TRUE;
}


/*****************************************************************************/
//  FUNCTION:     TP_Double_Rotate
//  Description:  How to veriy double rotate
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN TP_Double_Rotate(TPDSVR_SIG_T *sig)
{
    int32 rotat_denomi;
    int32 rotat_mole;
    //rotate
	//tan(a-b) = [tan(a)-tan(b)]/[1+tan(a)*tan(b)]
	//tan(a-b) = (y/x-y1/x1)/(1+ (y*y1)/(x*x1)) = (x1y-xy1)/(y1y+xx1)
	//x1 = delta_hori[1], x = delta_hori[0], y= delta_vert[0], y1 = delta_vert[1]
	rotat_denomi = tp_ges.zoom_ges.delta_vert[1]*tp_ges.zoom_ges.delta_vert[0]+tp_ges.zoom_ges.delta_hori[0]*tp_ges.zoom_ges.delta_hori[1];
	if(rotat_denomi != 0)
	{
	    rotat_mole = tp_ges.zoom_ges.delta_hori[1]*tp_ges.zoom_ges.delta_vert[0] - tp_ges.zoom_ges.delta_hori[0]*tp_ges.zoom_ges.delta_vert[1];
		if((rotat_mole / rotat_denomi) >= 1)
		{
            TP_GESTURE_LOG("gesture_type:2 TG_ROTATE_RIGHT \n");
	    	sig->gesture_type = tp_ges.gesture_type = TG_ROTATE_RIGHT;
	    	sig->SignalCode = TP_GESTURE_MSG;
		}
		if((rotat_mole / rotat_denomi) <= -1)
		{
            TP_GESTURE_LOG("gesture_type:2 TG_ROTATE_LEFT \n");
			sig->gesture_type = tp_ges.gesture_type = TG_ROTATE_LEFT;
			sig->SignalCode = TP_GESTURE_MSG;
		}
	}
	else
	{
		if(tp_ges.zoom_ges.delta_hori[0]== 0 && tp_ges.zoom_ges.delta_vert[1] == 0 && tp_ges.zoom_ges.delta_hori[1] < 0)
		{
			if(tp_ges.zoom_ges.delta_vert[0] <0)
			{
                TP_GESTURE_LOG("gesture_type:1 TG_ROTATE_LEFT \n");
				sig->gesture_type = tp_ges.gesture_type = TG_ROTATE_LEFT;
				sig->SignalCode = TP_GESTURE_MSG;
			}
			else
			{
                TP_GESTURE_LOG("gesture_type:3 TG_ROTATE_RIGHT \n");
				sig->gesture_type = tp_ges.gesture_type = TG_ROTATE_RIGHT;
				sig->SignalCode = TP_GESTURE_MSG;
			}
		}
		else if(tp_ges.zoom_ges.delta_hori[0]== 0 && tp_ges.zoom_ges.delta_vert[1] == 0 && tp_ges.zoom_ges.delta_hori[1] >0)
		{
			if(tp_ges.zoom_ges.delta_vert[0] <0)
			{
                TP_GESTURE_LOG("gesture_type:4 TG_ROTATE_RIGHT \n");
				sig->gesture_type = tp_ges.gesture_type = TG_ROTATE_RIGHT;
				sig->SignalCode = TP_GESTURE_MSG;
			}
			else
			{
                TP_GESTURE_LOG("gesture_type:5 TG_ROTATE_LEFT \n");
				sig->gesture_type = tp_ges.gesture_type = TG_ROTATE_LEFT;
				sig->SignalCode = TP_GESTURE_MSG;
			}
		}
						
		if(tp_ges.zoom_ges.delta_hori[1]== 0 && tp_ges.zoom_ges.delta_vert[0] == 0 && tp_ges.zoom_ges.delta_hori[0] < 0)
		{
			if(tp_ges.zoom_ges.delta_vert[1] <0)
			{
                TP_GESTURE_LOG("gesture_type:6 TG_ROTATE_RIGHT \n");
		        sig->gesture_type = tp_ges.gesture_type = TG_ROTATE_RIGHT;
		        sig->SignalCode = TP_GESTURE_MSG;
			}
			else
			{
                TP_GESTURE_LOG("gesture_type:7 TG_ROTATE_LEFT \n");
			    sig->gesture_type = tp_ges.gesture_type = TG_ROTATE_LEFT;
			    sig->SignalCode = TP_GESTURE_MSG;
			}
		}
		else if(tp_ges.zoom_ges.delta_hori[1]== 0 && tp_ges.zoom_ges.delta_vert[0] == 0 && tp_ges.zoom_ges.delta_hori[0] > 0)
		{
			if(tp_ges.zoom_ges.delta_vert[1] <0)
			{
                TP_GESTURE_LOG("gesture_type:8 TG_ROTATE_LEFT \n");
				sig->gesture_type = tp_ges.gesture_type = TG_ROTATE_LEFT;
				sig->SignalCode = TP_GESTURE_MSG;
			}
			else
			{
                TP_GESTURE_LOG("gesture_type:9 TG_ROTATE_RIGHT \n");
				sig->gesture_type = tp_ges.gesture_type = TG_ROTATE_RIGHT;
				sig->SignalCode = TP_GESTURE_MSG;
			}
		}
	}
	return SCI_TRUE;
}

/*****************************************************************************/
//  FUNCTION:     TP_SetGesture_Reset
//  Description:  Reset after pen down
//	Note:
/*****************************************************************************/
PUBLIC void TP_SetGesture_Reset(TPDSVR_SIG_T *sig)
{
    TP_GESTURE_LOG("gesture:DOWN num_of_point %d, enable_rotate %d, enable_zoom %d\n",sig->num_of_point, tp_ges.enable_rotate, tp_ges.enable_zoom);
    sig->gesture_type = tp_ges.gesture_type = TG_NO_DETECT;
    if(!(TP_Gesture_Enable()))
        return;
    tp_ges.num_of_point = sig->num_of_point;
    move_msg = SCI_TRUE;

    SCI_MEMCPY(&sig_sigle_save, sig , sizeof(TPDSVR_SIG_T));
    SCI_MEMCPY(&sig_send_msg, sig , sizeof(TPDSVR_SIG_T));
}


/*****************************************************************************/
//  FUNCTION:     TP_Gesture_Read
//  Description:  the function is verifying gesture
//	Note:
/*****************************************************************************/

PUBLIC void TP_Gesture_Read(TPDSVR_SIG_T *sig)
{
    TP_GESTURE_LOG("gesture:MOVE num_of_point %d, enable_rotate %d, enable_zoom %d,move_msg %d\n",sig->num_of_point, tp_ges.enable_rotate, tp_ges.enable_zoom,move_msg);

    SCI_MEMCPY(&sig_send_msg, sig , sizeof(TPDSVR_SIG_T));
    
    if(!TP_Gesture_Enable())
        return;
        
    if( TP_UP_MSG == sig->SignalCode )
    {
        tp_ges.num_of_point = 0;
        ges_rotate_mark = 0;
        SCI_MEMSET(position, 0 , 2*sizeof(XY_DATA_T));
        return;
    }
     
    if( tp_ges.num_of_point != sig->num_of_point || tp_ges.gesture_type != TG_NO_DETECT )
    {
        ges_rotate_mark = 0;//for rotate gesture
        if(sig->num_of_point == 2)
        {
            move_msg = SCI_FALSE;
        }
        
        tp_ges.gesture_type = sig->gesture_type = TG_NO_DETECT;
        tp_ges.num_of_point = sig->num_of_point; 
        TP_Init_Coordinate(sig);
        
        if((tp_ges.num_of_point == 2) && (tp_ges.enable_zoom))
        {
            //zoom in/out
            tp_ges.zoom_ges.pythago_squval[0] = tp_ges.zoom_ges.delta_hori[0]*tp_ges.zoom_ges.delta_hori[0] + tp_ges.zoom_ges.delta_vert[0]*tp_ges.zoom_ges.delta_vert[0];
            pythago_val[0] = (uint32)(sqrt((double)tp_ges.zoom_ges.pythago_squval[0]));
            TP_GESTURE_LOG("ZOOM IN/OUT %d",pythago_val[0]);
        }
        if(!move_msg)
        {
            sig->x_key = sig_sigle_save.x_key;
            sig->y_key = sig_sigle_save.y_key;
            if(sig->num_of_point == 2)
            {
                sig->xy_position[0].x_position = 0;
                sig->xy_position[0].y_position = 0;
            }
        }
        return;
    }
    if(tp_ges.num_of_point == 1)
    {
        if(!move_msg)
        {
            sig->x_key = sig_sigle_save.x_key;
            sig->y_key = sig_sigle_save.y_key;
        }
        else
        {
            sig_sigle_save.x_key = sig->x_key;
            sig_sigle_save.y_key = sig->y_key;
        }
    }
    
    if( sig->num_of_point == 1)
    {
        if(tp_ges.enable_rotate)
        {
            TP_Single_Rotate(sig);
        }
        if(!move_msg)
        {
            sig->x_key = sig_sigle_save.x_key;
            sig->y_key = sig_sigle_save.x_key;
        }
    }
    else if(sig->num_of_point == 2)
    {
        
        tp_ges.zoom_ges.delta_hori[1] = sig->x_key - sig->xy_position[0].x_position;
        tp_ges.zoom_ges.delta_vert[1] = sig->y_key - sig->xy_position[0].y_position;

        if(tp_ges.enable_rotate)
		{
		    TP_Double_Rotate(sig);
		}

        if(tp_ges.enable_zoom)
        {
            TP_Zoom(sig);
        }
        if(sig_sigle_save.x_key != 0)
        {
            sig->x_key = sig_sigle_save.x_key;
            sig->y_key = sig_sigle_save.y_key;
            sig->xy_position[0].x_position = 0;
            sig->xy_position[0].y_position = 0;
        }
    }  
}

/*****************************************************************************/
//  Function name:	TP_Gesture_Init
//  Description:
//  Global resource dependence: 
//  Note:
/*****************************************************************************/
PUBLIC void TP_Gesture_Init(void)
{   
    SCI_MEMSET(&tp_ges, 0, sizeof(TP_GESTURE_T));
    tp_ges.enable_rotate = SCI_TRUE;
    tp_ges.enable_zoom = SCI_TRUE;
    tp_ges.zoom_ges.interval = TP_SPEED_ZOOM_FAST;
    SCI_MEMSET(position, 0 , 2*sizeof(XY_DATA_T));
}

/*****************************************************************************/
//  Function name:	TP_Gesture_Uninit
//  Description:
//  Global resource dependence: 
//  Note:
/*****************************************************************************/
PUBLIC void TP_Gesture_Uninit(void)
{
}

/*****************************************************************************/
//  Function name:	TP_Ioctl
//  Description:
//  Global resource dependence: 
//  Note:
/*****************************************************************************/
PUBLIC void TP_Ioctl(uint32 cmd, void *arg)
{
    TP_SPEED_MODE_E speed_mode = TP_SPEED_MODE_FAST;

    switch(cmd)
    {
        case TP_IOCTL_CMD_SPEED_MODE:
            if(arg != PNULL)
                speed_mode = *(TP_SPEED_MODE_E *)arg;

            if(speed_mode == TP_SPEED_MODE_FAST)
            {
                tp_ges.zoom_ges.interval = TP_SPEED_ZOOM_FAST;
            }
            else if(speed_mode == TP_SPEED_MODE_NORMAL)
            {
                tp_ges.zoom_ges.interval = TP_SPEED_ZOOM_NORMAL;
            }
            else if(speed_mode == TP_SPEED_MODE_SLOW)
            {
                tp_ges.zoom_ges.interval = TP_SPEED_ZOOM_SLOW;
            }
        break;

        case TP_IOCTL_GET_THRES_ZOOM:
            if(arg != PNULL)
            {
                *((uint16 *)arg) = tp_ges.zoom_ges.interval;
            }
            break;
        
        case TP_IOCTL_SET_THRES_ZOOM:
            if(arg != PNULL)
            {
                if(*((uint16 *)arg) < TP_SPEED_ZOOM_FAST )
                {
                    tp_ges.zoom_ges.interval = TP_SPEED_ZOOM_FAST;
                }
                else
                {
                    tp_ges.zoom_ges.interval = *((uint16 *)arg);
                }
            }
            TP_GESTURE_LOG("zoom_interval %d",tp_ges.zoom_ges.interval);
        break;
        
        case TP_IOCTL_GET_INT_TIME:
        break;
        case TP_IOCTL_ENABLE_ZOOM:
            tp_ges.enable_zoom = SCI_TRUE;
        break;
        
        case TP_IOCTL_DISABLE_ZOOM:
            tp_ges.enable_zoom = SCI_FALSE;
        break;
        
        case TP_IOCTL_ENABLE_ROTATE:
            tp_ges.enable_rotate = SCI_TRUE;
        break;
        
        case TP_IOCTL_DISABLE_ROTATE:
            tp_ges.enable_rotate = SCI_FALSE;
        break;
       default:
        break;
    }
}

PUBLIC uint32 TP_Read(TPDSVR_SIG_T *data)
{
    if(data != PNULL)
    {
        //SCI_MEMSET(data, 0, sizeof(TPDSVR_SIG_T));
		SCI_MEMCPY(data, &sig_send_msg, sizeof(TPDSVR_SIG_T));
	}
	else
	{
		return SCI_ERROR;
	}	
    return SCI_SUCCESS;
}
/*lint -restore*/
#ifdef __cplusplus
}
#endif

