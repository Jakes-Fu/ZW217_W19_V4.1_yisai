/******************************************************************************
** File Name:      mmi_osbridge.h                                             *
** Author:                                                                    *
** Date:           13/05/2004                                                 *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.          *
** Description:    This file is used to describe rtos and mmi interface       *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 13/05/2004       jibin          Create									  *
******************************************************************************/

/*!
 *  \addtogroup mmk_osbridge_group_label
 *  @{
 */


/*! 
 *  \file 	mmi_osbridge.h
 *  \author	bin.ji
 *  \date	13/05/2004
 *  \brief 	This file is used to describe rtos and mmi interface
 */

/*!
 * mmi_osbridge.h会被mmk_app.c包含, 如果用户修改需要重新编译mmk_app.c, @liqing.peng, 08-06-26
 */


#ifndef MMI_BRIDGE_H
#define MMI_BRIDGE_H

#include "sci_types.h"
#include "user_tasks_id.h"
#include "mmi_signal.h"


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern   "C"
    {
#endif

#ifdef MMI_BRIDGE_C
#define EXT 
#else
#define EXT 					extern				/*!< function retun paramater,is extern or null */
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/* temp solution */
#define TASK_FL_ID  				P_APP			/*!< task id defenition */
#define TASK_DISH_ID 				P_LCD
#define DISH_QUEUE_ID 				TASK_DISH_ID

#define MILLISECONDS_TO_TICKS(_MS)   _MS
#define DevAssert   				SCI_ASSERT
#define DevFail     				SCI_ASSERT


/*! \brief mmi signal struct */
typedef struct MmiSignalSTag
{
    SIGNAL_VARS
    void *sig;				/*!< signal data */
}MmiSignalS;


/******************************************************************************/
/*! \brief mmi alloc memory
 *  \param[in]		numberOfBytesRequired	byte size to alloc
 *  \param[out]		**buffer				memory pointer allocated
 */
/******************************************************************************/
EXT void MmiAllocMemory(  
								uint16               numberOfBytesRequired,
                              	void                **buffer
                       );


/******************************************************************************/
/*! \brief mmi free memory
 *  \param[in]		**buffer				memory pointer to be freed
 */
/******************************************************************************/
EXT void MmiFreeMemory(void **buffer);


/******************************************************************************/
/*! \brief mmi receive a signal from a queue
 *  \param[in]		queueId					queue id
 *  \param[in]		**signal				signal pointer
 */
/******************************************************************************/
EXT void MmiReceiveSignal(
								uint16 				queueId, 
								MmiSignalS 			**signal
						);



/******************************************************************************/
/*! \brief mmi create a signal
 *  \param[in]		signalType		signal type
 *  \param[in]		signalSize		signal size
 *  \param[in]		**signal		signal pointer
 */
/******************************************************************************/
EXT void MmiCreateSignal (
								uint16     			signalType,
                       			int16      			signalSize,
                       			MmiSignalS 			**signal
                       	);



/******************************************************************************/
/*! \brief mmi destroy a signal
 *  \param[in]		**signal		signal pointer
 */
/******************************************************************************/
EXT void MmiDestroySignal(MmiSignalS **signal);


/******************************************************************************/
/*! \brief  mmi send a signal to a task
 *  \param[in]		destTask	task id
 *  \param[in]		*signal		signal pointer
 */
/******************************************************************************/
EXT void MmiSendSignal (
								uint16 				destTask, 
								MmiSignalS 			*signal
						);
#undef EXT

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus   
    }
#endif

#endif
/*! @} */ 

