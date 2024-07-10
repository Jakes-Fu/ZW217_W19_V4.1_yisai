/****************************************************************************************
** File Name:		env_logsave_signal.h										    	*	
** Author:			William Qian														*
** DATE:			02/10/2010															*
** CopyRight		2010 SpreadTrum Communications CORP. All Rights Reserved.			*
** Description:		this header file define logsave received signals					*
*****************************************************************************************


*****************************************************************************************
**					Edit History														*	
** -------------------------------------------------------------------------------------*
**																						*
** DATE:			NAME				DESCIPTION										*
** 02/09/2010		William Qian		Create.											*
*****************************************************************************************/

#ifndef _ENV_LOGSAVE_SIGNAL_H
#define _ENV_LOGSAVE_SIGNAL_H

#define ENV_LOGSAVE_SIGNAL_LIST	\
        LOGSAVE_SET_READY   = ((P_LOGSAVE << 8) | 1),  \
        LOGSAVE_SET_NOT_READY,	\
        LOGSAVE_ARM_TRACE_IND,  \
        LOGSAVE_ARM_MESSAGE_IND,    \
        LOGSAVE_DSP_LOG_IND,    \
        LOGSAVE_CP_LOG_IND,    \
        LOGSAVE_WRITE_FILE_REQ, \
        LOGSAVE_WRITE_FILE_POS_REQ, \
        LOGSAVE_SAVE_ALL_BUFFER_REQ,\
		END_LOGSAVE_SIGNAL, 
		
#endif  //end _ENV_LOGSAVE_SIGNAL_H
