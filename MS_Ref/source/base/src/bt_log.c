/******************************************************************************
 ** File Name:      BT_log.c                                                     *
 ** Author:         Fei Zhang                                              *
 ** DATE:           07/04/2011                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/04/2010     fei.zhang        Add BT USB Log function       *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
#include "chip_plf_export.h"
#include "cmddef.h"
#include "bt_log.h"
#include "ref_param.h"
#include "mem_prod.h"
#include "ref_outport.h"
#include "sfs.h"
#include "priority_system.h"
/*begin add cm4 check bug:620209*/
#ifdef BT_USB_LOG
//#include "Doidleonpage.h"
#endif
/*end add cm4 check bug:620209*/

/**---------------------------------------------------------------------------*
 **                                                 Compiler Flag                                                               *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/*-----------------------------SIO DEBUG MACOR-------------------------------*/

//#define BT_TRACE
#ifdef BT_TRACE
#define SIO_BT_TRACE   SCI_TRACE_LOW
#else
#define SIO_BT_TRACE
#endif
/*lint -save -e551*/



#define BTLOG_DEFAULT_MAX_SIZE (50*1024*1024) /*50M*/

#define  TASK_BT_LOG_PRI   PRI_BT_LOG_TASK   //31

#define  MEM_BTTRACE_STRUCT_ADDR    0x301003D4

//#define  MEM_BTTRACE_SIZE       0x0005000   // psram log size 20K
#define  MEM_BTTRACE_SIZE       0x0004000   // psram log size 16K

#define  MEM_BTTRACE_START_ADDR   (bt_code_addr + bt_code_size - MEM_BTTRACE_SIZE)


/*-----------------------------END--------------------------------------------*/

/**---------------------------------------------------------------------------*
 **  Static Variables & functions
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **  external Variables
 **---------------------------------------------------------------------------*/
extern SCI_MUTEX_PTR   comm_mutex;
extern uint32 s_sio_tx_dma_is_start;
//extern uint32 armLogSendContinueCnt;  //arm log continue send out count,must be cleared to 0 when other log be sent out
extern uint32 BTLOGSendContinueCnt;   //BT log continue send out count,must be cleared to 0 when other log be sent out
extern uint8 *bt_code_addr;   //bt code start address
extern uint32 bt_code_size;    //bt code size
/**---------------------------------------------------------------------------*
 **  Protocol functions
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **  User port variables & functions
 **---------------------------------------------------------------------------*/
//LOCAL uint8  sio_debug_bt_send_buf[DEBUG_PORT_BT_SEND_BUF_SIZE];// Debug port tx buffer
LOCAL  BOOLEAN  is_usb_data_enable = FALSE;  //judge if usb data is enable
LOCAL BLOCK_ID bt_log_thread_id = SCI_INVALID_BLOCK_ID;
PUBLIC SCI_SEMAPHORE_PTR bt_log_semaphore = SCI_NULL;
LOCAL uint32 sioPPPFrameSeqNum = 0;  //BT log frame sequence num
LOCAL  BOOLEAN  is_bt_log_usb_enable = FALSE;  //BT LOG from ARM COM_DEBUG function enable flag
//LOCAL uint8  sio_BTLOG_trans_buf[2048+4] = {0};// BT log trans temp buffer

PUBLIC SIO_BT_LOG_INFO_S *sio_BT_log_headInfo_ptr = SCI_NULL;  //point to BT log head info
PUBLIC SIO_BT_FRAME_CTRL_T    s_sio_bt_frame_ctrl;  //BT log buf information

BOOLEAN b_bt_log_enabled = TRUE;

uint32  sio_BT_log_TPBuffBaseAddr = 0;
uint32  sio_BT_log_TPBuffSize = 0;


LOCAL SFS_HANDLE  sfs_bt_log = NULL; 
LOCAL uint16 BTLOGfile_num = 0, deletefile_num = 0;
LOCAL uint32 BTLOG_saved_size = 0, BTLOG_stepup_size = BTLOG_DEFAULT_MAX_SIZE;
BTLOG_BUF_T BTLOG_buf;

/*begin add cm4 check bug:620209*/
#ifdef BT_USB_LOG
#define BT_CHECK_CM4_TIMER  4
#define  BT_CHECK_DELAY_TIMEOUT       (5000)

LOCAL SCI_TIMER_PTR  s_bt_check_cm4_time_handle = NULL;
static int16 s_bt_check_count = 0;
static uint32 s_bt_check_cm4[BT_CHECK_CM4_TIMER] = {1};
uint32 s_check_timer = 0;
PUBLIC SCI_TIMER_PTR     s_bt_check_delay_timer_ptr = PNULL;
#endif
static BOOLEAN s_bt_check_cm4_open = SCI_FALSE;
/*end add cm4 check bug:620209*/


PUBLIC uint32 *l1c_get_BT_log_adr (void) 
{
    return (SIO_BT_LOG_INFO_S *)MEM_BTTRACE_STRUCT_ADDR;
}

/*****************************************************************************/
// Description: get Frame sequence num for BT log
// Global resource dependence: is_bt_log_usb_enable
// Author: fei.zhang
// Note:
/*****************************************************************************/
LOCAL uint32 SIO_GetPPPFrameSeqNum (void)   //Returns the sequence num
{
    return sioPPPFrameSeqNum++;
}


/*****************************************************************************/
// Description: get BT log from com_debug enable status
// Global resource dependence: is_bt_log_usb_enable
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN  SIO_IsBTLOGUSBEnable (void) //Returns the enbale status
{
    return is_bt_log_usb_enable;
}


/*****************************************************************************/
// Description: set BT log from com_debug enable status
// Global resource dependence: is_bt_log_usb_enable
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void  SIO_BTLOGUSBEnableSet (
    BOOLEAN en  //[IN]set value
)
{
    is_bt_log_usb_enable =  en;
}


/*****************************************************************************/
// Description: BT log ISR handler
// Global resource dependence: bt_log_semaphore
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void SIO_BT_ISR_Handler (
    uint32 int_num  //[IN]interrupt number
)
{
	uint32 * flag_ptr = (uint32 *)(0x301003F0);
	
      *(volatile uint32 *)(0X8B000160) |= BIT_18;  //CLR_CM4_TO_ARM9_IRQ_INT2, Clear IRQ
      
       //SIO_BT_TRACE ("!!!!!!!!!!!!!!!!!!!!!SIO BT ISR!!!!!!!!!!!!!!!!!!!");
       
	if(0x5a5a == *(flag_ptr) & 0xffff)
	{
    	SCI_PASSERT(0, ("CM4 assert"));
	}

	if(TRUE == b_bt_log_enabled)	// If ARM Log On, turn on BT USB LOG. If ARM Log Close, turn off BT USB LOG
	{
    	    SCI_PutSemaphore (bt_log_semaphore);
	}

}


/*****************************************************************************/
// Description: Init  Head Info
// Global resource dependence: sio_BT_log_headInfo_ptr
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void SIO_BT_LOG_Info_Notify (uint32 buff_info_address)
{
    if(SCI_NULL == sio_BT_log_headInfo_ptr)
   {
      sio_BT_log_headInfo_ptr = (SIO_BT_LOG_INFO_S *)(0x80000000 | (buff_info_address & 0x00FFFFFF) ); 
#ifdef BT_TRACE
	SIO_BT_TRACE ("SIO_BT_LOG_Info_Notify:buff info bt addr0x%x,buff info ap addr0x%x",  \
	  	buff_info_address,sio_BT_log_headInfo_ptr);
#endif
      sio_BT_log_TPBuffBaseAddr = (uint32)(0x80000000 | (sio_BT_log_headInfo_ptr->TPBuffBaseAddr & 0x00FFFFFF) );  
      sio_BT_log_TPBuffSize =  sio_BT_log_headInfo_ptr->TPBuffSize;  //DWORD
#ifdef BT_TRACE
      SIO_BT_TRACE ("SIO_BT_LOG_Info_Notify:buff bt addr0x%x,buff ap addr0x%x,buffSize0x%X DWord",  \
	  	sio_BT_log_headInfo_ptr->TPBuffBaseAddr,sio_BT_log_TPBuffBaseAddr,sio_BT_log_TPBuffSize);
#endif
    }

   if(bt_log_semaphore)
      SCI_PutSemaphore(bt_log_semaphore);

}



/*****************************************************************************/
// Description: write usb log to 2K buffer
// Global resource dependence: s_sio_bt_frame_ctrl
// Author: fei.zhang
// Note:
/*****************************************************************************/
LOCAL uint32 SIO_WriteBTFrameToBuf ( //Returns the left data size which have not written to 2K buffer
    uint8 *src, //[IN]log addr
    uint32 size //[IN]log length
)
{
    register int    data_size = 0;

    uint8 head = s_sio_bt_frame_ctrl.head;
    uint8 *buf = s_sio_bt_frame_ctrl.frame_list[head].addr;
    uint16 buf_size = s_sio_bt_frame_ctrl.frame_list[head].length;

    SCI_ASSERT (buf != NULL);/*assert verified*/
    SCI_ASSERT (src != NULL);/*assert verified*/

    if (s_sio_bt_frame_ctrl.count >= s_sio_bt_frame_ctrl.list_size)
    {
        return 0;
    }

    while (size != 0)
    {
        switch (*src)
        {
            case FLAG_BYTE:
            case ESCAPE_BYTE:
                buf[s_sio_bt_frame_ctrl.frame_list[head].pos++] = ESCAPE_BYTE;

                if (s_sio_bt_frame_ctrl.frame_list[head].pos > buf_size - 1)
                {
                    return size;
                }

                buf[s_sio_bt_frame_ctrl.frame_list[head].pos++] = *src ^ COMPLEMENT_BYTE;

                if (s_sio_bt_frame_ctrl.frame_list[head].pos > buf_size - 1)
                {
                    return size;
                }

                data_size += 2;

                break;

            default:
                buf[s_sio_bt_frame_ctrl.frame_list[head].pos++] = *src;

                if (s_sio_bt_frame_ctrl.frame_list[head].pos > buf_size - 1)
                {
                    return size;
                }

                data_size++;
                break;
        }

        src++;
        size--;
    }

    return size;
}


/*****************************************************************************/
// Description: write BT log to 2K buffer
// Global resource dependence: comm_mutex, s_sio_bt_frame_ctrl
// Author: fei.zhang
// Note:
/*****************************************************************************/
LOCAL uint32 SIO_WriteBTFrame (//Returns the data size which have been written to BT 2K Log Buffer
    uint8 *src,             //[IN]src-log addr
    uint32 size         //[IN]log length
)
{
    uint32 write_left_size = 0;
    uint8  head = 0;
    uint8 *buf = 0;
    uint16 buf_size =  0;
    uint16 start = 0;

    head = s_sio_bt_frame_ctrl.head;
    buf = s_sio_bt_frame_ctrl.frame_list[head].addr;
    buf_size =  s_sio_bt_frame_ctrl.frame_list[head].length;
    start = 0;

    SCI_ASSERT (s_sio_bt_frame_ctrl.count <= s_sio_bt_frame_ctrl.list_size);/*assert verified*/
    SCI_ASSERT (buf != NULL);/*assert verified*/

    //SIO_BT_TRACE("!!!SIO  BT  a_ctu = %d, a_cnt = %d, d_ctu = %d ,d_cnt = %d",armLogSendContinueCnt,s_sio_frame_ctrl.count,BTLOGSendContinueCnt,s_sio_bt_frame_ctrl.count);

    //When the space is FULL, then throw it away
    if (s_sio_bt_frame_ctrl.count >= s_sio_bt_frame_ctrl.list_size)
    {
        //send log quickly!!
        if (comm_mutex)
        {
            SCI_GetMutex (comm_mutex,  SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);
        }

        SIO_SendData();

        if (comm_mutex)
        {
            SCI_PutMutex (comm_mutex);
        }

        return 0;
    }

    s_sio_bt_frame_ctrl.frame_list[head].pos = 0;

    buf[s_sio_bt_frame_ctrl.frame_list[head].pos++] = FLAG_BYTE;  //start

    write_left_size = SIO_WriteBTFrameToBuf (src,size);

    if (write_left_size > 0)   //frame buffer is full,return  encoded size
    {
#ifdef BT_TRACE
        SIO_BT_TRACE ("!!!BT LOG Left > 0");
#endif
        return size - write_left_size;
    }

    SCI_MEMSET ( ( (uint8 *) &buf[s_sio_bt_frame_ctrl.frame_list[head].pos]),\
                 FLAG_BYTE, \
                 (buf_size - s_sio_bt_frame_ctrl.frame_list[head].pos));

    return  size;
}


/*****************************************************************************/
// Description: BT log send out Triger in usb DMA mode
// Global resource dependence: comm_mutex, s_sio_bt_frame_ctrl
// Author: fei.zhang
// Note:
/*****************************************************************************/
LOCAL void  SIO_IsBTLogFrameFullSendOut (
    volatile uint32 *readidx_ptr,   //[IN]addr in share memory
    uint32 read_idx                 //[IN]set value
)
{
    if (comm_mutex)
    {
        SCI_GetMutex (comm_mutex,  SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);
    }

    /*must disable irq*/
    SCI_DisableIRQ();
    s_sio_bt_frame_ctrl.count++;
    s_sio_bt_frame_ctrl.head++ ;

    if (s_sio_bt_frame_ctrl.head >= s_sio_bt_frame_ctrl.list_size)
    {
        s_sio_bt_frame_ctrl.head = 0;
    }

    *readidx_ptr = read_idx;   //modify idx in share memory
    
    SCI_RestoreIRQ();

    SIO_SendData();   

    if (comm_mutex)
    {
        SCI_PutMutex (comm_mutex);
    }
}


/*****************************************************************************/
// Description: Check BT log frame buffer is full or not
// Global resource dependence: s_sio_bt_frame_ctrl
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN SIO_IsSIO_IsBTLogFrameFullFrameFull (void)     // If BT Log Frame full, returns TRUE
// else returns FALSE
{
    if (s_sio_bt_frame_ctrl.count >= s_sio_bt_frame_ctrl.list_size)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



LOCAL uint16 _gb2ucs(					//return the number of UCS2 code that have been converted.
						uint16 *ucs2_ptr, 	//destination: string pointer to UCS2 string.
						const uint8 *gb_ptr,//source: string pointer to GB string
						uint16 len			//the length to convert from GB to UCS2.
						)
{
    uint16 tmp_gb, count = 0;

    SCI_ASSERT(PNULL != gb_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != ucs2_ptr); /*assert verified*/

    while((len > 0) && (len < 0xfff))
    {
        len--;
        count += 2;
        tmp_gb = (uint16)(*gb_ptr++);

        if (tmp_gb < 0x80)	//An ASCII
        {
            *ucs2_ptr++ = tmp_gb;
        }
    }

    return count;
}

LOCAL BOOLEAN _CreatFile(
                SFS_HANDLE*     fd_ptr,
                uint16*         filename,
                uint16*         device_name
                )
{
    SFS_ERROR_E device_result = SFS_ERROR_NO_SPACE;
    BOOLEAN return_result = FALSE;  
        
    device_result = SFS_GetDeviceStatus(device_name);   

    if(SFS_ERROR_NONE == device_result)
    {       
        *fd_ptr = SFS_CreateFile(filename, SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, NULL, NULL);/*lint !e655*/
           
        if(NULL != *fd_ptr)     
        {
#ifdef BT_TRACE
            SIO_BT_TRACE("BTLOG: create file success!");
#endif
            return_result = TRUE;
        }
        else
        {
#ifdef BT_TRACE
            SIO_BT_TRACE("BTLOG: create file fail!");
#endif
            return_result = FALSE;
        }      
    }    
    else
    {
#ifdef BT_TRACE
        SIO_BT_TRACE("BTLOG: device status(%d) error!", device_result);
#endif
        return_result = FALSE;
    }     
    
    return return_result;
}

LOCAL BOOLEAN _CreatBTLOGFile( uint16 logfile_num)
{
    BOOLEAN        ret_create_file;
    char        BT_log_file_name_gb[SFS_MAX_PATH+1] ={0};
    uint16      BT_log_file_name[SFS_MAX_PATH+1] ={0};
    uint16      BT_log_file_name_len = 0;
    uint16      tcard_name[2] = {'E', 0};    

	BT_log_file_name_len = sprintf(BT_log_file_name_gb,"E:\\BTLOG%04d.log",(uint)logfile_num);
	_gb2ucs(BT_log_file_name,  (uint8 *) BT_log_file_name_gb, BT_log_file_name_len);
#ifdef BT_TRACE
	SIO_BT_TRACE("BTLOG: start to save BTLOG in sdCard ,logfile name is %s", BT_log_file_name_gb); 
#endif
	/* create new log file in sfs, original file */        
	ret_create_file = _CreatFile(&sfs_bt_log,BT_log_file_name,tcard_name);		

	if(FALSE == ret_create_file)      
	{
#ifdef BT_TRACE
	    SIO_BT_TRACE("BTLOG: create file %s fail, finish_time %d",BT_log_file_name_gb, SCI_GetTickCount());
#endif
	    return FALSE;      
	}  
#ifdef BT_TRACE
    SIO_BT_TRACE("BTLOG: create file suc, finish_time %d", SCI_GetTickCount());
#endif
    return TRUE;
}


/* step:50M, BTLOGfile_num++, and create file  */
LOCAL BOOLEAN _WriteFile_Stepupfile(void)
{
        BOOLEAN ret;

        SFS_CloseFile( sfs_bt_log);  

        BTLOGfile_num++;
        ret = _CreatBTLOGFile(BTLOGfile_num);

        if(ret)   
        {   
            REFPARAM_SetBtCardLogNum(BTLOGfile_num);
            return TRUE;       
        }
        else
        {
        	return FALSE;	
        }
}

LOCAL BOOLEAN _WriteFile_DelFile(void)
{
    SFS_ERROR_E     file_ret = SFS_ERROR_NONE;
    char        file_name_gb[SFS_MAX_PATH+1] ={0};
    uint16      file_name[SFS_MAX_PATH+1] ={0};
    uint16      file_name_len = 0;	

 	for( ;deletefile_num <= BTLOGfile_num; )
	{		
		if( deletefile_num == BTLOGfile_num )	
		{
			/* Close log file in sfs */
	    	SFS_CloseFile( sfs_bt_log); 
			/* reset BTLOG_saved_size,or stepup one time() */
			BTLOG_saved_size = 0;
		}	
				
		file_name_len = sprintf(file_name_gb,"E:\\BTLOG%04d.Log",(uint)deletefile_num);	
		_gb2ucs(file_name,  (uint8 *) file_name_gb, file_name_len); 	
	
		file_ret = SFS_DeleteFile( file_name,PNULL );	
		if( SFS_ERROR_NONE == file_ret )
		{
#ifdef BT_TRACE
			SIO_BT_TRACE("BTLOG: _WriteFile_DelFile, del(%s) ok",file_name);
#endif
			if( deletefile_num == BTLOGfile_num )	
			{
                        BTLOGfile_num++;
                        /*recreate log file in sfs*/
                        _CreatBTLOGFile( BTLOGfile_num ); 
                        REFPARAM_SetBtCardLogNum(BTLOGfile_num);
			}	
			deletefile_num++;				
			return TRUE;			
		}
		else
		{
			deletefile_num++;
		}	
	}

#ifdef BT_TRACE
 	SIO_BT_TRACE("BTLOG: _WriteFile_DelFile, del file fail,cur num(%d)",BTLOGfile_num);
#endif
	return FALSE;	

}

LOCAL BOOLEAN _WriteBTLOGFile(
                uint8*              write_buff_ptr,
                uint32              write_length
                )
{
    SFS_ERROR_E         result = SFS_ERROR_NO_SPACE;
    uint32              wrtlen;
    BOOLEAN          return_result = FALSE;

    result = SFS_WriteFile(sfs_bt_log, (void*)write_buff_ptr, write_length, &wrtlen, PNULL);

    switch( result )
    {
        case SFS_ERROR_NONE:/*0:success*/
        {
        	BTLOG_saved_size += write_length;
        	
        	if( BTLOG_saved_size >= BTLOG_stepup_size )
        	{		
#ifdef BT_TRACE
        		SIO_BT_TRACE("BTLOG: stepupfile, BTLOG_saved_size(%d),BTLOG_stepup_size(%d)",BTLOG_saved_size,BTLOG_stepup_size);/* cr208045 */
#endif
        		/* step of save size is 50M */
        		BTLOG_saved_size = 0;
        		return_result  = _WriteFile_Stepupfile();
        		if( FALSE == return_result )
        		{				
        			/* turn off saving */
        			SFS_CloseFile( sfs_bt_log); 
        			return_result = FALSE; 
        		}	
        		else
        		{				    
        	    	    return_result = TRUE; 			
        		}						
        	}	
        	else
        	{
            	    return_result = TRUE; 			
        	}			
            break ;
        }  
        
        case SFS_ERROR_NO_SPACE:/*11:sd card is full,del 50M,*/
        {
#ifdef BT_TRACE
        	SIO_BT_TRACE("BTLOG: _WriteBTLOGFile,ret = SFS_ERROR_NO_SPACE");
#endif
        	return_result  = _WriteFile_DelFile();
        	if( FALSE == return_result )
        	{
        		/* just turn off saving, tbd more better */
        		SFS_CloseFile( sfs_bt_log); 
        		return_result = FALSE; 
        		break;				
        	}
        	else
        	{
        		return_result = _WriteBTLOGFile(BTLOG_buf.buf, BTLOG_buf.buf_len); 
        		if( FALSE == return_result )
        		{	
        			SCI_PASSERT(0, ("BTLOG: _WriteBTLOGFile, rewrite fail, fatal err!!!"));   /*assert verified*/
        		}
        		return_result = TRUE; 
        	}
            break ;
        } 	
        
        case SFS_ERROR_DEVICE:/*1: device error*/
        case SFS_ERROR_INVALID_PARAM:/*3: param you give is invalid*/
        {
            /*In UDisk mode, SFS will be unmounted, and return 3 */
#ifdef BT_TRACE
            SIO_BT_TRACE("BTLOG: _WriteBTLOGFile, result = %d",result);
#endif
            SFS_CloseFile( sfs_bt_log);     
            return_result = FALSE; 
            break;
        }
        
        default:
        {
#ifdef BT_TRACE
            SIO_BT_TRACE("BTLOG: _WriteBTLOGFile, result = %d",result);
#endif
            SFS_CloseFile( sfs_bt_log);  
            return_result = FALSE;   
            break ;
        } 				
    }	 
   
    return return_result;
}

/*****************************************************************************/
// Description: enable BT log save to T card
// Global resource dependence: BTLOGfile_num, sfs_bt_log
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN SIO_BTCardLogOpen (void)
{
    BOOLEAN ret = FALSE;

    if(sfs_bt_log)
    {
        SFS_CloseFile( sfs_bt_log);
        sfs_bt_log = NULL;
    }

    BTLOGfile_num = REFPARAM_GetBtCardLogNum() + 1;
    ret = _CreatBTLOGFile(BTLOGfile_num);
    if(!ret)
    {
        sfs_bt_log = NULL;
#ifdef BT_TRACE
        SIO_BT_TRACE ("BTLOG: Open BT Log file fail!");
#endif
        return FALSE;
    }

#ifdef BT_TRACE
    SIO_BT_TRACE ("BTLOG: Open BT Log file success!");
#endif

    REFPARAM_SetBtCardLogNum(BTLOGfile_num);
    REFPARAM_SetBtCardLogFlag(TRUE);

    return TRUE;
}

/*****************************************************************************/
// Description: disable BT log save to T card
// Global resource dependence: sfs_bt_log
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN SIO_BTCardLogClose (void)
{    
    SFS_CloseFile( sfs_bt_log);
    sfs_bt_log = NULL;

#ifdef BT_TRACE
    SIO_BT_TRACE ("BTLOG: Close BT Log file success!");
#endif
    REFPARAM_SetBtCardLogFlag(FALSE);

    return TRUE;
}

/*begin add cm4 check bug:620209*/
#if 0 //def BT_USB_LOG
/*****************************************************************************/
// Description:check statusFlag
// Global resource dependence: 
// Author:
// Note:
/*****************************************************************************/
LOCAL BOOLEAN  SIO_BTCheckStatusFlagEq (void)
{
    uint16 i = 0;
    
    for(i=0;i<BT_CHECK_CM4_TIMER;i++)
    {
        if(sio_BT_log_headInfo_ptr->statusFlag != s_bt_check_cm4[i])
        {
            if((sio_BT_log_headInfo_ptr->statusFlag != s_bt_check_cm4[0]))
            {
                 s_check_timer = 0;
            }
            return SCI_FALSE;
        }
        else if((BT_CHECK_CM4_TIMER-1) <= i)
        {
            return SCI_TRUE;
        }
        else if((sio_BT_log_headInfo_ptr->statusFlag == s_bt_check_cm4[0]) && (0 == s_check_timer))
        {
            s_check_timer = SCI_GetTickCount();
        }
    }
}

/*****************************************************************************/
// Description:check cm4 status
// Global resource dependence: 
// Author:
// Note:
/*****************************************************************************/
LOCAL void  SIO_BTCheckCm4Status (void)
{        
    uint32 tick_tmp = 0;
    
    tick_tmp = SCI_GetTickCount();
    SCI_TRACE_LOW("SIO_BTCheckCm4Status start statusFlag:0x%x,timer",sio_BT_log_headInfo_ptr->statusFlag,tick_tmp);
    
    if(!s_bt_check_cm4_open)
    {
        return;
    }

    //SCI_TRACE_LOW("SIO_BTCheckCm4Status11111 s_check_timer:%d,s_bt_check_count:%d",s_check_timer,s_bt_check_count);

    s_bt_check_count++;

    if(!SIO_BTCheckStatusFlagEq() && (s_bt_check_count <= BT_CHECK_CM4_TIMER))
    {
       // SCI_TRACE_LOW("SIO_BTCheckCm4Status statusFlag:0x%x,tick_count:%d,s_check_timer:%d",sio_BT_log_headInfo_ptr->statusFlag,(tick_tmp - s_check_timer),s_check_timer);
        s_bt_check_cm4[s_bt_check_count-1] = sio_BT_log_headInfo_ptr->statusFlag;
    }
    else if(SIO_BTCheckStatusFlagEq() && s_bt_check_count > BT_CHECK_CM4_TIMER)
    {
        //CM4 EXCEPTION
        SCI_PASSERT(0, ("cm4 exception!statusFlag:0x%x,tick_count:%d",sio_BT_log_headInfo_ptr->statusFlag,(tick_tmp - s_check_timer+BT_CHECK_DELAY_TIMEOUT)));/*assert to do*/
    }

    tick_tmp = SCI_GetTickCount();
    SCI_TRACE_LOW("SIO_BTCheckCm4Status endstatusFlag:0x%x,timer",sio_BT_log_headInfo_ptr->statusFlag,tick_tmp);
    
    if(s_bt_check_count > BT_CHECK_CM4_TIMER)
    {
        s_bt_check_count = 0;
    }

    if(s_bt_check_delay_timer_ptr)
    {
        SCI_ChangeTimer(s_bt_check_delay_timer_ptr, SIO_BTCheckCm4Status, BT_CHECK_DELAY_TIMEOUT);
        SCI_ActiveTimer(s_bt_check_delay_timer_ptr);
    }
}

/*****************************************************************************/
// Description: start check cm4 status
// Global resource dependence: 
// Author:
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN SIO_BTOpenCheckCm4Status (BOOLEAN open)
{  
    SCI_TRACE_LOW("SIO_BTOpenCheckCm4Status %d",open);
    
    s_bt_check_cm4_open = open;
    
    if(open && s_bt_check_delay_timer_ptr)
    {
        SCI_TRACE_LOW("SIO_BTOpenCheckCm4Status active timer");
        SCI_ChangeTimer(s_bt_check_delay_timer_ptr, SIO_BTCheckCm4Status, BT_CHECK_DELAY_TIMEOUT);
        SCI_ActiveTimer(s_bt_check_delay_timer_ptr);
    }
    else if(s_bt_check_delay_timer_ptr)
    {
        SCI_TRACE_LOW("SIO_BTOpenCheckCm4Status close timer");
        SCI_DeactiveTimer(s_bt_check_delay_timer_ptr);
    }
}
/*end add cm4 check bug:620209*/

#endif



/*****************************************************************************/
// Description: BT log process Task
// Global resource dependence: sio_BTLOG_trans_buf, bt_log_semaphore, sio_BT_log_headInfo_ptr
// Author: fei.zhang
// Note:
/*****************************************************************************/
LOCAL void SIO_BTLogThread (uint32 id, void *pdata)
{
    //uint32 i = 0;
    uint32 tpReadIdx = 0;
    uint32 tpWriteIdx = 0;
    //uint32 BTLOGStatusFlag = 0;
    uint32 tpDataSize = 0;
    uint32 tpBuffSize = 0;
    uint32 tpBuffBaseAddr = 0;

    //uint32 status = 0;
    uint32 size  = 0;
    uint32 seqNum = 0;
    uint32 readIdx_tmp = 0;
    BOOLEAN isRetry = FALSE;
    uint32 already_write_size = 0;
    uint8 *transBuffAddr = NULL;
    uint32  BTLOGFrameHeadLen = 0;
    BTLOG_FRM_HEAD_T  *BTLOGFrmHead_ptr;
    BOOLEAN ret = FALSE;
    uint16  frame_list_num = 0;
    uint8 *tmp_buf = NULL;

    //Initalize BT PPP Frame Control
    s_sio_bt_frame_ctrl.count = 0;
    s_sio_bt_frame_ctrl.head = 0;
    s_sio_bt_frame_ctrl.tail = 0;
    s_sio_bt_frame_ctrl.list_size = MAX_SIO_BT_PPP_FRAME_SIZE;

    for (frame_list_num = 0; frame_list_num < s_sio_bt_frame_ctrl.list_size; frame_list_num++)
    {
        s_sio_bt_frame_ctrl.frame_list[frame_list_num].addr = (uint8 *) SCI_ALLOC(MAX_SIO_BT_PPP_FRAME_BUFF_SIZE); // (uint8 *) &sio_debug_bt_send_buf[frame_list_num*MAX_SIO_BT_PPP_FRAME_BUFF_SIZE];
        if(s_sio_bt_frame_ctrl.frame_list[frame_list_num].addr == NULL) // alloc fail?
            SCI_ASSERT(0);
        s_sio_bt_frame_ctrl.frame_list[frame_list_num].length = MAX_SIO_BT_PPP_FRAME_BUFF_SIZE;
        s_sio_bt_frame_ctrl.frame_list[frame_list_num].pos = 0;
    }
    
    tmp_buf =  (uint8 *) SCI_ALLOC(MAX_SIO_BT_PPP_FRAME_BUFF_SIZE+4);
    if(tmp_buf == NULL)
        SCI_ASSERT(0);
    transBuffAddr = (uint8 *) ( ( (uint32) (tmp_buf + 3) >>2) <<2);
    
    if(REFPARAM_GetBtCardLogFlag())
    {
        BTLOGfile_num = REFPARAM_GetBtCardLogNum() + 1;
        ret = _CreatBTLOGFile(BTLOGfile_num);
        if(!ret)
        {
            sfs_bt_log = NULL;
#ifdef BT_TRACE
            SIO_BT_TRACE ("BTLOG: Open BT Log file fail!");
#endif
        }
        REFPARAM_SetBtCardLogNum(BTLOGfile_num);
    }
 
    for (; ;)
    {
        SCI_GetSemaphore (bt_log_semaphore,SCI_WAIT_FOREVER); //SCI_WAIT_FOREVER
#ifdef BT_TRACE
        SIO_BT_TRACE ("*********************Get bt_log_semaphore*****************");
#endif
        /******************************************************************
        **   |free        |-> baseAddr
        **   |free        |
        **   |reserve   |->readIdx
        **   |data 0     |->first data to be read
        **   |data 1     |
        **      .
        **      .
        **      .
        **   |data n-1 |
        **   |data n     |
        **   |free        |->writeIdx ,first free space could be Write
        **   |free        |
        **   |free        |
        **   reserve data is used for checking the buffer is full or empty
        **  when writeIdx==readIdx,buffer is full
        **      readIdx + 1== writeIdx,buffer is empty
        **  in BT data uint is DWORD(4 bytes), the index unit is DWORD also
        **  in ARM DATA Process is byte by byte
        ***********************************************************************/

        tpBuffSize = sio_BT_log_TPBuffSize << BT_LOG_IDX_TYPE;
        tpBuffBaseAddr = sio_BT_log_TPBuffBaseAddr;

        tpReadIdx = ( (sio_BT_log_headInfo_ptr->TPRead_idx + 1) << BT_LOG_IDX_TYPE) % tpBuffSize; // Read index must +1, then point to the first data to be read
        tpWriteIdx = (sio_BT_log_headInfo_ptr->TPWrite_idx) << BT_LOG_IDX_TYPE;
        //BTLOGStatusFlag = sio_BT_log_headInfo_ptr->statusFlag;

        tpDataSize = (tpWriteIdx + tpBuffSize - tpReadIdx) % tpBuffSize;
#ifdef BT_TRACE
        SIO_BT_TRACE ("!!!!SIO_IsBTLogFrameFullThread  TPRead_idx = 0x%x TPWrite_idx = 0x%x, tpsize = %dbytes", sio_BT_log_headInfo_ptr->TPRead_idx , sio_BT_log_headInfo_ptr->TPWrite_idx, tpDataSize);
#endif

        /*TP data*/
        while ((tpDataSize > 0)
                && (tpDataSize >= BT_LOG_FRAME_BUFF_GET_SIZE))  //need to consider!
        {
            size = MIN (tpDataSize, BT_LOG_FRAME_BUFF_GET_SIZE);

            if(REFPARAM_GetBtCardLogFlag())
            {
#ifdef BT_TRACE
                SIO_BT_TRACE ("BTLog:save to SD card");
#endif
                /*copy tpdata into tans buffer*/
                if (tpReadIdx + size > tpBuffSize)
                {
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr), (uint8 *) (tpBuffBaseAddr+tpReadIdx),tpBuffSize-tpReadIdx);
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr+tpBuffSize-tpReadIdx), (uint8 *) tpBuffBaseAddr,size + tpReadIdx -tpBuffSize);
                }
                else
                {
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr), (uint8 *) (tpBuffBaseAddr+tpReadIdx),size);
                }

                BTLOG_buf.buf = transBuffAddr;
                BTLOG_buf.buf_len = size;

                _WriteBTLOGFile(transBuffAddr, size);

                readIdx_tmp= ( (tpReadIdx + tpBuffSize + size - (1 << BT_LOG_IDX_TYPE)) % tpBuffSize) >> BT_LOG_IDX_TYPE;
                sio_BT_log_headInfo_ptr->TPRead_idx = readIdx_tmp;
                tpReadIdx = (tpReadIdx + size) % tpBuffSize;
                tpDataSize -= size;                       
            }
            else            
            {
                if (SIO_IsSIO_IsBTLogFrameFullFrameFull())
                {
                    break;
                }

                if (SCI_GetArmLogFlagTemp() == SCI_FALSE)
                {
                    readIdx_tmp= ( (tpReadIdx + tpBuffSize + size - (1 << BT_LOG_IDX_TYPE)) % tpBuffSize) >> BT_LOG_IDX_TYPE;
                    sio_BT_log_headInfo_ptr->TPRead_idx = readIdx_tmp;
                    tpReadIdx = (tpReadIdx + size) % tpBuffSize;
                    tpDataSize -= size;
                    continue;
                }
#ifdef BT_TRACE
               SIO_BT_TRACE ("BTLog:use PPP send");
#endif
                seqNum = SIO_GetPPPFrameSeqNum();

                BTLOGFrameHeadLen = sizeof (BTLOG_FRM_HEAD_T);
                //frame head
                BTLOGFrmHead_ptr = (BTLOG_FRM_HEAD_T *) transBuffAddr;
                BTLOGFrmHead_ptr->seq_num = seqNum;
                BTLOGFrmHead_ptr->len = size + BTLOGFrameHeadLen;
                BTLOGFrmHead_ptr->type =DIAG_MOBILETV_DSP_LOG_F;
                BTLOGFrmHead_ptr->subtype = BT_LOG_SUBTYPE_TP;

                /*copy tpdata into tans buffer*/
                if (tpReadIdx + size > tpBuffSize)
                {
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr+BTLOGFrameHeadLen), (uint8 *) (tpBuffBaseAddr+tpReadIdx),tpBuffSize-tpReadIdx);
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr+BTLOGFrameHeadLen+tpBuffSize-tpReadIdx), (uint8 *) tpBuffBaseAddr,size + tpReadIdx -tpBuffSize);
                }
                else
                {
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr+BTLOGFrameHeadLen), (uint8 *) (tpBuffBaseAddr+tpReadIdx),size);
                }

                size += BTLOGFrameHeadLen;

                do
                {

                    already_write_size = SIO_WriteBTFrame (\
                                                            transBuffAddr, \
                                                            size);
                    if (already_write_size == 0)
                    {
                        isRetry = FALSE;
                    }
                    else if (already_write_size < size)
                    {
#ifdef BT_TRACE
                        SIO_BT_TRACE ("!!!BT LOG tp retry");
#endif
                        /*if Packet the data fail, half the size, packet again*/
                        size = ( ( (size - BTLOGFrameHeadLen) >>3) <<2) + BTLOGFrameHeadLen;
                        BTLOGFrmHead_ptr->len = size;
                        isRetry = TRUE;
                    }
                    else
                    {
#ifdef BT_TRACE
                        SIO_BT_TRACE ("!!!BT LOG tp success");
#endif
                        readIdx_tmp= ( (tpReadIdx + tpBuffSize + size - BTLOGFrameHeadLen - (1 << BT_LOG_IDX_TYPE)) % tpBuffSize) >> BT_LOG_IDX_TYPE;
                        SIO_IsBTLogFrameFullSendOut (&sio_BT_log_headInfo_ptr->TPRead_idx,readIdx_tmp);
                        tpReadIdx = (tpReadIdx + size - BTLOGFrameHeadLen) % tpBuffSize;
                        tpDataSize -= size - BTLOGFrameHeadLen;
                        isRetry = FALSE;
                    }
                }
                while (isRetry);
            }
        } 

    }

    // free memory if it can reach here
    for (frame_list_num = 0; frame_list_num < s_sio_bt_frame_ctrl.list_size; frame_list_num++)
    {
        if(s_sio_bt_frame_ctrl.frame_list[frame_list_num].addr != NULL)
        {
            SCI_FREE(s_sio_bt_frame_ctrl.frame_list[frame_list_num].addr); 
            s_sio_bt_frame_ctrl.frame_list[frame_list_num].addr = NULL;
        }
    }
    if(tmp_buf != NULL)
    {
        SCI_FREE(tmp_buf);
        tmp_buf = NULL;
    }
}


/*****************************************************************************/
// Description: BT log task creat
// Global resource dependence: bt_log_semaphore, bt_log_thread_id, s_sio_bt_frame_ctrl
// Author: fei.zhang
// Note:
/*****************************************************************************/

PUBLIC void SIO_CreateBTLOGThread (void)
{
    if (SCI_NULL == bt_log_semaphore)
    {
        bt_log_semaphore = SCI_CreateSemaphore ("bt_log_semaphore", 0);
    }

    SCI_PASSERT ( (bt_log_semaphore != SCI_NULL), ("Create bt_log_semaphore failed!"));/*assert verified*/

    if (SCI_INVALID_BLOCK_ID == bt_log_thread_id)
    {
        bt_log_thread_id = SCI_CreateThread ("BT_LOG_Task", "BT_LOG_QUEUE",
                                              SIO_BTLogThread/*void(* entry)(uint32,void *)*/,
                                              0, NULL, 10240, 10,
                                              PRI_BT_LOG_TASK,  // same priority as sme
                                              SCI_PREEMPT,
                                              SCI_AUTO_START);
    }

/*begin add cm4 check bug:620209*/
#if 0 //def BT_USB_LOG
    if (!s_bt_check_delay_timer_ptr)
    {
        s_bt_check_delay_timer_ptr = SCI_CreateTimer ("bt check cm4 status timer",\
                                                                SIO_BTCheckCm4Status,\
                                                                0, 
                                                                BT_CHECK_DELAY_TIMEOUT,
                                                                SCI_NO_ACTIVATE);
    }
#endif
/*end add cm4 check bug:*/

    SCI_PASSERT ( (bt_log_thread_id != SCI_INVALID_BLOCK_ID), ("Create BT LOG task failed!"));/*assert verified*/
#ifdef BT_TRACE
    SIO_BT_TRACE ("!!!Create SIO_SIO_IsBTLogFrameFullThread successfully!!!");
#endif
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif /* End of sio.c*/
