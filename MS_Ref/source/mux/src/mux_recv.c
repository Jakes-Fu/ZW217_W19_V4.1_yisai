/******************************************************************************
 ** File Name:      mux_recv.c                                      *
 ** Author:         yayan.xu                                             *
 ** DATE:           07/10/2008                                              *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:      The file defines MUX RECV TASK behavior*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/10/2008     yayan.xu     Create.                                   *
 ******************************************************************************/
#include "ms_ref_mux_trc.h"
#include "mux_all.h"
#include "mux_driver.h"
#include "priority_base.h"

extern MUX_MUTEX_PTR   mux_send_mutex;
/*MUX RECV TASK ID*/
uint32  mux_recv_task_id    = 0;

MUX_RETURN_E MUX_Get_Data_Block(uint32 len, uint8 *data_ptr, MUX_INFO_T * mux_ptr);
LOCAL void        MUX_Data_Parse_Basic(uint32 rec_num, uint8 *src_buf, MUX_INFO_T * mux_ptr);
LOCAL void        MUX_Data_Parse_Advance(uint32 rec_num, uint8 *src_buf, MUX_INFO_T * mux_ptr);
LOCAL int           MUX_Frame_Parse(MUX_FRAME_HEAD *mux_pkt_head_ptr, uint8 *inf_data_buff, MUX_INFO_T * mux_ptr);


/*****************************************************************************/
// Description :   Init mux_rev_buf
//
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      10th Nov.08
// Note :           
/*****************************************************************************/
MUX_RETURN_E  MUX_Rev_Buf_Init(MUX_INFO_T *mux_ptr)
{

        mux_ptr->mux_rev_buf = MUX_ALLOC(MUX_MAX_BUF_SIZE);
        MUX_ASSERT(mux_ptr->mux_rev_buf != PNULL);/*assert verified*/
        mux_ptr->end_ptr = mux_ptr->mux_rev_buf;
        mux_ptr->start_ptr = NULL;
        mux_ptr->framelen= -1;
        mux_ptr->adv_escape_flag = FALSE; //only used in advance mode
        mux_ptr->adv_start_flag = FALSE;
        return MUX_RET_SUCCESS;

}


/*****************************************************************************/
// Description :   MUX RECV TASK main entry 
//
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      10th July.08
// Note :        MUX_RecvTaskEntry1 for COM_DATA-->MUX_1
/*****************************************************************************/
void MUX_RecvTaskEntry1(uint32 argc, void *argv)
{
#ifndef _WINDOWS

        int count, point_start,point_end;
        uint32   ret_event_flag;
        uint8 mux_index = MUX_1;
        MUX_INFO_T *mux_ptr = &g_mux_sys_info[mux_index];

	for (;;)
       {

           
            if(NULL == mux_ptr->g_MuxRecv_Eptr)
            {
                //MUX_TRACE_WARNING:"MUX_Res_Wait: g_MuxRecv_Eptr is null ptr in MUX entry1!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_RECV_70_112_2_18_1_49_25_122,(uint8*)"");
                MUX_SLEEP(MUX_SLEEP_TIME);	            
                continue;
            }
            else
            {
                MUX_GET_EVENT(mux_ptr->g_MuxRecv_Eptr, mux_ptr->mux_recvtask_event, MUX_OR_CLEAR, &ret_event_flag, MUX_WAIT_FOREVER);  

            }


    		if (SIO_ALREADY_OPEN != mux_port[mux_index].open_flag)
    		{
                        MUX_SLEEP(MUX_SLEEP_TIME);	            
                        continue;
    		}
    		
                  
    		point_end = mux_port[mux_index].rx_buf.end_point ;
    		point_start =  mux_port[mux_index].rx_buf.start_point;
    		count = point_end - point_start ;
            
    		if (0 == count)
    		{
    			continue;
    		}
            


            if(count > 0 )
            {
                while(count >= MUX_MAX_BUF_SIZE)
                {
                        MUX_Get_Data_Block(MUX_MAX_BUF_SIZE,  &(mux_port[mux_index].rx_buf.sio_buf_ptr[point_start]), mux_ptr);
                        point_start += MUX_MAX_BUF_SIZE;
                        count -= MUX_MAX_BUF_SIZE;
    			   continue;                
                }
    			MUX_Get_Data_Block(count,  &(mux_port[mux_index].rx_buf.sio_buf_ptr[point_start]), mux_ptr);
            }
            else //end_ponit < start ponit;real count = MUX_REC_DATA_MAX + (end_point - start_point)
            {
    			count = (int)mux_port[mux_index].rx_buf.size - point_start ;
      			while(count >= MUX_MAX_BUF_SIZE)
    			{
      				MUX_Get_Data_Block(MUX_MAX_BUF_SIZE, &(mux_port[mux_index].rx_buf.sio_buf_ptr[point_start]), mux_ptr);
      				point_start += MUX_MAX_BUF_SIZE;
                            count -= MUX_MAX_BUF_SIZE;
    				continue;
    			}
    			MUX_Get_Data_Block(count,  &(mux_port[mux_index].rx_buf.sio_buf_ptr[point_start]), mux_ptr);

                     count = point_end;
                     point_start = 0;
                
      			while(count >= MUX_MAX_BUF_SIZE)
    			{
      				MUX_Get_Data_Block(MUX_MAX_BUF_SIZE,  &(mux_port[mux_index].rx_buf.sio_buf_ptr[point_start]), mux_ptr);
      				point_start += MUX_MAX_BUF_SIZE;
                            count -= MUX_MAX_BUF_SIZE;
    				continue;
    			}
    			MUX_Get_Data_Block(count,  &(mux_port[mux_index].rx_buf.sio_buf_ptr[point_start]), mux_ptr);           
            }
            mux_port[mux_index].rx_buf.start_point = point_end ;
     }

#endif
	/*lint  -e527*/

    return;
/*lint  +e527*/

}
/*****************************************************************************/
// Description :   MUX RECV TASK main entry 
//
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      10th July.08
// Note :        MUX_RecvTaskEntry1 for COM_DATA2-->MUX_2
/*****************************************************************************/
void MUX_RecvTaskEntry2(uint32 argc, void *argv)
{
#ifndef _WINDOWS

        int count, point_start,point_end;
        uint32   ret_event_flag;
        uint8 mux_index = MUX_2;
        MUX_INFO_T *mux_ptr = &g_mux_sys_info[mux_index];

	for (;;)
       {

            if(NULL == mux_ptr->g_MuxRecv_Eptr)
            {
                //MUX_TRACE_WARNING:"MUX_Res_Wait: g_MuxRecv_Eptr is null ptr in MUX entry2!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_RECV_171_112_2_18_1_49_26_123,(uint8*)"");
                MUX_SLEEP(MUX_SLEEP_TIME);	            
                continue;
            }
            else
            {
                MUX_GET_EVENT(mux_ptr->g_MuxRecv_Eptr, mux_ptr->mux_recvtask_event, MUX_OR_CLEAR, &ret_event_flag, MUX_WAIT_FOREVER);  

            }


    		if (SIO_ALREADY_OPEN != mux_port[mux_index].open_flag)
    		{
                        MUX_SLEEP(MUX_SLEEP_TIME);	            
                        continue;
    		}
    		
                  
    		point_end = mux_port[mux_index].rx_buf.end_point ;
    		point_start =  mux_port[mux_index].rx_buf.start_point;
    		count = point_end - point_start ;
            
    		if (0 == count)
    		{
    			continue;
    		}
            

            if(count > 0 )
            {
                while(count >= MUX_MAX_BUF_SIZE)
                {
                        MUX_Get_Data_Block(MUX_MAX_BUF_SIZE,  &(mux_port[mux_index].rx_buf.sio_buf_ptr[point_start]), mux_ptr);
                        point_start += MUX_MAX_BUF_SIZE;
                        count -= MUX_MAX_BUF_SIZE;
    			   continue;                
                }
    			MUX_Get_Data_Block(count,  &(mux_port[mux_index].rx_buf.sio_buf_ptr[point_start]), mux_ptr);
            }
            else //end_ponit < start ponit;real count = MUX_REC_DATA_MAX + (end_point - start_point)
            {
    			count = (int)mux_port[mux_index].rx_buf.size - point_start ;
      			while(count >= MUX_MAX_BUF_SIZE)
    			{
      				MUX_Get_Data_Block(MUX_MAX_BUF_SIZE, &(mux_port[mux_index].rx_buf.sio_buf_ptr[point_start]), mux_ptr);
      				point_start += MUX_MAX_BUF_SIZE;
                            count -= MUX_MAX_BUF_SIZE;
    				continue;
    			}
    			MUX_Get_Data_Block(count,  &(mux_port[mux_index].rx_buf.sio_buf_ptr[point_start]), mux_ptr);

                     count = point_end;
                     point_start = 0;
                
      			while(count >= MUX_MAX_BUF_SIZE)
    			{
      				MUX_Get_Data_Block(MUX_MAX_BUF_SIZE,  &(mux_port[mux_index].rx_buf.sio_buf_ptr[point_start]), mux_ptr);
      				point_start += MUX_MAX_BUF_SIZE;
                            count -= MUX_MAX_BUF_SIZE;
    				continue;
    			}
    			MUX_Get_Data_Block(count,  &(mux_port[mux_index].rx_buf.sio_buf_ptr[point_start]), mux_ptr);           
            }
            mux_port[mux_index].rx_buf.start_point = point_end ;
     }

#endif
	/*lint  -e527*/

    return;
/*lint  +e527*/

}
/*****************************************************************************/
// Description :   MUX RECV TASK init, create mux_recv_task 
//
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      10th July.08
// Note :           
/*****************************************************************************/
PUBLIC void MUX_RecvTaskInit(void)
{

   MUX_INFO_T  *mux_ptr = NULL;
   uint8 i = 0 ;
   char info[50];
   
    //MS00148955 
    if (NULL == mux_send_mutex)
    {
        mux_send_mutex = MUX_CREATE_MUTEX("MUX_SEND_MUTEX", MUX_INHERIT);
    }
   
   
   for(i = 0 ; i < MAX_MUX_NUM; i++)
   {
        mux_ptr = &g_mux_sys_info[i];
        mux_ptr->is_start = MUX_STATUS_OFF;
	 mux_ptr->TE_wait_OK = FALSE;	 //MS00145432 
        mux_ptr->mux_index = i; //MUX_1 or MUX_2
        
        if(0 == i )
        {
              mux_ptr->ctl_link = MUX_COM1_CTL_LINK;
        }
        else
        {
              mux_ptr->ctl_link = MUX_COM2_CTL_LINK;
        }
        
        /*ONLY USE LINK0, used to serial mode before enter MUX*/
        MUX_FlushDlci(mux_ptr->ctl_link);

        g_mux_dlc_info[mux_ptr->ctl_link].from = mux_ptr->mux_index;

        mux_ptr->g_MuxEvent_ptr = NULL;
        
#ifndef _WINDOWS
       
        sprintf(info, "Mux RECV TASK %d Event", i); //NOTE: size of info is 50.

        /*Create MUX RECV TASK wait event */
        mux_ptr->g_MuxRecv_Eptr = MUX_CREATE_EVENT(info);

        MUX_ASSERT(PNULL != mux_ptr->g_MuxRecv_Eptr);  /*assert verified*/   

        mux_ptr->mux_recvtask_event = 1;/*bit 0*/

       //sprintf(info, "Mux RECV TASK%d ", i);
       if( 0 == i)
       {
            mux_ptr->mux_recv_task_id = MUX_CREATE_THREAD("Mux RECV TASK1", "Mux Queue", MUX_RecvTaskEntry1, 0, NULL, 51200, 10,
            PRI_MUX_RECV_TASK1,//10, //MUX_PRIORITY_ABOVE_NORMAL,
            MUX_PREEMPT, MUX_AUTO_START);   
       }
       else
       {
            mux_ptr->mux_recv_task_id = MUX_CREATE_THREAD("Mux RECV TASK2", "Mux Queue", MUX_RecvTaskEntry2, 0, NULL, 51200, 10,
            PRI_MUX_RECV_TASK2,//10, //MUX_PRIORITY_ABOVE_NORMAL,
            MUX_PREEMPT, MUX_AUTO_START);   
       }
#endif

       
   }


    return;
}
/*****************************************************************************/
// Description :   MUX RECV TASK main entry 
//
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      10th July.08
// Note :           
/*****************************************************************************/
MUX_RETURN_E MUX_Get_Data_Block(uint32 len, uint8 *data_ptr, MUX_INFO_T * mux_ptr)
{

    MUX_ASSERT(mux_ptr != PNULL);/*assert verified*/

#ifdef _MUX_DEBUG_
    /*In order to debug, first trace*/
    MUX_TRACE_LOW(("MUX_Get_Data_Block  in %d:", mux_ptr->mux_index));
    MUX_TraceData(data_ptr, len);
#endif


    if (MUX_STATUS_OFF == mux_ptr->is_start)
    {
        /*Serial port work mode.*/       
        /*link 0 is used to send and recieve data.*/
        MUX_Forward_Up(mux_ptr->ctl_link, len, data_ptr);

        return MUX_RET_SUCCESS;
    }

    if((MUX_SIDE_UE == mux_ptr->side)&&(MUX_STATUS_ON_REQ == mux_ptr->is_start))
    {
         //Wait for ATC send OK of AT+CMUX; Discard all received data
        //MUX_TRACE_WARNING:"MUX_Get_Data_Block discard data(len=%d)  in %d:"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_RECV_348_112_2_18_1_49_26_124,(uint8*)"dd", len, mux_ptr->mux_index);
        return MUX_RET_SUCCESS;
    }
    if (MUX_STATUS_ON_REQ == mux_ptr->is_start)
    {
        /*In this status, MUX wait for response of AT+CMUX*/
        MUX_Parse_Atcmd_Res(len, data_ptr, mux_ptr);
        return MUX_RET_SUCCESS;
    }

    switch (mux_ptr->sys_parameter.mux_mode)
    {
        case  MUX_OPTION_BASIC:
            MUX_Data_Parse_Basic(len, data_ptr, mux_ptr);
            break;
        case  MUX_OPTION_ADVANCED:
            MUX_Data_Parse_Advance(len, data_ptr, mux_ptr);
            break;
        case  MUX_OPTION_ADVANCED_ERM:
        default:
            //MUX_TRACE_WARNING:"MUX_Get_Data_Block: work_mode error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_RECV_368_112_2_18_1_49_26_125,(uint8*)"");
            break;
    }
    return MUX_RET_SUCCESS;
}
/*****************************************************************************/
// Description :  Push  User data  to Upper layer AP.
//
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      14th July.08
// Note :    memory of  data_ptr [IN]   is allocate by dynamic.
/*****************************************************************************/
MUX_RETURN_E MUX_Forward_Up(uint8 link_id, uint32 len, uint8 *data_ptr)
{
    /*check +++*/
    uint8   i;
    uint8   plus_char[MAX_PLUS_NUMBER];
    uint32  plus_num1       = MAX_PLUS_NUMBER;
    uint8  *tmp_data_ptr    = data_ptr;
#ifdef MUX_ATCIPSEND    
    MUX_ATCIPSEND_T * pMux = MUX_ATC_RtnGblCtl();
#endif
	
	MUX_TRACE_WARNING(("MUX_Forward_Up"));

    /*check link id validity*/
    if (link_id >= MUX_MAX_LINK_NUM)
    {
        MUX_TRACE_WARNING(("MUX_Forward_Up fail due to link id(%d) execed", link_id));
        return MUX_RET_LINK_NUM_ERR;
    }
    
    // If it is possible that we got three '+'
#ifdef MUX_ATCIPSEND
    if ((MAX_PLUS_NUMBER >= len) && (MUX_DLC_DATA_MODE == g_mux_dlc_info[link_id].work_mode) && !(pMux->atc_is_max_mode) )
#else
    if ((MAX_PLUS_NUMBER >= len) && (MUX_DLC_DATA_MODE == g_mux_dlc_info[link_id].work_mode))
#endif
    {
        tmp_data_ptr = data_ptr;
        // Fisrt get the char from FIFO
        for (i = 0; i < MAX_PLUS_NUMBER; i++)
        {
            plus_char[i] = *tmp_data_ptr++;

            // Check if it is a plus char
            if (PLUS_CHAR != plus_char[i])
            {
                plus_num1 = 0;
            }
        }

        if (MAX_PLUS_NUMBER == plus_num1)
        {
            MUX_TRACE_LOW(("MUX: Receive +++ in link %d\r\n", link_id));
            // Set Timer to check if we received a mode change command.
            //MS00226768
            if (NULL == g_mux_dlc_info[link_id].mux_plus_timer)
            {
				g_mux_dlc_info[link_id].mux_plus_timer = MUX_CREATE_TIMER("MUX PLUS TIMER", (TIMER_FUN) MUX_Check_ThreePlus, link_id, MUX_MODE_CHANGE_DELAY, MUX_NO_ACTIVATE);
				if (NULL == g_mux_dlc_info[link_id].mux_plus_timer)
				{
					MUX_TRACE_LOW(("MUX: Create timer fails in link %d\r\n", link_id));
					return MUX_RET_FAILURE;
				}
            }
            MUX_DEACTIVE_TIMER(g_mux_dlc_info[link_id].mux_plus_timer);
            MUX_CHANGE_TIMER(g_mux_dlc_info[link_id].mux_plus_timer, MUX_Check_ThreePlus, MUX_MODE_CHANGE_DELAY + 100);  // Add 100 to make the timer response longer, so that the timeout logic will be OK.
            MUX_ACTIVE_TIMER(g_mux_dlc_info[link_id].mux_plus_timer);
            return MUX_RET_SUCCESS;
        }
    }

    if (MUX_DLC_AT_MODE == g_mux_dlc_info[link_id].work_mode)
    {
#ifdef MUX_ATCIPSEND
    
        pMux->cur_atc_frame_size += len;

        if(!(pMux->atc_is_max_mode) )
        {
            /*AT mode, notify ATC module*/
            if (g_mux_dlc_info[link_id].common_recv_func)   
            {                                                           
                (*g_mux_dlc_info[link_id].common_recv_func) (link_id, len, data_ptr);           
            }                                               
            else                                               
            {                                               
                MUX_TRACE_WARNING(("MUX_Forward_Up: ATC task Not Ready,Discard Recv Data!"));        	 
            }                           
            pMux->cur_atc_frame_size = 0;
        }
        else
        {
            //SCI_TRACE_LOW("SIO_RxForUserPort cur_atc_frame_size %d",pMux->cur_atc_frame_size);
        	if(pMux->atc_cmdline_maxlen <= pMux->cur_atc_frame_size)
        	{
        		SCI_TRACE_LOW("MUX_Forward_Up cur_atc_frame_size %d", pMux->cur_atc_frame_size);
        		if (g_mux_dlc_info[link_id].common_recv_func)   
                {                                                           
                    (*g_mux_dlc_info[link_id].common_recv_func) (link_id, len, data_ptr);           
                }                                               
                else                                               
                {                                               
                    MUX_TRACE_WARNING(("MUX_Forward_Up: ATC task Not Ready,Discard Recv Data!"));        	 
                }                           
        		pMux->cur_atc_frame_size = 0;
        	}
        }
        
#else
        /*AT mode, notify ATC module*/
        if (g_mux_dlc_info[link_id].common_recv_func)   
        {                                                           
            (*g_mux_dlc_info[link_id].common_recv_func) (link_id, len, data_ptr);           
        }                                               
        else                                               
        {                                               
            MUX_TRACE_WARNING(("MUX_Forward_Up: ATC task Not Ready,Discard Recv Data!"));        	 
        }                                               
#endif  //#ifdef MUX_ATCIPSEND
       
    }
    else
    {
        if (g_mux_dlc_info[link_id].entity.entity_recv_func)
        {
            (*g_mux_dlc_info[link_id].entity.entity_recv_func) (link_id, len, data_ptr);
        }
        else
        {
            
            //MUX_TRACE_WARNING:"MUX_Forward_Up: REcv task Not Ready,Discard Recv Data!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_RECV_446_112_2_18_1_49_26_127,(uint8*)"");
        }
    }

    return MUX_RET_SUCCESS;
}
/*****************************************************************************/
// Description :  Parse data, Parse layer 1
////copy data to mux_rev_buf, and check mux_rev_buf, 
//to find a whole MUX frame according to MUX basic structure 
// Calc CRC.
//
// Global resource dependence : none
// Author :    from Amoi
// DATE :      14th July.08
// Note :           
/*****************************************************************************/
LOCAL void MUX_Data_Parse_Basic(uint32 rec_num, uint8 *src_buf, MUX_INFO_T * mux_ptr)
{
    uint32          count;
    uint8          *search_ptr, *to, *from;
    MUX_FRAME_HEAD *mux_pkt_head_ptr;
    uint32  crc_error;
    uint8  *inf_data_start;
    uint32  inf_len;
    uint32 left_count = 0;
    uint8 *left_data_ptr;

    MUX_ASSERT(mux_ptr != PNULL);/*assert verified*/
    if(mux_ptr->mux_rev_buf == NULL)
    {
    	  MUX_TRACE_LOW(("mux_ptr->mux_rev_buf == NULL"));
    	  return;
    }
    MUX_TRACE_LOW(("mux_data_parse begin, rec_num = %d in mux_index %d", rec_num, mux_ptr->mux_index));
    count = rec_num;
    if (count > (MUX_MAX_BUF_SIZE - (uint32)(mux_ptr->end_ptr - mux_ptr->mux_rev_buf)))
    {
        MUX_TRACE_LOW(("MUX receive_worker: !!!!! Exceed buffer boundary in mux %d\n", mux_ptr->mux_index));
        count = (MUX_MAX_BUF_SIZE - (mux_ptr->end_ptr - mux_ptr->mux_rev_buf));
    }

    MUX_MEMCPY(mux_ptr->end_ptr, src_buf, count);
    mux_ptr->end_ptr += count;

    left_count = rec_num - count;//left receive data in src_buf yayan.xu@2009-1-7
    left_data_ptr = src_buf + count;
    if ((mux_ptr->start_ptr != 0) && (mux_ptr->framelen != -1))
    {
        if ((mux_ptr->end_ptr - mux_ptr->start_ptr) < mux_ptr->framelen)
        {
            return;
        }
    }
    
    search_ptr =mux_ptr->mux_rev_buf;
    while (1)/*lint !e716*/
    {
        if (mux_ptr->start_ptr == 0)     /* Frame Start Flag not found */
        {
            mux_ptr->framelen = -1;
            while (search_ptr < mux_ptr->end_ptr)
            {
                if (*search_ptr == MUX_BASIC_FLAG)
                {
                    mux_ptr->start_ptr = search_ptr;
                    break;
                }
                search_ptr++;
            }

            if (mux_ptr->start_ptr == 0)
            {
                mux_ptr->end_ptr = mux_ptr->mux_rev_buf;
                break;
            }
        }
        else
        {
            /* Frame Start Flag found */
            /* 1 start flag + 1 address + 1 control + 1 or 2 length + lengths data + 1 FCS + 1 end flag */
            if ((-1 == mux_ptr->framelen) && ((mux_ptr->end_ptr - mux_ptr->start_ptr) > MUX_MAX_HDR_SIZE))
            {
                mux_pkt_head_ptr = (MUX_FRAME_HEAD *) (mux_ptr->start_ptr + ADDRESS_FIELD_OFFSET);

                if (mux_pkt_head_ptr->length.ea == 1)
                {
                    /* short frame */
                    mux_ptr->framelen = MUX_MAX_HDR_SIZE + (uint8) (mux_pkt_head_ptr->length.l_len) + FLAG_SIZE - 1 ;
                }
                else
                {
                    /* long frame */
                   mux_ptr->framelen = MUX_MAX_HDR_SIZE + GET_LONG_LENGTH(mux_pkt_head_ptr->length) + FLAG_SIZE;
                }

                if (mux_ptr->framelen > MUX_MAX_TOTAL_FRAME_SIZE)
                {
                    //MUX_TRACE_WARNING:"MUX Error: frame length:%d is bigger than Max total frame size:%d\n"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_RECV_542_112_2_18_1_49_27_128,(uint8*)"dd", mux_ptr->framelen, (MUX_MAX_TOTAL_FRAME_SIZE));
                    search_ptr = mux_ptr->start_ptr + 1;
                    mux_ptr->start_ptr = 0;
                    mux_ptr->framelen = -1;
                    continue;
                }
            }

            if ((mux_ptr->framelen != -1) && ((mux_ptr->end_ptr - mux_ptr->start_ptr) >= mux_ptr->framelen))
            {
                if (*(mux_ptr->start_ptr + mux_ptr->framelen - 1) == MUX_BASIC_FLAG)/* OK, We got one frame */
                {
                    mux_pkt_head_ptr = (MUX_FRAME_HEAD *) (mux_ptr->start_ptr + ADDRESS_FIELD_OFFSET);
                    if ((mux_pkt_head_ptr->length.ea) == 0)
                    {
                        //  mux_pkt_ptr->data_ptr=(uint8 *) (start_ptr+MUX_MAX_HDR_SIZE);
                        inf_len = GET_LONG_LENGTH(mux_pkt_head_ptr->length);
                        inf_data_start = (uint8 *) (mux_ptr->start_ptr + MUX_MAX_HDR_SIZE);;
                        crc_error = MUX_Crc_Check((uint8 *) (mux_ptr->start_ptr + ADDRESS_FIELD_OFFSET), LONG_CRC_CHECK, *(inf_data_start + inf_len));
                    }
                    else
                    {
                        //   mux_pkt_ptr->data_ptr=(uint8 *) (start_ptr+MUX_MAX_HDR_SIZE-1);
                        inf_len = mux_pkt_head_ptr->length.l_len;
                        inf_data_start = (uint8 *) (mux_ptr->start_ptr + MUX_MAX_HDR_SIZE - 1);;
                        crc_error = MUX_Crc_Check((uint8 *) (mux_ptr->start_ptr + ADDRESS_FIELD_OFFSET), SHORT_CRC_CHECK, *(inf_data_start + inf_len));
                    }

                    if (!crc_error)
                    {
                        /*CRC success*/
                        MUX_Frame_Parse(mux_pkt_head_ptr, inf_data_start, mux_ptr);
                    }
                    else
                    {
                        /* crc_error */
                        search_ptr = mux_ptr->start_ptr + 1;
                        mux_ptr->start_ptr = 0;
                        mux_ptr->framelen = -1;
                        continue;
                    }   /*End if(!crc_error) */

                    search_ptr = mux_ptr->start_ptr + mux_ptr->framelen;
                }
                else
                {
                    //MUX_TRACE_WARNING:"MUX: Lost synchronization in MUX %d\n"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_RECV_588_112_2_18_1_49_27_129,(uint8*)"d", mux_ptr->mux_index);
                    MUX_TraceData(mux_ptr->start_ptr, mux_ptr->framelen);
                    search_ptr = mux_ptr->start_ptr + 1;
                }

                mux_ptr->start_ptr = 0;
                mux_ptr->framelen = -1;
                continue;
            }/*if ((framelen != -1) && ((end_ptr - start_ptr) >= framelen))*/
            if(mux_ptr->mux_rev_buf == NULL)
            {
            		 MUX_TRACE_LOW(("mux_ptr->mux_rev_buf == NULL"));
    	           break;
            }
            if (mux_ptr->start_ptr != mux_ptr->mux_rev_buf)
            {
                to = mux_ptr->mux_rev_buf;
                from = mux_ptr->start_ptr;
                count = mux_ptr->end_ptr - mux_ptr->start_ptr;
                while (count--)
                {
                    *to++ = *from++;
                }
                mux_ptr->end_ptr -= (mux_ptr->start_ptr - mux_ptr->mux_rev_buf);
                mux_ptr->start_ptr = mux_ptr->mux_rev_buf;
            }

            if(left_count>0)//mod @yayan.xu 2009-1-7
            {
                count = left_count;
                if (count > (MUX_MAX_BUF_SIZE - (uint32)(mux_ptr->end_ptr - mux_ptr->mux_rev_buf)))
                {
                    MUX_TRACE_LOW(("MUX receive_worker: !!!!! Exceed buffer boundary 2 in MUX %d!\n", mux_ptr->mux_index));
                    count = (MUX_MAX_BUF_SIZE - (mux_ptr->end_ptr - mux_ptr->mux_rev_buf));
                }

                MUX_MEMCPY(mux_ptr->end_ptr, left_data_ptr, count);
                mux_ptr->end_ptr += count;
                left_count = left_count - count;//left receive data in src_buf
                left_data_ptr = left_data_ptr + count;
                continue;
            }
            break;
        }/* End Frame Start Flag found */
    }   /* End while(1) */
}




/*****************************************************************************/
// Description :  Parse data, Parse layer 1
////copy data to mux_rev_buf, and check mux_rev_buf, 
//to find a whole MUX frame according to MUX advance structure 
// Calc CRC.
//
// Global resource dependence : none
// Author :    yayan.xu
// DATE :      14th Sep.08
// Note :           
/*****************************************************************************/
LOCAL void MUX_Data_Parse_Advance(uint32 rec_num, uint8 *src_buf, MUX_INFO_T *mux_ptr)
{
    LOCAL uint32    count   = 0;
   // LOCAL unsigned char * end_lptr = mux_ptr->mux_rev_buf;/*end_ptr store mux_rev_buf end_point.*/
    //LOCAL BOOLEAN   start_flag  = FALSE;
    //LOCAL BOOLEAN   escape_flag = FALSE;
    unsigned char * search_ptr = NULL; 
    MUX_FRAME_HEAD  mux_pkt_head;
    uint32          crc_error;
    uint8          *inf_data_start;

    uint32          i;
    unsigned char tmp_char = 0x00;

    MUX_ASSERT(mux_ptr != PNULL);/*assert verified*/
    if(mux_ptr->mux_rev_buf == NULL)
    {
    	    MUX_TRACE_LOW(("mux_ptr->mux_rev_buf == NULL"));
          return;
    }      
    MUX_TRACE_LOW(("MUX_Data_Parse_Advance begin, length = %d in MUX %d", rec_num, mux_ptr->mux_index));

    if (rec_num > (MUX_MAX_BUF_SIZE - (uint32)(mux_ptr->end_ptr - mux_ptr->mux_rev_buf)))
    {
        //MUX_TRACE_WARNING:"MUX MUX_Data_Parse_Advance: !!!!! Exceed buffer boundary in MUX %d!\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_RECV_671_112_2_18_1_49_27_130,(uint8*)"d", mux_ptr->mux_index);
        rec_num = (MUX_MAX_BUF_SIZE - (mux_ptr->end_ptr - mux_ptr->mux_rev_buf));
    }


    search_ptr = src_buf;

    for (i = 0; i < rec_num; i++)
    {
        tmp_char = *search_ptr++;

        if (FALSE == mux_ptr->adv_start_flag)     /* Frame Start Flag not found */
        {
            if (MUX_ADVANCE_FLAG == tmp_char)
            {
                mux_ptr->adv_start_flag = TRUE;/*record frame header position.*/                        
                *mux_ptr->end_ptr++ = tmp_char;/*0x7E write to mux_rev_buf.*/
                count++;
            }
            /*Before find Frame start Flag, Discard invalid char*/
            continue;
        }

        if (MUX_ESCAPE_FLAG == tmp_char)
        {
            mux_ptr->adv_escape_flag = TRUE;
            continue;
        }

        if (TRUE == mux_ptr->adv_escape_flag)
        {
            *mux_ptr->end_ptr++ = tmp_char ^ MUX_COMPLEMENT_BYTE; 
            mux_ptr->adv_escape_flag = FALSE;
        }
        else
        {
            *mux_ptr->end_ptr++ = tmp_char;
        }

        count++;/*valid char number add 1*/

        if (count > MUX_MAX_TOTAL_FRAME_SIZE)
        {
            //MUX_TRACE_WARNING:"MUX Error: frame length:%d is bigger than Max total frame size:%d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_RECV_714_112_2_18_1_49_27_131,(uint8*)"dd", count, (MUX_MAX_TOTAL_FRAME_SIZE));
            mux_ptr->adv_start_flag = FALSE;
            mux_ptr->adv_escape_flag = FALSE;
            mux_ptr->end_ptr = mux_ptr->mux_rev_buf;
            count = 0;
            continue;
        }

        if (MUX_ADVANCE_FLAG == tmp_char)/*find Frame end_flag*/
        {
            /*Receive total MUX frame*/
            crc_error = MUX_Crc_Check((uint8 *) (mux_ptr->mux_rev_buf + ADDRESS_FIELD_OFFSET), 2, *(mux_ptr->end_ptr - 2));/*position of end_ptr -2 is FCS */

            if (!crc_error)/*CRC success*/
            {
                /*In order to portiable for basic mode, We fill length field*/
                SCI_MEMCPY(&mux_pkt_head,  (mux_ptr->mux_rev_buf + ADDRESS_FIELD_OFFSET), 2);
                if ((count-5) > MUX_SHORTFRAME_INF_MAX)
                {
                    /*length filed is 2 bytes*/
                    SET_LONG_LENGTH(mux_pkt_head.length, (count-5));
                }
                else
                {
                    /*length filed is 1 bytes*/
                    mux_pkt_head.length.ea = 1;
                    mux_pkt_head.length.l_len = count-5;
                }

                inf_data_start = &(mux_ptr->mux_rev_buf[MUX_MAX_ADVANCE_HDR_SIZE]);

                MUX_Frame_Parse(&mux_pkt_head, inf_data_start, mux_ptr);
            }
            else
            {
                /* crc_error */
                //MUX_TRACE_WARNING:"MUX: CRC check failure!\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_RECV_750_112_2_18_1_49_27_132,(uint8*)"");
            }   /*End if(!crc_error) */
            if(mux_ptr->mux_rev_buf == NULL)
            {
        	      MUX_TRACE_LOW(("mux_ptr->mux_rev_buf == NULL"));
    	          break;
            }              
            mux_ptr->adv_start_flag = FALSE;
            mux_ptr->adv_escape_flag = FALSE;
            mux_ptr->end_ptr = mux_ptr->mux_rev_buf;
            count = 0;
            continue;
        }/*if(MUX_ADVANCE_FALG == tmp_char)*/
    }/*for end*/

    return;
}



/*****************************************************************************/
// Description :  Parse frame, Parse layer 2
//
// Global resource dependence : none
// Author :    from Amoi
// DATE :      14th July.08
// Note :      
/*****************************************************************************/
LOCAL int MUX_Frame_Parse(MUX_FRAME_HEAD *mux_pkt_head_ptr, uint8 *inf_data_buff, MUX_INFO_T *mux_ptr)
{
    //     MUX_SHORT_FRAME *short_pkt;
    //MUX_LONG_FRAME *long_pkt;
    //uint8 *inf_data_start;
    uint8   dlci;
    //uint8 be_connecting;

    //short_pkt = (MUX_SHORT_FRAME *) data;


    dlci = mux_pkt_head_ptr->addr.dlc_chn;

    switch (CLR_PF(mux_pkt_head_ptr->control))
    {
        case MUX_SABM:
            MUX_SABM_Handler(mux_pkt_head_ptr, inf_data_buff, mux_ptr);
            break;

        case MUX_UA:
            MUX_UA_Handler(mux_pkt_head_ptr, inf_data_buff, mux_ptr);
            break;

        case MUX_DM:
            MUX_DM_Handler(mux_pkt_head_ptr, inf_data_buff, mux_ptr);
            break;

        case MUX_DISC:
            MUX_DISC_Handler(mux_pkt_head_ptr, inf_data_buff, mux_ptr);
            break;

        case MUX_UIH:
            MUX_UIH_Handler(mux_pkt_head_ptr, inf_data_buff, mux_ptr);
            break;

        default:
            //MUX_TRACE_WARNING:"Mux_Frame_Parse: illegal packet in DLCI %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_RECV_812_112_2_18_1_49_27_133,(uint8*)"d", dlci);
            break;
    }
    return 0;
}
/*****************************************************************************/
// Description :    this function is mux_plus_timer timeout function
// Global resource dependence :
// Author :         yayan.xu
// DATE :         2008.07.15
// Note :           
/*****************************************************************************/
PUBLIC void MUX_Check_ThreePlus(uint32 link_id)
{
    /*LINK state change to AT mode from data mode*/
    g_mux_dlc_info[link_id].work_mode = MUX_DLC_AT_MODE;

    return ;
}


