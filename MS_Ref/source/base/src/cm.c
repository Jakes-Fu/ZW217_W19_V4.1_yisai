/******************************************************************************
 ** File Name:      cm.c                                                      *
 ** Author:         Richard Yang                                              *
 ** DATE:           22/08/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 channel manager thread. It codes messages from other      *
 **					threads and sends them to the serrial device, on the      *
 **					other hand, it decodes messages from serrial ports and    *
 **					send them to the corresponding threads.                   * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 22/08/2001     Richard.Yang     Create.                                   *
 ** 29/07/2002     Richard.Yang     Modified for the new communication        *
 **                                 protocol.                                 *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_ref_base_trc.h"
#include "os_api.h"
#include "threadx_os.h"
#include "os_apiext.h"
#include "tb_hal.h"
#include "tasks_id.h"
#include "cmddef.h"
#include "cm.h"
#include "ref_param.h"
#ifdef WIN32
    #include "bridgelayer.h"
#else
    #include "arm_reg.h" 
    #include "sio.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/
LOCAL void   SendResponseToChannelServer(uint16 seq_num);
LOCAL void   SendPSMessage(xSignalHeader sig_ptr);
LOCAL uint32 SendMsgToTasks(xSignalHeader sig_ptr,uint32 size);
LOCAL uint32 CM_get_queue_avilable(BLOCK_ID  thread_id);	
#ifndef WIN32
extern void   SIO_SaveToPPPBuf(char * data, uint32 len);
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structure Definition                        *
 **---------------------------------------------------------------------------*/
 #define SME_CM_BUF_SIZE                 61440
typedef struct {
	uint32 u32read_offset;
	uint32 u32wrtite_offset;
	uint8  sme_cm_buf[SME_CM_BUF_SIZE];
}T_SMEBUF;

#define GET_SEMBUF_FREE_SIZE(data_size, _buf_size,_buf)  \
do  \
{   \
    if (_buf->u32read_offset > _buf->u32wrtite_offset) \
    {\
        data_size =  (_buf->u32read_offset - _buf->u32wrtite_offset);\
    }\
    else\
    {\
        data_size =  ((int)(_buf_size)+ _buf->u32read_offset - _buf->u32wrtite_offset);\
    }\
} while(0);

/**---------------------------------------------------------------------------*
 **                      Local Variables  Definition                          *
 **---------------------------------------------------------------------------*/
#ifdef PRODUCT_DM
LOCAL T_SMEBUF s_cmsme_buf = {0};
#endif

LOCAL SCI_EVENT_GROUP_PTR  cm_sme_event = NULL;   

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     THREAD_ENTRY(CM)                                                * 
 **                                                                           *	 
 ** DESCRIPTION                                                               *
 **     Main thread of channel manager. It manages the messages of both       *
 **     Uplink messages and Downlink messages.                                *
 **                                                                           *
 ** INPUT                                                                     *
 **     uint32 argc :                                                         *
 **     void *argv:                                                           *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     None                                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *	 
 ** DEPENDENCIES                                                              *
 **     uint32 SendMsgToTasks(const void *msg, uint32 size);                  *
 **                                                                           *	 
 **---------------------------------------------------------------------------*/
THREAD_ENTRY(CM)/*lint -esym(765,thread_entry_CM)*/
{
      uint32  status			= 0xffffffff;          
      uint32	len				= 0;      // Message size
      xSignalHeader	sig_ptr	= SCI_NULL;
      uint8   buffer[8];
      
      
      MSG_HEAD_T        *msg_head_ptr = SCI_NULL;/*lint -esym(529,msg_head_ptr)*/
#if defined(PRODUCT_DM) 
      T_SMEBUF             *p_smebuf =&s_cmsme_buf; 
#endif
      
#ifndef WIN32
      
      uint32                   buf_max_size = 0;	
      uint32                   buf_count = 0;
      uint32                   packet_size=0;
      
      // Set protocol stack not ready! 
#ifdef PRODUCT_DM
      uint8*                   buf = NULL;
      if (NULL == cm_sme_event)
      {
           cm_sme_event = SCI_CreateEvent("cm_sme_event"); // Create a comm event group
      }
      
      /*useful at ps&layer1 unit test*/
      if(REFPARAM_GetUartRecvMode() != 0)
      {
             SCI_SuspendThread(CM);
      }
#endif
      
      buf_max_size = sizeof(buffer);
#endif // End of WIN32  @hongliang.xin 2010-8-16 for simulator with monkey test
      
      for(;;)
      {
#ifdef WIN32
            sig_ptr = SCI_GetSignal(CM);
            
            SCI_ASSERT(SCI_NULL != sig_ptr);/*assert verified*/
            
            msg_head_ptr = (MSG_HEAD_T *)(sig_ptr + 1);
            
            SendMsgToTasks(sig_ptr,msg_head_ptr->len);
            
#else
            
            status  = SIO_ReadPPPFrame(COM_DEBUG, buffer,sizeof(buffer) , &len);
            sig_ptr = SCI_NULL;
            if(len != 0)
            {
                   if(packet_size==0)
                   {
                          
                          msg_head_ptr = (MSG_HEAD_T*)buffer;
                          packet_size = msg_head_ptr->len;
                          
                          sig_ptr		= (xSignalHeader)SCI_ALLOC_APP(packet_size + sizeof(xSignalHeaderRec)+4);
                          if(sig_ptr)
                          {
                          	  sig_ptr->SignalSize = packet_size + sizeof(xSignalHeaderRec);
                              sig_ptr->Sender		= CM;
                          }
                   }
                   if(sig_ptr)
                   {
                       SCI_MEMCPY(((uint8 *)(sig_ptr + 1))+buf_count, buffer, len);/*lint !e718 !e746 -e718 */
                       buf_count += len;
                       len = 0;
				   
                       if(status != SIO_PPP_SUCCESS)
                       {
                          status  = SIO_ReadPPPFrame(COM_DEBUG, ((uint8 *)(sig_ptr + 1))+buf_count,packet_size-buf_count+1 , &len);
                       }
                   }
            }
			
            buf_count += len;  /*lint !e737 */
            len = 0;
			
            if((status == SIO_PPP_SUCCESS)&&(0!=buf_count)&&(sig_ptr))
            {
                   if (buf_count==packet_size)
                   {
                   
       #ifdef PRODUCT_DM

                           buf = (uint8 *)(sig_ptr + 1);
                           msg_head_ptr = (MSG_HEAD_T*)buf;

                           if(msg_head_ptr->type == 0xF9 || msg_head_ptr->type == 0x66)
                           {
                                    uint32 smebuf_freesize = 0;
                                    GET_SEMBUF_FREE_SIZE(smebuf_freesize,SME_CM_BUF_SIZE,p_smebuf);
                                    if(smebuf_freesize >= buf_count)
                                    {
                                            if((SME_CM_BUF_SIZE - p_smebuf->u32wrtite_offset)> buf_count)
                                            {
                                                   memcpy(&p_smebuf->sme_cm_buf[p_smebuf->u32wrtite_offset],buf,buf_count);
                                                   p_smebuf->u32wrtite_offset += buf_count;
                                            }
                                            else
                                            {
                                                   memcpy(&p_smebuf->sme_cm_buf[p_smebuf->u32wrtite_offset],buf,SME_CM_BUF_SIZE - p_smebuf->u32wrtite_offset); 
                                                   memcpy(&p_smebuf->sme_cm_buf[0],buf + (SME_CM_BUF_SIZE - p_smebuf->u32wrtite_offset), p_smebuf->u32wrtite_offset+ buf_count - SME_CM_BUF_SIZE);  
                                                   p_smebuf->u32wrtite_offset =  p_smebuf->u32wrtite_offset+ buf_count - SME_CM_BUF_SIZE;
                                            }
                                            SCI_SetEvent(cm_sme_event, 1, SCI_OR);
                                    
                                    }
                                    else
                                    {
                                           //SCI_TRACE_LOW:"[CM]rec cmd size error!,buf_countn=%d\r\n"
                                           SCI_TRACE_ID(TRACE_TOOL_CONVERT,CM_221_112_2_18_1_3_52_0,(uint8*)"d",buf_count);
                                    
                                    }
                                    SCI_FREE(sig_ptr);
                                    packet_size = 0;
                                    buf_count = 0;
                                    continue;
                           }  
       #endif       
                           SendMsgToTasks(sig_ptr,buf_count);

                   }
                   else
                   {
                   	     SCI_FREE(sig_ptr);
                   }
                   packet_size = 0;
                   //buf_count should less than buf_max_size
                   
                   buf_count = 0;
            }
            else
            {
            	     if(sig_ptr)
             	     {
             	     	    SCI_FREE(sig_ptr);
             	     }
            }			
#endif	// End of WIN32
      }      
}


/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     uint32 SendMsgToTasks(xSignalHeader sig_ptr,uint32 size)              *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     This function sends the message to the corresponding thread           *
 **     according to the message type.                                        * 
 **                                                                           *
 ** INPUT                                                                     *
 **     msg:        Pointer of the input message.                             *
 **     size:       Size of the input message in uint8.                       *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     dest:       Pointer of the encoded message.                           *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     Return  SCI_SUCCESS: success, else failed by some reason              *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 SendMsgToTasks(xSignalHeader sig_ptr,uint32 size)
{
    uint32      status;       
    int32      msg_type;		/* Message type.*/
	uint16		seq_num;
    MSG_HEAD_T  *msg_head;
    
    msg_head = (MSG_HEAD_T *)(sig_ptr + 1);
    msg_type = msg_head->type;
	seq_num	 = msg_head->seq_num;

    /*
    * if received data size is not equal to the size of cmd
    * discard it directly
    */
    if(msg_head->len != size)
    {
        SCI_FREE(sig_ptr);
        
        //SCI_TRACE_LOW:"[CM]rec cmd size error!,msg_head->len=%d,size=%d\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CM_293_112_2_18_1_3_52_1,(uint8*)"dd",msg_head->len, size);

        return SCI_SIZE_ERROR;
    }
    
    // The message is belong to diagnostic thread.
    if ((msg_type >= MSG_REQ_REP_START) && (msg_type <= MSG_REQ_REP_END)) 
    {
        //switch(msg_type)
        //{
        //   default:
            // Send message to diagnostic task.
            //status = SCI_SendMessage(CM, DIAG, msg, SCI_PRI_NORMAL, SCI_NO_WAIT);
            if(0 == CM_get_queue_avilable(DIAG))
            {
                 SCI_Free(sig_ptr);
		         return SCI_QUEUE_ERROR;
            }
            status = SCI_SendSignal(sig_ptr, DIAG);            
            if (status != SCI_SUCCESS)
            {
                // Send back to channel server to indicate msg is received.
                SendResponseToChannelServer(seq_num);
                SCI_TRACE_ERROR("Can not send message to diag queue!\r\n");  /*assert verified*/

                return SCI_QUEUE_ERROR;
            }
        //  break;
        //}
    }
    // The message need no response.
    else if ((msg_type >= MSG_REQ_START) && (msg_type <= MSG_REQ_END)) 
    {
        // Send back to channel server to indicate msg is received.
        SendResponseToChannelServer(seq_num);
        
        switch(msg_type)
        {
        case PS_REQ_F:
            // Send message to protocol stack task.
            SendPSMessage(sig_ptr);
            break;

#ifndef WIN32
		case PPP_PACKET_R:
			// 
			SIO_SaveToPPPBuf((char *) ((char *)(sig_ptr + 1) + sizeof(MSG_HEAD_T)), 
				(uint32) (msg_head->len - sizeof(MSG_HEAD_T)));

			SCI_FREE(sig_ptr);
			break;
#endif
        default:
            SCI_FREE(sig_ptr);
            break;
        }
    }
    // Well, it is impossible, but we still send a ok message to channel server.
    else
    {
        // Send back to channel server to indicate msg is received.
        
        SendResponseToChannelServer(seq_num);
        SCI_FREE(sig_ptr);
    }
    
    return SCI_SUCCESS;
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     uint16 SendResponseToChannelServer()                                  *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     This function Send receive OK response to Channel server.             *
 **                                                                           *
 ** INPUT                                                                     *
 **     seq_num:          Seqeunce number for the message frame.              *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     None                                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **---------------------------------------------------------------------------*/
LOCAL void SendResponseToChannelServer(uint16 seq_num)
{
    static MSG_HEAD_T send_ok_data=
    {
        0,           // seq_num
        0x08,        // size
        MSG_SEND_OK, // Main type
        0            // subtype
    };
    
    send_ok_data.seq_num  = seq_num;
    
    SIO_SendPacket((uint8 *) &send_ok_data, sizeof(send_ok_data));
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SendPSMessage(xSignalHeader sig_ptr)                             *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     This function Send messages to protocol stack.                        *
 **                                                                           *
 ** INPUT                                                                     *
 **     msg:              Pointer of the input message.                       *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     None                                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **---------------------------------------------------------------------------*/
LOCAL void SendPSMessage(xSignalHeader sig_ptr)
{
#ifdef _FEATURE_BSSIM_TEST
    xSignalHeader send_sig_ptr;
    
    // Get length of the signal.
    uint32 len = sig_ptr->SignalSize - sizeof(xSignalHeaderRec) - sizeof(MSG_HEAD_T);
    
    // Make sure that length is less than 255!
    SCI_ASSERT(0xFF >= len);/*assert verified*/

    if (is_ps_ready)
    {
        // Malloc buffer for signal.
        send_sig_ptr = (xSignalHeader)SCI_ALLOC_APP(len);
        // modify end
        // Copy signal to the buffer.
        SCI_MEMCPY(send_sig_ptr, (void *)((uint32)sig_ptr + sizeof(xSignalHeaderRec)
			+ sizeof(MSG_HEAD_T)), len);
    
        SCIAI_AIRMsgToPSMsg(&(send_sig_ptr->SignalCode));

        // Send message to proper task.
        send_sig_ptr->Sender = CM;
		if (CM == (send_sig_ptr->SignalCode >> 8))
		{
			SCI_ASSERT(0);/*assert verified*/
		}
        if(0 == CM_get_queue_avilable(send_sig_ptr->SignalCode >> 8))
        {
             SCI_FREE(send_sig_ptr);
		     SCI_FREE(sig_ptr);		 
             return;
        }
        SCI_SendSignal(send_sig_ptr, send_sig_ptr->SignalCode >> 8);
    }
#endif
    // Free message.
    SCI_FREE(sig_ptr);
}

PUBLIC  int SME_ReadPPPFrame(
                     uint8  *src,            // Data to receive
                     int    size,            // MAX size to receive 
                     uint32 *dest_len        // Actual size received
                     )
{
#ifdef PRODUCT_DM
	 uint32  u32_actual_event,u32_sembuf_freesize, u32cpysize= 0;
     T_SMEBUF *    p_smebuf =&s_cmsme_buf; 
	 
     GET_SEMBUF_FREE_SIZE(u32_sembuf_freesize,SME_CM_BUF_SIZE,p_smebuf);
     if(u32_sembuf_freesize == SME_CM_BUF_SIZE)
     {
		  SCI_GetEvent(cm_sme_event, 
                         1, 
                          SCI_AND_CLEAR, 
                          &u32_actual_event, 
                          SCI_WAIT_FOREVER);/*replace sleep by SCI_GetEvent */
     }
	GET_SEMBUF_FREE_SIZE(u32_sembuf_freesize,SME_CM_BUF_SIZE,p_smebuf);
	u32cpysize = (SME_CM_BUF_SIZE  - u32_sembuf_freesize)>(uint32)size?(uint32)size:(SME_CM_BUF_SIZE  - u32_sembuf_freesize);
	if((SME_CM_BUF_SIZE - p_smebuf->u32read_offset)> u32cpysize)
	{
	    memcpy(src,&p_smebuf->sme_cm_buf[p_smebuf->u32read_offset],u32cpysize);
	    p_smebuf->u32read_offset += u32cpysize;
	}
	else
	{
	    memcpy(src, &p_smebuf->sme_cm_buf[p_smebuf->u32read_offset],SME_CM_BUF_SIZE - p_smebuf->u32read_offset); 
	    memcpy(&p_smebuf->sme_cm_buf[0],src+(SME_CM_BUF_SIZE - p_smebuf->u32read_offset), p_smebuf->u32read_offset+ u32cpysize - SME_CM_BUF_SIZE);  
	    p_smebuf->u32read_offset =  p_smebuf->u32read_offset+ u32cpysize - SME_CM_BUF_SIZE;
	}
	*dest_len = u32cpysize;
	return u32cpysize;
#else
    return 0;
#endif	
}

/*****************************************************************************/
//  Description : Get the appoint task id queue's avilable queue
//  Global resource dependence : 
//  Author:zhenghuai qin
//  Note: :Tp_srv.c --> get_app_queue_avilable  
/*****************************************************************************/
LOCAL uint32 CM_get_queue_avilable(BLOCK_ID  thread_id)
{
    uint32          status           = 0;
    uint32          count            = 0;
    uint32          enqueued         = 0;
    uint32          store            = 0;
    char           *queue_name_ptr   = SCI_NULL;
    TX_THREAD      *suspended_thread = SCI_NULL;    
    TX_QUEUE       *next_queue       = SCI_NULL;
    SCI_THREAD_T   *thread_block     = SCI_NULL;

    thread_block = (SCI_THREAD_T *)SCI_GetThreadBlockFromId(thread_id);
    status = tx_queue_info_get((TX_QUEUE *)&(thread_block->queue),
                               &queue_name_ptr,
                               (uint32 *)&enqueued,
                               (uint32 *)&store,
                               (TX_THREAD **)&suspended_thread,
                               (uint32 *)&count,
                               (TX_QUEUE **)&next_queue);
    return store;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

