/******************************************************************************
 ** File Name:      intel_m18_drv.c                                            *
 ** Author:         Younger.yang                                              *
 ** DATE:           12/10/2006                                                *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 Intel flash Low-level drvier.                             * 
 **                 This driver support ALL INTEL/ST flash                    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12-10-2006     lin.liu          Create.                                   *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "os_api.h"
#include "arm_reg.h"
#include "tb_hal.h"
#include "sc_reg.h"
#include "flash.h"         
#include "emc_drv.h"
#include "intel_m18_drv.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


/**---------------------------------------------------------------------------*
 **              macro define                                             *
 **---------------------------------------------------------------------------*/
#define M18_REGION_SIZE     1024 //unit:byte
#define M18_MODE_DIFF_ADDR  0x10

/**---------------------------------------------------------------------------*
 **              variable declare                                             *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Global Variable                                   *
 **---------------------------------------------------------------------------*/

//LOCAL uint32 s_flash_init_flag = 0x0;
//LOCAL uint32 s_flash_flash_size;
//LOCAL uint32 s_flash_bank_num;
LOCAL uint32 s_flash_sector_size;
LOCAL uint32 s_flash_sector_num;
//LOCAL uint32 s_flash_wb_len;
//LOCAL uint32 s_flash_wb_flag;
LOCAL uint32 s_fixed_nv_start_addr;
LOCAL uint32 s_efs_start_addr;
LOCAL uint32 s_product_info_addr;

LOCAL FLASH_PTR_TYPE s_flash_erasing_addr;

LOCAL uint32 flash_store_optr_array[2];

LOCAL uint32 * flash_store_optr_ptr;

// store the current interrupt flag
LOCAL uint32 s_flash_interrupt_save;

//LOCAL uint32 s_flash_timing_save;

#define STORE_INTEL_OPTR(optr)    STORE_FLASH_OPTR(optr , flash_store_optr_ptr)
#define GET_INTEL_STORE_OPTR      GET_FLASH_STORE_OPTR(flash_store_optr_ptr)

// NON-zero indicate need monitor interrupt flag and handle it while erasing/programing.
// Zero     indicate needn't monitor the interrupt flag
LOCAL BOOLEAN      b_need_monitor_int = 0;

LOCAL FLASH_TRACE_INFOR_T  flash_trace_infor;/*lint -esym(551, flash_trace_infor)*/

//Intel driver struct.
const NOR_FLASH_DRIVER_T  g_Intel_m18_driver =
{
    INTEL_Init,
    INTEL_ReadWord,
    INTEL_Read,
    INTEL_WriteWord,
    INTEL_Write,
    INTEL_Erase,
    INTEL_IsEraseCompleted,
    INTEL_Suspend,
    INTEL_Resume,
    INTEL_Unlock,
    INTEL_Lock,
    INTEL_EnterRead,
    INTEL_ExitRead,
    INTEL_Close,
    INTEL_Config,
    INTEL_EfsAddrConvert
};

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

#define MAX_TIMER_CNT          0xFFFF
#define TIMER_LOAD_DELAY    50
//#define REG32(x)    (*(volatile uint32 *)(x))
#define TIMER1_TICK_COUNT               32      // us   
#define TIMER1_INT_COUNT                0x7fffff   

LOCAL uint32 __time1_init(void)
{
    uint32 cnt = 0;
    while(REG32(TM1_CLR) & BIT_4)  // if busy, then wait for a while
    {
        cnt++;
        if(cnt > MAX_TIMER_CNT)
        {
            break;  // timeout and break
        }
    }
    REG32(TM1_LOAD) = TIMER1_INT_COUNT;    // timer load

    cnt = 0;
    while(REG32(TM1_CLR) & BIT_4)  // if busy, then wait for a while
    {
        cnt++;
        if(cnt > MAX_TIMER_CNT)
        {
            break;  // timeout and break
        }
    }
    // Delay 50us for timer1 load value
    FLASH_DELAY_US(TIMER_LOAD_DELAY);
    
    REG32(TM1_CTL) |= BIT_7;          // timer start    

    return SCI_TRUE;
}

LOCAL uint32 __time1_get_value(void)
{
    volatile uint32 tmp_tick1;
    volatile uint32 tmp_tick2;

    tmp_tick1 = REG32(TM1_VALUE);
    tmp_tick2 = REG32(TM1_VALUE);

    while (tmp_tick1 != tmp_tick2)
    {
        tmp_tick1 = tmp_tick2;
        tmp_tick2 = REG32(TM1_VALUE);
    }

    return tmp_tick1;     
}

 LOCAL uint32 INTEL_EfsAddrConvert(uint32 addr)
{
    uint32 temp_addr;

    if(FLASH_M18_Type_Get())
    {
        if((addr >=s_efs_start_addr) && (addr < s_efs_start_addr + s_flash_sector_num*s_flash_sector_size))
        {

            if(addr & M18_MODE_DIFF_ADDR)
            {
                temp_addr = M18_MODE_DIFF_ADDR;
                addr = (addr+s_flash_sector_size*s_flash_sector_num) & (~temp_addr);
            }	

        }
        else
        {

        }
    }
    return addr;
}


/*****************************************************************************/
// Description :    This function is used to init Intel flash
// Global resource dependence :
// Author :         Nick.Zhao
// Note :           
/*****************************************************************************/  
LOCAL void INTEL_Init(void)  /* If Init success, return true, else return false */
{	

    FLASH_SAVE_TRACE_INFOR_INIT;

    //s_flash_init_flag = FLASH_INIT_MAGIC;

    flash_store_optr_ptr = flash_store_optr_array;

    return;
}


/*****************************************************************************/
// Description :    This function is used to close Intel flash
// Global resource dependence :
// Author :         Nick.Zhao
// Note :           it is called when power down
/*****************************************************************************/
LOCAL void INTEL_Close(void)
{
	//s_flash_init_flag = 0;
}


LOCAL int INTEL_GetPollStatus( FLASH_PTR_TYPE  addr_ptr )
{
    volatile uint16  tmp;
    
    FLASH_WR(addr_ptr ,TMPL_READ_STATUS_REGISTER);
        
    tmp = FLASH_RD(addr_ptr);        
               
    if(!mIsStatusReady(tmp))
    {
        return FLASH_OPTR_STATUS_RUNNING;
        // check again
    }
    else
    {
        // not toggle, has complete

        //check for any errors

        if(tmp&0x001A)
        {
            if(tmp&0x0008)
                {
                }
            else if(tmp&0x0002)
                {
                }
            else if((tmp&0x0010)&&(tmp&0x0020))
                {
                }
            else
                {
                }

            FLASH_WR(addr_ptr,0x0050);//Clear Status Register

            return FLASH_OPTR_STATUS_FAILED;
        }
        else
        {
            return FLASH_OPTR_STATUS_COMPLETED;
        }
    }
}

LOCAL uint32 _get_current_system_clock(void)
{
    volatile uint32 tmp_tick1;
    volatile uint32 tmp_tick2;

    tmp_tick1 = SYSTEM_CURRENT_CLOCK;
    tmp_tick2 = SYSTEM_CURRENT_CLOCK;

    while (tmp_tick1 != tmp_tick2)
    {
        tmp_tick1 = tmp_tick2;
        tmp_tick2 = SYSTEM_CURRENT_CLOCK;
    }

    return tmp_tick1;
}

#define DELAY_CHECK_INT_INTERVAL (100) /*us*/

LOCAL uint32 Intel_RS_Delay(uint32 delay_us)
{
   uint32 delay_count = 0;
   
   while(1)
   {
        FLASH_DELAY_US(DELAY_CHECK_INT_INTERVAL);

        if((GET_INTERRUPT_FLAG & (1<<TB_DSP_INT)) != 0)
        {
            break;
        }
        if((delay_count * DELAY_CHECK_INT_INTERVAL) > delay_us)
        {
            break;
        }     
        delay_count++;
    }

   return SCI_TRUE;
}

// check the optr status.
//
// 0 : fail,  1 : success
// When call this function, the caller has call the DisableIRQ, 
// So this function needn't call the DisableIRQ
//
// Notes:
//     Before call this function, make sure the irq has been disabled.
//     This function need be located in SRAM or other space.
//     This function mustn't call any function( or use any data ) that located in the flash area.
//
LOCAL int INTEL_CheckStatus(FLASH_PTR_TYPE addr_ptr, uint  max_tick)
{	    
    int              result = 0;
    uint32           suspend_time;
    volatile uint32  old_tick, cur_tick, start_tick, last_resume_tick, i, bloop;
    volatile uint32   cur_timer1_tick;
    uint32 resume_to_suspend_latency, resume_to_suspend_cnt; 

    
    ENABLE_SYSTEM_TICK_COUNT;
    
    resume_to_suspend_latency = 100;  //us
    resume_to_suspend_cnt = resume_to_suspend_latency / TIMER1_TICK_COUNT;
    start_tick        = _get_current_system_clock();
  
    //@lin.liu. CR30772. should init the value
    cur_tick          = start_tick; 
    suspend_time      = 0;
    last_resume_tick = TIMER1_INT_COUNT;
    bloop = 1;
    
    while( (cur_tick - start_tick) < (max_tick + suspend_time) ) 
    {
        result = INTEL_GetPollStatus( addr_ptr );
        switch( result )
        {
            case FLASH_OPTR_STATUS_COMPLETED:
                result = 1;
                bloop = 0;
            break;
            case FLASH_OPTR_STATUS_RUNNING:	                             
            break;
            case FLASH_OPTR_STATUS_FAILED:
            default:
                result = 0;
                bloop = 0;
            break;
        }
        if(0 == bloop)
        {
            break;  // jump out of the "while" cycle
        }
        cur_tick = _get_current_system_clock();
            
        if( b_need_monitor_int && GET_INTERRUPT_FLAG )
        {
            // One or more interrupt need to be handled, 
            // suspend the current optr
            if(last_resume_tick == TIMER1_INT_COUNT)
            {
                __time1_init();
            }

            // One or more interrupt need to be handled, 
            // suspend the current optr
            cur_timer1_tick = __time1_get_value();
            if(((last_resume_tick - cur_timer1_tick) < resume_to_suspend_cnt) && ((last_resume_tick >= cur_timer1_tick)))
            {
                Intel_RS_Delay(TIMER1_TICK_COUNT * (resume_to_suspend_cnt - (last_resume_tick - cur_timer1_tick)));
            }
            
            old_tick = _get_current_system_clock();     
	        if(FLASH_GetSuspendFlag())           
	        {
                INTEL_Suspend();
	        }
            old_tick = _get_current_system_clock();
	            
            // now enable the interrupt to handle it
            // then disable again.
            FLASH_RESTORE_IRQ;	            
            
             // make sure the interrupt can be handled.
            for( i = 0; i < 10; i++ );
            
            FLASH_DISABLE_IRQ;
            
            if(FLASH_GetSuspendFlag())    
            {       
                INTEL_RESUME_CYCLE( s_flash_erasing_addr );
	            STORE_INTEL_OPTR(FLASH_STORE_OPTR_ERASING);
	        }    
            
            cur_tick   = _get_current_system_clock();            
            last_resume_tick = __time1_get_value();            
            suspend_time += (cur_tick - old_tick + 1);
                
        }        
    } // end while

    return result;
}

//
// enter_read/exit_read should be called with pair
//
PUBLIC void INTEL_EnterRead( void )
{
    uint32  status;
	status = GET_INTEL_STORE_OPTR;
	
	if((FLASH_STORE_OPTR_PROGRAMING == status) 
    || (FLASH_STORE_OPTR_ERASING    == status))
    {
        // the flash being erase, so suspend first
        INTEL_Suspend();
    }
}
PUBLIC void INTEL_ExitRead( void )
{
    uint32  status;
    
	status = GET_INTEL_STORE_OPTR;
	
    if(FLASH_STORE_OPTR_SUSPEND == status)
    {
        // the flash being suspended, so resume it.
        INTEL_Resume();
    }
}

/*****************************************************************************/
// 	Description :
//      Read word(two byte) from the given address.
//
//	Global resource dependence :
//      None
//  Author:
//      Lin.liu
//	Note:
//      the address MUST be word boundary.
/*****************************************************************************/
LOCAL uint16 INTEL_ReadWord(  // The word that has read from the given address.
    uint32 addr)        // the address that will be read.
{    
    uint16   read_data; // the data that be read.
    uint32   temp_addr=0;
    // Word boundary.
    SCI_ASSERT( !(addr & 1) );/*assert verified*/

    temp_addr = addr;
    temp_addr = INTEL_EfsAddrConvert(addr);
    
    // @lin.liu(2003-12-04). CR: MS5446.
    INTEL_EnterRead();

    read_data = *((FLASH_PTR_TYPE)temp_addr);

    INTEL_ExitRead();
    // @End. CR: 5446
    
    return read_data;
}



/*****************************************************************************/
// 	Description :
//      The function reads up to size bytes from the device and stores them in buffer.
//
//	Global resource dependence :
//      None
//  Author:
//      Lin.liu
//	Note:
//      
/*****************************************************************************/
LOCAL BOOLEAN INTEL_Read(      // If read successfully, return true, else false;
    uint32    addr,      // the start address that will be read
    uint8   * buf,       // store the data that being read to this buffer.
    uint32    read_len)  // the count of bytes that will be read.
{
    FLASH_PTR_TYPE     addr_ptr = (FLASH_PTR_TYPE)(addr & 0xFFFFFFFE); // word boundary.
    FLASH_PTR_TYPE     temp_addr_ptr =NULL;
  //  uint32 temp_addr;

    if( 0 == read_len )
    {
        return TRUE;    
    }
    
    // @lin.liu(2003-12-04). CR: MS5446.
    INTEL_EnterRead();
    // End.

    if( addr & 1 )     
    {
        /* 
        Not word boundary, so need read the first byte
        */
        temp_addr_ptr =  (FLASH_PTR_TYPE)INTEL_EfsAddrConvert((uint32)addr_ptr );
        *buf++ = SECOND_BYTE( *temp_addr_ptr );
        addr++;
        read_len--;        
    }

    addr_ptr = (FLASH_PTR_TYPE)(addr);
    while( read_len > 1 )
    {
    	temp_addr_ptr =  (FLASH_PTR_TYPE)INTEL_EfsAddrConvert((uint32)addr_ptr);
		*buf++ = FIRST_BYTE( *temp_addr_ptr );
		*buf++ = SECOND_BYTE(*temp_addr_ptr );
		addr_ptr++;
	
	    //@lin.liu(2003-12-16). CR: 6250
		if( 0 == (read_len & 0x1FF) )
		{
		    INTEL_ExitRead();
		    // Enable Handle INT.
		    INTEL_EnterRead();
		}		
		// End 6250
       	 read_len   -=2;
    }
    
    if( read_len )
    {
        /*
            Read the last byte.
        */
         temp_addr_ptr =  (FLASH_PTR_TYPE)INTEL_EfsAddrConvert((uint32)addr_ptr);
        *buf++ = FIRST_BYTE( *temp_addr_ptr );
    }
    
    INTEL_ExitRead();
    /*
        All Has Done.
    */
    return TRUE;
}


/*****************************************************************************/
// 	Description :
//      The function unlock the sector that contain the address.
//
//	Global resource dependence :
//      None
//  Author:
//      Lin.liu
//	Note:
//      
/*****************************************************************************/
LOCAL void INTEL_Unlock(
    uint32   addr)  // the sector that contain the address will be unlock.
{ 
}

/*****************************************************************************/
// 	Description :
//      The function lock down the sector that contain the address.
//
//	Global resource dependence :
//      None
//  Author:
//      Lin.liu
//	Note:
//      
/*****************************************************************************/
LOCAL void INTEL_Lock(uint32  addr )
{
}


/*****************************************************************************/
// 	Description :
//      The function write word to flash.
//
//	Global resource dependence :
//      None
//  Author:
//      Lin.liu
//	Note:
//      The address MUST be word boundary.
//
//      We not verify the data that written.
//      If some bit can't convert from 0 to 1, the data that has written 
//      is not correct.
/*****************************************************************************/
LOCAL BOOLEAN  INTEL_FlashWriteWord(
    uint32      addr,
    uint16      data)
{
    FLASH_PTR_TYPE          addr_ptr ; 
    volatile uint16         tmp;
//	BOOLEAN iret;

  //  uint32 start_tick;
   // uint32 cur_tick;
  //  uint32 consume_tick;

    if(addr & M18_MODE_DIFF_ADDR)
    {
        return FALSE;
    }
    
	addr_ptr = (FLASH_PTR_TYPE)addr;

	SCI_ASSERT( !(addr & 1));/*assert verified*/

	tmp = *addr_ptr;

   	if( tmp == data )
   	{
       	// It is same, needn't write
       	return TRUE;        
   	}
   	else if( (tmp & data) != data )
   	{
       	// some bits from 0 to 1, can't write
   	 	return FALSE;
   	}
		
	FLASH_DISABLE_IRQ; // disable the irq and save the prev state;
    // For the program only need 700us(max),
    // needn't monitor the interrupt status.

    b_need_monitor_int = 0;

    FLASH_WR(addr_ptr,TMPL_CONFIG_SETUP);//0x60 //Lock Setup
    FLASH_WR(addr_ptr,TMPL_CONFIRM);    //0xD0 (Block Unlock)

    FLASH_WR(addr_ptr,0x0050);//Clear Status Register
    
    FLASH_WR(addr_ptr,TMPL_M18_PROGRAM_SETUP);//Program Setup

    // PA = PD
    FLASH_WR(addr_ptr,data);
    
    STORE_INTEL_OPTR( FLASH_STORE_OPTR_PROGRAMING )

    // polling until complete.
    INTEL_CheckStatus( addr_ptr, FLASH_MAX_PROGRAM_TIME );
	// Reset it 
	
    INTEL_RESET_CYCLE( addr_ptr );
        
    STORE_INTEL_OPTR( FLASH_STORE_OPTR_IDLE )
        
    FLASH_RESTORE_IRQ;

    tmp = FLASH_RD(addr_ptr);

    return (BOOLEAN)( (tmp == data) );

}


LOCAL BOOLEAN  INTEL_WriteWord( 
    uint32    addr,
    uint16    data)
{
    uint32  loop,tmp_addr;
    uint16  content;

    for( loop = 0; loop < 3; loop++ )
    {
       tmp_addr = INTEL_EfsAddrConvert(addr);
        
       if( INTEL_FlashWriteWord( tmp_addr, data ) )
            return TRUE;
        
        // failed. If can write to the address directly, try it again.
        content = FLASH_RD(tmp_addr);
        if( data != (data & content) )
        {
            // Can't convert from 0 to 1.
            break;
        }
    }
    
    return FALSE;
}

/*****************************************************************************/
// 	Description :
//      The function write data to flash.
//
//	Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//	Note:
//
//      We not verify the data that written.
//      If some bit can't convert from 0 to 1, the data that has written 
//      is not correct.
/*****************************************************************************/
LOCAL BOOLEAN INTEL_Write(    // If the data being written to flash, return TRUE.
    uint32       addr,  // the start address that will be written.
    const uint8      * buf,   // Pointer to the buffer containing the data to be written.
    uint32       len)   // number of bytes to write.

{
	int 	ret;
	FLASH_PTR_TYPE    	addr_ptr,tmp_addr_ptr;    
//	uint32      sector_size; 
	//uint32      temp_addr = 0;
//	uint8*      data_ptr = NULL;
	uint16  	 tmp, data;  
	//uint16      loop_count = 0;
//	uint32      data_num;
 //   uint16*     testbuf;
    
	if( 0 == len )
		return 1;

	if(((addr>=s_fixed_nv_start_addr) && (addr< s_fixed_nv_start_addr+s_flash_sector_size))
			||((addr>=s_product_info_addr) && (addr<s_product_info_addr +s_flash_sector_size )))
	{

	        if(((addr&(M18_REGION_SIZE-1)) > 0) || ((len &(M18_REGION_SIZE -1)) > 0))
	        {
	        	//SCI_TRACE_LOW:"INTEL_Write:Error Address or Length!"
	        	SCI_TRACE_ID(TRACE_TOOL_CONVERT,INTEL_M18_DRV_706_112_2_18_0_34_32_1973,(uint8*)"");
	        	return FALSE;      
	        }  	

	        while(len>=M18_REGION_SIZE)
	    	{  
	            INTEL_WriteBuffer(addr, buf, M18_REGION_SIZE);
	            
	            len -= M18_REGION_SIZE;
	            addr+= M18_REGION_SIZE;
	            buf += M18_REGION_SIZE;
	    	}

	        return TRUE;
        
	}

	else if((addr >=s_efs_start_addr) && (addr < s_efs_start_addr + s_flash_sector_num*s_flash_sector_size))
	{
		// maybe need write first byte especially
		if( addr & 0x1 )
		{
			addr_ptr = (FLASH_PTR_TYPE)(addr & 0xFFFFFFFE);
			tmp_addr_ptr =  (FLASH_PTR_TYPE)INTEL_EfsAddrConvert((uint32)addr_ptr );
			tmp      = FIRST_BYTE( *tmp_addr_ptr );

			data = MAKE_WORD( tmp, *buf );
			buf++;
			/*addr convert contained in this function*/
			ret = INTEL_WriteWord( addr  & 0xFFFFFFFE, data );
			if( !ret )
			{
				return 0;
			}

			addr++;
			len--;
		}

		while( len > 1 )
		{
			// write word by word		    
			tmp  = (uint16)(*buf++);
			data = MAKE_WORD( tmp, *buf);
			buf++;
			if( INTEL_WriteWord( addr , data ) )
			{
				len	      -= 2;
				addr        += 2;		
			}
			else
			{
				return 0;
			}
		}

		// maybe need write last one byte
		if( len )
		{
			addr_ptr = (FLASH_PTR_TYPE)(addr & 0xFFFFFFFE);
			tmp_addr_ptr =  (FLASH_PTR_TYPE)INTEL_EfsAddrConvert((uint32)addr_ptr );
			tmp      = SECOND_BYTE( *tmp_addr_ptr );
			data     = MAKE_WORD( *buf, tmp );

			ret = INTEL_WriteWord(addr & 0xFFFFFFFE, data );
			if( ret )  len--;
		}
	}

	else
	{
		//SCI_ASSERT(0);
		//SCI_TRACE_LOW:"[DRV_INTEL_M18_DRV]<WARNING> addr is invalid\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,INTEL_M18_DRV_778_112_2_18_0_34_32_1974,(uint8*)"" );
	}


	return (0 == len ); 
}



/*****************************************************************************/
// 	Description :
//      Is the last erase optr completed?
//
//	Global resource dependence :
//      None
//
//	Return :
//      If the erase has been completely, return true, other false
//  Author:
//      Lin.liu
//  Note:    
//      This function MUST be called when system assert only,
/*****************************************************************************/
LOCAL BOOLEAN INTEL_IsEraseCompleted(void)
{
    uint32  status;
	status = GET_INTEL_STORE_OPTR;
	
	if((FLASH_STORE_OPTR_PROGRAMING == status) 
    || (FLASH_STORE_OPTR_ERASING    == status)
    || (FLASH_STORE_OPTR_SUSPEND    == status))
	{
	    return 0;
	}
	// Other case, we think has been completed.
	return 1;	
}
/*****************************************************************************/
// 	Description :
//      Erase the sector that contain the address.
//
//	Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//	Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//      
/*****************************************************************************/
LOCAL BOOLEAN INTEL_Erase(
    uint32 addr)
{
    FLASH_PTR_TYPE      addr_ptr;
    int   ret;
    uint32 i=0;

    if((addr >=s_efs_start_addr) && (addr < s_efs_start_addr + s_flash_sector_num*s_flash_sector_size))
    {
    	   for(i=0;i<0x2;i++)
    	   {
    	   	    if(i == 0x1)
    	   	    {
    	           	addr = addr+s_flash_sector_size*s_flash_sector_num;
    	   	    }
		    /*
		    only make sure the address is even.
		    */
		    addr_ptr = (FLASH_PTR_TYPE)(addr & 0xFFFFFFFE);
		    
		    s_flash_erasing_addr = addr_ptr;

		    FLASH_DISABLE_IRQ;// disable the irq and save the prev state;

		    // If the irq/fiq has been disabled before call this function, 
		    // needn't monitor the interrupt status.
		    b_need_monitor_int = (s_flash_interrupt_save ^ FLASH_IRQ_FIQ_DISABLE);

		    FLASH_WR(addr_ptr,TMPL_CLEAR_STATUS_REGISTER);
		    FLASH_WR(addr_ptr,TMPL_READ_ARRAY);
		    FLASH_WR(addr_ptr,TMPL_CONFIG_SETUP);//0x60 //Lock Setup
		    FLASH_WR(addr_ptr,TMPL_CONFIRM);//0xD0 (Block Unlock)

            FLASH_WR(addr_ptr,0x0050);//Clear Status Register
		    FLASH_WR(addr_ptr,TMPL_BLOCK_ERASE);//0x20 Block Erase Setup
		    FLASH_WR(addr_ptr,TMPL_CONFIRM);//0xD0 Erase Confirm
		    
		    STORE_INTEL_OPTR( FLASH_STORE_OPTR_ERASING)

		    ret = INTEL_CheckStatus( addr_ptr, FLASH_MAX_ERASE_TIME );        
		    
		    FLASH_WR(addr_ptr,TMPL_CLEAR_STATUS_REGISTER);
		    INTEL_RESET_CYCLE( addr_ptr );

		    STORE_INTEL_OPTR( FLASH_STORE_OPTR_IDLE )

		    FLASH_RESTORE_IRQ;

		    if(ret == 0 )
				break;
    	   }
     }
    else
    {
	    /*
	    only make sure the address is even.
	    */
	    addr_ptr = (FLASH_PTR_TYPE)(addr & 0xFFFFFFFE);
	    
	    s_flash_erasing_addr = addr_ptr;

	    FLASH_DISABLE_IRQ;// disable the irq and save the prev state;

	    // If the irq/fiq has been disabled before call this function, 
	    // needn't monitor the interrupt status.
	    b_need_monitor_int = (s_flash_interrupt_save ^ FLASH_IRQ_FIQ_DISABLE);

	    FLASH_WR(addr_ptr,TMPL_CLEAR_STATUS_REGISTER);
	    FLASH_WR(addr_ptr,TMPL_READ_ARRAY);
	    FLASH_WR(addr_ptr,TMPL_CONFIG_SETUP);//0x60 //Lock Setup
	    FLASH_WR(addr_ptr,TMPL_CONFIRM);//0xD0 (Block Unlock)
	    FLASH_WR(addr_ptr,TMPL_BLOCK_ERASE);//0x20 Block Erase Setup
	    FLASH_WR(addr_ptr,TMPL_CONFIRM);//0xD0 Erase Confirm
	    
	    STORE_INTEL_OPTR( FLASH_STORE_OPTR_ERASING )

	    ret = INTEL_CheckStatus( addr_ptr, FLASH_MAX_ERASE_TIME );        
	    
	    FLASH_WR(addr_ptr,TMPL_CLEAR_STATUS_REGISTER);
	    INTEL_RESET_CYCLE( addr_ptr );

	    STORE_INTEL_OPTR( FLASH_STORE_OPTR_IDLE )

	    FLASH_RESTORE_IRQ;
	}

    return (BOOLEAN)ret;

}


/*****************************************************************************/
// 	Description :
//      Suspend the program/erase operation
//
//	Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//	Note:
//      
/*****************************************************************************/
LOCAL void INTEL_Suspend(void)
{    
    uint32  status;
	status = GET_INTEL_STORE_OPTR;
	
	if((FLASH_STORE_OPTR_PROGRAMING == status) 
    || (FLASH_STORE_OPTR_ERASING    == status))
    {
        INTEL_SUSPEND_CYCLE(s_flash_erasing_addr);
        STORE_INTEL_OPTR(FLASH_STORE_OPTR_SUSPEND);
        INTEL_RESET_CYCLE(s_flash_erasing_addr);  
    }
}


/*****************************************************************************/
// 	Description :
//      Resume the program/erase operation
//      In some case, system will restart( not power on ) while erasing/programing.
//      so, flash manager need detect this casae and  recover/continue the last optr
//	Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//	Note:
//      
/*****************************************************************************/
LOCAL void INTEL_Resume(void)
{
    
    if(FLASH_STORE_OPTR_SUSPEND == GET_INTEL_STORE_OPTR)
    {

        FLASH_DISABLE_IRQ;// disable the irq and save the prev state;

        // If the irq/fiq has been disabled before call this function, 
        // needn't monitor the interrupt status.
        b_need_monitor_int = (s_flash_interrupt_save ^ FLASH_IRQ_FIQ_DISABLE);

        // resume and wait
        INTEL_RESUME_CYCLE( s_flash_erasing_addr );
        
        STORE_INTEL_OPTR( FLASH_STORE_OPTR_ERASING )

        INTEL_CheckStatus( s_flash_erasing_addr, FLASH_MAX_ERASE_TIME );        

        INTEL_RESET_CYCLE(s_flash_erasing_addr);

        STORE_INTEL_OPTR( FLASH_STORE_OPTR_IDLE )

        FLASH_RESTORE_IRQ;
        
    }
}

/*****************************************************************************/
// Description :    buffer write function
// Global resource dependence :
// s_flash_sector_size
// Author :         Younger.yang
// Parameter:
// Note :           
/*****************************************************************************/
LOCAL BOOLEAN INTEL_WriteBuffer(    // If the data being written to flash, return TRUE.
    uint32       addr,              // the start address that will be written.
    const uint8  * buf,             // Pointer to the buffer containing the data to be written.
    uint32       counter)           // number of byte to write.
{

    FLASH_PTR_TYPE    	addr_ptr;    

    uint16  			tmp, data, ret;    
    uint16              wc;
    uint32              dest_addr;

    SCI_ASSERT(!(addr    % 2));/*assert verified*/
    SCI_ASSERT(!(counter % 2));/*assert verified*/

	dest_addr = addr;
    
    FLASH_DISABLE_IRQ; // disable the irq and save the prev state;
    // For the program only need 700us(max),
    // needn't monitor the interrupt status.
    b_need_monitor_int = 0;

    wc = counter / 2;
    addr_ptr = (uint16 *)addr; 
    
    FLASH_WR(dest_addr,LOCK_BLOCK_SETUP);
    FLASH_WR(dest_addr,UNLOCK_BLOCK);

    // Buffer Setup command
    do 
    {
        FLASH_WR(dest_addr,M18_BUFFER_PROGRAM_SETUP);
    } while (( *((volatile uint16 *)(dest_addr)) & DEVICE_READY) != DEVICE_READY );


    // Issue buffer write length
    FLASH_WR(dest_addr,(counter/2 - 1));

    
    do{	
        tmp       = (uint16)(*buf++);
        data      = MAKE_WORD( tmp, *buf);
        buf++;
        addr_ptr  = (uint16 *)addr;

        *addr_ptr = data;
        addr     += 2;
        wc--;
    }while(wc > 0);
    
    FLASH_WR(dest_addr,BUFFERED_PROGRAM_CONFIRM);

    STORE_INTEL_OPTR( FLASH_STORE_OPTR_PROGRAMING )
 
    // polling until complete.
    ret = INTEL_CheckStatus( addr_ptr, FLASH_MAX_PROGRAM_TIME );
    // Delay 100ms
    // Re-lock the device
    FLASH_WR(dest_addr,LOCK_BLOCK_SETUP);
    FLASH_WR(dest_addr,LOCK_BLOCK);
    INTEL_RESET_CYCLE(dest_addr); 

    STORE_INTEL_OPTR( FLASH_STORE_OPTR_IDLE )
    
    FLASH_RESTORE_IRQ;

    return ret;

}

LOCAL void INTEL_Config(NOR_FLASH_CONFIG_PTR cfg_ptr,NORMCP_SPEC_PTR spec_ptr)
{
    //s_flash_bank_num             = cfg_ptr->bank_num; 
    s_flash_sector_num           = cfg_ptr->sect_num;
    s_flash_sector_size          = cfg_ptr->sect_size;
    //s_flash_flash_size           = cfg_ptr->flash_size;
    s_efs_start_addr			 = cfg_ptr->efs_start_addr;
    s_fixed_nv_start_addr        = cfg_ptr->fixnv_addr;
    s_product_info_addr          = cfg_ptr->prodinfo_addr;
    
    //s_flash_wb_len               = spec_ptr->flash_write_buf_len;
    //s_flash_wb_flag              = (spec_ptr->flash_write_mode & FLASH_WRITE_BUFFER_MODE) ?  FLASH_WRITE_BUFFER_MODE : FLASH_WRITE_SINGLE_MODE;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

/*      End flash.c       */
