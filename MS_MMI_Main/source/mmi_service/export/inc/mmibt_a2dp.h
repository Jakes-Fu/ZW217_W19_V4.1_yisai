/****************************************************************************
** File Name:      mmibt_a2dp.h                                         *
** Author:         kelly.li                                            *
** Date:           08/02/2005                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the a2dp function     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE				NAME             DESCRIPTION                             *
** August/12/2006    kelly.li     Create                                  *
**                                                                         *
****************************************************************************/

#ifndef _MMIBT_A2DP_H_    
#define  _MMIBT_A2DP_H_    

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/

//#include "mmiset.h"
//#include "mmiset_export.h"
//#include "mmifmm_export.h"   //bt_abs.h中用到ffs.h中的结构
//#ifdef WIN32
//#include "mmibt_win32_simu.h"
//#else
#include "bt_abs.h"
#include "a2dp_api.h"
//#endif
#include "mmibt_export.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//    BT_A2DP_NONE,           //无操作
//	BT_A2DP_PLAY   = 1,		//调用Audio play
//	BT_A2DP_RESUME = 2,		//调用Audio Resume
//	BT_A2DP_STOP   = 4,		//调用Audio stop
// 	BT_A2DP_PAUSE  = 8,		//调用Audio pause
typedef struct
{
	BT_ADDRESS    address;			//record the connected device address
	BOOLEAN		  has_connected_dev;//if there is a connected a2dp device
}MMIA2DP_DEVICE_INFO_T;	

//typedef struct
//{
//	A2DP_AUDIO_ACTION_E action[MMIBT_ACT_QUEUE_MAX_LEN + 1];
//    uint8               action_num;
// }MMIA2DP_ACT_QUEUE_T;		
/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : pause a2dp device of playing mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_A2dpPause( uint32 sample_ret);

/*****************************************************************************/
//  Description : open a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_A2dpOpenDevice(uint32 sample_ret);

/*****************************************************************************/
//  Description : Resume a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN	MMIBT_A2dpResumeDevice( uint32 sample_ret);

/*****************************************************************************/
//  Description : stop a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_A2dpStopDevice( uint32 sample_ret);

/*****************************************************************************/
//  Description : get a2dp address
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BT_ADDRESS MMIBT_GetA2dAddress(BT_MSG_T     *msg_body_ptr);

/*****************************************************************************/
//  Description : if there is active A2DP headset
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN	MMIBT_IsExitedActiveA2dp(void);

/*****************************************************************************/
//  Description : get a2dp link connected indication
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpLinkConnected(BT_MSG_T     *msg_body_ptr);


/*****************************************************************************/
//  Description : get a2dp link disconnected indication
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpLinkDisconnect(BT_MSG_T     *msg_body_ptr);

/*****************************************************************************/
//  Description : a2dp avrcp play music back word
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpAvrcpBackword(void);

/*****************************************************************************/
//  Discription: Get the status of current a2dp
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_GetCurrentA2dpStatus(BT_ADDRESS	*addr_ptr);

/*****************************************************************************/
//  Description : a2dp avrcp play music forward
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpAvrcpForward(void);

/*****************************************************************************/
//  Description : a2dp avrcp  play music adjust volume 
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpAvrcpAdjustVolum(BOOLEAN volume_up);

/*****************************************************************************/
//  Description : headset avrcp of a2dp stop play music
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_AvrcpA2dpPlay(void);

/*****************************************************************************/
//  Description : a2dp stop play music
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_AvrcpA2dpStop(void);

/*****************************************************************************/
//  Description : headset avrcp of a2dp stop pause music
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_AvrcpA2dpPause(void);

/*****************************************************************************/
//  Description : 耳机端发起建立链路的消息
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpStartStreamInd(BT_MSG_T* msg_body );


/*****************************************************************************/
//  Description :耳机端主动发起pause stream的消息
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void		MMIBT_A2dpPauseStreamInd(BT_MSG_T* msg_body );

/*****************************************************************************/
//  Description :耳机端主动发起resume stream ind
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_A2dpResumeStreamInd(BT_MSG_T* msg_body );

/*****************************************************************************/
//  Description : open a2dp headset stereo
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_OpenA2dpHeadset(BT_ADDRESS address);

/*****************************************************************************/
//  Description : disconnect a a2dp device by address
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_CloseA2dpHeadset(void);

/*****************************************************************************/
//  Description : get the sample rate of a2dp stream
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBT_GetA2DPSampleRate(void);

/*****************************************************************************/
//  Description :get a2dp abort indication
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void	MMIBT_A2dpGetAbortInd(void);

/*****************************************************************************/
//  Description : MMIBT Is A2dp Ready For Play
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMIBT_IsA2dpReadyForPlay(void);

/*****************************************************************************/
//  Description : MMIBT Is A2dp Ready For Play
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMIBT_IsA2dpPaused(void);

/*****************************************************************************/
//  Description:    Get A2DP actived device information
//  Author:         
//  Note:
//  Parameter:      has_active_device: used to tell app that whether there is a 
//					actived device
//					TRUE:  has a actived a2dp device
//					FALSE: no actived device
//  Returns:        return the address of actived device, if has_active_device
//					is FALSE, app should ignore this return parameter. 
//****************************************************************************/
PUBLIC BT_ADDRESS MMIBT_GetActiveA2DPDevice(BOOLEAN* has_active_device);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef   __cplusplus
    }
#endif

#endif
