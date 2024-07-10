/******************************************************************************
 ** File Name:      MMI_SIGNAL.h                                              *
 ** Author:         Richard.Yang                                              *
 ** DATE:           10/11/2004                                                *
 ** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description:    This file defines the basic Application Interface (API)   *
 **                 to the high-performance RTOS.                             *
 **                 All service prototypes for user and some data structure   *
 **                 definitions are defined in this file.                     *
 **                 Basic data type definitions is contained in sci_types.h   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/14/2002     Xueliang.Wang    Create.                                   *
 ** 09/12/2003     Zhemin.Lin       Modify according to CR:MS00004678         *
 ******************************************************************************/
  
/*!
 *  \addtogroup mmi_signal_group_label
 *  @{
 */
 
/*! 
 *  \file 	mmi_signal.h
 *  \author	Richard.Yan
 *  \date	Oct,2004
 *  \brief 	This file defines the basic Application Interface (API)  
 *                 to the high-performance RTOS.                     
 *                 All service prototypes for user and some data structure 
 *                 definitions are defined in this file.                    
 *                 Basic data type definitions is contained in sci_types.h 
 */
 
/*!
 * mmi_signal.h会被mmk_app.c包含, 如果用户修改需要重新编译mmk_app.c, @liqing.peng, 08-06-26
 */


#ifndef _MMI_SIGNAL_H
#define _MMI_SIGNAL_H

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/*---------------------------------------------------------------------------*
 ** SIGNAL MACRO: 
 **         MMI_GET_SIGCODE
 **         MMI_GET_SIGSENDER
 **         MMI_CREATE_SIGNAL
 **         MMI_SEND_SIGNAL
 **         MMI_RECEIVE_SIGNAL
 **         MMI_PEEK_SIGNAL
 **         MMI_FREE_SIGNAL
 **---------------------------------------------------------------------------*/
/******************************************************************************/
/*! \brief Get signal code from the pointer to signal.
 *  \author Xueliang.Wang
 *  \param[in]	_SIG_PTR        Pointer to the signal
 *  \return signal code of the signal.
 */
/******************************************************************************/
#define MMI_GET_SIGCODE(_SIG_PTR) \
        (((xSignalHeader)(_SIG_PTR))->SignalCode)

/******************************************************************************/
/*! \brief Get sender from the pointer to signal.

 *  \author Xueliang.Wang
 *  \param[in]	_SIG_PTR        Pointer to the signal
 *  \return Sender of the signal.
 */
/******************************************************************************/

#define MMI_GET_SIGSENDER(_SIG_PTR) \
        (((xSignalHeader)(_SIG_PTR))->Sender)


/******************************************************************************/
/*! \brief Create a signal.
 *  \author Xueliang.Wang
 *  \param[in]	_SIG_PTR        Pointer to the signal which will be created
 *  \param[in]	_SIG_CODE       Signal code.
 *  \param[in]	_SIG_SIZE       Number of bytes will be alloc for the signal
 *  \param[in]	_SENDER         Sender of this signal.
 *  \return None.
 */
/******************************************************************************/
#define MMI_CREATE_SIGNAL(_SIG_PTR, _SIG_CODE, _SIG_SIZE, _SENDER) \
		(_SIG_PTR) = (void *)SCI_ALLOC_APP(_SIG_SIZE); \
		SCI_ASSERT((_SIG_PTR) != SCI_NULL); \
		SCI_MEMSET((void*)(_SIG_PTR), 0 , _SIG_SIZE); \
		((xSignalHeader)(_SIG_PTR))->SignalSize = _SIG_SIZE; \
		((xSignalHeader)(_SIG_PTR))->SignalCode = _SIG_CODE; \
		((xSignalHeader)(_SIG_PTR))->Sender     = _SENDER;

/******************************************************************************/
/*! \brief Send a signal.
 *  \author Xueliang.Wang
 *  \param[in]	_SIG_PTR        Pointer to the signal which will be sent
 *  \param[in]	_RECEIVER       ID of thread whihc receives this signal.
 *  \return None.
 */
/******************************************************************************/
#define MMI_SEND_SIGNAL(_SIG_PTR, _RECEIVER) \
        SCI_SendSignal((xSignalHeader)(_SIG_PTR), _RECEIVER);

/******************************************************************************/
/*! \brief Receive a signal.
 *  \author Xueliang.Wang
 *  \param[in]	_SIG_PTR        Pointer to a signal to save received signal.
 *  \param[in]	_SELF_ID        ID of thread which receives this signal.
 *  \return None.
 */
/******************************************************************************/
#define MMI_RECEIVE_SIGNAL(_SIG_PTR, _SELF_ID) \
		_SIG_PTR = SCI_GetSignal(_SELF_ID); 

/******************************************************************************/
/*! \brief Gets a signal from a thread immediatelly, if no signal, function returns immediatelly. 
 *  \author Richard.Yang
 *  \param[in]	_SIG_PTR        Pointer to a signal to save received signal.
 *  \param[in]	_SELF_ID        ID of thread which receives this signal.
 *  \return None.
 */ 
/******************************************************************************/
#define MMI_PEEK_SIGNAL(_SIG_PTR, _SELF_ID) \
        _SIG_PTR = SCI_PeekSignal(_SELF_ID)    

/******************************************************************************/
/*! \brief Free a signal.
 *  \author Xueliang.Wang
 *  \param[in]	_SIG_PTR        Pointer to the signal which will be freed.
 *  \return None   
 */
/******************************************************************************/
#define MMI_FREE_SIGNAL(_SIG_PTR) \
        SCI_FREE(_SIG_PTR)

//liqing.peng, 2007-03-29
#define MMI_GROUP    0xE1           //注意一定要和sig_code.h中的group取不同值
                                    //这个group用于,当MMI除了P_APP外还有其他task(如APP_COPY_Task),这些task发送给P_APP消息用这个group


typedef enum
{
    APP_SIG_NONE = MMI_GROUP<<8,
    APP_COPY_END_IND,
	APP_COPY_ONE_FILE_END_IND,
	APP_COPY_DIR_END_IND,
	APP_COPY_ONE_DIR_END_IND,
	APP_DEL_DIR_END_IND,
	APP_CUT_DIR_END_IND,
	APP_FIND_FILES_END_IND,
    APP_DM_MESSAGE_IND,
    APP_OTHER_TASK_TO_APP,
    APP_WASYN_MSG_IND,
	APP_COPY_EXIST_IND,
    APP_TIMER_EXPIRY,
    APP_GSENSOR,
    APP_ATEST_IND,
    APP_MOVE_ONE_FILE_IND,
    APP_MOVE_FILE_EXIST,
    APP_MOVE_END_IND,
}MMI_APP_SELF_SIG_E;


/*----------------------------------------------------------------------------*/
/*                         The signal structure                               */
/*----------------------------------------------------------------------------*/
/*! \brief keypad press signal */
typedef struct MmiKeyPressTag
{
    SIGNAL_VARS
    uint32    					keyCode; 			// modify by louis.wei to keep accord with the Ref
}MmiKeyPress;

/*! \brief TP press signal */
typedef struct MmiTPPressTag
{
    SIGNAL_VARS
    uint32      x;
    uint32      y;
    uint16      gesture_type;
    uint32      tick_count;
}MmiTPPress;

/*! \brief timer expiry signal structure */
typedef struct MmiTimerExpirySTag
{    
    SIGNAL_VARS
    uint8						timerId; 			/*!<if timerId is not the same as originally returned to the user then ignore this signal */
    uint32						ref_num;
   //int16  userValue;
}MmiTimerExpiryS;

typedef struct
{
    SIGNAL_VARS
    uint16          data;
}MmiAppCopyMsgS;

typedef struct
{
    SIGNAL_VARS
    uint32          event_id;
    void            *param;
}MmiAppDMMsgS;

//the callback info
typedef struct
{
    SIGNAL_VARS
    uint32 hAudio;
    uint32 notify_info;
    uint32 affix_info;
    uint32 type;
}MmiAudioCallbackMsgS;


//用于异步ReadFileData, WriteFileData, DeleteFile等函数
typedef struct
{
    SIGNAL_VARS
    uint32       handle;
	uint32       device_type;
    uint32       error;
    uint32       win_id;
    uint32       ctrl_id;
    uint32       msg_id;    
}MMIFILE_ASYN_CALLBACK_T;

//其他task向mmi发送消息
typedef struct
{
    SIGNAL_VARS
    uint32           handle;
    uint32           msg_id;
    void*            param_ptr;
    uint32           size_of_param;
}MmiOtherTaskMsgS;
//  the signal define 
/*! \brief signal handle */
/*************************************************************************************
  使用这个Signal union时需要注意: 
    在发送消息时它是MmiSignalS结构体的最后一个成员, 所以Signal除了会是timerExpiry、
  keyPress、TpPress、MmiAppCopyMsg类型外，也可能是MnssmReadInd、MnSmsMemFullInd、
  MnReadSmsCnf等类型；但是这些类型在下面的定义中被注释掉了，原因是为了MMK能够独立于PS
 	typedef struct MmiSignalSTag
 	{
 		uint16           			signalCode;
 		uint16          	 		signalSize;
 		uint32           			pre;
 		uint32           			succ;
 		uint32           			send;
 		Signal           			sig;
 	}MmiSignalS;
**************************************************************************************/
typedef union 
{
	MmiTimerExpiryS				timerExpiry;
	
	// mmi Signal 
	MmiKeyPress					keyPress;
	
    // touch pannel signal
    MmiTPPress                  TpPress;
	
    MmiAppCopyMsgS              MmiAppCopyMsg;

    MmiAudioCallbackMsgS        audio_info;

    MmiAppDMMsgS                MmiAppDMMsg; 
    //other task message
    MmiOtherTaskMsgS            otherTaskMsg;

	MMIFILE_ASYN_CALLBACK_T     file_callback;
	/*----------------------------------------------------------------------------*/
	/*                mmi singnal list & The signal structure                     */
	/*----------------------------------------------------------------------------*/

// 	/*----------------------------------------------------------------------------*/
// 	/*                         SS MODULE DEFINE                                   */
// 	/*----------------------------------------------------------------------------*/
// 	
// 	/*----------------------------------------------------------------------------*/
// 	/*                         PHONE MODULE DEFINE                                */
// 	/*----------------------------------------------------------------------------*/
// 	/*----------------------------------------------------------------------------*/
// 	/*                         SIM MODULE DEFINE                                  */
// 	/*----------------------------------------------------------------------------*/
// 	/*----------------------------------------------------------------------------*/
// 	/*                         CALL MODULE DEFINE                                 */
// 	/*----------------------------------------------------------------------------*/ 
 	
// 	/*----------------------------------------------------------------------------*/
// 	/*                         CB sigla list                                      */
// 	/*----------------------------------------------------------------------------*/
// 	// add by allen 2004.09.28
// 	MMI_L1_MESSAGE 				L1MnGetDataCnf;	
}Signal;


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // End of _MMI_SIGNAL_H
/*! @} */
