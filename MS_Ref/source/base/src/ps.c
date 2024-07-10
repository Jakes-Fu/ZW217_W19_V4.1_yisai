/******************************************************************************
 ** File Name:      ps.c                                                      *
 ** Author:         Richard Yang                                              *
 ** DATE:           20/11/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 ps thread.                                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 20/11/2001     Richard.Yang     Create.                                   *
 ******************************************************************************/

#ifdef _FEATURE_BSSIM_TEST
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "es_api.h"
#include "tasks_id.h"
#include "cmddef.h"
#include "cm.h"
#include "scttypes.h"
#include "ps.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void thread_ps_entry()                                                * 
 **                                                                           *	 
 ** DESCRIPTION                                                               *
 **     Main thread of protocol task. It manages the messages from cm and     *
 **     protocol stack programme.                                             *
 **                                                                           *
 ** INPUT                                                                     *
 **     uint32 argc :                                                           *
 **     void *argv:                                                           *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     None                                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *	 
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **                                                                           *	 
 **---------------------------------------------------------------------------*/
void thread_ps_entry(uint32 argc, void *argv)
{
	uint32     status = 0;
    uint8     *msg;       
	BLOCK_ID src_ID;
	BOOLEAN    bPSReady=FALSE;

	while (1)
	{
		/* Wait for the messages in the queue_ps, pass if there is one message in the queue.*/
        status = es_queue_receive(PS, (void **) &msg, &src_ID, ES_WAIT_FOREVER);  
        if (status != ES_SUCCESS)
            continue;
		if (src_ID == WATCHDOG)
		{
			bPSReady = TRUE;
			free_sig((xSignalHeaderRec *)msg);
			continue;
		}
		/* Msg from channel manager. Forward the message to correspond thread.*/
		if (src_ID == CM)
		{
			xSignalHeader send_signal;

			/* Get length of the signal.*/
			uint16 length = ((msg_head_s *) msg)->len - 2;

			/* if stack ok? */
			if (!bPSReady)
			{
				free_sig((xSignalHeaderRec *)msg);
				continue;
			}
            
			/* Make sure that length is less than 255!*/
            ASSERT(length <= 0x0FF);/*assert verified*/
			/* Malloc buffer for signal.*/
//modified 12/10/2001
//			send_signal = alloc_sig(PARTITION_POOL(length));
            send_signal = alloc_sig(length);
//modify end
			/* Copy signal to the buffer.*/
			memcpy(send_signal, msg + sizeof(msg_head_s), length);

			/* Free message.*/
			es_free(msg);

			/* Send message to proper task.*/
			send_sig_no_to(send_signal, send_signal->SignalCode);
		}
        /* Msg from protocol stack. Forward the message to Channel manager.*/
		else
		{
			switch(src_ID)
			{
			/* From Layer 2.*/
			case P_DLR:
				{
					uint8 *send_msg;
					uint16 length = ((xSignalHeader) msg)->SignalSize + sizeof(msg_head_s);

					send_msg = es_malloc(length);
					memcpy(send_msg + sizeof(msg_head_s), msg, length - sizeof(msg_head_s));
                    
					CM_COPY_MSG_HEAD(send_msg, 0, length, PS_L2_A, 0);

					free_sig((xSignalHeader) msg);

	        		/* Send message to channel manager.*/
						        		/* Send message to channel manager.*/
					sio_send_packet((uint8 *) send_msg, length);	
					es_free(send_msg); 
				//	es_queue_send(PS, CM, (void *) send_msg, 
				//		          length, ES_PRI_NORMAL, ES_NO_WAIT);
				}
				break;
			/* From Layer 2.*/
         #ifndef _MSSIM
			case P_no_receiver:
         #else
 			case P_NETWORK:	 
         #endif
				{
					uint8 *send_msg;
					uint16 length = ((xSignalHeader) msg)->SignalSize + sizeof(msg_head_s);

					send_msg = es_malloc(length);
					memcpy(send_msg + sizeof(msg_head_s), msg, length - sizeof(msg_head_s));
//					memcpy(send_msg + sizeof(msg_head_s), msg, length);
                    
					CM_COPY_MSG_HEAD(send_msg, 0, length, PS_L2_A, 0);

					free_sig((xSignalHeader) msg);

	        		/* Send message to channel manager.*/
					sio_send_packet((uint8 *) send_msg, length);	        		
					es_free(send_msg);    
					//es_queue_send(PS, CM, (void *) send_msg, 
					//	          length, ES_PRI_NORMAL, ES_NO_WAIT);
				}
				break;
			default:
				/* Free singal.*/
				free_sig((xSignalHeaderRec *)msg);
				break;
			}
		}
	}
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif /* End _FEATURE_BSSIM_TEST*/
